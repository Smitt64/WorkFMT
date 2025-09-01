#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T12:06:53
#
#-------------------------------------------------

QT += widgets sql xml script scripttools core svg webengine webenginewidgets network

CONFIG   += force_debug_info
TARGET = FmtLib
TEMPLATE = lib

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

#QMAKE_LFLAGS_RELEASE += /MAP
#QMAKE_CFLAGS_RELEASE += /Zi /FC /Gy
#QMAKE_LFLAGS_RELEASE += /DEBUG /OPT:REF
#QMAKE_CFLAGS_RELEASE    = /O2 /MD /Zi
#QMAKE_LFLAGS_RELEASE    = /INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF
#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

DEFINES += FMTLIB_LIBRARY QT_DEPRECATED_WARNINGS QT_LOGGING_DEBUG FMTLIB_ABOUT_EXCEPTION
#DEFINES += FMT_RSD_DRIVER

INCLUDEPATH += ./h
INCLUDEPATH += .
LIBS += -lversion

CONFIG(debug, debug|release):DEFINES += FMT_DEBUG

SOURCES += \
    options/externaltoolspage.cpp \
    options/fmtcommandspage.cpp \
    options/fmtoptionsdlg.cpp \
    options/generaloptions.cpp \
    options/tablegroupoptions.cpp \
    options/usercommandsgroup.cpp \
    rslexecutors/checksaveexecutor.cpp \
    rslexecutors/generatorrslexecutor.cpp \
    rslexecutors/toolbaractionexecutor.cpp \
    src/core/fmttablecomparemodel.cpp \
    src/debugconnect.cpp \
    src/gensqltemplatedlg.cpp \
    src/core/fmtcore.cpp \
    src/connectioninfo.cpp \
    src/core/recentconnectionlist.cpp \
    src/dbinitdlg.cpp \
    src/documentwindow.cpp \
    src/fmtapplication.cpp \
    src/fmtfielddlg.cpp \
    src/fmtworkwindow.cpp \
    src/mainwindow.cpp \
    src/massop/destribcreate/massdestribprogress.cpp \
    src/models/odbctablemodel.cpp \
    src/models/odbctablemodeldelegate.cpp \
    src/models/tableobjectsmodel.cpp \
    src/oracleauthdlg.cpp \
    src/queryeditor/queryeditor.cpp \
    src/tablesgroupsdlg.cpp \
    src/widgets/comparefmt/comparecppstructpage.cpp \
    src/widgets/comparefmt/comparefmtviewtab.cpp \
    src/widgets/comparefmt/comparefmtwizard.cpp \
    src/widgets/comparefmt/compareparamspage.cpp \
    src/widgets/comparefmt/compareselecttablepage.cpp \
    src/widgets/filteredtablewidget.cpp \
    src/widgets/fmtsegmentflagsdlg.cpp \
    src/widgets/guiconverterdlg.cpp \
    src/widgets/selectfolderdlg.cpp \
    src/widgets/sqlconvertordlg.cpp \
    src/widgets/tablestructsqldlg.cpp \
    src/windowslistdlg.cpp \
    src/core/fmtlib.cpp \
    src/core/hotfixhelp.cpp \
    src/core/tablesgroupprovider.cpp \
    src/fmt/fmtfield.cpp \
    src/fmt/fmtimpexpwrp.cpp \
    src/fmt/fmtindex.cpp \
    src/fmt/fmttable.cpp \
    src/models/fmtfieldsdelegate.cpp \
    src/models/fmtfildsmodel.cpp \
    src/models/fmtindecesdelegate.cpp \
    src/models/fmtindecesmodel.cpp \
    src/models/fmtindecesmodelitem.cpp \
    src/models/fmttablesmodel.cpp \
    src/models/oracletnslistmodel.cpp \
    src/models/subwindowsmodel.cpp \
    src/widgets/fmtfieldstableview.cpp \
    src/widgets/fmtindextreeview.cpp \
    src/widgets/tablesdock.cpp \
    src/widgets/tablesdockwidget.cpp \
    src/widgets/tablesselectwidget.cpp \
    src/widgets/treecombobox.cpp \
    src/wizards/RichTextToInsertWizard/richtexttoinsertwizard.cpp \
    src/wizards/exporttoxmlwizard.cpp \
    src/wizards/expresultpage.cpp \
    src/wizards/expsettingspage.cpp \
    src/wizards/fmtfromrichtext.cpp \
    src/fmt/fmtgeninterface.cpp \
    src/fmtgentablessql.cpp \
    src/fmtworkwndgen.cpp \
    src/fmtgencpptemplate.cpp \
    src/gencppsettings.cpp \
    src/fmt/undoredo/fmtundotableproperty.cpp \
    src/fmt/undoredo/fmtundotableaddfield.cpp \
    src/fmt/undoredo/fmtundofieldproperty.cpp \
    src/fmt/undoredo/fmtundotableremovefield.cpp \
    src/fmt/undoredo/fmtundotableaddindex.cpp \
    src/fmt/undoredo/fmtundoremoveindex.cpp \
    src/fmt/undoredo/fmtundoremovesegment.cpp \
    src/fmt/undoredo/fmtundoindexaddsegment.cpp \
    src/fmt/fmtsegment.cpp \
    src/core/loggingcategories.cpp \
    src/fmt/undoredo/fmtundoindexproperty.cpp \
    src/fmt/undoredo/fmtundoindexsegmentproperty.cpp \
    src/widgets/undolistpopup.cpp \
    src/fmt/fmtdbftoolwrp.cpp \
    src/widgets/tablecomboaction.cpp \
    src/widgets/toolbtncombo.cpp \
    src/fmtgenhotfix.cpp \
    src/models/connectionlistmodel.cpp \
    src/selectconnectiondlg.cpp \
    src/stringlistdlg.cpp \
    src/models/selectfieldsmodel.cpp \
    src/widgets/selectfiltereddlg.cpp \
    src/core/fmtupdatescripts.cpp \
    h/lineeditaction.cpp \
    src/models/fmttablelistdelegate.cpp \
    src/fmt/fmtfile.cpp \
    src/fmtscriptwindow.cpp \
    src/widgets/fmrichtextwidget.cpp \
    src/widgets/connectionwzardpage/connactionpage.cpp \
    src/models/fmteditcontentmodel.cpp \
    src/widgets/editcontent/fmteditcontentwindow.cpp \
    src/widgets/editcontent/fmteditcontentdialog.cpp \
    src/widgets/datelineedit.cpp \
    src/widgets/editcontent/fmteditcontentfilter.cpp \
    src/widgets/dataselectdialog.cpp \
    src/models/fmtfieldstableheaderdelegate.cpp \
    mdisubinterface.cpp \
    fmtgeninputservicecpptemplate.cpp \
    fmtgencppclasstemplate.cpp \
    src/widgets/editcontent/import/importwizard.cpp \
    src/widgets/editcontent/import/selecttypepage.cpp \
    src/widgets/editcontent/import/texteditpage.cpp \
    src/widgets/editcontent/import/tablemaptofmtwidget.cpp \
    src/core/columnalignedlayout.cpp \
    src/fmt/undoredo/fmtundotablepastefield.cpp \
    src/widgets/editcontent/import/mapfieldspage.cpp \
    src/widgets/editcontent/import/mapfildsmodel.cpp \
    src/models/geninterfacefactorymodel.cpp \
    src/massop/massopinterface.cpp \
    src/wizards/massoperationwizard.cpp \
    src/massop/massopselectoperationpage.cpp \
    src/massop/massinittableoperation.cpp \
    src/massop/massopselecttablespage.cpp \
    src/massop/inittables/massinittableparams.cpp \
    src/massop/inittables/massinittablesparammodel.cpp \
    src/massop/inittables/massinittablesprogress.cpp \
    src/massop/btrvtemplate/massopbtrvtemplate.cpp \
    src/massop/btrvtemplate/massopbtrvtemplateresultpage.cpp \
    src/massop/massopenfunc/massopenfunctemplate.cpp \
    src/massop/massopenfunc/massopenfucnparams.cpp \
    src/massop/destribcreate/massdestribcreate.cpp \
    src/massop/destribcreate/massdestribparamspage.cpp \
    src/massop/destribcreate/massdestribparammodel.cpp \
    src/massop/destribcreate/massdestribitemparamdelegate.cpp \
    src/tnsnames.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    options/externaltoolspage.ui \
    options/generaloptions.ui \
    options/tablegroupoptions.ui \
    options/usercommandsgroup.ui \
    src/debugconnect.ui \
    src/widgets/comparefmt/comparecppstructpage.ui \
    src/widgets/comparefmt/comparefmtviewtab.ui \
    src/widgets/comparefmt/compareparamspage.ui \
    src/widgets/comparefmt/compareselecttablepage.ui \
    src/widgets/guiconverterdlg.ui \
    src/widgets/sqlconvertordlg.ui \
    src/widgets/tablestructsqldlg.ui \
    ui/connactionpage.ui \
    ui/gensqltemplatedlg.ui \
    src/massop/destribcreate/massdestribprogress.ui \
    src/widgets/fmtsegmentflagsdlg.ui \
    src/widgets/selectfolderdlg.ui \
    ui/dbinitdlg.ui \
    ui/documentwindow.ui \
    ui/errordlg.ui \
    ui/fmtfielddlg.ui \
    ui/fmtworkwindow.ui \
    ui/mainwindow.ui \
    ui/oracleauthdlg.ui \
    ui/tablesgroupsdlg.ui \
    ui/windowslistdlg.ui \
    ui/fmtworkwndgen.ui \
    ui/gencppsettings.ui \
    ui/selectconnectiondlg.ui \
    ui/stringlistdlg.ui \
    ui/selectfiltereddlg.ui \
    ui/fmteditcontentfilter.ui \
    ui/dataselectdialog.ui \
    ui/connactionpage.ui \
    src/widgets/editcontent/import/selecttypepage.ui \
    src/widgets/editcontent/import/texteditpage.ui \
    src/widgets/editcontent/import/tablemaptofmtwidget.ui \
    src/widgets/editcontent/import/mapfieldspage.ui \
    src/massop/massopselectoperationpage.ui \
    src/massop/inittables/massinittableparams.ui \
    src/massop/inittables/massinittablesprogress.ui \
    src/massop/btrvtemplate/massopbtrvtemplateresultpage.ui \
    src/massop/massopenfunc/massopenfucnparams.ui \
    src/massop/destribcreate/massdestribparamspage.ui

