#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T12:07:54
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkFMT
TEMPLATE = app

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

DEFINES += QT_DEPRECATED_WARNINGS

DISTFILES += \
    res/winfmt.rc
RC_FILE = res/winfmt.rc

SOURCES += \
    src/main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix:!macx: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib/h

target.path = $$PWD/../bin
INSTALLS += target
