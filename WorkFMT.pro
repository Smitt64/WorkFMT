#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T12:45:06
#
#-------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += \
    DiffToScript \
    DumpTool \
    FmtLib \
    HotfixCreator \
    WorkFMT \
    DBFileTool \
    FmtDbgHelp \
    FmtScript \
    FmtScriptTest \
    fmtdatapumpwrp \
    RsWorkTools/ToolsRuntime

win32-msvc* {

SUBDIRS += \
    qrsd \
    qrsdtest

    FmtLib.depends = qrsd
    fmtdatapumpwrp.depends = qrsd
}
FmtLib.depends = RsWorkTools/ToolsRuntime
WorkFMT.depends = FmtLib RsWorkTools/ToolsRuntime
FmtScript.depends = FmtLib
FmtScriptTest.depends = FmtLib FmtScript
DBFileTool.depends = FmtLib
HotfixCreator.depends = FmtLib
fmtdatapumpwrp.depends += FmtLib
DumpTool.depends = FmtLib fmtdatapumpwrp
DiffToScript.depends += FmtLib

RESOURCES += \
    FmtLib/ui/fmt.qrc
