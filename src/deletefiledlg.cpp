#include "deletefiledlg.h"
DeleteFileDlg::DeleteFileDlg(bool *isDone, QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("Delete File"));
    setWindowFlags(Qt::WindowMaximizeButtonHint);
    isDone_ = isDone;
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QSpacerItem *spacer = new QSpacerItem(50, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    setDeleteFileDlgDefault();
    hLayout1->addWidget(checkAllBox_);
    hLayout1->addSpacerItem(spacer);
    hLayout1->addWidget(deleteButton_);
    hLayout1->addWidget(exitButton_);

    hLayout2->addWidget(labelButton_);
    hLayout2->addWidget(progressBar_);
    vLayout->addWidget(table_);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    resize(600, 400);
    setTableDefaultSample();
}
void DeleteFileDlg::setDeleteFileDlgDefault()
{
    table_ = new QTableWidget(this);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setAlternatingRowColors(true);
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->verticalHeader()->setVisible(false);
    table_->setColumnCount(4);
    table_->setColumnWidth(0, 200);
    table_->setColumnWidth(1, 100);
    table_->setColumnWidth(2, 100);
    table_->setColumnWidth(3, 100);
    table_->resize(500, 300);
    QStringList header;
    header<<"Sample Name"<<"Analysis Type"<<"Mark Type"<<"User";
    table_->setHorizontalHeaderLabels(header);

    checkAllBox_ = new QCheckBox("check all", this);
    checkAllBox_->setChecked(true);
    deleteButton_ = new QPushButton("delete", this);
    exitButton_ = new QPushButton("exit", this);
    labelButton_ = new QPushButton("ready:",this);
    progressBar_ = new QProgressBar(this);
    connect(checkAllBox_, SIGNAL(clicked(bool)), this, SLOT(slotClickCheckAllBox(bool)));
    connect(deleteButton_, SIGNAL(clicked()), this, SLOT(slotClickDeleteButton()));
    connect(exitButton_, SIGNAL(clicked()), this, SLOT(close()));
}

void DeleteFileDlg::slotClickCheckAllBox(bool check)
{
    if(check)
    {
        for(int i=0; i<table_->rowCount(); i++)
        {
            table_->item(i, 0)->setCheckState(Qt::Checked);
        }
    }
    else
    {
        for(int i=0; i<table_->rowCount(); i++)
        {
            table_->item(i, 0)->setCheckState(Qt::Unchecked);
        }
    }
}


void DeleteFileDlg::slotClickDeleteButton()
{
    if(table_->rowCount()==0)
    {
        close();
        return;
    }
    QVector<int> index;
    for(int i=0; i<table_->rowCount();i++)
    {
        if(table_->item(i, 0)->checkState()==Qt::Checked)
        {
            index.push_back(i);
        }
    }
    if(index.size()==0)
    {
        QMessageBox::warning(this, tr("Soap Typing"), "Please choose sample to save!");
        return;
    }
    *isDone_ = true;
    checkAllBox_->setEnabled(false);
    exitButton_->setEnabled(false);
    deleteFile(index, sampleTreeInfoList_);
    close();
}

void DeleteFileDlg::setTableDefaultSample()
{
    getSampleTreeDataFromRealTimeDatabase(sampleTreeInfoList_);
    if(sampleTreeInfoList_.size()<=0)
        return;
    int size = sampleTreeInfoList_.size();
    table_->setRowCount(size);
    for(int i=0; i<size; i++)
    {
        const SampleTreeInfo& info = sampleTreeInfoList_.at(i);
        table_->setRowHeight(i, 20);
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setCheckState(Qt::Checked);
        item->setIcon(getIcon(info.analysisType, info.markType));
        item->setText(info.sampleName);
        table_->setItem(i, 0, item);

        item = new QTableWidgetItem;
        item->setText(getAnalysisType(info.analysisType));
        table_->setItem(i, 1, item);

        item = new QTableWidgetItem;
        item->setText(getMarkType(info.markType));
        table_->setItem(i, 2, item);

        item = new QTableWidgetItem;
        item->setText("default");
        table_->setItem(i, 3, item);
    }
}

void DeleteFileDlg::deleteFile(QVector<int> &index, QVector<SampleTreeInfo> &sampleInfos)
{
    labelButton_->setText("Waiting:..");
    progressBar_->setRange(0, index.size());
    for(int i=0; i<index.size(); i++)
    {
        deleteSample(sampleInfos.at(index.at(i)).sampleName.toAscii());
        progressBar_->setValue(i+1);
    }
    labelButton_->setText("Ready:");
    return;
}
