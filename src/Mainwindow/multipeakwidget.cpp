#include "multipeakwidget.h"
#include <QPainter>
#include "DataBase/soaptypingdb.h"
#include <QDebug>
#include <QMouseEvent>
#include <QtAlgorithms>
#include <QScrollArea>
#include <QScrollBar>
#include <QTime>
#include <set>
#include "log/log.h"
#include <QFontDatabase>
#include <QToolTip>

const int PEAKLINEHIGHT = 160;
const int HLINEHIGHT = 20;
const int PEAK_X_STEP = 2;
const int PEAK_Y_STEP = 5;

PeakLine::PeakLine(long size):m_lsize(size)
{
    m_loffset = 0;
    m_left_exclude = 0;
    m_right_exclude = 0;
    m_iAlignStart = 0;
    m_iAlignEnd = 0;

    m_vec_baseA.reserve(m_lsize);
    m_vec_baseT.reserve(m_lsize);
    m_vec_baseG.reserve(m_lsize);
    m_vec_baseC.reserve(m_lsize);
    m_vec_GeneLetter.reserve(m_lsize);
}

PeakLine::~PeakLine()
{
    m_vec_baseA.clear();
    m_vec_baseT.clear();
    m_vec_baseG.clear();
    m_vec_baseC.clear();
    m_vec_GeneLetter.clear();
}

int PeakLine::getSize()
{
    return m_lsize;
}

void PeakLine::SetBasePoint(char type, double x, double y)
{
    switch (type)
    {
    case 'A':
        m_vec_baseA.push_back(QPointF(x,y));
        break;
    case 'T':
        m_vec_baseT.push_back(QPointF(x,y));
        break;
    case 'G':
        m_vec_baseG.push_back(QPointF(x,y));
        break;
    case 'C':
        m_vec_baseC.push_back(QPointF(x,y));
        break;
    }
}

 QPolygonF& PeakLine::GetBasePoint(char type)
 {
     switch (type)
     {
         case 'A': return m_vec_baseA;
         case 'T': return m_vec_baseT;
         case 'G': return m_vec_baseG;
         case 'C': return m_vec_baseC;
     }
 }

void PeakLine::AddGeneLetter(GeneLetter &geneletter)
{
    m_vec_GeneLetter.push_back(geneletter);
}

QVector<GeneLetter>& PeakLine::GetGeneLetter()
{
    return m_vec_GeneLetter;
}

void PeakLine::SetFileName(QString &str)
{
    m_strFileName = str;
}

QString& PeakLine::GetFileName()
{
    return m_strFileName;
}

void PeakLine::SetAlignPos(int start, int end)
{
    m_iAlignStart = start;
    m_iAlignEnd = end;
}

void PeakLine::SetExcludePos(int left, int right)
{
    if(left >= 0)
    {
        m_left_exclude = m_iAlignStart + left;
    }

    if(right >= 0)
    {
        m_right_exclude = m_iAlignEnd - right;
    }
}

void PeakLine::GetExcludePos(int &left, int &right)
{
    left = m_left_exclude;
    right = m_right_exclude;
}

void PeakLine::SetOffset(int offset)
{
    m_loffset = offset;
}

int PeakLine::GetOffset()
{
    return m_loffset;
}

void PeakLine::SetGssp(bool isGssp)
{
    m_bGssp = isGssp;
}
bool PeakLine::GetGssp()
{
    return  m_bGssp;
}

void PeakLine::setAvgWidth(float width)
{
    m_f_avgwidth = width;
}

float PeakLine::getAvgWidth()
{
    return m_f_avgwidth;
}

void PeakLine::setAvgSignal(float signal)
{
    m_f_avgsignal = signal;
}

float PeakLine::getAvgSignal()
{
    return m_f_avgsignal;
}

void PeakLine::setPeakYSetp(int y)
{
    m_y_step = y;
}

int PeakLine::getPeakYStep()
{
    return m_y_step;
}

void PeakLine::setPeakHeight(int height)
{
    m_iPeakHeight = height;
}

int PeakLine::getPeakHeight()
{
    return m_iPeakHeight;
}

void PeakLine::setXLeft(int x)
{
    m_x_left = x;
}

int PeakLine::getXLeft()
{
    return m_x_left;
}

void PeakLine::setDelpos(int pos)
{
    m_vec_del.push_back(pos);
}

QVector<int> &PeakLine::getDelpos()
{
    return m_vec_del;
}


MultiPeakWidget::MultiPeakWidget(QWidget *parent)
    :QWidget(parent)
{
    m_bRefresh = false;
    m_x_step = PEAK_X_STEP;
    m_bIsSelect = false;
    m_l_xSize = 0;
    m_index_PeakLine = 0;
    m_bApplyOne = false;
    m_bEditOne = true;
    setFocusPolicy(Qt::StrongFocus);//如果不调用，keyPressEvent不响应
    CreateRightMenu();
    ConnectSignalandSlot();
    //loadFontFromFile();
}

MultiPeakWidget::~MultiPeakWidget()
{

}

