#include "mainwindow.h"
#include "statusbarwidget.h"
#include "savefiledlg.h"
#include "loadfiledlg.h"
#include "deletefiledlg.h"
#include "reportdlg.h"
#include "setdlg.h"
#include "exontrimdlg.h"
#include "updatedatabasedlg.h"
//#include "windowsmodernstyle.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("SoapTyping"));
    setWindowIcon(QIcon(":/images/logo.png"));
//    setStyleSheet("QToolBar{background-color:#cedcc5;border: 0px solid grey;height:140px};QToolBar::QAction:hover{color:blue};");
    has_database = false;
    createDefault();


    //setIconSize();

    sampleTree = new SampleTreeWidget(this);
    matchListTable = new MatchListWidget(this);
    exonNavigator = new ExonNavigator(this);
    baseAlignTable = new BaseAlignTable(this);
    main_mscillogram = new Main_Oscillogram();

    exonNavigator->resize(100,100);
    baseAlignTable->resize(100,100);

    align=new AlignmentDialog();
    QSplitter *leftSplitter = new QSplitter(Qt::Vertical, this);
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical, this);
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);

    leftSplitter->addWidget(exonNavigator);
    leftSplitter->addWidget(baseAlignTable);

    leftSplitter->setStretchFactor(0,1);
    leftSplitter->setStretchFactor(1,15);//9
    leftSplitter->addWidget(main_mscillogram);
    leftSplitter->setStretchFactor(2,60);//70

    rightSplitter->addWidget(sampleTree);
    rightSplitter->addWidget(matchListTable);
    rightSplitter->setStretchFactor(0,1);
    rightSplitter->setStretchFactor(1,2);

    mainSplitter->addWidget(leftSplitter);
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setStretchFactor(0,75);
    mainSplitter->setStretchFactor(1,25);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *vBoxLayout = new QVBoxLayout(centralWidget);
    vBoxLayout->setSpacing(6);
    vBoxLayout->setContentsMargins(8,8,8,8);
    vBoxLayout->addWidget(mainSplitter);
    setCentralWidget(centralWidget);

    connect(actionOpenNewFile, SIGNAL(triggered()), this, SLOT(slotOpen()));
    connect(actionSaveAll, SIGNAL(triggered()), this, SLOT(slotSave()));
    connect(actionDeleteAll, SIGNAL(triggered()), this, SLOT(slotDelete()));
    connect(actionOpenOldFile, SIGNAL(triggered()), this, SLOT(slotLoadFile()));
    connect(actionReport, SIGNAL(triggered()), this, SLOT(slotReport()));
    connect(actionAllelePairAlign, SIGNAL(triggered()), this, SLOT(slotAlignPair()));
    connect(actionLabAlleleAlign, SIGNAL(triggered()), this, SLOT(slotAlignLab()));
    connect(actionMarkSuccessfulAll, SIGNAL(triggered()), this, SLOT(slotMarkAllSampleApproved()));
    connect(actionMarkFinishedAll, SIGNAL(triggered()), SLOT(slotMarkAllSampleReviewed()));
    connect(actionReset, SIGNAL(triggered()), this, SLOT(slotReset()));
    connect(actionForward,SIGNAL(triggered()), exonNavigator, SLOT(slotMisPosForward()));
    connect(actionBackward, SIGNAL(triggered()), exonNavigator, SLOT(slotMisPosBackward()));
    connect(actionControl, SIGNAL(triggered()), this, SLOT(slotControl()));
    connect(actionExonTrim, SIGNAL(triggered()), this, SLOT(slotSetExonTrim()));
    connect(actionUpdateDatabase, SIGNAL(triggered()), this, SLOT(slotUpdateDatabase()));
    connect(actionDocument, SIGNAL(triggered()), this, SLOT(slotDocument()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

    connect(this, SIGNAL(signalAllelePair(QString&,QString&)), baseAlignTable, SLOT(slotAllelePairChanged(QString&,QString&)));


    connect(this, SIGNAL(signalFileChanged(SignalInfo&,int)), sampleTree, SLOT(slotFileChanged(SignalInfo&,int)));
    connect(sampleTree, SIGNAL(signalFileChnaged(SignalInfo&,int)),main_mscillogram,SLOT(slotRegionShowDataChanged(SignalInfo&,int)));
    connect(matchListTable, SIGNAL(signalFileChanged(SignalInfo&,int)), sampleTree, SLOT(slotFileChanged(SignalInfo&,int)));
    connect(sampleTree, SIGNAL(signalFileChnaged(SignalInfo&,int)), matchListTable, SLOT(slotFileChanged(SignalInfo&,int)));
    connect(sampleTree, SIGNAL(signalFileChnaged(SignalInfo&,int)), exonNavigator, SLOT(slotFileChanged(SignalInfo&,int)));
    connect(sampleTree, SIGNAL(signalFileChnaged(SignalInfo&,int)), baseAlignTable, SLOT(slotFileChanged(SignalInfo&,int)));
    connect(sampleTree, SIGNAL(signalFileChnaged(SignalInfo&,int)), this, SLOT(slotFileChanged(SignalInfo&,int)));

    connect(matchListTable, SIGNAL(signalIndelPostion(int)), exonNavigator, SLOT(slotIndelPosition(int)));
    connect(matchListTable, SIGNAL(signalAllelePair(QString&,QString&)), baseAlignTable, SLOT(slotAllelePairChanged(QString&,QString&)));
    connect(exonNavigator, SIGNAL(signalFocusPosition(int,int,int)), baseAlignTable, SLOT(slotFocusPosition(int,int,int)));

    connect(baseAlignTable, SIGNAL(signalFocusPosition(int,int,int)), exonNavigator, SLOT(slotFocusPosition(int,int,int)));
    connect(baseAlignTable, SIGNAL(signalTypeMisMatchPosition(QSet<int>&, int)), exonNavigator, SLOT(slotTypeMisMatchPostionChanged(QSet<int>&, int)));



    connect(actionApplyOne,SIGNAL(triggered()),main_mscillogram,SLOT(slotaddordecreaseOne()));
    connect(actionApplyOne,SIGNAL(triggered()),this,SLOT(slotApplyOne()));
    connect(actionApplyAll,SIGNAL(triggered()),main_mscillogram,SLOT(slotaddordecreaseAll()));
    connect(actionApplyAll,SIGNAL(triggered()),this,SLOT(slotApplyAll()));
    connect(analyseLater,SIGNAL(triggered()),this,SLOT(slotAnalyseLater()));
    connect(analyseNow,SIGNAL(triggered()),this,SLOT(slotAnalyseNow()));
    connect(analyseLater,SIGNAL(triggered()),main_mscillogram,SLOT(slotanalyseLater()));
    connect(analyseNow,SIGNAL(triggered()),main_mscillogram,SLOT(slotanalyseNow()));
    connect(analyse,SIGNAL(triggered()),main_mscillogram,SLOT(slotanalyse()));
    connect(analyse,SIGNAL(triggered()),this,SLOT(slotanalyse()));
    connect(main_mscillogram,SIGNAL(signalchangedbp()),this,SLOT(slotchengebp()));

    connect(exonNavigator, SIGNAL(signalFocusPosition(int,int,int)), main_mscillogram, SLOT(slotFocusMark(int,int,int)));
    connect(baseAlignTable, SIGNAL(signalFocusPosition(int,int,int)), main_mscillogram, SLOT(slotFocusMark(int,int,int)));

    connect(main_mscillogram, SIGNAL(signal_toinform_other(int,int,int)), exonNavigator, SLOT(slotFocusPosition(int,int,int)));
    connect(main_mscillogram, SIGNAL(signal_toinform_other(int,int,int)), baseAlignTable, SLOT(slotFocusPosition(int,int,int)));
    connect(main_mscillogram,SIGNAL(signalactApplyAll()),this,SLOT(slotApplyAll()));
    connect(main_mscillogram,SIGNAL(signalactApplyOne()),this,SLOT(slotApplyOne()));
    connect(main_mscillogram,SIGNAL(signalactanalyseNow()),this,SLOT(slotAnalyseNow()));
    connect(main_mscillogram,SIGNAL(signalactanalyseLater()),this,SLOT(slotAnalyseLater()));


    connect(main_mscillogram, SIGNAL(signalchangedbp(SignalInfo&,int)), sampleTree, SLOT(slotFileChanged(SignalInfo&,int)));
    connect(main_mscillogram, SIGNAL(signalchangedbp(SignalInfo&,int)), this, SLOT(slotFileChanged(SignalInfo&,int)));

    connect(actionZoomYAxisIncrease, SIGNAL(triggered()), main_mscillogram, SLOT(slotyRangeRoomUp()));
    connect(actionZoomYAxisReduce, SIGNAL(triggered()), main_mscillogram, SLOT(slotyRangeRoomDown()));
    connect(actionZoomYIncrease,SIGNAL(triggered()),main_mscillogram, SLOT(slotyRoomUp()));
    connect(actionZoomYReduce,SIGNAL(triggered()),main_mscillogram,SLOT(slotyRoomDown()));
    connect(actionZoomXIncrease,SIGNAL(triggered()),main_mscillogram, SLOT(slotxRoomUp()));
    connect(actionZoomXReduce,SIGNAL(triggered()),main_mscillogram,SLOT(slotxRoomDown()));
    connect(actionZoomReset, SIGNAL(triggered()), main_mscillogram, SLOT(resetRoomSetting()));
//    connect(actionReset, SIGNAL(triggered()), main_mscillogram, SLOT(slotReset()));

}

//MainWindow::~MainWindow()
//{

//}
void MainWindow::setDatabaseInfo(bool has)
{
    has_database = has;
}

void MainWindow::createDefault()
{
    createActions();
    createMenus();
    createBars();

    menuFile->addAction(actionOpenNewFile);
    menuFile->addAction(actionOpenOldFile);
    menuFile->addSeparator();
    menuFile->addAction(actionSaveAll);
    menuFile->addSeparator();
    menuFile->addAction(actionDeleteAll);
    menuFile->addAction(actionReport);

    menuTool->addAction(actionReset);
    menuTool->addSeparator();
    menuTool->addAction(actionBackward);
    menuTool->addAction(actionForward);
    menuTool->addSeparator();
    menuTool->addAction(actionMarkSuccessfulAll);
    menuTool->addAction(actionMarkFinishedAll);
    menuTool->addSeparator();
    menuTool->addAction(actionAllelePairAlign);
    menuTool->addAction(actionLabAlleleAlign);
    menuTool->addAction(actionUpdateDatabase);
    menuTool->addSeparator();
//    menuTool->addAction(actionControl);
//    menuTool->addAction(actionExonTrim);
    menuTool->addAction(menuSet->menuAction());

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuTool->menuAction());
    menuBar->addAction(menuadd->menuAction());
    menuBar->addAction(menuHelp->menuAction());

    menuadd->addAction(actionApplyOne);
    menuadd->addAction(actionApplyAll);
    menuadd->addSeparator();
    menuadd->addAction(analyseLater);
    menuadd->addAction(analyseNow);
    menuadd->addSeparator();
    menuadd->addAction(analyse);

    menuHelp->addAction(actionAbout);
    menuHelp->addAction(actionDocument);

    toolBarFile->addAction(actionOpenNewFile);
    toolBarFile->addAction(actionOpenOldFile);
    toolBarFile->addAction(actionSaveAll);
    toolBarFile->addAction(actionDeleteAll);
    toolBarFile->addAction(actionReport);

    toolBarTool->addAction(actionReset);
    toolBarTool->addAction(actionBackward);
    toolBarTool->addAction(actionForward);
    toolBarTool->addAction(actionMarkSuccessfulAll);
    toolBarTool->addAction(actionMarkFinishedAll);
    toolBarTool->addAction(actionAllelePairAlign);
    toolBarTool->addAction(actionLabAlleleAlign);
    toolBarTool->addAction(actionUpdateDatabase);
//    toolBarTool->addAction(actionControl);
//    toolBarTool->addAction(actionExonTrim);
    toolBarTool->addAction(menuSet->menuAction());

    toolBarPeak->addAction(actionZoomYAxisIncrease);
    toolBarPeak->addAction(actionZoomYAxisReduce);
    toolBarPeak->addAction(actionZoomYIncrease);
    toolBarPeak->addAction(actionZoomYReduce);
    toolBarPeak->addAction(actionZoomXIncrease);
    toolBarPeak->addAction(actionZoomXReduce);
    toolBarPeak->addAction(actionZoomReset);

    this->setMenuBar(menuBar);
    this->addToolBar(toolBarFile);
    this->addToolBar(toolBarTool);
 //   this->insertToolBarBreak(toolToolBar);
    this->addToolBar(toolBarPeak);
  //  this->insertToolBarBreak(peakToolBar);
    this->setStatusBar(statusBar);
}

