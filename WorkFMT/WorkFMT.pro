#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T12:07:54
#
#-------------------------------------------------

QT       += core gui sql svg

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime

HEADERS +=

ICONS_SRC =  \
    $$PWD/../FmtLib/res/icons \
    $$PWD/../ToolsRuntimeProj/ToolsRuntime/icons

defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

copyToDestDir($$ICONS_SRC, $$OUT_PWD/../bin/resources/icons)
