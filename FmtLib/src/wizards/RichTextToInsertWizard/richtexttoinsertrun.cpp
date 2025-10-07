#include "richtexttoinsertrun.h"
#include <QTextDocument>
#include <QTextTable>
#include <QTextFrame>
#include <QTextCursor>
#include <QSqlDriver>
#include <QSqlField>
#include <algorithm>
#include <QSet>
#include "fmttable.h"
#include "fmtfield.h"
#include "fmtcore.h"
#include "connectioninfo.h"

RichTextToInsertRun::RichTextToInsertRun(QObject *parent)
    : QObject{parent}, QRunnable()
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

void RichTextToInsertRun::setData(QTextDocument *document, FmtTable *table, bool firstRowAsHeader, const QMap<QString, QString> &fieldMapping)
{
    m_pDocument = document;
    m_pTable = table;
    m_bFirstRowAsHeader = firstRowAsHeader;
    m_FieldMapping = fieldMapping;
}

QString RichTextToInsertRun::generatePlsqlBlock()
{
    QTextTable *table = findTableInDocument();
    if (!table)
        return QString();

    int startRow = m_bFirstRowAsHeader ? 1 : 0;
    int rowCount = table->rows();

    if (rowCount <= startRow)
        return QString();

    QString functionName = QString("Insert%1Row").arg(m_pTable->name());

    QString plsql;

    plsql += "DECLARE\n\n";
    plsql += generateFunctionDeclaration(functionName);
    plsql += "\n";
    plsql += "BEGIN\n";

    for (int row = startRow; row < rowCount; ++row)
    {
        plsql += generateFunctionCall(row, table, functionName);
    }

    plsql += "END;\n";
    plsql += "/\n\n";

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

QString RichTextToInsertRun::generateFunctionDeclaration(const QString &functionName)
{
    QString declaration;

    QMap<int, QString> columnParameters;

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        QString source = it.value();

        if (source.startsWith("COLUMN|"))
        {
            QString columnStr = source.mid(7);
            bool ok;
            int columnIndex = columnStr.toInt(&ok);

            if (ok && columnIndex >= 0)
            {
                if (!columnParameters.contains(columnIndex))
                {
                    QString paramName = "p_col" + QString::number(columnIndex + 1);
                    columnParameters[columnIndex] = paramName;
                }
            }
        }
    }

    if (columnParameters.isEmpty())
    {
        declaration += "    PROCEDURE " + functionName + "\n";
    }
    else
    {
        declaration += QString("    PROCEDURE %1(\n").arg(functionName);

        int paramCount = 0;
        QList<int> columns = columnParameters.keys();
        std::sort(columns.begin(), columns.end());

        for (int col : columns)
        {
            QString paramName = columnParameters[col];
            QString paramType = "VARCHAR2";

            declaration += QString("        %1 IN %2").arg(paramName).arg(paramType);

            if (paramCount < columns.size() - 1)
                declaration += ",\n";
            else
                declaration += "\n";

            paramCount++;
        }

        declaration += "    )\n";
    }

    declaration += "    IS\n";

    QString nextvalVars = generateNextvalVariables();
    declaration += nextvalVars;

    declaration += "        v_row_num NUMBER;\n";
    declaration += "        v_error_msg VARCHAR2(4000);\n";

    declaration += "    BEGIN\n";

    QString nextvalInit = generateNextvalInitialization();
    declaration += nextvalInit;

    declaration += generateInsertStatement(columnParameters);

    declaration += generateExceptionHandler(columnParameters);

    declaration += "    END " + functionName + ";\n";

    return declaration;
}

QString RichTextToInsertRun::generateNextvalVariables()
{
    QString variables;

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        QString source = it.value();

        if ((source.startsWith("FUNC|") || source.startsWith("VAL|")) &&
            source.contains("nextval", Qt::CaseInsensitive))
        {
            variables += QString("        %1_seq NUMBER;\n").arg(it.key().toLower());
        }
    }

    return variables;
}

