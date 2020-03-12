#include "sampletreewidget.h"
#include <QHeaderView>
#include "DataBase/soaptypingdb.h"
#include <QDebug>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QContextMenuEvent>
#include "Dialog/usercommentdlg.h"
#include "Dialog/savefiledlg.h"
#include "Core/core.h"
#include "log/log.h"

SampleTreeWidget::SampleTreeWidget(QWidget *parent)
    :QTreeWidget(parent)
{
    InitUI();
    CreateRightMenu();
    ConnectSignalandSlot();
}

SampleTreeWidget::~SampleTreeWidget()
{

}

void SampleTreeWidget::ConnectSignalandSlot()
{
    connect(m_pActSaveAndClear, &QAction::triggered, this, &SampleTreeWidget::slotQuickSaveAndClear);
    connect(m_pActQuickSave, &QAction::triggered, this, &SampleTreeWidget::slotQuickSave);
    connect(m_pActSaveByDate, &QAction::triggered, this, &SampleTreeWidget::slotSaveByDate);
    connect(m_pActDelete, &QAction::triggered, this, &SampleTreeWidget::slotDelete);

    connect(m_pActUserComments, &QAction::triggered, this, &SampleTreeWidget::slotUserComments);
    connect(m_pActDelSelItem, &QAction::triggered, this, &SampleTreeWidget::slotDeleteSelectedItem);
    connect(m_pActCopy, &QAction::triggered, this, &SampleTreeWidget::slotCopyName);

    connect(m_pActMarkPendingReview, &QAction::triggered, this, &SampleTreeWidget::slotMarkPendingReview);
    connect(m_pActMarkReviewed, &QAction::triggered, this, &SampleTreeWidget::slotMarkReviewed);
    connect(m_pActMarkApproved, &QAction::triggered, this, &SampleTreeWidget::slotMarkApproved);
    connect(m_pActUnlock, &QAction::triggered, this, &SampleTreeWidget::slotUnlock);

    connect(m_pActMarkPendingAndClear, &QAction::triggered, this, &SampleTreeWidget::slotMarkPendingAndClear);
    connect(m_pActMarkReviewedAndClear, &QAction::triggered, this, &SampleTreeWidget::slotMarkReviewedAndClear);
    connect(m_pActMarkApprovedAndClear, &QAction::triggered, this, &SampleTreeWidget::slotMarkApprovedAndClear);

}

void SampleTreeWidget::InitUI()
{
    setColumnCount(3);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    header()->hide();
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    header()->setStretchLastSection(false);
    //setStyleSheet("QTreeView::branch {image:none;}");
    setIconSize(QSize(20,20));
}

void SampleTreeWidget::CreateRightMenu()
{
    m_pRightMenu = new QMenu(this);
    m_pActSaveAndClear = new QAction("Quick Save And Clear", this);
    m_pActQuickSave = new QAction("Quick Save", this);
    m_pActSaveByDate = new QAction("Save By Date", this);
    m_pActDelete = new QAction("Delete", this);

    m_pActUserComments = new QAction("User Comments", this);
    m_pActDelSelItem = new QAction("Delete Selected", this);
    m_pActCopy = new QAction("Copy Name(Ctrl+C)", this);

    m_pActMarkPendingReview = new QAction("Mark Pending Review", this);
    m_pActMarkReviewed = new QAction("Mark Reviewed", this);
    m_pActMarkApproved = new QAction("Mark Approved", this);
    m_pActUnlock = new QAction("Unlock", this);

    m_pActMarkPendingAndClear = new QAction("Mark Pending And Clear", this);
    m_pActMarkReviewedAndClear = new QAction("Mark Reviewed And Clear", this);
    m_pActMarkApprovedAndClear = new QAction("Mark Approved And Clear", this);


    m_pRightMenu->addAction(m_pActSaveAndClear);
    m_pRightMenu->addAction(m_pActQuickSave);
    m_pRightMenu->addAction(m_pActSaveByDate);
    m_pRightMenu->addAction(m_pActDelete);
    m_pRightMenu->addSeparator();

    m_pRightMenu->addAction(m_pActUserComments);
    m_pRightMenu->addAction(m_pActDelSelItem);
    m_pRightMenu->addAction(m_pActCopy);
    m_pRightMenu->addSeparator();

    m_pRightMenu->addAction(m_pActMarkPendingReview);
    m_pRightMenu->addAction(m_pActMarkReviewed);
    m_pRightMenu->addAction(m_pActMarkApproved);
    m_pRightMenu->addAction(m_pActUnlock);
    m_pRightMenu->addSeparator();

    m_pRightMenu->addAction(m_pActMarkPendingAndClear);
    m_pRightMenu->addAction(m_pActMarkReviewedAndClear);
    m_pRightMenu->addAction(m_pActMarkApprovedAndClear);
}

