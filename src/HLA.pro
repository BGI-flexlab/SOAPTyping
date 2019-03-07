#-------------------------------------------------
#
# Project created by QtCreator 2018-07-27T08:51:59
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = soaptyping
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    Mainwindow/mainwindow.cpp \
    Dialog/openfiledialog.cpp \
    DataBase/soaptypingdb.cpp \
    ThreadTask/analysisab1threadtask.cpp \
    ThreadTask/analysissamplethreadtask.cpp \
    Core/Ab1.cpp \
    Core/fileTablebase.cpp \
    Core/core.cpp \
    Mainwindow/sampletreewidget.cpp \
    Mainwindow/matchlistwidget.cpp \
    Mainwindow/multipeakwidget.cpp \
    Mainwindow/exonnavigatorwidget.cpp \
    Mainwindow/basealigntablewidget.cpp \
    Dialog/usercommentdlg.cpp \
    Dialog/savefiledlg.cpp \
    Dialog/gsspinfodlg.cpp \
    Dialog/finaltypedlg.cpp \
    Dialog/loadfiledlg.cpp \
    Dialog/deletefiledlg.cpp \
    Dialog/reportdlg.cpp \
    Dialog/allelepairdlg.cpp \
    Dialog/setdlg.cpp \
    Dialog/exontimdlg.cpp \
    Dialog/alignmentdlg.cpp \
    log/log.cpp \
    Dialog/updatedatadlg.cpp \
    ThreadTask/fileprocessthreadtask.cpp

HEADERS += \
    Mainwindow/mainwindow.h \
    Dialog/openfiledialog.h \
    DataBase/soaptypingdb.h \
    all_base_struct.h \
    ThreadTask/analysisab1threadtask.h \
    Core/Ab1.h \
    Core/fileTablebase.h \
    Core/core.h \
    ThreadTask/analysissamplethreadtask.h \
    Mainwindow/sampletreewidget.h \
    Mainwindow/matchlistwidget.h \
    Mainwindow/multipeakwidget.h \
    Mainwindow/exonnavigatorwidget.h \
    Mainwindow/basealigntablewidget.h \
    Dialog/usercommentdlg.h \
    Dialog/savefiledlg.h \
    Dialog/gsspinfodlg.h \
    Dialog/finaltypedlg.h \
    Dialog/loadfiledlg.h \
    Dialog/deletefiledlg.h \
    Dialog/reportdlg.h \
    Dialog/allelepairdlg.h \
    Dialog/setdlg.h \
    Dialog/exontimdlg.h \
    Dialog/alignmentdlg.h \
    log/log.h \
    log/macro_define.h \
    Dialog/updatedatadlg.h \
    ThreadTask/fileprocessthreadtask.h


FORMS += \
    Mainwindow/mainwindow.ui \
    Dialog/openfiledialog.ui \
    Dialog/usercommentdlg.ui \
    Dialog/savefiledlg.ui \
    Dialog/gsspinfodlg.ui \
    Dialog/finaltypedlg.ui \
    Dialog/loadfiledlg.ui \
    Dialog/deletefiledlg.ui \
    Dialog/reportdlg.ui \
    Dialog/allelepairdlg.ui \
    Dialog/setdlg.ui \
    Dialog/exontimdlg.ui \
    Dialog/alignmentdlg.ui \
    Dialog/updatedatadlg.ui
RESOURCES += \
    hla.qrc
RC_FILE += SoapTyping.rc

DISTFILES += \

mac:CONFIG -= app_bundle