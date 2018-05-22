#include "sampletreewidget.h"
#include "savefiledlg.h"
#include "realtimedatabase.h"
#include "analysis.h"
#include "usercommentdlg.h"
#include "key_update.h"
SampleTreeWidget::SampleTreeWidget(QWidget *parent)
    :QTreeWidget(parent)
{
    Key_Update key;
    if(!key.AllIsRigth())
    {
        return ;
    }
    //    setStyleSheet("background-color:auto");
    onControl = false;
    sampleTreeInfoList_.clear();
//    m_TreeInfoList.clear();
    setStyle(QStyleFactory::create("Windows"));
    setSampleTreeWidgetDefault();
    setSampleTreeWidgetActionAndConnectionDefault();
    this->setContentsMargins(10,0,0,0);
}

void SampleTreeWidget::setSampleTreeWidgetDefault()
{
    setColumnCount(3);
    header()->hide();
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    //    setSelectionMode(QAbstractItemView::SingleSelection);
    header()->setResizeMode(QHeaderView::ResizeToContents);
    header()->setStretchLastSection(false);
    oldItem_ = NULL;
    oldColumn_ = 0;
}

void SampleTreeWidget::setSampleTreeWidgetActionAndConnectionDefault()
{
    rightMenu = new QMenu(this);
    actionQuickSaveAndClear = new QAction("Quick Save And Clear", this);
    actionQuickSave = new QAction("Quick Save", this);
    actionSaveByDate = new QAction("Save By Date", this);
    actionDelete = new QAction("Delete", this);
    //    actionShowGSSPZCode = new QAction("Show GSSP Z Code", this);
    actionUserComments = new QAction("User Comments", this);
    //    actionSetFinalType = new QAction("Set Final Type", this);
    actionDeleteSelectedItem = new QAction("Delete Selected", this);
    actionMarkPendingReview = new QAction("Mark Pending Review", this);
    actionMarkReviewed = new QAction("Mark Reviewed", this);
    actionMarkApproved = new QAction("Mark Approved", this);
    actionUnlock = new QAction("Unlock", this);
    actionMarkPendingAndClear = new QAction("Mark Pending And Clear", this);
    actionMarkReviewedAndClear = new QAction("Mark Reviewed And Clear", this);
    actionMarkApprovedAndClear = new QAction("Mark Approved And Clear", this);
    action_copy = new QAction("Copy Name(Ctrl+C)", this);

    rightMenu->addAction(actionQuickSaveAndClear);
    rightMenu->addAction(actionQuickSave);
    rightMenu->addAction(actionSaveByDate);
    rightMenu->addAction(actionDelete);
    rightMenu->addSeparator();
    //    rightMenu->addAction(actionShowGSSPZCode);
    rightMenu->addAction(actionUserComments);
    rightMenu->addAction(actionDeleteSelectedItem);
    rightMenu->addAction(action_copy);
    //    rightMenu->addAction(actionSetFinalType);
    rightMenu->addSeparator();
    rightMenu->addAction(actionMarkPendingReview);
    rightMenu->addAction(actionMarkReviewed);
    rightMenu->addAction(actionMarkApproved);
    rightMenu->addAction(actionUnlock);
    rightMenu->addSeparator();
    rightMenu->addAction(actionMarkPendingAndClear);
    rightMenu->addAction(actionMarkReviewedAndClear);
    rightMenu->addAction(actionMarkApprovedAndClear);

    connect(actionQuickSaveAndClear, SIGNAL(triggered()), this, SLOT(slotQuickSaveAndClear()));
    connect(actionQuickSave, SIGNAL(triggered()), this, SLOT(slotQuickSave()));
    connect(actionSaveByDate, SIGNAL(triggered()), this, SLOT(slotSaveByDate()));
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(slotDelete()));
    //    connect(actionShowGSSPZCode, SIGNAL(triggered()), this, SLOT(slotShowGSSPZCode()));
    connect(actionUserComments, SIGNAL(triggered()), this, SLOT(slotUserComments()));
    //    connect(actionSetFinalType, SIGNAL(triggered()), this, SLOT(slotSetFinalType()));
    connect(actionDeleteSelectedItem, SIGNAL(triggered()), this, SLOT(slotDeleteSelectedItem()));
    connect(actionMarkPendingReview, SIGNAL(triggered()), this, SLOT(slotMarkPendingReview()));
    connect(actionMarkReviewed, SIGNAL(triggered()), this, SLOT(slotMarkReviewed()));
    connect(actionMarkApproved, SIGNAL(triggered()), this, SLOT(slotMarkApproved()));
    connect(actionUnlock, SIGNAL(triggered()), this, SLOT(slotUnlock()));
    connect(actionMarkPendingAndClear, SIGNAL(triggered()), this, SLOT(slotMarkPendingAndClear()));
    connect(actionMarkReviewedAndClear, SIGNAL(triggered()), this, SLOT(slotMarkReviewedAndClear()));
    connect(actionMarkApprovedAndClear, SIGNAL(triggered()), this, SLOT(slotMarkApprovedAndClear()));
    connect(action_copy, SIGNAL(triggered()), this, SLOT(slotCopyName()));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slotItemChanged(QTreeWidgetItem*, int)));
}

void SampleTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QTreeWidgetItem *item0 = NULL;
    item0 = this->itemAt(event->pos());
    if(item0 == NULL)
    {
        return;
    }
    rightMenu->exec(QCursor::pos());
    event->accept();
}


void SampleTreeWidget::setData(QVector<SampleTreeInfo> &sampleTreeInfoList)
{
    this->clear();
    QBrush brush;
    brush.setColor(Qt::blue);
    int sampleSize = sampleTreeInfoList.size();
    setIconSize(QSize(20,20));
    for(int i=0; i<sampleSize; i++)
    {
        const SampleTreeInfo &sampleTreeInfo = sampleTreeInfoList.at(i);
        QTreeWidgetItem *top = new QTreeWidgetItem;
        top->setText(0, sampleTreeInfo.sampleName);
        top->setText(1, sampleTreeInfo.geneName);
        //        top->setBackgroundColor(1, Qt::cyan);
        top->setForeground(1, brush);
        top->setIcon(0, getIcon(sampleTreeInfo.analysisType, sampleTreeInfo.markType));
        top->setSizeHint(0,QSize(200,20));
        int fileSize=sampleTreeInfo.fileTreeInfo.size();
        for(int j=0; j<fileSize; j++)
        {
            const FileTreeInfo &fileTreeInfo = sampleTreeInfo.fileTreeInfo.at(j);
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, fileTreeInfo.fileName);
            child->setSizeHint(0,QSize(300,20));
            if(fileTreeInfo.analysisType == 1)
            {
                child->setText(1, "UN");
                child->setIcon(0, QIcon(":/images/filetreeFile2.png"));
            }
            else
            {
                child->setText(1, QString("%1%2").arg(fileTreeInfo.exonIndex).arg(fileTreeInfo.rOrF));
                child->setIcon(0, QIcon(QString(":/images/filetreeFile%1.png").arg(fileTreeInfo.isGood)));
            }
            top->addChild(child);
        }
        int gsspSize = sampleTreeInfo.gsspTreeInfo.size();
        for(int j=0; j<gsspSize; j++)
        {
            const GsspTreeInfo &gsspTreeInfo = sampleTreeInfo.gsspTreeInfo.at(j);
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, gsspTreeInfo.fileName);
            if(gsspTreeInfo.analysisType == 4) //UNMATCH
            {
                child->setText(1, "UN");
                child->setText(2, QString("Filter:%1").arg(gsspTreeInfo.gsspName));
                child->setIcon(0, QIcon(":/images/filetreeFile2.png"));
            }
            else
            {
//                child->setText(1, QString("%1%2").arg(gsspTreeInfo.exonIndex).arg(gsspTreeInfo.rOrF));
                child->setText(1, QString("Filter:%1").arg(gsspTreeInfo.gsspName));
                child->setIcon(0, getIcon(gsspTreeInfo.analysisType, 0));
            }
            top->addChild(child);
        }
        if(gsspSize > 1)
        {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, "Combined");
            child->setIcon(0, getIcon(sampleTreeInfo.analysisType, 0));
            top->addChild(child);
        }
        addTopLevelItem(top);
    }
    oldColumn_ = 0;
    oldItem_ = NULL;
}