void SampleTreeWidget::SetTreeData()
{
    clear();
    m_map_SampleTreeInfo.clear();
    SoapTypingDB::GetInstance()->getSampleTreeDataFromSampleTable(m_map_SampleTreeInfo);
    if(m_map_SampleTreeInfo.isEmpty())
    {
        LOG_DEBUG("%s","no tree data");
        emit signalClearAll();
        return;
    }

    LOG_DEBUG("%d",m_map_SampleTreeInfo.size());
    foreach(const SampleTreeInfo_t &sampleTreeInfo, m_map_SampleTreeInfo.values())
    {
        int gsspSize = 0;
        QTreeWidgetItem *top = new QTreeWidgetItem;
        top->setText(0, sampleTreeInfo.sampleName);
        top->setText(1, sampleTreeInfo.geneName);
        //top->setForeground(1, brush);
        top->setIcon(0, Core::GetInstance()->getIcon(sampleTreeInfo.analysisType, sampleTreeInfo.markType));
        //top->setSizeHint(0,QSize(200,15));
        int treeSize=sampleTreeInfo.treeinfo.size();
        for(int j=0; j<treeSize; j++)
        {
            const FileTreeInfo_t &fileTreeInfo = sampleTreeInfo.treeinfo.at(j);
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, fileTreeInfo.fileName);
            //child->setSizeHint(0,QSize(350,25));
            if(!fileTreeInfo.isGssp)
            {
                if(fileTreeInfo.analysisType == 1)
                {
                    child->setText(1, "UN");
                    child->setIcon(0, QIcon(":/png/images/filetreeFile2.png"));
                }
                else
                {
                    child->setText(1, QString("%1%2").arg(fileTreeInfo.exonIndex).arg(fileTreeInfo.rOrF));
                    child->setIcon(0, QIcon(QString(":/png/images/filetreeFile%1.png").arg(fileTreeInfo.isGood)));
                }
            }
            else
            {
                child->setData(0,Qt::UserRole,fileTreeInfo.exonIndex);
                if(fileTreeInfo.analysisType == 4) //UNMATCH
                {
                    child->setText(1, QString("Filter:%1").arg(fileTreeInfo.gsspName));//2
                    child->setIcon(0, QIcon(":/png/images/filetreeFile2.png"));
                }
                else
                {
                    child->setText(1, QString("Filter:%1").arg(fileTreeInfo.gsspName));//2
                    child->setIcon(0, Core::GetInstance()->getIcon(fileTreeInfo.analysisType, 0));
                }
                gsspSize++;
            }
            top->addChild(child);
        }
        if(gsspSize > 1)
        {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, "Combined");
            child->setIcon(0, Core::GetInstance()->getIcon(sampleTreeInfo.analysisType, 0));
            top->addChild(child);
        }
        addTopLevelItem(top);
    }
    topLevelItem(0)->setSelected(true);
    expandItem(topLevelItem(0));
    setCurrentItem(topLevelItem(0)->child(0));
    emit itemClicked(topLevelItem(0)->child(0), 0);
}

void SampleTreeWidget::SetSelectItem(int index, const QString &str_sample)
{
    for(int i=0;i<topLevelItemCount();i++)
    {
        QString str_name = topLevelItem(i)->text(0);
        if(str_name == str_sample)
        {
            for(int j = 0;j<topLevelItem(i)->childCount();j++)
            {
                QTreeWidgetItem *item = topLevelItem(i)->child(j);
                if(item->text(1).contains(QString::number(index)))
                {
                    setCurrentItem(item);
                    break;
                }
            }
            break;
        }
    }
}

