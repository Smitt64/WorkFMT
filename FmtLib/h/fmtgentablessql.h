#ifndef FMTGENTABLESSQL_H
#define FMTGENTABLESSQL_H

#include "fmtgeninterface.h"

class FmtGenTablesSql : public FmtGenInterface
{
    Q_OBJECT
public:
    FmtGenTablesSql();
    virtual ~FmtGenTablesSql();
    virtual GenType getContentType() const { return FmtGenInterface::GenSql; }

    virtual QByteArray makeContent(FmtSharedTablePtr pTable);
};

#endif // FMTGENTABLESSQL_H
