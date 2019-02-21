#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T12:45:06
#
#-------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += \
    FmtLib \
    WorkFMT \
    DBFileTool \
    FmtDbgHelp \
    FmtScript \
    FmtScriptTest

WorkFMT.depends = FmtLib
FmtScript.depends = FmtLib
FmtScriptTest.depends = FmtLib FmtScript
DBFileTool.depends = FmtLib

RESOURCES += \
    FmtLib/ui/fmt.qrc
