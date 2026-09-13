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
    virtual QMap<QString, QByteArray> makeContent(FmtSharedTablePtr pTable) Q_DECL_OVERRIDE;

private:
    GenHighlightingRuleList m_HighlightingRuleList;
};

#endif // FMTGENTABLESSQL_H