void MainWindow::createActions()
{
    actionOpenNewFile = new QAction(QIcon(":/images/opennewfile.png"), tr("Open"), this);
    actionOpenOldFile = new QAction(QIcon(":/images/openoldfile.png"), tr("Load"), this);
    actionSaveAll = new QAction(QIcon(":/images/saveall.png"), tr("Save"), this);
    actionDeleteAll = new QAction(QIcon(":/images/deleteall.png"), tr("Delete"), this);
    actionReport = new QAction(QIcon(":/images/report.png"), tr("Export"), this);

    actionReset = new QAction(QIcon(":/images/reset.png"), tr("Reset"), this);
    actionMarkSuccessfulAll = new QAction(QIcon(":/images/marksuccessfulall.png"), tr("Approval"), this);
    actionMarkFinishedAll = new QAction(QIcon(":/images/markfinishedall.png"), tr("Review"), this);
    actionBackward = new QAction(QIcon(":/images/backward.png"), tr("Previous Mismatch Position"), this);
    actionForward = new QAction(QIcon(":/images/forward.png"), tr("Next Mismatch Position"), this);
    actionAllelePairAlign = new QAction(QIcon(":/images/alignpair.png"), tr("Allele Comparator"), this);
    actionLabAlleleAlign = new QAction(QIcon(":/images/alignlab.png"), tr("Allele Alignment"), this);

    actionShowGSSPZCode = new QAction(tr("Show GSSP Z Code"), this);
    actionShowUndefinedExon = new QAction(tr("Show Undefined Exon"), this);

    actionApplyOne=new QAction(QIcon(":/images/apply.png"),"Apply One",this);
    actionApplyOne->setIconVisibleInMenu(false);
    actionApplyAll=new QAction(QIcon(":/images/apply.png"),"Apply All",this);
    actionApplyAll->setIconVisibleInMenu(true);
    analyseLater=new QAction(QIcon(":/images/apply.png"),"Edit Multi",this);
    analyseLater->setIconVisibleInMenu(false);
    analyseLater->setShortcut(QKeySequence(Qt::ALT+Qt::Key_A));
    analyseNow=new QAction(QIcon(":/images/apply.png"),"Edit One",this);
    analyseNow->setIconVisibleInMenu(true);
    analyseNow->setShortcut(QKeySequence(Qt::ALT+Qt::Key_S));
    analyse=new QAction("Analyze",this);
    analyse->setEnabled(false);
//    analyse->setShortcut(QKeySequence(Qt::CTRL+Qt::ALT+Qt::Key_D));
    analyse->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));

    actionZoomYAxisIncrease = new QAction(QIcon(":/images/zoomyaxisincrease.png"), tr("Y Range Zoom (+)"), this);
    actionZoomYAxisReduce = new QAction(QIcon(":/images/zoomyaxisreduce.png"), tr("Y Range Zoom (-)"), this);
    actionZoomYIncrease = new QAction(QIcon(":/images/zoomyincrease.png"), tr("Y Zoom (+)"), this);
    actionZoomYReduce = new QAction(QIcon(":/images/zoomyreduce.png"), tr("Y Zoom (-)"), this);
    actionZoomXIncrease = new QAction(QIcon(":/images/zoomxincrease.png"), tr("X Zoom (+)"), this);
    actionZoomXReduce = new QAction(QIcon(":/images/zoomxreduce.png"), tr("X Zoom (-)"), this);
    actionZoomReset = new QAction(QIcon(":/images/zoomreset.png"), tr("Reset Zoom Setting"), this);

    actionOpenNewFile->setShortcut(tr("Ctrl+O"));
    actionOpenOldFile->setShortcut(tr("Ctrl+L"));
    actionSaveAll->setShortcut((tr("Ctrl+S")));
    actionDeleteAll->setShortcut((tr("Ctrl+D")));
    actionReset->setShortcut(tr("Ctrl+R"));
    actionBackward->setShortcut(tr("Left"));
    actionForward->setShortcut(tr("Right"));
    actionReport->setShortcut(tr("Ctrl+E"));

    actionControl = new QAction(tr("Set Thread"), this);
    actionExonTrim = new QAction(tr("Set Exon Trim"), this);
    menuSet = new QMenu(tr("Setting"),this);
    menuSet->setIcon(QIcon(":/images/control.png"));
    menuSet->addAction(actionControl);
    menuSet->addAction(actionExonTrim);

    actionUpdateDatabase = new QAction(QIcon(":/images/updatedatabase.png"), tr("Update Database"), this);
    actionAbout = new QAction(tr("About"), this);
    actionDocument = new QAction(tr("Help Documents"), this);
}

