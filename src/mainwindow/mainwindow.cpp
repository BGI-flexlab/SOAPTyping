#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include "Dialog/openfiledialog.h"
#include <QSplitter>
#include "sampletreewidget.h"
#include "matchlistwidget.h"
#include <QScrollArea>
#include "multipeakwidget.h"
#include "exonnavigatorwidget.h"
#include "basealigntablewidget.h"
#include <QtDebug>
#include <QScrollBar>
#include <QTime>
#include "Dialog/savefiledlg.h"
#include "Dialog/loadfiledlg.h"
#include "Dialog/deletefiledlg.h"
#include "Dialog/reportdlg.h"
#include "DataBase/soaptypingdb.h"
#include <QMessageBox>
#include "ThreadTask/analysissamplethreadtask.h"
#include "Dialog/allelepairdlg.h"
#include "Dialog/setdlg.h"
#include "Dialog/exontimdlg.h"
#include "Dialog/alignmentdlg.h"
#include <QFileInfo>
#include <QDesktopServices>
#include "log/log.h"

QT_CHARTS_USE_NAMESPACE

const QString VERSION("1.0.6.0");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
    ConnectSignalandSlot();
    SetStatusbar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUI()
{
    QSplitter *leftSplitter = new QSplitter(Qt::Vertical);
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical);
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);

    m_pExonNavigatorWidget = new ExonNavigatorWidget;
    m_pExonNavigatorWidget->setMinimumHeight(100);
    m_pExonNavigatorWidget->setMaximumHeight(100);

    m_pBaseAlignTableWidget = new BaseAlignTableWidget;
    m_pBaseAlignTableWidget->setMinimumHeight(220);

    m_pPeak_area = new QScrollArea;
    m_pMultiPeakWidget = new MultiPeakWidget();
    m_pPeak_area->setWidget(m_pMultiPeakWidget);
    m_pMultiPeakWidget->setMinimumHeight(400);

    m_pSampleTreeWidget = new SampleTreeWidget;
    m_pSampleTreeWidget->setMinimumSize(400,320);

    m_pMatchListWidget = new MatchListWidget;

    leftSplitter->addWidget(m_pExonNavigatorWidget);
    leftSplitter->addWidget(m_pBaseAlignTableWidget);
    leftSplitter->addWidget(m_pPeak_area);
    leftSplitter->setStretchFactor(0,10);
    leftSplitter->setStretchFactor(1,22);
    leftSplitter->setStretchFactor(2,68);

    rightSplitter->addWidget(m_pSampleTreeWidget);
    rightSplitter->addWidget(m_pMatchListWidget);
    rightSplitter->setStretchFactor(0,50);
    rightSplitter->setStretchFactor(1,50);

    mainSplitter->addWidget(leftSplitter);
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setStretchFactor(0,75);
    mainSplitter->setStretchFactor(1,25);

    ui->verticalLayout->addWidget(mainSplitter);
    ui->actionAnalyze->setEnabled(false);
    ui->statusbarleft->setStyleSheet("QLabel{border:1px solid rgb(139,139,139);}");
    ui->statusbarright->setStyleSheet("QLabel{border:1px solid rgb(139,139,139);}");

    ui->actionApply_All->setIconVisibleInMenu(false);
    ui->actionApply_One->setIconVisibleInMenu(false);
    ui->actionEdit_Multi->setIconVisibleInMenu(false);
    ui->actionEdit_One->setIconVisibleInMenu(false);
}

