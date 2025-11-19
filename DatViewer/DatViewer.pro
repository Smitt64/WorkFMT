QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datviewwindow.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    datviewwindow.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DiffToScriptlib/release/ -lDiffToScriptlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DiffToScriptlib/debug/ -lDiffToScriptlib
else:unix: LIBS += -L$$OUT_PWD/../DiffToScriptlib/ -lDiffToScriptlib

INCLUDEPATH += $$PWD/../DiffToScriptlib
DEPENDPATH += $$PWD/../DiffToScriptlib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

INCLUDEPATH += $$PWD/../FmtLib $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime

target.path = $$PWD/../bin
INSTALLS += target
