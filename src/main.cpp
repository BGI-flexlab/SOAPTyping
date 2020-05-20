#include "Mainwindow/mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "log/log.h"

#include <qapplication.h>
#include <QDateTime>
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (type < QtDebugMsg)
        return;
    static char s_types[5][6] = {"DEBUG", "WARN ", "ERROR", "FATAL", "INFO "};
    const char* szType = "DEBUG";
    if (type < 5) {
        szType = s_types[(int)type];
    }

    QString strlog = QString::asprintf("[%s][%s] %s\n",
                                       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").toLocal8Bit().data(),
                                       szType,
                                       msg.toLocal8Bit().data());


    static FILE* s_fp = NULL;
    if (s_fp) {
        fseek(s_fp, 0, SEEK_END);
        if (ftell(s_fp) > (2 << 20)) {
            fclose(s_fp);
            s_fp = NULL;
        }
    }

    if (!s_fp) {
        QString strLogfile = "log/qt_";
        strLogfile.append(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
        strLogfile.append(".log");
        s_fp = fopen(strLogfile.toLocal8Bit().data(), "w");
    }

    if (s_fp) {
        fputs(strlog.toLocal8Bit().data(), s_fp);
    }
}

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(myMessageOutput);
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
    w.InitData();
    w.show();

    LOG_DEBUG("%s","SOAPTyping end");
    return a.exec();
}