RESOURCES += \
    res/fmt.qrc

HEADERS += \
    h/gensqltemplatedlg.h \
    h/DataStructure.hpp \
    h/LockVar.hpp \
    h/connectioninfo.h \
    h/dbinitdlg.h \
    h/documentwindow.h \
    h/exporttoxmlwizard.h \
    h/expresultpage.h \
    h/expsettingspage.h \
    h/fmtapplication.h \
    h/fmtcore.h \
    h/fmtfield.h \
    h/fmtfielddlg.h \
    h/fmtfieldsdelegate.h \
    h/fmtfieldstableview.h \
    h/fmtfildsmodel.h \
    h/fmtfromrichtext.h \
    h/fmtimpexpwrp.h \
    h/fmtindecesdelegate.h \
    h/fmtindecesmodel.h \
    h/fmtindecesmodelitem.h \
    h/fmtindex.h \
    h/fmtindextreeview.h \
    h/fmtlib.h \
    h/fmtlib_global.h \
    h/fmttable.h \
    h/fmttablesmodel.h \
    h/fmtworkwindow.h \
    h/mainwindow.h \
    h/oracleauthdlg.h \
    h/oracletnslistmodel.h \
    h/subwindowsmodel.h \
    h/tablesdock.h \
    h/tablesdockwidget.h \
    h/tablesgroupprovider.h \
    h/tablesgroupsdlg.h \
    h/tablesselectwidget.h \
    h/treecombobox.h \
    h/windowslistdlg.h \
    h/resource.h \
    h/fmtgeninterface.h \
    h/fmtgentablessql.h \
    h/fmtlibfactory.h \
    h/fmtworkwndgen.h \
    h/fmtgencpptemplate.h \
    h/gencppsettings.h \
    h/fmtundotableproperty.h \
    h/fmtundotableproperty.h \
    h/fmtundotableaddfield.h \
    h/fmtundofieldproperty.h \
    h/fmtundotableremovefield.h \
    h/fmtundotableaddindex.h \
    h/fmtundoremoveindex.h \
    h/fmtundoremovesegment.h \
    h/fmtundoindexaddsegment.h \
    h/fmtsegment.h \
    h/loggingcategories.h \
    h/fmtundoindexproperty.h \
    h/fmtundoindexsegmentproperty.h \
    h/undolistpopup.h \
    h/fmtdbftoolwrp.h \
    h/tablecomboaction.h \
    h/toolbtncombo.h \
    h/fmtgenhotfix.h \
    h/connectionlistmodel.h \
    h/selectconnectiondlg.h \
    h/stringlistdlg.h \
    h/selectfieldsmodel.h \
    h/selectfiltereddlg.h \
    h/lineeditaction.h \
    h/fmttablelistdelegate.h \
