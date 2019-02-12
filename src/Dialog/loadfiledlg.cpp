#include "loadfiledlg.h"
#include "ui_loadfiledlg.h"
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include "Core/fileTablebase.h"
#include "DataBase/soaptypingdb.h"
#include "Core/core.h"

const QString RESULTPATH = "Result";

LoadFileDlg::LoadFileDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadFileDlg)
{
    ui->setupUi(this);
    InitUI();
    ConnectSignalandSlot();

    m_idaysNum = 0;
    getLoadInfo();
    getOkIndex();
    SetTableData();
}

LoadFileDlg::~LoadFileDlg()
{
    delete ui;
}

void LoadFileDlg::InitUI()
{
    QStringList li;
    li<<"Today"<<"Last 1 Days"<<"Last 3 Days"<<"Last 7 Days"<<"Last 30 Days"<<"All";
    ui->comboBox->addItems(li);

    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->setColumnWidth(2,150);
    ui->tableWidget->setColumnWidth(3,200);
    ui->tableWidget->setColumnWidth(5,200);
    QStringList header;
    header <<"Sample Name"<<"Analysis Type"<<"Mark Type"<<"Created Time"<<"Last Modifed Time";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void LoadFileDlg::ConnectSignalandSlot()
{
    connect(ui->btnLoad, &QPushButton::clicked, this, &LoadFileDlg::slotClickLoadButton);
    connect(ui->btnExit, &QPushButton::clicked, this, &LoadFileDlg::close);
    connect(ui->checkBox, &QCheckBox::clicked, this, &LoadFileDlg::slotClickCheckAllBox);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoadFileDlg::slotdateComboBoxChagned);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &LoadFileDlg::slotLineEditChanged);
}

void LoadFileDlg::slotClickLoadButton()
{
    if(m_vec_index.size()==0)
    {
        close();
        return;
    }

    ui->checkBox->setEnabled(false);
    ui->btnExit->setEnabled(false);
    ui->comboBox->setEditable(false);
    ui->lineEdit->setEnabled(false);


    QVector<QString> samplePaths;
    QVector<QString> filePaths;
    QVector<QString> gsspFilePaths;

    int size = m_vec_index.size();
    for(int i=0; i<size; i++)
    {
        if(ui->tableWidget->item(i, 0)->checkState()==Qt::Checked)
        {
            readListFile(m_vec_LoadInfo.at(m_vec_index.at(i)).listFile, samplePaths, filePaths, gsspFilePaths);
        }
    }

    int fileSize = samplePaths.size()+filePaths.size()+gsspFilePaths.size();
    int value=0;

    ui->btnStatus->setText("Waiting..");
    ui->progressBar->setRange(0, fileSize);
    for(int i=0; i<samplePaths.size();i++)
    {
        loadSample(samplePaths.at(i));
        ui->progressBar->setValue(++value);
    }

    for(int i=0; i<filePaths.size(); i++)
    {
        loadFile(filePaths.at(i));
        ui->progressBar->setValue(++value);
    }

    for(int i=0; i<gsspFilePaths.size(); i++)
    {
        loadGssp(gsspFilePaths.at(i));
        ui->progressBar->setValue(++value);
    }
    close();
}

void LoadFileDlg::slotClickCheckAllBox(bool status)
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        Qt::CheckState state = status ? Qt::Checked: Qt::Unchecked;
        ui->tableWidget->item(i, 0)->setCheckState(state);
    }
}

void LoadFileDlg::slotdateComboBoxChagned(int index)
{
    switch(index)
    {
    case 0:
        m_idaysNum=0;
        break;
    case 1:
        m_idaysNum=1;
        break;
    case 2:
        m_idaysNum=3;
        break;
    case 3:
        m_idaysNum=7;
        break;
    case 4:
        m_idaysNum=30;
        break;
    default:
        m_idaysNum=300000;
        break;
    }

    getOkIndex();
    SetTableData();
}

void LoadFileDlg::slotLineEditChanged(const QString & name)
{
    m_str_SearchName = name;
    getOkIndex();
    SetTableData();
}