QString RichTextToInsertRun::generateNextvalInitialization()
{
    QString init;

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        QString fieldName = it.key();
        QString source = it.value();

        if ((source.startsWith("FUNC|") || source.startsWith("VAL|")) &&
            source.contains("nextval", Qt::CaseInsensitive))
        {
            init += QString("        SELECT NVL(MAX(%1), 0) + 1 INTO %2_seq FROM %3;\n")
            .arg(fieldName)
                .arg(fieldName.toLower())
                .arg(m_pTable->name());
        }
    }

    return init;
}

QString RichTextToInsertRun::generateInsertStatement(const QMap<int, QString> &columnParameters)
{
    QString insert = "        INSERT INTO " + m_pTable->name() + " (";

    QStringList fields;
    QStringList values;

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        QString fieldName = it.key();
        QString source = it.value();
        FmtField *field = findFieldByName(fieldName);

        if (!field)
            continue;

        fields << fieldName;

        if (source.startsWith("COLUMN|"))
        {
            QString columnStr = source.mid(7);
            bool ok;
            int columnIndex = columnStr.toInt(&ok);

            if (ok && columnIndex >= 0 && columnParameters.contains(columnIndex))
            {
                QString paramName = columnParameters[columnIndex];
                values << paramName;
            }
            else
            {
                values << "NULL";
            }
        }
        else if (source.startsWith("SQL|"))
        {
            QString sqlValue = source.mid(4);
            values << sqlValue;
        }
        else if (source.startsWith("FUNC|"))
        {
            QString funcValue = source.mid(5);
            if (funcValue.compare("nextval", Qt::CaseInsensitive) == 0)
            {
                values << QString("%1_seq").arg(fieldName.toLower());
            }
            else
            {
                values << funcValue;
            }
        }
        else if (source.startsWith("VAL|"))
        {
            QString valValue = source.mid(4);
            if (valValue.compare("nextval", Qt::CaseInsensitive) == 0)
            {
                values << QString("%1_seq").arg(fieldName.toLower());
            }
            else
            {
                QString formattedValue = formatValueForField(valValue, field);
                values << formattedValue;
            }
        }
        else
        {
            QString formattedValue = formatValueForField(source, field);
            values << formattedValue;
        }
    }

    insert += fields.join(", ") + ")\n";
    insert += "        VALUES (" + values.join(", ") + ");\n\n";

    return insert;
}

QString RichTextToInsertRun::generateExceptionHandler(const QMap<int, QString> &columnParameters)
{
    QString exceptionHandler;

    exceptionHandler += "    EXCEPTION\n";
    exceptionHandler += "        WHEN DUP_VAL_ON_INDEX THEN\n";
    exceptionHandler += "            v_error_msg := 'Ошибка: нарушение уникальности индекса';\n";
    exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Сообщение: ' || SQLERRM;\n";
    exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Трассировка: ' || DBMS_UTILITY.FORMAT_ERROR_BACKTRACE;\n";
    exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Параметры записи: ';\n";

    int paramCount = 0;
    QList<int> columns = columnParameters.keys();
    std::sort(columns.begin(), columns.end());

    for (int col : columns)
    {
        QString paramName = columnParameters[col];
        if (paramCount == 0) {
            exceptionHandler += QString("            v_error_msg := v_error_msg || '%1=' || %2;\n")
            .arg(paramName).arg(paramName);
        } else {
            exceptionHandler += QString("            v_error_msg := v_error_msg || ', %1=' || %2;\n")
            .arg(paramName).arg(paramName);
        }
        paramCount++;
    }

    exceptionHandler += "            DBMS_OUTPUT.PUT_LINE(v_error_msg);\n";
    exceptionHandler += "            -- RAISE; -- раскомментировать, если нужно прервать выполнение\n";
    exceptionHandler += "            -- NULL; -- оставить, если нужно продолжить выполнение после ошибки\n";

    return exceptionHandler;
}

