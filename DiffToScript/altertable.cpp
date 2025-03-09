#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QRegularExpression>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <difflogging.h>

QString cleanColumnName(const QString &columnName)
{
    QString cleaned = columnName.trimmed();
    if (cleaned.startsWith(','))
        cleaned.remove(0, 1).trimmed();

    if (cleaned.endsWith(','))
    {
        cleaned.chop(1);
        cleaned = cleaned.trimmed();
    }

    return cleaned.trimmed();
}

QPair<QString, QString> extractColumnNameAndType(const QString &columnDefinition)
{
    QString cleaned = cleanColumnName(columnDefinition);
    QStringList parts = cleaned.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (parts.size() >= 2)
        return qMakePair(parts[0], parts[1]);

    return qMakePair(QString(), QString());
}

QString extractTableName(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName(); // Получаем имя файла с расширением
    return fileName.split('.').first(); // Убираем расширение
}

QString generateIndent(int spaces)
{
    return QString(spaces, ' ');
}

QString escapeSqlString(const QString &input)
{
    QString escapedString = input;

    // Экранируем одинарные кавычки (удваиваем их)
    escapedString.replace("'", "''");

    // Дополнительно можно экранировать другие символы, если это необходимо
    // Например, обратный слэш:
    escapedString.replace("\\", "\\\\");

    return escapedString;
}

QString normalColumnType(const QString &input)
{
    QString result = input.simplified();

    if (result.endsWith(","))
        result.chop(1);

    return result.toUpper();
}

typedef std::tuple<QString, QString> CommentAndColumpTyple;
CommentAndColumpTyple extractCommentAndColumn(const QString &input)
{
    QString columnName = input.section(' ', 3, 3);
    QString comment = input.section(' ', 6, -1);

    int commentpos = input.indexOf(columnName);
    if (commentpos != -1)
        comment = input.mid(commentpos + 1 + columnName.size());

    int dotpos = columnName.indexOf('.');
    if (dotpos != -1)
        columnName = columnName.mid(dotpos + 1);

    if (comment.startsWith("IS "))
        comment = comment.remove(0, 3);

    if (comment.endsWith(";"))
        comment.chop(1);

    if (comment.startsWith('\''))
        comment.remove(0, 1);

    if (comment.endsWith('\''))
        comment.chop(1);

    return std::make_tuple(columnName.toUpper(), comment);
}

