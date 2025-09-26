#ifndef RICHTEXTTOINSERTRUN_H
#define RICHTEXTTOINSERTRUN_H

#include <QObject>
#include <QRunnable>
#include <QMap>

class FmtTable;
class QTextDocument;
class QTextTable;
class RichTextToInsertRun : public QObject, public QRunnable
{
    Q_OBJECT
public:
    RichTextToInsertRun(QObject *parent = nullptr);
    virtual ~RichTextToInsertRun();

    virtual void run() Q_DECL_OVERRIDE;

    void setData(QTextDocument *document, FmtTable *table, bool firstRowAsHeader, const QMap<int, QString> &fieldMapping);

signals:
    void finished(const QString &plsqlBlock);

private:
    QString generatePlsqlBlock();
    QTextTable* findTableInDocument();
    QString generateFunctionHeader(const QString &functionName, int columnCount);
    QString generateInsertStatement(int row, QTextTable *table);
    QString getCellValue(QTextTable *table, int row, int col);
    QString generateFunctionCalls(const QString &functionName, int startRow, int rowCount, int columnCount);

    QTextDocument *m_pDocument = nullptr;
    FmtTable *m_pTable = nullptr;
    bool m_bFirstRowAsHeader = false;
    QMap<int, QString> m_FieldMapping;
};

#endif // RICHTEXTTOINSERTRUN_H
