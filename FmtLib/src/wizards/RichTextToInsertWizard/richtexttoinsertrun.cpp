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
#include "fmtindex.h"
#include "fmtcore.h"
#include "fmtsegment.h"
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

QString RichTextToInsertRun::padding(int level) const
{
    return QString(level * 2, ' ');
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
        declaration += padding(1) + "PROCEDURE " + functionName + "\n";
    }
    else
    {
        declaration += padding(1) + QString("PROCEDURE %1\n").arg(functionName);
        declaration += padding(1) + "(\n";

        int paramCount = 0;
        QList<int> columns = columnParameters.keys();
        std::sort(columns.begin(), columns.end());

        for (int col : columns)
        {
            QString paramName = columnParameters[col];
            QString paramType = "VARCHAR2";

            if (paramCount == columns.size() - 1)
                declaration += padding(2) + QString("%1 IN %2\n").arg(paramName).arg(paramType);
            else
                declaration += padding(2) + QString("%1 IN %2,\n").arg(paramName).arg(paramType);

            paramCount++;
        }

        declaration += padding(1) + ")\n";
    }

    declaration += padding(1) + "IS\n";

    QString nextvalVars = generateNextvalVariables();
    if (!nextvalVars.isEmpty())
    {
        declaration += nextvalVars;
    }

    declaration += padding(1) + "    v_row_num NUMBER;\n";
    declaration += padding(1) + "    v_error_msg VARCHAR2(4000);\n";

    declaration += padding(1) + "BEGIN\n";

    QString nextvalInit = generateNextvalInitialization();
    if (!nextvalInit.isEmpty())
    {
        declaration += nextvalInit;
    }

    // Добавляем проверку существования записи
    QString existsCondition = generateExistsCondition();
    if (!existsCondition.isEmpty())
    {
        declaration += existsCondition;
        declaration += padding(2) + "IF v_row_num > 0 THEN\n";
        declaration += padding(3) + "-- Запись существует, выполняем UPDATE\n";
        declaration += generateUpdateStatement(columnParameters);
        declaration += padding(2) + "ELSE\n";
        declaration += padding(3) + "-- Запись не существует, выполняем INSERT\n";
        declaration += generateInsertStatement(columnParameters);
        declaration += padding(2) + "END IF;\n\n";
    }
    else
    {
        // Без проверки существования - только INSERT
        declaration += generateInsertStatement(columnParameters);
    }

    declaration += generateExceptionHandler(columnParameters);
    declaration += padding(1) + "END " + functionName + ";\n";

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
            variables += padding(2) + QString("%1_seq NUMBER;\n").arg(it.key().toLower());
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
            init += padding(2) + QString("SELECT NVL(MAX(%1), 0) + 1 INTO %2_seq FROM %3;\n")
            .arg(fieldName)
                .arg(fieldName.toLower())
                .arg(m_pTable->name());
        }
    }

    return init;
}

QString RichTextToInsertRun::generateInsertStatement(const QMap<int, QString> &columnParameters)
{
    QString insert = padding(2) + "INSERT INTO " + m_pTable->name() + " (";

    // Получаем поля в порядке из FmtTable
    QStringList fields;
    if (m_pTable)
    {
        const QList<FmtField*> &tableFields = m_pTable->getFieldsList();
        for (FmtField *field : tableFields)
        {
            fields << field->name();
        }
    }

    insert += fields.join(", ") + ")\n";
    insert += padding(2) + "VALUES (\n";

    // Генерируем значения в том же порядке, что и поля
    QStringList values;
    for (int i = 0; i < fields.size(); ++i)
    {
        const QString &fieldName = fields[i];
        QString value;
        bool isLastField = (i == fields.size() - 1);

        if (!m_FieldMapping.contains(fieldName))
        {
            // Если поля нет в маппинге, используем NULL
            value = "NULL";
        }
        else
        {
            QString source = m_FieldMapping[fieldName];
            FmtField *field = findFieldByName(fieldName);

            if (!field)
            {
                value = "NULL";
            }
            else if (source.startsWith("COLUMN|"))
            {
                QString columnStr = source.mid(7);
                bool ok;
                int columnIndex = columnStr.toInt(&ok);

                if (ok && columnIndex >= 0 && columnParameters.contains(columnIndex))
                {
                    value = columnParameters[columnIndex];
                }
                else
                {
                    value = "NULL";
                }
            }
            else if (source.startsWith("SQL|"))
            {
                value = source.mid(4);
            }
            else if (source.startsWith("FUNC|"))
            {
                QString funcValue = source.mid(5);
                if (funcValue.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    value = QString("%1_seq").arg(fieldName.toLower());
                }
                else
                {
                    value = funcValue;
                }
            }
            else if (source.startsWith("VAL|"))
            {
                QString valValue = source.mid(4);
                if (valValue.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    value = QString("%1_seq").arg(fieldName.toLower());
                }
                else
                {
                    value = formatValueForField(valValue, field);
                }
            }
            else
            {
                value = formatValueForField(source, field);
            }
        }

        // Формируем строку: значение + запятая (если не последнее) + комментарий
        QString line = padding(3) + value;
        if (!isLastField)
        {
            line += ",";
        }
        line += " -- " + fieldName;

        values << line;
    }

    insert += values.join("\n") + "\n";
    insert += padding(2) + ");\n\n";

    return insert;
}

