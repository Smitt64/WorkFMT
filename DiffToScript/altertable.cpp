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

        QString indent = generateIndent(indentSpaces);

        if (type == "update")
        {
            // Обработка обновления столбца
            if (oldValue.startsWith("COMMENT ON COLUMN") && newValue.startsWith("COMMENT ON COLUMN"))
            {
                // Если это изменение комментария
                /*QString columnName = newValue.section(' ', 3, 3); // Извлекаем имя колонки
                QString comment = newValue.section(' ', 6, -1); // Извлекаем комментарий

                // Ищем, есть ли блок для этой колонки
                //bool columnBlockFound = false;
                for (const QJsonValue &change2 : qAsConst(changes))
                {
                    QJsonObject obj2 = change2.toObject();
                    QString type2 = obj2["type"].toString();
                    QString oldValue2 = obj2["oldValue"].toString().trimmed();
                    QString newValue2 = obj2["newValue"].toString().trimmed();

                    if ((type2 == "update" || type2 == "insert") && !oldValue2.startsWith("COMMENT ON COLUMN"))
                    {
                        auto column = extractColumnNameAndType(type2 == "update" ? oldValue2 : newValue2);

                        if (column.first == columnName)
                        {
                            // Если найден блок для колонки, добавляем комментарий в него
                            stream << indent << "EXECUTE IMMEDIATE 'COMMENT ON COLUMN " << tableName << "." << columnName << " IS " << comment << "';" << Qt::endl;
                            //columnBlockFound = true;
                            break;
                        }
                    }
                }*/

                /*if (!columnBlockFound)
                {*/
                    // Если блок для колонки не найден, создаем отдельный блок для комментария
                    stream << "DECLARE" << Qt::endl;
                    stream << indent << "e_col_exist EXCEPTION;" << Qt::endl;
                    stream << indent << "PRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
                    stream << "BEGIN" << Qt::endl;
                    stream << indent << "EXECUTE IMMEDIATE '" << escapeSqlString(newValue) << "';" << Qt::endl;
                    stream << "EXCEPTION" << Qt::endl;
                    stream << indent << "WHEN e_col_exist THEN NULL;" << Qt::endl;
                    stream << "END;" << Qt::endl << "/" << Qt::endl << Qt::endl;
                //}
            }
            else
            {
                // Если это изменение типа или имени колонки
                auto oldColumn = extractColumnNameAndType(oldValue);
                auto newColumn = extractColumnNameAndType(newValue);

                if (!oldColumn.first.isEmpty() && !newColumn.first.isEmpty())
                {
                    stream << "DECLARE" << Qt::endl;
                    stream << indent << "e_col_exist EXCEPTION;" << Qt::endl;
                    stream << indent << "PRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
                    stream << "BEGIN" << Qt::endl;

                    if (oldColumn.first != newColumn.first)
                    {
                        // Если имя колонки изменилось, выполняем RENAME COLUMN
                        stream << indent << "EXECUTE IMMEDIATE 'ALTER TABLE " << tableName << " RENAME COLUMN " << oldColumn.first << " TO " << newColumn.first << "';" << Qt::endl;
                    }

                    if (oldColumn.second != newColumn.second)
                    {
                        // Если тип колонки изменился, выполняем MODIFY
                        stream << indent << "EXECUTE IMMEDIATE 'ALTER TABLE " << tableName << " MODIFY " << newColumn.first << " " << newColumn.second << "';" << Qt::endl;
                    }

                    // Добавляем комментарий, если он есть
                    for (const QJsonValue &change2 : qAsConst(changes))
                    {
                        QJsonObject obj2 = change2.toObject();
                        QString type2 = obj2["type"].toString();
                        QString oldValue2 = obj2["oldValue"].toString().trimmed();
                        QString newValue2 = obj2["newValue"].toString().trimmed();

                        if (type2 == "update" && newValue2.startsWith("COMMENT ON COLUMN"))
                        {
                            QString columnName = newValue2.section(' ', 3, 3);
                            if (columnName == newColumn.first)
                            {
                                QString comment = newValue2.section(' ', 6, -1);
                                stream << indent << "EXECUTE IMMEDIATE 'COMMENT ON COLUMN " << tableName << "." << columnName << " IS " << comment << "';" << Qt::endl;
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
                    stream << indent << "EXECUTE IMMEDIATE 'ALTER TABLE " << tableName << " DROP COLUMN " << oldColumn.first << "';" << Qt::endl;
                    stream << "EXCEPTION" << Qt::endl;
                    stream << indent << "WHEN e_col_not_exist THEN NULL;" << Qt::endl;
                    stream << "END;" << Qt::endl << "/" << Qt::endl << Qt::endl;
                }
            }
        }
        else if (type == "insert")
        {
            // Обработка добавления нового столбца
            if (newValue.startsWith("COMMENT ON COLUMN")) {
                // Если это добавление комментария
                QString columnName = newValue.section(' ', 3, 3);
                QString comment = newValue.section(' ', 6, -1);

                // Ищем, есть ли блок для этой колонки
                bool columnBlockFound = false;
                for (const QJsonValue &change2 : qAsConst(changes))
                {
                    QJsonObject obj2 = change2.toObject();
                    QString type2 = obj2["type"].toString();
                    QString oldValue2 = obj2["oldValue"].toString().trimmed();
                    QString newValue2 = obj2["newValue"].toString().trimmed();

                    if ((type2 == "update" || type2 == "insert") && !oldValue2.startsWith("COMMENT ON COLUMN"))
                    {
                        auto column = extractColumnNameAndType(type2 == "update" ? oldValue2 : newValue2);
                        if (column.first == columnName)
                        {
                            // Если найден блок для колонки, добавляем комментарий в него
                            stream << indent << "EXECUTE IMMEDIATE 'COMMENT ON COLUMN " << tableName << "." << columnName << " IS " << comment << "';" << Qt::endl;
                            columnBlockFound = true;
                            break;
                        }
                    }
                }

                if (!columnBlockFound)
                {
                    // Если блок для колонки не найден, создаем отдельный блок для комментария
                    stream << "DECLARE" << Qt::endl;
                    stream << indent << "e_col_exist EXCEPTION;" << Qt::endl;
                    stream << indent << "PRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
                    stream << "BEGIN" << Qt::endl;
                    stream << indent << "EXECUTE IMMEDIATE 'COMMENT ON COLUMN " << tableName << "." << columnName << " IS " << comment << "';" << Qt::endl;
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
                    stream << indent << "EXECUTE IMMEDIATE 'ALTER TABLE " << tableName << " ADD " << newColumn.first << " " << newColumn.second << "';" << Qt::endl;

                    // Добавляем комментарий, если он есть
                    for (const QJsonValue &change2 : changes)
                    {
                        QJsonObject obj2 = change2.toObject();
                        QString type2 = obj2["type"].toString();
                        QString oldValue2 = obj2["oldValue"].toString().trimmed();
                        QString newValue2 = obj2["newValue"].toString().trimmed();

                        if (type2 == "insert" && newValue2.startsWith("COMMENT ON COLUMN"))
                        {
                            QString columnName = newValue2.section(' ', 3, 3);
                            if (columnName == newColumn.first) {
                                QString comment = newValue2.section(' ', 6, -1);
                                stream << indent << "EXECUTE IMMEDIATE 'COMMENT ON COLUMN " << tableName << "." << columnName << " IS " << comment << "';" << Qt::endl;
                            }
                        }
                    }

                    stream << "EXCEPTION" << Qt::endl;
                    stream << indent << "WHEN e_col_exist THEN NULL;" << Qt::endl;
                    stream << "END;" << Qt::endl << "/" << Qt::endl << Qt::endl;
                }
            }
        }
    }
}
