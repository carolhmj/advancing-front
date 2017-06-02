#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T17:36:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdvancingFront
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    geometry.cpp \
    model.cpp

HEADERS  += mainwindow.h \
    geometry.h \
    model.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../glm/