void SampleTreeWidget::setData(QVector<SampleTreeInfo_t> &sampleTreeInfoList)
{
    this->clear();
    QBrush brush;
    brush.setColor(Qt::blue);
    int sampleSize = sampleTreeInfoList.size();
    setIconSize(QSize(20,20));
    for(int i=0; i<sampleSize; i++)
    {
        int gsspSize = 0;
        const SampleTreeInfo_t &sampleTreeInfo = sampleTreeInfoList.at(i);
        QTreeWidgetItem *top = new QTreeWidgetItem;
        top->setText(0, sampleTreeInfo.sampleName);
        top->setText(1, sampleTreeInfo.geneName);
        top->setForeground(1, brush);
        top->setIcon(0, getIcon(sampleTreeInfo.analysisType, sampleTreeInfo.markType));
        top->setSizeHint(0,QSize(200,20));
        int treeSize=sampleTreeInfo.treeinfo.size();
        for(int j=0; j<treeSize; j++)
        {
            const TreeInfo_t &fileTreeInfo = sampleTreeInfo.treeinfo.at(j);
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, fileTreeInfo.fileName);
            child->setSizeHint(0,QSize(350,25));
            if(!fileTreeInfo.isGssp)
            {
                if(fileTreeInfo.analysisType == 1)
                {
                    child->setText(1, "UN");
                    child->setIcon(0, QIcon(":/images/filetreeFile2.png"));
                }
                else
                {
                    child->setText(1, QString("%1%2").arg(fileTreeInfo.exonIndex).arg(fileTreeInfo.rOrF));
                    child->setIcon(0, QIcon(QString(":/images/filetreeFile%1.png").arg(fileTreeInfo.isGood)));
                }
            }
            else
            {
                if(fileTreeInfo.analysisType == 4) //UNMATCH
                {
//                    child->setText(1, "UN");
                    child->setText(1, QString("Filter:%1").arg(fileTreeInfo.gsspName));//2
                    child->setIcon(0, QIcon(":/images/filetreeFile2.png"));
                }
                else
                {
//                    child->setText(1, QString("%1%2").arg(fileTreeInfo.exonIndex).arg(fileTreeInfo.rOrF));
                    child->setText(1, QString("Filter:%1").arg(fileTreeInfo.gsspName));//2
                    child->setIcon(0, getIcon(fileTreeInfo.analysisType, 0));
                }
                gsspSize++;
            }
            top->addChild(child);
        }
        if(gsspSize > 1)
        {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, "Combined");
            child->setIcon(0, getIcon(sampleTreeInfo.analysisType, 0));
            top->addChild(child);
        }
        addTopLevelItem(top);
    }
    oldColumn_ = 0;
    oldItem_ = NULL;
}

