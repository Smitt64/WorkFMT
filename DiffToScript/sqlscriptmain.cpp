#include "sqlscriptmain.h"
#include "difflogging.h"

const QString PADDING = "  ";

SqlScriptMain::SqlScriptMain(QScopedPointer<DbSpelling>& dbSpelling)
{
    _dbSpelling.swap(dbSpelling);
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
            if (joinTable->datTable->fields[i].name.toLower() == keyField.toLower())
                keyFieldIndexes.append(i);
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

    if (keyFieldIndexes.count() == 0)
        keyFieldIndexes = indexesOfUniqueIndex(joinTable);

    for (int i = 0; i < keyFieldIndexes.count(); ++i)
        where.append(joinTable->datTable->fields[keyFieldIndexes[i]].name + " = " + rec.values[keyFieldIndexes[i]]);

    return where;
}

int SqlScriptMain::buildInsertStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex)
{
    DatRecord rec = joinTable->datTable->records[recIndex];

    //Определение полей для замены

    //Определение автоинкрементного поля и замена на ноль
    int autoIncIndex = getAutoincIndex(joinTable->datTable);
    if (autoIncIndex != -1)
        rec.values[autoIncIndex] = "0";

    //QString autoIncField = joinTable->datTable->fields[autoIncIndex].name;

    //Замена автоинкремента родительской талицы
    int foreignIndex = getForeignReplaceField(joinTable, rec);
    if (foreignIndex != -1)
        rec.values[foreignIndex] = buildVariableName(joinTable->getParentJoin()->parent->datTable);

    replaceForeignAutoinc(joinTable, rec);

    dateSpelling(joinTable, rec);

    //Проверка индекса на автоинкрементное поле
    QString variable = buildVariableName(joinTable->datTable);
    if (variable != "")
    {
        int autoIncIndex = getAutoincIndex(joinTable->datTable);
        QString autoIncField = joinTable->datTable->fields[autoIncIndex].name;

        QString values = rec.values.join(", ");
        sql.append(QString(PADDING + "INSERT INTO %1 VALUES (%2) RETURNING %3 INTO %4;").arg(joinTable->datTable->name,  values, autoIncField, variable));
    }
    else
    {
        QString values = rec.values.join(", ");
        sql.append(QString(PADDING + "INSERT INTO %1 VALUES (%2);").arg(joinTable->datTable->name,  values));
    }
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
        if (oldRec.values[i] != newRec.values[i])
            modifiedValues.append(i);

    replaceForeignAutoinc(joinTable, newRec);

    for (int i: modifiedValues)
        setList.append(joinTable->datTable->fields[i].name + " = " + newRec.values[i]);

    QStringList where = buildWhere(joinTable, oldRec);

    //Проверка наличия автоинкрементного поля, чтобы потом сохранить его в переменную sql
    QString variable = buildVariableName(joinTable->datTable);
    if (variable != "")
    {
        //Получение имени автоинкрементного поля
        int autoIncIndex = getAutoincIndex(joinTable->datTable);
        QString autoIncField = joinTable->datTable->fields[autoIncIndex].name;

        sql.append(QString(PADDING + "UPDATE %1 SET %2 WHERE %3 RETURNING %4 INTO %5;").arg(joinTable->datTable->name, setList.join(", "), where.join(" AND "), autoIncField, variable));
    }
    else
    {
        sql.append(QString(PADDING + "UPDATE %1 SET %2 WHERE %3;").arg(joinTable->datTable->name, setList.join(", "), where.join(" AND ")));
    }
    return ++oldIndex;
}

int SqlScriptMain::buildStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex)
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
        for (nextRecIndex = recIndex; nextRecIndex < joinTable->datTable->records.count(); ++nextRecIndex)
             if (joinTable->datTable->records[nextRecIndex].lineType != ltUpdate)
                 break;

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

        //Обрадотка дочерних записей
        qCInfo(logSqlScriptMain)
                << "Parent primary keys =" << childJoin->getValuesByIndex(childJoin->parentForeignFields, childJoin->parent->datTable->records[recIndex])
                << ". Child records count =" << childJoin->indexUpToDown[recIndex].count();
        for (int childIndex: childJoin->indexUpToDown[recIndex])
            buildStatement(os, childJoin->child, sql, childIndex);
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
    QStringList variables = makeVariables(joinTable);
    QStringList sql;
    sql << _dbSpelling->getDeclare()
        << variables
        << _dbSpelling->getBegin();

    for (int recno = 0; recno < joinTable->datTable->records.count(); )
    {
        recno = buildStatement(os, joinTable, sql, recno);
    }

    sql << _dbSpelling->getEnd() << "";
    os << sql.join("\n");
}

void SqlScriptMain::dateSpelling(const JoinTable *joinTable, DatRecord &rec)
{
    for (int i = 0; i < joinTable->datTable->fields.count(); ++i)
        if (joinTable->datTable->fields[i].isDate())
            rec.values[i] = _dbSpelling->toDate(rec.values[i]);
}















