#include "matchlistwidget.h"
#include <QHeaderView>
#include "DataBase/soaptypingdb.h"
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "Dialog/gsspinfodlg.h"
#include "Dialog/finaltypedlg.h"
#include <QContextMenuEvent>
#include "log/log.h"
#include "Core/core.h"

const int I_COLNUM = 5;
const int I_ROWNUM = 500;
const int MIN_GAP = 50;

MatchListWidget::MatchListWidget(QWidget *parent)
    :QTableWidget(parent)
{
    m_bRefresh = false;
    InitUI();
    CreateRightMenu();
    ConnectSignalandSlot();
}

MatchListWidget::~MatchListWidget()
{

}

void MatchListWidget::InitUI()
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);

    verticalHeader()->setVisible(false);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setFixedHeight(40);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    setShowGrid(false);
    setRowCount(I_ROWNUM);
    setColumnCount(I_COLNUM);

    QStringList defaultHead_;
    defaultHead_<<"  Mis"<<"Allele1"<<"Allele2"<<"Info"<<"GSSP";
    this->setColumnWidth(0,40);
    this->setColumnWidth(1,100);
    this->setColumnWidth(2,100);
    this->setColumnWidth(3,55);
    this->setColumnWidth(4,40);
    this->setHorizontalHeaderLabels(defaultHead_);
    QTableWidgetItem *itemArray = new QTableWidgetItem[I_ROWNUM * I_COLNUM];
    for(int i=0; i<I_ROWNUM; i++)
    {
        for(int j=0; j<I_COLNUM; j++)
        {
            itemArray->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            this->setItem(i, j, itemArray++);
            if(i%2==0)
            {
                item(i,j)->setBackgroundColor(QColor(235,244,223));
            }
            else
            {
                item(i,j)->setBackgroundColor(QColor(255,255,255));
            }
        }
    }
    m_iRowCount = 0;
}

void MatchListWidget::ClearTable()
{
    for(int i=0; i<m_iRowCount; i++)
    {
        for(int j=0;j<5;j++)
        {
            item(i,j)->setText("");
        }
    }
}

void MatchListWidget::SetTableData(const QString &str_sample, const QString &str_file,
                                   const QString &str_info, int col)
{
    LOG_DEBUG("%s",str_file.toStdString().c_str());
    if(!m_bRefresh)//如果不要求刷新，需要判断是否切换了样品
    {
        if(m_str_SampleName != str_sample || m_str_file != str_file || m_iCol != col)
        {
            m_str_SampleName = str_sample;
            m_str_file = str_file;
            m_iCol = col;
        }
        else
        {
            return;
        }
    }
    else
    {
        m_bRefresh = false;
    }

    bool b_setgssp = false;
    m_strlist_result.clear();
    ClearTable();
    if(str_file.contains("Combined"))
    {
        SoapTypingDB::GetInstance()->getResultDataFromsampleTable(str_sample,true,m_strlist_result);
    }
    else if(col == 1)//样品列表选中的是第1列
    {
        SoapTypingDB::GetInstance()->getResultDataFromGsspTable(m_str_file,true,false,m_strlist_result);
    }
    else if(str_info.contains("Filter"))//样品列表选中的是第0列，且文件是gssp
    {
        SoapTypingDB::GetInstance()->getResultDataFromGsspTable(m_str_file,false,true,m_strlist_result);
    }
    else
    {
        b_setgssp = true;
        SoapTypingDB::GetInstance()->getResultDataFromsampleTable(str_sample,false,m_strlist_result);
    }

    m_iRowCount = m_strlist_result.size();
    int i_row = 0;
    QString strig('1');
    bool bignore = true;
    Core::GetInstance()->GetConfig("Set/Ignore", strig);
    if(strig == '0')
    {
        bignore = false;
    }
    for(int i=0; i<m_iRowCount; i++)
    {
        QStringList line = m_strlist_result.at(i).split(",");
        if(line.size() > 3 && line.at(3).toInt()!=0) //该型别对存在插入缺失的情况
        {
            line[0].append("*");
            if(bignore)
            {
                continue;
            }
        }
        line.removeAt(3);
        for(int j=0;j<line.size();j++)
        {
            this->item(i_row, j)->setText(line.at(j));
        }
        i_row++;
        if(i_row >= I_ROWNUM)
        {
            m_iRowCount = I_ROWNUM;
            break;
        }
    }
    m_iRowCount = i_row;
    m_iOldRow = -1;
    setCurrentItem(NULL);

    update();
    if(m_iRowCount && b_setgssp)
    {
        m_str_SampleName = str_sample;
        //setGssp();
    }
}

