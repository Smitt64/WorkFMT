#ifndef FMTLIB_GLOBAL_H
#define FMTLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FMTLIB_LIBRARY)
#  define FMTLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FMTLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FMTLIB_GLOBAL_H
