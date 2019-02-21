#ifndef FMTGENINPUTSERVICECPPTEMPLATE_H
#define FMTGENINPUTSERVICECPPTEMPLATE_H

#include "fmtgeninterface.h"

class FmtGenInputServiceCppTemplate : public FmtGenInterface
{
public:
    FmtGenInputServiceCppTemplate();
    virtual ~FmtGenInputServiceCppTemplate();

    virtual GenType getContentType() const { return FmtGenInterface::GenCpp; }

    static QString getSelectTypedefName(const FmtSharedTablePtr &pTable);
    static QString getCacheClassName(const FmtSharedTablePtr &pTable);
    static QString getCacheWrapperName(const FmtSharedTablePtr &pTable);
    static QString getEnumRSB_INPUT_SERVICE(const FmtSharedTablePtr &pTable);
    static QString getInputServiceTypedef(const FmtSharedTablePtr &pTable);
    static QString getInputServiceClassName(const FmtSharedTablePtr &pTable);
    static QString getInputServicePtMemberName(const FmtSharedTablePtr &pTable);

protected:
    virtual QByteArray makeContent(FmtSharedTablePtr pTable);

private:
    void createCacheClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createCacheClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createInputServiceAdmOpClass(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createGetSelectIDForService(const FmtSharedTablePtr &pTable, QTextStream &stream);
};

#endif // FMTGENINPUTSERVICECPPTEMPLATE_H
