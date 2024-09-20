#ifndef FMTGENTABLESSQL_H
#define FMTGENTABLESSQL_H

#include "fmtgeninterface.h"

class FmtGenTablesSql : public FmtGenInterface
{
    Q_OBJECT
public:
    FmtGenTablesSql();
    virtual ~FmtGenTablesSql();
    virtual int getContentType() const { return HighlighterSql; }

    virtual GenHighlightingRuleList highlightingRuleList() const;
    virtual QByteArray makeContent(FmtSharedTablePtr pTable);

private:
    GenHighlightingRuleList m_HighlightingRuleList;
};

#endif // FMTGENTABLESSQL_H
