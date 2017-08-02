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
} GenCppTemplateBlock;

class FmtGenCppTemplate : public FmtGenInterface
{
public:
    FmtGenCppTemplate();

    virtual GenType getContentType() const { return FmtGenInterface::GenCpp; }

    virtual void propertyEditor(QWidget *parent);
    virtual bool hasPropertes() const { return true; }

protected:
    virtual QByteArray makeContent(QSharedPointer<FmtTable> pTable);

private:
    void createOpenFuncDecl(const QSharedPointer<FmtTable> &pTable, QTextStream &stream);
    void createStruct(const QSharedPointer<FmtTable> &pTable, QTextStream &stream);
    void createKeysUnion(const QSharedPointer<FmtTable> &pTable, QTextStream &stream);
    void createKeysEnum(const QSharedPointer<FmtTable> &pTable, QTextStream &stream);
    void createOpenFunc(const QSharedPointer<FmtTable> &pTable, QTextStream &stream);
    void createFindFunctions(const QSharedPointer<FmtTable> &pTable, QTextStream &stream);
    void createSkfDeclFunctions(const QSharedPointer<FmtTable> &pTable, QTextStream &stream);
    void createSkfFunctions(const QSharedPointer<FmtTable> &pTable, QTextStream &stream);

    void createSkfKfReturnSegment(const QString &fldName, const QString &keyNam, QTextStream &stream, bool asString = false, bool descOrder = false);
    void createSkfKfFunctions(FmtIndex *pIndex, QTextStream &stream);

    qint16 calcMaxCppLenght(qint16 *maxfieldname, const QSharedPointer<FmtTable> &pTable);
    QString normalizeFieldName(const QString &m_Name);
    QString getBFileName(const QString &table) const;

    void CreateFindFuncForIndex(FmtIndex *pIndex, const QSharedPointer<FmtTable> &pTable, QTextStream &stream, bool isDefault, int IndexNum);

    void CreateBlocks(const QSharedPointer<FmtTable> &pTable);
    GenCppTemplateBlock *getTemplateBlock(const QSharedPointer<FmtTable> &pTable);
    GenCppTemplateBlock *getTemplateBlock(const FmtTable *pTable);
    QString FormatName(QString &Mask, const GenCppTemplateBlock *block);
    GenCppSettingsParams prm;

    QMap<FmtTable*,GenCppTemplateBlock*> m_Blocks;
    QList<GenCppTemplateBlock*> m_BlocksStore;
};

#endif // FMTGENCPPTEMPLATE_H
