#include "sqlscriptmain.h"
#include "difflogging.h"
#include "fmtcore.h"
#include <QSqlQuery>

const QString PADDING = "  ";

static QString Padding(int depth = 1)
{
    return PADDING.repeated(depth);
}

SqlScriptMain::SqlScriptMain(QSharedPointer<DbSpelling> &dbSpelling, QSharedPointer<DiffConnection> connection)
{
    _dbSpelling = dbSpelling;
    _connection = connection;
}

QStringList SqlScriptMain::makeVariables(JoinTable* joinTable)
{
    QStringList variables;
    variables << buildVariable(joinTable->datTable);

    JoinListIterator it(joinTable, joinTable->joinList.firstChild(joinTable));
    for (; it.itChild != joinTable->joinList.end(); it = it.nextChild())
    {
        variables << buildVariable((*it.itChild)->child->datTable);
    }

    if (variables.back() == "")
        variables.pop_back();

    return variables;
}

int SqlScriptMain::getAutoincIndex(const DatTable* datTable)
{
    for (int i = 0; i < datTable->fields.count(); ++i)
        if (datTable->fields[i].isAutoinc)
            return i;
    return -1;
}

//Если автоинкремениа нет, то вернёт пустую строку.
QString SqlScriptMain::buildVariableName(const DatTable* datTable)
{
    int autoIncIndex = getAutoincIndex(datTable);
    if (autoIncIndex == -1)
        return "";

    QString autoIncField = datTable->fields[autoIncIndex].name;
    QString variableName = QString("v_%1__%2").arg(datTable->name, autoIncField);
    return variableName;
}

//Если автоинкремениа нет, то вернёт пустую строку.
QString SqlScriptMain::buildVariable(const DatTable* datTable)
{
    int autoIncIndex = getAutoincIndex(datTable);
    if (autoIncIndex == -1)
        return "";

    QString autoIncField = datTable->fields[autoIncIndex].name;
    QString variableName = buildVariableName(datTable);
    return QString(PADDING + "%1 %2.%3\%TYPE;").arg(variableName, datTable->name, autoIncField);
}

QVector<int> SqlScriptMain::indexesOfKeyFields(const JoinTable *joinTable)
{
    QVector<int> keyFieldIndexes;

    //Уникальный индекс из json.
    for (int i = 0; i < joinTable->datTable->fields.count(); ++i)
    {
        for (const Field& keyField: joinTable->keyFields)
        {
            if (joinTable->datTable->fields[i].name.toLower() == keyField.toLower())
            {
                keyFieldIndexes.append(joinTable->datTable->realFields.indexOf(keyField.toUpper()));
            }
        }
    }

    if (keyFieldIndexes.count())
        return keyFieldIndexes;

    keyFieldIndexes << indexesOfUniqueIndex(joinTable);

    return keyFieldIndexes;
}

QVector<int> SqlScriptMain::indexesOfUniqueIndex(const JoinTable *joinTable)
{
    QVector<int> keyFieldIndexes;

    //Индекс из fmt
    for (const DatIndex& index: joinTable->datTable->indexes)
    {
        if (index.isUnique && !index.hasAutoinc())
        {
            for (const IndexField& field: index.fields)
            {
                int indx = joinTable->datTable->fields.indexByFieldName(field.name);
                if (indx != -1)
                    keyFieldIndexes.append(indx);
            }
        }
    }

    return keyFieldIndexes;

}

//Заменяет на имя переменной из родительской таблицы.
void SqlScriptMain::replaceForeignAutoinc(const JoinTable* joinTable, DatRecord& rec)
{
    //Связь с родительской таблицей
    Join* parentJoin = joinTable->getParentJoin();
    if (parentJoin == nullptr)
        return;

    int foreignIndex = indexForeignAutoinc(joinTable, rec);

    //В индексе связи есть автоинеремент
    if (foreignIndex != -1)
    {
        //Замена значения автоинкрементного поля на имя переменной
        rec.values[parentJoin->childForeignFields[foreignIndex]] = buildVariableName(parentJoin->parent->datTable);
    }
}