void MultiPeakWidget::loadFontFromFile()
{
    int loadedFontID = QFontDatabase::addApplicationFont("./font/msyh.ttc");
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if(!loadedFontFamilies.empty())
    {
        m_str_fontName = loadedFontFamilies.at(0);
    }
}

void MultiPeakWidget::ClearMultiPeak()
{
    m_vec_filetable.clear();
    m_vec_Peakline.clear();
    m_index_Select = 0;
    m_index_PeakLine = 0;
}

void MultiPeakWidget::SetPeakData(const QString &str_samplename, int index, const QString &str_file)
{
    LOG_DEBUG("%s",str_samplename.toStdString().c_str());
    if(!m_bRefresh)
    {
        if(m_str_SampleName != str_samplename || m_index_Exon != index)
        {
            m_str_SampleName = str_samplename;
            m_index_Exon = index;
        }
        else
        {
            for(int i=0;i<m_vec_Peakline.size();i++)
            {
                if(m_vec_Peakline[i]->GetFileName() == str_file)
                {
                    m_index_PeakLine = i;
                }
            }
            update();
            return;
        }
    }
    else
    {
        m_bRefresh = false;
    }

    ClearMultiPeak();

    SoapTypingDB::GetInstance()->getAlldataFormRealTime(str_samplename, index, m_vec_filetable);
    int i_count_peak = m_vec_filetable.size();
    if(i_count_peak == 0)
    {
        update();
        return;
    }
    std::set<int> set_left,set_right; //以ExonStartPos和ExonEndPos为界，计算左右两边的长度
    int len_exon = 0;
    for(int i=0;i<i_count_peak;i++)
    {
        Ab1FileTableBase &table = m_vec_filetable[i];
        long l_size = table.getSignalNumber();
        QSharedPointer<PeakLine> pPeakLine = QSharedPointer<PeakLine>(new PeakLine(l_size));
        pPeakLine->SetFileName(table.getFileName());
        if(table.getFileName() == str_file)
        {
            m_index_PeakLine = i;
        }
        pPeakLine->SetAlignPos(table.getAlignStartPos(), table.getAlignEndPos());
        pPeakLine->SetExcludePos(table.getExcludeLeft(), table.getExcludeRight());
        pPeakLine->SetGssp(table.getIsGssp());
        pPeakLine->setAvgWidth(table.getAverageBaseWidth());
        pPeakLine->setAvgSignal(table.getAvgsignal());
        pPeakLine->setPeakYSetp(PEAK_Y_STEP);
        pPeakLine->setPeakHeight(PEAKLINEHIGHT);

        if(len_exon == 0)
        {
            m_start_exon = table.getExonStartPos();
            m_end_exon = table.getExonEndPos();
            len_exon = m_end_exon - m_start_exon;
        }

        m_vec_Peakline.push_back(pPeakLine);
    }
}

void MultiPeakWidget::CalcPeakLineData(int exon_pos)
{
    std::set<int> set_left,set_right;
    int i_count_peak = m_vec_filetable.size();
    if(i_count_peak == 0)
    {
        update();
        return;
    }
    for(int i=0;i<i_count_peak;i++)
    {
        Ab1FileTableBase &table = m_vec_filetable[i];
        long l_size = table.getSignalNumber();

        QVector<QStringRef> baseposion = table.getBasePostion().splitRef(':');

        QVector<QStringRef> aligninfo = table.getAlignInfo().splitRef(':');
        QString ref(QString::number(exon_pos-1));
        int index = aligninfo.indexOf(&ref);//根据外显子坐标，找到对齐的峰图下标
        int i_left = 0;
        int i_right = 0;
        if(index != -1) //没有超过峰图边界
        {
            if(i == m_index_PeakLine)
            {
                m_index_Select = index;
            }
            int tmp = baseposion[index].toInt();
            i_left = tmp*m_x_step;
            i_right = (l_size - tmp)*m_x_step;
        }

        set_left.insert(i_left);
        set_right.insert(i_right);

        QSharedPointer<PeakLine> pPeakLine = m_vec_Peakline[i];
        pPeakLine->setXLeft(i_left);
    }

    Q_ASSERT(!set_left.empty());
    for(int i=0;i<i_count_peak;i++)
    {
        QSharedPointer<PeakLine> pPeakLine = m_vec_Peakline[i];
        int i_offset = *set_left.crbegin() - pPeakLine->getXLeft();
        pPeakLine->SetOffset(i_offset);
    }

    m_l_xSize = *(set_left.crbegin()) + *(set_right.crbegin());//以当前所选的位置为对齐，取其左右最大值，相加为总长度
    m_maxleft = *(set_left.crbegin());
    SetPeakLineData();
}