void MatchListWidget::setAllelePairData()
{
    setCurrentItem(item(0, 1));
    QString allele1 = item(0, 1)->text();
    QString allele2 = item(0, 2)->text();
    if(allele2.isEmpty())
    {
        allele2 = allele1;
    }

    emit signalAllelePair(allele1, allele2);
}

QStringList & MatchListWidget::GetMatchList()
{
    return m_strlist_result;
}

void MatchListWidget::setGssp()
{
    QVector<AllelePair> allelePairs;
    m_vec_gsspInfo.clear();
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

    bool r = processGssp(allelePairs, m_str_SampleName, m_vec_gsspInfo);
    if(r)
    {
        for(int i=0; i<allelePairs.size(); i++)
        {
            item(i,4)->setText("Yes");
        }
    }
}

void MatchListWidget::CreateRightMenu()
{
    m_pRightMenu = new QMenu(this);

    m_pActShowGSSPZCode = new QAction(tr("Show GSSP Z Code"), this);
    m_pActSetFinalType = new QAction(tr("Set Final Type"), this);

    m_pRightMenu->addAction(m_pActShowGSSPZCode);
    m_pRightMenu->addAction(m_pActSetFinalType);
}

void MatchListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QTableWidgetItem *itemNow = itemAt(event->pos());
    if(itemNow->row() < m_iRowCount)
    {
        m_pRightMenu->exec(QCursor::pos());
        event->accept();
    }
}

void MatchListWidget::ConnectSignalandSlot()
{
    connect(this, &QTableWidget::itemDoubleClicked, this, &MatchListWidget::slotClickIndelItem);
    connect(this, &QTableWidget::itemClicked, this, &MatchListWidget::slotRowChanged);

    connect(m_pActShowGSSPZCode, &QAction::triggered, this, &MatchListWidget::slotShowGsspZCode);
    connect(m_pActSetFinalType, &QAction::triggered, this, &MatchListWidget::slotSetFinalType);
}

void MatchListWidget::slotClickIndelItem(QTableWidgetItem* itemNow)
{
    if(itemNow->column()==0 && itemNow->text().contains('*'))
    {
        QString allele1 = item(itemNow->row(), 1)->text();
        QString allele2 = item(itemNow->row(), 2)->text();
        IndelInfo indelInfo1, indelInfo2;
        SoapTypingDB::GetInstance()->getIndelInfoFromalleleTable(allele1, indelInfo1);
        SoapTypingDB::GetInstance()->getIndelInfoFromalleleTable(allele2, indelInfo2);
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
            {
                if(indelInfo1.isIndel==1 && indelInfo2.isIndel==1)
                {
                    SoapTypingDB::GetInstance()->updateShieldAllelesToSampleTable(m_str_SampleName,
                                                                                  QString("%1;%2").arg(allele1).arg(allele2));
                }
                else if(indelInfo1.isIndel==1)
                {
                    SoapTypingDB::GetInstance()->updateShieldAllelesToSampleTable(m_str_SampleName, allele1);
                }
                else if(indelInfo2.isIndel==1)
                {
                    SoapTypingDB::GetInstance()->updateShieldAllelesToSampleTable(m_str_SampleName, allele2);
                }

                emit signalChangeDB(m_str_SampleName);
                break;
            }
        case QMessageBox::No:
            break;
        default:
            break;
        }
    }
}

void MatchListWidget::slotRowChanged(QTableWidgetItem* itemNow)
{
    int row = itemNow->row();

    if(row == m_iOldRow || row > m_iRowCount || itemNow->text().isEmpty())
    {
        return;
    }

    m_iOldRow = row;
    QString allele1 = item(row, 1)->text();
    QString allele2 = item(row, 2)->text();
    if(allele2.isEmpty())
    {
        allele2 = allele1;
    }

    emit signalAllelePair(allele1, allele2);
}


void MatchListWidget::slotShowGsspZCode()
{
    setGssp();
    GsspInfoDlg gssp(this);
    gssp.setSampleName(m_str_SampleName);
    gssp.setGsspTable1(m_vec_gsspInfo);
    gssp.exec();
}

void MatchListWidget::slotSetFinalType()
{
    QVector<QStringList> result;

    for(int i=0; i<this->rowCount(); i++)
    {
        QStringList line;
        line.push_back(item(i,0)->text());
        line.push_back(item(i,1)->text());
        line.push_back(item(i,2)->text());
        result.push_back(line);
    }

    FinalTypeDlg finalTypeDlg;
    finalTypeDlg.setSampleName(m_str_SampleName);
    finalTypeDlg.setAlleleResult(result);
    finalTypeDlg.exec();
}