QVector<SampleTreeInfo_t> SampleTreeWidget::sortData(QVector<SampleTreeInfo> sampleTreeInfoList)
{
    QVector<SampleTreeInfo_t> returndata;
    int sampleSize = sampleTreeInfoList.size();
    for(int i = 0;i < sampleSize;i++)
    {
        SampleTreeInfo_t tmpSampleTreeInfo;
        SampleTreeInfo sampleTreeInfo  = sampleTreeInfoList.at(i);
        tmpSampleTreeInfo.analysisType = sampleTreeInfo.analysisType;
        tmpSampleTreeInfo.geneName     = sampleTreeInfo.geneName;
        tmpSampleTreeInfo.markType     = sampleTreeInfo.markType;
        tmpSampleTreeInfo.sampleName   = sampleTreeInfo.sampleName;
        int fileSize = sampleTreeInfo.fileTreeInfo.size();
        int GsspSize = sampleTreeInfo.gsspTreeInfo.size();
        for(int j = 0;j < fileSize;j++)
        {
            TreeInfo_t tmpTreeInfo;
            tmpTreeInfo.analysisType = sampleTreeInfo.fileTreeInfo[j].analysisType;
            tmpTreeInfo.exonIndex    = sampleTreeInfo.fileTreeInfo[j].exonIndex;
            tmpTreeInfo.fileName     = sampleTreeInfo.fileTreeInfo[j].fileName;
            tmpTreeInfo.isGood       = sampleTreeInfo.fileTreeInfo[j].isGood;
            tmpTreeInfo.rOrF         = sampleTreeInfo.fileTreeInfo[j].rOrF;
            tmpTreeInfo.isGssp       = false;
            tmpTreeInfo.gsspName     = "";
            tmpSampleTreeInfo.treeinfo.append(tmpTreeInfo);
        }
        for(int j = 0;j < GsspSize;j++)
        {
            TreeInfo_t tmpTreeInfo;
            tmpTreeInfo.analysisType = sampleTreeInfo.gsspTreeInfo[j].analysisType;
            tmpTreeInfo.exonIndex    = sampleTreeInfo.gsspTreeInfo[j].exonIndex;
            tmpTreeInfo.fileName     = sampleTreeInfo.gsspTreeInfo[j].fileName;
            tmpTreeInfo.isGood       = sampleTreeInfo.gsspTreeInfo[j].isGood;
            tmpTreeInfo.rOrF         = sampleTreeInfo.gsspTreeInfo[j].rOrF;
            tmpTreeInfo.gsspName     = sampleTreeInfo.gsspTreeInfo[j].gsspName;
            tmpTreeInfo.isGssp       = true;
            tmpSampleTreeInfo.treeinfo.append(tmpTreeInfo);
        }
        int treeSize =  tmpSampleTreeInfo.treeinfo.size();
        for(int j = 0;j < treeSize;j++)
        {
            TreeInfo_t tmpTreeInfo;
            for(int u = 0;u < treeSize - 1 - j;u++)
            {
                if(tmpSampleTreeInfo.treeinfo[u].exonIndex > tmpSampleTreeInfo.treeinfo[u+1].exonIndex)
                {
                    tmpTreeInfo = tmpSampleTreeInfo.treeinfo[u];
                    tmpSampleTreeInfo.treeinfo[u]   = tmpSampleTreeInfo.treeinfo[u+1];
                    tmpSampleTreeInfo.treeinfo[u+1] = tmpTreeInfo;
                }
            }
        }
        for(int j=0; j<treeSize - 1; j++)
        {
            TreeInfo_t tmpTreeInfo;
            if(tmpSampleTreeInfo.treeinfo[j].exonIndex == tmpSampleTreeInfo.treeinfo[j+1].exonIndex)
            {
                if(tmpSampleTreeInfo.treeinfo[j].rOrF.toAscii() > tmpSampleTreeInfo.treeinfo[j+1].rOrF.toAscii())
                {
                    tmpTreeInfo = tmpSampleTreeInfo.treeinfo[j];
                    tmpSampleTreeInfo.treeinfo[j] = tmpSampleTreeInfo.treeinfo[j+1];
                    tmpSampleTreeInfo.treeinfo[j+1] = tmpTreeInfo;
                }
            }
        }
        for(int j=0; j<treeSize - 1; j++)
        {
            TreeInfo_t tmpTreeInfo;
            if(tmpSampleTreeInfo.treeinfo[j].exonIndex == tmpSampleTreeInfo.treeinfo[j+1].exonIndex)
            {
                if(tmpSampleTreeInfo.treeinfo[j].isGssp)
                {
                    tmpTreeInfo = tmpSampleTreeInfo.treeinfo[j];
                    tmpSampleTreeInfo.treeinfo[j] = tmpSampleTreeInfo.treeinfo[j+1];
                    tmpSampleTreeInfo.treeinfo[j+1] = tmpTreeInfo;
                }
            }
        }
        returndata.append(tmpSampleTreeInfo);
    }
    return returndata;
}
/*
QVector<SampleTreeInfo> SampleTreeWidget::sortData(QVector<SampleTreeInfo> sampleTreeInfoList)
{
    QVector<SampleTreeInfo> returndata;
    int sampleSize = sampleTreeInfoList.size();
    for(int i=0; i<sampleSize; i++)
    {
        SampleTreeInfo sampleTreeInfo = sampleTreeInfoList.at(i);
        int fileSize = sampleTreeInfo.fileTreeInfo.size();
        int gsspSize = sampleTreeInfo.gsspTreeInfo.size();
        for(int j=0; j<fileSize - 1; j++)
        {
            FileTreeInfo tmpfileTreeInfo;
            for(int u = 0;u < fileSize - 1 - j;u++)
            {
                if(sampleTreeInfo.fileTreeInfo[u].exonIndex > sampleTreeInfo.fileTreeInfo[u+1].exonIndex)
                {
                    tmpfileTreeInfo = sampleTreeInfo.fileTreeInfo[u];
                    sampleTreeInfo.fileTreeInfo[u] = sampleTreeInfo.fileTreeInfo[u+1];
                    sampleTreeInfo.fileTreeInfo[u+1] = tmpfileTreeInfo;
                }
            }
        }
        for(int j=0; j<fileSize - 1; j++)
        {
            FileTreeInfo tmpfileTreeInfo;
            if(sampleTreeInfo.fileTreeInfo[j].exonIndex == sampleTreeInfo.fileTreeInfo[j+1].exonIndex)
            {
                if(sampleTreeInfo.fileTreeInfo[j].rOrF.toAscii() > sampleTreeInfo.fileTreeInfo[j+1].rOrF.toAscii())
                {
                    tmpfileTreeInfo = sampleTreeInfo.fileTreeInfo[j];
                    sampleTreeInfo.fileTreeInfo[j] = sampleTreeInfo.fileTreeInfo[j+1];
                    sampleTreeInfo.fileTreeInfo[j+1] = tmpfileTreeInfo;
                }
            }
        }

        for(int j=0; j<gsspSize - 1; j++)
        {
            GsspTreeInfo tmpGsspTreeInfo;
            for(int u = 0;u < gsspSize - 1 - j;u++)
            {
                if(sampleTreeInfo.gsspTreeInfo[u].exonIndex > sampleTreeInfo.gsspTreeInfo[u+1].exonIndex)
                {
                    tmpGsspTreeInfo = sampleTreeInfo.gsspTreeInfo[u];
                    sampleTreeInfo.gsspTreeInfo[u] = sampleTreeInfo.gsspTreeInfo[u+1];
                    sampleTreeInfo.gsspTreeInfo[u+1] = tmpGsspTreeInfo;
                }
            }
        }
        for(int j=0; j<gsspSize - 1; j++)
        {
            GsspTreeInfo tmpGsspTreeInfo;
            if(sampleTreeInfo.gsspTreeInfo[j].exonIndex == sampleTreeInfo.gsspTreeInfo[j+1].exonIndex)
            {
                if(sampleTreeInfo.gsspTreeInfo[j].rOrF.toAscii() > sampleTreeInfo.gsspTreeInfo[j+1].rOrF.toAscii())
                {
                    tmpGsspTreeInfo = sampleTreeInfo.gsspTreeInfo[j];
                    sampleTreeInfo.gsspTreeInfo[j] = sampleTreeInfo.gsspTreeInfo[j+1];
                    sampleTreeInfo.gsspTreeInfo[j+1] = tmpGsspTreeInfo;
                }
            }
        }

        returndata.append(sampleTreeInfo);
    }
    return returndata;
}*/

