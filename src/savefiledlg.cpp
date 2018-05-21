#include "savefiledlg.h"
const QString resultPath = "Result";
SaveFileDlg::SaveFileDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("Save File"));
    setWindowFlags(Qt::WindowMaximizeButtonHint);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QSpacerItem *spacer = new QSpacerItem(50, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    setSaveFileDlgDefault();
    hLayout1->addWidget(checkAllBox_);
    hLayout1->addWidget(saveByDateBox_);
    hLayout1->addSpacerItem(spacer);
    hLayout1->addWidget(saveButton_);
    hLayout1->addWidget(exitButton_);

    hLayout2->addWidget(labelButton_);
    hLayout2->addWidget(progressBar_);
    vLayout->addWidget(table_);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    resize(600, 400);
    setTableDefaultSample();

}

void SaveFileDlg::autoSaveAllFile()
{
    QString tmpPath = "Tmp";
    QDir dir;
    dir.mkpath(tmpPath);
    QDate date(QDate::currentDate());
    QString dates = date.toString("yyyyMMdd");
    for(int i=0; i<sampleTreeInfoList_.size(); i++)
    {
        QString dirPath = QString("%1%2%3_%4").arg(tmpPath).arg(QDir::separator()).arg(sampleTreeInfoList_.at(i).sampleName.split("_").at(0)).arg(dates);
        QDir dir0(dirPath);
        if(!dir0.exists())
        {
            QDir dir;
            dir.mkpath(dirPath);
        }


        QString listFileName = QString("%1%2list.txt").arg(dirPath).arg(QDir::separator());
        QFile file(listFileName);
        if(file.exists())
        {
            file.remove();
        }
        file.open(QFile::WriteOnly);
        QTextStream stream(&file);
        QString outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(sampleTreeInfoList_.at(i).sampleName);
        stream<<"AType:"<<sampleTreeInfoList_.at(i).analysisType<<"\n";
        stream<<"MType:"<<sampleTreeInfoList_.at(i).markType<<"\n";
        stream<<"sampleTable:"<<outFile<<"\n";
        saveSample(sampleTreeInfoList_.at(i).sampleName.toAscii(), outFile, dates);

        for(int j=0; j<sampleTreeInfoList_.at(i).fileTreeInfo.size(); j++)
        {
            const FileTreeInfo &fileInfo = sampleTreeInfoList_.at(i).fileTreeInfo.at(j);
            outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(fileInfo.fileName);
            stream<<"fileTable:"<<outFile<<"\n";
            saveFile(fileInfo.fileName.toAscii(), outFile, dirPath, dates);
        }

        for(int j=0; j<sampleTreeInfoList_.at(i).gsspTreeInfo.size(); j++)
        {
            const GsspTreeInfo &gsspInfo = sampleTreeInfoList_.at(i).gsspTreeInfo.at(j);
            outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(gsspInfo.fileName);
            stream<<"gsspFileTable:"<<outFile<<"\n";
            saveGssp(gsspInfo.fileName.toAscii(), outFile, dirPath, dates);
        }
        file.close();
    }
}

void SaveFileDlg::setSaveFileDlgDefault()
{
    saveByDate_ = false;

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
    saveByDateBox_ = new QCheckBox("save by date", this);
    saveButton_ = new QPushButton("save", this);
    exitButton_ = new QPushButton("exit", this);
    labelButton_ = new QPushButton("ready:",this);
    progressBar_ = new QProgressBar(this);
    connect(checkAllBox_, SIGNAL(clicked(bool)), this, SLOT(slotClickCheckAllBox(bool)));
    connect(saveByDateBox_, SIGNAL(clicked(bool)),this, SLOT(slotClickCheckSaveByDateBox(bool)));
    connect(saveButton_, SIGNAL(clicked()), this, SLOT(slotClickSaveButton()));
    connect(exitButton_, SIGNAL(clicked()), this, SLOT(close()));
    //connect(this, SIGNAL(signalClose()), this, SLOT(close()));
}

void SaveFileDlg::slotClickCheckAllBox(bool check)
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

void SaveFileDlg::slotClickCheckSaveByDateBox(bool byDate)
{
    saveByDate_ = byDate;
}

void SaveFileDlg::slotClickSaveButton()
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
    checkAllBox_->setEnabled(false);
    saveByDateBox_->setEnabled(false);
    exitButton_->setEnabled(false);
    saveFileT(index, sampleTreeInfoList_);
    close();
    return;
}

