#ifndef FMTGENCPPTEMPLATE_H
#define FMTGENCPPTEMPLATE_H

#include "fmtgeninterface.h"
#include "gencppsettings.h"

typedef struct
{
    qint16 m_MaxCppLenght, m_MaxFldLenght;
    QString m_StructName, m_UnionName, m_EnumName, m_SkfDefaultFunc;

    QString m_SkfDefaultParams;

    QMap<FmtIndex*, QString> m_IndexEnumValue;
    QMap<FmtIndex*, QString> m_IndexUnionValue;
    QMap<FmtIndex*, QString> m_SkfNameValue;
    QMap<FmtIndex*, QString> m_SkfParamsValue;
    QMap<FmtIndex*, QString> m_SkfFullFuncNameValue;

    QStringList m_Decl;
} GenCppTemplateBlock;

class FmtGenCppTemplate : public FmtGenInterface
{
public:
    enum
    {
        SkfMode_Create = 0x01,
        SkfMode_Display = 0x02,
        SkfMode_All = SkfMode_Create | SkfMode_Display,
    };
    FmtGenCppTemplate();
    virtual ~FmtGenCppTemplate();

    virtual GenType getContentType() const { return FmtGenInterface::GenCpp; }

    virtual void propertyEditor(QWidget *parent);
    virtual bool hasPropertes() const { return false; }//{ return true; }

    void initSettings();
    void CreateBlocks(const FmtSharedTablePtr &pTable);

    GenCppSettingsParams *param();

    void createOpenFuncDecl(const FmtSharedTablePtr &pTable, QTextStream &stream, bool inlineComment = false);
    void createStruct(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createKeysUnion(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createKeysEnum(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createOpenFunc(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createFindFunctions(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void createSkfDeclFunctions(const FmtSharedTablePtr &pTable, QTextStream &stream, const int &Mode = SkfMode_All);
    void createSkfFunctions(const FmtSharedTablePtr &pTable, QTextStream &stream);

    void createSkfKfReturnSegment(const QString &fldName, const QString &keyNam, QTextStream &stream, bool asString = false, bool descOrder = false);
    void createSkfKfFunctions(FmtIndex *pIndex, QTextStream &stream);
    void createDeclExtern(const FmtSharedTablePtr &pTable, QTextStream &stream);
    void WriteTableComment(const FmtSharedTablePtr &pTable, QTextStream &stream, bool inlineComment = false);

protected:
    virtual QByteArray makeContent(FmtSharedTablePtr pTable);

private:
    void WrapSkfAssignValue(QTextStream &stream, const QString &keyName, const QString &fldName, const QString &minval, const QString &maxval, const QString &zeroval, const bool &descOrder);

    qint16 calcMaxCppLenght(qint16 *maxfieldname, const FmtSharedTablePtr &pTable);
    QString normalizeFieldName(const QString &m_Name);
    QString getBFileName(const QString &table) const;

    void CreateFindFuncForIndex(FmtIndex *pIndex, const FmtSharedTablePtr &pTable, QTextStream &stream, bool isDefault, int IndexNum);

    void AppendFunctionDeclExtern(GenCppTemplateBlock *block, const QString &func);

    GenCppTemplateBlock *getTemplateBlock(const FmtSharedTablePtr &pTable);
    GenCppTemplateBlock *getTemplateBlock(const FmtTable *pTable);
    QString FormatName(const QString &Mask, const GenCppTemplateBlock *block);
    GenCppSettingsParams prm;

    QMap<FmtTable*,GenCppTemplateBlock*> m_Blocks;
    QList<GenCppTemplateBlock*> m_BlocksStore;
};

#endif // FMTGENCPPTEMPLATE_H
