#include "exonnavigatorwidget.h"
#include <QPainter>
#include <QtDebug>
#include <QMouseEvent>
#include <QtAlgorithms>
#include "DataBase/soaptypingdb.h"
#include "log/log.h"

ExonNavigatorWidget::ExonNavigatorWidget(QWidget *parent)
    :QWidget(parent)
{
    m_bRefresh = false;
    m_iheight = 100;
    m_itop1 = m_iheight*0.45;
    m_itop2 = m_iheight*0.5;
    m_itop3 = m_iheight*0.7+1;
    m_ih0 = m_iheight*0.3;
    m_ih1 = m_iheight*0.5;
    m_ih2 = m_iheight*0.4;
    m_ih3 = m_iheight*0.2;
    m_iMidgap = 40;
    m_igap = 20;
    m_isub_pos = 0;
    m_dXscale = 0.0;
    m_vec_Exon.reserve(4);
}

ExonNavigatorWidget::~ExonNavigatorWidget()
{

}

void ExonNavigatorWidget::ClearExonNav()
{
    m_vecExonIndex.clear();
    m_Exoninfo.maxExonIndex = 0;
    m_Exoninfo.minExonIndex = 0;
    m_Exoninfo.vec_editPos.clear();
    m_Exoninfo.vec_frMis.clear();
    m_Exoninfo.vec_frUnEqual.clear();
    m_Exoninfo.vec_pcMis.clear();
    m_map_mispos.clear();
    m_map_typemispos.clear();
    m_map_TotalMisPos.clear();
    m_vec_Exon.clear();
    m_isub_pos = 0;
    m_iStartPeakpos = 0;
}