void MultiPeakWidget::SetPeakLineData()
{
    m_iPeakHeightTotal = 0;
    for(int k=0;k<m_vec_Peakline.size();k++)
    {
        Ab1FileTableBase &table = m_vec_filetable[k];
        long l_size = table.getSignalNumber();
        QSharedPointer<PeakLine> pPeakLine = m_vec_Peakline[k];
        int iPeakHeight = pPeakLine->getPeakHeight();
        double d_ystep = pPeakLine->getPeakYStep()*1.0/100;

        QVector<QStringRef> A_list = table.getBaseASignal().splitRef(':');
        QVector<QStringRef> T_list = table.getBaseTSignal().splitRef(':');
        QVector<QStringRef> G_list = table.getBaseGSignal().splitRef(':');
        QVector<QStringRef> C_list = table.getBaseCSignal().splitRef(':');

        QVector<QStringRef> baseposion = table.getBasePostion().splitRef(':');
        QVector<QStringRef> basequal = table.getBaseQuality().splitRef(':');

        int i_offset = pPeakLine->GetOffset();

        int i_basenum = table.getBaseNumber();
        m_iPeakHeightTotal +=iPeakHeight;
        int height_letter = m_iPeakHeightTotal - iPeakHeight;

        pPeakLine->GetBasePoint('A').clear();
        pPeakLine->GetBasePoint('T').clear();
        pPeakLine->GetBasePoint('G').clear();
        pPeakLine->GetBasePoint('C').clear();

        for(int i=0;i<l_size;i++)
        {
            double a_x = i*m_x_step+i_offset;
            double a_y = m_iPeakHeightTotal-A_list[i].toInt()*d_ystep;
            pPeakLine->SetBasePoint('A',a_x,a_y);

            double t_y = m_iPeakHeightTotal-T_list[i].toInt()*d_ystep;
            pPeakLine->SetBasePoint('T',a_x,t_y);

            double g_y = m_iPeakHeightTotal-G_list[i].toInt()*d_ystep;
            pPeakLine->SetBasePoint('G',a_x,g_y);

            double c_y = m_iPeakHeightTotal-C_list[i].toInt()*d_ystep;
            pPeakLine->SetBasePoint('C',a_x,c_y);
        }

        if(pPeakLine->GetGeneLetter().isEmpty())
        {
            QByteArray &baseseq = table.getBaseSequence();
            for(int i=0;i<i_basenum;i++)
            {
                int pos = baseposion[i].toInt();
                GeneLetter geneletter;
                geneletter.pos.setX((pos-1)*m_x_step+i_offset);
                geneletter.pos.setY(37 + height_letter);
                geneletter.type = baseseq[i];
                geneletter.oldtype = ' ';
                geneletter.qual = basequal[i].toInt();

                geneletter.a_signal = A_list[pos].toInt();
                geneletter.t_signal = T_list[pos].toInt();
                geneletter.g_signal = G_list[pos].toInt();
                geneletter.c_signal = C_list[pos].toInt();
                pPeakLine->AddGeneLetter(geneletter);
            }

            if(!table.getEditInfo().isEmpty()) //显示编辑的碱基
            {
                QVector<QStringRef> editinfo = table.getEditInfo().splitRef(';');
                foreach(QStringRef str, editinfo)
                {
                    QVector<QStringRef> temp = str.split(':');
                    int editpos = temp[0].toInt();
                    GeneLetter &geneletter = pPeakLine->GetGeneLetter()[editpos];
                    geneletter.oldtype = geneletter.type;
                    geneletter.type = temp[2][0].toLatin1();
                }
            }
        }
        else
        {
            for(int i=0;i<pPeakLine->GetGeneLetter().size();i++)
            {
                int pos = baseposion[i].toInt();
                GeneLetter &geneletter = pPeakLine->GetGeneLetter()[i];
                geneletter.pos.setX((pos-1)*m_x_step+i_offset);
                geneletter.pos.setY(37 + height_letter);
            }
        }
    }
    resize(m_l_xSize, m_iPeakHeightTotal+20);//如果不调用，paintEvent不响应
}

void MultiPeakWidget::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter pter(this);

    DrawExcludeArea(&pter);

    DrawSelectFrame(&pter);

    DrawPeakLines(&pter);

    DrawHLines(&pter);

    DrawPeakHead(&pter);

}

void MultiPeakWidget::DrawPeakLines(QPainter *pter)
{
    foreach(const QSharedPointer<PeakLine> & peakline, m_vec_Peakline)
    {
        pter->setPen(QColor(57,181,74));
        pter->drawPolyline(peakline->GetBasePoint('A'));

        pter->setPen(Qt::red);
        pter->drawPolyline(peakline->GetBasePoint('T'));

        pter->setPen(Qt::black);
        pter->drawPolyline(peakline->GetBasePoint('G'));

        pter->setPen(Qt::blue);
        pter->drawPolyline(peakline->GetBasePoint('C'));
    }
}

void MultiPeakWidget::DrawHLines(QPainter *pter)
{
    QPen pen(Qt::darkGray);
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    pter->setPen(pen);


    int i_width = width();
    int height_total = 0;
    for(int i=0;i<m_vec_Peakline.size();i++)
    {
        int i_height = m_vec_Peakline[i]->getPeakHeight(); //每个峰图的高度
        height_total += i_height;
        int height_line = height_total - i_height;

        if(m_index_PeakLine == i)
        {
            pter->setBrush(Qt::yellow);
            pter->drawRect(0,height_line, i_width,20);
        }
        pter->setBrush(Qt::NoBrush);

        pter->drawLine(0,20+height_line, i_width,20+height_line);

        pter->drawLine(0,40+height_line, i_width,40+height_line);

        pter->drawLine(0,60+height_line, i_width,60+height_line);

        pter->drawLine(0, height_total, i_width, height_total);
    }
}

