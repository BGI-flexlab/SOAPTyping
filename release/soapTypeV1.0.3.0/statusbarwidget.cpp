#include "statusbarwidget.h"
StatusBarWgt::StatusBarWgt(QWidget *parent)
    :QWidget(parent)
{
    setMinimumSize(460, 20);
    setMaximumSize(460, 20);
    setBackgroundRole(QPalette::Base);
}

QSize StatusBarWgt::minimumSizeHint()
{
    return QSize(460,20);
}

void StatusBarWgt::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(palette().dark().color());
    painter.setPen(Qt::darkGray);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0,0,width()-2,height()-2));
    painter.setPen(Qt::lightGray);
    painter.drawRect(QRect(1,1,width()-1,height()-1));
    QFont font;
    font.setBold(true);
    //font.Weight(QFont::Bold);

    painter.setFont(font);
    int x=5;
    int a=14;
    int b=9;
    int c=10;
    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"R-");
    x+=a;
    painter.setPen(Qt::green);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(Qt::black);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"Y-");
    x+=a;
    painter.setPen(Qt::red);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b;
    painter.setPen(Qt::blue);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"K-");
    x+=a;
    painter.setPen(Qt::black);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(Qt::red);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a+2,height()),Qt::AlignCenter|Qt::AlignHCenter,"M-");
    x+=a+2;
    painter.setPen(Qt::green);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(Qt::blue);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"S-");
    x+=a;
    painter.setPen(Qt::black);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(Qt::blue);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a+2,height()),Qt::AlignCenter|Qt::AlignHCenter,"W-");
    x+=a+2;
    painter.setPen(Qt::green);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(Qt::red);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"B-");
    x+=a;
    painter.setPen(Qt::black);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(Qt::red);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b;
    painter.setPen(Qt::blue);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"D-");
    x+=a;
    painter.setPen(Qt::black);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(Qt::green);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(Qt::red);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"H-");
    x+=a;
    painter.setPen(Qt::green);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(Qt::blue);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b;
    painter.setPen(Qt::red);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b+c;

    painter.setPen(Qt::darkRed);
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"V-");
    x+=a;
    painter.setPen(Qt::black);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(Qt::blue);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b;
    painter.setPen(Qt::green);
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b+c;
}
