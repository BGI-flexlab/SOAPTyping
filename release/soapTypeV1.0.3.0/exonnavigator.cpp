#include "exonnavigator.h"
#include "key_update.h"
ExonNavigator::ExonNavigator(QWidget *parent)
    :QWidget(parent)
{
    Key_Update key;
    if(!key.AllIsRigth())
    {
        return ;
    }
    isDown_ = false;
    lastX_ = -1;
    gap_ = 25;
    xscale_ = -1.0;
    newLength_ = 1;
    exonInfo_.clear();
    typeMisPosition_.clear();
    sampleMisPostion_.clear();
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    //setStyleSheet("background-image:url(background1.jpg);background-attachment:scroll");
}
QSize ExonNavigator::minimumSizeHint() const
{
    return QSize(0,50);
}

void ExonNavigator::paintEvent(QPaintEvent *e)
{
    QPainter exonPainter(this);
    exonPainter.setPen(palette().dark().color());
    exonPainter.setPen(Qt::gray);
    exonPainter.setBrush(Qt::NoBrush);
    exonPainter.setRenderHint(QPainter::Antialiasing, false);
    int widthNum = width();
    int heightNum = height();
    exonPainter.drawRect(QRect(0,0,widthNum-1,heightNum -1));
    if(exonInfo_.maxExonIndex == 0)
        return;
    QFont fontRegion;
    QFont fontPos;
    fontRegion.setPointSize(16);
    fontRegion.setFamily(tr("Outer Sider BRK"));
    fontPos.setPointSize(12);
    int top1 = heightNum*0.45;
    int top2 = heightNum*0.5;
    int top3 = heightNum*0.7+1;
    int h1 = heightNum*0.5;
    int h2 = heightNum*0.4;
    int h3 = heightNum*0.2;
    xscale_ = 1.0*widthNum/newLength_;
    exonPainter.setBrush(Qt::SolidPattern);
    exonPainter.setBrush(Qt::NoBrush);
//    QPen fontPen;
//    QBrush fontBrush;
//    fontBrush.setStyle(Qt::BDiagPattern);
//    fontPen.setBrush(fontBrush);
//    fontPen.setColor(Qt::darkRed);
//    fontPen.setWidth(20);
//    fontPen.setStyle(Qt::DashLine);
    for(int i=exonInfo_.minExonIndex; i<=exonInfo_.maxExonIndex; i++)
    {
        exonPainter.setPen(Qt::gray);
        exonPainter.drawRect(QRect(oldPosToNewPos(exonPositionIndex_.at(i-1))*xscale_, top2, (exonPositionIndex_.at(i)-exonPositionIndex_.at(i-1))*xscale_, h2));
        QRect recti(oldPosToNewPos(exonPositionIndex_.at(i-1))*xscale_, 0, (exonPositionIndex_.at(i)-exonPositionIndex_.at(i-1))*xscale_, h1);
        exonPainter.setFont(fontRegion);
//        exonPainter.setPen(Qt::red);
//        exonPainter.setPen(fontPen);
        exonPainter.setPen(QColor(139,139,139));
        exonPainter.drawText(recti, Qt::AlignHCenter|Qt::AlignCenter, QString("Exon %1").arg(i));
        exonPainter.setPen(QColor(139,139,139));
        exonPainter.setFont(fontPos);
        exonPainter.drawText(recti, Qt::AlignLeft|Qt::AlignBottom, QString::number(exonPositionIndex_.at(i-1)+1));
        exonPainter.drawText(recti, Qt::AlignRight|Qt::AlignBottom, QString::number(exonPositionIndex_.at(i)-1+1));
    }

    if(lastX_ > 0)
    {
        fontPos.setPointSize(14);
        exonPainter.setFont(fontPos);
        exonPainter.setBrush(Qt::NoBrush);
        exonPainter.setPen(Qt::black);
        int x = oldPosToNewPos(lastX_)*xscale_;
        exonPainter.drawRect(QRect(x-15, top1, 31, h1+1));
        exonPainter.setPen(Qt::white);
        exonPainter.drawRect(QRect(x-14, top1+1, 29, h1-1));
        exonPainter.setPen(Qt::gray);
        exonPainter.drawRect(QRect(x-15, top1, 30, h1));
        exonPainter.drawLine(QPoint(x, top2), QPoint(x, top2+h2-1));
        exonPainter.setPen(Qt::black);
        exonPainter.drawLine(QPoint(x+1, top2+1), QPoint(x+1,top2+h2));
        exonPainter.setPen(Qt::blue);
        exonPainter.drawText(QRect(x-15, 0, 30, h2), Qt::AlignBottom|Qt::AlignCenter, QString::number(lastX_+1));
        exonPainter.setPen(Qt::darkGreen);
        if(isDown_)
        {
            exonPainter.drawLine(QPoint(x-5,top2+h2*3/4), QPoint(x+5, top2+h2*3/4));
        }
        else
        {
            exonPainter.drawLine(QPoint(x-5,top2+h2/4), QPoint(x+5, top2+h2/4));
        }
    }

    exonPainter.setPen(Qt::NoPen);
    exonPainter.setBrush(QColor(18, 19, 200));
    for(int i=0; i<exonInfo_.frUnEqual.size(); i++)
    {
        exonPainter.drawRect(QRect(oldPosToNewPos(exonInfo_.frUnEqual.at(i))*xscale_, top2, 2, h3));
    }

    exonPainter.setBrush(QColor(157, 171, 174));
    for(int i=0; i<exonInfo_.editPos.size(); i++)
    {
        exonPainter.drawRect(QRect(oldPosToNewPos(exonInfo_.editPos.at(i))*xscale_, top2, 2, h3));
    }

    exonPainter.setBrush(QColor(219, 239, 108));
    for(int i=0; i<exonInfo_.frMis.size(); i++)
    {
        exonPainter.drawRect(QRect(oldPosToNewPos(exonInfo_.frMis.at(i))*xscale_, top2, 2, h3));
    }

    exonPainter.setBrush(QColor(206, 23, 48));
    for(int i=0; i<exonInfo_.pcMis.size(); i++)
    {
        exonPainter.drawRect(QRect(oldPosToNewPos(exonInfo_.pcMis.at(i))*xscale_, top2, 2, h3));
    }

    for(QMap<int, int>::iterator it=typeMisPosition_.begin(); it!=typeMisPosition_.end(); it++)
    {
        exonPainter.drawRect(QRect(oldPosToNewPos(it.key())*xscale_, top3, 2, h3));
    }

}