void MultiPeakWidget::GetBaseColor(QPainter *pter, const QChar &base)
{
    switch(base.toLatin1())
    {
    case 'A':
        pter->setPen(QColor(57,181,74));
        break;
    case 'T':
        pter->setPen(Qt::red);
        break;
    case 'G':
        pter->setPen(Qt::black);
        break;
    case 'C':
        pter->setPen(Qt::blue);
        break;
    default:
        pter->setPen(Qt::darkRed);
    }
}

void MultiPeakWidget::DrawPeakHead(QPainter *pter)
{
    QFont font_letter("微软雅黑");
    font_letter.setPointSize(15);
    font_letter.setBold(true);

    QFont font_other;
    font_letter.setPointSize(13);

    for(int i=0;i<m_vec_Peakline.size();i++)
    {
        int i_height = m_vec_Peakline[i]->getPeakHeight(); //每个峰图的高度

        pter->setFont(font_other);
        pter->drawText(3,15+i*i_height, m_vec_Peakline[i]->GetFileName());
        int i_index = 0;
        foreach(const GeneLetter &letter, m_vec_Peakline[i]->GetGeneLetter())
        {
            pter->setFont(font_letter);
            GetBaseColor(pter, letter.type);
            pter->drawText(letter.pos, QString(letter.type));
            if(letter.oldtype != ' ')
            {
                GetBaseColor(pter, letter.oldtype);
                pter->drawText(letter.pos.x(),letter.pos.y()+HLINEHIGHT, QString(letter.oldtype));
            }

            if(i_index%10 == 0)
            {
                pter->setFont(font_other);
                pter->drawText(letter.pos.x()-3,letter.pos.y()-HLINEHIGHT, QString::number(i_index));
            }
            i_index++;
        }
    }
}

void MultiPeakWidget::DrawSelectFrame(QPainter *pter)
{
    pter->setBrush(QColor(255,255,105));
    pter->setPen(Qt::NoPen);
    pter->drawRect(m_select_pos.x()-4,0,20,m_iPeakHeightTotal+20); //绘制比对框

    if(m_bIsSelect)
    {
        pter->setBrush(Qt::NoBrush);
        pter->setPen(Qt::black);
        pter->drawRect(m_select_pos.x()-4,m_select_pos.y()-16,20,18); //绘制编辑碱基框
    }
}

void MultiPeakWidget::DrawExcludeArea(QPainter *pter)
{
    pter->setPen(Qt::NoPen);
    pter->setBrush(Qt::darkGray);

    int i_width = width();
    //int i_adjust = m_x_step/2-4;
    int height_total = 0;
    for(int i=0;i<m_vec_Peakline.size();i++)
    {
        int i_height = m_vec_Peakline[i]->getPeakHeight(); //每个峰图的高度
        height_total += i_height;
        int height_area = height_total - i_height;

        int left_exclude,right_exclude;
        m_vec_Peakline[i]->GetExcludePos(left_exclude, right_exclude);

        QVector<GeneLetter> &vec_geneLetter = m_vec_Peakline[i]->GetGeneLetter();
        int w_left =  vec_geneLetter[left_exclude].pos.x();
        pter->drawRect(0,60+height_area, w_left, i_height-60);
        int w_right = vec_geneLetter[right_exclude-1].pos.x();
        pter->drawRect(w_right,60+height_area, i_width, i_height-60);
    }
}