void LoadFileDlg::getLoadInfo()
{
    m_vec_LoadInfo.clear();
    QDir dir;
    dir.setPath(RESULTPATH);
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
        loadInfo.createdTime = list.at(i).birthTime().toString();
        QString fileName = QString("%1%2%3%4list.txt").arg(RESULTPATH).arg(QDir::separator()).
                arg(loadInfo.sampleName).arg(QDir::separator());
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
        m_vec_LoadInfo.push_back(loadInfo);
    }
}

void LoadFileDlg::getOkIndex()
{
    m_vec_index.clear();
    for(int i=0; i<m_vec_LoadInfo.size();i++)
    {
        if(m_vec_LoadInfo.at(i).dateToNow > m_idaysNum)
            continue;
        if(!m_str_SearchName.isEmpty() && !m_vec_LoadInfo.at(i).sampleName.contains(m_str_SearchName))
            continue;
        m_vec_index.push_back(i);
    }
}

void LoadFileDlg::SetTableData()
{
    if(m_vec_index.size()<=0)
        return;

    int size = m_vec_index.size();
    ui->tableWidget->setRowCount(size);

    for(int i=0; i<size;i++)
    {
        QTableWidgetItem *itemN = new QTableWidgetItem;
        const LoadInfo &loadInfo = m_vec_LoadInfo.at(m_vec_index.at(i));
        itemN->setCheckState(ui->checkBox->checkState());
        itemN->setIcon(Core::GetInstance()->getIcon(loadInfo.analysisType, loadInfo.markType));
        itemN->setText(loadInfo.sampleName);
        ui->tableWidget->setItem(i, 0, itemN);
        itemN = new QTableWidgetItem;
        itemN->setText(Core::GetInstance()->getAnalysisType(loadInfo.analysisType));
        ui->tableWidget->setItem(i, 1, itemN);

        itemN = new QTableWidgetItem;
        itemN->setText(Core::GetInstance()->getMarkType(loadInfo.markType));
        ui->tableWidget->setItem(i, 2, itemN);

        itemN = new QTableWidgetItem;
        itemN->setText(loadInfo.createdTime);
        ui->tableWidget->setItem(i, 3, itemN);

        itemN = new QTableWidgetItem;
        itemN->setText(loadInfo.modifiedTime);
        ui->tableWidget->setItem(i, 4, itemN);
    }
}

void LoadFileDlg::readListFile(const QString &listFilePath, QVector<QString> &samplePaths,
                  QVector<QString> &filePaths, QVector<QString> &gsspFilePaths)
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