void findUsefulGssp_new(const char *seq11, const char *seq12, const char *seq21, const char *seq22,
                    int exonStart, int exonEnd, QVector<GsspTable> &gsspTables,
                    QStringList &gssps, QStringList &infos)
{
    QSet<int> dif_pos;
    for(int i=exonStart; i<exonEnd; i++)
    {
        if(seq11[i]!='*' && seq21[i]!='*'&& seq11[i]!=seq21[i])
        {
            dif_pos.insert(i);
        }
    }

    for(int i=exonStart; i<exonEnd; i++)
    {
        if(seq12[i]!='*' && seq22[i]!='*'&& seq12[i]!=seq22[i])
        {
            dif_pos.insert(i);
        }
    }
    if(dif_pos.size()==0)
        return;

    foreach(const GsspTable & gsspinfo, gsspTables)
    {
        if(dif_pos.contains(gsspinfo.position-1))
        {
            gssps.push_back(gsspinfo.gsspName);
            QChar ch11 = seq11[gsspinfo.position];
            QChar ch12 = seq12[gsspinfo.position];
            QChar ch21 = seq21[gsspinfo.position];
            QChar ch22 = seq22[gsspinfo.position];

            infos.push_back(QString("%1:%2:%3:%4:%5:%6:%7:%8:%9").arg(ch11).arg(ch12).arg(ch21).arg(ch22)
                            .arg(gsspinfo.position)
                            .arg(gsspinfo.exonIndex).arg(gsspinfo.rOrF)
                            .arg(gsspinfo.gsspName)
                            .arg(QString(gsspinfo.base)));
        }
    }


}

/*void MatchListWidget::findUsefulGssp(const char *seq11, const char *seq12, const char *seq21, const char *seq22,
                    int exonStart, int exonEnd, QVector<GsspTable> &gsspTables,
                    QStringList &gssps, QStringList &infos)
{
    QVector<int> pos;
    for(int i=exonStart; i<exonEnd; i++)
    {
        if(seq11[i]!=seq12[i])
        {
            pos.push_back(i);
        }
    }
    if(pos.size()==0)
        return;

    QMap<int, int> same1;
    QMap<int, int> same2;
    QMap<int, int> posM;

    for(int i=0; i<pos.size(); i++)
    {
        if(seq11[pos[i]] == seq21[pos[i]])
        {
            same1.insert(pos[i], 0);
        }
        else
        {
            same2.insert(pos[i], 0);
        }
        posM.insert(pos[i], 0);
    }
    if(same1.size()==0 || same2.size()==0)
        return;

    bool flag = true;
    QMap<int, int> *sameP=NULL, *diffP=NULL;
    if(same1.size() >= same2.size())
    {
        sameP = &same1;
        diffP = &same2;
        flag = true; //13, 24
    }
    else
    {
        sameP = &same2;
        diffP = &same1;
        flag = false; //13, 23
    }

    int sb = sameP->begin().key(), se = (sameP->end()--).key();
    int db = diffP->begin().key(), de = (diffP->end()--).key();

    for(int i=0; i<gsspTables.size(); i++)
    {
        int point = gsspTables.at(i).position;
        if(!posM.contains(point))
            continue;

        if(sameP->contains(point))
        {
            if(gsspTables.at(i).rOrF.contains("F"))
            {
                if(de - point < MIN_GAP )
                    continue;
            }
            else
            {
                if(point - db < MIN_GAP)
                    continue;
            }
        }
        else
        {
            if(gsspTables.at(i).rOrF.contains("F"))
            {
                if(se - point < MIN_GAP)
                    continue;
            }
            else
            {
                if(point - sb < MIN_GAP)
                    continue;
            }
        }
        const char *gsspSeq = gsspTables.at(i).base.data();
        bool flag2 = false;
        if(flag)
        {
            for(int j=0; j<strlen(gsspSeq); j++)
            {
                if(seq11[point+j] != gsspSeq[j] || seq21[point+j] != gsspSeq[j])
                {
                    flag2 = true;
                    break;
                }
            }
            if(flag2)
            {
                for(int j=0; j<strlen(gsspSeq); j++)
                {
                    if(seq12[point+j] != gsspSeq[j] || seq22[point+j] != gsspSeq[j])
                    {
                        flag2 = true;
                        break;
                    }
                }
            }
        }
        else
        {
            for(int j=0; j<strlen(gsspSeq); j++)
            {
                if(seq11[point+j] != gsspSeq[j] || seq22[point+j] != gsspSeq[j])
                {
                    flag2 = true;
                    break;
                }
            }
            if(flag2)
            {
                for(int j=0; j<strlen(gsspSeq); j++)
                {
                    if(seq12[point+j] != gsspSeq[j] || seq21[point+j] != gsspSeq[j])
                    {
                        flag2 = true;
                        break;
                    }
                }
            }

        }
        if(flag2)
            continue;
        QString s11,s12,s21,s22;
        for(int j=0; j<strlen(gsspSeq); j++)
        {
            s11.push_back(seq11[point+j]);
            s12.push_back(seq12[point+j]);
            s21.push_back(seq21[point+j]);
            s22.push_back(seq22[point+j]);
        }
        //do something
        gssps.push_back(gsspTables.at(i).gsspName);
        infos.push_back(QString("%1:%2:%3:%4:%5:%6:%7:%8:%9").arg(s11).arg(s12).arg(s21).arg(s22)
                        .arg(point)
                        .arg(gsspTables.at(i).exonIndex).arg(QString(gsspTables.at(i).rOrF))
                        .arg(QString(gsspTables.at(i).gsspName))
                        .arg(QString(gsspTables.at(i).base)));
    }
}*/