#    src/scriptable/jsfmttable.h \
    h/fmtfile.h \
    h/fmtscriptwindow.h \
    h/fmrichtextwidget.h \
    h/fmteditcontentmodel.h \
    h/fmteditcontentwindow.h \
    options/externaltoolspage.h \
    options/fmtcommandspage.h \
    options/fmtoptionsdlg.h \
    options/generaloptions.h \
    options/tablegroupoptions.h \
    options/usercommandsgroup.h \
    rslexecutors/checksaveexecutor.h \
    rslexecutors/generatorrslexecutor.h \
    rslexecutors/toolbaractionexecutor.h \
    src/core/fmttablecomparemodel.h \
    src/debugconnect.h \
    src/massop/destribcreate/massdestribprogress.h \
    src/models/tableobjectsmodel.h \
    src/queryeditor/queryeditor.h \
    src/widgets/comparefmt/comparecppstructpage.h \
    src/widgets/comparefmt/comparefmtviewtab.h \
    src/widgets/comparefmt/comparefmtwizard.h \
    src/widgets/comparefmt/compareparamspage.h \
    src/widgets/comparefmt/compareselecttablepage.h \
    src/widgets/editcontent/fmteditcontentdialog.h \
    src/widgets/connectionwzardpage/connactionpage.h \
    h/datelineedit.h \
    src/widgets/editcontent/fmteditcontentfilter.h \
    h/dataselectdialog.h \
    h/fmtfieldstableheaderdelegate.h \
    mdisubinterface.h \
    fmtgeninputservicecpptemplate.h \
    fmtgencppclasstemplate.h \
    src/widgets/editcontent/import/importwizard.h \
    src/widgets/editcontent/import/selecttypepage.h \
    src/widgets/editcontent/import/texteditpage.h \
    src/widgets/editcontent/import/tablemaptofmtwidget.h \
    h/columnalignedlayout.h \
    h/fmtundotablepastefield.h \
    src/widgets/editcontent/import/mapfieldspage.h \
    src/widgets/editcontent/import/mapfildsmodel.h \
    h/geninterfacefactorymodel.h \
    h/massopinterface.h \
    h/massoperationwizard.h \
    src/core/abstractfactorymodel.hpp \
    src/massop/massopselectoperationpage.h \
    src/massop/massinittableoperation.h \
    src/massop/massopselecttablespage.h \
    src/massop/inittables/massinittableparams.h \
    src/massop/inittables/massinittablesparammodel.h \
    src/massop/inittables/massinittablesprogress.h \
    src/massop/btrvtemplate/massopbtrvtemplate.h \
    src/massop/btrvtemplate/massopbtrvtemplateresultpage.h \
    src/massop/massopenfunc/massopenfunctemplate.h \
    src/massop/massopenfunc/massopenfucnparams.h \
    src/massop/destribcreate/massdestribcreate.h \
    src/massop/destribcreate/massdestribparamspage.h \
    src/massop/destribcreate/massdestribparammodel.h \
    src/massop/destribcreate/massdestribitemparamdelegate.h \
    src/widgets/filteredtablewidget.h \
    h/fmtsegmentflagsdlg.h \
    h/selectfolderdlg.h \
    h/odbctablemodel.h \
    h/odbctablemodeldelegate.h \
    h/recentconnectionlist.h \
    h/tnsnames.h \
    src/widgets/guiconverterdlg.h \
    src/widgets/sqlconvertordlg.h \
    src/widgets/tablestructsqldlg.h \
    src/wizards/RichTextToInsertWizard/richtexttoinsertwizard.h

INCLUDEPATH += $$PWD/../FmtDbgHelp
target.path = $$PWD/../bin
INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime $$PWD/../ToolsRuntimeProj/ToolsRuntime/optionsdlg $$PWD/../ToolsRuntimeProj/ToolsRuntime/widgets
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime

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

copyToDestDir($$OTHER_FILES, $$OUT_PWD/../bin/mac)
copyToDestDir($$OTHER_FILES, $$OUT_PWD/../WorkFMT/debug/mac)
