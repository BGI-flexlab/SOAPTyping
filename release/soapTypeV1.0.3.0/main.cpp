#include "staticdatabase.h"
#include "realtimedatabase.h"
#include <QApplication>
#include "mainwindow.h"
#include <QtGui>
#include <QtCore>
#include "logindlg.h"
#include "config.h"
#include "key_update.h"
#include "commongsspdatabase.h"

//Q_IMPORT_PLUGIN(qsqlite)
int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    QFile qssF(":/qss/qss");
    qssF.open(QFile::ReadOnly);
    if(qssF.isOpen())
    {
        QString sss = QLatin1String(qssF.readAll());
        app.setStyleSheet(sss);
    }
    qssF.close();


    Key_Update key;
    if(!key.AllIsRigthNet())
    {
        return 0;
    }
    app.setWindowIcon(QIcon(":/images/logo.png"));
    //   QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QFont f = QApplication::font();
    f.setStyleStrategy(QFont::PreferAntialias);
    QApplication::setFont(f);
    // QApplication::setStyle("plastique");

    // embedded custom font on MS
#if defined( Q_WS_WIN )
    QStringList list;
    list << "consola.ttf" << "consolab.ttf" << "consolai.ttf" << "consolaz.ttf" << "outersid.ttf";
    int fontID(-1);
    bool fontWarningshown(false);
    for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator)
    {
        QFile res(":/fonts/" + *constIterator);
        //qDebug() << res.fileName();
        if (res.open(QIODevice::ReadOnly) == false)
        {
            if (fontWarningshown == false)
            {
                QMessageBox::warning(0, "Application", (QString)"error  load font" + QChar(0x00AB) + "consola" + QChar(0x00BB) + ".");
                fontWarningshown = true;
            }
        }
        else
        {
            fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
            //qDebug()<<fontID;
            if (fontID == -1 && fontWarningshown == false)
            {
                QMessageBox::warning(0, "Application", (QString)"error font" + QChar(0x00AB) + "consola" + QChar(0x00BB) + ".");
                fontWarningshown = true;
            }
        }
    }

    QApplication::setFont(QFont("consolas", 10));
#endif

    //#if defined( Q_WS_WIN )
    //    app.setStyle( "windowsmodernstyle" );
    //#endif
    bool log = false;
    CLoginDlg login;
    login.setOK(&log);
    login.exec();
    if(log)
    {
        int a=loadStaticDatabase();
        int b=loadGsspDatabase();
        loadCommonGsspDatabase();
//        createCommonGsspTable();
//        readCommonGsspTableTxt("commonGsspTable.txt");
//        showCommonGsspDatabase();
        if(a!=0 || b!=0)
        {
            QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        }
        newStartRealTimeDatabase();
        MainWindow *mainwindow = new MainWindow();
        mainwindow->setDatabaseInfo((a+b)>0 ? false:true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(20);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainwindow->sizePolicy().hasHeightForWidth());
        mainwindow->setSizePolicy(sizePolicy);
        mainwindow->resize(1000, 600);
        mainwindow->showMaximized();
        return app.exec();
    }
    if(!key.AllIsRigthNet())
    {
        return 0;
    }
}
