#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T12:45:06
#
#-------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += \
    FmtLib \
    HotfixCreator \
    WorkFMT \
    DBFileTool \
    FmtDbgHelp \
    FmtScript \
    FmtScriptTest \
    qrsd \
    qrsdtest

WorkFMT.depends = FmtLib
FmtScript.depends = FmtLib
FmtScriptTest.depends = FmtLib FmtScript
DBFileTool.depends = FmtLib
HotfixCreator.depends = FmtLib

RESOURCES += \
    FmtLib/ui/fmt.qrc