void MainWindow::ConnectSignalandSlot()
{
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::slotShowOpenDlg);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::slotShowLoadFileDlg);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::slotShowSaveDlg);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::slotShowDeleteDlg);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::slotShowExportDlg);

    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::slotReset);
    connect(ui->actionForward, &QAction::triggered, this, &MainWindow::slotMisPosForward);
    connect(ui->actionBackward, &QAction::triggered, this, &MainWindow::slotMisPosBackward);
    connect(ui->actionApproval, &QAction::triggered, this, &MainWindow::slotMarkAllSampleApproved);
    connect(ui->actionReview, &QAction::triggered, this, &MainWindow::slotMarkAllSampleReviewed);
    connect(ui->actionAllele_Comparator, &QAction::triggered, this, &MainWindow::slotAlignPair);
    connect(ui->actionAllele_Alignment, &QAction::triggered, this, &MainWindow::slotAlignLab);
    connect(ui->actionUpdate_Database, &QAction::triggered, this, &MainWindow::slotUpdateDatabase);
    connect(ui->actionSet_Thread, &QAction::triggered, this, &MainWindow::slotControl);
    connect(ui->actionSet_Exon_Trim, &QAction::triggered, this, &MainWindow::slotSetExonTrim);

    connect(ui->actionY_Range_Zoom_Increase, &QAction::triggered, this, &MainWindow::slotyRangeRoomUp);
    connect(ui->actionY_Range_Zoom_Reduce, &QAction::triggered, this, &MainWindow::slotyRangeRoomDown);
    connect(ui->actionY_Zoom_Increase, &QAction::triggered, this, &MainWindow::slotyRoomUp);
    connect(ui->actionY_Zoom_Reduce, &QAction::triggered, this, &MainWindow::slotyRoomDown);
    connect(ui->actionX_Zoom_Increase, &QAction::triggered, this, &MainWindow::slotxRoomUp);
    connect(ui->actionX_Zoom_Reduce, &QAction::triggered, this, &MainWindow::slotxRoomDown);
    connect(ui->actionReset_Zoom_Setting, &QAction::triggered, this, &MainWindow::slotResetRoom);


    connect(ui->actionApply_All, &QAction::triggered, this, &MainWindow::slotApplyAll);
    connect(ui->actionApply_All, &QAction::triggered, m_pMultiPeakWidget, &MultiPeakWidget::slotApplyAll);
    connect(ui->actionApply_One, &QAction::triggered, this, &MainWindow::slotApplyOne);
    connect(ui->actionApply_One, &QAction::triggered, m_pMultiPeakWidget, &MultiPeakWidget::slotApplyOne);
    connect(ui->actionEdit_Multi, &QAction::triggered, this, &MainWindow::slotAnalyseLater);
    connect(ui->actionEdit_Multi, &QAction::triggered, m_pMultiPeakWidget, &MultiPeakWidget::slotAnalyseLater);
    connect(ui->actionEdit_One, &QAction::triggered, this, &MainWindow::slotAnalyseNow);
    connect(ui->actionEdit_One, &QAction::triggered, m_pMultiPeakWidget, &MultiPeakWidget::slotAnalyseNow);
    connect(ui->actionAnalyze, &QAction::triggered, this, &MainWindow::slotanalyse);
    connect(m_pMultiPeakWidget, &MultiPeakWidget::signalPeakAct, this, &MainWindow::slotPeakAct);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::slotAbout);
    connect(ui->actionHelp_Documents, &QAction::triggered, this, &MainWindow::slotHelp);


    connect(m_pSampleTreeWidget, &QTreeWidget::itemClicked, this, &MainWindow::slotSampleTreeItemChanged);

    connect(m_pExonNavigatorWidget, &ExonNavigatorWidget::signalExonFocusPosition,
            this, &MainWindow::slotExonFocusPosition);

    connect(m_pBaseAlignTableWidget, &QTableWidget::itemClicked, this, &MainWindow::slotAlignTableFocusPosition);

    connect(m_pMultiPeakWidget, &MultiPeakWidget::signalPeakFocusPosition, this, &MainWindow::slotPeakFocusPosition);



    connect(m_pMatchListWidget, &MatchListWidget::signalAllelePair, this, &MainWindow::slotAllelePairChanged);

    connect(m_pBaseAlignTableWidget, &BaseAlignTableWidget::signalTypeMisMatchPosition,
            this, &MainWindow::slotTypeMisMatchPostion);

    connect(m_pMultiPeakWidget, &MultiPeakWidget::signalSendStatusBarMsg, this , &MainWindow::slotShowStatusBarMsg);

    connect(m_pMultiPeakWidget, &MultiPeakWidget::signalChangeDB, this, &MainWindow::slotChangeDB);
    connect(m_pMultiPeakWidget, &MultiPeakWidget::signalChangeDBByFile, this, &MainWindow::slotChangeDBByFile);

    connect(m_pSampleTreeWidget, &SampleTreeWidget::signalChangeDBByFile, this, &MainWindow::slotChangeDBByFile);

    connect(m_pMatchListWidget, &MatchListWidget::signalChangeDB, this, &MainWindow::slotChangeDB);

    connect(m_pSampleTreeWidget, &SampleTreeWidget::signalClearAll, this, &MainWindow::slotClearAll);
}

