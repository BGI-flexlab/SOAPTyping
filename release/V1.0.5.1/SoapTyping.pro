#LIBS += E:/Qt/4.8.4/plugins/sqldrivers/libqsqlite.a
HEADERS += \
    database/staticdatabase.h \
    database/realtimedatabase.h \
    dialog/openfiledlg.h \
    core/analysis.h \
    core/core.h \
    core/semigloble.h \
    core/interface.h \
    core/ab1.h \
    core/equal.h \
    core/common.h \
    mainwindow/basealigntable.h \
    mainwindow/sampletreewidget.h \
    mainwindow/exonnavigator.h \
    mainwindow/matchlist.h \
    mainwindow/mainwindow.h \
    dialog/allelepairdlg.h \
    mainwindow/statusbarwidget.h \
    dialog/alignmentdialog.h \
    dialog/savefiledlg.h \
    mainwindow/curvedline.h \
    mainwindow/container_oscillogram.h \
    All_Base_Struct.h \
    mainwindow/Main_Oscillogram.h \
    dialog/loadfiledlg.h \
    dialog/deletefiledlg.h \
    dialog/gsspinfodlg.h \
    dialog/finaltypedlg.h \
    dialog/usercommentdlg.h \
    dialog/reportdlg.h \
    thread/ghthreadengine.h \
    thread/typethreadpool.h \
    dialog/logindlg.h \
    thread/ghthreadobject.h \
    thread/ghthreadtaskitem.h \
    core/config.h \
    dialog/setdlg.h \
    core/key_update.h \
    core/taesclass.h \
    dialog/exontrimdlg.h \
    dialog/updatedatabasedlg.h \
    database/commongsspdatabase.h \
    mainwindow/soapletter.h
SOURCES += \
    main.cpp \
    database/staticdatabase.cpp \
    database/realtimedatabase.cpp \
    dialog/openfiledlg.cpp \
    core/analysis.cpp \
    core/interface.cpp \
    core/core.cpp \
    core/ab1.cpp \
    core/equal.cpp \
    core/common.cpp \
    mainwindow/basealigntable.cpp \
    mainwindow/sampletreewidget.cpp \
    mainwindow/exonnavigator.cpp \
    mainwindow/matchlist.cpp \
    mainwindow/mainwindow.cpp \
    dialog/allelepairdlg.cpp \
    mainwindow/statusbarwidget.cpp \
    dialog/alignmentdialog.cpp \
    dialog/savefiledlg.cpp \
    mainwindow/Main_Oscillogram.cpp \
    mainwindow/curvedline.cpp \
    mainwindow/container_oscillogram.cpp \
    dialog/loadfiledlg.cpp \
    dialog/deletefiledlg.cpp \
    dialog/gsspinfodlg.cpp \
    dialog/finaltypedlg.cpp \
    dialog/usercommentdlg.cpp \
    dialog/reportdlg.cpp \
    thread/ghthreadtaskitem.cpp \
    thread/ghthreadobject.cpp \
    thread/ghthreadengine.cpp \
    thread/typethreadpool.cpp \
    dialog/logindlg.cpp \
    core/semigloble.cpp \
    core/config.cpp \
    dialog/setdlg.cpp \
    core/key_update.cpp \
    core/TAesClass.cpp \
    dialog/exontrimdlg.cpp \
    dialog/updatedatabasedlg.cpp \
    database/commongsspdatabase.cpp \
    mainwindow/soapletter.cpp
FORMS += \
    mainwindow/basealigntablewidget.ui
QT += sql
QT += network
#OTHER_FILES += \
#    SoapTyping.rc
RC_FILE += SoapTyping.rc
CONFIG += static
CONFIG += exceptions
TARGET = SoapTyping1.0.5.1
DESTDIR = ..\

RESOURCES += \
    mainwindow.qrc
