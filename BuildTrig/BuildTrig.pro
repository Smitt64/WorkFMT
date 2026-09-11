QT       += core gui sql xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    buildtrigwizard.cpp \
    ../DiffToScript/wizard/svnlogmodel.cpp \
    ../DiffToScript/wizard/svnlogdlg.cpp \
    repopage.cpp \
    triggerpage.cpp

HEADERS += \
    buildtrigwizard.h \
    ../DiffToScript/wizard/svnlogmodel.h \
    ../DiffToScript/wizard/svnlogdlg.h \
    repopage.h \
    triggerpage.h

FORMS += \
    repopage.ui \
    ../DiffToScript/wizard/svnlogdlg.ui

RESOURCES += \
    ../DiffToScript/res.qrc \
    res/buildtrig.qrc

RC_FILE = res/buildtrig.rc

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

target.path = $$PWD/../bin
INSTALLS += target

INCLUDEPATH += $$PWD/../FmtLib
DEPENDPATH += $$PWD/../FmtLib

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib/h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix:!macx: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime

INCLUDEPATH += $$PWD/../DiffToScript/wizard
DEPENDPATH += $$PWD/../DiffToScript/wizard