void MainWindow::DisConnectSignalandSolt()
{

}

void MainWindow::InitData()
{
    m_pSampleTreeWidget->SetTreeData();
}

void MainWindow::slotShowOpenDlg()
{
    OpenFileDialog dlg(this);
    dlg.exec();

    m_pMatchListWidget->SetRefresh(true);
    m_pExonNavigatorWidget->SetRefresh(true);
    m_pBaseAlignTableWidget->SetRefresh(true);
    m_pMultiPeakWidget->SetRefresh(true);
    m_pSampleTreeWidget->SetTreeData();
}

void MainWindow::SetStatusbar()
{
    QString strA("<b><font color='#39B54A' size='5'>A</font></b>");
    QString strC("<b><font color='#0000ff' size='5'>C</font></b>");
    QString strG("<b><font color='#000000' size='5'>G</font></b>");
    QString strT("<b><font color='#ff0000' size='5'>T</font></b>");

    QString strR("<b><font color='#C1272d' size='5'>R-</font></b>");
    QString strY("<b><font color='#C1272d' size='5'>Y-</font></b>");
    QString strK("<b><font color='#C1272d' size='5'>K-</font></b>");
    QString strM("<b><font color='#C1272d' size='5'>M-</font></b>");
    QString strS("<b><font color='#C1272d' size='5'>S-</font></b>");
    QString strW("<b><font color='#C1272d' size='5'>W-</font></b>");
    QString strB("<b><font color='#C1272d' size='5'>B-</font></b>");
    QString strD("<b><font color='#C1272d' size='5'>D-</font></b>");
    QString strH("<b><font color='#C1272d' size='5'>H-</font></b>");
    QString strV("<b><font color='#C1272d' size='5'>V-</font></b>");
    QString strspace("  ");

    QString str = strR + strA+ strG + strspace +
                  strY + strT + strC + strspace +
                  strK + strG + strT + strspace +
                  strM + strA + strC + strspace +
                  strS + strG + strC + strspace +
                  strW + strA + strT + strspace +
                  strB + strG + strT + strC + strspace +
                  strD + strG + strA + strT + strspace +
                  strH + strA + strC + strT + strspace +
                  strV + strG + strC + strA;
    ui->statusbarleft->setText(str);
}

void MainWindow::slotSampleTreeItemChanged(QTreeWidgetItem *item, int col)
{
    if(item == nullptr)
    {
        return;
    }

    QTreeWidgetItem *pParent = item->parent();
    if(pParent == nullptr)
    {
        return;
    }
    QString str_sample = pParent->text(0);
    QString str_gene  = pParent->text(1);

    QString strfile = item->text(0);
    QString str_info = item->text(1);
    LOG_DEBUG("%s",strfile.toStdString().c_str());
    m_pMatchListWidget->SetTableData(str_sample,strfile, str_info, col);

    if(!strfile.contains(".ab1"))//如果不是ab1文件，左侧的模块不用刷新
    {
        return;
    }

    m_pSelectItem = item;
    m_str_SelectFile = strfile;
    m_str_SelectSample = str_sample;

    m_pExonNavigatorWidget->SetExonData(str_sample, str_gene);

    m_pBaseAlignTableWidget->SetAlignTableData(str_sample,strfile, str_info, col);

    int index_exon = str_info.left(1).toInt();
    if(str_info.contains("Filter"))//如果是gssp文件
    {
        index_exon = item->data(0,Qt::UserRole).toInt();
    }

    m_pMultiPeakWidget->SetPeakData(str_sample,index_exon);


    int startpos;
    int selectpos;
    int exonstartpos;
    m_pExonNavigatorWidget->setSelectFramePosition(index_exon, startpos, selectpos, exonstartpos);
    LOG_DEBUG("%d %d %d %d",index_exon, startpos, selectpos, exonstartpos);

    int i_columnPos = selectpos - startpos;
    int sliderPos = i_columnPos*25-20;
    m_pBaseAlignTableWidget->selectColumn(i_columnPos);
    m_pBaseAlignTableWidget->horizontalScrollBar()->setSliderPosition(sliderPos);

    int i_sub = selectpos - exonstartpos;
    m_pMultiPeakWidget->SetSelectPos(i_sub);
}

