#ifndef DIFFTOSCRIPTL_GLOBAL_H
#define DIFFTOSCRIPTL_GLOBAL_H

#include <QtCore/qglobal.h>

// DiffToScriptl_global.h

#if defined(DIFFTOSCRIPTL_LIBRARY)
#  define DIFFTOSCRIPTL_EXPORT Q_DECL_EXPORT
#else
#  define DIFFTOSCRIPTL_EXPORT Q_DECL_IMPORT
#endif

#endif // DIFFTOSCRIPTL_GLOBAL_H
