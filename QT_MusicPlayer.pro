#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T13:48:56
#
#-------------------------------------------------

QT       += core gui webkit
QT       += network
QT       += multimedia
QT       += multimediawidgets
QT       += webkitwidgets
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_MusicPlayer
TEMPLATE = app


SOURCES += main.cpp\
        musicwindow.cpp \
    mylyric.cpp \
    logindialog.cpp \
    downloaddialog.cpp \
    thread.cpp \
    aboutdialog.cpp

HEADERS  += musicwindow.h \
    mylyric.h \
    logindialog.h \
    downloaddialog.h \
    thread.h \
    aboutdialog.h

FORMS    += musicwindow.ui \
    logindialog.ui \
    downloaddialog.ui \
    aboutdialog.ui