void MainWindow::createMenus()
{
    menuFile = new QMenu(tr("File"), this);
    menuTool = new QMenu(tr("Tool"), this);
    menuadd=new QMenu(tr("Zoom"),this);
    menuFileTree = new QMenu(this);
    menuTypeTable = new QMenu(this);
    menuRegionShow = new QMenu(this);

    menuHelp = new QMenu(tr("Help"),this);
}

void MainWindow::createBars()
{
    menuBar = new QMenuBar(this);
//    menuBar->setMinimumHeight(100);


    toolBarFile = new QToolBar(tr("File Tool Bar"), this);
    toolBarTool = new QToolBar(tr("Tool Tool Bar"), this);
    toolBarPeak = new QToolBar(tr("Peak Tool Bar"), this);

    QSize size(32,25);
    toolBarFile->setIconSize(size);
    toolBarTool->setIconSize(size);
    toolBarPeak->setIconSize(size);

    statusBar = new QStatusBar(this);
    StatusBarWgt *atgcn = new StatusBarWgt(statusBar);
    statusBar->setContentsMargins(6,0,0,0);
    statusBar->addWidget(atgcn);
}

void MainWindow::slotOpen()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    OpenFileDlg openFileDlg;
    bool isOpen = false;
    SignalInfo signalInfo;
    openFileDlg.setData(&isOpen, &signalInfo);
    openFileDlg.exec();
    if(isOpen)
    {
//        if(signalInfo_.sampleName.isEmpty())
//        {
//            signalInfo_ = signalInfo;
//        }
        emit signalFileChanged(signalInfo_, 1);
    }

}
void MainWindow::slotSave()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    SaveFileDlg save;
    save.exec();
}
void MainWindow::slotDelete()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    bool isDone = false;
    DeleteFileDlg deleteFileDlg(&isDone, 0);
    deleteFileDlg.exec();
    if(isDone)
    {
        signalFileChanged(signalInfo_, 1);
    }
}

