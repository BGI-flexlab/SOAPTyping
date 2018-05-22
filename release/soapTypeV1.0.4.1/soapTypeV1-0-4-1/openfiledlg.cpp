#include "openfiledlg.h"
#include "core.h"
#include "config.h"
#include "commongsspdatabase.h"

OpenFileDlg::OpenFileDlg(QWidget *parent)
    :QDialog(parent)
{
    file_num = 0;
    sample_num = 0;
    setWindowTitle(tr("Open File"));
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    QHBoxLayout *hBoxLayout2 = new QHBoxLayout;
    checkBox_ = new QCheckBox("Check All");
    checkBox_->setCheckState(Qt::Unchecked);
    removeCheckedButton_ = new QPushButton("Delete Checked",this);
    openFileButton_ = new QPushButton("Open", this);
    analysisButton_ = new QPushButton("Analysis All", this);
    analysisButton_->setEnabled(false);
    cancelButton_ = new QPushButton("Cancel", this);
    QSpacerItem *spacer1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    //    QSpacerItem *spacer2 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    //    QSpacerItem *spacer3 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    tableWidget_ = new QTableWidget;
    tableWidget_->setEditTriggers(QAbstractItemView::DoubleClicked);
    tableWidget_->setStyleSheet("selection-background-color:#3CBADA");
    QStringList theader;
    theader<<"Sample Name"<<"Gene"<<"Gssp"<<"Exon"<<"F/R"<<"File Name"<<"File Path"<<"";
    tableWidget_->verticalHeader()->setVisible(false);
    tableWidget_->setColumnCount(7);
    tableWidget_->setHorizontalHeaderLabels(theader);
    tableWidget_->horizontalHeader()->setStretchLastSection(true);
    tableWidget_->setColumnWidth(0, 180);
    tableWidget_->setColumnWidth(1, 60);
    tableWidget_->setColumnWidth(2, 50);
    tableWidget_->setColumnWidth(3, 50);
    tableWidget_->setColumnWidth(4, 50);
    tableWidget_->setColumnWidth(5, 250);
    tableWidget_->setColumnWidth(6, 390);
    button_ = new QPushButton(this);
    button_->setMinimumWidth(150);
    button_->setText(QString("F:%1/S:%2 Ready:   ").arg(file_num).arg(sample_num));
    progressBar_ = new QProgressBar(this);
    hBoxLayout->addWidget(checkBox_);
    hBoxLayout->addWidget(removeCheckedButton_);
    hBoxLayout->addSpacerItem(spacer1);
    hBoxLayout->addWidget(openFileButton_);
    //    hBoxLayout->addSpacerItem(spacer2);
    hBoxLayout->addWidget(analysisButton_);
    //    hBoxLayout->addSpacerItem(spacer3);
    hBoxLayout->addWidget(cancelButton_);

    hBoxLayout2->addWidget(button_);
    hBoxLayout2->addWidget(progressBar_);
    vBoxLayout->addWidget(tableWidget_);
    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addLayout(hBoxLayout2);
    resize(600, 400);
    connect(checkBox_, SIGNAL(stateChanged(int)), this, SLOT(slotCheck(int)));
    connect(removeCheckedButton_, SIGNAL(clicked()), this, SLOT(slotRemove()));
    connect(openFileButton_, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(analysisButton_, SIGNAL(clicked()), this, SLOT(slotAnalysis()));
    connect(cancelButton_, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(signalGetNumbers()), this, SLOT(slotGetNumbers()));
    setWindowFlags(Qt::WindowMaximizeButtonHint);

    //    fileTableWatcher_ = new QFutureWatcher<bool>(this);
    //    gsspFileTableWatcher_ = new QFutureWatcher<bool>(this);
    //    sampleTableWatcher_ = new QFutureWatcher<bool>(this);
    range_ = 0;
    value_ = 0;
    sampleNameList_.clear();
    fileTableList_.clear();
    gsspFileTableList_.clear();
    sampleSet_.clear();
    threadpool =new TypeThreadPool();



}

void OpenFileDlg::setData(bool *done, SignalInfo *signalInfo)
{
    done_ = done;
    signalInfo_ = signalInfo;
    getGeneNames(geneNames_);
    gsspNames_.push_back("");
    getGsspNames(gsspNames_);
    getGsspMapToExonAndFR(mapToExonAndRF_);
    exonNames_<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
    rOrFNames_<<"F"<<"R";
}
void OpenFileDlg::addRowToTableWidget(const OpenFileTable &openFileTable)
{
    int rowC = tableWidget_->rowCount();
    tableWidget_->setRowCount(rowC+1);
    tableWidget_->setRowHeight(rowC, 18);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setCheckState(Qt::Unchecked);
    item0->setText(openFileTable.sampleName);
    if(!openFileTable.right)
    {
        item0->setBackgroundColor(QColor(230,230,230));
    }
    tableWidget_->setItem(rowC, 0, item0);

    QComboBoxNew *box1 = new QComboBoxNew;
    box1->addItems(geneNames_);
    setComboBoxData(box1, openFileTable.geneName);
    tableWidget_->setCellWidget(rowC, 1, box1);

    QComboBoxNew *box2 = new QComboBoxNew;
    box2->addItems(gsspNames_);
    setComboBoxData(box2, openFileTable.gsspName);
    tableWidget_->setCellWidget(rowC, 2, box2);

    QComboBoxNew *box3 = new QComboBoxNew;
    box3->addItems(exonNames_);
    setComboBoxData(box3, openFileTable.exonIndex);
    tableWidget_->setCellWidget(rowC, 3, box3);

    QComboBoxNew *box4 = new QComboBoxNew;
    box4->addItems(rOrFNames_);
    setComboBoxData(box4, openFileTable.rOrF);
    tableWidget_->setCellWidget(rowC, 4, box4);
    tableWidget_->cellWidget(rowC, 4)->backgroundRole();

    QTableWidgetItem *item5 = new QTableWidgetItem;
    item5->setText(openFileTable.fileName);
    if(!openFileTable.right)
    {
        item5->setBackgroundColor(QColor(230,230,230));
    }
    tableWidget_->setItem(rowC, 5, item5);

    QTableWidgetItem *item6 = new QTableWidgetItem;
    item6->setText(openFileTable.filePath);
    if(!openFileTable.right)
    {
        item6->setBackgroundColor(QColor(230,230,230));
    }
    tableWidget_->setItem(rowC, 6, item6);
}

void OpenFileDlg::setComboBoxData(QComboBoxNew *box, const QString &text)
{
    int index = box->findText(text);
    box->setCurrentIndex(index);
}

void OpenFileDlg::filePathProcess(const QString &filePath)
{
    OpenFileTable openFileTable;
    bool isOK = analysisFileName(filePath, openFileTable);
    if(isOK)
    {
        addRowToTableWidget(openFileTable);
    }

}
void OpenFileDlg::filePathListProcess(const QStringList &filePathList)
{
    for(int i=0; i<filePathList.size(); i++)
    {
        filePathProcess(filePathList.at(i));
        progressBar_->setValue(i+1);
        QCoreApplication::processEvents();
    }
}

bool OpenFileDlg::analysisFileName(const QString &filePath, OpenFileTable &openFileTable)
{
    openFileTable.filePath  = filePath;
    QFileInfo fileInfo(filePath);
    //    if(fileInfo.suffix() != "ab1")
    //    {
    //        qDebug()<<filePath<<"is not ab1 file";
    //        return false;
    //    }
    openFileTable.fileName = fileInfo.fileName();
    if(fileExists_.contains(openFileTable.fileName))
        return false;
    QStringList part = openFileTable.fileName.split("_");
    if(part.size() != 4)
    {
        openFileTable.right = false;
    }
    else
    {
        openFileTable.sampleName = part.at(0);
        openFileTable.geneName = part.at(1);
        bool isRight = analysisExonInfo(part.at(2), openFileTable);
        if(!isRight)
        {
            openFileTable.right = false;
        }
    }
    fileExists_.insert(openFileTable.fileName);
    return true;
}

bool OpenFileDlg::analysisExonInfo(const QString &exonString, OpenFileTable &openFileTable)
{
    char RF = exonString.at(1).toAscii();
    if((RF=='R' || RF=='F') && exonString.size() == 2)
    {
        openFileTable.exonIndex = exonString.at(0);
        openFileTable.rOrF = exonString.at(1);
        openFileTable.gsspName = "";
        return true;
    }
    else
    {
        QMap<QString, ExonAndRF>::iterator it = mapToExonAndRF_.find(exonString);
        if(it == mapToExonAndRF_.end())
        {
            //            qDebug()<<"gssp"<<exonString<<"error in file"<<openFileTable.fileName;
            //            return false;
            return findExonAndRFByCommonGsspName(exonString, openFileTable.exonIndex, openFileTable.rOrF);
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
void OpenFileDlg::slotCheck(int state)
{
    int rowC = tableWidget_->rowCount();
    for(int i=0; i<rowC; i++)
    {
        tableWidget_->item(i, 0)->setCheckState(Qt::CheckState(state));
    }
}
void OpenFileDlg::slotRemove()
{
    int rowC = tableWidget_->rowCount();
    for(int i=rowC-1; i>=0; i--)
    {
        if(tableWidget_->item(i, 0)->checkState() == Qt::Checked)
        {
            fileExists_.remove(tableWidget_->item(i, 5)->text());
            delete tableWidget_->item(i, 6);
            delete tableWidget_->item(i, 5);
            for(int j=4; j>0; j--)
            {
                tableWidget_->removeCellWidget(i, j);
            }
            delete tableWidget_->item(i, 0);
            tableWidget_->removeRow(i);
        }
    }
    emit signalGetNumbers();
}
void  Savedir(QString dir)
{
    QFile data("Database/dir.ini");
    if (!data.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }
    QTextStream out(&data);//QDataStream
    out << dir<<"\r\n";
    data.flush();
    data.close();

}

QString  Readdir()
{
    QString dir;
    QFile data("Database/dir.ini");
    if (!data.open(QFile::ReadOnly | QFile::Text))
    {
        return "";
    }
    QTextStream in(&data);//QDataStream
    in >> dir;
    data.close();
    return dir;
}
void OpenFileDlg::slotOpenFile()
{
    //    QStringList filePathList = QFileDialog::getOpenFileNames(this, tr("open files"));
    QStringList filePathList = QFileDialog::getOpenFileNames(this, tr("open files"), Readdir(), "Peak Files(*.ab1;*abi)");

    if(filePathList.size() <= 0)
        return;
    Savedir(filePathList[0]);
    button_->setText(QString("F:%1/S:%2 Wating...").arg(file_num).arg(sample_num));
    progressBar_->setRange(0, filePathList.size());
    filePathListProcess(filePathList);
    button_->setText(QString("F:%1/S:%2 Ready:   ").arg(file_num).arg(sample_num));
    progressBar_->reset();
    analysisButton_->setEnabled(true);
    emit signalGetNumbers();
}

void OpenFileDlg::slotAnalysis()
{
    int rowC = tableWidget_->rowCount();
    if(rowC <= 0)
    {
        close();
        return;
    }

    analysisButton_->setEnabled(false);
    checkBox_->setEnabled(false);
    removeCheckedButton_->setEnabled(false);
    openFileButton_->setEnabled(false);
    cancelButton_->setEnabled(false);
    button_->setText(QString("F:%1/S:%2 Wating...").arg(file_num).arg(sample_num));
    sampleNameList_.clear();
    fileTableList_.clear();
    gsspFileTableList_.clear();
    sampleSet_.clear();

    bool flag = true;
    for(int i=0; i<rowC; i++)
    {
        QComboBoxNew *box = (QComboBoxNew*)tableWidget_->cellWidget(i, 2);
        if(box->currentText().isEmpty())
        {
            FileTable fileTable;
            fileTable.sampleName = tableWidget_->item(i, 0)->text().toAscii();
            box = (QComboBoxNew*)tableWidget_->cellWidget(i, 1);
            fileTable.geneName = box->currentText().toAscii();
            box = (QComboBoxNew*)tableWidget_->cellWidget(i, 3);
            fileTable.exonIndex = box->currentText().toInt();
            box = (QComboBoxNew*)tableWidget_->cellWidget(i, 4);
            fileTable.rOrF = box->currentText().toAscii();

            fileTable.fileName = tableWidget_->item(i, 5)->text().toAscii();
            fileTable.filePath = tableWidget_->item(i, 6)->text();

            if(fileTable.sampleName.isEmpty()||fileTable.geneName.isEmpty()||fileTable.rOrF.isEmpty()
                    ||fileTable.exonIndex<1 || fileTable.exonIndex>8)
            {
                tableWidget_->selectRow(i);
                tableWidget_->verticalScrollBar()->setSliderPosition(i);
                QMessageBox::warning(this, tr("SoapTyping"),QString("File: %1\nIncomplete information at line %2").arg(QString(fileTable.fileName)).arg(i+1));
                analysisButton_->setEnabled(true);
                checkBox_->setEnabled(true);
                removeCheckedButton_->setEnabled(true);
                openFileButton_->setEnabled(true);
                cancelButton_->setEnabled(true);
                button_->setText(QString("F:%1/S:%2 Ready:   ").arg(file_num).arg(sample_num));
                return;
            }

            fileTableList_.push_back(fileTable);
            if(!sampleSet_.contains(fileTable.sampleName))
            {
                sampleNameList_.push_back(fileTable.sampleName);
                sampleSet_.insert(fileTable.sampleName);
            }
            if(flag)
            {
                signalInfo_->sampleName = fileTable.sampleName;
                signalInfo_->fileName = fileTable.fileName;
                flag = false;
            }
        }
        else
        {
            GsspFileTable gsspFileTable;
            gsspFileTable.gsspName = box->currentText().toAscii();
            gsspFileTable.sampleName = tableWidget_->item(i, 0)->text().toAscii();
            box = (QComboBoxNew*)tableWidget_->cellWidget(i, 1);
            gsspFileTable.geneName = box->currentText().toAscii();
            box = (QComboBoxNew*)tableWidget_->cellWidget(i, 3);
            gsspFileTable.exonIndex = box->currentText().toInt();
            box = (QComboBoxNew*)tableWidget_->cellWidget(i, 4);
            gsspFileTable.rOrF = box->currentText().toAscii();
            gsspFileTable.fileName = tableWidget_->item(i, 5)->text().toAscii();
            gsspFileTable.filePath = tableWidget_->item(i, 6)->text();
            if(gsspFileTable.sampleName.isEmpty()||gsspFileTable.geneName.isEmpty()||gsspFileTable.rOrF.isEmpty()
                    ||gsspFileTable.exonIndex<1 || gsspFileTable.exonIndex>8)
            {
                tableWidget_->selectRow(i);
                tableWidget_->verticalScrollBar()->setSliderPosition(i);
                QMessageBox::warning(this, tr("SoapTyping"),QString("File: %1\nIncomplete information at line %2").arg(QString(gsspFileTable.fileName)).arg(i+1));
                analysisButton_->setEnabled(true);
                checkBox_->setEnabled(true);
                removeCheckedButton_->setEnabled(true);
                openFileButton_->setEnabled(true);
                cancelButton_->setEnabled(true);
                button_->setText(QString("F:%1/S:%2 Ready:   ").arg(file_num).arg(sample_num));
                return;
            }
            gsspFileTableList_.push_back(gsspFileTable);
            if(!sampleSet_.contains(gsspFileTable.sampleName))
            {
                sampleNameList_.push_back(gsspFileTable.sampleName);
                sampleSet_.insert(gsspFileTable.sampleName);
            }
            if(flag)
            {
                signalInfo_->sampleName = gsspFileTable.sampleName;
                signalInfo_->fileName = gsspFileTable.fileName;
                flag = false;
            }
        }
    }

    *done_ = true;
    range_ = fileTableList_.size()+gsspFileTableList_.size()+sampleNameList_.size();
    if(range_ > 0)
    {
        progressBar_->setRange(0, range_);
    }
    if(sampleNameList_.size()>1)
    {
        changeThreadNum(1);
    }
    if(fileTableList_.size() > 0)
    {
        connect(threadpool, SIGNAL(resultReadyAt(int)), this, SLOT(slotProgressBarSetValue(int)));
        connect(threadpool, SIGNAL(finished(bool)), this, SLOT(slotFileTableWatcherFinished(bool)));
        analysisFileTable(fileTableList_);
    }
    else
    {
        slotFileTableWatcherFinished(false);
    }
}

void OpenFileDlg::analysisFileTable(QList<FileTable> &fileTableList)
{
    //qDebug()<<"file start  "<<fileTableList.count();
    threadpool->mapped(fileTableList,get_info_from_ab1);
    //线程入口
    //fileTableWatcher_->setFuture(QtConcurrent::mapped(fileTableList.begin(),fileTableList.end(), get_info_from_ab1));
}

void OpenFileDlg::analysisGsspFileTable(QList<GsspFileTable> &gsspFileTableList)
{
    //qDebug()<<"assp start"<<gsspFileTableList.count();
    threadpool->mapped(gsspFileTableList,get_info_from_gssp);
    // gsspFileTableWatcher_->setFuture(QtConcurrent::mapped(gsspFileTableList.begin(), gsspFileTableList.end(), get_info_from_gssp));
}

void OpenFileDlg::analysisSampleTable(QList<QByteArray> &sampleList)
{
    //qDebug()<<"sample start"<<sampleList.count();
    threadpool->mapped(sampleList,analysisSample);
    //sampleTableWatcher_->setFuture(QtConcurrent::mapped(sampleList.begin(), sampleList.end(), analysisSample));
}

void OpenFileDlg::slotProgressBarSetValue(int i)
{
//    qDebug()<<i;
    progressBar_->setValue(++value_);
}

void OpenFileDlg::slotFileTableWatcherFinished(bool doit)
{
    if(doit)
    {
        // qDebug()<<"file finished";
        disconnect(threadpool, SIGNAL(resultReadyAt(int)), this, SLOT(slotProgressBarSetValue(int)));
        disconnect(threadpool, SIGNAL(finished(bool)), this, SLOT(slotFileTableWatcherFinished(bool)));
        // qDebug()<<"file done";
    }

    if(gsspFileTableList_.size()>0)
    {
        connect(threadpool, SIGNAL(resultReadyAt(int)), this, SLOT(slotProgressBarSetValue(int)));
        connect(threadpool, SIGNAL(finished(bool)), this, SLOT(slotGsspFileTableWatcherFinished(bool)));
        analysisGsspFileTable(gsspFileTableList_);
    }
    else
    {
        slotGsspFileTableWatcherFinished(false);
    }
}

void OpenFileDlg::slotGsspFileTableWatcherFinished( bool doit)
{
    if(doit)
    {
        //qDebug()<<"Gssp finished";
        disconnect(threadpool, SIGNAL(resultReadyAt(int)), this, SLOT(slotProgressBarSetValue(int)));
        disconnect(threadpool, SIGNAL(finished(bool)), this, SLOT(slotGsspFileTableWatcherFinished(bool)));
        //qDebug()<<"gssp done";
    }

    if(sampleNameList_.size()>0)
    {
        connect(threadpool, SIGNAL(resultReadyAt(int)), this, SLOT(slotProgressBarSetValue(int)));
        connect(threadpool, SIGNAL(finished(bool)), this, SLOT(slotSampleTableWathcerFinished(bool)));
        analysisSampleTable(sampleNameList_);
    }
    else
    {
        slotSampleTableWathcerFinished(false);
    }
}

void OpenFileDlg::slotSampleTableWathcerFinished(bool doit)
{
    if(doit)
    {
        //qDebug()<<"Sample finished";
        disconnect(threadpool, SIGNAL(resultReadyAt(int)), this, SLOT(slotProgressBarSetValue(int)));
        disconnect(threadpool, SIGNAL(finished(bool)), this, SLOT(slotSampleTableWathcerFinished(bool)));
    }
    //        showFileTable();
    //        showGsspFileTable();
    //        showSampleTable();

    changeThreadNum(getAnalysisThreadNum());
    close();
}

void OpenFileDlg::slotGetNumbers()
{
    file_num = tableWidget_->rowCount();
    QSet<QString> name;
    for(int i=0; i<tableWidget_->rowCount(); i++)
    {
        name.insert(tableWidget_->item(i,0)->text());

    }
    sample_num = name.size();
    button_->setText(QString("F:%1/S:%2 Ready:   ").arg(file_num).arg(sample_num));
}
