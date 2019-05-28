#include "openfiledialog.h"
#include "ui_openfiledialog.h"
#include <QFileDialog>
#include <QThreadPool>
#include <QDebug>
#include "DataBase/soaptypingdb.h"
#include "Core/Ab1.h"
#include "ThreadTask/analysisab1threadtask.h"
#include "ThreadTask/analysissamplethreadtask.h"
#include "ThreadTask/fileprocessthreadtask.h"
#include "Core/fileTablebase.h"
#include "Core/core.h"



OpenFileDialog::OpenFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenFileDialog)
{
    ui->setupUi(this);
    InitUi();
    InitData();
    ConnectSignalandSlot();
    m_iPrgvalue = 1;
    qRegisterMetaType<OpenFileTable>("OpenFileTable&");
}

OpenFileDialog::~OpenFileDialog()
{
    delete ui;
}

void OpenFileDialog::InitUi()
{
    QStringList header;
    header<<"Sample Name"<<"Gene"<<"GSSP"<<"Exon"<<"F/R"<<"File Name"<<"File Path"<<"";
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidget->setColumnWidth(0, 180);
    ui->tableWidget->setColumnWidth(1, 60);
    ui->tableWidget->setColumnWidth(2, 60);
    ui->tableWidget->setColumnWidth(3, 50);
    ui->tableWidget->setColumnWidth(4, 50);
    ui->tableWidget->setColumnWidth(5, 250);
    ui->tableWidget->setColumnWidth(6, 390);

    ui->btnAnalysis->setEnabled(false);
    ui->btnDel->setEnabled(false);

    m_geneNames_ptr = new QListWidget(this);
    m_gsspNames_ptr = new QListWidget(this);
    m_exonNames_ptr = new QListWidget(this);
    m_ROrFNames_ptr = new QListWidget(this);

    m_geneNames_ptr->setMaximumSize(60, 250);
    m_gsspNames_ptr->setMaximumSize(60, 250);
    m_exonNames_ptr->setMaximumSize(60, 250);
    m_ROrFNames_ptr->setMaximumSize(60, 250);

    m_geneNames_ptr->hide();
    m_gsspNames_ptr->hide();
    m_exonNames_ptr->hide();
    m_ROrFNames_ptr->hide();
}

void OpenFileDialog::InitData()
{
    m_geneNames_List<<""<<"A"<<"B"<<"C"<<"DPA1"<<"DPB1"<<"DQA1"<<"DQB1"<<"DRB1"<<"DRB3"<<"DRB4"<<"DRB5"<<"G";
    m_exonNames_List<<""<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
    m_ROrFNames_List<<""<<"F"<<"R";
    m_gsspNames_List<<"";
    SoapTypingDB::GetInstance()->GetGsspNames(m_gsspNames_List);
    //SoapTypingDB::GetInstance()->GetGsspMapToExonAndFR(m_map_ExonAndRF);

    m_geneNames_ptr->addItems(m_geneNames_List);
    m_gsspNames_ptr->addItems(m_gsspNames_List);
    m_exonNames_ptr->addItems(m_exonNames_List);
    m_ROrFNames_ptr->addItems(m_ROrFNames_List);
}

void OpenFileDialog::ConnectSignalandSlot()
{
    connect(ui->btnOpen, &QPushButton::clicked, this, &OpenFileDialog::SlotOpenFile);
    connect(ui->btnAnalysis, &QPushButton::clicked, this, &OpenFileDialog::SlotAnalysisFile);
    connect(ui->btnCancel, &QPushButton::clicked, this, &OpenFileDialog::close);
    connect(ui->checkBox,&QCheckBox::stateChanged, this, &OpenFileDialog::SlotCheckAll);
    connect(ui->btnDel, &QPushButton::clicked, this, &OpenFileDialog::SlotDelSelect);

    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &OpenFileDialog::showPopupList);
    connect(m_geneNames_ptr, &QListWidget::itemClicked, this, &OpenFileDialog::changetableitem);
    connect(m_gsspNames_ptr, &QListWidget::itemClicked, this, &OpenFileDialog::changetableitem);
    connect(m_exonNames_ptr, &QListWidget::itemClicked, this, &OpenFileDialog::changetableitem);
    connect(m_ROrFNames_ptr, &QListWidget::itemClicked, this, &OpenFileDialog::changetableitem);
}

