#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H
#include <QtCore>
#include <QtGui>

class StatusBarWgt : public QWidget
{
    Q_OBJECT
public:
    StatusBarWgt(QWidget *parent=0);
protected:
    void paintEvent(QPaintEvent *e);
    QSize minimumSizeHint();
};

#endif // STATUSBARWIDGET_H