void LoadFileDlg::loadSample(const QString &samplePath)
{
    SampleTable sampletable;

    QFile file(samplePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream stream(&file);

    sampletable.setSampleName(stream.readLine());
    sampletable.setGeneName(stream.readLine());
    sampletable.setFileType(stream.readLine().toInt());
    sampletable.setMarkType(stream.readLine().toInt());
    sampletable.setAnalysisType(stream.readLine().toInt());
    sampletable.setMinExonIndex(stream.readLine().toInt());
    sampletable.setMaxExonIndex(stream.readLine().toInt());
    sampletable.setExonStartPos(stream.readLine().toInt());
    sampletable.setExonEndPos(stream.readLine().toInt());
    sampletable.setConsensusSequence(stream.readLine());
    sampletable.setForwardSequence(stream.readLine());
    sampletable.setReverseSequence(stream.readLine());
    sampletable.setPatternSequence(stream.readLine());
    sampletable.setMismatchBetweenPC(stream.readLine());
    sampletable.setMismatchBetweenFR(stream.readLine());
    sampletable.setMmismatchBetweenFR(stream.readLine());
    sampletable.setEditPostion(stream.readLine());
    sampletable.setTypeResult(stream.readLine());
    sampletable.setGsspInfo(stream.readLine());
    sampletable.setShieldAllele(stream.readLine());
    sampletable.setSetResult(stream.readLine());
    sampletable.setSetNote(stream.readLine());
    sampletable.setSetGSSP(stream.readLine());
    sampletable.setCombinedResult(stream.readLine());

    SoapTypingDB::GetInstance()->insertOneSampleTable(sampletable);
    return;
}

void LoadFileDlg::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream stream(&file);

    Ab1FileTableBase filetable;
    filetable.setFileName(stream.readLine());
    filetable.setSampleName(stream.readLine());
    filetable.setFilePath(stream.readLine());
    filetable.setExtraFile(stream.readLine().toInt());
    filetable.setGeneName(stream.readLine());
    filetable.setExonIndex(stream.readLine().toInt());
    filetable.setROrF(stream.readLine().at(0));
    filetable.setExonStartPos(stream.readLine().toInt());
    filetable.setExonEndPos(stream.readLine().toInt());
    filetable.setUsefulSequence(stream.readLine());
    filetable.setBaseSequence(stream.readLine().toLatin1());
    filetable.setBasePostion(stream.readLine());
    filetable.setBaseQuality(stream.readLine());
    filetable.setBaseNumber(stream.readLine().toInt());
    filetable.setBaseASignal(stream.readLine());
    filetable.setBaseTSignal(stream.readLine());
    filetable.setBaseGSignal(stream.readLine());
    filetable.setBaseCSignal(stream.readLine());
    filetable.setSignalNumber(stream.readLine().toInt());
    filetable.setMaxSignal(stream.readLine().toInt());
    filetable.setMaxQuality(stream.readLine().toInt());
    filetable.setAverageBaseWidth(stream.readLine().toFloat());
    filetable.setIsGood(stream.readLine().toInt());
    filetable.setAlignResult(stream.readLine().toInt());
    filetable.setAlignStartPos(stream.readLine().toInt());
    filetable.setAlignEndPos(stream.readLine().toInt());
    filetable.setAlignInfo(stream.readLine());
    filetable.setExcludeLeft(stream.readLine().toInt());
    filetable.setExcludeRight(stream.readLine().toInt());
    filetable.setEditInfo(stream.readLine());

    SoapTypingDB::GetInstance()->InsertOneFileTable(filetable);
    file.close();
    return;
}

void LoadFileDlg::loadGssp(const QString &gsspFilePath)
{
    QFile file(gsspFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream stream(&file);

    Ab1FileTableBase filetable;
    filetable.setFileName(stream.readLine());
    filetable.setSampleName(stream.readLine());
    filetable.setFilePath(stream.readLine());
    filetable.setGsspName(stream.readLine());
    filetable.setGeneName(stream.readLine());
    filetable.setExonIndex(stream.readLine().toInt());
    filetable.setROrF(stream.readLine().at(0));
    filetable.setExonStartPos(stream.readLine().toInt());
    filetable.setExonEndPos(stream.readLine().toInt());
    filetable.setUsefulSequence(stream.readLine());
    filetable.setBaseSequence(stream.readLine().toLatin1());
    filetable.setBasePostion(stream.readLine());
    filetable.setBaseQuality(stream.readLine());
    filetable.setBaseNumber(stream.readLine().toInt());
    filetable.setBaseASignal(stream.readLine());
    filetable.setBaseTSignal(stream.readLine());
    filetable.setBaseGSignal(stream.readLine());
    filetable.setBaseCSignal(stream.readLine());
    filetable.setSignalNumber(stream.readLine().toInt());
    filetable.setMaxSignal(stream.readLine().toInt());
    filetable.setMaxQuality(stream.readLine().toInt());
    filetable.setAverageBaseWidth(stream.readLine().toFloat());
    filetable.setIsGood(stream.readLine().toInt());
    filetable.setAlignResult(stream.readLine().toInt());
    filetable.setAlignStartPos(stream.readLine().toInt());
    filetable.setAlignEndPos(stream.readLine().toInt());
    filetable.setAlignInfo(stream.readLine());
    filetable.setExcludeLeft(stream.readLine().toInt());
    filetable.setExcludeRight(stream.readLine().toInt());
    filetable.setEditInfo(stream.readLine());
    filetable.setTypeResult(stream.readLine());
    filetable.setFilterResult(stream.readLine());

    SoapTypingDB::GetInstance()->insertOneGsspFileTable(filetable);
    file.close();
    return;
}