QIcon SampleTreeWidget::getIcon(int analysisType, int markType)
{
    return QIcon(QString(":/images/filetree%1%2.png").arg(markType).arg(analysisType));
}

void SampleTreeWidget::markSampleType(int markType)
{
    int mark = getMarkTypeBySampleName(signalInfo_.sampleName.toAscii());
    int analysis = getAnalysisTypeBySampleName(signalInfo_.sampleName.toAscii());
    if(mark==APPROVED && markType!=OWNED)
    {
        QMessageBox::warning(this, tr("Soap Typing"),tr("Please unlock the sample as it was marked approved"));
        return;
    }
    setMarkTypeBySampleName(signalInfo_.sampleName.toAscii(), markType);
    int topIndex = getSampleIndex(signalInfo_.sampleName);
    sampleTreeInfoList_[topIndex].markType = markType;
    QTreeWidgetItem *itm = currentItem();
    if(itm->parent()!=NULL)
        itm=itm->parent();
    itm->setIcon(0, getIcon(analysis, markType));
}

void SampleTreeWidget::expandSample(int type)
{
    int topIndex = getSampleIndex(signalInfo_.sampleName);
    if(topIndex==-1)
    {
        signalInfo_.sampleName="";
        emit signalFileChnaged(signalInfo_, 1);
        return;
    }
    topLevelItem(topIndex)->setSelected(true);
    expandItem(topLevelItem(topIndex));
    int childIndex = getFileIndex(signalInfo_.fileName, topIndex);
    emitFileChangedSignal(topIndex, childIndex, type);
}

