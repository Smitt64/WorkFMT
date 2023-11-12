#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T12:45:06
#
#-------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += \
    DumpTool \
    FmtLib \
    HotfixCreator \
    WorkFMT \
    DBFileTool \
    FmtDbgHelp \
    FmtScript \
    FmtScriptTest \
    fmtdatapumpwrp
win32-msvc* {

SUBDIRS += \
    qrsd \
    qrsdtest

    FmtLib.depends = qrsd
    fmtdatapumpwrp.depends = qrsd
}

WorkFMT.depends = FmtLib
FmtScript.depends = FmtLib
FmtScriptTest.depends = FmtLib FmtScript
DBFileTool.depends = FmtLib
HotfixCreator.depends = FmtLib
fmtdatapumpwrp.depends += FmtLib
DumpTool.depends = FmtLib fmtdatapumpwrp

RESOURCES += \
    FmtLib/ui/fmt.qrc