void MainWindow::slotLoadFile()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    bool isDone = false;
    LoadFileDlg load(&isDone, 0);
    load.exec();
    if(isDone)
    {
        emit signalFileChanged(signalInfo_, 1);
    }
}

void MainWindow::slotReport()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    ReportDlg report;
    report.exec();
//    QString fileName = QFileDialog::getSaveFileName(this);
//    //qDebug()<<fileName;
//    printReport(fileName);
}

void MainWindow::slotFileChanged(SignalInfo &signalInfo, int type)
{
    signalInfo_ = signalInfo;
}

void MainWindow::slotAlignPair()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    AllelePairDlg align;
    QString allele1, allele2;
    align.setData(signalInfo_.geneName, &allele1, &allele2);
    align.exec();
    if(!allele1.isEmpty()&&!allele2.isEmpty())
    {
        emit signalAllelePair(allele1, allele2);
    }
}

void MainWindow::slotAlignLab()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    align->show();
}

void MainWindow::slotMarkAllSampleApproved()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    QMessageBox informationBox;
    informationBox.setWindowTitle(tr("Soap Typing"));
    informationBox.setIcon(QMessageBox::Information);
    informationBox.setText(tr("Would you really like to Mark all samples as approved"));
    informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    switch(informationBox.exec())
    {
    case QMessageBox::Yes:
        break;
    case QMessageBox::No:
        return;
    default:
        return;
    }

    markAllSampleApproved();
    emit signalFileChanged(signalInfo_, 1);
}

