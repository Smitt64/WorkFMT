QT -= gui
QT += sql
#sql-private

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++98

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
#    rsdcachedresult.cpp \
    rsdcommandex.cpp \
    rsddriver.cpp \
    rsdsqlresult.cpp \
    sqldriverplugin.cpp

HEADERS += \
    BaseErrorSetter.hpp \
#    rsdcachedresult.h \
    rsdcommandex.h \
    rsddriver.h \
    rsdsqlresult.h \
    sqldriverplugin.h

DISTFILES += qrsd.json

DISTFILES += \
    res/qrsd.rc
#RC_FILE = res/qrsd.rc

#_MBCS
INCLUDEPATH += $$PWD/rsd/include $$PWD/tools/h

DEFINES +=  SQLBUILD NUMERIC_AS_MONEY USE_NUMERIC NUMERIC_OVER_DOUBLE MDB_USE_INDEX_MAPS USE_FDECIMAL BMKASCLASS NO_OLD_FIELD_TYPES
#DEFINES += _MBCS

LIBS += -L$$PWD/rsd/lib/release -lrsdc
LIBS += -L$$PWD/tools/lib/release -lrsrtlwm
#-O2
#QMAKE_CXXFLAGS_RELEASE -=  -MD
#QMAKE_CXXFLAGS_RELEASE += -MDd
#QMAKE_LFLAGS_RELEASE -= /INCREMENTAL:NO /OPT:REF
#QMAKE_LFLAGS_RELEASE += /INCREMENTAL /OPT:NOREF /DEBUG

target.path = ../bin/sqldrivers
INSTALLS += target
