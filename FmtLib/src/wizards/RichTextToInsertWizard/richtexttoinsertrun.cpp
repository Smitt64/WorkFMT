#include "richtexttoinsertrun.h"
#include <QTextDocument>
#include <QTextTable>
#include <QTextFrame>
#include "fmttable.h"
#include "fmtfield.h"

RichTextToInsertRun::RichTextToInsertRun(QObject *parent)
    : QObject{parent},
    QRunnable()
{

}

RichTextToInsertRun::~RichTextToInsertRun()
{

}

void RichTextToInsertRun::run()
{
    if (!m_pDocument || !m_pTable)
        return;

    QString plsqlBlock = generatePlsqlBlock();
    emit finished(plsqlBlock);
}

void RichTextToInsertRun::setData(QTextDocument *document, FmtTable *table, bool firstRowAsHeader, const QMap<int, QString> &fieldMapping)
{
    m_pDocument = document;
    m_pTable = table;
    m_bFirstRowAsHeader = firstRowAsHeader;
    m_FieldMapping = fieldMapping;
}

QString RichTextToInsertRun::generatePlsqlBlock()
{
    // Находим таблицу в документе
    QTextTable *table = findTableInDocument();
    if (!table)
        return QString();

    int startRow = m_bFirstRowAsHeader ? 1 : 0;
    int columnCount = table->columns();
    int rowCount = table->rows();

    if (rowCount <= startRow)
        return QString();

    QString functionName = QString("Insert%1").arg(m_pTable->name());

    QString plsql;

    // Генерируем заголовок функции
    plsql += generateFunctionHeader(functionName, columnCount);
    plsql += "IS\n";
    plsql += "BEGIN\n";

    // Генерируем операторы INSERT для каждой строки
    for (int row = startRow; row < rowCount; ++row)
    {
        plsql += generateInsertStatement(row, table);
    }

    plsql += "END " + functionName + ";\n";
    plsql += "/\n\n";

    // Генерируем вызовы функции для каждой строки
    //plsql += generateFunctionCalls(functionName, startRow, rowCount, columnCount);

    return plsql;
}

QTextTable* RichTextToInsertRun::findTableInDocument()
{
    QTextFrame::iterator it;
    for (it = m_pDocument->rootFrame()->begin(); it != m_pDocument->rootFrame()->end(); ++it)
    {
        QTextFrame *childFrame = it.currentFrame();
        if (childFrame)
        {
            QTextTable *table = qobject_cast<QTextTable*>(childFrame);
            if (table)
                return table;
        }
    }
    return nullptr;
}

QString RichTextToInsertRun::generateFunctionHeader(const QString &functionName, int columnCount)
{
    QString header = QString("CREATE OR REPLACE FUNCTION %1(\n").arg(functionName);

    for (int i = 0; i < columnCount; ++i)
    {
        QString paramName = QString("p_column_%1").arg(i + 1);
        QString paramType = "VARCHAR2"; // или можно определить по типу поля

        header += QString("    %1 IN %2").arg(paramName).arg(paramType);

        if (i < columnCount - 1)
            header += ",\n";
        else
            header += "\n";
    }

    header += ") RETURN NUMBER\n";
    return header;
}

QString RichTextToInsertRun::generateInsertStatement(int row, QTextTable *table)
{
    QString insert = "    INSERT INTO " + m_pTable->name() + " (";

    QStringList fields;
    QStringList values;

    // Группируем маппинг по типам
    QMap<int, QString> columnMappings;    // колонки Word
    QMap<int, QString> otherMappings;     // другие источники

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        if (it.key() >= 0)
            columnMappings[it.key()] = it.value();
        else
            otherMappings[it.key()] = it.value();
    }

    // Обрабатываем колонки Word
    for (int col = 0; col < table->columns(); ++col)
    {
        if (columnMappings.contains(col))
        {
            QString fieldName = columnMappings[col];
            QString cellValue = getCellValue(table, row, col);

            fields << fieldName;
            values << cellValue;
        }
    }

    // Обрабатываем другие источники (SQL, функции, значения)
    for (auto it = otherMappings.begin(); it != otherMappings.end(); ++it)
    {
        QStringList parts = it.value().split("|");
        if (parts.size() >= 2)
        {
            QString fieldName = parts[1];
            QString value = parts.size() > 2 ? parts[2] : "NULL";

            fields << fieldName;

            if (parts[0] == "SQL" || parts[0] == "FUNC")
                values << value; // Уже готовое выражение
            else
                values << "'" + value.replace("'", "''") + "'";
        }
    }

    insert += fields.join(", ") + ")\n";
    insert += "    VALUES (" + values.join(", ") + ");\n";

    return insert;
}

QString RichTextToInsertRun::getCellValue(QTextTable *table, int row, int col)
{
    QTextTableCell cell = table->cellAt(row, col);
    if (!cell.isValid())
        return "NULL";

    QTextCursor cursor(m_pDocument);
    cursor.setPosition(cell.firstCursorPosition().position());
    cursor.setPosition(cell.lastCursorPosition().position(), QTextCursor::KeepAnchor);

    QString text = cursor.selectedText().trimmed();

    // Если значение пустое, возвращаем NULL
    if (text.isEmpty())
        return "NULL";

    // Экранируем строки для SQL
    return "'" + text.replace("'", "''") + "'";
}

QString RichTextToInsertRun::generateFunctionCalls(const QString &functionName, int startRow, int rowCount, int columnCount)
{
    QString calls = "-- Вызовы функции для каждой строки:\n";
    calls += "DECLARE\n";
    calls += "    result NUMBER;\n";
    calls += "BEGIN\n";

    for (int row = startRow; row < rowCount; ++row)
    {
        calls += QString("    result := %1(").arg(functionName);

        for (int col = 0; col < columnCount; ++col)
        {
            // В реальной реализации здесь нужно получить значения из таблицы
            calls += QString("'значение_row%1_col%2'").arg(row + 1).arg(col + 1);

            if (col < columnCount - 1)
                calls += ", ";
        }

        calls += ");\n";
    }

    calls += "    COMMIT;\n";
    calls += "END;\n";
    calls += "/\n";

    return calls;
}