void MainWindow::slotMarkAllSampleReviewed()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    QMessageBox informationBox;
    informationBox.setWindowTitle(tr("Soap Typing"));
    informationBox.setIcon(QMessageBox::Information);
    informationBox.setText(tr("Would you really like to Marked all samples as reviewed?"));
    informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    switch(informationBox.exec())
    {
    case QMessageBox::Yes:
        break;
    case QMessageBox::No:
        return;
    default:
        return;
    }
    int t=markAllSampleReviewed();
    if(t==1)
    {
        QMessageBox::warning(this, tr("Soap Typing"),tr("Please unlock the sample witch were marked approved"));
    }
    emit signalFileChanged(signalInfo_, 1);
}

void MainWindow::slotReset()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    if(signalInfo_.sampleName.isEmpty())
    {
        return;
    }
    int isApproved = getMarkTypeBySampleName(signalInfo_.sampleName.toAscii());
    if(isApproved == APPROVED || isApproved == -1)
    {
        QMessageBox::warning(this, tr("Soap Typing"),tr("Please unlock the sample as it was marked approved"));
        return;
    }
    QString info;
    info.append(QString("Exon %1:%2\n").arg(signalInfo_.exonIndex).arg(signalInfo_.rOrF));
    if(signalInfo_.isGssp)
    {
        info.append("Is Gssp file\n");
    }
    info.append(QString("File: %1\n").arg(signalInfo_.fileName));
    info.append(QString("Would you like to reset this file?"));
    QMessageBox informationBox;
    informationBox.setWindowTitle(tr("SoapTyping"));
    informationBox.setIcon(QMessageBox::Information);
    informationBox.setText(info);
    informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    switch(informationBox.exec())
    {
    case QMessageBox::Yes:
        if(signalInfo_.isGssp)
        {
            resetGsspFileByFileName(signalInfo_.fileName.toAscii());
        }
        else
        {
            resetFileByFileName(signalInfo_.fileName.toAscii());
        }
        analysisSample(signalInfo_.sampleName.toAscii());
        emit signalFileChanged(signalInfo_, 1);
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}



