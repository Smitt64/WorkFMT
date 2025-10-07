#ifndef RICHTEXTTOINSERTRUN_H
#define RICHTEXTTOINSERTRUN_H

#include <QObject>
#include <QRunnable>
#include <QTextDocument>
#include <QMap>

class FmtTable;
class FmtField;
class QTextTable;

class RichTextToInsertRun : public QObject, public QRunnable
{
    Q_OBJECT

public:
    RichTextToInsertRun(QObject *parent = nullptr);
    ~RichTextToInsertRun();

    void run() override;

    void setData(QTextDocument *document, FmtTable *table, bool firstRowAsHeader, const QMap<QString, QString> &fieldMapping);

signals:
    void finished(const QString &plsqlCode);

private:
    // Основные методы генерации
    QString generatePlsqlBlock();
    QTextTable* findTableInDocument();

    // Генерация функции
    QString generateFunctionDeclaration(const QString &functionName);
    QString generateFunctionCall(int row, QTextTable *table, const QString &functionName);

    // Генерация INSERT
    QString generateInsertStatement(const QMap<int, QString> &columnParameters);
    QString generateNextvalVariables();
    QString generateNextvalInitialization();
    QString generateExceptionHandler(const QMap<int, QString> &columnParameters);

    // Вспомогательные методы
    QString getCellValue(QTextTable *table, int row, int col);
    QString getRawCellValue(QTextTable *table, int row, int col);
    FmtField* findFieldByName(const QString &fieldName);
    QString formatValueForField(const QString &value, FmtField *field);
    QString formatValueForFieldFallback(const QString &value, FmtField *field);

    // Парсинг дат/времени
    QDate parseDate(const QString &dateString);
    QTime parseTime(const QString &timeString);
    QDateTime parseDateTime(const QString &dateTimeString);

private:
    QTextDocument *m_pDocument = nullptr;
    FmtTable *m_pTable = nullptr;
    bool m_bFirstRowAsHeader = false;
    QMap<QString, QString> m_FieldMapping; // поле БД -> источник данных
};

#endif // RICHTEXTTOINSERTRUN_H