void ExonNavigator::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint pos = event->pos();
        if(pos.y()>height()*0.7 && !typeMisPosition_.empty())
        {
            isDown_ = true;
        }
        else
        {
            isDown_ = false;
        }
        int lastX = newPosToOldPos(pos.x() / xscale_);
        if(lastX != -1)
        {
            lastX_ = lastX;
            repaint();
            int exonNum = getExonNum(lastX_);
            emit signalFocusPosition(lastX_, exonNum, 200);
        }
        return;
    }
    return;
}
int ExonNavigator::getExonNum(int basePos)
{
    int exon = 0;
    for(int i=0; i<exonPositionIndex_.size(); i++)
    {
        if(basePos>=exonPositionIndex_.at(i) && basePos<exonPositionIndex_.at(i+1))
        {
            exon = i+1;
            break;
        }
    }
    return exon;
}

int ExonNavigator::oldPosToNewPos(int oldPos)
{
    if(exonInfo_.maxExonIndex==0)
        return -1;
    int exon = getExonNum(oldPos);
    if(exon<exonInfo_.minExonIndex || exon>exonInfo_.maxExonIndex)
        return -1;
    return oldPos-exonPositionIndex_.at(exonInfo_.minExonIndex-1) + 2*gap_*(exon-exonInfo_.minExonIndex)+gap_;
}