QString RichTextToInsertRun::generateExceptionHandler(const QMap<int, QString> &columnParameters)
{
    QString exceptionHandler;

    exceptionHandler += padding(1) + "EXCEPTION\n";
    exceptionHandler += padding(2) + "WHEN DUP_VAL_ON_INDEX THEN\n";
    exceptionHandler += padding(3) + "v_error_msg := 'Ошибка: нарушение уникальности индекса';\n";
    exceptionHandler += padding(3) + "v_error_msg := v_error_msg || CHR(10) || 'Сообщение: ' || SQLERRM;\n";
    exceptionHandler += padding(3) + "v_error_msg := v_error_msg || CHR(10) || 'Трассировка: ' || DBMS_UTILITY.FORMAT_ERROR_BACKTRACE;\n";
    exceptionHandler += padding(3) + "v_error_msg := v_error_msg || CHR(10) || 'Параметры записи: ';\n";

    int paramCount = 0;
    QList<int> columns = columnParameters.keys();
    std::sort(columns.begin(), columns.end());

    for (int col : columns)
    {
        QString paramName = columnParameters[col];
        if (paramCount == 0) {
            exceptionHandler += padding(3) + QString("v_error_msg := v_error_msg || '%1=' || %2;\n")
            .arg(paramName).arg(paramName);
        } else {
            exceptionHandler += padding(3) + QString("v_error_msg := v_error_msg || ', %1=' || %2;\n")
            .arg(paramName).arg(paramName);
        }
        paramCount++;
    }

    exceptionHandler += padding(3) + "DBMS_OUTPUT.PUT_LINE(v_error_msg);\n";
    exceptionHandler += padding(3) + "-- RAISE; -- раскомментировать, если нужно прервать выполнение\n";
    exceptionHandler += padding(3) + "-- NULL; -- оставить, если нужно продолжить выполнение после ошибки\n\n";

    // Добавляем обработку OTHERS
    exceptionHandler += padding(2) + "WHEN OTHERS THEN\n";
    exceptionHandler += padding(3) + "v_error_msg := 'Неизвестная ошибка при вставке записи';\n";
    exceptionHandler += padding(3) + "v_error_msg := v_error_msg || CHR(10) || 'Код ошибки: ' || SQLCODE;\n";
    exceptionHandler += padding(3) + "v_error_msg := v_error_msg || CHR(10) || 'Сообщение: ' || SQLERRM;\n";
    exceptionHandler += padding(3) + "v_error_msg := v_error_msg || CHR(10) || 'Трассировка: ' || DBMS_UTILITY.FORMAT_ERROR_BACKTRACE;\n";
    exceptionHandler += padding(3) + "v_error_msg := v_error_msg || CHR(10) || 'Параметры записи: ';\n";

    paramCount = 0;
    for (int col : columns)
    {
        QString paramName = columnParameters[col];
        if (paramCount == 0) {
            exceptionHandler += padding(3) + QString("v_error_msg := v_error_msg || '%1=' || %2;\n")
            .arg(paramName).arg(paramName);
        } else {
            exceptionHandler += padding(3) + QString("v_error_msg := v_error_msg || ', %1=' || %2;\n")
            .arg(paramName).arg(paramName);
        }
        paramCount++;
    }

    exceptionHandler += padding(3) + "DBMS_OUTPUT.PUT_LINE(v_error_msg);\n";
    exceptionHandler += padding(3) + "RAISE; -- повторно вызываем исключение для прерывания выполнения\n";

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
        call = padding(1) + QString("%1;\n").arg(functionName);
    }
    else
    {
        call = padding(1) + QString("%1(%2);\n").arg(functionName).arg(parameters.join(", "));
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

    text = replaceProblematicSymbols(text);

    // Список проблемных символов для замены на CHR()
    static const QVector<QPair<QChar, int>> specialChars = {
        {'"', 34},   // Двойная кавычка
        {'&', 38},   // Амперсанд (подстановка переменных)
        {'%', 37},   // Процент (может влиять на LIKE)
        {'_', 95},   // Подчеркивание (может влиять на LIKE)
        {'\'', 39},  // Одиночная кавычка (экранируется отдельно)
        {'<', 60},   // Меньше (может влиять на XML)
        {'>', 62},   // Больше (может влиять на XML)
        {'\\', 92}   // Обратный слеш (экранирование)
    };

    // Проверяем, есть ли проблемные символы
    bool hasSpecialChars = false;
    for (const auto &specialChar : specialChars) {
        if (text.contains(specialChar.first)) {
            hasSpecialChars = true;
            break;
        }
    }

    // Если нет специальных символов, просто экранируем одиночные кавычки
    if (!hasSpecialChars)
    {
        text.replace("'", "''");
        return "'" + text + "'";
    }

    // Обрабатываем текст со специальными символами
    QString result;
    QString currentSegment;

    for (int i = 0; i < text.length(); ++i)
    {
        QChar ch = text[i];
        bool isSpecialChar = false;
        int charCode = 0;

        // Проверяем, является ли символ специальным
        for (const auto &specialChar : specialChars) {
            if (ch == specialChar.first) {
                isSpecialChar = true;
                charCode = specialChar.second;
                break;
            }
        }

        if (isSpecialChar)
        {
            // Если накопился сегмент до специального символа - добавляем его
            if (!currentSegment.isEmpty())
            {
                currentSegment.replace("'", "''");
                if (result.isEmpty())
                    result = "'" + currentSegment + "'";
                else
                    result += " || '" + currentSegment + "'";
                currentSegment.clear();
            }

            // Добавляем CHR для специального символа
            QString chrCall = "CHR(" + QString::number(charCode) + ")";

            if (result.isEmpty())
                result = chrCall;
            else
                result += " || " + chrCall;
        }
        else
        {
            currentSegment += ch;
        }
    }

    // Добавляем оставшийся сегмент после последнего специального символа
    if (!currentSegment.isEmpty())
    {
        currentSegment.replace("'", "''");
        if (result.isEmpty())
            result = "'" + currentSegment + "'";
        else
            result += " || '" + currentSegment + "'";
    }

    return result;
}