void MultiPeakWidget::mousePressEvent(QMouseEvent *event)
{
    if(m_vec_Peakline.empty())
    {
        return;
    }

    QPoint pos = event->pos();
    m_bIsSelect = false;

    m_index_PeakLine = 0;
    int height_total = 0;

    for(int i=0; i<m_vec_Peakline.size(); i++)
    {
        int i_height = m_vec_Peakline[i]->getPeakHeight(); //每个峰图的高度
        height_total += i_height;
        int height_cur = height_total - i_height;

        if(pos.y() > height_cur && pos.y() <= height_total)
        {
            m_index_PeakLine = i;
            break;
        }
    }
    emit SignalChangePeak(m_vec_Peakline[m_index_PeakLine]->GetFileName());

    //QPoint pos_tip = event->globalPos();
//    QPoint pos_par = mapToParent(event->pos());
//    qDebug()<<__func__<<pos<<pos_tip<<pos_par;
    //QToolTip::showText(pos_tip,m_vec_Peakline[m_index_PeakLine]->GetFileName());

    QVector<GeneLetter> &vec_GeneLetter = m_vec_Peakline[m_index_PeakLine]->GetGeneLetter();
    int left_exclude,right_exclude;
    m_vec_Peakline[m_index_PeakLine]->GetExcludePos(left_exclude, right_exclude);
    if(vec_GeneLetter.size() == right_exclude) right_exclude--;//避免越界
    int w_left = vec_GeneLetter[left_exclude].pos.x();
    int w_right = vec_GeneLetter[right_exclude].pos.x();

    if(pos.x() > w_left && pos.x() < w_right)
    {
        for(int i=left_exclude; i<right_exclude; i++)
        {
            int i_low = vec_GeneLetter[i].pos.x();
            int i_high = vec_GeneLetter[i+1].pos.x();
            if(pos.x() >= i_low && pos.x() < i_high)
            {
                m_index_Select = i;
                break;
            }
        }

        if(event->button() == Qt::LeftButton)
        {
            if(m_index_Select>=left_exclude && m_index_Select<=right_exclude)
            {
                int height_peak = 0;
                for(int i=0; i<m_index_PeakLine; i++)
                {
                    int i_height = m_vec_Peakline[i]->getPeakHeight(); //每个峰图的高度
                    height_peak += i_height;
                }

                if(pos.y() > HLINEHIGHT + height_peak &&
                   pos.y() < 2*HLINEHIGHT + height_peak)
                {
                    m_bIsSelect = true; //判断是否选中了碱基字符
                }

                QVector<QStringRef> aligninfo = m_vec_filetable[m_index_PeakLine].getAlignInfo().splitRef(':');
                m_x_index =  aligninfo[m_index_Select].toInt()+1;
                if(m_x_index == 0)
                {
                    qDebug()<<"mismatch del";
                    return;
                }

                CalcPeakLineData(m_x_index);

                m_select_pos = vec_GeneLetter[m_index_Select].pos;
                QPoint pos_peak = mapToParent(m_select_pos);
                emit signalPeakFocusPosition(m_index_Exon, m_x_index , pos_peak);
            }
            else
            {
                LOG_DEBUG("error:%d %d %d %d",m_index_Select,pos.x(),w_left,w_right);
                return;
            }
        }



        QString str_msg;
        if(m_index_Select>1)
        {
            if(m_bIsSelect)
            {
                str_msg = QString("Signal G:%1 T:%2 A:%3 C:%4 N/S:%5% QV:%6 Space:%7").
                        arg(vec_GeneLetter[m_index_Select].g_signal).arg(vec_GeneLetter[m_index_Select].t_signal).
                        arg(vec_GeneLetter[m_index_Select].a_signal).arg(vec_GeneLetter[m_index_Select].c_signal).
                        arg(QString::number(m_vec_Peakline[m_index_PeakLine]->getAvgSignal()*100,'f',2)).
                        arg(vec_GeneLetter[m_index_Select].qual).
                        arg(QString::number(m_vec_Peakline[m_index_PeakLine]->getAvgWidth(),'f',2));
            }
            else
            {
                int selectpos = m_x_index;
                int integer = (selectpos + 2 - m_vec_filetable[m_index_PeakLine].getAlignStartPos())/3;
                int Remainder = (selectpos + 2 - m_vec_filetable[m_index_PeakLine].getAlignStartPos())%3;
                QString str_codon = QString("%1.%2").arg(integer).arg(Remainder);
                QString str_code(vec_GeneLetter[m_index_Select-1].type);
                str_code.append(vec_GeneLetter[m_index_Select].type);
                str_code.append(vec_GeneLetter[m_index_Select+1].type);

                str_msg = QString("Exon:%1 Codon:%2 Pos:%3 Code:%4 QV:%5").arg(m_index_Exon).arg(str_codon).
                        arg(selectpos).arg(str_code).arg(vec_GeneLetter[m_index_Select].qual);
            }
            emit signalSendStatusBarMsg(str_msg);
        }
        update();
    }
}

void MultiPeakWidget::keyPressEvent(QKeyEvent *event)
{
    if(m_bIsSelect)
    {
        m_bIsSelect = false;
        char type = event->key();
        qDebug()<<"keyPressEvent"<<type;

        QByteArray byetary("ATGCRYKMSWBDHVN-");
        QVector<GeneLetter> &vec_GeneLetter = m_vec_Peakline[m_index_PeakLine]->GetGeneLetter();
        if(byetary.contains(type) && vec_GeneLetter[m_index_Select].type != type)
        {
            vec_GeneLetter[m_index_Select].oldtype = vec_GeneLetter[m_index_Select].type;
            vec_GeneLetter[m_index_Select].type = type;
            update();

            if(m_bEditOne)
            {
                int left_exclude,right_exclude;
                m_vec_Peakline[m_index_PeakLine]->GetExcludePos(left_exclude, right_exclude);

                QVector<QStringRef> aligninfo = m_vec_filetable[m_index_PeakLine].getAlignInfo().splitRef(':');
                QStringList list_editinfo;//每个string包含三个部分：碱基序列实际位置 映射到比对位置的pos 修改后的碱基
                for(int i=left_exclude;i<right_exclude;i++)
                {
                    if(vec_GeneLetter[i].oldtype != ' ')
                    {
                        int selectpos = aligninfo[i].toInt();
                        list_editinfo.push_back(QString("%1:%2:%3").arg(i).arg(selectpos).arg(vec_GeneLetter[i].type));
                    }
                }

                if(!list_editinfo.empty())
                {
                    SoapTypingDB::GetInstance()->upDatabyChangebp(m_vec_Peakline[m_index_PeakLine]->GetFileName(),
                                                              list_editinfo.join(';'),
                                                              m_vec_Peakline[m_index_PeakLine]->GetGssp());
                    emit signalChangeDB(m_str_SampleName);//数据库发生变化，需要重新分析样品，重新显示信息
                    QPoint pos_peak = mapToParent(m_select_pos);
                    emit signalPeakFocusPosition(m_index_Exon, m_x_index, pos_peak);
                }
            }
        }

    }
}