int ExonNavigator::newPosToOldPos(int newPos)
{
    if(exonInfo_.maxExonIndex==0)
        return -1;
    int tmpeEnd0 = 0;
    for(int i=exonInfo_.minExonIndex; i<=exonInfo_.maxExonIndex; i++)
    {
        int tmpStartI = tmpeEnd0 + gap_;
        int tmpEndI = exonPositionIndex_.at(i)-exonPositionIndex_.at(i-1)+tmpStartI;
        if(newPos>=tmpStartI && newPos<tmpEndI)
        {
            return (newPos - tmpStartI + exonPositionIndex_.at(i-1));
        }
        tmpeEnd0 = tmpEndI + gap_;
    }
    return -1;
}
void ExonNavigator::setData()
{
    exonInfo_.clear();
//    lastX_ = -1;
    sampleMisPostion_.clear();
    getExonNavigatorInfo(signalInfo_.sampleName, exonInfo_);
    if(exonInfo_.maxExonIndex==0)
        return;
    for(int i=0; i<exonInfo_.frUnEqual.size(); i++)
    {
        sampleMisPostion_.insert(exonInfo_.frUnEqual.at(i),0);
    }
    for(int i=0; i<exonInfo_.editPos.size(); i++)
    {
        sampleMisPostion_.insert(exonInfo_.editPos.at(i),0);
    }
    for(int i=0; i<exonInfo_.frMis.size(); i++)
    {
        sampleMisPostion_.insert(exonInfo_.frMis.at(i),0);
    }
    for(int i=0; i<exonInfo_.pcMis.size(); i++)
    {
        sampleMisPostion_.insert(exonInfo_.pcMis.at(i),0);
    }
    if(sampleMisPostion_.size()>0)
    {
        samplePoint_ = sampleMisPostion_.begin();
    }
    isDown_ = false;
    newLength_ = exonPositionIndex_.at(exonInfo_.maxExonIndex)-exonPositionIndex_.at(exonInfo_.minExonIndex-1)
            + (exonInfo_.maxExonIndex - exonInfo_.minExonIndex + 1)*2*gap_;
}
int ExonNavigator::getLastXBySampleMisPos(int type)
{
    int x=0, exon=0;
    if(signalInfo_.sampleName.isEmpty() || exonPositionIndex_.size()<1)
        return -1;
    if(type!=2)
    {
    if(sampleMisPostion_.size()==0)
    {
        exon = signalInfo_.exonIndex;
        x = exonPositionIndex_.at(exon-1);
    }
    else
    {
        QMap<int, int>::iterator it=sampleMisPostion_.find(lastX_);
        x=lastX_;exon=getExonNum(x);
        if(it!=sampleMisPostion_.end() && exon==signalInfo_.exonIndex)
        {
            samplePoint_ = it;
        }
        else
        {
            exon = signalInfo_.exonIndex;
            x=exonPositionIndex_.at(exon-1);
            for(QMap<int, int>::iterator it=sampleMisPostion_.begin(); it!=sampleMisPostion_.end(); it++)
            {
                int exoni = getExonNum(it.key());
                if(exoni==exon)
                {
                    x=it.key();
                    samplePoint_ = it;
                    break;
                }
            }
        }
    }
    }
    else
    {
        exon = signalInfo_.exonIndex;
        x = lastX_;
    }
    if(!signalInfo_.isUndefined)
    {
        emit signalFocusPosition(x, exon, 200);
    }
    return x;
}

void ExonNavigator::slotFileChanged(SignalInfo &signalInfo, int type)
{
    if(signalInfo_.geneName != signalInfo.geneName)
    {
        exonPositionIndex_.clear();
        getExonPositionIndexFromStaticDatabase(signalInfo.geneName.toAscii(), exonPositionIndex_);
    }
    if(signalInfo_.sampleName != signalInfo.sampleName || type != 0)
    {
        signalInfo_ = signalInfo;
        typeMisPosition_.clear();
        setData();
        lastX_ = getLastXBySampleMisPos(type);
        repaint();
    }
    else
    {

        signalInfo_ = signalInfo;
        lastX_ = getLastXBySampleMisPos(type);
        repaint();
    }
}