void generateUpdateScript(const QByteArray &jsonData, QTextStream &stream, int indentSpaces)
{
    // Декодируем данные из кодировки IBM-866
    QTextCodec *codec = QTextCodec::codecForName("IBM-866");
    if (!codec)
    {
        qWarning() << "IBM-866 codec not available!";
        return;
    }

    QString jsonText = codec->toUnicode(jsonData);

    // Парсинг JSON
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8());
    if (doc.isNull())
    {
        qWarning() << "Failed to parse JSON";
        return;
    }

    QJsonArray changes = doc.array();
    QString tableName;
    QStringList BlockedCommentsColumn;

    // Сначала ищем секцию index, чтобы извлечь имя таблицы
    for (const QJsonValue &change : qAsConst(changes))
    {
        QJsonObject obj = change.toObject();
        QString type = obj["type"].toString();
        if (type == "index")
        {
            QString oldValue = obj["oldValue"].toString().trimmed();
            tableName = extractTableName(oldValue);
            break; // Имя таблицы найдено, выходим из цикла
        }
    }

    if (tableName.isEmpty())
    {
        qWarning() << "Table name not found in the JSON!";
        return;
    }

    // Генерируем SQL-скрипт на основе изменений
    for (const QJsonValue &change : qAsConst(changes))
    {
        QJsonObject obj = change.toObject();
        QString type = obj["type"].toString();
        QString oldValue = obj["oldValue"].toString().trimmed();
        QString newValue = obj["newValue"].toString().trimmed();

        if (oldValue.simplified().isEmpty() && newValue.simplified().isEmpty())
            continue;

        QString indent = generateIndent(indentSpaces);

        if (type == "update")
        {
            // Обработка обновления столбца
            if (oldValue.startsWith("COMMENT ON COLUMN") && newValue.startsWith("COMMENT ON COLUMN"))
            {
                CommentAndColumpTyple coltuple = extractCommentAndColumn(newValue);
                QString columnName = std::get<0>(coltuple);

                if (!BlockedCommentsColumn.contains(columnName))
                {
                    // Если блок для колонки не найден, создаем отдельный блок для комментария
                    stream << "DECLARE" << Qt::endl;
                    stream << indent << "e_col_exist EXCEPTION;" << Qt::endl;
                    stream << indent << "PRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
                    stream << "BEGIN" << Qt::endl;

                    QString comment = escapeSqlString(newValue);
                    if (comment.endsWith(";"))
                        comment.chop(1);

                    stream << indent << "EXECUTE IMMEDIATE '" << comment << "';" << Qt::endl;
                    stream << "EXCEPTION" << Qt::endl;
                    stream << indent << "WHEN e_col_exist THEN NULL;" << Qt::endl;
                    stream << "END;" << Qt::endl << "/" << Qt::endl << Qt::endl;
                }
            }
            else
            {
                // Если это изменение типа или имени колонки
                auto oldColumn = extractColumnNameAndType(oldValue);
                auto newColumn = extractColumnNameAndType(newValue);

                if (!oldColumn.first.isEmpty() && !newColumn.first.isEmpty() &&
                        oldColumn.first != newColumn.first &&
                        oldColumn.second != newColumn.second)
                {
                    stream << "DECLARE" << Qt::endl;
                    stream << indent << "e_col_exist EXCEPTION;" << Qt::endl;
                    stream << indent << "PRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
                    stream << "BEGIN" << Qt::endl;

                    if (oldColumn.first != newColumn.first)
                    {
                        // Если имя колонки изменилось, выполняем RENAME COLUMN
                        stream << indent << "EXECUTE IMMEDIATE 'ALTER TABLE " << tableName << " RENAME COLUMN " << oldColumn.first.toUpper() << " TO " << normalColumnType(newColumn.first) << "';" << Qt::endl;
                    }

                    if (oldColumn.second != newColumn.second)
                    {
                        // Если тип колонки изменился, выполняем MODIFY
                        stream << indent << "EXECUTE IMMEDIATE 'ALTER TABLE " << tableName << " MODIFY " << newColumn.first.toUpper() << " " << normalColumnType(newColumn.second) << "';" << Qt::endl;
                    }
                    else
                        continue;

                    // Добавляем комментарий, если он есть
                    for (const QJsonValue &change2 : qAsConst(changes))
                    {
                        QJsonObject obj2 = change2.toObject();
                        QString type2 = obj2["type"].toString();
                        QString oldValue2 = obj2["oldValue"].toString().trimmed();
                        QString newValue2 = obj2["newValue"].toString().trimmed();

                        if (type2 == "update" && newValue2.startsWith("COMMENT ON COLUMN"))
                        {
                            CommentAndColumpTyple coltuple = extractCommentAndColumn(newValue);
                            QString columnName = std::get<0>(coltuple);
                            QString comment = std::get<1>(coltuple);

                            if (columnName == newColumn.first.toUpper())
                            {
                                BlockedCommentsColumn.append(columnName);
                                stream << indent << "EXECUTE IMMEDIATE 'COMMENT ON COLUMN " << tableName << "." << columnName.toUpper() << " IS ''" << comment << "''';" << Qt::endl;
                            }
                        }
                    }

                    stream << "EXCEPTION" << Qt::endl;
                    stream << indent << "WHEN e_col_exist THEN NULL;" << Qt::endl;
                    stream << "END;" << Qt::endl << "/" << Qt::endl << Qt::endl;
                }
            }
        }
        else if (type == "delete")
        {
            // Обработка удаления столбца
            if (oldValue.startsWith("COMMENT ON COLUMN"))
            {
                // Если это удаление комментария
                QString columnName = oldValue.section(' ', 3, 3);
                stream << "-- Comment removed for column: " << columnName << Qt::endl << Qt::endl;
            }
            else
            {
                auto oldColumn = extractColumnNameAndType(oldValue);
                if (!oldColumn.first.isEmpty())
                {
                    stream << "DECLARE" << Qt::endl;
                    stream << indent << "e_col_not_exist EXCEPTION;" << Qt::endl;
                    stream << indent << "PRAGMA EXCEPTION_INIT (e_col_not_exist, -904);" << Qt::endl;
                    stream << "BEGIN" << Qt::endl;
                    stream << indent << "EXECUTE IMMEDIATE 'ALTER TABLE " << tableName << " DROP COLUMN " << oldColumn.first.toUpper() << "';" << Qt::endl;
                    stream << "EXCEPTION" << Qt::endl;
                    stream << indent << "WHEN e_col_not_exist THEN NULL;" << Qt::endl;
                    stream << "END;" << Qt::endl << "/" << Qt::endl << Qt::endl;
                }
            }
        }
        else if (type == "insert")
        {
            // Обработка добавления нового столбца
            if (newValue.startsWith("COMMENT ON COLUMN"))
            {
                // Если это добавление комментария
                CommentAndColumpTyple coltuple = extractCommentAndColumn(newValue);
                QString columnName = std::get<0>(coltuple);
                QString comment = std::get<1>(coltuple);

                if (!BlockedCommentsColumn.contains(columnName))
                {
                    // Если блок для колонки не найден, создаем отдельный блок для комментария
                    stream << "DECLARE" << Qt::endl;
                    stream << indent << "e_col_exist EXCEPTION;" << Qt::endl;
                    stream << indent << "PRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
                    stream << "BEGIN" << Qt::endl;
                    stream << indent << "EXECUTE IMMEDIATE 'COMMENT ON COLUMN " << tableName << "." << columnName.toUpper() << " IS ''" << escapeSqlString(comment) << "''';" << Qt::endl;
                    stream << "EXCEPTION" << Qt::endl;
                    stream << indent << "WHEN e_col_exist THEN NULL;" << Qt::endl;
                    stream << "END;" << Qt::endl << "/" << Qt::endl << Qt::endl;
                }
            }
            else
            {
                auto newColumn = extractColumnNameAndType(newValue);
                if (!newColumn.first.isEmpty())
                {
                    stream << "DECLARE" << Qt::endl;
                    stream << indent << "e_col_exist EXCEPTION;" << Qt::endl;
                    stream << indent << "PRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
                    stream << "BEGIN" << Qt::endl;
                    stream << indent << "EXECUTE IMMEDIATE 'ALTER TABLE " << tableName << " ADD " << newColumn.first.toUpper() << " " << normalColumnType(newColumn.second) << "';" << Qt::endl;

                    // Добавляем комментарий, если он есть
                    for (const QJsonValue &change2 : qAsConst(changes))
                    {
                        QJsonObject obj2 = change2.toObject();
                        QString type2 = obj2["type"].toString();
                        QString oldValue2 = obj2["oldValue"].toString().trimmed();
                        QString newValue2 = obj2["newValue"].toString().trimmed();

                        if (type2 == "insert" && newValue2.startsWith("COMMENT ON COLUMN"))
                        {
                            CommentAndColumpTyple coltuple = extractCommentAndColumn(newValue2);
                            QString columnName = std::get<0>(coltuple);
                            QString comment = std::get<1>(coltuple);

                            if (columnName == newColumn.first.toUpper())
                            {
                                BlockedCommentsColumn.append(columnName);
                                stream << indent << "EXECUTE IMMEDIATE 'COMMENT ON COLUMN " << tableName << "." << columnName.toUpper() << " IS ''" << escapeSqlString(comment) << "''';" << Qt::endl;
                            }
                        }
                    }

                    stream << "EXCEPTION" << Qt::endl;
                    stream << indent << "WHEN e_col_exist THEN NULL;" << Qt::endl;
                    stream << "END;" << Qt::endl << "/" << Qt::endl << Qt::endl;
                }
            }
        }

        stream.flush();
    }
}