//导航条起始pos,选中的峰图pos，选中的导航条起始pos,选中的导航条index
void MainWindow::slotExonFocusPosition(int startpos, int selectpos, int exonstartpos, int index)
{
    LOG_DEBUG("%d %d %d %d",startpos, selectpos, exonstartpos, index);
    int i_columnPos = selectpos - startpos; //二者之差为表格的第几列
    int sliderPos = i_columnPos*25+8;
    m_pBaseAlignTableWidget->horizontalScrollBar()->setSliderPosition(sliderPos);
    m_pBaseAlignTableWidget->selectColumn(i_columnPos+1);

    int i_sub = selectpos - exonstartpos;
    m_pSampleTreeWidget->SetSelectItem(index, m_str_SelectSample);
    m_pMultiPeakWidget->SetPeakData(m_str_SelectSample, index);
    m_pMultiPeakWidget->SetSelectPos(i_sub);
}

void MainWindow::slotAlignTableFocusPosition(QTableWidgetItem *item)
{
    int selectpos;
    int exonstartpos;
    int index;

    int i_colnum = item->column();
    QTableWidgetItem *item_first = item->tableWidget()->item(0,i_colnum);
    if(i_colnum < 1 || item_first->text().isEmpty())
    {
        return;
    }

    m_pExonNavigatorWidget->SetSelectPos(i_colnum, selectpos, exonstartpos ,index);
    LOG_DEBUG("%d %d %d %d",i_colnum, selectpos, exonstartpos ,index);

    int i_sub = selectpos - exonstartpos;
    m_pSampleTreeWidget->SetSelectItem(index, m_str_SelectSample);
    m_pMultiPeakWidget->SetPeakData(m_str_SelectSample, index);
    m_pMultiPeakWidget->SetSelectPos(i_sub);
}

void MainWindow::slotPeakFocusPosition(int index, int colnum)
{
    LOG_DEBUG("%d %d",index, colnum);
    int i_columnPos;
    m_pExonNavigatorWidget->SetSelectFramePos(index, colnum,i_columnPos);
    m_pBaseAlignTableWidget->selectColumn(i_columnPos+1);
    m_pBaseAlignTableWidget->horizontalScrollBar()->setSliderPosition(i_columnPos*25+8);
}


void MainWindow::slotShowSaveDlg()
{
    SaveFileDlg dlg(this);
    dlg.exec();
}


void MainWindow::slotShowLoadFileDlg()
{
    LoadFileDlg load(this);
    load.exec();
}

void MainWindow::slotShowDeleteDlg()
{
    DeleteFileDlg dlg(this);
    dlg.exec();
}

void MainWindow::slotShowExportDlg()
{
    ReportDlg report(this);
    report.setVersion(VERSION); //新增
    report.exec();
}