int SqlScriptMain::indexForeignAutoinc(const JoinTable *joinTable, const DatRecord &rec)
{
    //Связь с родительской таблицей
    Join* parentJoin = joinTable->getParentJoin();

    //Проверка индекса связи на автоинкрементное поле
    if (parentJoin != nullptr)
    {
        int parentAutoInc = getAutoincIndex(parentJoin->parent->datTable);

        //У родительской таблицы есть автоинкремент
        if (parentAutoInc != -1)
        {
            // Расположение автоинеремента в индексе
            QVector<int> &pff = parentJoin->parentForeignFields;
            for (int i = 0; i < pff.size(); ++i)
                if (pff[i] == parentAutoInc)
                    return i;
        }
    }
    return -1;
}


int SqlScriptMain::getForeignReplaceField(const JoinTable* joinTable, DatRecord& rec)
{
    //Связь с родительской таблицей
    Join* parentJoin = joinTable->getParentJoin();

    //Проверка индекса связи на автоинкрементное поле
    if (parentJoin != nullptr)
    {
        int parentAutoInc = getAutoincIndex(parentJoin->parent->datTable);

        //У родительской таблицы есть автоинкремент
        if (parentAutoInc != -1)
        {
            // Расположение автоинеремента в индексе
            int foreignIndex = parentJoin->parentForeignFields.indexOf(parentAutoInc);
            return parentJoin->childForeignFields[foreignIndex];
        }
    }
    return -1;
}

QStringList SqlScriptMain::buildWhere(const JoinTable *joinTable, const DatRecord& oldRec)
{
    QStringList where;
    DatRecord rec = oldRec;

    //В where пойдут только ключевые поля.
    QVector<int> keyFieldIndexes = indexesOfKeyFields(joinTable);

    replaceForeignAutoinc(joinTable, rec);

    dateSpelling(joinTable, rec);
    stringSpelling(joinTable, rec);

    if (keyFieldIndexes.count() == 0)
        keyFieldIndexes = indexesOfUniqueIndex(joinTable);

    for (int i = 0; i < keyFieldIndexes.count(); ++i)
    {
        where.append(joinTable->datTable->fields[keyFieldIndexes[i]].name + " = " + rec.values[keyFieldIndexes[i]]);
    }

    return where;
}

int SqlScriptMain::buildInsertStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex)
{
    DatRecord rec = joinTable->datTable->records[recIndex];

    //Проверка индекса на автоинкрементное поле
    QString variable = buildVariableName(joinTable->datTable);

    //Определение полей для замены

    //Определение автоинкрементного поля и замена на ноль
    int autoIncIndex = getAutoincIndex(joinTable->datTable);
    if (autoIncIndex != -1)
        rec.values[autoIncIndex] = variable;

    QString InsertFieldList;
    for (const QString &fld : joinTable->datTable->realFields)
    {
        if (!InsertFieldList.isEmpty())
            InsertFieldList += ",";

        InsertFieldList += fld;
    }

    //Замена автоинкремента родительской талицы
    int foreignIndex = getForeignReplaceField(joinTable, rec);
    if (foreignIndex != -1)
        rec.values[foreignIndex] = buildVariableName(joinTable->getParentJoin()->parent->datTable);

    replaceForeignAutoinc(joinTable, rec);

    dateSpelling(joinTable, rec);
    stringSpelling(joinTable, rec);

    sql.append(QString(Padding(1) + "BEGIN"));
    if (variable != "")
    {
        int autoIncIndex = getAutoincIndex(joinTable->datTable);
        QString autoIncField = joinTable->datTable->fields[autoIncIndex].name;

        if (autoIncIndex != -1)
            rec.values[autoIncIndex] = variable;

        sql.append(QString(Padding(2) + "SELECT NVL(MAX(%1), 0) + 1 INTO %3 FROM %2;")
                   .arg(autoIncField)
                   .arg(joinTable->datTable->name)
                   .arg(variable));

        QString values = rec.values.join(", ");
        sql.append(QString(Padding(2) + "INSERT INTO %1(%3) VALUES (%2);")
                   .arg(joinTable->datTable->name,
                        values,
                        InsertFieldList));
    }
    else
    {
        QString values = rec.values.join(", ");
        sql.append(QString(Padding(2) + "INSERT INTO %1(%3) VALUES (%2);")
                   .arg(joinTable->datTable->name,
                        values,
                        InsertFieldList));
    }

    QString exception = _dbSpelling->getExceptionName(DbSpelling::ExceptDupValOnIndex);
    sql.append(QString(Padding(1) + QString("EXCEPTION WHEN %1 THEN NULL;")
                       .arg(exception)));
    sql.append(QString(Padding(1) + "END;"));

    return ++recIndex;
}

