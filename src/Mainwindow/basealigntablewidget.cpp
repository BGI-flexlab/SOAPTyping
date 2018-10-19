#include "basealigntablewidget.h"
#include <QHeaderView>
#include "DataBase/soaptypingdb.h"
#include "Core/core.h"
#include <QDebug>
#include <QScrollBar>
#include "log/log.h"

BaseAlignTableWidget::BaseAlignTableWidget()
{
    m_iRowNum = 9;
    m_iColNum = 1200;
    m_bRefresh = false;
    InitUI();
}


BaseAlignTableWidget::~BaseAlignTableWidget()
{

}

void BaseAlignTableWidget::InitUI()
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectColumns);
    verticalHeader()->setVisible(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setContextMenuPolicy(Qt::NoContextMenu);
    horizontalHeader()->setStretchLastSection(true);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    verticalHeader()->setDefaultSectionSize(20); //设置行高
    horizontalHeader()->setFixedHeight(20); //设置表头的高度
    horizontalHeader()->setHighlightSections(false);
    setRowCount(m_iRowNum);
    setColumnCount(m_iColNum);

    for(int i=1; i<m_iColNum; i++)
    {
        this->setColumnWidth(i,20);
        m_sl_defaulthead<<"";
    }

    setColumnWidth(0,230);
    setHorizontalHeaderLabels(m_sl_defaulthead);


    QTableWidgetItem *itemArray = new QTableWidgetItem[m_iRowNum * m_iColNum];
    for(int i=0; i<m_iRowNum; i++)
    {
        for(int j=0; j<m_iColNum; j++)
        {

            if(j == 0)
            {
                itemArray->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            }
            else {
                itemArray->setTextAlignment(Qt::AlignCenter);
            }
            this->setItem(i, j, itemArray);

            if (i == 0)
            {
                itemArray->setBackgroundColor(QColor(0, 169,157,51));
                itemArray->setTextColor(QColor(0, 169,157));
            }
            else if (i == 1)
            {
                itemArray->setBackgroundColor(QColor(140,98,57,51));
                itemArray->setTextColor(QColor(140,98,57));
            }
            else if (i == 2)
            {
                itemArray->setBackgroundColor(QColor(0,113,188,51));
                itemArray->setTextColor(QColor(0,113,188));
            }
            else if (i == 3)
            {
                itemArray->setBackgroundColor(QColor(247,147,30,51));
                itemArray->setTextColor(QColor(247,147,30));
            }
            else if (i == 4)
            {
                itemArray->setBackgroundColor(QColor(102,45,145,51));
                itemArray->setTextColor(QColor(102,45,145));
            }
            else if (i == 5)
            {
                itemArray->setBackgroundColor(QColor(57,181,74,51));
                itemArray->setTextColor(QColor(57,181,74));
            }
            else if (i == 6)
            {
                itemArray->setBackgroundColor(QColor(223, 223, 223));
                itemArray->setTextColor(QColor(237,30,121));
            }

            itemArray++;
        }
    }

    QFont font;
    font.setBold(true);
    font.setPointSize(12);
    for(int i=0; i<9; i++)
    {
        item(i, 0)->setFont(font);
    }


    item(0, 0)->setText("  Consensus Sequence");
    item(1, 0)->setText("  Forward Sequence");
    item(2, 0)->setText("  Reverse Sequence");
    item(3, 0)->setText("  GSSP Sequence");
    item(4, 0)->setText("  Consensus Alignment");
    item(5, 0)->setText("  Pattern Sequence");

}

void BaseAlignTableWidget::ClearBaseAlignTable()
{
    clearBaseAlignTableSampleItem();
    m_BaseAlignSampleInfo.clear();
}

