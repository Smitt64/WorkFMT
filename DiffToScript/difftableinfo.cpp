#include "difftableinfo.h"
#include "fmtcore.h"
#include "toolsruntime.h"
#include "fmtfield.h"
#include "difflogging.h"
#include "fmtindex.h"
#include "fmtsegment.h"
#include <QtSql>
#include <QInputDialog>

DiffTableInfo::DiffTableInfo()
{

}

QString BlobTypeToString(int type)
{
    switch (type)
    {
    case 0:
        return "BT_BLOB_NO";
    case 1:
        return "BT_BLOB_VAR";
    case 2:
        return "BT_BLOB_STREAM";
    case 3:
        return "BT_CLOB";
    }

    throw std::runtime_error(QObject::tr("Неизвестное значение типа блоба: %1")
                             .arg(type).toLocal8Bit().data());
}

DiffField DiffTableInfo::field(const QString &name) const
{
    DiffFields::const_iterator iter = std::find_if(fields.cbegin(), fields.cend(), [=](const DiffField &fld) -> bool
    {
        return fld.name.toUpper() == name.toUpper();
    });

    if (iter != fields.cend())
        return *iter;

    return DiffField();
}

DiffFields DiffTableInfo::missingFldInDat() const
{
    DiffFields missing;

    DiffFields::const_iterator iter = fields.cbegin();
    for (; iter != fields.cend(); ++iter)
    {
        if (!realFields.contains((*iter).name.toUpper()))
            missing.append(*iter);
    }

    return missing;
}

bool DiffTableInfo::firstUniq(DatIndex &idx, bool skipAutoInc) const
{
    bool found = false;

    for (const DatIndex &index : indexes)
    {
        if (index.isUnique)
        {
            if (index.hasAutoinc() && skipAutoInc)
                continue;
            else
            {
                found = true;
                idx = index;
                break;
            }
        }
    }
    return found;
}

QStringList DiffTableInfo::readColumnsFromFile(const QString& filePath)
{
    QStringList columns;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Не удалось открыть файл:" << filePath;
        return columns;
    }

    QTextStream in(&file);
    in.setCodec("IBM 866"); // Устанавливаем кодировку (866 для кириллицы)

    QString fileContent = in.readAll(); // Читаем весь файл в строку
        file.close();

        // 1. Ищем первое вхождение "("
        int startPos = fileContent.indexOf('(');
        if (startPos == -1)
        {
            qWarning() << "Секция с колонками не найдена.";
            return columns;
        }

        // 2. Ищем вхождение "BEGINDATA"
        int begindataPos = fileContent.indexOf("BEGINDATA", startPos);
        if (begindataPos == -1)
        {
            qWarning() << "Секция BEGINDATA не найдена.";
            return columns;
        }

        // 3. Отрезаем секцию с колонками: от "(" до "BEGINDATA"
        QString columnsSection = fileContent.mid(startPos + 1, begindataPos - startPos - 1);

        // 4. Разделяем на строки по символу новой строки
        QStringList lines = columnsSection.split('\n', Qt::SkipEmptyParts);

        // 5. Обрабатываем каждую строку
        for (const QString& line : qAsConst(lines))
        {
            QString trimmedLine = line.trimmed(); // Убираем пробелы в начале и конце

            // Убираем закрывающую скобку, если она есть
            if (trimmedLine.endsWith(')'))
                trimmedLine.chop(1); // Удаляем последний символ

            // Берем только первое слово (до первого пробела или запятой)
            int spacePos = trimmedLine.indexOf(' ');
            int commaPos = trimmedLine.indexOf(',');

            // Определяем, какой символ встречается раньше: пробел или запятая
            int endOfWordPos = -1;
            if (spacePos != -1 && commaPos != -1)
                endOfWordPos = qMin(spacePos, commaPos);
            else if (spacePos != -1)
                endOfWordPos = spacePos;
            else if (commaPos != -1)
                endOfWordPos = commaPos;

            // Извлекаем первое слово
            QString column;
            if (endOfWordPos != -1)
                column = trimmedLine.left(endOfWordPos).trimmed();
            else
                column = trimmedLine; // Если нет пробелов или запятых, берем всю строку

            if (!column.isEmpty())
                columns.append(column.toUpper());
        }

        return columns;
}

