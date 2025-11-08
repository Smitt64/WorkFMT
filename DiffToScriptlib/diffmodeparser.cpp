#include "diffmodeparser.h"
#include "toolsruntime.h"
#include <QJsonDocument>

QJsonObject DiffLine::toJson() const
{
    QJsonObject json;
    json["type"] = typeToString(type);
    json["oldValue"] = oldValue;
    json["newValue"] = newValue;
    return json;
}

QString DiffLine::typeToString(Type type) const
{
    switch (type)
    {
    case Index: return "index";
    case Update: return "update";
    case Delete: return "delete";
    case Insert: return "insert";
    }

    return "Unknown";
}

void DiffLine::toXml(QXmlStreamWriter& writer) const
{
    writer.writeStartElement("DiffLine");
    writer.writeAttribute("type", typeToString(type));

    if (!oldValue.isEmpty())
        writer.writeTextElement("oldValue", oldValue);

    if (!newValue.isEmpty())
        writer.writeTextElement("newValue", newValue);

    writer.writeEndElement();
}

// -------------------------------------------------------------------------------------

DiffModeParser::DiffModeParser(QTextStream& stream)
{
    parse(stream);
}

const QVector<DiffLine>& DiffModeParser::getResult() const
{
    return diffLines;
}

QJsonArray DiffModeParser::toJson() const
{
    QJsonArray jsonArray;
    for (const auto& diffLine : diffLines)
        jsonArray.append(diffLine.toJson());

    return jsonArray;
}

void DiffModeParser::toXml(QXmlStreamWriter& writer) const
{
    writer.writeStartDocument();
    writer.writeStartElement("DiffLines");

    for (const auto& diffLine : diffLines)
        diffLine.toXml(writer);

    writer.writeEndElement();
    writer.writeEndDocument();
}

void DiffModeParser::serialize(QTextStream& outputStream, const QString& format) const
{
    if (format.toLower() == "json")
    {
        QJsonArray jsonArray = toJson();
        QJsonDocument doc(jsonArray);
        outputStream << doc.toJson(QJsonDocument::Indented);
    }
    else if (format.toLower() == "xml")
    {
        QXmlStreamWriter writer(outputStream.device());
        writer.setCodec(outputStream.codec());
        writer.setAutoFormatting(true); // Включаем форматирование для читаемости
        toXml(writer);
    }
    else
        outputStream << "Unsupported format:" << format;
}

bool DiffModeParser::isPotentialUpdate(const QString& deletedLine, const QString& addedLine)
{
    // Пример логики: строки считаются обновлением, если они отличаются только в конце
    // (например, добавление "12" в вашем случае)
    return deletedLine.trimmed() != addedLine.trimmed() &&
            toolLevenshteinSimilarity(deletedLine, addedLine) > 60.0;
}

void DiffModeParser::parse(QTextStream& stream)
{
    QString line;
    while (stream.readLineInto(&line))
    {
        if (line.startsWith("Index:"))
        {
            // Информация о файле
            diffLines.append({DiffLine::Index, line, ""});
        }
        else if (line.startsWith("@@"))
        {
            // Чанк (hunk) - пропускаем, так как нас интересуют только изменения
            continue;
        }
        else if (line.startsWith("---") || line.startsWith("+++"))
        {
            // Информация о версиях файла - пропускаем
            continue;
        }
        else if (line.startsWith("-"))
        {
            // Удаленная строка
            deletedLines.append(line.mid(1)); // Сохраняем удаленную строку в буфер
            diffLines.append({DiffLine::Delete, line.mid(1), ""});
        }
        else if (line.startsWith("+"))
        {
            // Вставленная строка
            QString addedLine = line.mid(1);
            bool isUpdate = false;

            // Проверяем, есть ли в буфере удаленных строк подходящая строка для обновления
            for (int i = 0; i < deletedLines.size(); ++i)
            {
                if (isPotentialUpdate(deletedLines[i], addedLine))
                {
                    // Если найдено совпадение, это обновление
                    diffLines.append({DiffLine::Update, deletedLines[i], addedLine});

                    /*QVector<DiffLine>::iterator iter = std::remove_if(diffLines.begin(), diffLines.end(), [=](const DiffLine &line)
                    {
                        if (line.oldValue == deletedLines[i])
                            return true;

                        return false;
                    });

                    diffLines.removeAt(std::distance(diffLines.begin(), iter));*/

                    auto iter = std::remove_if(diffLines.begin(), diffLines.end(), [&](const DiffLine& line)
                    {
                        return line.type == DiffLine::Delete && line.oldValue == deletedLines[i];
                    });

                    diffLines.erase(iter, diffLines.end()); // Удаляем перемещенные элементы
                    deletedLines.remove(i); // Удаляем из буфера
                    isUpdate = true;
                    break;
                }
            }

            if (!isUpdate)
            {
                // Если не найдено совпадение, это вставка
                diffLines.append({DiffLine::Insert, "", addedLine});
            }
        }
        else if (line.startsWith(" "))
        {
            // Неизмененная строка (контекст) - пропускаем
            continue;
        }
    }
}
