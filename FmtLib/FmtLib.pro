#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T12:06:53
#
#-------------------------------------------------

QT       += widgets sql xml script scripttools core
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

INCLUDEPATH += ./h
INCLUDEPATH += .
LIBS += -lversion

SOURCES += \
    src/core/fmtcore.cpp \
    src/connectioninfo.cpp \
    src/dbinitdlg.cpp \
    src/documentwindow.cpp \
    src/errordlg.cpp \
    src/fmtapplication.cpp \
    src/fmtfielddlg.cpp \
    src/fmtworkwindow.cpp \
    src/impexpparams.cpp \
    src/mainwindow.cpp \
    src/oracleauthdlg.cpp \
    src/tablesgroupsdlg.cpp \
    src/windowslistdlg.cpp \
    src/core/fmtcore.cpp \
    src/core/fmtlib.cpp \
    src/core/hotfixhelp.cpp \
    src/core/tablesgroupprovider.cpp \
    src/fmt/fmterrors.cpp \
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
    src/wizards/exporttoxmlwizard.cpp \
    src/wizards/expresultpage.cpp \
    src/wizards/expsettingspage.cpp \
    src/wizards/fmtfromrichtext.cpp \
    src/aboutdlg.cpp \
    src/fmt/fmtgeninterface.cpp \
    src/fmtgentablessql.cpp \
    src/fmtworkwndgen.cpp \
    src/widgets/codeeditor.cpp \
    src/highlighter.cpp \
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
    src/models/fmterrorfiltermodel.cpp \
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
    src/core/showcodedialog.cpp \
    h/lineeditaction.cpp \
    src/models/fmttablelistdelegate.cpp \
    src/fmt/fmtfile.cpp \
    src/fmtscriptwindow.cpp \
    src/widgets/fmrichtextwidget.cpp \
    src/models/fmteditcontentmodel.cpp \
    src/widgets/editcontent/fmteditcontentwindow.cpp \
    src/widgets/editcontent/fmteditcontentdialog.cpp \
    src/logsettingsdlg.cpp \
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
    src/wizards/massoperationwizard.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ui/aboutdlg.ui \
    ui/dbinitdlg.ui \
    ui/documentwindow.ui \
    ui/errordlg.ui \
    ui/fmtfielddlg.ui \
    ui/fmtworkwindow.ui \
    ui/impexpparams.ui \
    ui/mainwindow.ui \
    ui/oracleauthdlg.ui \
    ui/tablesgroupsdlg.ui \
    ui/windowslistdlg.ui \
    ui/fmtworkwndgen.ui \
    ui/gencppsettings.ui \
    ui/selectconnectiondlg.ui \
    ui/stringlistdlg.ui \
    ui/selectfiltereddlg.ui \
    ui/logsettingsdlg.ui \
    ui/fmteditcontentfilter.ui \
    ui/dataselectdialog.ui \
    src/widgets/editcontent/import/selecttypepage.ui \
    src/widgets/editcontent/import/texteditpage.ui \
    src/widgets/editcontent/import/tablemaptofmtwidget.ui \
    src/widgets/editcontent/import/mapfieldspage.ui

RESOURCES += \
    res/fmt.qrc

HEADERS += \
    h/connectioninfo.h \
    h/dbinitdlg.h \
    h/documentwindow.h \
    h/errordlg.h \
    h/exporttoxmlwizard.h \
    h/expresultpage.h \
    h/expsettingspage.h \
    h/fmtapplication.h \
    h/fmtcore.h \
    h/fmterrors.h \
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
    h/impexpparams.h \
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
    h/aboutdlg.h \
    h/resource.h \
    h/fmtgeninterface.h \
    h/fmtgentablessql.h \
    h/fmtlibfactory.h \
    h/fmtworkwndgen.h \
    h/codeeditor.h \
    h/highlighter.h \
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
    h/fmterrorfiltermodel.h \
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
    src/widgets/editcontent/fmteditcontentdialog.h \
    h/logsettingsdlg.h \
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
    h/massoperationwizard.h

INCLUDEPATH += $$PWD/../FmtDbgHelp
target.path = ../bin
INSTALLS += target
