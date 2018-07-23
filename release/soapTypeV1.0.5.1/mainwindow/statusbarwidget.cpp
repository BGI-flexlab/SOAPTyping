#include "statusbarwidget.h"
#include <QDebug>

StatusBarWidget::StatusBarWidget(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(704, 40);
    setMaximumSize(704, 40);
    setBackgroundRole(QPalette::Base);
}

void StatusBarWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    //    exonPainter.setPen(palette().dark().color());
    painter.setPen(QColor(211,211,211));
    painter.setBrush(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, false);
    int widthNum = width();
    int heightNum = height();
    painter.drawRect(QRect(0,0,widthNum-1,heightNum -1));
    int j=2;
    int x=7;
    int a=26;
    int b=14;
    int c=10;
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    //    font.Weight(QFont::Bold);
    font.setLetterSpacing(QFont::AbsoluteSpacing, j);
    painter.setFont(font);

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"R-");
    x+=a;
    painter.setPen(QColor(57,181,74));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(QColor(0,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");

    x+=b+c;
    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"Y-");
    x+=a;
    painter.setPen(QColor(255,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b;
    painter.setPen(QColor(0,0,255));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b+c;

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"K-");
    x+=a;
    painter.setPen(QColor(0,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(QColor(255,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b+c;

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a+2,height()),Qt::AlignCenter|Qt::AlignHCenter,"M-");
    x+=a+2;
    painter.setPen(QColor(57,181,74));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(QColor(0,0,255));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b+c;

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"S-");
    x+=a;
    painter.setPen(QColor(0,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(QColor(0,0,255));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b+c;

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a+2,height()),Qt::AlignCenter|Qt::AlignHCenter,"W-");
    x+=a+2;
    painter.setPen(QColor(57,181,74));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(QColor(255,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b+c;

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"B-");
    x+=a;
    painter.setPen(QColor(0,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(QColor(255,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b;
    painter.setPen(QColor(0,0,255));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b+c;

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"D-");
    x+=a;
    painter.setPen(QColor(0,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(QColor(57,181,74));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(QColor(255,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b+c;

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"H-");
    x+=a;
    painter.setPen(QColor(57,181,74));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b;
    painter.setPen(QColor(0,0,255));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b;
    painter.setPen(QColor(255,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"T");
    x+=b+c;

    painter.setPen(QColor(193,39,45));
    painter.drawText(QRect(x,0,a,height()),Qt::AlignCenter|Qt::AlignHCenter,"V-");
    x+=a;
    painter.setPen(QColor(0,0,0));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"G");
    x+=b;
    painter.setPen(QColor(0,0,255));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"C");
    x+=b;
    painter.setPen(QColor(57,181,74));
    painter.drawText(QRect(x,0,b,height()),Qt::AlignCenter|Qt::AlignHCenter,"A");
    x+=b+c;
}
