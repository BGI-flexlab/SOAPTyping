#include "matchlist.h"
#include "analysis.h"
#include "gsspinfodlg.h"
#include "finaltypedlg.h"
#include "config.h"
#include "key_update.h"

MatchListWidget::MatchListWidget(QWidget *parent)
    :QTableWidget(parent)
{
    Key_Update key;
    if(!key.AllIsRigth())
    {
        return ;
    }
    setConfig();
    colNum_ = 5;
    rowNum_ = 500;
    rowNumTmp_ = 0;
    oldRow_ = -1;

    menuRight_ = new QMenu(this);
    actionShowGSSPZCode_ = new QAction(tr("Show Gssp Z Code"), this);
//    actionShowUndefinedExon_ = new QAction(tr("Show Undefined Exon"), this);
//    actionSetGsspInfo_ = new QAction(tr("Set Gssp Info"), this);
    actionSetFinalType_ = new QAction(tr("Set Final Type"), this);
    menuRight_->addAction(actionShowGSSPZCode_);
//    menuRight_->addAction(actionShowUndefinedExon_);
    menuRight_->addAction(actionSetFinalType_);
//    menuRight_->addAction(actionSetGsspInfo_);

    setMatchListWidgetDefault();
    connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(slotClickIndelItem(QTableWidgetItem*)));
    connect(this, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotRowChanged(QTableWidgetItem*)));
    connect(actionShowGSSPZCode_, SIGNAL(triggered()), this, SLOT(slotShowGsspZCode()));
    connect(actionSetFinalType_,SIGNAL(triggered()), this, SLOT(slotSetFinalType()));
}

void MatchListWidget::setMatchListWidgetDefault()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->verticalHeader()->setVisible(false);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->horizontalHeader()->setStretchLastSection(true);
    this->setStyleSheet("selection-background-color:#3CBADA");
    this->setRowCount(rowNum_);
    this->setColumnCount(colNum_);
    for(int i=0; i<rowNum_; i++)
    {
        this->setRowHeight(i,15);
    }

    //defaultHead_<<""<<""<<""<<""<<"";
    defaultHead_<<"Mis"<<"Allele1"<<"Allele2"<<"Info"<<"Gssp";
    this->setColumnWidth(0,40);
    this->setColumnWidth(1,90);
    this->setColumnWidth(2,90);
    this->setColumnWidth(3,70);
    this->setColumnWidth(4,40);
    this->setHorizontalHeaderLabels(defaultHead_);
    QTableWidgetItem *itemArray = new QTableWidgetItem[rowNum_ * colNum_];
    for(int i=0; i<rowNum_; i++)
    {
        for(int j=0; j<colNum_; j++)
        {
            this->setItem(i, j, itemArray++);
        }
    }
}
void MatchListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if(rowNumTmp_ < 1)
        return;
    QTableWidgetItem *itemNow = itemAt(event->pos());
    if(itemNow->row()<rowNumTmp_)
    {
        menuRight_->exec(QCursor::pos());
        event->accept();
    }
}

void MatchListWidget::clearItem()
{
    this->setHorizontalHeaderLabels(defaultHead_);
    for(int i=0; i<colNum_; i++)
    {
        for(int j=0; j<rowNumTmp_; j++)
        {
            this->item(j, i)->setText("");
        }
    }
    rowNumTmp_ = 0;
    oldRow_ = -1;
}

void MatchListWidget::setData(QStringList &typeResult)
{
    this->clearItem();
    this->clearSelection();
    if(typeResult.size()<1)
        return;

    QStringList head;
    head<<"Mis"<<"Allele1"<<"Allele2"<<"Info"<<"Gssp";
    this->setHorizontalHeaderLabels(head);
    if(signalInfo_.isGssp && !signalInfo_.isGsspFilter)
    {
        for(int i=0; i<typeResult.size(); i++)
        {
            QStringList line=typeResult.at(i).split(",");
            this->item(i, 0)->setText(line.at(0));
            this->item(i, 1)->setText(line.at(1));
            rowNumTmp_++;
            if(rowNumTmp_ >= 500)
                break;
        }
    }
    else if(isShieldIndel_)
    {
        for(int i=0; i<typeResult.size(); i++)
        {
            QStringList line = typeResult.at(i).split(",");
            if(line.at(3).toInt()!=0)
            {
                if(isIndelInRange(line.at(1).toAscii(),alignStart_, alignEnd_) || isIndelInRange(line.at(2).toAscii(), alignStart_, alignEnd_))
                    continue;
                line[0].append("*");
            }
            this->item(rowNumTmp_, 0)->setText(line.at(0));
            this->item(rowNumTmp_, 1)->setText(line.at(1));
            this->item(rowNumTmp_, 2)->setText(line.at(2));
            this->item(rowNumTmp_, 3)->setText(line.at(4));
            rowNumTmp_++;
            if(rowNumTmp_ >= 500)
                break;
        }
    }
    else
    {
        for(int i=0; i<typeResult.size(); i++)
        {
            QStringList line = typeResult.at(i).split(",");
            if(line.at(3).toInt()!=0)
            {
                line[0].append("*");
            }
            this->item(rowNumTmp_, 0)->setText(line.at(0));
            this->item(rowNumTmp_, 1)->setText(line.at(1));
            this->item(rowNumTmp_, 2)->setText(line.at(2));
            this->item(rowNumTmp_, 3)->setText(line.at(4));
            rowNumTmp_++;
            if(rowNumTmp_ >= 500)
                break;
        }
    }
}

