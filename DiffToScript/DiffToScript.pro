QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG   += console
CONFIG   += force_debug_info

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cmdparser.cpp \
    dattable.cpp \
    dattableinfo.cpp \
    dbspelling.cpp \
    dbspellingoracle.cpp \
    dbspellingpostgres.cpp \
    diffconnection.cpp \
    diffconnection.cpp \
    difffield.cpp \
    difflogging.cpp \
    difftoscripttest.cpp \
    join.cpp \
    linespareser.cpp \
    linesparsermain.cpp \
    main.cpp \
    mainwindow.cpp \
    recordparser.cpp \
    sqlscriptmain.cpp \
    streamcontrol.cpp \
    tablelinks.cpp \
    tablestree.cpp \
    task.cpp \
    taskoptions.cpp \
    wherescript.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix:!macx: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib


HEADERS += \
    cmdparser.h \
    datfield.h \
    dattable.h \
    dattableinfo.h \
    dbspelling.h \
    dbspellingoracle.h \
    dbspellingpostgres.h \
    diffconnection.h \
    diffconnection.h \
    diffcore.h \
    difffield.h \
    difflogging.h \
    difftoscripttest.h \
    join.h \
    linespareser.h \
    linesparsermain.h \
    mainwindow.h \
    recordparser.h \
    sqlscriptmain.h \
    streamcontrol.h \
    tablelinks.h \
    tablestree.h \
    task.h \
    taskoptions.h \
    wherescript.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

target.path = $$PWD/../bin
INSTALLS += target

INCLUDEPATH += $$PWD/../FmtLib
DEPENDPATH += $$PWD/../FmtLib

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib/h


QMAKE_CFLAGS += -FS
QMAKE_CXXFLAGS += -FS