int SampleTreeWidget::getSampleIndex(const QString &sampleName)
{
    if(topLevelItemCount()<=0)
        return -1;
    for(int i=0; i<topLevelItemCount(); i++)
    {
        if(topLevelItem(i)->text(0)==sampleName)
            return i;
    }
    return 0;
}

int SampleTreeWidget::getFileIndex(const QString &fileName, int sampleIndex)
{
    if(fileName.isEmpty())
        return 0;
    for(int i=0; i<topLevelItem(sampleIndex)->childCount(); i++)
    {
        if(topLevelItem(sampleIndex)->child(i)->text(0)==fileName)
        {
            topLevelItem(sampleIndex)->child(i)->setSelected(true);
            return i;
        }
    }
    topLevelItem(sampleIndex)->child(0)->setSelected(true);
    return 0;
}

void SampleTreeWidget::emitFileChangedSignal(int topIndex, int childIndex, int type)
{
    QTreeWidgetItem *item0 = topLevelItem(topIndex)->child(childIndex);
    if(item0->text(1).contains("UN"))
    {
        signalInfo_.isUndefined = true;
    }
    else
    {
        signalInfo_.isUndefined = false;
    }
    if(item0->text(0).contains("Combined"))
    {
        signalInfo_.isCombined = true;
    }
    else
    {
        signalInfo_.isCombined = false;
    }
    if(item0->text(1).contains("Filter"))
    {
        signalInfo_.isGssp = true;
    }
    else
    {
        signalInfo_.isGssp = false;
    }
    if(oldColumn_ == 1)
    {
        signalInfo_.isGsspFilter = true;
    }
    else
    {
        signalInfo_.isGsspFilter = false;
    }
//    const SampleTreeInfo &sampleTreeInfo = m_TreeInfoList.at(topIndex);
//    signalInfo_.sampleName = sampleTreeInfo.sampleName;
//    signalInfo_.geneName = sampleTreeInfo.geneName;
//    if(childIndex < sampleTreeInfo.fileTreeInfo.size())
//    {
//        const FileTreeInfo &fileTreeInfo = sampleTreeInfo.fileTreeInfo.at(childIndex);
//        signalInfo_.fileName = fileTreeInfo.fileName;
//        signalInfo_.exonIndex = fileTreeInfo.exonIndex;
//        signalInfo_.rOrF = fileTreeInfo.rOrF;
//    }
//    else
//    {
//        childIndex = childIndex - sampleTreeInfo.fileTreeInfo.size();
//        if(signalInfo_.isCombined)
//            childIndex = 0;
//        const GsspTreeInfo &gsspTreeInfo = sampleTreeInfo.gsspTreeInfo.at(childIndex);
//        signalInfo_.fileName = gsspTreeInfo.fileName;
//        signalInfo_.exonIndex = gsspTreeInfo.exonIndex;
//        signalInfo_.rOrF = gsspTreeInfo.rOrF;
//    }
//    emit signalFileChnaged(signalInfo_, type);
//    emit signalGeneName(sampleTreeInfo.geneName);

    const SampleTreeInfo_t &sampleTreeInfo = m_TreeInfoList.at(topIndex);
    signalInfo_.sampleName = sampleTreeInfo.sampleName;
    signalInfo_.geneName = sampleTreeInfo.geneName;
    if(signalInfo_.isCombined)
        childIndex = 0;
    const TreeInfo_t &fileTreeInfo = sampleTreeInfo.treeinfo.at(childIndex);
    signalInfo_.fileName = fileTreeInfo.fileName;
    signalInfo_.exonIndex = fileTreeInfo.exonIndex;
    signalInfo_.rOrF = fileTreeInfo.rOrF;
    emit signalFileChnaged(signalInfo_, type);
    emit signalGeneName(sampleTreeInfo.geneName);
}