void SampleTreeWidget::RefreshSelectSample(const QString &str_sample)
{
    for(int i=0;i<topLevelItemCount();i++)
    {
        QString str_name = topLevelItem(i)->text(0);
        if(str_name == str_sample)
        {
            SampleTreeInfo_t sampleTreeInfo;
            SoapTypingDB::GetInstance()->getSampleanalysisType(str_sample, sampleTreeInfo);
            topLevelItem(i)->setIcon(0, Core::GetInstance()->getIcon(sampleTreeInfo.analysisType, sampleTreeInfo.markType));
            int treeSize=topLevelItem(i)->childCount();
            for(int j=0; j<treeSize; j++)
            {
                QTreeWidgetItem *child = topLevelItem(i)->child(j);
                QString strname = child->text(0);

                if (strname == "Combined")
                {
                    child->setIcon(0, Core::GetInstance()->getIcon(sampleTreeInfo.analysisType, 0));
                }
                else
                {
                    const FileTreeInfo_t &fileTreeInfo = sampleTreeInfo.treeinfo.at(j);
                    if(strname == fileTreeInfo.fileName)
                    {
                        if(!fileTreeInfo.isGssp)
                        {
                            if(fileTreeInfo.analysisType == 1)
                            {
                                child->setIcon(0, QIcon(":/png/images/filetreeFile2.png"));
                            }
                            else
                            {
                                child->setIcon(0, QIcon(QString(":/png/images/filetreeFile%1.png").arg(fileTreeInfo.isGood)));
                            }
                        }
                        else
                        {
                            if(fileTreeInfo.analysisType == 4) //UNMATCH
                            {
                                child->setIcon(0, QIcon(":/png/images/filetreeFile2.png"));
                            }
                            else
                            {
                                child->setIcon(0, Core::GetInstance()->getIcon(fileTreeInfo.analysisType, 0));
                            }
                        }
                    }
                }
            }

            topLevelItem(i)->setSelected(true);
            expandItem(topLevelItem(i));
            //setCurrentItem(topLevelItem(i)->child(0));
            //emit itemClicked(topLevelItem(i)->child(0), 0);

            break;
        }
    }
}

void SampleTreeWidget::SetSelectItemByName(const QString &str_sample, const QString &str_file)
{
    for(int i=0;i<topLevelItemCount();i++)
    {
        QString str_name = topLevelItem(i)->text(0);
        if(str_name == str_sample)
        {
            for(int j = 0;j<topLevelItem(i)->childCount();j++)
            {
                QTreeWidgetItem *item = topLevelItem(i)->child(j);
                if(item->text(0).contains(str_file))
                {
                    setCurrentItem(item);
                    break;
                }
            }
            break;
        }
    }
}

void SampleTreeWidget::markSampleType(int markType)
{
    QTreeWidgetItem *item = m_pSelByRightItem;
    QString str_sample = item->text(0).split('_').at(0);
    int mark,analysis;
    SoapTypingDB::GetInstance()->getMarkTypeAndAnalysisFromSampleTable(str_sample, mark, analysis);
    if(mark==APPROVED && markType!=OWNED)
    {
        QMessageBox::warning(this, tr("Soap Typing"),tr("Please unlock the sample as it was marked approved"));
        return;
    }
    SoapTypingDB::GetInstance()->setMarkTypeBySampleName(str_sample, markType);

    m_map_SampleTreeInfo[str_sample].markType = markType;

    if(item->parent()!=nullptr)
    {
        item = item->parent();
    }
    item->setIcon(0, Core::GetInstance()->getIcon(analysis, markType));
}

void SampleTreeWidget::markSampleTypeAndClear(int markType)
{
    QTreeWidgetItem *item = m_pSelByRightItem;
    QString str_sample = item->text(0).split('_').at(0);
    int mark,analysis;
    SoapTypingDB::GetInstance()->getMarkTypeAndAnalysisFromSampleTable(str_sample, mark, analysis);
    if(mark==APPROVED && markType!=OWNED)
    {
        QMessageBox::warning(this, tr("Soap Typing"),tr("Please unlock the sample as it was marked approved"));
        return;
    }
    SoapTypingDB::GetInstance()->setMarkTypeBySampleName(str_sample, markType);

    m_map_SampleTreeInfo[str_sample].markType = markType;

    slotQuickSaveAndClear();
}

void SampleTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    m_pSelByRightItem = this->itemAt(event->pos());
    if(m_pSelByRightItem != nullptr)
    {
        m_pRightMenu->exec(QCursor::pos());
        event->accept();
    }
}

