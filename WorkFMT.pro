#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T12:45:06
#
#-------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += \
    ChangeNote \
    DatViewer \
    DiffToScript \
    DiffToScriptlib \
    DumpTool \
    FmtLib \
    FmtRslModule \
    HfProjects \
    HotfixCreator \
    RegparmTool \
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
DiffToScriptlib.depends += FmtLib ToolsRuntimeProj
DiffToScript.depends += FmtLib DiffToScriptlib ToolsRuntimeProj
FmtRslModule.depends += FmtLib
RegparmTool.depends += FmtLib ToolsRuntimeProj DiffToScriptlib
DatViewer.depends += FmtLib ToolsRuntimeProj DiffToScriptlib
#ToolsRuntime.depends += RslToolRuntimeStatic

RESOURCES += \
    FmtLib/ui/fmt.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/ToolsRuntimeProj/ToolsRuntime