void DiffTableInfo::loadFromFmt(FmtTable *fmtTable, const QString &datfilename)
{
    FmtInit();
    fields.clear();
    name = fmtTable->name().toUpper();
    qCInfo(logScriptTable) << "Start load from fmt. Table name = " << name;
    for (int i = 0; i < fmtTable->fieldsCount(); ++i)
    {
        FmtField* fld = fmtTable->field(i);
        bool isString = (fld->isString() || fld->type() == fmtt_CHR);

        DiffField df;
        df.name = fld->name().toUpper();
        df.type = fld->type();
        df.size = fld->size();
        df.typeName = fmtTypeNameForType(fld->type());
        df.isAutoinc = fld->isAutoInc();
        df.isString = isString;

        fields.append(df);
        realFields.append(df.name.toUpper());
        qCInfo(logScriptTable) << "Loaded FMT field: " << df.name << " " << df.typeName << " " << (df.isAutoinc?"autoinc":"");
    }

    if (fmtTable->blobLen() > 0)
    {
        DiffField df = { BlobFieldString(fmtTable->blobType()), fmtTable->blobType(), BlobTypeToString(fmtTable->blobType()), false, true};
        fields.append(df);
        qCInfo(logScriptTable) << "Loaded FMT field: " << df.name << " " << df.typeName << " " << (df.isAutoinc?"autoinc":"");
    }

    for (int i = 0; i < fmtTable->indecesCount(); ++i)
    {
        FmtIndex* indx = fmtTable->tableIndex(i);
        DatIndexes::iterator it = indexes.insert(indexes.end(), DatIndex{});
        it->name = indx->name();
        it->isUnique = indx->isUnique();

        qCInfo(logScriptTable) << "Loaded FMT index: " << it->name << " isUnique " << it->isUnique;

        for (int j = 0; j < indx->segmentsCount(); ++j)
        {
            FmtSegment* sgmt = indx->segment(j);
            it->fields.append(
                        {
                            sgmt->field()->name(),
                            sgmt->field()->isAutoInc(),
                            sgmt->field()->type(),
                            sgmt->field()->isString()
                        });
            qCInfo(logScriptTable) << "Added field for index: name = " << sgmt->field()->name() << " isAutoInc = " << sgmt->field()->isAutoInc();
        }
    }
    qCInfo(logScriptTable) << "End load from fmt. Table name = " << name;

    QStringList tmpFields;

    if (!datfilename.isEmpty())
        tmpFields = readColumnsFromFile(datfilename);

    if (!tmpFields.isEmpty())
        realFields = tmpFields;
    else
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
        QString infoFile = toolFullFileNameFromDir("datstruct.info");
        db.setDatabaseName(infoFile);

        if (db.open())
        {
            QSqlQuery q(db);
            if (q.prepare("SELECT column FROM DAT_FIELDS where upper(NAME) = :val order by id"))
            {
                q.bindValue(":val", name);

                if (q.exec())
                {
                    bool isFirst = true;
                    while(q.next())
                    {
                        if (isFirst)
                        {
                            realFields.clear();
                            isFirst = false;
                        }

                        QString fld = q.value(0).toString();
                        qCInfo(logDatTable) << "Loaded dat struct field" << fld;

                        realFields.append(fld.toUpper());
                    }
                }
                else
                {
                    QString err = q.lastError().text();
                    qCWarning(logDatTable) << "Can't find dat struct info for table" << name << q.lastError().text();
                }
            }
            else
            {
                QString err = q.lastError().text();
                qCWarning(logDatTable) << "Can't find dat struct info for table" << name << q.lastError().text();
            }

            db.close();
        }
        else
            qCWarning(logDatTable) << "Can't open dat struct info file: datstruct.info";
    }
}