void ExonNavigatorWidget::SetExonData(QString &str_sample, QString &str_gene)
{
    LOG_DEBUG("%s",str_sample.toStdString().c_str());
    if(!m_bRefresh)//如果不要求刷新，需要判断是否切换了样品
    {
        if(m_str_SampleName !=str_sample || m_str_GeneName != str_gene)
        {
            m_str_SampleName = str_sample;
            m_str_GeneName = str_gene;
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

    ClearExonNav();

    SoapTypingDB::GetInstance()->getExonPositionIndex(str_gene, m_vecExonIndex);
    if(m_vecExonIndex.empty())
    {
        return;
    }

    SoapTypingDB::GetInstance()->getExonNavigatorInfo(str_sample, m_Exoninfo);

    m_isub_pos = m_vecExonIndex[m_Exoninfo.maxExonIndex] - m_vecExonIndex[m_Exoninfo.minExonIndex-1];
    m_iStartPeakpos = m_vecExonIndex[m_Exoninfo.minExonIndex-1];
    assert(m_isub_pos> 0);

    foreach(int pos,m_Exoninfo.vec_frUnEqual)
    {
        m_map_mispos.insert(pos,0);
    }

    foreach(int pos,m_Exoninfo.vec_editPos)
    {
        m_map_mispos.insert(pos,0);
    }

    foreach(int pos,m_Exoninfo.vec_frMis)
    {
        m_map_mispos.insert(pos,0);
    }

    foreach(int pos,m_Exoninfo.vec_pcMis)
    {
        m_map_mispos.insert(pos,0);
    }

    m_map_TotalMisPos = m_map_mispos;
    CalcExonData();
}

void ExonNavigatorWidget::CalcExonData()
{
    int i_width = width();
    m_vec_Exon.clear();
    int sub_index = m_Exoninfo.maxExonIndex - m_Exoninfo.minExonIndex;
    m_dXscale = (i_width - 2*m_igap - sub_index*m_iMidgap)*1.0/m_isub_pos;

    for(int i=m_Exoninfo.minExonIndex; i<=m_Exoninfo.maxExonIndex; i++)
    {
        Exon exon;
        exon.i_exonstartpos = m_vecExonIndex[i-1]+1;
        exon.i_exonendpos= m_vecExonIndex[i];
        exon.i_screenstartpos = m_igap+(m_vecExonIndex[i-1]-m_iStartPeakpos)*m_dXscale
                +(i-m_Exoninfo.minExonIndex)*m_iMidgap;//碱基导航条起始坐标
        exon.i_screenwidth = (m_vecExonIndex[i] - m_vecExonIndex[i-1])*m_dXscale;//碱基导航条长度
        exon.i_exonindex = i;
        m_vec_Exon.push_back(exon);
    }
}

void ExonNavigatorWidget::resizeEvent(QResizeEvent *event)
{
    if(m_isub_pos)
    {
        CalcExonData();
    }
}

void ExonNavigatorWidget::paintEvent(QPaintEvent *event)
{
    QPainter exonPainter(this);
    int i_width = width();
    exonPainter.setPen(QColor(139,139,139));
    exonPainter.setBrush(Qt::white);
    exonPainter.drawRect(QRect(0,0,i_width-1,m_iheight -1));

    if(m_isub_pos)
    {
        DrawExonArea(exonPainter);
        DrawSelectFrame(exonPainter);
        DrawExonPos(exonPainter);
    }
}

void ExonNavigatorWidget::DrawExonArea(QPainter &exonPainter)
{
    QFont fontRegion;
    QFont fontPos;
    fontRegion.setPointSize(20);
    fontRegion.setWeight(QFont::Black);
    fontRegion.setFamily(tr("微软雅黑"));
    fontPos.setPointSize(12);

    foreach(const Exon &exon, m_vec_Exon)
    {
        int x_pos = exon.i_screenstartpos;
        int width = exon.i_screenwidth;
        int x_pos_index = x_pos + (width-80)/2;//碱基导航条index起始坐标,默认长度为80

        exonPainter.setBrush(QBrush(QColor(235,244,223)));
        exonPainter.drawRect(x_pos, m_itop2, width, m_ih2);
        QRect recti(x_pos_index, 0, 80, m_ih0);
        exonPainter.setBrush(Qt::NoBrush);
        exonPainter.setFont(fontRegion);
        exonPainter.drawText(recti, Qt::AlignHCenter|Qt::AlignCenter, QString("Exon%1").arg(exon.i_exonindex));
        exonPainter.setFont(fontPos);
        QRect recti1(x_pos, 0, width, m_ih1);
        exonPainter.drawText(recti1, Qt::AlignLeft|Qt::AlignBottom, QString::number(exon.i_exonstartpos));
        exonPainter.drawText(recti1, Qt::AlignRight|Qt::AlignBottom, QString::number(exon.i_exonendpos));
    }
}

void ExonNavigatorWidget::DrawSelectFrame(QPainter &exonPainter)
{
    int x_pos = PeakPosToScreenPos(m_iSelectPeakPos);
    exonPainter.setPen(QColor(136,136,136));
    exonPainter.setBrush(QBrush(QColor(255,255,0,127)));
    exonPainter.drawRect(QRect(x_pos-15, m_itop1-15, 25, m_ih1+15));
    exonPainter.setBrush(Qt::NoBrush);
    exonPainter.setPen(Qt::black);
    exonPainter.drawText(QRect(x_pos-15, 0, 25, m_ih1), Qt::AlignBottom|Qt::AlignCenter,
                         QString::number(m_iSelectPeakPos));
}

void ExonNavigatorWidget::DrawExonPos(QPainter &exonPainter)
{
    exonPainter.setPen(Qt::NoPen);
    exonPainter.setBrush(QColor(0,0,255));
    foreach(int pos,m_Exoninfo.vec_frUnEqual)
    {
        exonPainter.drawRect(PeakPosToScreenPos(pos), m_itop2, 2, m_ih3);
    }

    exonPainter.setBrush(QColor(0,0,0));
    foreach(int pos,m_Exoninfo.vec_editPos)
    {
        exonPainter.drawRect(PeakPosToScreenPos(pos), m_itop2, 2, m_ih3);
    }

    exonPainter.setBrush(QColor(57,181,74));
    foreach(int pos,m_Exoninfo.vec_frMis)
    {
        exonPainter.drawRect(PeakPosToScreenPos(pos), m_itop2, 2, m_ih3);
    }

    exonPainter.setBrush(QColor(237,28,36));
    foreach(int pos,m_Exoninfo.vec_pcMis)
    {
        exonPainter.drawRect(PeakPosToScreenPos(pos), m_itop2, 2, m_ih3);
    }

    foreach(int pos, m_map_typemispos.keys())
    {
        exonPainter.drawRect(PeakPosToScreenPos(pos), m_itop3, 2, m_ih3);
    }
}

void ExonNavigatorWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint pos = event->pos();
        int x_pos = pos.x();
        foreach(const Exon &exon, m_vec_Exon)
        {
            if(x_pos >= exon.i_screenstartpos && x_pos <= exon.i_screenstartpos+exon.i_screenwidth)
            {
                m_iSelectPeakPos = ScreenPosToPeakPos(x_pos);
                update();
                emit signalExonFocusPosition(m_iStartPeakpos+1,
                        m_iSelectPeakPos,
                        exon.i_exonstartpos, exon.i_exonindex);
                break;
            }
        } 
    }
}

