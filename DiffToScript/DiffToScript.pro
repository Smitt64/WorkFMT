QT       += core gui sql testlib xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG   += console
CONFIG   += force_debug_info

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

RC_FILE = res/difftoscript.rc
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
    wherescript.cpp \
    wizard/actionpage.cpp \
    wizard/connactionpage.cpp \
    wizard/diffwizard.cpp \
    wizard/scriptspage.cpp \
    wizard/svnlogdlg.cpp \
    wizard/svnlogmodel.cpp \
    wizard/svnsatatusmodel.cpp \
    wizard/svntool.cpp

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
    wherescript.h \
    wizard/actionpage.h \
    wizard/connactionpage.h \
    wizard/diffwizard.h \
    wizard/scriptspage.h \
    wizard/svnlogdlg.h \
    wizard/svnlogmodel.h \
    wizard/svnsatatusmodel.h \
    wizard/svntool.h

FORMS += \
    mainwindow.ui \
    wizard/actionpage.ui \
    wizard/connactionpage.ui \
    wizard/scriptspage.ui \
    wizard/svnlogdlg.ui

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