int SqlScriptMain::buildDeleteStatement(QTextStream &os, const JoinTable *joinTable, QStringList &sql, int recIndex)
{
    DatRecord rec = joinTable->datTable->records[recIndex];

    replaceForeignAutoinc(joinTable, rec);

    QStringList where = buildWhere(joinTable, rec);

    //Проверка наличия автоинкрементного поля, чтобы потом сохранить его в переменную sql
    QString variable = buildVariableName(joinTable->datTable);
    if (variable != "")
    {
        //Получение имени автоинкрементного поля
        int autoIncIndex = getAutoincIndex(joinTable->datTable);
        QString autoIncField = joinTable->datTable->fields[autoIncIndex].name;

        sql.append(QString(PADDING + "DELETE FROM %1 WHERE %2 RETURNING %3 INTO %4;").arg(joinTable->datTable->name,  where.join(" AND "), autoIncField, variable));
    }
    else
    {
        sql.append(QString(PADDING + "DELETE FROM %1 WHERE %2;").arg(joinTable->datTable->name,  where.join(" AND ")));
    }
    return ++recIndex;
}


int SqlScriptMain::buildUpdateStatement(QTextStream &os, const JoinTable *joinTable, QStringList &sql, int oldIndex, int newIndex)
{
    DatRecord oldRec = joinTable->datTable->records[oldIndex];
    DatRecord newRec = joinTable->datTable->records[newIndex];

    //Заполнение SET
    QStringList setList;

    //Изменёные значения
    QVector<int> modifiedValues;
    for (int i = 0; i < oldRec.values.count(); ++i)
    {
        if (oldRec.values[i] != newRec.values[i])
            modifiedValues.append(i);
    }

    replaceForeignAutoinc(joinTable, newRec);
    dateSpelling(joinTable, newRec);
    stringSpelling(joinTable, newRec);

    for (int i: modifiedValues)
        setList.append(joinTable->datTable->realFields[i] + " = " + newRec.values[i]);

    QStringList where = buildWhere(joinTable, oldRec);

    //Проверка наличия автоинкрементного поля, чтобы потом сохранить его в переменную sql
    QString variable = buildVariableName(joinTable->datTable);
    if (variable != "")
    {
        //Получение имени автоинкрементного поля
        int autoIncIndex = getAutoincIndex(joinTable->datTable);
        QString autoIncField = joinTable->datTable->fields[autoIncIndex].name;

        sql.append(QString(PADDING + "UPDATE %1 SET %2 WHERE %3 RETURNING %4 INTO %5;")
                   .arg(joinTable->datTable->name,
                        setList.join(", "),
                        where.join(" AND "),
                        autoIncField,
                        variable));
    }
    else
    {
        sql.append(QString(PADDING + "UPDATE %1 SET %2 WHERE %3;")
                   .arg(joinTable->datTable->name,
                        setList.join(", "),
                        where.join(" AND ")));
    }
    return ++oldIndex;
}