void MainWindow::slotReset()
{   
    QStringList str_list = m_str_SelectFile.split('_');

    int isApproved = SoapTypingDB::GetInstance()->getMarkTypeBySampleName(str_list.at(0));
    if(isApproved == APPROVED || isApproved == -1)
    {
        QMessageBox::warning(this, tr("Soap Typing"),tr("Please unlock the sample as it was marked approved"));
        return;
    }
    QString info;
    info.append(QString("Exon %1\n").arg(str_list.at(2)));
    bool bIsGssp = true;
    if(str_list.at(2).contains('R') || str_list.at(2).contains('F'))
    {
        bIsGssp = false;
    }
    else
    {
        info.append("Is Gssp file\n");
    }

    info.append(QString("File: %1\n").arg(m_str_SelectFile));
    info.append(QString("Would you like to reset this file?"));
    QMessageBox informationBox(this);
    informationBox.setWindowTitle(tr("SoapTyping"));
    informationBox.setIcon(QMessageBox::Information);
    informationBox.setText(info);
    informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    switch(informationBox.exec())
    {
    case QMessageBox::Yes:
    {
        SoapTypingDB::GetInstance()->resetFileByFileName(str_list.at(0), bIsGssp);
        slotChangeDB(str_list.at(0));
        break;
    }
    case QMessageBox::No:
        break;
    default:
        break;
    }
}

void MainWindow::slotMisPosForward()
{
    m_pExonNavigatorWidget->ActForward();
}

void MainWindow::slotMisPosBackward()
{
    m_pExonNavigatorWidget->ActBackward();
}