void SaveFileDlg::setTableDefaultSample()
{
    getSampleTreeDataFromRealTimeDatabase(sampleTreeInfoList_);
    if(sampleTreeInfoList_.size()<=0)
        return;
    int size = sampleTreeInfoList_.size();
//    QTableWidgetItem *item = new QTableWidgetItem[size * 4];
    table_->setRowCount(size);
    for(int i=0; i<size; i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        const SampleTreeInfo& info = sampleTreeInfoList_.at(i);
        table_->setRowHeight(i, 20);
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

void SaveFileDlg::saveFileT(QVector<int> &index, QVector<SampleTreeInfo> &sampleInfos)
{
    QDir dir;
    dir.mkpath(resultPath);
    labelButton_->setText("Waiting:..");
    progressBar_->setRange(0, index.size());
    if(saveByDate_)
    {
        QDate date(QDate::currentDate());
        QString dates = QString("%1%2%3").arg(date.year()).arg(date.month()).arg(date.day());
        for(int i=0; i<index.size(); i++)
        {
            saveSampleTByDate(sampleInfos.at(index.at(i)), dates);
            progressBar_->setValue(i+1);
        }
    }
    else
    {
        for(int i=0; i<index.size(); i++)
        {
            saveSampleT(sampleInfos.at(index.at(i)));
            progressBar_->setValue(i+1);
        }
    }
    labelButton_->setText("Ready:");
    return;
}



QString getAnalysisType(int type)
{
    switch(type)
    {
    case 0:
        return "Match(common)";
    case 1:
        return "Match(rare)";
    case 2:
        return "Match(bad quality)";
    case 3:
        return "Mismatch";
    default:
        return "Undefined";
    }
    return "Undefined";
}

QString getMarkType(int type)
{
    switch(type)
    {
    case 0:
        return "OWNED";
    case 1:
        return "PENDING";
    case 2:
        return "REVIEWED";
    case 3:
        return "APPROVED";
    default:
        return "OWNED";
    }
    return "OWNED";
}

QIcon getIcon(int analysisType, int markType)
{
    return QIcon(QString(":/images/filetree%1%2.png").arg(markType).arg(analysisType));
}

void saveSampleT(const SampleTreeInfo &sampleInfo)
{
    QString dirPath = QString("%1%2%3").arg(resultPath).arg(QDir::separator()).arg(sampleInfo.sampleName);
    QDir dir0(dirPath);
    if(!dir0.exists())
    {
        QDir dir;
        dir.mkpath(dirPath);
    }
    QString listFileName = QString("%1%2list.txt").arg(dirPath).arg(QDir::separator());
    QFile file(listFileName);
    if(file.exists())
    {
        file.remove();
    }
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    QString outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(sampleInfo.sampleName);
    stream<<"AType:"<<sampleInfo.analysisType<<"\n";
    stream<<"MType:"<<sampleInfo.markType<<"\n";
    stream<<"sampleTable:"<<outFile<<"\n";
    saveSample(sampleInfo.sampleName.toAscii(), outFile,"");

    for(int i=0; i<sampleInfo.fileTreeInfo.size(); i++)
    {
        const FileTreeInfo &fileInfo = sampleInfo.fileTreeInfo.at(i);
        outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(fileInfo.fileName);
        stream<<"fileTable:"<<outFile<<"\n";
        saveFile(fileInfo.fileName.toAscii(), outFile, dirPath, "");
    }

    for(int i=0; i<sampleInfo.gsspTreeInfo.size(); i++)
    {
        const GsspTreeInfo &gsspInfo = sampleInfo.gsspTreeInfo.at(i);
        outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(gsspInfo.fileName);
        stream<<"gsspFileTable:"<<outFile<<"\n";
        saveGssp(gsspInfo.fileName.toAscii(), outFile, dirPath, "");
    }
    file.close();
}

void saveSampleTByDate(const SampleTreeInfo &sampleInfo, QString date)
{

    QString dirPath = QString("%1%2%3_%4").arg(resultPath).arg(QDir::separator()).arg(sampleInfo.sampleName.split("_").at(0)).arg(date);
    if(sampleInfo.sampleName.contains('_'))
    {
        QStringList sp = sampleInfo.sampleName.split("_",QString::SkipEmptyParts);
        QString dirPath = QString("%1%2%3_%4").arg(resultPath).arg(QDir::separator()).arg(sp.at(0)).arg(date);
    }
    QDir dir0(dirPath);
    if(!dir0.exists())
    {
        QDir dir;
        dir.mkpath(dirPath);
    }
    QString listFileName = QString("%1%2list.txt").arg(dirPath).arg(QDir::separator());
    QFile file(listFileName);
    if(file.exists())
    {
        file.remove();
    }
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    QString outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(sampleInfo.sampleName);
    stream<<"AType:"<<sampleInfo.analysisType<<"\n";
    stream<<"MType:"<<sampleInfo.markType<<"\n";
    stream<<"sampleTable:"<<outFile<<"\n";
    saveSample(sampleInfo.sampleName.toAscii(), outFile, date);

    for(int i=0; i<sampleInfo.fileTreeInfo.size(); i++)
    {
        const FileTreeInfo &fileInfo = sampleInfo.fileTreeInfo.at(i);
        outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(fileInfo.fileName);
        stream<<"fileTable:"<<outFile<<"\n";
        saveFile(fileInfo.fileName.toAscii(), outFile, dirPath, date);
    }

    for(int i=0; i<sampleInfo.gsspTreeInfo.size(); i++)
    {
        const GsspTreeInfo &gsspInfo = sampleInfo.gsspTreeInfo.at(i);
        outFile = QString("%1%2%3.txt").arg(dirPath).arg(QDir::separator()).arg(gsspInfo.fileName);
        stream<<"gsspFileTable:"<<outFile<<"\n";
        saveGssp(gsspInfo.fileName.toAscii(), outFile, dirPath, date);
    }
    file.close();
}



