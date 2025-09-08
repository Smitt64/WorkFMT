#ifndef RICHTEXTTOINSERTWIZARD_H
#define RICHTEXTTOINSERTWIZARD_H

#include <QWizard>

class ConnectionInfo;
class QTextDocument;
class MassOpSelectTablesPage;
class FmtTable;
class RichTextPage;
class FmtImportFldSourcePage;
class RichTextToInsertWizard : public QWizard
{
    Q_OBJECT
public:
    RichTextToInsertWizard(ConnectionInfo *info, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    RichTextToInsertWizard(FmtTable *Table, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    bool addTables(const QString &str);
    bool removeTables(const QString &str);

    FmtTable *table() const;
    QTextDocument *document();

private:
    void Init(ConnectionInfo *info);
    MassOpSelectTablesPage *m_pSelectTables;
    RichTextPage *m_pRichTextPage;
    FmtImportFldSourcePage *m_pSourcePage;

    QTextDocument *m_pRichText;
    FmtTable *m_pTable;
    QStringList m_Tables;
};

#endif // RICHTEXTTOINSERTWIZARD_H
