#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPainter>
//改，颜色变动较大
//explicit关键字的意思
class StatusBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusBarWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *e);
    //改、删QSize minimumSizeHint();
signals:

public slots:

};

#endif // STATUSBARWIDGET_H
