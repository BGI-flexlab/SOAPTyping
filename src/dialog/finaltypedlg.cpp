#include "finaltypedlg.h"
#include "ui_finaltypedlg.h"
#include "DataBase/soaptypingdb.h"

FinalTypeDlg::FinalTypeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinalTypeDlg)
{
    ui->setupUi(this);
    InitUI();
    ConnectSignalandSlot();
}

FinalTypeDlg::~FinalTypeDlg()
{
    delete ui;
}

void FinalTypeDlg::InitUI()
{
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnCount(3);
    QStringList header;
    header<<"Mis"<<"Allele1"<<"Allele2";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnWidth(0, 50);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 100);
}

void FinalTypeDlg::ConnectSignalandSlot()
{
    connect(ui->btnSet, &QPushButton::clicked, this, &FinalTypeDlg::slotClickSetButton);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &FinalTypeDlg::slotRowChanged);
    connect(ui->btnSave,&QPushButton::clicked, this, &FinalTypeDlg::slotClickSaveButton);
    connect(ui->btnExit, &QPushButton::clicked, this, &FinalTypeDlg::close);
}

void FinalTypeDlg::slotClickSetButton()
{
    ui->lineEdit_allele1->setText(m_str_allele1);
    ui->lineEdit_allele2->setText(m_str_allele2);
}

void FinalTypeDlg::slotRowChanged()
{
    int row = ui->tableWidget->currentRow();
    m_str_allele1 = ui->tableWidget->item(row, 1)->text();
    m_str_allele2 = ui->tableWidget->item(row, 2)->text();
}

void FinalTypeDlg::slotClickSaveButton()
{
    QString result = QString("%1,%2").arg(ui->lineEdit_allele1->text()).arg(ui->lineEdit_allele2->text());
    SoapTypingDB::GetInstance()->updateSetResultBySampleName(m_str_SampleName, result);
    close();
}

void FinalTypeDlg::setSampleName(const QString &sampleName)
{
    m_str_SampleName = sampleName;
    ui->lineEdit_samplename->setText(sampleName);
    QString resultString;
    SoapTypingDB::GetInstance()->getSetResultBySampleName(sampleName, resultString);
    if(resultString.isEmpty())
    {
        return;
    }

    QStringList sp = resultString.split(",", QString::SkipEmptyParts);
    m_str_allele1 = sp.at(0);
    m_str_allele2 = sp.at(1);
    slotClickSetButton();
}

void FinalTypeDlg::setAlleleResult(const QVector<QStringList> &result)
{
    int size = result.size();
    if (size <= 0)
        return;
    ui->tableWidget->setRowCount(size);

    for(int i=0; i<size; i++)
    {
        ui->tableWidget->setRowHeight(i, 18);
        for(int j=0; j<3; j++)
        {
            QTableWidgetItem *item =  new QTableWidgetItem;
            item->setText(result.at(i).at(j));
            ui->tableWidget->setItem(i, j, item);
        }
    }
}
