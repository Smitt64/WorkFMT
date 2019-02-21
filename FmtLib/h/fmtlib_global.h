#ifndef FMTLIB_GLOBAL_H
#define FMTLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FMTLIB_LIBRARY)
#  define FMTLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FMTLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

typedef qint16  FmtNumber5;
typedef qint32  FmtNumber10;

typedef quint16 FmtFldType;
typedef qint16  FmtFldIndex;
typedef quint64 FmtRecId;

#endif // FMTLIB_GLOBAL_H
