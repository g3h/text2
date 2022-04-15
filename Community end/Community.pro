#-------------------------------------------------
#
# Project created by QtCreator 2021-05-23T19:23:40
#
#-------------------------------------------------

QT        += core gui
QT        += network
QT        += multimedia
RC_ICONS  = 001.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Community
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

CONFIG += c++11

SOURCES += \
    audiorecthread.cpp \
    audiothread.cpp \
        main.cpp \
        mainwindow.cpp \
    login.cpp \
    filesrvdlg.cpp \
    filecntdlg.cpp \
    vediosrvdlg.cpp

HEADERS += \
    audiorecthread.h \
    audiothread.h \
        mainwindow.h \
    login.h \
    filesrvdlg.h \
    filecntdlg.h \
    vediosrvdlg.h

FORMS += \
        mainwindow.ui \
    login.ui \
    filesrvdlg.ui \
    filecntdlg.ui \
    vediosrvdlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

DISTFILES +=
