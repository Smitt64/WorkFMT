QT += core gui sql xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += DIFFTOSCRIPTL_LIBRARY

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dattable.cpp \
    diffconnection.cpp \
    diffcore.cpp \
    difffield.cpp \
    difflogging.cpp \
    diffmodeparser.cpp \
    difftableinfo.cpp \
    difftoscriptl.cpp \
    join.cpp \
    linespareser.cpp \
    linesparsermain.cpp \
    recordparser.cpp \
    scripttable.cpp \
    streamcontrol.cpp \
    tablelinks.cpp

HEADERS += \
    DiffToScriptl_global.h \
    dattable.h \
    diffconnection.h \
    diffcore.h \
    difffield.h \
    difflogging.h \
    diffmodeparser.h \
    difftableinfo.h \
    difftoscriptl.h \
    join.h \
    linespareser.h \
    linesparsermain.h \
    recordparser.h \
    scripttable.h \
    streamcontrol.h \
    tablelinks.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

INCLUDEPATH += $$PWD/../FmtLib
DEPENDPATH += $$PWD/../FmtLib

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib/h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime

target.path = $$PWD/../bin
INSTALLS += target