void SampleTreeWidget::slotFileChanged(SignalInfo &signalInfo, int type)
{
    clearSelection();
    if(type!=0)
    {
        signalInfo_ = signalInfo;
        sampleTreeInfoList_.clear();
        m_TreeInfoList.clear();
        getSampleTreeDataFromRealTimeDatabase(sampleTreeInfoList_);
        m_TreeInfoList = sortData(sampleTreeInfoList_);
        setData(m_TreeInfoList);
        expandSample(type);
    }
    else
    {
        int topIndex = getSampleIndex(signalInfo.sampleName);
        if(topIndex==-1)
        {
            return;
        }
        topLevelItem(topIndex)->setSelected(true);
        expandItem(topLevelItem(topIndex));
        int childIndex = getFileIndex(signalInfo.fileName, topIndex);
    }
}

void SampleTreeWidget::slotItemChanged(QTreeWidgetItem *item0, int col)
{
    if(onControl)
        return;

//    if(col == 1 || item0->text(2).isEmpty())
//    {
//        col = 0;//样品名, 和不含有filter的行, 重复点击无效.
//    }
    if(col == 0||!item0->text(1).contains("Filter"))
    {
        col = 0;//样品名, 和不含有filter的行, 重复点击无效.
    }
    if(item0 == oldItem_ && col == oldColumn_)
    {
        return; //样品名, 和不含有filter的行, 重复点击无效.
    }
    oldItem_ = item0;
    oldColumn_ = col;
    if(item0->parent() == NULL)
    {
        item0 = item0->child(0);
    }
    int topIndex = indexOfTopLevelItem(item0->parent());
    int childIndex = item0->parent()->indexOfChild(item0);
    emitFileChangedSignal(topIndex, childIndex, 0);
}

void SampleTreeWidget::slotQuickSaveAndClear()
{
    slotQuickSave();
    deleteSample(signalInfo_.sampleName.toAscii());
    slotFileChanged(signalInfo_, 1);
}

void SampleTreeWidget::slotQuickSave()
{
    int topIndex = getSampleIndex(signalInfo_.sampleName);
    if(topIndex != -1)
    {
        saveSampleT(sampleTreeInfoList_.at(topIndex));
    }
    else
    {
        QMessageBox::warning(this, tr("Soap Typing"), tr("Sample info error when save!"));
    }
}

void SampleTreeWidget::slotSaveByDate()
{
    int topIndex = getSampleIndex(signalInfo_.sampleName);
    if(topIndex != -1)
    {
        QDate date(QDate::currentDate());
        QString dates = QString("%1%2%3").arg(date.year()).arg(date.month()).arg(date.day());
        saveSampleTByDate(sampleTreeInfoList_.at(topIndex), dates);
    }
    else
    {
        QMessageBox::warning(this, tr("Soap Typing"), tr("Sample info error when save by date!"));
    }
}

