#include "allelepairdlg.h"
#include "ui_allelepairdlg.h"
#include "DataBase/soaptypingdb.h"

AllelePairDlg::AllelePairDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllelePairDlg)
{
    ui->setupUi(this);
    ConnectSignalandSlot();
}

AllelePairDlg::~AllelePairDlg()
{
    delete ui;
}

void AllelePairDlg::SetData(const QString &str_genename)
{
    QStringList alleleNames;
    SoapTypingDB::GetInstance()->getAlleleNameListFromStaticDabase(str_genename, alleleNames);
    ui->comboBox->addItems(alleleNames);
    ui->comboBox_2->addItems(alleleNames);
}

void AllelePairDlg::ConnectSignalandSlot()
{
    connect(ui->btnOk, &QPushButton::clicked, this, &AllelePairDlg::slotClickOK);
}

void AllelePairDlg::slotClickOK()
{
    m_str_allele1 = ui->comboBox->currentText();
    m_str_allele2 = ui->comboBox->currentText();
    close();
}

void AllelePairDlg::getSelectAllele(QString &str_allele1, QString &str_allele2)
{
    str_allele1 = m_str_allele1;
    str_allele2 = m_str_allele2;
}