void ExonNavigator::slotFocusPosition(int basePosition, int exonIndex, int leftDistance)
{
    if(oldPosToNewPos(basePosition) == -1)
    {
        lastX_ = -1;
    }
    else
    {
        lastX_ = basePosition;
    }
    repaint();
}
void ExonNavigator::slotIndelPosition(int basePos)
{
    if(oldPosToNewPos(basePos) == -1)
    {
        return;
    }
    lastX_ = basePos;
    repaint();
    emit signalFocusPosition(basePos, getExonNum(basePos), 200);
}

void ExonNavigator::slotTypeMisMatchPostionChanged(QSet<int> &typeMismatchPositions, int type)
{
    typeMisPosition_.clear();
    for(QSet<int>::iterator it=typeMismatchPositions.begin(); it!=typeMismatchPositions.end(); it++)
    {
        typeMisPosition_.insert(*it, 0);
    }

    if(typeMisPosition_.size() > 0)
    {
        int x = (typeMisPosition_.begin()).key();
        typePoint_ = typeMisPosition_.begin();
        for(QMap<int, int>::iterator it=typeMisPosition_.begin(); it!=typeMisPosition_.end(); it++)
        {
            if(it.key() >= lastX_)
            {
                x = it.key();
                typePoint_ = it;
                break;
            }
        }

        isDown_ = true;
        if(type!=2)
        {
            lastX_ = x;
            emit signalFocusPosition(lastX_, getExonNum(lastX_), 200);
        }
    }

    repaint();
}

void ExonNavigator::slotMisPosForward()
{
    if(!typeMisPosition_.empty() && isDown_)
    {
        int pos = typeMisPosition_.begin().key();
        for(typePoint_ = typeMisPosition_.begin(); typePoint_!=typeMisPosition_.end(); typePoint_++)
        {
            if(typePoint_.key()>lastX_)
            {
                pos = typePoint_.key();
                break;
            }
        }

        lastX_ = pos;
        repaint();
        emit signalFocusPosition(lastX_, getExonNum(lastX_), 200);
    }
    else if(!sampleMisPostion_.empty())
    {
        samplePoint_++;
        int pos = sampleMisPostion_.begin().key();
        for(samplePoint_ = sampleMisPostion_.begin(); samplePoint_!=sampleMisPostion_.end(); samplePoint_++)
        {
            if(samplePoint_.key()>lastX_)
            {
                pos = samplePoint_.key();
                break;
            }
        }

        lastX_ = pos;
        repaint();
        emit signalFocusPosition(lastX_, getExonNum(lastX_), 200);
    }
}

void ExonNavigator::slotMisPosBackward()
{
    if(!typeMisPosition_.empty() && isDown_)
    {
        QMap<int, int>::iterator end = typeMisPosition_.end();
        end--;
        int pos = end.key();
        for(typePoint_ = end;; typePoint_--)
        {
            if(typePoint_.key()<lastX_)
            {
                pos = typePoint_.key();
                break;
            }
            if(typePoint_==typeMisPosition_.begin())
                break;
        }

        lastX_ = pos;
        repaint();
        emit signalFocusPosition(lastX_, getExonNum(lastX_), 200);
    }
    else if(!sampleMisPostion_.empty())
    {
        QMap<int, int>::iterator end = sampleMisPostion_.end();
        end--;
        int pos = end.key();
        for(samplePoint_ = end;; samplePoint_--)
        {
            if(samplePoint_.key()<lastX_)
            {
                pos = samplePoint_.key();
                break;
            }
            if(samplePoint_==sampleMisPostion_.begin())
                break;
        }

        lastX_ = pos;
        repaint();
        emit signalFocusPosition(lastX_, getExonNum(lastX_), 200);
    }
}
