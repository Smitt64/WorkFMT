#ifndef FMTFILE_H
#define FMTFILE_H

#include <QtCore>
#include <fmttable.h>
#include "fmtlib_global.h"

class FMTLIBSHARED_EXPORT FmtFile
{
public:
    FmtFile();
    FmtFile(QSharedPointer<FmtTable> &pTable);
    FmtFile(const FmtFile &other);
    virtual ~FmtFile();

    FmtFile &operator =(const FmtFile &other);
};

#endif // FMTFILE_H