void MainWindow::slotApplyAll()
{
    actionApplyAll->setIconVisibleInMenu(true);
    actionApplyOne->setIconVisibleInMenu(false);
}

void MainWindow::slotApplyOne()
{
    actionApplyAll->setIconVisibleInMenu(false);
    actionApplyOne->setIconVisibleInMenu(true);
}

void MainWindow::slotControl()
{
    SetDlg set;
    set.exec();
    matchListTable->setConfig();
    matchListTable->slotFileChanged(signalInfo_, 1);
}

void MainWindow::slotSetExonTrim()
{
    if(!has_database)
    {
        QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        return;
    }

    ExonTimDlg exonTrimDlg;
    exonTrimDlg.exec();
}

void MainWindow::slotUpdateDatabase()
{
    UpdateDlg updateDlg;
    updateDlg.exec();
}

void MainWindow::slotDocument()
{
    QFileInfo info("Documents/Help.pdf");
    if(info.exists())
    {
        bool ok=QDesktopServices::openUrl(QUrl(info.absoluteFilePath(), QUrl::TolerantMode));
        if(!ok)
        {
            QMessageBox::warning(this, "SoapTyping", QString("Can't open %1").arg(info.absoluteFilePath()));
        }
    }
    else
    {
        QMessageBox::warning(this, "SoapTyping", "Documents are missing!");
    }
}

void MainWindow::slotAbout()
{
    QMessageBox message(QMessageBox::NoIcon, "About SoapTyping Software", QString("SoapTyping V%1\nCopyright (C) 2012-2013 BGI").arg(VERSION));
    message.setIconPixmap(QPixmap(":/images/about.png"));
    message.exec();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox informationBox;
    informationBox.setIcon(QMessageBox::Warning);
    informationBox.setText("Would you really like to quit?");
    informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    bool cls = true;
    switch(informationBox.exec())
    {
    case QMessageBox::Yes:
        cls = true;
        break;
    case QMessageBox::No:
        cls = false;
        break;
    default:
        cls = false;
        break;
    }
    if(!cls)
        e->ignore();
}