void SampleTreeWidget::slotQuickSaveAndClear()
{
    QString str_sample = m_pSelByRightItem->text(0).split('_').at(0);
    slotQuickSave();
    SoapTypingDB::GetInstance()->deleteSample(str_sample);
    SetTreeData();
}

void SampleTreeWidget::slotQuickSave()
{
    QString str_sample = m_pSelByRightItem->text(0).split('_').at(0);

    SaveFileDlg::saveSampleT(false, m_map_SampleTreeInfo[str_sample]);
}

void SampleTreeWidget::slotSaveByDate()
{
    QString str_sample = m_pSelByRightItem->text(0).split('_').at(0);

    SaveFileDlg::saveSampleT(true, m_map_SampleTreeInfo[str_sample]);
}

void SampleTreeWidget::slotDelete()
{
    QTreeWidgetItem *itemN = m_pSelByRightItem;
    if(itemN->text(0).contains("Combined"))
    {
       return;
    }

    QString info;
    bool deleteSamples = false;
    if(itemN->parent() == nullptr)
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
            SoapTypingDB::GetInstance()->deleteSample(itemN->text(0));
            QVector<QString> vec_sample;
            vec_sample.push_back(itemN->text(0));
            emit signalChangeDBByFile(vec_sample);
        }
        else
        {
            QString str_type = m_pSelByRightItem->text(1);
            if (str_type.contains("Filter")) //gssp文件
            {
                SoapTypingDB::GetInstance()->deleteFile(true, itemN->text(0));
            }
            else
            {
                SoapTypingDB::GetInstance()->deleteFile(false, itemN->text(0));
            }

            QTreeWidgetItem *pParent = m_pSelByRightItem->parent();
            int count = pParent->childCount() -1;
            for(int i=0;i<pParent->childCount();i++)
            {
                QTreeWidgetItem *pitem = pParent->child(i);
                if(pitem->text(0).contains("Combined"))
                {
                    count--;
                }
            }
            if(count == 0)
            {
                SoapTypingDB::GetInstance()->deleteSample(itemN->text(0));
            }
            QString str_sample = m_pSelByRightItem->text(0).split('_').at(0);
            QVector<QString> vec_sample;
            vec_sample.push_back(str_sample);
            emit signalChangeDBByFile(vec_sample);
        }
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}

void SampleTreeWidget::slotUserComments()
{
    QString str_sample = m_pSelByRightItem->text(0).split('_').at(0);
    UserCommentDlg dlg(this);
    dlg.setSampleName(str_sample);
    dlg.exec();
}

void SampleTreeWidget::slotDeleteSelectedItem()
{
    QMap<QString,QString> fileNames;
    QVector<QString> sampleNames;
    QVector<QString> updateNames;
    for(int i=0; i<topLevelItemCount();i++)
    {
        if(topLevelItem(i)->isSelected())
        {
            sampleNames.push_back(topLevelItem(i)->text(0));
            continue;
        }

        int count = 0; //每个样品下选中的文件数
        for(int j=0;j<topLevelItem(i)->childCount(); j++)
        {
            if(!topLevelItem(i)->child(j)->text(0).contains("Combined"))
            {
                if(topLevelItem(i)->child(j)->isSelected())
                {
                    count++;
                    fileNames.insert(topLevelItem(i)->child(j)->text(0),topLevelItem(i)->child(j)->text(1));
                }
            }
            else
            {
                count++;
            }
        }

        if(count == topLevelItem(i)->childCount())//当前样品选中了所有文件
        {
            sampleNames.push_back(topLevelItem(i)->text(0));
            continue;
        }

        if(count)
        {
            updateNames.push_back(topLevelItem(i)->text(0));
        }
    }
    for(int i=0; i<sampleNames.size();i++)
    {
        SoapTypingDB::GetInstance()->deleteSample(sampleNames[i]);
    }

    QMap<QString,QString>::iterator itor = fileNames.begin();
    for(; itor != fileNames.end(); itor++)
    {
        if (itor.value().contains("Filter")) //gssp文件
        {
            SoapTypingDB::GetInstance()->deleteFile(true, itor.key());
        }
        else
        {
            SoapTypingDB::GetInstance()->deleteFile(false, itor.key());
        }
    }

    emit signalChangeDBByFile(updateNames);
}

void SampleTreeWidget::slotCopyName()
{
    QClipboard *board = QApplication::clipboard();
    board->setText(currentItem()->text(0));
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
