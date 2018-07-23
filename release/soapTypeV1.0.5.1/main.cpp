#include "database/staticdatabase.h"
#include "database/realtimedatabase.h"
#include <QApplication>
#include "mainwindow/mainwindow.h"
#include <QtGui>
#include <QtCore>
#include "dialog/logindlg.h"
#include "core/config.h"
#include "core/key_update.h"
#include "database/commongsspdatabase.h"
//改字体
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

    //    QFont f = QApplication::font();
    //    f.setStyleStrategy(QFont::PreferAntialias);
    QFont f("微软雅黑");
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

    QApplication::setFont(QFont("微软雅黑", 10));
#endif

    //#if defined( Q_WS_WIN )
    //    app.setStyle( "windowsmodernstyle" );
    //#endif
    bool log = true;//false;
    //CLoginDlg login;
    //login.setOK(&log);
    //login.exec();
    if(log)
    {
        int a=loadStaticDatabase();
        int b=loadGsspDatabase();
        loadCommonGsspDatabase();
        //        createCommonGsspTable();
        //        readCommonGsspTableTxt("D:\\workspace\\Test\\Database\\commonGsspTable.txt");
        //        showCommonGsspDatabase();
        if(a!=0 || b!=0)
        {
            QMessageBox::warning(0, "SoapTyping","Database files are missing!");
        }
        /**
          *修正，闪退问题的另一种解决方案，就是不把数据库清空
          *不过这样，打开的时候有点慢
          **/
        if(!loadRealTimeDatabase()){
            newStartRealTimeDatabase();
        }

        MainWindow *mainwindow = new MainWindow();
        mainwindow->setDatabaseInfo((a+b)>0 ? false:true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(20);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainwindow->sizePolicy().hasHeightForWidth());
        mainwindow->setSizePolicy(sizePolicy);
        mainwindow->resize(1000, 600);
        mainwindow->showMaximized();
        /**
          *修正，闪退问题的另一种解决方案，就是不把数据库清空
          *不过这样，打开的时候有点慢
          **/
        mainwindow->onWindowOpen();
        //showGsspTable();
        //showCommonGsspDatabase();
        return app.exec();
    }
    if(!key.AllIsRigthNet())
    {
        return 0;
    }
}
