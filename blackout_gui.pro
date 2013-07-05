#-------------------------------------------------
#
# Project created by QtCreator 2013-07-05T23:23:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = blackout_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    blackout.cpp \
    tools.cpp \
    gameManager.cpp \
    board.cpp

HEADERS  += mainwindow.h \
    tools.h \
    gameManager.h \
    board.h \
    blackout.h

FORMS    += mainwindow.ui