void MultiPeakWidget::SetSelectPos(int exon_pos,int x_pos)
{
    LOG_DEBUG("%d %d",exon_pos, m_index_PeakLine);
    CalcPeakLineData(exon_pos);
    if(!m_vec_Peakline.isEmpty())
    {
        m_select_pos = QPoint(m_maxleft, 0);
        update();

        QScrollArea *pParent = dynamic_cast<QScrollArea*>(parentWidget()->parentWidget());
        int pos = m_maxleft-x_pos+30;
        pParent->horizontalScrollBar()->setSliderPosition(pos);

        QString str_msg("Ready");
        QVector<GeneLetter> &vec_GeneLetter = m_vec_Peakline[m_index_PeakLine]->GetGeneLetter();
        if(m_index_Select>1 && m_index_Select < vec_GeneLetter.size()-1)
        {
            int selectpos = exon_pos;
            int integer = (selectpos + 2 - m_vec_filetable[m_index_PeakLine].getAlignStartPos())/3;
            int Remainder = (selectpos + 2 - m_vec_filetable[m_index_PeakLine].getAlignStartPos())%3;
            QString str_codon = QString("%1.%2").arg(integer).arg(Remainder);
            QString str_code(vec_GeneLetter[m_index_Select-1].type);
            str_code.append(vec_GeneLetter[m_index_Select].type);
            str_code.append(vec_GeneLetter[m_index_Select+1].type);

            str_msg = QString("Exon:%1 Codon:%2 Pos:%3 Code:%4 QV:%5").arg(m_index_Exon).arg(str_codon).
                    arg(selectpos).arg(str_code).arg(vec_GeneLetter[m_index_Select].qual);
        }

        emit signalSendStatusBarMsg(str_msg);
    }
}


void MultiPeakWidget::CreateRightMenu()
{
    m_pActDelete = new QAction(tr("Delete Selected File"),this);
    m_pActInsertBaseN = new QAction(tr("Insert Base 'N'"),this);
    m_pActHideTraceDisplay = new QAction(tr("Hide Trace Display"),this);

    m_pActInsertBaseN->setDisabled(true);
    m_pActHideTraceDisplay->setDisabled(true);

    m_pActApplyOne = new QAction(QIcon(":/png/images/apply.png"),tr("Apply One"),this);
    m_pActApplyOne->setIconVisibleInMenu(false);
    m_pActApplyOne->setShortcut(QKeySequence(Qt::ALT+Qt::Key_E));

    m_pActApplyAll = new QAction(QIcon(":/png/images/apply.png"),tr("Apply All"),this);
    m_pActApplyAll->setDisabled(true);
    m_pActApplyAll->setShortcut(QKeySequence(Qt::ALT+Qt::Key_R));

    m_pActanalyzeNow = new QAction(QIcon(":/png/images/apply.png"),tr("Edit One"),this);
    m_pActanalyzeNow->setDisabled(true);
    m_pActanalyzeNow->setShortcut(QKeySequence(Qt::ALT+Qt::Key_O));

    m_pActanalyzeLater = new QAction(QIcon(":/png/images/apply.png"),tr("Edit Multi"),this);
    m_pActanalyzeLater->setIconVisibleInMenu(false);
    m_pActanalyzeLater->setShortcut(QKeySequence(Qt::ALT+Qt::Key_M));

    m_pActanalyze = new QAction(tr("Analyze"),this);
    m_pActanalyze->setShortcut(QKeySequence(Qt::ALT+Qt::Key_T));
    m_pActanalyze->setDisabled(true);

    m_pActExcludeLeft = new QAction(tr("Exclude left"),this);
    m_pActExcludeRight = new QAction(tr("Exclude right"),this);
    m_pActResetExclude = new QAction(tr("Reset Exclude"),this);

    m_pRightMenu = new QMenu(this);
    m_pRightMenu->addAction(m_pActDelete);
    m_pRightMenu->addAction(m_pActInsertBaseN);
    m_pRightMenu->addAction(m_pActHideTraceDisplay);
    m_pRightMenu->addSeparator();

    m_pRightMenu->addAction(m_pActApplyOne);
    m_pRightMenu->addAction(m_pActApplyAll);
    m_pRightMenu->addSeparator();

    m_pRightMenu->addAction(m_pActanalyzeNow);
    m_pRightMenu->addAction(m_pActanalyzeLater);
    m_pRightMenu->addSeparator();

    m_pRightMenu->addAction(m_pActanalyze);
    m_pRightMenu->addSeparator();
    m_pRightMenu->addAction(m_pActExcludeLeft);
    m_pRightMenu->addAction(m_pActExcludeRight);
    m_pRightMenu->addAction(m_pActResetExclude);
    //m_pRightMenu->addSeparator();
}

void MultiPeakWidget::contextMenuEvent(QContextMenuEvent *event)
{
    m_pRightMenu->exec(QCursor::pos());
    event->accept();
}

