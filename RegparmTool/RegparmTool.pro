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
    actionpage.cpp \
    connactionpage.cpp \
    main.cpp \
    operation/addfromwordoperation.cpp \
    operation/operationbase.cpp \
    operationwizardpage.cpp \
    regparmmodel/regparmitem.cpp \
    regparmmodel/regparmmodel.cpp \
    regparmmodel/regparmproxymodel.cpp \
    regparmwizard.cpp \
    rsl/extractinfoexecutor.cpp \
    rsl/reginfoobj.cpp \
    viewdatpage.cpp \
    wordcontentpage.cpp \
    wordpreviewregpage.cpp

HEADERS += \
    actionpage.h \
    connactionpage.h \
    operation/addfromwordoperation.h \
    operation/operationbase.h \
    operationwizardpage.h \
    regparmmodel/regparmitem.h \
    regparmmodel/regparmmodel.h \
    regparmmodel/regparmproxymodel.h \
    regparmwizard.h \
    rsl/extractinfoexecutor.h \
    rsl/reginfoobj.h \
    viewdatpage.h \
    wordcontentpage.h \
    wordpreviewregpage.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    actionpage.ui \
    connactionpage.ui \
    operationwizardpage.ui \
    viewdatpage.ui \
    wordcontentpage.ui \
    wordpreviewregpage.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime

RESOURCES += \
    ../DiffToScript/res.qrc \
    regtool.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib

target.path = $$PWD/../bin
INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DiffToScriptlib/release/ -lDiffToScriptlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DiffToScriptlib/debug/ -lDiffToScriptlib
else:unix: LIBS += -L$$OUT_PWD/../DiffToScriptlib/ -lDiffToScriptlib

INCLUDEPATH += $$PWD/../DiffToScriptlib
DEPENDPATH += $$PWD/../DiffToScriptlib

OTHER_FILES += \
    $$PWD/mac

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

copyToDestDir($$OTHER_FILES, $$OUT_PWD/../bin/mac/regparmtool)
copyToDestDir($$OTHER_FILES, $$OUT_PWD/../RegparmTool/debug/mac/regparmtool)
