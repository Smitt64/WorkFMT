#ifndef FMTGENINPUTSERVICECPPTEMPLATE_H
#define FMTGENINPUTSERVICECPPTEMPLATE_H

#include "fmtgeninterface.h"

/*#define RSBPARTY_HPP "RsbParty.hpp"
#define RSBPARTY_CPP "RsbParty.cpp"
#define CBPTMSG_H "cbptmsg.h"
#define PTR_PT_C "ptr_pt.c"*/

#define HEADER_H "*.h"
#define BF_CPP "bf_*.cpp"
#define ALINPSRV_HPP "AlInpSrv.hpp"
#define RSBPARTY_HPP "RsbParty.hpp"
#define RSBPARTY_CPP "RsbParty.cpp"

class FmtGenInputServiceCppTemplate : public FmtGenInterface
{
public:
    FmtGenInputServiceCppTemplate();
    virtual ~FmtGenInputServiceCppTemplate();

    virtual QStringList tabs() Q_DECL_OVERRIDE;
    virtual int getContentType() const Q_DECL_OVERRIDE { return HighlighterCpp; }

    static QString getSelectTypedefName(const FmtSharedTablePtr &pTable);
    static QString getCacheClassName(const FmtSharedTablePtr &pTable);
    static QString getCacheWrapperName(const FmtSharedTablePtr &pTable);
    static QString getEnumRSB_INPUT_SERVICE(const FmtSharedTablePtr &pTable);
    static QString getInputServiceTypedef(const FmtSharedTablePtr &pTable);
    static QString getInputServiceClassName(const FmtSharedTablePtr &pTable);
    static QString getInputServicePtMemberName(const FmtSharedTablePtr &pTable);

    virtual GenHighlightingRuleList highlightingRuleList() const Q_DECL_OVERRIDE;

protected:
    virtual QMap<QString, QByteArray> makeContent(FmtSharedTablePtr pTable) Q_DECL_OVERRIDE;

private:
    void createCacheClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createCacheClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createInputServiceAdmOpClass(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createGetSelectIDForService(const FmtSharedTablePtr &pTable, QTextStream &stream);

    GenHighlightingRuleList m_HighlightingRuleList;
};

#endif // FMTGENINPUTSERVICECPPTEMPLATE_H
