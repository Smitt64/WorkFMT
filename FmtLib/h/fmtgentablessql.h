#ifndef FMTGENTABLESSQL_H
#define FMTGENTABLESSQL_H

#include "fmtgeninterface.h"

class FmtGenTablesSql : public FmtGenInterface
{
    Q_OBJECT
public:
    FmtGenTablesSql();

    virtual GenType getContentType() const { return FmtGenInterface::GenSql; }

    virtual QWidget *propertyEditor() { return NULL; }
    virtual bool hasPropertes() const { return false; }

protected:
    virtual QByteArray makeContent(QSharedPointer<FmtTable> pTable);
};

#endif // FMTGENTABLESSQL_H
