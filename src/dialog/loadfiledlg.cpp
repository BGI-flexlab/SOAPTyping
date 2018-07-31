#include "loadfiledlg.h"
#include "savefiledlg.h"
#include <QtGui>
const QString resultPath = "Result";

LoadFileDlg::LoadFileDlg(bool *isDone, QWidget *parent)
    : QDialog(parent)
{
    isDone_ = isDone;
    this->setWindowTitle("Load Samples");
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    QHBoxLayout *hBoxLayout1 = new QHBoxLayout;
    QHBoxLayout *hBoxLayout2 = new QHBoxLayout;
    QHBoxLayout *hBoxLayout3 = new QHBoxLayout;
    QLabel *findByDataLabel = new QLabel;
    findByDataLabel->setText("Search By Date: ");
    QLabel *findByNameLabel = new QLabel;
    findByNameLabel->setText("Search By Name: ");
    QSpacerItem *spacer1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer2 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer3 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

    setLoadFileDlgDefault();

    hBoxLayout1->addWidget(findByDataLabel);
    hBoxLayout1->addWidget(dateComboBox);
    hBoxLayout1->addSpacerItem(spacer1);
    hBoxLayout1->addWidget(findByNameLabel);
    hBoxLayout1->addWidget(nameLineEdit);
    hBoxLayout1->addSpacerItem(spacer2);

    hBoxLayout2->addWidget(checkAllBox);
    hBoxLayout2->addSpacerItem(spacer3);
    hBoxLayout2->addWidget(loadButton);
    hBoxLayout2->addWidget(exitButton);

    hBoxLayout3->addWidget(labelButton);
    hBoxLayout3->addWidget(progressBar);

    vBoxLayout->addWidget(table);
    vBoxLayout->addLayout(hBoxLayout1);
    vBoxLayout->addLayout(hBoxLayout2);
    vBoxLayout->addLayout(hBoxLayout3);
    this->resize(900, 600);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(slotClickLoadButton()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(checkAllBox, SIGNAL(clicked(bool)), this, SLOT(slotClickCheckAllBox(bool)));
    connect(dateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotdateComboBoxChagned(int)));
    connect(nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLineEditChanged(QString)));

    daysNum = 0;
    getLoadInfo();
    getOkIndex();
    setTableDate();
}
void LoadFileDlg::setLoadFileDlgDefault()
{

    dateComboBox = new QComboBox(this);
    QStringList li;
    li<<"Today"<<"Last 1 Days"<<"Last 3 Days"<<"Last 7 Days"<<"Last 30 Days"<<"All";
    dateComboBox->addItems(li);
    dateComboBox->setEditable(false);
    nameLineEdit = new QLineEdit(this);

    checkAllBox = new QCheckBox("check all",this);
    checkAllBox->setChecked(true);

    loadButton = new QPushButton("load",this);
    exitButton = new QPushButton("exit", this);

    table = new QTableWidget(this);
    table->verticalHeader()->setVisible(false);
    table->setColumnCount(5);
    table->setColumnWidth(0,200);
    table->setColumnWidth(1,150);
    table->setColumnWidth(2,150);
    table->setColumnWidth(3,200);
    table->setColumnWidth(5,200);
    QStringList header;
    header <<"Sample Name"<<"Analysis Type"<<"Mark Type"<<"Created Time"<<"Last Modifed Time";
    table->setHorizontalHeaderLabels(header);
    table->horizontalHeader()->setStretchLastSection(true);
    table->resize(800,500);

    labelButton = new QPushButton("Ready:", this);
    progressBar = new QProgressBar(this);
}

void LoadFileDlg::setTableDate()
{
    table->clear();
    QStringList header;
    header <<"Sample Name"<<"Analysis Type"<<"Mark Type"<<"Created Time"<<"Last Modifed Time";
    table->setHorizontalHeaderLabels(header);
    if(okFileIndex.size()<=0)
        return;

    int size = okFileIndex.size();
    table->setRowCount(size);
    //QTableWidgetItem *itemN = new QTableWidgetItem[size * 5];
    for(int i=0; i<size;i++)
    {
        QTableWidgetItem *itemN = new QTableWidgetItem;
        const LoadInfo &loadInfo = loadInfos.at(okFileIndex.at(i));
        itemN->setCheckState(checkAllBox->checkState());
        itemN->setIcon(getIcon(loadInfo.analysisType, loadInfo.markType));
        itemN->setText(loadInfo.sampleName);
        table->setItem(i, 0, itemN);
        itemN = new QTableWidgetItem;
        itemN->setText(getAnalysisType(loadInfo.analysisType));
        table->setItem(i, 1, itemN);

        itemN = new QTableWidgetItem;
        itemN->setText(getMarkType(loadInfo.markType));
        table->setItem(i, 2, itemN);

        itemN = new QTableWidgetItem;
        itemN->setText(loadInfo.createdTime);
        table->setItem(i, 3, itemN);

        itemN = new QTableWidgetItem;
        itemN->setText(loadInfo.modifiedTime);
        table->setItem(i, 4, itemN);
    }
}

