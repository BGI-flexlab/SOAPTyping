#include "exontimdlg.h"
#include "ui_exontimdlg.h"
#include "DataBase/soaptypingdb.h"

ExonTimDlg::ExonTimDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExonTimDlg)
{
    ui->setupUi(this);
    InitUI();
    ConnectSignalandSlot();
    setGeneBoxData();
}

ExonTimDlg::~ExonTimDlg()
{
    delete ui;
}

void ExonTimDlg::InitUI()
{
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QStringList theader;
    theader<<"Gene"<<"Exon"<<"R/F"<<"Default Start"<<"Default End"<<"Exclude Left Num"<<"Exclude Right Num";
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(theader);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 70);
    ui->tableWidget->setColumnWidth(1, 50);
    ui->tableWidget->setColumnWidth(2, 40);
    ui->tableWidget->setColumnWidth(3, 100);
    ui->tableWidget->setColumnWidth(4, 95);
    ui->tableWidget->setColumnWidth(5, 130);
    ui->tableWidget->setColumnWidth(6, 100);
}

void ExonTimDlg::ConnectSignalandSlot()
{
    connect(ui->btnReset, &QPushButton::clicked, this, &ExonTimDlg::slotClickResetButton);
    connect(ui->btnApply, &QPushButton::clicked, this, &ExonTimDlg::slotClickApplyButton);
    connect(ui->btnExit, &QPushButton::clicked, this, &ExonTimDlg::close);
    connect(ui->comboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
            this, &ExonTimDlg::slotGeneBoxIndexChanged);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &ExonTimDlg::slotClickTableItem);
}

void ExonTimDlg::slotClickResetButton()
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->item(i, 5)->setText("0");
        ui->tableWidget->item(i, 6)->setText("0");
    }
}

void ExonTimDlg::slotClickApplyButton()
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        ExonTrimTable etTable;
        etTable.geneName=ui->tableWidget->item(i, 0)->text();
        etTable.exonIndex=ui->tableWidget->item(i, 1)->text();
        etTable.fOrR=ui->tableWidget->item(i, 2)->text();
        etTable.exonStart=ui->tableWidget->item(i, 3)->text();
        etTable.exonEnd=ui->tableWidget->item(i, 4)->text();
        etTable.excludeLeft=ui->tableWidget->item(i, 5)->text();
        etTable.excludeRight=ui->tableWidget->item(i, 6)->text();
        etTable.etKey=QString("%1_%2_%3").arg(etTable.geneName).arg(etTable.exonIndex).arg(etTable.fOrR);
        SoapTypingDB::GetInstance()->updateExonTrim(etTable);
    }
}

void ExonTimDlg::slotGeneBoxIndexChanged(const QString &genename)
{
    QVector<ExonTrimTable> etList;
    SoapTypingDB::GetInstance()->getExonTrimListByGeneName(genename, etList);
    setTableData(etList);
}

void ExonTimDlg::slotClickTableItem()
{
    int c = ui->tableWidget->currentColumn();
    if (c<=4)
    {
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    else
    {
        ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    }
}

void ExonTimDlg::setGeneBoxData()
{
    QStringList geneNames;
    SoapTypingDB::GetInstance()->getGeneNames(geneNames);
    ui->comboBox->addItems(geneNames);
    ui->comboBox->setCurrentIndex(0);
}

void ExonTimDlg::setTableData(QVector<ExonTrimTable> &etList)
{
    ui->tableWidget->clear();
//    QStringList theader;
//    theader<<"Gene"<<"Exon"<<"R/F"<<"Default Start"<<"Default End"<<"Exclude Left Num"<<"Exclude Right Num";
//    table->setHorizontalHeaderLabels(theader);

    int size = etList.size();
    if(size<0)
        return;
    ui->tableWidget->setRowCount(size);
    for(int i=0; i<size; i++)
    {
        ui->tableWidget->setRowHeight(i, 18);
        const ExonTrimTable &etTable = etList.at(i);
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(etTable.geneName);
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem;
        item->setText(etTable.exonIndex);
        ui->tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem;
        item->setText(etTable.fOrR);
        ui->tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem;
        item->setText(etTable.exonStart);
        ui->tableWidget->setItem(i, 3, item);

        item = new QTableWidgetItem;
        item->setText(etTable.exonEnd);
        ui->tableWidget->setItem(i, 4, item);

        item = new QTableWidgetItem;
        item->setText(etTable.excludeLeft);
        ui->tableWidget->setItem(i, 5, item);

        item = new QTableWidgetItem;
        item->setText(etTable.excludeRight);
        ui->tableWidget->setItem(i, 6, item);
    }
}