void SampleTreeWidget::slotDelete()
{
    QTreeWidgetItem *itemN = this->currentItem();
    if(itemN->text(0).contains("Combined"))
        return;
    QString info;
    bool deleteSamples = false;
    if(itemN->parent()==NULL)
    {
        deleteSamples = true;
        info.append(QString("Would you like to delete the sample:\n%1").arg(itemN->text(0)));
    }
    else
    {
        info.append(QString("Would you like to delete the trace:\n%1: %2").arg(itemN->text(1)).arg(itemN->text(0)));
    }
    QMessageBox informationBox;
    informationBox.setWindowTitle(tr("Soap Typing"));
    informationBox.setIcon(QMessageBox::Information);
    informationBox.setText(info);
    informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    switch(informationBox.exec())
    {
    case QMessageBox::Yes:
        if(deleteSamples)
        {
            deleteSample(itemN->text(0).toAscii());
        }
        else
        {
            deleteFile(itemN->text(0).toAscii()); //包括删除样品名.
            analysisSample(signalInfo_.sampleName.toAscii());
        }
        slotFileChanged(signalInfo_, 1);
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}

void SampleTreeWidget::slotShowGSSPZCode()
{

}

void SampleTreeWidget::slotUserComments()
{
    UserCommentDlg user;
    user.setSampleName(signalInfo_.sampleName);
    user.exec();

}

void SampleTreeWidget::slotSetFinalType()
{

}

void SampleTreeWidget::slotDeleteSelectedItem()
{
    QVector<QString> fileNames;
    QVector<QString> sampleNames;
    QVector<QString> updateNames;
    for(int i=0; i<topLevelItemCount();i++)
    {
        if(topLevelItem(i)->isSelected())
        {
            sampleNames.push_back(topLevelItem(i)->text(0));
            continue;
        }
        bool flag = false;
        for(int j=0; j<topLevelItem(i)->childCount(); j++)
        {
            if(topLevelItem(i)->child(j)->isSelected() && !topLevelItem(i)->child(j)->text(0).contains("Combined"))
            {
                fileNames.push_back(topLevelItem(i)->child(j)->text(0));
                flag = true;
            }
        }
        if(flag)
        {
            updateNames.push_back(topLevelItem(i)->text(0));
        }
    }
    for(int i=0; i<sampleNames.size();i++)
    {
        deleteSample(sampleNames.at(i).toAscii());
    }
    for(int i=0; i<fileNames.size();i++)
    {
        deleteFile(fileNames.at(i).toAscii());
    }
    for(int i=0; i<updateNames.size();i++)
    {
        analysisSample(updateNames.at(i).toAscii());
    }
    slotFileChanged(signalInfo_, 1);
}

void SampleTreeWidget::slotMarkPendingReview()
{
    markSampleType(PENDING);
}

void SampleTreeWidget::slotMarkReviewed()
{
    markSampleType(REVIEWED);
}

void SampleTreeWidget::slotMarkApproved()
{
    markSampleType(APPROVED);
}

void SampleTreeWidget::slotUnlock()
{
    markSampleType(OWNED);
}

void SampleTreeWidget::slotMarkPendingAndClear()
{

    markSampleTypeAndClear(PENDING);
}

void SampleTreeWidget::slotMarkReviewedAndClear()
{
    markSampleTypeAndClear(REVIEWED);
}

void SampleTreeWidget::slotMarkApprovedAndClear()
{
    markSampleTypeAndClear(APPROVED);
}
void SampleTreeWidget::slotCopyName()
{
    QClipboard *board = QApplication::clipboard();
    board->setText(currentItem()->text(0));
}

void SampleTreeWidget::markSampleTypeAndClear(int markType)
{
    int mark = getMarkTypeBySampleName(signalInfo_.sampleName.toAscii());
    if(mark==APPROVED && markType!=OWNED)
    {
        QMessageBox::warning(this, tr("Soap Typing"),tr("Please unlock the sample as it was marked approved"));
        return;
    }
    setMarkTypeBySampleName(signalInfo_.sampleName.toAscii(), markType);
    int topIndex = getSampleIndex(signalInfo_.sampleName);
    sampleTreeInfoList_[topIndex].markType = markType;
    slotQuickSaveAndClear();
}

void SampleTreeWidget::emitAfterDelete()
{
    if(topLevelItemCount()<=0)
    {
        signalInfo_.sampleName="";
        emit signalFileChnaged(signalInfo_, 1);
    }
    else
    {
        expandSample(0);
    }
}

void SampleTreeWidget::keyPressEvent(QKeyEvent *event)
{
    if(topLevelItemCount()<=0)
        return;

    if(event->key()==Qt::Key_Control || event->key()==Qt::Key_Shift)
    {
        onControl = true;
    }
    else if(event->key()==Qt::Key_C)
    {
        slotCopyName();
    }
}
void SampleTreeWidget::keyReleaseEvent(QKeyEvent *event)
{
    onControl = false;
}