QString RichTextToInsertRun::generateFunctionCall(int row, QTextTable *table, const QString &functionName)
{
    QString call;

    QStringList parameters;
    QMap<int, QString> columnParameters;

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        QString source = it.value();
        if (source.startsWith("COLUMN|"))
        {
            QString columnStr = source.mid(7);
            bool ok;
            int columnIndex = columnStr.toInt(&ok);
            if (ok && columnIndex >= 0)
            {
                QString paramName = "p_col" + QString::number(columnIndex + 1);
                if (!columnParameters.contains(columnIndex))
                {
                    columnParameters[columnIndex] = paramName;
                }
            }
        }
    }

    QList<int> columns = columnParameters.keys();
    std::sort(columns.begin(), columns.end());

    for (int col : columns)
    {
        QString cellValue = getCellValue(table, row, col);
        parameters << cellValue;
    }

    if (parameters.isEmpty())
    {
        call = QString("    %1;\n").arg(functionName);
    }
    else
    {
        call = QString("    %1(%2);\n").arg(functionName).arg(parameters.join(", "));
    }

    return call;
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

    if (text.isEmpty())
        return "NULL";

    QString escapedValue = text;
    escapedValue.replace("'", "''");
    return "'" + escapedValue + "'";
}

QString RichTextToInsertRun::getRawCellValue(QTextTable *table, int row, int col)
{
    QTextTableCell cell = table->cellAt(row, col);
    if (!cell.isValid())
        return QString();

    QTextCursor cursor(m_pDocument);
    cursor.setPosition(cell.firstCursorPosition().position());
    cursor.setPosition(cell.lastCursorPosition().position(), QTextCursor::KeepAnchor);

    return cursor.selectedText().trimmed();
}

FmtField* RichTextToInsertRun::findFieldByName(const QString &fieldName)
{
    if (!m_pTable)
        return nullptr;

    const QList<FmtField*> &fields = m_pTable->getFieldsList();

    for (FmtField *field : fields)
    {
        if (field->name().compare(fieldName, Qt::CaseInsensitive) == 0)
        {
            return field;
        }
    }

    return nullptr;
}

QString RichTextToInsertRun::formatValueForField(const QString &value, FmtField *field)
{
    if (value.compare("sql:NULL", Qt::CaseInsensitive) == 0)
        return "NULL";

    if (value.compare("nextval", Qt::CaseInsensitive) == 0)
    {
        return QString("%1_seq").arg(field->name().toLower());
    }

    if (value.startsWith("TO_DATE", Qt::CaseInsensitive) ||
        value.startsWith("CHR", Qt::CaseInsensitive) ||
        value.startsWith("SYSDATE", Qt::CaseInsensitive) ||
        value.startsWith("NULL", Qt::CaseInsensitive) ||
        value.contains("SELECT", Qt::CaseInsensitive) ||
        value.contains("SEQ_", Qt::CaseInsensitive))
    {
        return value;
    }

    if (m_pTable && m_pTable->connection())
    {
        QSqlDriver *driver = m_pTable->connection()->driver();
        if (driver)
        {
            QSqlField sqlField;

            switch(field->type())
            {
            case fmtt_INT:
            case fmtt_LONG:
            case fmtt_BIGINT:
            {
                sqlField.setType(QVariant::Int);
                bool ok;
                int intValue = value.toInt(&ok);
                if (ok)
                    sqlField.setValue(intValue);
                else
                    sqlField.setValue(value);
            }
            break;

            case fmtt_FLOAT:
            case fmtt_DOUBLE:
            {
                sqlField.setType(QVariant::Double);
                bool ok;
                double doubleValue = value.toDouble(&ok);
                if (ok)
                    sqlField.setValue(doubleValue);
                else
                    sqlField.setValue(value);
            }
            break;

            case fmtt_MONEY:
            case fmtt_NUMERIC:
                sqlField.setType(QVariant::String);
                sqlField.setValue(value);
                break;

            case fmtt_DATE:
            {
                sqlField.setType(QVariant::Date);
                QDate date = parseDate(value);
                if (date.isValid())
                    sqlField.setValue(date);
                else
                    sqlField.setValue(value);
            }
            break;

            case fmtt_TIME:
            {
                sqlField.setType(QVariant::Time);
                QTime time = parseTime(value);
                if (time.isValid())
                    sqlField.setValue(time);
                else
                    sqlField.setValue(value);
            }
            break;

            case fmtt_DATETIME:
            {
                sqlField.setType(QVariant::DateTime);
                QDateTime datetime = parseDateTime(value);
                if (datetime.isValid())
                    sqlField.setValue(datetime);
                else
                    sqlField.setValue(value);
            }
            break;

            case fmtt_STRING:
            case fmtt_SNR:
            case fmtt_CHR:
            case fmtt_UCHR:
            default:
                sqlField.setType(QVariant::String);
                sqlField.setValue(value);
                break;
            }

            QString formattedValue = driver->formatValue(sqlField);

            if (m_pTable->connection()->type() == ConnectionInfo::CON_ORA)
            {
                if (field->isNumber() && formattedValue.startsWith('\'') && formattedValue.endsWith('\''))
                {
                    formattedValue = formattedValue.mid(1, formattedValue.length() - 2);
                }
            }

            return formattedValue;
        }
    }

    return formatValueForFieldFallback(value, field);
}

