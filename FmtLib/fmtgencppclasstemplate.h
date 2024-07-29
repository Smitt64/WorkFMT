#ifndef FMTGENCPPCLASSTEMPLATE_H
#define FMTGENCPPCLASSTEMPLATE_H

#include "fmtgeninterface.h"
#include "gencppsettings.h"

class FmtGenCppClassTemplate : public FmtGenInterface
{
public:
    FmtGenCppClassTemplate();
    virtual ~FmtGenCppClassTemplate();

    virtual int getContentType() const { return HighlighterCpp; }
    virtual GenHighlightingRuleList highlightingRuleList() const;

protected:
    virtual QByteArray makeContent(FmtSharedTablePtr pTable);

private:
    void createClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createFirstDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createNextDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createLastDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createPreviousDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createUpdateDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createGetDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createDeleteDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createAddDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);

    void createRslClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createRslClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createRslFldPropertyDefenition(const FmtSharedTablePtr &pTable, FmtField *pfld, QTextStream &stream);
    void createRslClassMoveFuncDefenition(const FmtSharedTablePtr &pTable, const QString &func, QTextStream &stream, const bool &intstat = false);
    void createRslClassGetDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createRslAddDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createRslClassDefines(const FmtSharedTablePtr &pTable, QTextStream &stream);

    QString getClassName(const FmtSharedTablePtr &pTable);
    QString getRslClassName(const FmtSharedTablePtr &pTable);
    QString FormatName(QString &Mask, const FmtSharedTablePtr &pTable);
    QString normalizeFieldName(const QString &m_Name);
    QString getUnionKeyName(const FmtSharedTablePtr &pTable, const qint16 &key);
    GenCppSettingsParams prm;
    QString pkUnionName;

    GenHighlightingRuleList m_HighlightingRuleList;
};

#endif // FMTGENCPPCLASSTEMPLATE_H
