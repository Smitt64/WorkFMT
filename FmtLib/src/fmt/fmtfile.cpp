#include "fmtfile.h"

FmtFile::FmtFile()
{

}

FmtFile::FmtFile(QSharedPointer<FmtTable> &pTable)
{
    Q_UNUSED(pTable);
}

FmtFile::FmtFile(const FmtFile &other)
{
    Q_UNUSED(other);
}

FmtFile::~FmtFile()
{

}

FmtFile &FmtFile::operator =(const FmtFile &other)
{
    Q_UNUSED(other);
    return *this;
}
