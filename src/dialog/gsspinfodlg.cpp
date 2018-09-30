#include "gsspinfodlg.h"
#include "ui_gsspinfodlg.h"
#include "DataBase/soaptypingdb.h"

GsspInfoDlg::GsspInfoDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GsspInfoDlg)
{
    ui->setupUi(this);
    InitUI();
    ConnectSignalandSlot();
}

GsspInfoDlg::~GsspInfoDlg()
{
    delete ui;
}

void GsspInfoDlg::InitUI()
{
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(6);
    QStringList theader;
    theader<<"Allele 11"<<"Allele 12"<<"="<<"Allele 21"<<"Allele 22"<<"GSSPs";
    ui->tableWidget->setHorizontalHeaderLabels(theader);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 20);
    ui->tableWidget->setColumnWidth(3, 100);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 200);

    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    ui->tableWidget_2->setColumnCount(10);
    QStringList header2;
    header2<<"Allele 11"<<"Allele 12"<<"="<<"Allele 21"<<"Allele 22"<<"Position"<<"Exon"<<"R/F"<<"GSSP"<<"GSSP Seq";
    ui->tableWidget_2->setHorizontalHeaderLabels(header2);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->setColumnWidth(0, 100);
    ui->tableWidget_2->setColumnWidth(1, 100);
    ui->tableWidget_2->setColumnWidth(2, 20);
    ui->tableWidget_2->setColumnWidth(3, 100);
    ui->tableWidget_2->setColumnWidth(4, 100);
    ui->tableWidget_2->setColumnWidth(5, 60);
    ui->tableWidget_2->setColumnWidth(6, 40);
    ui->tableWidget_2->setColumnWidth(7, 40);
    ui->tableWidget_2->setColumnWidth(8, 50);
    ui->tableWidget_2->setColumnWidth(9, 100);
}

void GsspInfoDlg::ConnectSignalandSlot()
{
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &GsspInfoDlg::slotSetGsspInfoTable2);
    connect(ui->btnDefault , &QPushButton::clicked, this, &GsspInfoDlg::slotClickSaveDefaultBt);
    connect(ui->btnUser, &QPushButton::clicked, this, &GsspInfoDlg::slotClickSaveUserBt);
    connect(ui->btnExit, &QPushButton::clicked, this, &GsspInfoDlg::close);
}

void GsspInfoDlg::slotSetGsspInfoTable2()
{
    int index = ui->tableWidget->currentRow();
    ui->tableWidget_2->clear();
    QStringList head;
    head<<ui->tableWidget->item(index, 0)->text()
        <<ui->tableWidget->item(index, 1)->text()
        <<"="
        <<ui->tableWidget->item(index, 3)->text()
        <<ui->tableWidget->item(index, 4)->text()
        <<"position"<<"Exon"<<"R/F"
        <<"GSSP"<<"GSSP Seq";
    ui->tableWidget_2->setHorizontalHeaderLabels(head);
    int size = m_vec_infos.at(index).size();
    ui->tableWidget_2->setRowCount(size);
    for(int i=0; i<size; i++)
    {
        ui->tableWidget_2->setRowHeight(i, 18);
        QStringList line = m_vec_infos.at(index).at(i).split(":");
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(line.at(0));
        ui->tableWidget_2->setItem(i, 0, item);

        item = new QTableWidgetItem;
        item->setText(line.at(1));
        ui->tableWidget_2->setItem(i, 1, item);

        item = new QTableWidgetItem;
        item->setText(line.at(2));
        ui->tableWidget_2->setItem(i, 3, item);

        item = new QTableWidgetItem;
        item->setText(line.at(3));
        ui->tableWidget_2->setItem(i, 4, item);

        item = new QTableWidgetItem;
        item->setText(line.at(4));
        ui->tableWidget_2->setItem(i, 5, item);

        item = new QTableWidgetItem;
        item->setText(line.at(5));
        ui->tableWidget_2->setItem(i, 6, item);

        item = new QTableWidgetItem;
        item->setText(line.at(6));
        ui->tableWidget_2->setItem(i, 7, item);

        item = new QTableWidgetItem;
        item->setText(line.at(7));
        ui->tableWidget_2->setItem(i, 8, item);

        item = new QTableWidgetItem;
        item->setText(line.at(8));
        ui->tableWidget_2->setItem(i, 9, item);
    }
}

void GsspInfoDlg::slotClickSaveDefaultBt()
{
    QString gssp = ui->lineEdit_Recommened->text();
    SoapTypingDB::GetInstance()->updateSetGsspBySampleName(m_str_SampleName, gssp);
    close();
}

void GsspInfoDlg::slotClickSaveUserBt()
{
    QString gssp = ui->lineEdit_User->text();
    SoapTypingDB::GetInstance()->updateSetGsspBySampleName(m_str_SampleName, gssp);
    close();
}

void GsspInfoDlg::setSampleName(const QString &sampleName)
{
    m_str_SampleName = sampleName;
    ui->lineEdit_Name->setText(m_str_SampleName);
}

void GsspInfoDlg::setGsspTable1(QVector<QStringList> &gsspInfos)
{
    int size = gsspInfos.size();
    ui->tableWidget->setRowCount(size);
    for(int i=0; i<size; i++)
    {
        ui->tableWidget->setRowHeight(i, 18);
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(0));
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(1));
        ui->tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem;
        item->setText("=");
        ui->tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(2));
        ui->tableWidget->setItem(i, 3, item);

        item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(3));
        ui->tableWidget->setItem(i, 4, item);

        item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(4));
        ui->tableWidget->setItem(i, 5, item);

        QStringList info = gsspInfos.at(i).at(5).split(";", QString::SkipEmptyParts);
        m_vec_infos.push_back(info);
    }
}


