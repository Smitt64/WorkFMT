#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T12:45:06
#
#-------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += \
    ChangeNote \
    DiffToScript \
    DumpTool \
    FmtLib \
    FmtRslModule \
    HotfixCreator \
    WorkFMT \
    DBFileTool \
    FmtDbgHelp \
    FmtScriptTest \
    fmtdatapumpwrp \
    ToolsRuntimeProj

win32-msvc* {

SUBDIRS += \
    qrsd \
    qrsdtest

    FmtLib.depends = qrsd
    fmtdatapumpwrp.depends = qrsd
}

FmtLib.depends = ToolsRuntimeProj
WorkFMT.depends = FmtLib ToolsRuntimeProj
DBFileTool.depends = FmtLib
HotfixCreator.depends = FmtLib ToolsRuntimeProj
fmtdatapumpwrp.depends += FmtLib
DumpTool.depends = FmtLib fmtdatapumpwrp
DiffToScript.depends += FmtLib
FmtRslModule.depends += FmtLib
#ToolsRuntime.depends += RslToolRuntimeStatic

RESOURCES += \
    FmtLib/ui/fmt.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/ToolsRuntimeProj/ToolsRuntime