void MatchListWidget::slotRowChanged(QTableWidgetItem *itemNow)
{
    int row = itemNow->row();
    if(row>=rowNumTmp_)
        return;
    if(row == oldRow_)
        return;
    oldRow_ = row;
    QString allele1 = item(row, 1)->text();
    QString allele2 = item(row, 2)->text();
    if(allele2.isEmpty())
        allele2 = allele1;
    emit signalAllelePair(allele1, allele2);
}

void MatchListWidget::slotClickIndelItem(QTableWidgetItem *itemNow)
{
    if(itemNow->column()==0 && itemNow->text().contains('*'))
    {
        QString allele1 = item(itemNow->row(), 1)->text();
        QString allele2 = item(itemNow->row(), 2)->text();
        IndelInfo indelInfo1, indelInfo2;
        getIndelInfoFromStaticDatabase(allele1.toAscii(), indelInfo1);
        getIndelInfoFromStaticDatabase(allele2.toAscii(), indelInfo2);
        QString info;
        int basePos;
        if(indelInfo1.isIndel == 1)
        {
            info.append(QString("%1 has %2 at %3\n").arg(allele1).arg(indelInfo1.indelInfo).arg(indelInfo1.indelPostion+1));
            basePos = indelInfo1.indelPostion;
        }
        if(indelInfo2.isIndel == 1)
        {
            info.append(QString("%1 has %2 at %3\n").arg(allele2).arg(indelInfo2.indelInfo).arg(indelInfo2.indelPostion+1));
            basePos = indelInfo2.indelPostion;
        }
        emit signalIndelPostion(basePos);
        info.append("Would you like to filter this Allele?");
        QMessageBox informationBox;
        informationBox.setWindowTitle(tr("Soap Typing SBT"));
        informationBox.setIcon(QMessageBox::Information);
        informationBox.setText(info);
        informationBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        switch(informationBox.exec())
        {
        case QMessageBox::Yes:
            if(indelInfo1.isIndel==1 && indelInfo2.isIndel==1)
            {
                shieldAllele(signalInfo_.sampleName, QString("%1;%2").arg(allele1).arg(allele2));
            }
            else if(indelInfo1.isIndel==1)
            {
                shieldAllele(signalInfo_.sampleName,allele1);
            }
            else if(indelInfo2.isIndel==1)
            {
                shieldAllele(signalInfo_.sampleName,allele2);
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
}

void MatchListWidget::slotFileChanged(SignalInfo &signalInfo, int type)
{
    if(signalInfo_.sampleName==signalInfo.sampleName
            && (signalInfo_.isCombined|signalInfo.isCombined) == false
            && (signalInfo_.isUndefined|signalInfo.isUndefined) == false
            && (signalInfo_.isGssp|signalInfo.isGssp) == false
            && (signalInfo_.isGsspFilter|signalInfo.isGsspFilter) == false
            && type == 0)
    {
        return;
    }

    this->verticalScrollBar()->setSliderPosition(0);
    signalInfo_ = signalInfo;
    QStringList typeResult;
    getSampleStartEndBySampleName(signalInfo_.sampleName.toAscii(), alignStart_, alignEnd_);
    if(signalInfo_.isUndefined)
    {

    }
    else if(signalInfo_.isCombined)
    {
        getCombinedFromRealTimeDatabase(signalInfo_.sampleName.toAscii(), typeResult);
    }
    else if(signalInfo_.isGsspFilter)
    {
        getGsspFilterFromRealTimeDatabase(signalInfo_.fileName.toAscii(), typeResult);
    }
    else if(signalInfo_.isGssp)
    {
        getGsspResultFromRealTimeDatabase(signalInfo_.fileName.toAscii(), typeResult);
    }
    else
    {
        getTypeResultFromRealTimeDatabse(signalInfo_.sampleName.toAscii(), typeResult);
    }
    setData(typeResult);
    setGssp();
}

void MatchListWidget::slotShieldIndelAllelePair(bool isShield)
{
    isShieldIndel_ = isShield;
}

void MatchListWidget::slotShowGsspZCode()
{
    GsspInfoDlg gssp;
    gssp.setSampleName(signalInfo_.sampleName);
    gssp.setGsspTable1(gsspInfos_);
    gssp.exec();
}

void MatchListWidget::slotSetFinalType()
{
    if(signalInfo_.isGssp && !signalInfo_.isGsspFilter)
        return;
    QVector<QStringList> result;

    for(int i=0; i<this->rowCount(); i++)
    {
        if(item(i, 0)->text().at(0).toAscii() != '0')
            break;
        QStringList line;
        line.push_back(item(i,0)->text());
        line.push_back(item(i,1)->text());
        line.push_back(item(i,2)->text());
        result.push_back(line);
    }
    FinalTypeDlg finalTypeDlg;
    finalTypeDlg.setSampleName(signalInfo_.sampleName);
    finalTypeDlg.setAlleleResult(result);
    finalTypeDlg.exec();
}

void MatchListWidget::setGssp()
{
    QVector<AllelePair> allelePairs;
    gsspInfos_.clear();
    if(signalInfo_.isGssp || signalInfo_.isCombined || signalInfo_.isUndefined || signalInfo_.isGsspFilter)
        return;
    for(int i=0; i<rowCount(); i++)
    {
        if(!item(i, 0)->text().contains("0"))
            break;
        AllelePair ap;
        ap.allele1 = item(i, 1)->text();
        ap.allele2 = item(i, 2)->text();
        allelePairs.push_back(ap);
    }
    if(allelePairs.size()<2)
        return;

    bool r = processGssp(allelePairs, signalInfo_.sampleName, gsspInfos_);
    if(r)
    {
        for(int i=0; i<allelePairs.size(); i++)
        {
            item(i,4)->setText("Yes");
        }
    }
}

void MatchListWidget::setConfig()
{
    isShieldIndel_ = getIgnoreIndel();
}