int SqlScriptMain::buildStatement(QTextStream& os, const JoinTable* joinTable,
                                  QStringList& sql,
                                  int recIndex,
                                  Join* childJoin,
                                  const QStringList &ParentValuesByIndex)
{
    int nextRecIndex = recIndex + 1;
    if (recIndex >= joinTable->datTable->records.count())
    {
        qWarning() << "Table name = " << joinTable->datTable->name << ", recIndex = " << recIndex << ", records.count() = " << joinTable->datTable->records.count() << Qt::endl;
        return nextRecIndex;
    }

    DatRecord& rec = joinTable->datTable->records[recIndex];
    if (rec.lineType == ltInsert)
    {
        qCInfo(logSqlScriptMain) << "Build script for insert. Table " << joinTable->datTable->name << ", record index" << recIndex;

        buildInsertStatement(os, joinTable, sql, recIndex);
        buildChildStatement(os, joinTable, sql, recIndex);
    }
    else if (rec.lineType == ltDelete)
    {
        qCInfo(logSqlScriptMain) << "Build script for delete. Table " << joinTable->datTable->name << ", record index" << recIndex;
        buildDeleteStatement(os, joinTable, sql, recIndex);
        buildChildStatement(os, joinTable, sql, recIndex);
    }
    else if (rec.lineType == ltUpdate)
    {
        if (!childJoin)
        {
            for (nextRecIndex = recIndex; nextRecIndex < joinTable->datTable->records.count(); ++nextRecIndex)
            {
                const DatRecord& upd_rec = joinTable->datTable->records[nextRecIndex];
                if (joinTable->datTable->records[nextRecIndex].lineType != ltUpdate)
                    break;
            }

            int updCnt = (nextRecIndex - recIndex);
            int mid = recIndex + updCnt / 2;

            for (int oldRec = recIndex; oldRec < mid; ++oldRec)
            {
                int newRec = oldRec + updCnt / 2;
                qCInfo(logSqlScriptMain) << "Build script for update. Table " << joinTable->datTable->name << ", record index" << recIndex;
                buildUpdateStatement(os, joinTable, sql, oldRec, newRec);
                buildChildStatement(os, joinTable, sql, newRec);
            }
        }
        else
        {
            int OldIndex = -1;
            int NewIndex = -1;

            for (nextRecIndex = recIndex; nextRecIndex < joinTable->datTable->records.count(); ++nextRecIndex)
            {
                const DatRecord& upd_rec = joinTable->datTable->records[nextRecIndex];

                QStringList ThisValuesByIndex = childJoin->getValuesByIndex(childJoin->parentForeignFields, upd_rec);

                if (ThisValuesByIndex == ParentValuesByIndex)
                {
                    if (upd_rec.lineType == ltUpdate && upd_rec.lineUpdateType == lutOld)
                        OldIndex = nextRecIndex;

                    if (upd_rec.lineType == ltUpdate && upd_rec.lineUpdateType == lutNew)
                        NewIndex = nextRecIndex;

                    if (OldIndex != -1 && NewIndex != -1)
                    {
                        qCInfo(logSqlScriptMain) << "Build script for update. Table " << joinTable->datTable->name << ", record index" << recIndex;
                        buildUpdateStatement(os, joinTable, sql, OldIndex, NewIndex);
                        buildChildStatement(os, joinTable, sql, NewIndex);

                        OldIndex = -1;
                        NewIndex = -1;
                    }

                    if (upd_rec.lineType != ltUpdate)
                    {
                        OldIndex = -1;
                        NewIndex = -1;
                    }
                }
            }
        }


    }

    return nextRecIndex;
}

int SqlScriptMain::buildChildStatement(QTextStream &os, const JoinTable *parentJoinTable, QStringList &sql, int recIndex)
{
    int nextRecIndex = recIndex + 1;

    //Есть дочерние таблицы
    for (Join* childJoin: parentJoinTable->joinList)
    {
        //Нужен только Join для связи с дочерней таблицей.
        if (childJoin->parent->datTable->name.toLower() != parentJoinTable->datTable->name.toLower())
            continue;
        qCInfo(logSqlScriptMain) << "There is a child join" << childJoin->parent->datTable->name  << childJoin->child->datTable->name;

        //Есть связанные записи
        JoinIndex::iterator it = childJoin->indexUpToDown.find(recIndex);
        if (it == childJoin->indexUpToDown.end())
            continue;

        qCInfo(logSqlScriptMain) << "There are child records" << childJoin->parent->datTable->name  << childJoin->child->datTable->name;

        QStringList ParentValuesByIndex = childJoin->getValuesByIndex(childJoin->parentForeignFields, childJoin->parent->datTable->records[recIndex]);
        //Обрадотка дочерних записей
        qCInfo(logSqlScriptMain)
                << "Parent primary keys =" << ParentValuesByIndex
                << ". Child records count =" << childJoin->indexUpToDown[recIndex].count();
        for (int childIndex: childJoin->indexUpToDown[recIndex])
            buildStatement(os, childJoin->child, sql, childIndex, childJoin, ParentValuesByIndex);
    }
    return nextRecIndex;
}

