#-------------------------------------------------
#
# Project created by QtCreator 2017-08-29T13:50:25
#
#-------------------------------------------------

QT       += core script widgets sql uitools

TARGET = FmtScript
TEMPLATE = lib
CONFIG += plugin force_debug_info

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        scriptextensionplugin.cpp \
    jsfmttable.cpp \
    jsconnectioninfo.cpp \
    jsfmtlib.cpp \
    jsfmtfile.cpp \
    jsfmtfileprototype.cpp \
    jsuilib.cpp

HEADERS += \
        scriptextensionplugin.h \
    jsfmttable.h \
    jsconnectioninfo.h \
    jsfmtlib.h \
    jsfmtfile.h \
    jsfmtfileprototype.h
DISTFILES += FmtScript.json 

target.path = ../bin/script
INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix:!macx: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib/h
