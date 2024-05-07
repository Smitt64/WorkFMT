#-------------------------------------------------
#
# Project created by QtCreator 2017-05-04T09:52:19
#gui
#-------------------------------------------------

QT       += core  sql
CONFIG   += console
CONFIG   += force_debug_info

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBFileTool
TEMPLATE = app

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

DISTFILES += \
    res/dbftool.rc
RC_FILE = res/dbftool.rc
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    dbfileobject.cpp \
    dbmainwindow.cpp \
    dbttoolwizard.cpp \
    exportpage.cpp \
    loghighlighter.cpp \
    oraconnectionpage.cpp \
    selectactionpage.cpp \
    selecttablespage.cpp \
    task.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix:!macx: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

INCLUDEPATH += $$PWD/../FmtLib
DEPENDPATH += $$PWD/../FmtLib

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib/h

target.path = ../bin
INSTALLS += target

HEADERS += \
    dbfileobject.h \
    dbmainwindow.h \
    dbttoolwizard.h \
    exportpage.h \
    loghighlighter.h \
    oraconnectionpage.h \
    selectactionpage.h \
    selecttablespage.h \
    task.h

FORMS += \
    dbmainwindow.ui \
    exportpage.ui \
    oraconnectionpage.ui \
    selectactionpage.ui

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RsWorkTools/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RsWorkTools/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../RsWorkTools/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../RsWorkTools/ToolsRuntime
DEPENDPATH += $$PWD/../RsWorkTools/ToolsRuntime
