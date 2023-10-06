#ifndef FMTGENTABLESSQL_H
#define FMTGENTABLESSQL_H

#include "fmtgeninterface.h"
#include "highlighter.h"

class FmtGenTablesSql : public FmtGenInterface
{
    Q_OBJECT
public:
    FmtGenTablesSql();
    virtual ~FmtGenTablesSql();
    virtual GenType getContentType() const { return FmtGenInterface::GenSql; }

    virtual FmtGenHighlightingRuleList highlightingRuleList() const;
    virtual QByteArray makeContent(FmtSharedTablePtr pTable);

private:
    FmtGenHighlightingRuleList m_HighlightingRuleList;
};

#endif // FMTGENTABLESSQL_H
