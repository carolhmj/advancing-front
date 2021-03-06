#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T17:36:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdvancingFront
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14 -O0

DEFINES += INTERFACE=0

SOURCES += main.cpp\
        mainwindow.cpp \
    geometry.cpp \
    model.cpp \
    primitives.cpp

HEADERS  += mainwindow.h \
    geometry.h \
    model.h \
    primitives.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../glm/
