QT += widgets sql

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

TEMPLATE = lib
DEFINES += FMTRSLMODULE_LIBRARY

CONFIG += c++11

DISTFILES += RslToolsRuntimeModule.json

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fmtfunctions.cpp \
    fmtnamespace.cpp \
    fmtrslmodule.cpp \
    fmtrslmoduleplugin.cpp

HEADERS += \
    FmtRslModule_global.h \
    fmtnamespace.h \
    fmtrslmodule.h \
    fmtrslmoduleplugin.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

INCLUDEPATH += $$PWD/../FmtLib/h $$PWD/../FmtLib
DEPENDPATH += $$PWD/../FmtLib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime

target.path = $$PWD/../bin
INSTALLS += target
