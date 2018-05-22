#LIBS += E:/Qt/4.8.4/plugins/sqldrivers/libqsqlite.a
HEADERS += \
    staticdatabase.h \
    realtimedatabase.h \
    openfiledlg.h \
    analysis.h \
    core.h \
    semigloble.h \
    interface.h \
    ab1.h \
    equal.h \
    common.h \
    basealigntable.h \
    sampletreewidget.h \
    exonnavigator.h \
    matchlist.h \
    mainwindow.h \
    allelepairdlg.h \
    statusbarwidget.h \
    alignmentdialog.h \
    savefiledlg.h \
    curvedline.h \
    container_oscillogram.h \
    All_Base_Struct.h \
    Main_Oscillogram.h \ 
    loadfiledlg.h \
    deletefiledlg.h \
    gsspinfodlg.h \
    finaltypedlg.h \
    usercommentdlg.h \
    reportdlg.h \
    ghthreadengine.h \
    typethreadpool.h \
    logindlg.h \
    ghthreadobject.h \
    ghthreadtaskitem.h \
    config.h \
    setdlg.h \
    key_update.h \
    taesclass.h \
    exontrimdlg.h \
    updatedatabasedlg.h \
    commongsspdatabase.h
SOURCES += \
    main.cpp \
    staticdatabase.cpp \
    realtimedatabase.cpp \
    openfiledlg.cpp \
    analysis.cpp \
    interface.cpp \
    core.cpp \
    ab1.cpp \
    equal.c \
    common.cpp \
    basealigntable.cpp \
    sampletreewidget.cpp \
    exonnavigator.cpp \
    matchlist.cpp \
    mainwindow.cpp \
    allelepairdlg.cpp \
    statusbarwidget.cpp \
    alignmentdialog.cpp \
    savefiledlg.cpp \
    Main_Oscillogram.cpp \
    curvedline.cpp \
    container_oscillogram.cpp \ 
    loadfiledlg.cpp \
    deletefiledlg.cpp \
    gsspinfodlg.cpp \
    finaltypedlg.cpp \
    usercommentdlg.cpp \
    reportdlg.cpp \
    ghthreadtaskitem.cpp \
    ghthreadobject.cpp \
    ghthreadengine.cpp \
    typethreadpool.cpp \
    logindlg.cpp \
    semigloble.cpp \
    config.cpp \
    setdlg.cpp \
    key_update.cpp \
    TAesClass.cpp \
    exontrimdlg.cpp \
    updatedatabasedlg.cpp \
    commongsspdatabase.cpp
FORMS +=
QT += sql
QT += network
#OTHER_FILES += \
#    SoapTyping.rc
RC_FILE += SoapTyping.rc
CONFIG += static
CONFIG += exceptions
TARGET = SoapTyping1.0.2.0
DESTDIR = ..

RESOURCES += \
    mainwindow.qrc