int ExonNavigatorWidget::PeakPosToScreenPos(int peakpos) //峰图坐标转换成显示坐标
{
    int screenpos = 0;

    foreach(const Exon &exon, m_vec_Exon)
    {
        if(peakpos >= exon.i_exonstartpos && peakpos <= exon.i_exonendpos)
        {
            screenpos = (peakpos - m_iStartPeakpos)*m_dXscale
                    + m_igap + (exon.i_exonindex-m_Exoninfo.minExonIndex)*m_iMidgap;
            break;
        }
    }
    return screenpos;
}

int ExonNavigatorWidget::ScreenPosToPeakPos(int screenpos) //显示坐标转换成峰图坐标
{
    int peakpos = 0;
    foreach(const Exon &exon, m_vec_Exon)
    {
        if(screenpos > exon.i_screenstartpos && screenpos < exon.i_screenstartpos+exon.i_screenwidth)
        {
            peakpos = (screenpos - m_igap - (exon.i_exonindex-m_Exoninfo.minExonIndex)*m_iMidgap)/m_dXscale
                    + m_iStartPeakpos+1;
            break;
        }
        else if (screenpos == exon.i_screenstartpos)
        {
            peakpos = exon.i_exonstartpos;
             break;
        }
        else if (screenpos == exon.i_screenstartpos+exon.i_screenwidth)
        {
            peakpos = exon.i_exonendpos;
             break;
        }
    }
    return peakpos;
}

void ExonNavigatorWidget::SetSelectPos(int colnum, int &selectpos, int &exonstartpos, int &index)
{
    selectpos = colnum + m_iStartPeakpos;
    foreach(const Exon &exon, m_vec_Exon)
    {
        if(selectpos >= exon.i_exonstartpos && selectpos <= exon.i_exonendpos)
        {
            m_iSelectPeakPos = selectpos;
            exonstartpos = exon.i_exonstartpos;
            index = exon.i_exonindex;
            break;
        }
    }
    update();
}

void ExonNavigatorWidget::setSelectFramePosition(int index, int &startpos, int &selectpos, int &exonstartpos)
{  
    foreach(const Exon &exon, m_vec_Exon)
    {
        if(exon.i_exonindex == index)
        {
            startpos = m_iStartPeakpos;
            exonstartpos = exon.i_exonstartpos;

            foreach(int peakpos, m_map_TotalMisPos.keys())
            {
                if(peakpos > exon.i_exonstartpos && peakpos < exon.i_exonendpos)
                {
                    peakpos++;//从数据库获取的要加1？？？？
                    m_iSelectPeakPos = peakpos;
                    selectpos = peakpos;
                    update();
                    return;
                }
            }

            //如果没有错配位点，默认为导航条起始位置
            m_iSelectPeakPos = exon.i_exonstartpos;
            selectpos = exon.i_exonstartpos;
            update();
            break;
        }
    }
}