QString RichTextToInsertRun::formatValueForFieldFallback(const QString &value, FmtField *field)
{
    if (field->isNumber())
    {
        return value;
    }
    else if (field->type() == fmtt_DATE || field->type() == fmtt_DATETIME)
    {
        if (!value.startsWith("TO_DATE", Qt::CaseInsensitive))
        {
            QString escapedValue = value;
            escapedValue.replace("'", "''");
            return QString("TO_DATE('%1', 'DD.MM.YYYY')").arg(escapedValue);
        }
        return value;
    }
    else if (field->type() == fmtt_TIME)
    {
        if (!value.startsWith("TO_DATE", Qt::CaseInsensitive))
        {
            QString escapedValue = value;
            escapedValue.replace("'", "''");
            return QString("TO_DATE('%1', 'HH24:MI:SS')").arg(escapedValue);
        }
        return value;
    }
    else
    {
        QString escapedValue = value;
        escapedValue.replace("'", "''");
        return "'" + escapedValue + "'";
    }
}

QDate RichTextToInsertRun::parseDate(const QString &dateString)
{
    QList<QString> formats = {
        "dd.MM.yyyy",
        "dd/MM/yyyy",
        "dd-MM-yyyy",
        "yyyy-MM-dd",
        "MM/dd/yyyy",
        "d.M.yyyy",
        "d/M/yyyy",
        "d-M-yyyy",
        "yyyy-M-d"
    };

    for (const QString &format : formats)
    {
        QDate date = QDate::fromString(dateString, format);
        if (date.isValid())
            return date;
    }

    return QDate();
}

QTime RichTextToInsertRun::parseTime(const QString &timeString)
{
    QList<QString> formats = {
        "hh:mm:ss",
        "hh:mm",
        "h:mm:ss",
        "h:mm"
    };

    for (const QString &format : formats)
    {
        QTime time = QTime::fromString(timeString, format);
        if (time.isValid())
            return time;
    }

    return QTime();
}

QDateTime RichTextToInsertRun::parseDateTime(const QString &dateTimeString)
{
    QList<QString> formatsWithTime = {
        "dd.MM.yyyy hh:mm:ss",
        "dd/MM/yyyy hh:mm:ss",
        "dd-MM-yyyy hh:mm:ss",
        "yyyy-MM-dd hh:mm:ss",
        "MM/dd/yyyy hh:mm:ss",
        "dd.MM.yyyy hh:mm",
        "dd/MM/yyyy hh:mm",
        "dd-MM-yyyy hh:mm",
        "yyyy-MM-dd hh:mm",
        "MM/dd/yyyy hh:mm"
    };

    for (const QString &format : formatsWithTime)
    {
        QDateTime datetime = QDateTime::fromString(dateTimeString, format);
        if (datetime.isValid())
            return datetime;
    }

    QDate date = parseDate(dateTimeString);
    if (date.isValid())
    {
        return QDateTime(date, QTime(0, 0, 0));
    }

    return QDateTime();
}