void BaseAlignTableWidget::SetAlignTableData(QString &str_samplename,  QString &str_file,
                                             QString str_info, int col)
{
    LOG_DEBUG("%s",str_file.toStdString().c_str());
    m_str_file = str_file;
    m_str_info = str_info;
    m_i_col = col;

    if(!m_bRefresh)//如果不要求刷新，需要判断是否切换了样品
    {
        if(m_str_SampleName != str_samplename)
        {
            m_str_SampleName = str_samplename;
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

    ClearBaseAlignTable();

    SoapTypingDB::GetInstance()->getBaseAlignSampleInfo(str_samplename, m_BaseAlignSampleInfo);
    int i_startColumn = 1;
    if(m_BaseAlignSampleInfo.alignStartPos == m_BaseAlignSampleInfo.alignEndPos)
    {
        return;
    }

    QStringList header;
    header<<str_samplename;
    getTableHead(header, m_BaseAlignSampleInfo.alignEndPos - m_BaseAlignSampleInfo.alignStartPos, m_BaseAlignSampleInfo.alignStartPos);
    setHorizontalHeaderLabels(header);
    QString &line = m_BaseAlignSampleInfo.consensusSeq;
    for(int i=0; i<line.size(); i++)
    {
        item(0, i+i_startColumn)->setText(line.at(i));
        item(4, i+i_startColumn)->setText("-");
    }

    line = m_BaseAlignSampleInfo.forwardSeq;
    for(int i=0; i<line.size(); i++)
    {
        if(line.at(i)!='-')
        {
            item(1, i+i_startColumn)->setText(line.at(i));
        }
    }
    line = m_BaseAlignSampleInfo.reverseSeq;
    for(int i=0; i<line.size(); i++)
    {
        if(line.at(i)!='-')
        {
            this->item(2, i+i_startColumn)->setText(line.at(i));
        }
    }
    line = m_BaseAlignSampleInfo.patternSeq;
    for(int i=0; i<line.size(); i++)
    {
        if(line.at(i)!='-')
        {
            item(5, i+i_startColumn)->setText(line.at(i));
        }
    }

    for(QSet<int>::iterator it=m_BaseAlignSampleInfo.editPostion.begin(); it!=m_BaseAlignSampleInfo.editPostion.end(); it++)
    {
        char A = m_BaseAlignSampleInfo.forwardSeq[(*it)-m_BaseAlignSampleInfo.alignStartPos].toLatin1();
        char B = m_BaseAlignSampleInfo.reverseSeq[(*it)-m_BaseAlignSampleInfo.alignStartPos].toLatin1();
        char C = Core::GetInstance()->mergeBases(A, B);
        item(4, (*it)-m_BaseAlignSampleInfo.alignStartPos+i_startColumn)->setText(QString("%1").arg(C));
    }

    for(QSet<int>::iterator it=m_BaseAlignSampleInfo.frMisMatchPostion.begin(); it!=m_BaseAlignSampleInfo.frMisMatchPostion.end(); it++)
    {
        char A = m_BaseAlignSampleInfo.forwardSeq[(*it)-m_BaseAlignSampleInfo.alignStartPos].toLatin1();
        char B = m_BaseAlignSampleInfo.reverseSeq[(*it)-m_BaseAlignSampleInfo.alignStartPos].toLatin1();
        char C = Core::GetInstance()->mergeBases(A, B);
        item(4, (*it)-m_BaseAlignSampleInfo.alignStartPos+i_startColumn)->setText(QString("%1").arg(C));
    }

    for(QSet<int>::iterator it=m_BaseAlignSampleInfo.pcMisMatchPostion.begin(); it!=m_BaseAlignSampleInfo.pcMisMatchPostion.end(); it++)
    {
        char A = m_BaseAlignSampleInfo.forwardSeq[*it-m_BaseAlignSampleInfo.alignStartPos].toLatin1();
        char B = m_BaseAlignSampleInfo.reverseSeq[*it-m_BaseAlignSampleInfo.alignStartPos].toLatin1();
        char C = Core::GetInstance()->mergeBases(A, B);
        item(4, (*it)-m_BaseAlignSampleInfo.alignStartPos+i_startColumn)->setText(QString("%1").arg(C));
    }

    for(QMap<QString, BaseAlignGsspInfo>::iterator it=m_BaseAlignSampleInfo.gsspInfoMap.begin(); it!=m_BaseAlignSampleInfo.gsspInfoMap.end(); it++)
    {
        BaseAlignGsspInfo &info = it.value();
        line = info.gsspFileSeq;
        for(int i=0; i<line.size(); i++)
        {
            if(line.at(i)!='-')
            {
                item(3, i+info.gsspFileAlignStartPos - m_BaseAlignSampleInfo.alignStartPos + i_startColumn)->setText(line.at(i));
            }
        }
        line = info.gsspSeq;
        for(int i=0; i<line.size(); i++)
        {
            item(3, i+info.gsspPostion - m_BaseAlignSampleInfo.alignStartPos + i_startColumn)->setText(line.at(i));
        }
    }
}

void BaseAlignTableWidget::clearBaseAlignTableSampleItem()
{
    this->setHorizontalHeaderLabels(m_sl_defaulthead);
    item(6, 0)->setText("");
    item(7, 0)->setText("");
    item(8, 0)->setText("");
    for(int i=1; i<m_iColNum; i++)
    {
        item(0, i)->setText("");
        item(1, i)->setText("");
        item(2, i)->setText("");
        item(3, i)->setText("");
        item(4, i)->setText("");
        item(4, i)->setBackgroundColor(QColor(102,45,145,51));
        item(5, i)->setText("");
        item(6, i)->setText("");
        item(6, i)->setBackgroundColor(QColor(223, 223, 223));
        item(7, i)->setText("");
        item(8, i)->setText("");
    }
}

void BaseAlignTableWidget::getTableHead(QStringList &head, int length, int start)
{
    bool startWithNull = false;
    start += 1;
    for(int i=0; i<length; i++)
    {
        int pos = i + start;
        int TPos = (pos+2) / 1000;
        int HPos = ((pos+1) % 1000) / 100;
        int DPos = (pos % 100) / 10;
        int Ipos = (pos-1) % 10;
        if(pos/100)
        {
            Ipos = (pos-2) % 10;
        }
        switch(Ipos)
        {
        case 1:
            if(startWithNull)
            {
                head<<QString("%1").arg(1);
            }
            else
            {
                head<<QString("");
            }
            break;
        case 0:
            if(startWithNull&&(HPos>0 || TPos >0 || DPos >0))
            {
                head<<QString("%1").arg(DPos);
            }
            else
            {
                head<<"";
            }
            break;
        case 9:
            if(startWithNull&&(TPos>0 || HPos>0))
            {
                head<<QString("%1").arg(HPos);
            }
            else
            {
                head<<"";
            }
            break;
        case 8:
            if(TPos>0&&startWithNull)
            {
                head<<QString("%1").arg(TPos);
            }
            else
            {
                head<<"";
            }
            break;
        default:
            startWithNull = true;
            head<<"";
        }
    }

    if(m_iColNum>length+1)
    {
        for(int i=length+1; i<m_iColNum; i++)
        {
            head<<"";
        }
    }
}

void BaseAlignTableWidget::SetAllelePairData(QString &allele1, QString &allele2)
{
    if(allele1.isEmpty() || allele2.isEmpty())
    {
        return;
    }

    for(int i=1; i<m_iColNum; i++)
    {
        this->item(6, i)->setText("");
        this->item(7, i)->setText("");
        this->item(8, i)->setText("");
        this->item(6, i)->setBackgroundColor(QColor(223, 223, 223));
    }

    QByteArray allele1Seq, allele2Seq, result;
    SoapTypingDB::GetInstance()->getAlleleSequence(allele1, allele1Seq, m_BaseAlignSampleInfo.alignStartPos,
                      m_BaseAlignSampleInfo.alignEndPos-m_BaseAlignSampleInfo.alignStartPos);
    if(allele1 == allele2)
    {
        allele2Seq = allele1Seq;
    }
    else
    {
        SoapTypingDB::GetInstance()->getAlleleSequence(allele2, allele2Seq, m_BaseAlignSampleInfo.alignStartPos,
                                              m_BaseAlignSampleInfo.alignEndPos-m_BaseAlignSampleInfo.alignStartPos);
    }
    result.fill('-', allele1Seq.size());
    QSet<int> misMatch;
    if(m_str_info.contains("Filter") && m_i_col != 1)//文件是gssp文件，且选中的不是第1列
    {
        QMap<QString, BaseAlignGsspInfo>::iterator it = m_BaseAlignSampleInfo.gsspInfoMap.find(m_str_file);
        if(it != m_BaseAlignSampleInfo.gsspInfoMap.end())
        {
            QByteArray gsspSeq = it.value().gsspFileSeq.toLatin1();
            getTypeAlignResult(result.data(), gsspSeq.data(), allele1Seq.data(),
                               allele2Seq.data(), misMatch, m_BaseAlignSampleInfo.alignStartPos,
                               it.value().gsspFileAlignStartPos);
        }
    }
    else
    {
        QByteArray patternSeq = m_BaseAlignSampleInfo.patternSeq.toLatin1();
        getTypeAlignResult(result.data(), patternSeq.data(), allele1Seq.data(),
                           allele2Seq.data(), misMatch, m_BaseAlignSampleInfo.alignStartPos,
                           m_BaseAlignSampleInfo.alignStartPos);
    }

    QString results = QString(result);
    QString allele1Seqs = QString(allele1Seq);
    QString allele2Seqs = QString(allele2Seq);
    for(int i=0; i<result.size(); i++)
    {
        this->item(6, i+1)->setText(results.at(i));
        this->item(7, i+1)->setText(allele1Seqs.at(i));
        this->item(8, i+1)->setText(allele2Seqs.at(i));
    }
    this->item(6, 0)->setText("  Type Result");
    this->item(7, 0)->setText("  "+allele1);
    this->item(8, 0)->setText("  "+allele2);

    for(QSet<int>::iterator it=misMatch.begin(); it!=misMatch.end(); it++)
    {
        this->item(6, (*it)-m_BaseAlignSampleInfo.alignStartPos+1)->setTextColor(QColor(255,255,255));
        this->item(6, (*it)-m_BaseAlignSampleInfo.alignStartPos+1)->setBackgroundColor(QColor(255,0,0));
    }
    if(misMatch.size()>0)
    {
        emit signalTypeMisMatchPosition(misMatch, 0);
    }
}

void BaseAlignTableWidget::getTypeAlignResult(char *result, char *pattern, char *alleleSeq1, char *alleleSeq2,
                                              QSet<int> &misMatch, int alignStart1, int alignStart2)
{
    int size =strlen(pattern);
    int num = alignStart2 - alignStart1;
    for(int i=0; i<size; i++)
    {
        if(pattern[i]=='-')
            continue;
        if(alleleSeq1[i+num] == '*')
        {
            if(alleleSeq2[i+num]=='*')
                continue;
            else if (alleleSeq2[i+num] != pattern[i])
            {
                result[i+num]=alleleSeq2[i+num];
                misMatch.insert(i+alignStart2);
            }
        }
        else
        {
            if(alleleSeq2[i+num]=='*' || alleleSeq1[i+num]==alleleSeq2[i+num])
            {
                if(alleleSeq1[i+num] != pattern[i])
                {
                    result[i+num]=alleleSeq1[i+num];
                    misMatch.insert(i+alignStart2);
                }
            }
            else
            {
                char A = Core::GetInstance()->mergeBases(alleleSeq1[i+num],alleleSeq2[i+num]);
                if(A != pattern[i])
                {
                    result[i+num]=A;
                    misMatch.insert(i+alignStart2);
                }
            }
        }
    }
}

