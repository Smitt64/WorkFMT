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

    qDebug() << "=== DEBUG: setData called ===";
    qDebug() << "First row as header:" << m_bFirstRowAsHeader;
    qDebug() << "Field mapping contents:";
    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        qDebug() << "  Key:" << it.key() << "Value:" << it.value();
    }

    if (m_pTable) {
        qDebug() << "Table fields:";
        const QList<FmtField*> &fields = m_pTable->getFieldsList();
        for (FmtField *field : fields)
        {
            qDebug() << "  -" << field->name() << "(undecorated:" << field->undecorateName() << ")";
        }
    }
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

    QMap<QString, QString> parameters;
    QList<int> columnMappings;

    qDebug() << "=== DEBUG: Generating function declaration ===";
    qDebug() << "Field mapping contents:";
    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        qDebug() << "  Key:" << it.key() << "Value:" << it.value();
        if (it.key() >= 0)
        {
            parameters[it.value()] = "VARCHAR2";
            columnMappings << it.key();
        }
    }

    qDebug() << "Parameters for function:" << parameters.keys();
    qDebug() << "Column mappings:" << columnMappings;

    if (parameters.isEmpty())
        return declaration;

    declaration += QString("    PROCEDURE %1(\n").arg(functionName);

    int paramCount = 0;
    for (auto it = parameters.begin(); it != parameters.end(); ++it)
    {
        QString paramName = it.key().toLower();
        QString paramType = it.value();

        declaration += QString("        %1 IN %2").arg(paramName).arg(paramType);

        if (paramCount < parameters.size() - 1)
            declaration += ",\n";
        else
            declaration += "\n";

        paramCount++;
    }

    declaration += "    )\n";
    declaration += "    IS\n";

    QString nextvalVars = generateNextvalVariables();
    declaration += nextvalVars;

    declaration += "        v_row_num NUMBER;\n";
    declaration += "        v_error_msg VARCHAR2(4000);\n";

    declaration += "    BEGIN\n";

    QString nextvalInit = generateNextvalInitialization();
    declaration += nextvalInit;

    declaration += generateInsertStatement(parameters.keys());

    declaration += generateExceptionHandler();

    declaration += "    END " + functionName + ";\n";

    return declaration;
}

QString RichTextToInsertRun::generateNextvalVariables()
{
    QString variables;

    QSet<QString> nextvalFields;

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        if (it.key() < 0)
        {
            QStringList parts = it.value().split("|");

            if (parts.size() >= 2)
            {
                QString fieldName = parts[1];
                QString value = parts.size() > 2 ? parts[2] : "NULL";

                if (value.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    nextvalFields.insert(fieldName);
                }
            }
        }
    }

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        if (it.key() >= 0)
        {
            QString fieldName = it.value();

            QTextTable *table = findTableInDocument();
            if (table)
            {
                int startRow = m_bFirstRowAsHeader ? 1 : 0;
                for (int row = startRow; row < table->rows(); ++row)
                {
                    QString cellValue = getRawCellValue(table, row, it.key());

                    if (cellValue.compare("nextval", Qt::CaseInsensitive) == 0)
                    {
                        nextvalFields.insert(fieldName);
                        break;
                    }
                }
            }
        }
    }

    QTextTable *table = findTableInDocument();
    if (table)
    {
        int startRow = m_bFirstRowAsHeader ? 1 : 0;
        for (int row = startRow; row < table->rows(); ++row)
        {
            for (int col = 0; col < table->columns(); ++col)
            {
                QString rawValue = getRawCellValue(table, row, col);
                if (rawValue.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    QString fieldName = m_FieldMapping.value(col);
                    if (!fieldName.isEmpty())
                    {
                        nextvalFields.insert(fieldName);
                    }
                }
            }
        }
    }

    for (const QString &fieldName : nextvalFields)
    {
        if (!fieldName.isEmpty())
        {
            variables += QString("        %1_seq NUMBER;\n").arg(fieldName.toLower());
        }
    }

    return variables;
}

