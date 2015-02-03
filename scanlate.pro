#-------------------------------------------------
#
# Project created by QtCreator 2014-05-27T08:17:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scanlate
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    loadingwindow.cpp \
    qscanlateserver.cpp \
    qscanlateuser.cpp \
    qscanlate.cpp \
    qscanlateproject.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    loadingwindow.h \
    qscanlateserver.h \
    qscanlateuser.h \
    qscanlate.h \
    qscanlateproject.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    loadingwindow.ui

OTHER_FILES += \
    loading.gif

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