void LoadFileDlg::getOkIndex()
{
    okFileIndex.clear();
    for(int i=0; i<loadInfos.size();i++)
    {
        if(loadInfos.at(i).dateToNow > daysNum)
            continue;
        if(!name.isEmpty() && !loadInfos.at(i).sampleName.contains(name))
            continue;
        okFileIndex.push_back(i);
    }
}


void LoadFileDlg::getLoadInfo()
{
    loadInfos.clear();
    QDir dir;
    dir.setPath(resultPath);
    if(!dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::Dirs);
    dir.setSorting(QDir::Name|QDir::Time);
    QFileInfoList list=dir.entryInfoList();
    for(int i=0; i<list.size();i++)
    {

        LoadInfo loadInfo;

        loadInfo.sampleName = list.at(i).baseName();
        loadInfo.createdTime = list.at(i).created().toString();
        QString fileName = QString("%1%2%3%4list.txt").arg(resultPath).arg(QDir::separator()).arg(loadInfo.sampleName).arg(QDir::separator());
        QFileInfo fileInfo(fileName);
        if(!fileInfo.exists())
            continue;
        loadInfo.dateToNow = fileInfo.lastModified().daysTo(QDateTime::currentDateTime());
        loadInfo.modifiedTime=fileInfo.lastModified().toString();
        loadInfo.listFile = fileInfo.filePath();
        QFile file(fileInfo.filePath());
        file.open(QFile::ReadOnly);
        QTextStream stream(&file);
        loadInfo.analysisType=stream.readLine().split(":").at(1).toInt();
        loadInfo.markType=stream.readLine().split(":").at(1).toInt();
        file.close();
        loadInfos.push_back(loadInfo);
    }
}

void LoadFileDlg::slotClickCheckAllBox(bool check)
{
    if(check)
    {
        for(int i=0; i<table->rowCount(); i++)
        {
            table->item(i, 0)->setCheckState(Qt::Checked);
        }
    }
    else
    {
        for(int i=0; i<table->rowCount(); i++)
        {
            table->item(i, 0)->setCheckState(Qt::Unchecked);
        }
    }
}

void LoadFileDlg::slotClickLoadButton()
{
    if(okFileIndex.size()==0)
    {
        close();
        return;
    }

    int size = okFileIndex.size();
    for(int i=0; i<size; i++)
    {
        if(table->item(i, 0)->checkState()==Qt::Checked)
        {
            *isDone_ = true;
            break;
        }
    }
    if(!*isDone_)
    {
        QMessageBox::warning(this, tr("Soap Typing"), tr("Please choose sample to load!"));
        return;
    }

    checkAllBox->setEnabled(false);
    exitButton->setEnabled(false);
    dateComboBox->setEditable(false);
    nameLineEdit->setEnabled(false);


    QVector<QString> samplePaths;
    QVector<QString> filePaths;
    QVector<QString> gsspFilePaths;

    for(int i=0; i<size; i++)
    {
        if(table->item(i, 0)->checkState()==Qt::Checked)
        {
            readListFile(loadInfos.at(okFileIndex.at(i)).listFile, samplePaths, filePaths, gsspFilePaths);
        }
    }

    int fileSize = samplePaths.size()+filePaths.size()+gsspFilePaths.size();
    int value=0;

    labelButton->setText("Waiting..");
    progressBar->setRange(0, fileSize);
    for(int i=0; i<samplePaths.size();i++)
    {
        loadSample(samplePaths.at(i));
        progressBar->setValue(++value);
    }

    for(int i=0; i<filePaths.size(); i++)
    {
        loadFile(filePaths.at(i));
        progressBar->setValue(++value);
    }

    for(int i=0; i<gsspFilePaths.size(); i++)
    {
        loadGssp(gsspFilePaths.at(i));
        progressBar->setValue(++value);
    }
    close();
}


void LoadFileDlg::slotdateComboBoxChagned(int i)
{
    switch(i)
    {
    case 0:
        daysNum=0;
        break;
    case 1:
        daysNum=1;
        break;
    case 2:
        daysNum=3;
        break;
    case 3:
        daysNum=7;
        break;
    case 4:
        daysNum=30;
        break;
    default:
        daysNum=300000;
        break;
    }
    getOkIndex();
    setTableDate();
}

void LoadFileDlg::slotLineEditChanged(QString name0)
{
    name = name0;
    getOkIndex();
    setTableDate();
}

void readListFile(const QString &listFilePath, QVector<QString> &samplePaths, QVector<QString> &filePaths, QVector<QString> &gsspFilePaths)
{
    QFile file(listFilePath);
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QStringList line = stream.readLine().split(":");
        if(line.at(0)=="sampleTable")
        {
            samplePaths.push_back(line.at(1));
        }
        else if(line.at(0)=="fileTable")
        {
            filePaths.push_back(line.at(1));
        }
        else if(line.at(0)=="gsspFileTable")
        {
            gsspFilePaths.push_back(line.at(1));
        }
    }
    file.close();
}
