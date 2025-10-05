#ifndef RICHTEXTTOINSERTRUN_H
#define RICHTEXTTOINSERTRUN_H

#include <QObject>
#include <QRunnable>
#include <QMap>
#include <QDate>
#include <QTime>
#include <QDateTime>

class FmtTable;
class FmtField;
class QTextDocument;
class QTextTable;

class RichTextToInsertRun : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RichTextToInsertRun(QObject *parent = nullptr);
    virtual ~RichTextToInsertRun();

    virtual void run() Q_DECL_OVERRIDE;

    void setData(QTextDocument *document, FmtTable *table, bool firstRowAsHeader, const QMap<int, QString> &fieldMapping);

signals:
    void finished(const QString &plsqlBlock);

private:
    QString generatePlsqlBlock();
    QTextTable* findTableInDocument();
    QString generateFunctionDeclaration(const QString &functionName);
    QString generateInsertStatement(const QStringList &fieldNames);
    QString generateFunctionCall(int row, QTextTable *table, const QString &functionName);
    QString getCellValue(QTextTable *table, int row, int col);
    QString getRawCellValue(QTextTable *table, int row, int col);

    // Вспомогательные методы для форматирования значений
    FmtField* findFieldByName(const QString &fieldName);
    QString formatValueForField(const QString &value, FmtField *field);
    QString formatValueForFieldFallback(const QString &value, FmtField *field);
    QDate parseDate(const QString &dateString);
    QTime parseTime(const QString &timeString);
    QDateTime parseDateTime(const QString &dateTimeString);

    // Методы для генерации nextval
    QString generateNextvalVariables();
    QString generateNextvalInitialization();

    // Метод для генерации обработчика исключений
    QString generateExceptionHandler();

private:
    QTextDocument *m_pDocument = nullptr;
    FmtTable *m_pTable = nullptr;
    bool m_bFirstRowAsHeader = false;
    QMap<int, QString> m_FieldMapping;
};

#endif // RICHTEXTTOINSERTRUN_H