void MultiPeakWidget::ConnectSignalandSlot()
{
    connect(m_pActDelete, &QAction::triggered, this, &MultiPeakWidget::slotDelteThisFile);
    connect(m_pActApplyOne, &QAction::triggered, this, &MultiPeakWidget::slotActApplyOne);
    connect(m_pActApplyAll, &QAction::triggered, this, &MultiPeakWidget::slotActApplyAll);
    connect(m_pActanalyzeLater, &QAction::triggered, this, &MultiPeakWidget::slotActanalyzeLater);
    connect(m_pActanalyzeNow, &QAction::triggered, this, &MultiPeakWidget::slotActanalyzeNow);
    connect(m_pActanalyze, &QAction::triggered, this, &MultiPeakWidget::slotActanalyze);
    connect(m_pActExcludeLeft, &QAction::triggered, this, &MultiPeakWidget::slotHighLightLeftPart);
    connect(m_pActExcludeRight, &QAction::triggered, this, &MultiPeakWidget::slotHighLightRightPart);
    connect(m_pActResetExclude, &QAction::triggered, this, &MultiPeakWidget::slotResetExclude);
}

void MultiPeakWidget::slotDelteThisFile()
{
    QString &str_file = m_vec_Peakline[m_index_PeakLine]->GetFileName();
    LOG_DEBUG("%s",str_file.toStdString().c_str());
    bool isgssp = m_vec_Peakline[m_index_PeakLine]->GetGssp();
    SoapTypingDB::GetInstance()->deleteFile(isgssp, str_file);

    QVector<QString> vec_samplename;
    vec_samplename.push_back(m_str_SampleName);
    emit signalChangeDBByFile(vec_samplename);
}

void MultiPeakWidget::slotApplyOne()
{
    m_bApplyOne = true;
    m_pActApplyOne->setDisabled(true);
    m_pActApplyAll->setDisabled(false);
    m_pActApplyOne->setIconVisibleInMenu(true);
    m_pActApplyAll->setIconVisibleInMenu(false);
}

void MultiPeakWidget::slotActApplyOne()
{
    slotApplyOne();
    emit signalPeakAct(1);
}

void MultiPeakWidget::slotApplyAll()
{
    m_bApplyOne = false;
    m_pActApplyOne->setIconVisibleInMenu(false);
    m_pActApplyAll->setIconVisibleInMenu(true);
    m_pActApplyOne->setDisabled(false);
    m_pActApplyAll->setDisabled(true);
}

void MultiPeakWidget::slotActApplyAll()
{
    slotApplyAll();
    emit signalPeakAct(2);
}

void MultiPeakWidget::slotAnalyseLater()
{
    m_bEditOne = false;
    m_pActanalyzeNow->setIconVisibleInMenu(false);
    m_pActanalyzeLater->setIconVisibleInMenu(true);
    m_pActanalyzeLater->setDisabled(true);
    m_pActanalyzeNow->setDisabled(false);
    m_pActanalyze->setDisabled(false);
}

void MultiPeakWidget::slotActanalyzeLater()
{
    slotAnalyseLater();
    emit signalPeakAct(3);
}

void MultiPeakWidget::slotAnalyseNow()
{
    m_bEditOne = true;
    m_pActanalyzeNow->setIconVisibleInMenu(true);
    m_pActanalyzeLater->setIconVisibleInMenu(false);
    m_pActanalyzeLater->setDisabled(false);
    m_pActanalyzeNow->setDisabled(true);
    m_pActanalyze->setDisabled(true);
}

void MultiPeakWidget::slotActanalyzeNow()
{
    slotAnalyseNow();
    emit signalPeakAct(4);
}

void MultiPeakWidget::slotActanalyze()
{
    bool bChangedb = false;
    for(int index=0;index<m_vec_Peakline.size();index++)
    {
        int left_exclude,right_exclude;
        m_vec_Peakline[index]->GetExcludePos(left_exclude, right_exclude);
        QVector<GeneLetter> &vec_GeneLetter = m_vec_Peakline[index]->GetGeneLetter();

        QVector<QStringRef> aligninfo = m_vec_filetable[index].getAlignInfo().splitRef(':');
        QStringList list_editinfo;
        for(int i=left_exclude;i<right_exclude;i++)
        {
            if(vec_GeneLetter[i].oldtype != ' ')
            {
                int selectpos = aligninfo[i].toInt();
                list_editinfo.push_back(QString("%1:%2:%3").arg(i).arg(selectpos).arg(vec_GeneLetter[i].type));
            }
        }

        if(!list_editinfo.empty())
        {
            bChangedb = true;
            SoapTypingDB::GetInstance()->upDatabyChangebp(m_vec_Peakline[index]->GetFileName(),
                                                      list_editinfo.join(';'),
                                                      m_vec_Peakline[index]->GetGssp());
        }
    }

    if(bChangedb)
    {
        emit signalChangeDB(m_str_SampleName);//数据库发生变化，需要重新分析样品，重新显示信息
        QPoint pos_peak = mapToParent(m_select_pos);
        emit signalPeakFocusPosition(m_index_Exon, m_x_index, pos_peak);
    }
}