bool MatchListWidget::processGssp(const QVector<AllelePair> &allelePairs, const QString &sampleName, QVector<QStringList> &result)
{
    if(allelePairs.size()<2)
        return false;
    QMap<QString, QByteArray> seqMap;
    for(int i=0; i<allelePairs.size(); i++)
    {
        if(seqMap.find(allelePairs.at(i).allele1) == seqMap.end())
        {
            QByteArray seq;
            SoapTypingDB::GetInstance()->getAlleleSequence(allelePairs.at(i).allele1, seq);
            seqMap.insert(allelePairs.at(i).allele1, seq);
        }
        if(seqMap.find(allelePairs.at(i).allele2) == seqMap.end())
        {
            QByteArray seq;
            SoapTypingDB::GetInstance()->getAlleleSequence(allelePairs.at(i).allele2, seq);
            seqMap.insert(allelePairs.at(i).allele2, seq);
        }
    }

    int exonStart, exonEnd;
    QByteArray geneName;
    SoapTypingDB::GetInstance()->getExonIndexAndGeneBySampleName(sampleName, exonStart, exonEnd, geneName);

    QVector<GsspTable> gsspTable;
    SoapTypingDB::GetInstance()->getGsspTablesFromGsspDatabase(geneName, 2, gsspTable);
//    QVector<GsspTable> gsspTable2;
//    QVector<GsspTable> gsspTable3;
//    if(exonStart<=2 && exonEnd >=2)
//    {
//        SoapTypingDB::GetInstance()->getGsspTablesFromGsspDatabase(geneName, 2, gsspTable2);
//    }
//    if(exonStart<=3 && exonEnd >=3)
//    {
//        SoapTypingDB::GetInstance()->getGsspTablesFromGsspDatabase(geneName, 3, gsspTable3);
//    }
    QVector<int> exonIndex;
    SoapTypingDB::GetInstance()->getExonPositionIndexFromStaticDatabase(geneName, exonIndex);
    for(int i=0; i<allelePairs.size(); i++)
    {
        const char *seq11 = (seqMap.find(allelePairs.at(i).allele1).value()).data();
        const char *seq12 = (seqMap.find(allelePairs.at(i).allele2).value()).data();
        for(int j=i+1; j<allelePairs.size(); j++)
        {
            QStringList gssps, infos;
            const char *seq21 = (seqMap.find(allelePairs.at(j).allele1).value()).data();
            const char *seq22 = (seqMap.find(allelePairs.at(j).allele2).value()).data();

            findUsefulGssp_new(seq11, seq12, seq21, seq22,
                               exonIndex.at(1),exonIndex.at(3), gsspTable, gssps, infos);
//            if(gsspTable2.size()>0)
//            {
//                findUsefulGssp(seq11, seq12, seq21, seq22,
//                               exonIndex.at(1),exonIndex.at(2), gsspTable2, gssps, infos
//                               );
//            }
//            if(gsspTable3.size()>0)
//            {
//                findUsefulGssp(seq11, seq12, seq21, seq22,
//                               exonIndex.at(2),exonIndex.at(3), gsspTable3, gssps, infos
//                               );
//            }

            if(gssps.size()==0)
                continue;
            QStringList re;
            re.push_back(allelePairs.at(i).allele1);
            re.push_back(allelePairs.at(i).allele2);
            re.push_back(allelePairs.at(j).allele1);
            re.push_back(allelePairs.at(j).allele2);
            re.push_back(gssps.join(":"));
            re.push_back(infos.join(";"));
            result.push_back(re);
            if(result.size()>50)
            {
                return true;
            }
        }
    }
    if(result.size()==0)
        return false;
    return true;
}
