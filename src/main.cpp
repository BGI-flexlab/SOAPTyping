#include "Mainwindow/mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "log/log.h"

#ifndef QT_NO_DEBUG
#include <qapplication.h>
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    }
 }
#endif

int main(int argc, char *argv[])
{
#ifndef QT_NO_DEBUG
    //qInstallMessageHandler(myMessageOutput);
#endif
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
