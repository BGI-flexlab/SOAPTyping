#include "Mainwindow/mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "log/log.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qssF(":/qss/qss");
    if(qssF.open(QFile::ReadOnly))
    {
        QTextStream stream(&qssF);
        QString sss = stream.readAll();
        a.setStyleSheet(sss);
    }
    qssF.close();
    log_init(LL_DEBUG,"test","./log");

    MainWindow w;
    w.show();
    w.InitData();
    return a.exec();
}