void OpenFileDialog::SlotOpenFile()
{
    QString strdir;
    Core::GetInstance()->GetConfig("Path/OpenDir", strdir);
    QStringList filePathList = QFileDialog::getOpenFileNames(this, tr("open files"),
                                                             strdir, "Peak Files(*.ab1 *.*)");

    if(!filePathList.empty())
    {
        Core::GetInstance()->SetConfig("Path/OpenDir", filePathList[0]);
        m_iOpenfile = filePathList.size();
        ui->progressBar->setRange(0, m_iOpenfile);

        fileprocessthreadtask *task = new fileprocessthreadtask(filePathList);
        connect(task, &fileprocessthreadtask::processone, this, &OpenFileDialog::slotprocessone);
        QThreadPool::globalInstance()->start(task);
    }
}

bool SetAb1FileTable(QTableWidget *tablewidget,int i, Ab1FileTableBase &filetable)
{
    QString str_samplename = tablewidget->item(i, 0)->text();
    QString str_geneName = tablewidget->item(i,1)->text();
    int i_exonIndex = tablewidget->item(i, 3)->text().toInt();
    QChar c_rOrF = tablewidget->item(i, 4)->text().at(0);
    QString str_fileName = tablewidget->item(i, 5)->text();
    QString str_filePath = tablewidget->item(i, 6)->text();

    filetable.setSampleName(str_samplename);
    filetable.setGeneName(str_geneName);
    filetable.setExonIndex(i_exonIndex);
    filetable.setROrF(c_rOrF);
    filetable.setFileName(str_fileName);
    filetable.setFilePath(str_filePath);
	
	return true;
}

void OpenFileDialog::SlotAnalysisFile()
{
    ui->btnAnalysis->setEnabled(false);
    ui->checkBox->setEnabled(false);
    ui->btnDel->setEnabled(false);
    ui->btnOpen->setEnabled(false);
    ui->btnCancel->setEnabled(false);

    int i_Row = ui->tableWidget->rowCount();
    int i_samplesize = m_set_sample.size();
    ui->btnStatus->setText(QString("F:%1/S:%2 Wating...").arg(i_Row).arg(i_samplesize));

    ui->progressBar->setRange(0,i_Row+i_samplesize);
    for(int i=0; i<i_Row; i++)
    {
        QString str_RF = ui->tableWidget->item(i,4)->text();
        if(str_RF.isEmpty()) //无法分析的文件，跳过
        {
            SetProcessbarValue();
            continue;
        }

        QString str_gsspname = ui->tableWidget->item(i, 2)->text();
        if(str_gsspname.isEmpty()) //普通的ab1文件
        {
            Ab1FileTableBase *pAb1filetable = new Ab1NormalFileTable;
            SetAb1FileTable(ui->tableWidget,i,*pAb1filetable);
            pAb1filetable->setIsGssp(false);
            AnalysisAB1ThreadTask *task = new AnalysisAB1ThreadTask(pAb1filetable);
            connect(task, &AnalysisAB1ThreadTask::analysisfinished, this, &OpenFileDialog::SetProcessbarValue);
            QThreadPool::globalInstance()->start(task);
        }
        else //Gssp文件
        {
            Ab1FileTableBase *pgsspfiletable = new Ab1GsspFileTable;
            SetAb1FileTable(ui->tableWidget,i,*pgsspfiletable);
            pgsspfiletable->setIsGssp(true);
            pgsspfiletable->setGsspName(str_gsspname);
            AnalysisAB1ThreadTask *task = new AnalysisAB1ThreadTask(pgsspfiletable);
            connect(task, &AnalysisAB1ThreadTask::analysisfinished, this, &OpenFileDialog::SetProcessbarValue);
            QThreadPool::globalInstance()->start(task);
        }
    }
}

void OpenFileDialog::SetProcessbarValue()
{
    ui->progressBar->setValue(m_iPrgvalue);
    if (m_iPrgvalue == ui->tableWidget->rowCount())
    {
        foreach(QString str_sample, m_set_sample)
        {
            AnalysisSampleThreadTask *task = new AnalysisSampleThreadTask(str_sample);
            connect(task, &AnalysisSampleThreadTask::analysisfinished, this, &OpenFileDialog::SetProcessbarValue);
            QThreadPool::globalInstance()->start(task);
        }
    }
    else if (m_iPrgvalue == ui->tableWidget->rowCount()+m_set_sample.size())
    {
        close();
    }
    m_iPrgvalue++;
}

void OpenFileDialog::SlotCheckAll(int state)
{
    int i_Row = ui->tableWidget->rowCount();
    for(int i=0; i<i_Row; i++)
    {
        ui->tableWidget->item(i, 0)->setCheckState(Qt::CheckState(state));
    }
}

