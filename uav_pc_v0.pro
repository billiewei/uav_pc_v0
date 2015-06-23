#-------------------------------------------------
#
# Project created by QtCreator 2015-06-21T15:33:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = uav_pc_v0
TEMPLATE = app


SOURCES += main.cpp \
    MainWindow.cpp

HEADERS  += \
    MainWindow.h

FORMS    += \
    mainwindow.ui

DISTFILES += \
    Harvard.jpg \
    HKUST.jpg
