#include "openfiledialog.h"
#include "ui_openfiledialog.h"
#include <QFileDialog>
#include <QThreadPool>
#include <QDebug>
#include "DataBase/soaptypingdb.h"
#include "Core/Ab1.h"
#include "ThreadTask/analysisab1threadtask.h"
#include "ThreadTask/analysissamplethreadtask.h"
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

    //ui->btnAnalysis->setEnabled(false);
    //ui->btnDel->setEnabled(false);
}

void OpenFileDialog::InitData()
{
    m_geneNames_List<<"A"<<"B"<<"C"<<"DPA1"<<"DPB1"<<"DQA1"<<"DQB1"<<"DRB1"<<"DRB3"<<"DRB4"<<"DRB5"<<"G";
    m_exonNames_List<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
    m_ROrFNames_List<<"F"<<"R";
    SoapTypingDB::GetInstance()->GetGsspNames(m_gsspNames_List);
    SoapTypingDB::GetInstance()->GetGsspMapToExonAndFR(m_map_ExonAndRF);
}

void OpenFileDialog::ConnectSignalandSlot()
{
    connect(ui->btnOpen, &QPushButton::clicked, this, &OpenFileDialog::SlotOpenFile);
    connect(ui->btnAnalysis, &QPushButton::clicked, this, &OpenFileDialog::SlotAnalysisFile);
    connect(ui->btnCancel, &QPushButton::clicked, this, &OpenFileDialog::close);
    connect(ui->checkBox,&QCheckBox::stateChanged, this, &OpenFileDialog::SlotCheckAll);
    connect(ui->btnDel, &QPushButton::clicked, this, &OpenFileDialog::SlotDelSelect);
}

void OpenFileDialog::SlotOpenFile()
{
    QString strdir;
    Core::GetInstance()->GetConfig("Path/OpenDir", strdir);
    QStringList filePathList = QFileDialog::getOpenFileNames(this, tr("open files"),
                                                             strdir, "Peak Files(*.ab1;*abi)");

    if(!filePathList.empty())
    {
        Core::GetInstance()->SetConfig("Path/OpenDir", filePathList[0]);
        ui->progressBar->setRange(0, filePathList.size());
        FilePathListProcess(filePathList);
        ui->progressBar->reset();
        ui->btnAnalysis->setEnabled(true);
        int i_Row = ui->tableWidget->rowCount();
        int i_samplesize = m_set_sample.size();
        ui->btnStatus->setText(QString("F:%1/S:%2 Ready").arg(i_Row).arg(i_samplesize));

        //emit signalGetNumbers();
    }
}

bool SetAb1FileTable(QTableWidget *tablewidget,int i, Ab1FileTableBase &filetable)
{
    QString str_samplename = tablewidget->item(i, 0)->text();
    QComboBoxNew *pbox = static_cast<QComboBoxNew*>(tablewidget->cellWidget(i, 1));
    QString str_geneName = pbox->currentText();
    pbox = static_cast<QComboBoxNew*>(tablewidget->cellWidget(i, 3));
    int i_exonIndex = pbox->currentText().toInt();
    pbox = static_cast<QComboBoxNew*>(tablewidget->cellWidget(i, 4));
    char c_rOrF = pbox->currentText().at(0).toLatin1();
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
        QComboBoxNew *pbox = static_cast<QComboBoxNew*>(ui->tableWidget->cellWidget(i, 4));
        QString str_RF = pbox->currentText();
        if(str_RF.isEmpty()) //无法分析的文件，跳过
        {
            SetProcessbarValue();
            continue;
        }
        pbox = static_cast<QComboBoxNew*>(ui->tableWidget->cellWidget(i, 2));
        QString str_gsspname = pbox->currentText();
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

void OpenFileDialog::FilePathListProcess(const QStringList &filePathList)
{
    for(int i=0; i<filePathList.size(); i++)
    {
        FilePathProcess(filePathList.at(i));
        ui->progressBar->setValue(i+1);
        //QCoreApplication::processEvents();
    }
}

void OpenFileDialog::FilePathProcess(const QString &filePath)
{
    OpenFileTable openFileTable;
    bool isOK = AnalysisFileName(filePath, openFileTable);
    if(isOK)
    {
        AddRowToTableWidget(openFileTable);
    }
}

bool OpenFileDialog::AnalysisFileName(const QString &filePath, OpenFileTable &openFileTable)
{
    openFileTable.filePath  = filePath;
    QFileInfo fileInfo(filePath);
    openFileTable.fileName = fileInfo.fileName();
    if(!m_set_File.contains(openFileTable.fileName)) //文件集合不包含指定文件
    {
        QStringList part = openFileTable.fileName.split("_");
        if(part.size() != 4)
        {
            openFileTable.right = false;
        }
        else
        {
            openFileTable.sampleName = part.at(0);
            openFileTable.geneName = part.at(1);
            m_set_sample.insert(openFileTable.sampleName);
            bool isRight = AnalysisExonInfo(part.at(2), openFileTable);
            if(!isRight)
            {
                openFileTable.right = false;
            }
        }
        m_set_File.insert(openFileTable.fileName);
        return true;
    }
    else
    {
        return false;
    }
}

void OpenFileDialog::AddRowToTableWidget(const OpenFileTable &openFileTable)
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

    QComboBoxNew *box1 = new QComboBoxNew;
    box1->addItems(m_geneNames_List);
    SetComboBoxData(box1, openFileTable.geneName);
    ui->tableWidget->setCellWidget(iRow, 1, box1);

    QComboBoxNew *box2 = new QComboBoxNew;
    box2->addItems(m_gsspNames_List);
    SetComboBoxData(box2, openFileTable.gsspName);
    ui->tableWidget->setCellWidget(iRow, 2, box2);

    QComboBoxNew *box3 = new QComboBoxNew;
    box3->addItems(m_exonNames_List);
    SetComboBoxData(box3, openFileTable.exonIndex);
    ui->tableWidget->setCellWidget(iRow, 3, box3);

    QComboBoxNew *box4 = new QComboBoxNew;
    box4->addItems(m_ROrFNames_List);
    SetComboBoxData(box4, openFileTable.rOrF);
    ui->tableWidget->setCellWidget(iRow, 4, box4);
    ui->tableWidget->cellWidget(iRow, 4)->backgroundRole();

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

void OpenFileDialog::SetComboBoxData(QComboBoxNew *box, const QString &text)
{
    int index = box->findText(text);
    box->setCurrentIndex(index);
}

bool OpenFileDialog::AnalysisExonInfo(const QString &exonString, OpenFileTable &openFileTable)
{
    QChar RF = exonString.at(1);
    if((RF==QChar('R') || RF==QChar('F')) && exonString.size() == 2)
    {
        openFileTable.exonIndex = exonString.at(0);
        openFileTable.rOrF = exonString.at(1);
        openFileTable.gsspName = "";
        return true;
    }
    else
    {
        QMap<QString, ExonAndRF>::iterator it = m_map_ExonAndRF.find(exonString);
        if(it == m_map_ExonAndRF.end())
        {
            return SoapTypingDB::GetInstance()->FindExonAndRFByGsspName(exonString,
                                                     openFileTable.exonIndex,
                                                     openFileTable.rOrF);
        }
        else
        {
            openFileTable.gsspName = exonString;
            openFileTable.exonIndex = it.value().exonIndex;
            openFileTable.rOrF = it.value().rOrF;
            return true;
        }
    }
    return false;
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
    //emit signalGetNumbers();
}
