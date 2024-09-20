#ifndef FMTGENHOTFIX_H
#define FMTGENHOTFIX_H

#include "fmtgeninterface.h"

class FmtGenHotFix : public FmtGenInterface
{
    Q_OBJECT
public:
    FmtGenHotFix();
    virtual ~FmtGenHotFix();
    virtual int getContentType() const { return HighlighterSql; }

protected:
    virtual QByteArray makeContent(QSharedPointer<FmtTable> pTable);
};

#endif // FMTGENHOTFIX_H
