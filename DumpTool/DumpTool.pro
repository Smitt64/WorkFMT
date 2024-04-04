QT += core gui widgets sql

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    actionlogpage.cpp \
    baseloghighlighter.cpp \
    baseoperation.cpp \
    dumptoolwizard.cpp \
    exportoperation.cpp \
    expparampage.cpp \
    exppgparampage.cpp \
    filelistdialog.cpp \
    impfileselectpage.cpp \
    importoperation.cpp \
    main.cpp \
    pgaddjarsdlg.cpp \
    pgexportaddprmdlg.cpp \
    pgexportoperation.cpp \
    pgimportoperation.cpp \
    pgimpparampage.cpp \
    selectactionpage.cpp \
    selectserverdlg.cpp \
    selserverdirdlg.cpp \
    summarypage.cpp \
    userconnectionpage.cpp

HEADERS += \
    actionlogpage.h \
    baseloghighlighter.h \
    baseoperation.h \
    dumptoolwizard.h \
    exportoperation.h \
    expparampage.h \
    exppgparampage.h \
    filelistdialog.h \
    impfileselectpage.h \
    importoperation.h \
    pgaddjarsdlg.h \
    pgexportaddprmdlg.h \
    pgexportoperation.h \
    pgimportoperation.h \
    pgimpparampage.h \
    selectactionpage.h \
    selectserverdlg.h \
    selserverdirdlg.h \
    summarypage.h \
    userconnectionpage.h

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

RC_FILE = res/dumptool.rc

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib/h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix:!macx: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

target.path = $$PWD/../bin
INSTALLS += target

FORMS += \
    actionlogpage.ui \
    expparampage.ui \
    exppgparampage.ui \
    filelistdialog.ui \
    impfileselectpage.ui \
    pgaddjarsdlg.ui \
    pgexportaddprmdlg.ui \
    pgimpparampage.ui \
    selectactionpage.ui \
    selectserverdlg.ui \
    selserverdirdlg.ui \
    summarypage.ui \
    userconnectionpage.ui

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RsWorkTools/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RsWorkTools/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../RsWorkTools/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../RsWorkTools/ToolsRuntime
DEPENDPATH += $$PWD/../RsWorkTools/ToolsRuntime