void ExonNavigatorWidget::SetSelectFramePos(int index, int colnum, int &columnPos)
{
    foreach(const Exon &exon, m_vec_Exon)
    {
        if(exon.i_exonindex == index)
        {
            int selectpos = colnum + exon.i_exonstartpos;
            columnPos = selectpos  - m_iStartPeakpos -1;
            if(selectpos >= exon.i_exonstartpos && selectpos <= exon.i_exonendpos)
            {
                m_iSelectPeakPos = selectpos;
                break;
            }
        }
    }
    update();
}

void ExonNavigatorWidget::ActForward()
{
    auto itor = m_map_TotalMisPos.keyBegin();
    auto end = m_map_TotalMisPos.keyEnd();

    if(m_iSelectPeakPos >= m_map_TotalMisPos.lastKey())
    {
        m_iSelectPeakPos = m_map_TotalMisPos.firstKey()+1;
    }
    else
    {
        for(;itor != end; itor++)
        {
            if(m_iSelectPeakPos <= *itor)
            {
                m_iSelectPeakPos = *itor+1;
                break;
            }
        }
    }

    update();

    foreach(const Exon &exon, m_vec_Exon)
    {
        if(m_iSelectPeakPos >= exon.i_exonstartpos && m_iSelectPeakPos <= exon.i_exonendpos)
        {
            emit signalExonFocusPosition(m_iStartPeakpos+1,
                    m_iSelectPeakPos,
                    exon.i_exonstartpos, exon.i_exonindex);
            break;
        }
    }
}

void ExonNavigatorWidget::ActBackward()
{
    auto end = m_map_TotalMisPos.keyEnd();

    if(m_iSelectPeakPos <= *m_map_TotalMisPos.keyBegin()+1)
    {
        m_iSelectPeakPos = m_map_TotalMisPos.lastKey()+1;
    }
    else
    {
        auto itor =  end;
        for(itor--;; itor--)
        {
            if(m_iSelectPeakPos > *itor+1)
            {
                m_iSelectPeakPos = *itor+1;
                break;
            }
        }
    }

    update();

    foreach(const Exon &exon, m_vec_Exon)
    {
        if(m_iSelectPeakPos >= exon.i_exonstartpos && m_iSelectPeakPos <= exon.i_exonendpos)
        {
            emit signalExonFocusPosition(m_iStartPeakpos+1,
                    m_iSelectPeakPos,
                    exon.i_exonstartpos, exon.i_exonindex);
            break;
        }
    }
}

void ExonNavigatorWidget::SetTypeMisPos(QSet<int> &typeMismatchPos)
{
    m_map_typemispos.clear();
    m_map_TotalMisPos.clear();
    m_map_TotalMisPos = m_map_mispos;
    for(QSet<int>::iterator it=typeMismatchPos.begin(); it!=typeMismatchPos.end(); it++)
    {
        m_map_typemispos.insert(*it, 0);
        m_map_TotalMisPos.insert(*it, 0);
    }

    m_iSelectPeakPos = m_map_typemispos.firstKey()+1;
    update();
    foreach(const Exon &exon, m_vec_Exon)
    {
        if(m_iSelectPeakPos >= exon.i_exonstartpos && m_iSelectPeakPos <= exon.i_exonendpos)
        {
            emit signalExonFocusPosition(m_iStartPeakpos+1,
                    m_iSelectPeakPos,
                    exon.i_exonstartpos, exon.i_exonindex);
            break;
        }
    }
}