void MultiPeakWidget::ExcludeArea(int type)
{
    if (type == 3)//恢复排除区域
    {
        m_vec_Peakline[m_index_PeakLine]->SetExcludePos(0, 0);
        SoapTypingDB::GetInstance()->upDataExclude(m_vec_Peakline[m_index_PeakLine]->GetGssp(),
                                                   m_vec_Peakline[m_index_PeakLine]->GetFileName(),
                                                   0, 0);
        update();
        return;
    }

    if(m_index_Select)
    {       
        if (type == 1)//调整左排除
        {
            int selectpos = m_index_Select-m_vec_filetable[m_index_PeakLine].getAlignStartPos();
            m_vec_Peakline[m_index_PeakLine]->SetExcludePos(selectpos, -1);
            SoapTypingDB::GetInstance()->upDataExclude(m_vec_Peakline[m_index_PeakLine]->GetGssp(),
                                                       m_vec_Peakline[m_index_PeakLine]->GetFileName(),
                                                       selectpos,
                                                       m_vec_filetable[m_index_PeakLine].getExcludeRight());
        }
        else if (type == 2)//调整右排除
        {
            int selectpos = m_vec_filetable[m_index_PeakLine].getAlignEndPos() - m_index_Select;
            m_vec_Peakline[m_index_PeakLine]->SetExcludePos(-1, selectpos);
            SoapTypingDB::GetInstance()->upDataExclude(m_vec_Peakline[m_index_PeakLine]->GetGssp(),
                                                       m_vec_Peakline[m_index_PeakLine]->GetFileName(),
                                                       m_vec_filetable[m_index_PeakLine].getExcludeLeft(),
                                                       selectpos);
        }

        update();
    }

}

void MultiPeakWidget::slotHighLightLeftPart()
{
    ExcludeArea(1);
    emit signalChangeDB(m_str_SampleName);
}

void MultiPeakWidget::slotHighLightRightPart()
{
    ExcludeArea(2);
    emit signalChangeDB(m_str_SampleName);
}

void MultiPeakWidget::slotResetExclude()
{
    ExcludeArea(3);
    emit signalChangeDB(m_str_SampleName);
}

void MultiPeakWidget::AdjustPeakHeight(int height)
{
    m_bIsSelect = false;
    if(m_bApplyOne)
    {
        int peak_height = m_vec_Peakline[m_index_PeakLine]->getPeakHeight();
        peak_height += height;
        if(peak_height >= 100 && peak_height <= 300)
        {
            m_vec_Peakline[m_index_PeakLine]->setPeakHeight(peak_height);
        }
        else
        {
            return;
        }
    }
    else
    {
        for(int i=0;i<m_vec_Peakline.size();i++)
        {
            int peak_height = m_vec_Peakline[i]->getPeakHeight();
            peak_height += height;
            if(peak_height >= 100 && peak_height <= 300)
            {
                m_vec_Peakline[i]->setPeakHeight(peak_height);
            }
            else
            {
                return;
            }
        }
    }

    if(height > 0)
    {
        AdjustPeakY(1);
    }
    else
    {
        AdjustPeakY(-1);
    }
}

void MultiPeakWidget::AdjustPeakY(int y)
{    
    m_bIsSelect = false;
    if(m_bApplyOne)
    {
        int peak_y_step = m_vec_Peakline[m_index_PeakLine]->getPeakYStep();
        peak_y_step += y;
        if(peak_y_step >= 2 && peak_y_step <= 8)
        {
            m_vec_Peakline[m_index_PeakLine]->setPeakYSetp(peak_y_step);
        }
    }
    else
    {
        for(int i=0;i<m_vec_Peakline.size();i++)
        {
            int peak_y_step = m_vec_Peakline[i]->getPeakYStep();
            peak_y_step += y;
            if(peak_y_step >= 2 && peak_y_step <= 8)
            {
                m_vec_Peakline[i]->setPeakYSetp(peak_y_step);
            }
        }
    }

    SetPeakLineData();
    update();
}

void MultiPeakWidget::AdjustPeakX(int x)
{
    m_bIsSelect = false;
    m_x_step += x;
    if(m_x_step >= 2 && m_x_step <= 5*2)
    {
        SetPeakLineData();
        QVector<GeneLetter> &vec_GeneLetter = m_vec_Peakline[m_index_PeakLine]->GetGeneLetter();
        m_select_pos = vec_GeneLetter[m_index_Select].pos;
        update();

        QScrollArea *pParent = dynamic_cast<QScrollArea*>(parentWidget()->parentWidget());
        pParent->horizontalScrollBar()->setSliderPosition(m_select_pos.x()-240);
    }
    else
    {
        m_x_step -= x;
    }
}

void MultiPeakWidget::RestorePeak()
{
    m_bIsSelect = false;
    m_x_step = PEAK_X_STEP;
    for(int i=0;i<m_vec_Peakline.size();i++)
    {
        m_vec_Peakline[i]->setPeakHeight(PEAKLINEHIGHT);
        m_vec_Peakline[i]->setPeakYSetp(PEAK_Y_STEP);
    }

    SetPeakLineData();
    update();
}