void MainWindow::slotMarkAllSampleApproved()
{
    QMessageBox informationBox(this);
    informationBox.setWindowTitle(tr("Soap Typing"));
    informationBox.setIcon(QMessageBox::Information);
    informationBox.setText(tr("Would you really like to Mark all samples as approved"));
    informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    switch(informationBox.exec())
    {
    case QMessageBox::Yes:
        SoapTypingDB::GetInstance()->markAllSampleApproved();
        m_pSampleTreeWidget->SetTreeData();
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}

void MainWindow::slotMarkAllSampleReviewed()
{
    QMessageBox informationBox(this);
    informationBox.setWindowTitle(tr("Soap Typing"));
    informationBox.setIcon(QMessageBox::Information);
    informationBox.setText(tr("Would you really like to Marked all samples as reviewed?"));
    informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    switch(informationBox.exec())
    {
    case QMessageBox::Yes:
    {
        int t = SoapTypingDB::GetInstance()->markAllSampleReviewed();
        if(t==1)
        {
            QMessageBox::warning(this, tr("Soap Typing"),tr("Please unlock the sample witch were marked approved"));
        }
        m_pSampleTreeWidget->SetTreeData();
        break;
    }
    case QMessageBox::No:
        return;
    default:
        return;
    }

}

void MainWindow::slotAlignPair()
{
    QStringList str_list = m_str_SelectFile.split('_');
    if(str_list.isEmpty())
    {
        return;
    }
    QStringList typeResultList = m_pMatchListWidget->GetMatchList();
    QStringList typeResult;

    AllelePairDlg align(this);
    align.SetData(str_list.at(1));
    align.exec();
    QString allele1, allele2;
    align.getSelectAllele(allele1,allele2);
    if(!allele1.isEmpty()&&!allele2.isEmpty())
    {
        slotAllelePairChanged(allele1, allele2);
    }
}

void MainWindow::slotAlignLab()
{
    AlignmentDlg dlg(this);
    dlg.exec();
}

void MainWindow::slotUpdateDatabase()
{

}

void MainWindow::slotControl()
{
    SetDlg dlg(this);
    dlg.exec();
}

void MainWindow::slotSetExonTrim()
{
    ExonTimDlg exonTrimDlg(this);
    exonTrimDlg.exec();
}

void MainWindow::slotyRangeRoomUp()
{
    m_pMultiPeakWidget->AdjustPeakHeight(20);
}

void MainWindow::slotyRangeRoomDown()
{
    m_pMultiPeakWidget->AdjustPeakHeight(-20);
}

void MainWindow::slotyRoomUp()
{
    m_pMultiPeakWidget->AdjustPeakY(1);
}

void MainWindow::slotyRoomDown()
{
    m_pMultiPeakWidget->AdjustPeakY(-1);
}

void MainWindow::slotxRoomUp()
{
    m_pMultiPeakWidget->AdjustPeakX(14);
}

void MainWindow::slotxRoomDown()
{
    m_pMultiPeakWidget->AdjustPeakX(-14);
}

void MainWindow::slotResetRoom()
{
    m_pMultiPeakWidget->RestorePeak();
}

void MainWindow::slotApplyOne()
{
    ui->actionApply_All->setIconVisibleInMenu(false);
    ui->actionApply_One->setIconVisibleInMenu(true);
}

void MainWindow::slotApplyAll()
{
    ui->actionApply_All->setIconVisibleInMenu(true);
    ui->actionApply_One->setIconVisibleInMenu(false);
}

void MainWindow::slotAnalyseLater()
{
    ui->actionEdit_Multi->setIconVisibleInMenu(true);
    ui->actionEdit_One->setIconVisibleInMenu(false);
}

void MainWindow::slotAnalyseNow()
{
    ui->actionEdit_Multi->setIconVisibleInMenu(false);
    ui->actionEdit_One->setIconVisibleInMenu(true);
}

void MainWindow::slotanalyse()
{
    ui->actionAnalyze->setEnabled(false);
}

void MainWindow::slotAbout()
{
    QMessageBox message(QMessageBox::NoIcon, "About SoapTyping Software",
                        QString("SoapTyping V%1\nCopyright (C) 2012-2013 BGI").arg(VERSION),
                        QMessageBox::Ok,this);
    message.setIconPixmap(QPixmap(":/png/images/about.png"));

    message.exec();
}

void MainWindow::slotHelp()
{
    QFileInfo info("Documents/Help.pdf");
    if(info.exists())
    {
        bool ok= QDesktopServices::openUrl(QUrl(info.absoluteFilePath(), QUrl::TolerantMode));
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

void MainWindow::slotAllelePairChanged(QString &allele1, QString &allele2)
{
    m_pBaseAlignTableWidget->SetAllelePairData(allele1, allele2);
}

void MainWindow::slotTypeMisMatchPostion(QSet<int> &typeMismatchPos, int type)
{
    m_pExonNavigatorWidget->SetTypeMisPos(typeMismatchPos);
}

void MainWindow::slotShowStatusBarMsg(const QString &msg)
{
    ui->statusbarright->setText(msg);
}

//由于峰图进行了编辑或变更，导致数据库发生变化，除峰图外，其他需要刷新
void MainWindow::slotChangeDB(const QString &str_samplename)
{
    AnalysisSampleThreadTask *pTask = new AnalysisSampleThreadTask(str_samplename);
    pTask->run();
    delete pTask;

    m_pMatchListWidget->SetRefresh(true);
    m_pExonNavigatorWidget->SetRefresh(true);
    m_pBaseAlignTableWidget->SetRefresh(true);
    slotSampleTreeItemChanged(m_pSelectItem, 0);
}

//由于样品文件的删除，导致数据库发生变化，需要重新刷新界面
void MainWindow::slotChangeDBByFile(QVector<QString> &vec_samplename)
{
    foreach(const QString &str,vec_samplename)
    {
        AnalysisSampleThreadTask *pTask = new AnalysisSampleThreadTask(str);
        pTask->run();
        delete pTask;
    }

    m_pMatchListWidget->SetRefresh(true);
    m_pExonNavigatorWidget->SetRefresh(true);
    m_pBaseAlignTableWidget->SetRefresh(true);
    m_pMultiPeakWidget->SetRefresh(true);
    m_pSampleTreeWidget->SetTreeData();
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
    {
        e->ignore();
    }
}

void MainWindow::slotClearAll()
{
    m_pMatchListWidget->ClearTable();
    m_pExonNavigatorWidget->ClearExonNav();
    m_pExonNavigatorWidget->update();
    m_pBaseAlignTableWidget->ClearBaseAlignTable();
    m_pMultiPeakWidget->ClearMultiPeak();
    m_pMultiPeakWidget->update();
    slotShowStatusBarMsg(QString("Ready"));
}

void MainWindow::slotPeakAct(int type)
{
    switch (type)
    {
    case 1:
        slotApplyOne();
        break;
    case 2:
        slotApplyAll();
        break;
    case 3:
        slotAnalyseLater();
        break;
    case 4:
        slotAnalyseNow();
        break;
    default:
        break;
    }

}