QString RichTextToInsertRun::generateNextvalInitialization()
{
    QString init;

    QSet<QString> nextvalFields;

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        if (it.key() < 0)
        {
            QStringList parts = it.value().split("|");
            if (parts.size() >= 2)
            {
                QString fieldName = parts[1];
                QString value = parts.size() > 2 ? parts[2] : "NULL";

                if (value.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    nextvalFields.insert(fieldName);
                }
            }
        }
    }

    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        if (it.key() >= 0)
        {
            QString fieldName = it.value();
            QTextTable *table = findTableInDocument();
            if (table)
            {
                int startRow = m_bFirstRowAsHeader ? 1 : 0;
                for (int row = startRow; row < table->rows(); ++row)
                {
                    QString cellValue = getRawCellValue(table, row, it.key());
                    if (cellValue.compare("nextval", Qt::CaseInsensitive) == 0)
                    {
                        nextvalFields.insert(fieldName);
                        break;
                    }
                }
            }
        }
    }

    QTextTable *table = findTableInDocument();
    if (table)
    {
        int startRow = m_bFirstRowAsHeader ? 1 : 0;
        for (int row = startRow; row < table->rows(); ++row)
        {
            for (int col = 0; col < table->columns(); ++col)
            {
                QString rawValue = getRawCellValue(table, row, col);
                if (rawValue.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    QString fieldName = m_FieldMapping.value(col);
                    if (!fieldName.isEmpty())
                    {
                        nextvalFields.insert(fieldName);
                    }
                }
            }
        }
    }

    for (const QString &fieldName : nextvalFields)
    {
        if (!fieldName.isEmpty())
        {
            init += QString("        SELECT NVL(MAX(%1), 0) + 1 INTO %2_seq FROM %3;\n")
            .arg(fieldName)
                .arg(fieldName.toLower())
                .arg(m_pTable->name());
        }
    }

    return init;
}

QString RichTextToInsertRun::generateInsertStatement(const QStringList &fieldNames)
{
    QString insert = "        INSERT INTO " + m_pTable->name() + " (";

    QStringList fields;
    QStringList values;

    qDebug() << "=== DEBUG: Generating INSERT statement ===";
    qDebug() << "Field names from parameters:" << fieldNames;

    // Поля из параметров функции
    for (const QString &fieldName : fieldNames)
    {
        FmtField *field = findFieldByName(fieldName);
        QString actualFieldName = field ? field->name() : fieldName;

        qDebug() << "Parameter field:" << fieldName << "-> Actual field:" << actualFieldName << "Field found:" << (field != nullptr);

        fields << actualFieldName;
        values << fieldName.toLower();
    }

    qDebug() << "=== DEBUG: Processing negative mappings ===";

    // Дополнительные поля из маппинга (с отрицательными ключами)
    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        if (it.key() < 0)
        {
            QStringList parts = it.value().split("|");
            qDebug() << "Negative mapping key:" << it.key() << "value:" << it.value() << "parts:" << parts;

            if (parts.size() >= 2)
            {
                QString fieldName = parts[1];
                QString value = parts.size() > 2 ? parts[2] : "NULL";

                FmtField *field = findFieldByName(fieldName);
                QString actualFieldName = field ? field->name() : fieldName;

                qDebug() << "  Field:" << fieldName << "-> Actual:" << actualFieldName << "Value:" << value << "Field found:" << (field != nullptr);

                fields << actualFieldName;

                if (field)
                {
                    if (value.compare("sql:NULL", Qt::CaseInsensitive) == 0)
                    {
                        values << "NULL";
                        qDebug() << "    -> NULL value";
                    }
                    else if (value.compare("nextval", Qt::CaseInsensitive) == 0)
                    {
                        values << QString("%1_seq").arg(fieldName.toLower());
                        qDebug() << "    -> nextval variable:" << QString("%1_seq").arg(fieldName.toLower());
                    }
                    else if (parts[0] == "SQL" || parts[0] == "FUNC")
                    {
                        values << value;
                        qDebug() << "    -> SQL/FUNC value:" << value;
                    }
                    else
                    {
                        QString formattedValue = formatValueForField(value, field);
                        values << formattedValue;
                        qDebug() << "    -> Formatted value:" << formattedValue;
                    }
                }
                else
                {
                    qDebug() << "    -> Field not found, using basic formatting";
                    if (value.compare("sql:NULL", Qt::CaseInsensitive) == 0)
                    {
                        values << "NULL";
                    }
                    else if (value.compare("nextval", Qt::CaseInsensitive) == 0)
                    {
                        values << QString("%1_seq").arg(fieldName.toLower());
                    }
                    else if (parts[0] == "SQL" || parts[0] == "FUNC")
                    {
                        values << value;
                    }
                    else
                    {
                        QString escapedValue = value;
                        escapedValue.replace("'", "''");
                        values << "'" + escapedValue + "'";
                    }
                }
            }
        }
    }

    qDebug() << "=== DEBUG: Final fields and values ===";
    qDebug() << "Fields:" << fields;
    qDebug() << "Values:" << values;

    insert += fields.join(", ") + ")\n";
    insert += "        VALUES (" + values.join(", ") + ");\n\n";

    return insert;
}

