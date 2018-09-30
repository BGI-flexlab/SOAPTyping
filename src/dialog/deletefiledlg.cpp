#include "deletefiledlg.h"
#include "ui_deletefiledlg.h"
#include "DataBase/soaptypingdb.h"
#include "Core/core.h"
#include <QMessageBox>

DeleteFileDlg::DeleteFileDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteFileDlg)
{
    ui->setupUi(this);
    InitUI();
    ConnectSignalandSlot();
    setTableDefaultSample();
}

DeleteFileDlg::~DeleteFileDlg()
{
    delete ui;
}

void DeleteFileDlg::InitUI()
{
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 100);
    QStringList header;
    header<<"Sample Name"<<"Analysis Type"<<"Mark Type"<<"User";
    ui->tableWidget->setHorizontalHeaderLabels(header);

}

void DeleteFileDlg::ConnectSignalandSlot()
{
    connect(ui->checkBox, &QCheckBox::clicked, this, &DeleteFileDlg::slotClickCheckAllBox);
    connect(ui->btnDel, &QPushButton::clicked, this, &DeleteFileDlg::slotClickDeleteButton);
    connect(ui->btnExit, &QPushButton::clicked, this, &DeleteFileDlg::close);
}

void DeleteFileDlg::slotClickCheckAllBox(bool status)
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        Qt::CheckState state = status ? Qt::Checked: Qt::Unchecked;
        ui->tableWidget->item(i, 0)->setCheckState(state);
    }
}

void DeleteFileDlg::slotClickDeleteButton()
{
    int i_row = ui->tableWidget->rowCount();
    if(i_row > 0)
    {
        QVector<SampleTreeInfo_t> vec_checked;
        for(int i=0; i<i_row;i++)
        {
            if(ui->tableWidget->item(i, 0)->checkState() == Qt::Checked)
            {
                QString str_name = ui->tableWidget->item(i, 0)->text();
                vec_checked.push_back(m_map_SampleTreeInfo[str_name]);
            }
        }

        if(vec_checked.empty())
        {
            QMessageBox::warning(this, tr("Soap Typing"), "Please choose sample to save!");
            return;
        }

        ui->checkBox->setEnabled(false);
        ui->btnExit->setEnabled(false);
        ui->btnDel->setEnabled(false);
        deleteFile(vec_checked);
        close();
    }
}

void DeleteFileDlg::deleteFile(QVector<SampleTreeInfo_t> &sampleInfos)
{
    ui->btnStatus->setText("Waiting:..");
    ui->progressBar->setRange(0, sampleInfos.size());
    for(int i=0; i<sampleInfos.size(); i++)
    {
        SoapTypingDB::GetInstance()->deleteSample(sampleInfos.at(i).sampleName);
        ui->progressBar->setValue(i+1);
    }
    ui->btnStatus->setText("Ready:");
    return;
}

void DeleteFileDlg::setTableDefaultSample()
{
    SoapTypingDB::GetInstance()->getSampleTreeDataFromSampleTable(m_map_SampleTreeInfo);

    ui->tableWidget->setRowCount(m_map_SampleTreeInfo.size());
    int i = 0;
    foreach(const SampleTreeInfo_t& info, m_map_SampleTreeInfo.values())
    {
        ui->tableWidget->setRowHeight(i, 20);
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setCheckState(Qt::Checked);
        item->setIcon(Core::GetInstance()->getIcon(info.analysisType, info.markType));
        item->setText(info.sampleName);
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem;
        item->setText(Core::GetInstance()->getAnalysisType(info.analysisType));
        ui->tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem;
        item->setText(Core::GetInstance()->getMarkType(info.markType));
        ui->tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem;
        item->setText("default");
        ui->tableWidget->setItem(i, 3, item);
        i++;
    }
}

