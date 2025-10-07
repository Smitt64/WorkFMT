#ifndef RICHTEXTTOINSERTWIZARD_H
#define RICHTEXTTOINSERTWIZARD_H

#include <QWizard>
#include <QTextDocument>
#include <QMap>

class ConnectionInfo;
class FmtTable;
class MassOpSelectTablesPage;
class RichTextPage;
class FmtImportFldSourcePage;
class ExistsConditionPage;
class RichTextToInsertResultPage;

class RichTextToInsertWizard : public QWizard
{
    Q_OBJECT

public:
    RichTextToInsertWizard(ConnectionInfo *info, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    RichTextToInsertWizard(FmtTable *Table, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    FmtTable *table() const;
    QTextDocument *document();
    QMap<QString, QString> fieldMapping() const;
    bool firstAsHeader() const { return field("FirstAsHeader").toBool(); }

    bool addTables(const QString &str);
    bool removeTables(const QString &str);

private:
    void Init(ConnectionInfo *info);

private:
    FmtTable *m_pTable = nullptr;
    QStringList m_Tables;

    QTextDocument *m_pRichText = nullptr;
    MassOpSelectTablesPage *m_pSelectTables = nullptr;
    RichTextPage *m_pRichTextPage = nullptr;
    FmtImportFldSourcePage *m_pSourcePage = nullptr;
    ExistsConditionPage *m_pExistsConditionPage = nullptr;
    RichTextToInsertResultPage *m_pResultPage = nullptr;
};

#endif // RICHTEXTTOINSERTWIZARD_H