QString RichTextToInsertRun::generateExceptionHandler()
{
    QString exceptionHandler;

    exceptionHandler += "    EXCEPTION\n";
    exceptionHandler += "        WHEN DUP_VAL_ON_INDEX THEN\n";
    exceptionHandler += "            v_error_msg := 'Ошибка: нарушение уникальности индекса';\n";
    exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Сообщение: ' || SQLERRM;\n";
    exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Трассировка: ' || DBMS_UTILITY.FORMAT_ERROR_BACKTRACE;\n";
    exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Параметры записи: ';\n";

    QMap<QString, QString> parameters;
    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        if (it.key() >= 0)
        {
            parameters[it.value()] = "VARCHAR2";
        }
    }

    int paramCount = 0;
    for (auto it = parameters.begin(); it != parameters.end(); ++it)
    {
        QString paramName = it.key().toLower();
        if (paramCount == 0) {
            exceptionHandler += QString("            v_error_msg := v_error_msg || '%1=' || %2;\n")
            .arg(paramName).arg(paramName);
        } else {
            exceptionHandler += QString("            v_error_msg := v_error_msg || ', %1=' || %2;\n")
            .arg(paramName).arg(paramName);
        }
        paramCount++;
    }

    // Добавляем диагностику для определения проблемного индекса
    exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Диагностика:';\n";
    exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Проверьте уникальные индексы и ограничения:';\n";

    // Получаем список уникальных индексов таблицы
    if (m_pTable && m_pTable->hasUniqueIndexes()) {
        exceptionHandler += "            v_error_msg := v_error_msg || CHR(10) || 'Возможные проблемные индексы:';\n";
        // Здесь можно добавить логику для вывода конкретных индексов
        // но для этого нужен доступ к метаданным таблицы
    }

    exceptionHandler += "            DBMS_OUTPUT.PUT_LINE(v_error_msg);\n";
    exceptionHandler += "            -- RAISE; -- раскомментировать, если нужно прервать выполнение\n";
    exceptionHandler += "            -- NULL; -- оставить, если нужно продолжить выполнение после ошибки\n";

    return exceptionHandler;
}

QString RichTextToInsertRun::generateFunctionCall(int row, QTextTable *table, const QString &functionName)
{
    QString call = QString("    %1(").arg(functionName);

    QStringList parameters;

    QList<int> columnMappings;
    for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
    {
        if (it.key() >= 0)
        {
            columnMappings << it.key();
        }
    }

    std::sort(columnMappings.begin(), columnMappings.end());

    for (int col : columnMappings)
    {
        QString rawValue = getRawCellValue(table, row, col);
        QString fieldName = m_FieldMapping.value(col);
        FmtField *field = findFieldByName(fieldName);

        QString cellValue;
        if (rawValue.compare("nextval", Qt::CaseInsensitive) == 0 && field)
        {
            cellValue = QString("%1_seq").arg(fieldName.toLower());
        }
        else
        {
            cellValue = getCellValue(table, row, col);
        }

        parameters << cellValue;
    }

    call += parameters.join(", ") + ");\n";

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

    QString fieldName = m_FieldMapping.value(col);
    FmtField *field = findFieldByName(fieldName);

    if (field)
    {
        return formatValueForField(text, field);
    }

    QString escapedValue = text;
    escapedValue.replace("'", "''");
    return "'" + escapedValue + "'";
}

FmtField* RichTextToInsertRun::findFieldByName(const QString &fieldName)
{
    if (!m_pTable)
        return nullptr;

    const QList<FmtField*> &fields = m_pTable->getFieldsList();

    qDebug() << "=== DEBUG: Searching for field:" << fieldName;
    qDebug() << "Available fields in table:";
    for (FmtField *field : fields)
    {
        qDebug() << "  -" << field->name() << "(undecorated:" << field->undecorateName() << ")";
    }

    // Сначала ищем точное совпадение по имени
    for (FmtField *field : fields)
    {
        if (field->name().compare(fieldName, Qt::CaseInsensitive) == 0)
        {
            qDebug() << "  Found by exact name match";
            return field;
        }
    }

    // Затем ищем по undecorateName
    for (FmtField *field : fields)
    {
        if (field->undecorateName().compare(fieldName, Qt::CaseInsensitive) == 0)
        {
            qDebug() << "  Found by undecorateName match";
            return field;
        }
    }

    qDebug() << "  Field not found!";
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
