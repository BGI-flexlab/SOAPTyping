#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPainter>

class StatusBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusBarWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *e);
signals:

public slots:

};

#endif // STATUSBARWIDGET_H
