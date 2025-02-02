QT       += core gui sql
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hotfixcontentmodel.cpp \
    hotfixwizard.cpp \
    main.cpp \
    model/contenttreeitem.cpp \
    model/filecontenttreeitem.cpp \
    model/fmtcontenttreeitem.cpp \
    model/foldercontenttreeitem.cpp \
    seldirspage.cpp \
    structsettingspage.cpp

HEADERS += \
    hotfixcontentmodel.h \
    hotfixwizard.h \
    model/contenttreeitem.h \
    model/filecontenttreeitem.h \
    model/fmtcontenttreeitem.h \
    model/foldercontenttreeitem.h \
    seldirspage.h \
    structsettingspage.h

# Default rules for deployment.
target.path = $$PWD/../bin
INSTALLS += target

FORMS += \
    datpage.ui \
    seldirspage.ui \
    structsettingspage.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/release/ -lFmtLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FmtLib/debug/ -lFmtLib
else:unix: LIBS += -L$$OUT_PWD/../FmtLib/ -lFmtLib

INCLUDEPATH += $$PWD/../FmtLib/h
DEPENDPATH += $$PWD/../FmtLib

RESOURCES += \
    hfcres.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/release/ -lToolsRuntime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/debug/ -lToolsRuntime
else:unix: LIBS += -L$$OUT_PWD/../ToolsRuntimeProj/ToolsRuntime/ -lToolsRuntime

INCLUDEPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
DEPENDPATH += $$PWD/../ToolsRuntimeProj/ToolsRuntime