QString RichTextToInsertRun::getRawCellValue(QTextTable *table, int row, int col)
{
    QTextTableCell cell = table->cellAt(row, col);
    if (!cell.isValid())
        return QString();

    QTextCursor cursor(m_pDocument);
    cursor.setPosition(cell.firstCursorPosition().position());
    cursor.setPosition(cell.lastCursorPosition().position(), QTextCursor::KeepAnchor);

    QString text = cursor.selectedText().trimmed();

    // ЗАМЕНЯЕМ ПРОБЛЕМНЫЕ СИМВОЛЫ
    return replaceProblematicSymbols(text);
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
    QString processedValue = replaceProblematicSymbols(value);

    if (processedValue.compare("sql:NULL", Qt::CaseInsensitive) == 0)
        return "NULL";

    if (processedValue.compare("nextval", Qt::CaseInsensitive) == 0)
    {
        return QString("%1_seq").arg(field->name().toLower());
    }

    if (processedValue.startsWith("TO_DATE", Qt::CaseInsensitive) ||
        processedValue.startsWith("CHR", Qt::CaseInsensitive) ||
        processedValue.startsWith("SYSDATE", Qt::CaseInsensitive) ||
        processedValue.startsWith("NULL", Qt::CaseInsensitive) ||
        processedValue.contains("SELECT", Qt::CaseInsensitive) ||
        processedValue.contains("SEQ_", Qt::CaseInsensitive))
    {
        return processedValue;
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

void RichTextToInsertRun::setExistsCondition(FmtIndex *index, const QString &customCondition, bool useCustomCondition)
{
    m_existsIndex = index;
    m_customExistsCondition = customCondition;
    m_useCustomCondition = useCustomCondition;
}

QString RichTextToInsertRun::generateExistsCondition()
{
    if (m_useCustomCondition && m_customExistsCondition.isEmpty())
        return QString();

    if (!m_useCustomCondition && !m_existsIndex)
        return QString();

    QString condition;

    if (m_useCustomCondition)
    {
        QString customCondition = m_customExistsCondition.trimmed();

        // Для условий EXISTS используем специальную замену
        QString processedCondition = replaceFieldReferencesInCondition(customCondition);

        // Определяем, является ли условие полным SQL выражением
        bool isFullSql = processedCondition.toUpper().contains("SELECT") ||
                         processedCondition.toUpper().contains("EXISTS");

        condition = padding(2) + "-- Проверка существования записи по пользовательскому условию\n";

        if (isFullSql)
        {
            condition += padding(2) + "SELECT COUNT(*) INTO v_row_num FROM " + m_pTable->name() + " t\n";
            condition += padding(2) + "WHERE EXISTS (" + processedCondition + ");\n\n";
        }
        else
        {
            condition += padding(2) + "SELECT COUNT(*) INTO v_row_num FROM " + m_pTable->name() + " \n";
            condition += padding(2) + "WHERE " + processedCondition + ";\n\n";
        }
    }
    else if (m_existsIndex)
    {
        condition = padding(2) + "-- Проверка существования записи по индексу: " + m_existsIndex->name() + "\n";
        condition += padding(2) + "SELECT COUNT(*) INTO v_row_num FROM " + m_pTable->name() + " \n";
        condition += padding(2) + "WHERE " + generateIndexCondition(m_existsIndex) + ";\n\n";
    }

    return condition;
}

QString RichTextToInsertRun::generateIndexCondition(FmtIndex *index)
{
    if (!index)
        return QString();

    QStringList conditions;

    // Получаем все сегменты индекса
    for (int i = 0; i < index->segmentsCount(); ++i)
    {
        FmtSegment *segment = index->segment(i);
        if (!segment || !segment->field())
            continue;

        FmtField *field = segment->field();
        QString fieldName = field->name();

        // Ищем источник данных для этого поля
        QString fieldValue;
        if (m_FieldMapping.contains(fieldName))
        {
            QString source = m_FieldMapping[fieldName];

            if (source.startsWith("COLUMN|"))
            {
                // Данные из колонки Word - используем параметр функции
                QString columnStr = source.mid(7);
                bool ok;
                int columnIndex = columnStr.toInt(&ok);

                if (ok && columnIndex >= 0)
                {
                    fieldValue = "p_col" + QString::number(columnIndex + 1);
                }
                else
                {
                    fieldValue = "NULL";
                }
            }
            else if (source.startsWith("SQL|"))
            {
                fieldValue = source.mid(4);
            }
            else if (source.startsWith("FUNC|"))
            {
                QString funcValue = source.mid(5);
                if (funcValue.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    fieldValue = QString("%1_seq").arg(fieldName.toLower());
                }
                else
                {
                    fieldValue = funcValue;
                }
            }
            else if (source.startsWith("VAL|"))
            {
                QString valValue = source.mid(4);
                if (valValue.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    fieldValue = QString("%1_seq").arg(fieldName.toLower());
                }
                else
                {
                    fieldValue = formatValueForField(valValue, field);
                }
            }
            else
            {
                fieldValue = formatValueForField(source, field);
            }
        }
        else
        {
            // Поля нет в маппинге - используем NULL
            fieldValue = "NULL";
        }

        // Формируем условие сравнения
        QString condition;
        if (fieldValue == "NULL")
        {
            condition = fieldName + " IS NULL";
        }
        else
        {
            condition = fieldName + " = " + fieldValue;
        }

        conditions << condition;
    }

    return conditions.join(" AND ");
}

QString RichTextToInsertRun::generateUpdateStatement(const QMap<int, QString> &columnParameters)
{
    QString update = padding(3) + "UPDATE " + m_pTable->name() + " \n";
    update += padding(3) + "SET \n";

    // Получаем поля в порядке из FmtTable
    QStringList fields;
    if (m_pTable)
    {
        const QList<FmtField*> &tableFields = m_pTable->getFieldsList();
        for (FmtField *field : tableFields)
        {
            fields << field->name();
        }
    }

    // Получаем поля, которые участвуют в условии WHERE (индексные поля)
    QSet<QString> indexFields;
    if (m_existsIndex)
    {
        for (int i = 0; i < m_existsIndex->segmentsCount(); ++i)
        {
            FmtSegment *segment = m_existsIndex->segment(i);
            if (segment && segment->field())
            {
                indexFields.insert(segment->field()->name());
            }
        }
    }

    // Генерируем SET часть для UPDATE (исключая индексные поля и _seq поля)
    QStringList setClauses;
    for (int i = 0; i < fields.size(); ++i)
    {
        const QString &fieldName = fields[i];

        // Пропускаем индексные поля
        if (indexFields.contains(fieldName))
        {
            continue;
        }

        // Пропускаем поля, которые заканчиваются на _seq (последовательности)
        if (fieldName.endsWith("_seq", Qt::CaseInsensitive))
        {
            continue;
        }

        if (!m_FieldMapping.contains(fieldName))
        {
            // Если поля нет в маппинге, пропускаем его в UPDATE
            continue;
        }

        QString source = m_FieldMapping[fieldName];
        FmtField *field = findFieldByName(fieldName);

        if (!field)
        {
            continue;
        }

        QString value;
        bool isDefaultValue = false;
        bool isSeqValue = false;

        if (source.startsWith("COLUMN|"))
        {
            QString columnStr = source.mid(7);
            bool ok;
            int columnIndex = columnStr.toInt(&ok);

            if (ok && columnIndex >= 0 && columnParameters.contains(columnIndex))
            {
                value = columnParameters[columnIndex];
            }
            else
            {
                value = "NULL";
                isDefaultValue = true;
            }
        }
        else if (source.startsWith("SQL|"))
        {
            value = source.mid(4);
        }
        else if (source.startsWith("FUNC|"))
        {
            QString funcValue = source.mid(5);
            if (funcValue.compare("nextval", Qt::CaseInsensitive) == 0)
            {
                // Пропускаем поля с nextval в UPDATE
                isSeqValue = true;
                value = QString("%1_seq").arg(fieldName.toLower());
            }
            else
            {
                value = funcValue;
            }
        }
        else if (source.startsWith("VAL|"))
        {
            QString valValue = source.mid(4);
            if (valValue.compare("nextval", Qt::CaseInsensitive) == 0)
            {
                // Пропускаем поля с nextval в UPDATE
                isSeqValue = true;
                value = QString("%1_seq").arg(fieldName.toLower());
            }
            else
            {
                value = formatValueForField(valValue, field);
                // Проверяем, является ли значение значением по умолчанию
                QString defaultValue = fmtGetOraDefaultVal(field->type(), field->size(), false);
                isDefaultValue = (value == defaultValue);
            }
        }
        else
        {
            value = formatValueForField(source, field);
            // Проверяем, является ли значение значением по умолчанию
            QString defaultValue = fmtGetOraDefaultVal(field->type(), field->size(), false);
            isDefaultValue = (value == defaultValue);
        }

        // Пропускаем значения по умолчанию и последовательности
        if (isDefaultValue || isSeqValue)
        {
            continue;
        }

        // Формируем SET clause
        bool isLastField = (i == fields.size() - 1);
        QString setClause = padding(4) + fieldName + " = " + value;
        if (!isLastField)
        {
            setClause += ",";
        }
        setClause += " -- " + fieldName;

        setClauses << setClause;
    }

    // Если нет полей для обновления, добавляем фиктивное поле чтобы не ломать синтаксис
    if (setClauses.isEmpty())
    {
        setClauses << padding(4) + "1 = 1 -- Нет полей для обновления";
    }

    update += setClauses.join("\n") + "\n";

    // Добавляем WHERE условие для UPDATE
    if (m_useCustomCondition && !m_customExistsCondition.isEmpty())
    {
        QString processedCondition = replaceFieldReferencesInCondition(m_customExistsCondition);
        update += padding(3) + "WHERE " + processedCondition + ";\n\n";
    }
    else if (m_existsIndex)
    {
        update += padding(3) + "WHERE " + generateIndexCondition(m_existsIndex) + ";\n\n";
    }
    else
    {
        update += padding(3) + "WHERE 1=0; -- Нет условия для UPDATE\n\n";
    }

    return update;
}

bool RichTextToInsertRun::isDefaultValue(const QString &value, FmtField *field)
{
    if (!field)
        return false;

    QString defaultValue = fmtGetOraDefaultVal(field->type(), field->size(), false);

    // Сравниваем значения
    if (value == defaultValue)
        return true;

    // Для дат сравниваем с экранированной версией
    if (field->type() == fmtt_DATE || field->type() == fmtt_TIME || field->type() == fmtt_DATETIME)
    {
        QString escapedDefault = fmtGetOraDefaultVal(field->type(), field->size(), true);
        if (value == escapedDefault)
            return true;
    }

    return false;
}

QString RichTextToInsertRun::replaceFieldReferences(const QString &value)
{
    // Этот метод используется только для INSERT/UPDATE, где все поля должны быть заменены
    QString result = value;

    if (!m_pTable)
        return result;

    const QList<FmtField*> &fields = m_pTable->getFieldsList();
    for (FmtField *field : fields)
    {
        QString fieldName = field->name();

        if (m_FieldMapping.contains(fieldName))
        {
            QString source = m_FieldMapping[fieldName];
            QString replacement;

            if (source.startsWith("COLUMN|"))
            {
                QString columnStr = source.mid(7);
                bool ok;
                int columnIndex = columnStr.toInt(&ok);

                if (ok && columnIndex >= 0)
                {
                    replacement = "p_col" + QString::number(columnIndex + 1);
                }
                else
                {
                    replacement = "NULL";
                }
            }
            else if (source.startsWith("SQL|"))
            {
                replacement = source.mid(4);
            }
            else if (source.startsWith("FUNC|"))
            {
                QString funcValue = source.mid(5);
                if (funcValue.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    replacement = QString("%1_seq").arg(fieldName.toLower());
                }
                else
                {
                    replacement = funcValue;
                }
            }
            else if (source.startsWith("VAL|"))
            {
                QString valValue = source.mid(4);
                if (valValue.compare("nextval", Qt::CaseInsensitive) == 0)
                {
                    replacement = QString("%1_seq").arg(fieldName.toLower());
                }
                else
                {
                    replacement = formatValueForField(valValue, field);
                }
            }
            else
            {
                replacement = formatValueForField(source, field);
            }

            QRegularExpression regex("\\b" + QRegularExpression::escape(fieldName) + "\\b");
            result.replace(regex, replacement);
        }
    }

    return result;
}

QString RichTextToInsertRun::replaceFieldReferencesInCondition(const QString &condition)
{
    QString result = condition;

    if (!m_pTable)
        return result;

    // Простой подход: заменяем только те поля, которые в левой части условий
    // и только если для них есть COLUMN| маппинг

    // Разбиваем условие на части по AND/OR
    QStringList parts;
    QString currentPart;
    int parenLevel = 0;

    for (int i = 0; i < result.length(); ++i)
    {
        QChar ch = result[i];

        if (ch == '(') parenLevel++;
        else if (ch == ')') parenLevel--;

        if (parenLevel == 0 && i > 0)
        {
            QString lookahead = result.mid(i, 3).toUpper();
            if (lookahead == " AND" || lookahead == " OR ")
            {
                if (!currentPart.trimmed().isEmpty())
                {
                    parts << currentPart.trimmed();
                    currentPart.clear();
                }
                i += 3; // Пропускаем AND/OR
                continue;
            }
        }

        currentPart += ch;
    }

    if (!currentPart.trimmed().isEmpty())
    {
        parts << currentPart.trimmed();
    }

    // Обрабатываем каждую часть отдельно
    QStringList processedParts;

    for (const QString &part : parts)
    {
        QString processedPart = part;

        // Ищем шаблон "field = value" или "field operator value"
        QRegularExpression conditionRegex(
            "\\b([a-zA-Z_][a-zA-Z0-9_]*)\\s*([=<>!]+|\\s+LIKE\\s+|\\s+IN\\s+)\\s*(.+)$",
            QRegularExpression::CaseInsensitiveOption
            );

        QRegularExpressionMatch match = conditionRegex.match(part);
        if (match.hasMatch())
        {
            QString fieldName = match.captured(1).trimmed();
            QString operatorStr = match.captured(2).trimmed();
            QString rightPart = match.captured(3).trimmed();

            // Проверяем, есть ли поле в маппинге как COLUMN|
            if (m_FieldMapping.contains(fieldName))
            {
                QString source = m_FieldMapping[fieldName];
                if (source.startsWith("COLUMN|"))
                {
                    QString columnStr = source.mid(7);
                    bool ok;
                    int columnIndex = columnStr.toInt(&ok);

                    if (ok && columnIndex >= 0)
                    {
                        QString paramName = "p_col" + QString::number(columnIndex + 1);

                        // Проверяем, не является ли правая часть тем же самым параметром
                        if (rightPart != paramName)
                        {
                            // Заменяем поле на параметр в левой части
                            processedPart = paramName + " " + operatorStr + " " + rightPart;
                        }
                    }
                }
            }
        }

        processedParts << processedPart;
    }

    return processedParts.join(" AND ");
}

bool RichTextToInsertRun::isParameterExists(const QString &paramName) const
{
    // Извлекаем номер колонки из имени параметра: "p_col3" -> 3
    QRegularExpression regex("p_col(\\d+)");
    QRegularExpressionMatch match = regex.match(paramName);
    if (match.hasMatch())
    {
        int colNumber = match.captured(1).toInt();

        // Проверяем, есть ли такой номер колонки в маппинге
        for (auto it = m_FieldMapping.begin(); it != m_FieldMapping.end(); ++it)
        {
            QString source = it.value();
            if (source.startsWith("COLUMN|"))
            {
                QString columnStr = source.mid(7);
                bool ok;
                int columnIndex = columnStr.toInt(&ok);
                if (ok && columnIndex >= 0 && (columnIndex + 1) == colNumber)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

QString RichTextToInsertRun::replaceProblematicSymbols(const QString &text)
{
    if (text.isEmpty())
        return text;

    QString result = text;

    // Заменяем проблемные символы для CP866
    QHash<QChar, QChar> symbolReplacements = {
        {QChar(0x2013), '-'},    // Длинное тире → обычный дефис
        {QChar(0x2014), '-'},    // Длинное тире → обычный дефис
        {QChar(0x2018), '\''},   // Левая одинарная кавычка → обычная кавычка
        {QChar(0x2019), '\''},   // Правая одинарная кавычка → обычная кавычка
        {QChar(0x201C), '\"'},   // Левая двойная кавычка → обычная кавычка
        {QChar(0x201D), '\"'},   // Правая двойная кавычка → обычная кавычка
        {QChar(0x00AB), '\"'},   // Левая угловая кавычка → обычная кавычка
        {QChar(0x00BB), '\"'},   // Правая угловая кавычка → обычная кавычка
        {QChar(0x2039), '\''},   // Левая одинарная угловая кавычка → обычная кавычка
        {QChar(0x203A), '\''},   // Правая одинарная угловая кавычка → обычная кавычка
        {QChar(0x2026), '.'},    // Многоточие → три точки (обработаем отдельно)
        {QChar(0x201E), '\"'},   // Нижняя двойная кавычка → обычная кавычка
        {QChar(0x2E3A), '-'},    // Двойное длинное тире → обычный дефис
        {QChar(0x2E3B), '-'},    // Тройное длинное тире → обычный дефис
    };

    // Замена символов из хэша
    for (auto it = symbolReplacements.begin(); it != symbolReplacements.end(); ++it)
    {
        result.replace(it.key(), it.value());
    }

    // Специальная обработка для многоточия
    result.replace(QString(QChar(0x2026)), "...");

    // Дополнительно: заменяем "умные" кавычки, которые могут быть в тексте
    result.replace("«", "\"");
    result.replace("»", "\"");
    result.replace("„", "\"");
    result.replace("“", "\"");
    result.replace("‟", "\"");
    result.replace("”", "\"");
    result.replace("‘", "'");
    result.replace("’", "'");
    result.replace("‛", "'");
    result.replace("‚", "'");

    return result;
}