const JoinTable* getParent(JoinTable* joinTable)
{
    for (const Join* join: joinTable->joinList)
        if (join->child->datTable->name.toLower() == joinTable->datTable->name.toLower())
            return join->parent;
}

void SqlScriptMain::build(QTextStream& os, JoinTable* joinTable)
{
    if (!joinTable)
        return;

    QStringList variables = makeVariables(joinTable);
    QStringList sql;
    sql << _dbSpelling->getDeclare()
        << variables
        << _dbSpelling->getBegin();

    auto DisEnableAutoIncTrigger = [=](DatTable* datTable, QStringList &sql, bool enable = false)
    {
        for (const DatIndex &idx : datTable->indexes)
        {
            if (idx.hasAutoinc())
            {
                if (!enable)
                {
                    sql.append(QString("%1rsb_ainc.disable_trigger('%2');")
                               .arg(Padding(1))
                               .arg(datTable->name.toUpper()));
                }
                else
                {
                    sql.append(QString("%1rsb_ainc.enable_trigger('%2');")
                               .arg(Padding(1))
                               .arg(datTable->name.toUpper()));

                    sql.append(QString("%1rsb_ainc.restore_seq('%2');")
                               .arg(Padding(1))
                               .arg(datTable->name.toUpper()));
                }
                break;
            }
        }
    };

    DisEnableAutoIncTrigger(joinTable->datTable, sql);

    for (const Join *idx : joinTable->joinList)
    {
        DisEnableAutoIncTrigger(idx->child->datTable, sql);
        sql.append(QString());
    }

    for (int recno = 0; recno < joinTable->datTable->records.count(); )
    {
        recno = buildStatement(os, joinTable, sql, recno);
        sql.append(QString());
    }

    DisEnableAutoIncTrigger(joinTable->datTable, sql, true);

    for (const Join *idx : joinTable->joinList)
        DisEnableAutoIncTrigger(idx->child->datTable, sql, true);

    sql << _dbSpelling->getEnd() << "";
    os << sql.join("\n");
}

void SqlScriptMain::dateSpelling(const JoinTable *joinTable, DatRecord &rec)
{
    for (int i = 0; i < joinTable->datTable->realFields.count(); ++i)
    {
        DiffField fld = joinTable->datTable->field(joinTable->datTable->realFields[i]);
        qCInfo(logSqlScriptMain) << fld.name << joinTable->datTable->fields[i].typeName;

        if (fld.isDate())
            rec.values[i] = _dbSpelling->toDate(rec.values[i]);
    }
}

void SqlScriptMain::stringSpelling(const JoinTable *joinTable, DatRecord &rec)
{
    for (int i = 0; i < joinTable->datTable->realFields.count(); ++i)
    {
        DiffField fld = joinTable->datTable->field(joinTable->datTable->realFields[i]);
        qint16 type = fld.type;

        if (type == fmtt_STRING || type == fmtt_SNR)
        {
            if (rec.values[i].isEmpty() || rec.values[i].at(0) == QChar(1))
            {
                rec.values[i] = QString("%1(1)")
                        .arg(_dbSpelling->chr());
            }
        }
        else if (type == fmtt_CHR || type == fmtt_UCHR)
        {
            if (rec.values[i].isEmpty() || rec.values[i].at(0) == QChar(0) || rec.values[i].at(0) == QChar(2))
            {
                rec.values[i] = QString("%1(0)")
                        .arg(_dbSpelling->chr());
            }
            else if (rec.values[i].size() == 3 && rec.values[i].at(1) == QChar(2))
            {
                rec.values[i] = QString("%1(0)")
                        .arg(_dbSpelling->chr());
            }
        }
        else if (isBlobType(fld.typeName))
        {
            rec.values[i] = _dbSpelling->toBlob(rec.values[i]);
        }
    }
}