void OpenFileDialog::SlotDelSelect()
{
    int i_Row = ui->tableWidget->rowCount();
    for(int i = i_Row-1; i >= 0; i--)
    {
        Qt::CheckState st = ui->tableWidget->item(i, 0)->checkState();
        if(st == Qt::Checked)
        {
            QString str_filename = ui->tableWidget->item(i, 5)->text();
            m_set_File.remove(str_filename);
            ui->tableWidget->removeRow(i);
        }
    }
    i_Row = ui->tableWidget->rowCount();
    for(int i=0;i<i_Row;i++)
    {
        QString str_sample = ui->tableWidget->item(i, 0)->text();
        m_set_sample.insert(str_sample);
    }
    ui->btnStatus->setText(QString("F:%1/S:%2 Ready").arg(m_set_File.size()).arg(m_set_sample.size()));
}


void OpenFileDialog::AddRowToTableWidget_s(const OpenFileTable &openFileTable)
{
    int iRow = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(iRow+1);
    ui->tableWidget->setRowHeight(iRow, 18);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setCheckState(Qt::Unchecked);
    item0->setText(openFileTable.sampleName);
    if(!openFileTable.right)
    {
        item0->setBackgroundColor(QColor(230,230,230));
    }
    ui->tableWidget->setItem(iRow, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(openFileTable.geneName);
    ui->tableWidget->setItem(iRow, 1, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText(openFileTable.gsspName);
    ui->tableWidget->setItem(iRow, 2, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem;
    item3->setText(openFileTable.exonIndex);
    ui->tableWidget->setItem(iRow, 3, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem;
    item4->setText(openFileTable.rOrF);
    ui->tableWidget->setItem(iRow, 4, item4);

    QTableWidgetItem *item5 = new QTableWidgetItem;
    item5->setText(openFileTable.fileName);
    if(!openFileTable.right)
    {
        item5->setBackgroundColor(QColor(230,230,230));
    }
    ui->tableWidget->setItem(iRow, 5, item5);

    QTableWidgetItem *item6 = new QTableWidgetItem;
    item6->setText(openFileTable.filePath);
    if(!openFileTable.right)
    {
        item6->setBackgroundColor(QColor(230,230,230));
    }
    ui->tableWidget->setItem(iRow, 6, item6);
}


void OpenFileDialog::slotprocessone(const OpenFileTable &info)
{
    ui->progressBar->setValue(m_iPrgvalue);

    if(!m_set_File.contains(info.fileName))
    {
        m_set_File.insert(info.fileName);
        AddRowToTableWidget_s(info);
    }
    if(m_iPrgvalue == m_iOpenfile)
    {
        m_iPrgvalue = 1;
        int i_Row = ui->tableWidget->rowCount();
        for(int i=0;i<i_Row;i++)
        {
            QString str_sample = ui->tableWidget->item(i, 0)->text();
            m_set_sample.insert(str_sample);
        }
        ui->progressBar->reset();
        ui->btnAnalysis->setEnabled(true);
        ui->btnDel->setEnabled(true);
        ui->btnStatus->setText(QString("F:%1/S:%2 Ready").arg(i_Row).arg(m_set_sample.size()));
    }
    else
    {
       m_iPrgvalue++;
    }
}

void OpenFileDialog::showPopupList(int row, int column)
{
    m_geneNames_ptr->hide();
    m_gsspNames_ptr->hide();
    m_exonNames_ptr->hide();
    m_ROrFNames_ptr->hide();

    int x = ui->tableWidget->columnViewportPosition(column);
    int y = ui->tableWidget->rowViewportPosition(row)+30;
    m_tableitem_ptr = ui->tableWidget->item(row,column);
    QPoint pos(x,y);
    switch (column) {
    case 1:
    {
        m_geneNames_ptr->move(pos);
        m_geneNames_ptr->show();
        break;
    }
    case 2:
    {
        m_gsspNames_ptr->move(pos);
        m_gsspNames_ptr->show();
        break;
    }
    case 3:
    {
        m_exonNames_ptr->move(pos);
        m_exonNames_ptr->show();
        break;
    }
    case 4:
    {
        m_ROrFNames_ptr->move(pos);
        m_ROrFNames_ptr->show();
        break;
    }
    }
}

void OpenFileDialog::changetableitem(QListWidgetItem *item)
{
    m_tableitem_ptr->setText(item->text());
    int column = m_tableitem_ptr->column();
    switch (column) {
    case 1:
        m_geneNames_ptr->hide();
        break;
    case 2:
        m_gsspNames_ptr->hide();
        break;
    case 3:
        m_exonNames_ptr->hide();
        break;
    case 4:
        m_ROrFNames_ptr->hide();
        break;
    }
}
