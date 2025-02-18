#include "sqlscriptmain.h"
#include "difflogging.h"
#include "fmtcore.h"
#include <cstdio>
#include <QSqlQuery>
#include <QTextCodec>
#include <QTextStream>

const QString PADDING = "  ";

QString Padding(int depth)
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
    variables << buildVariable(joinTable->scriptTable);

    JoinListIterator it(joinTable, joinTable->joinList.firstChild(joinTable));
    for (; it.itChild != joinTable->joinList.end(); it = it.nextChild())
    {
        variables << buildVariable((*it.itChild)->child->scriptTable);
    }

    if (variables.back() == "")
        variables.pop_back();

    return variables;
}

QStringList SqlScriptMain::makeInsertFunctions(JoinTable* joinTable)
{
    QStringList functions;

    if (joinTable->scriptTable->hasInserts())
    {
        functions << buildInsertFunctions(joinTable->scriptTable);
        functions.append(QString());
    }

    JoinListIterator it(joinTable, joinTable->joinList.firstChild(joinTable));
    for (; it.itChild != joinTable->joinList.end(); it = it.nextChild())
    {
        if ((*it.itChild)->child->scriptTable->hasInserts())
        {
            functions << buildInsertFunctions((*it.itChild)->child->scriptTable);
            functions.append(QString());
        }
    }

    return functions;
}

QStringList SqlScriptMain::buildInsertFunctions(const ScriptTable* datTable)
{
    QTextCodec *code = QTextCodec::codecForName("IBM 866");
    QStringList function;

    QString params, returnType, fullFunctionName;
    QString undecorateTable = undecorateTableName(datTable->name).toLower();
    QString variable = buildVariableName(datTable);

    undecorateTable.front() = undecorateTable.front().toUpper();

    QString name = QString("InsertInto%1").arg(undecorateTable);
    int autoIncIndex = getAutoincIndex(datTable);

    if (autoIncIndex != -1)
    {
        QString autoIncField = datTable->fields[autoIncIndex].name;

        returnType = QString("%1.%2\%TYPE")
                .arg(datTable->name.toUpper())
                .arg(autoIncField);
    }

    QString InsertFieldList;
    for (const QString &fld : datTable->realFields)
    {
        if (!InsertFieldList.isEmpty())
            InsertFieldList += ",";

        InsertFieldList += fld.toUpper();
    }

    QString missingFldInDatValues;
    DiffFields missingFldInDat = datTable->missingFldInDat();
    for (DiffFields::iterator iter = missingFldInDat.begin(); iter != missingFldInDat.end(); ++iter)
    {
        InsertFieldList += ", ";
        missingFldInDatValues += ", ";

        InsertFieldList += (*iter).name.toUpper();

        missingFldInDatValues += _dbSpelling->getDefaultValueForType(iter->type, iter->size);
    }

    DatRecord rec = datTable->records[0];
    for (int i = 0; i < datTable->realFields.size(); i++)
    {
        if (i != autoIncIndex)
        {
            QString undecorateFldName = datTable->realFields[i];
            if (undecorateFldName.mid(0, 2).toLower() == "t_")
                undecorateFldName = undecorateFldName.mid(2);

            if (!params.isEmpty())
                params += ", ";

            QString paramName = QString("p_%1")
                    .arg(undecorateFldName);

            QString paramType = _dbSpelling->functionParamType(datTable->field(datTable->realFields[i]).type);

            if (datTable->fields[i].isBlob())
                paramType = _dbSpelling->blobTypeName(datTable->field(datTable->realFields[i]).type);

            params += QString("%1 IN %2")
                    .arg(paramName)
                    .arg(paramType);

            rec.values[i] = paramName;
        }
    }

    QStringList BeginCreateReplace;
    QStringList EndCreateReplace;

    _dbSpelling->functionChunks(BeginCreateReplace, EndCreateReplace,
                                name, params, returnType);

    fullFunctionName = QString(BeginCreateReplace.first())
            .remove("CREATE OR REPLACE ");
    function.append(BeginCreateReplace);

    if (autoIncIndex != -1)
    {
        QString variableName = buildVariable(datTable);
        function.append(variableName);
    }

    function.append("BEGIN");

    if (autoIncIndex != -1)
    {
        rec.values[autoIncIndex] = variable;

        QString autoIncField = datTable->fields[autoIncIndex].name;
        function.append(QString(Padding(1) + "SELECT %4(MAX(%1), 0) + 1 INTO %3 FROM %2;")
                        .arg(autoIncField)
                        .arg(datTable->name.toUpper())
                        .arg(variable)
                        .arg(_dbSpelling->nvl()));

        function.append(QString());

        QString values = rec.values.join(", ");
        function.append(QString(Padding(1) + "INSERT INTO %1(%3) VALUES (%2);")
                        .arg(datTable->name,
                             values + missingFldInDatValues,
                             InsertFieldList));
    }
    else
    {
        QString values = rec.values.join(", ");
        function.append(QString(Padding(2) + "INSERT INTO %1(%3) VALUES (%2);")
                        .arg(datTable->name.toUpper(),
                             values + missingFldInDatValues,
                             InsertFieldList));
    }

    if (autoIncIndex != -1)
    {
        QString variableName = buildVariableName(datTable);
        function.append(QString());
        function.append(Padding(1) + QString("RETURN %1;").arg(variableName));
    }

    QString exception = _dbSpelling->getExceptionName(DbSpelling::ExceptDupValOnIndex);

    //QStringList ParentValuesByIndex = childJoin->getValuesByIndex(childJoin->parentForeignFields, childJoin->parent->datTable->records[recIndex]);
    function.append(QString("EXCEPTION "));
    QString tmpException = QString("dbms_output.put_line('%WARNING%: Ошибка вставки в таблицу %1: ' || ErrorText)")
            .arg(datTable->name.toUpper());
    QString rusException = tmpException;//sqlerrm
    QString exceptionInfo;

    DatIndex Unique;
    bool hasUnique = datTable->firstUniq(Unique, true);

    if (hasUnique)
    {
        QString fldNames, fldValues;

        for (const IndexField &fld : qAsConst(Unique.fields))
        {
            if (!fldNames.isEmpty())
            {
                fldNames += ",";
                fldValues += " || ', ' || ";
            }

            fldNames += fld.name;

            int realfld = datTable->realFields.indexOf(fld.name.toUpper());
            fldValues += rec.values[realfld];
        }

        exceptionInfo = QString("dbms_output.put_line('(%1) = (' || %2 || ')');")
                .arg(fldNames)
                .arg(fldValues);
    }

    if (autoIncIndex == -1)
    {
        function.append(QString("WHEN %1 THEN")
                        .arg(exception));

        function.append(_dbSpelling->getExceptionInfo("ErrorText"));
        function.append(Padding(1) + _dbSpelling->callProcedure(rusException));

        if (!exceptionInfo.isEmpty())
            function.append(Padding(1) + _dbSpelling->callProcedure(exceptionInfo));

        function.append(QString("WHEN OTHERS THEN"));
        function.append(_dbSpelling->getExceptionInfo("ErrorText"));
        function.append(Padding(1) + _dbSpelling->callProcedure(rusException));

        if (!exceptionInfo.isEmpty())
            function.append(Padding(1) + _dbSpelling->callProcedure(exceptionInfo));
    }
    else
    {
        function.append(QString("WHEN %1 THEN ")
                        .arg(exception));

        function.append(_dbSpelling->getExceptionInfo("ErrorText"));
        function.append(Padding(1) + _dbSpelling->callProcedure(rusException));

        if (!exceptionInfo.isEmpty())
            function.append(Padding(1) + _dbSpelling->callProcedure(exceptionInfo));

        function.append(Padding(1) + "RETURN -1;");

        function.append(QString("WHEN OTHERS THEN"));
        function.append(_dbSpelling->getExceptionInfo("ErrorText"));
        function.append(Padding(1) + _dbSpelling->callProcedure(rusException));

        if (!exceptionInfo.isEmpty())
            function.append(Padding(1) + _dbSpelling->callProcedure(exceptionInfo));

        function.append(Padding(1) + "RETURN -1;");
    }
    function.append(EndCreateReplace);

    m_InsertFunctions[datTable->name.toUpper()] = { name, returnType, fullFunctionName };

    return function;
}

QString SqlScriptMain::undecorateTableName(const QString &name) const
{
    QRegularExpression re("[d|D](\\w+)\\_\\w{3}");
    QRegularExpressionMatch match = re.match(name);

    if (match.hasMatch())
        return match.captured(1);

    return name;
}


int SqlScriptMain::getAutoincIndex(const ScriptTable* datTable)
{
    for (int i = 0; i < datTable->fields.count(); ++i)
        if (datTable->fields[i].isAutoinc)
            return i;
    return -1;
}

//Если автоинкремениа нет, то вернёт пустую строку.
QString SqlScriptMain::buildVariableName(const ScriptTable* datTable)
{
    int autoIncIndex = getAutoincIndex(datTable);
    if (autoIncIndex == -1)
        return "";

    QString autoIncField = datTable->fields[autoIncIndex].name;
    QString variableName = QString("v_%1__%2").arg(datTable->name, autoIncField);
    return variableName;
}

//Если автоинкремениа нет, то вернёт пустую строку.
QString SqlScriptMain::buildVariable(const ScriptTable* datTable)
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
    for (int i = 0; i < joinTable->scriptTable->fields.count(); ++i)
    {
        for (const Field& keyField: joinTable->keyFields)
        {
            if (joinTable->scriptTable->fields[i].name.toLower() == keyField.toLower())
            {
                if (!keyFieldIndexes.contains(i))
                    keyFieldIndexes.append(joinTable->scriptTable->realFields.indexOf(keyField.toUpper()));
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
    for (const DatIndex& index: joinTable->scriptTable->indexes)
    {
        if (index.isUnique && !index.hasAutoinc())
        {
            for (const IndexField& field: index.fields)
            {
                int indx = joinTable->scriptTable->fields.indexByFieldName(field.name);
                if (indx != -1)
                    keyFieldIndexes.append(indx);
            }
        }
    }

    return keyFieldIndexes;

}

//Заменяет родительский автоинкремент на имя переменной родительской таблицы.
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
        rec.values[parentJoin->childForeignFields[foreignIndex]] = buildVariableName(parentJoin->parent->scriptTable);
    }
}

int SqlScriptMain::indexForeignAutoinc(const JoinTable *joinTable, const DatRecord &rec)
{
    //Связь с родительской таблицей
    Join* parentJoin = joinTable->getParentJoin();

    //Проверка индекса связи на автоинкрементное поле
    if (parentJoin != nullptr)
    {
        int parentAutoInc = getAutoincIndex(parentJoin->parent->scriptTable);

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
        int parentAutoInc = getAutoincIndex(parentJoin->parent->scriptTable);

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
    foreach(const DiffField &uf, joinTable->scriptTable->uniqFields)
    {
        where.append(uf.name + " = " + rec.values[joinTable->scriptTable->fields.indexByFieldName(uf.name)]);
    }

    return where;
}

int SqlScriptMain::buildInsertStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex)
{
    DatRecord rec = joinTable->scriptTable->records[recIndex];

    //Определение полей для замены

    //Определение автоинкрементного поля и замена на ноль
    int autoIncIndex = getAutoincIndex(joinTable->scriptTable);
    if (autoIncIndex != -1)
        rec.values[autoIncIndex] = "0";

    QString InsertFieldList;
    for (const QString &fld : joinTable->scriptTable->realFields)
    {
        if (!InsertFieldList.isEmpty())
            InsertFieldList += ",";

        InsertFieldList += fld;
    }

    replaceForeignAutoinc(joinTable, rec);

    dateSpelling(joinTable, rec);
    stringSpelling(joinTable, rec);

    //Проверка индекса на автоинкрементное поле
    QString variable = buildVariableName(joinTable->scriptTable);
    QStringList fields;
    for (int i = 0; i < joinTable->scriptTable->fields.count(); ++i)
        fields.append(joinTable->scriptTable->fields[i].name);

    QString values = rec.values.join(", ");
    QString names = fields.join(", ");

    if (variable != "")
    {
        int autoIncIndex = getAutoincIndex(joinTable->scriptTable);
        QString autoIncField = joinTable->scriptTable->fields[autoIncIndex].name;
        //sql.append(QString(PADDING + "INSERT INTO %1 (%2) VALUES (%3) RETURNING %4 INTO %5;").arg(joinTable->scriptTable->name, names, values, autoIncField, variable));
        QString call = QString("%1 := %2 (%3);")
                .arg(variable)
                .arg(m_InsertFunctions[joinTable->scriptTable->name.toUpper()].name)
                .arg(values);

        sql.append(Padding(1) + call);
    }
    else
    {
        //sql.append(QString(PADDING + "INSERT INTO %1 (%2) VALUES (%3);").arg(joinTable->scriptTable->name,  names, values));
        QString values = rec.values.join(", ");
        QString call = QString("%1 (%2);")
                .arg(m_InsertFunctions[joinTable->scriptTable->name.toUpper()].name)
                .arg(values);

        sql.append(Padding(1) + _dbSpelling->callProcedure(call));
    }

    return ++recIndex;
}

int SqlScriptMain::buildDeleteStatement(QTextStream &os, const JoinTable *joinTable, QStringList &sql, int recIndex)
{
    DatRecord rec = joinTable->scriptTable->records[recIndex];

    replaceForeignAutoinc(joinTable, rec);

    QStringList where = buildWhere(joinTable, rec);

    //Проверка наличия автоинкрементного поля, чтобы потом сохранить его в переменную sql
    QString variable = buildVariableName(joinTable->scriptTable);
    if (variable != "")
    {
        //Получение имени автоинкрементного поля
        int autoIncIndex = getAutoincIndex(joinTable->scriptTable);
        QString autoIncField = joinTable->scriptTable->fields[autoIncIndex].name;

        sql.append(QString(PADDING + "DELETE FROM %1 WHERE %2 RETURNING %3 INTO %4;").arg(joinTable->scriptTable->name,  where.join(" AND "), autoIncField, variable));
    }
    else
    {
        sql.append(QString(PADDING + "DELETE FROM %1 WHERE %2;").arg(joinTable->scriptTable->name,  where.join(" AND ")));
    }
    return ++recIndex;
}


int SqlScriptMain::buildUpdateStatement(QTextStream &os, const JoinTable *joinTable, QStringList &sql, int oldIndex, int newIndex)
{
    DatRecord oldRec = joinTable->scriptTable->records[oldIndex];
    DatRecord newRec = joinTable->scriptTable->records[newIndex];

    //Заполнение SET
    QStringList setList;

    //Изменёные значения
    QVector<int> modifiedValues;
    for (int i = 0; i < oldRec.values.count(); ++i)
        if (oldRec.values[i].compare(newRec.values[i]) != 0)
        {
            qCInfo(logSqlScriptMain) << oldRec.values[i] << "\" != \"" << newRec.values[i];
            modifiedValues.append(i);
        }

    qCInfo(logSqlScriptMain) << "Modifed fields count =" << modifiedValues.count();

    replaceForeignAutoinc(joinTable, newRec);
    dateSpelling(joinTable, newRec);
    stringSpelling(joinTable, newRec);

    for (int i: modifiedValues)
        setList.append(joinTable->scriptTable->realFields[i] + " = " + newRec.values[i]);

    QStringList where = buildWhere(joinTable, oldRec);

    //Проверка наличия автоинкрементного поля, чтобы потом сохранить его в переменную sql
    QString variable = buildVariableName(joinTable->scriptTable);
    if (!setList.isEmpty())
    {
        if (variable != "")
        {
            //Получение имени автоинкрементного поля
            int autoIncIndex = getAutoincIndex(joinTable->scriptTable);
            QString autoIncField = joinTable->scriptTable->fields[autoIncIndex].name;

            sql.append(QString(PADDING + "UPDATE %1 SET %2 WHERE %3 RETURNING %4 INTO %5;")
                       .arg(joinTable->scriptTable->name,
                            setList.join(", "),
                            where.join(" AND "),
                            autoIncField,
                            variable));

            //sql.append(QString(PADDING + "UPDATE %1 SET %2 WHERE %3 RETURNING %4 INTO %5;").arg(joinTable->scriptTable->name, setList.join(", "), where.join(" AND "), autoIncField, variable));
        }
        else
        {
            sql.append(QString(PADDING + "UPDATE %1 SET %2 WHERE %3;")
                       .arg(joinTable->scriptTable->name,
                            setList.join(", "),
                            where.join(" AND ")));
        }
        //sql.append(QString(PADDING + "UPDATE %1 SET %2 WHERE %3;").arg(joinTable->scriptTable->name, setList.join(", "), where.join(" AND ")));
    }
    return ++oldIndex;
}

int SqlScriptMain::buildStatement(QTextStream& os, JoinTable* joinTable,
                                  QStringList& sql,
                                  int recIndex,
                                  Join* childJoin,
                                  const QStringList &ParentValuesByIndex)
{
    int nextRecIndex = recIndex + 1;

    // Индекс не выходит за диапазон записей таблицы
    if (recIndex >= joinTable->scriptTable->records.count())
    {
        qWarning() << "Table name = " << joinTable->scriptTable->name << ", recIndex = " << recIndex << ", records.count() = " << joinTable->scriptTable->records.count() << Qt::endl;
        return nextRecIndex;
    }

    DatRecord& rec = joinTable->scriptTable->records[recIndex];
    toScript(joinTable, rec);
    if (rec.lineType == ltInsert)
    {
        qCInfo(logSqlScriptMain) << "Build script for insert. Table " << joinTable->scriptTable->name << ", record index" << recIndex;

        QStringList childsql;
        buildInsertStatement(os, joinTable, sql, recIndex);
        buildChildStatement(os, joinTable, sql, recIndex);
        joinTable->processedRecords[recIndex] = true;
        int autoIncIndex = getAutoincIndex(joinTable->scriptTable);
        if (autoIncIndex == -1)
            sql.append(childsql);
        else
        {
            QString variable = buildVariableName(joinTable->scriptTable);
            sql.append(Padding(1) + QString("IF %1 <> -1 THEN")
                       .arg(variable));

            for (const QString &s : childsql)
                sql.append(Padding(1) + s);

            sql.append(Padding(1) + "END IF;");
        }
    }
    else if (rec.lineType == ltDelete)
    {
        qCInfo(logSqlScriptMain) << "Build script for delete. Table " << joinTable->scriptTable->name << ", record index" << recIndex;
        buildDeleteStatement(os, joinTable, sql, recIndex);
        buildChildStatement(os, joinTable, sql, recIndex);
        joinTable->processedRecords[recIndex] = true;
    }
    else if (rec.lineType == ltUpdate)
    {
        // определение последней записи для обновления
        int newIndex, oldIndex;
        if(joinTable->scriptTable->records[recIndex].lineUpdateType == lutOld)
        {
            oldIndex = recIndex;
            newIndex = nextRecIndex;
        }
        else
        {
            oldIndex = nextRecIndex;
            newIndex = recIndex;
        }
        qCInfo(logSqlScriptMain) << "Build script for update. Table " << joinTable->scriptTable->name << ", record index" << recIndex;

        if (newIndex < joinTable->scriptTable->records.size())
        {
            toScript(joinTable, joinTable->scriptTable->records[newIndex]);
            buildUpdateStatement(os, joinTable, sql, oldIndex, newIndex);
            buildChildStatement(os, joinTable, sql, newIndex);
            joinTable->processedRecords[oldIndex] = true;
            joinTable->processedRecords[newIndex] = true;
        }
        else
            qCWarning(logSqlScriptMain) << "Can't " << recIndex << ". Record not processed: " << rec.values.join(", ");

        nextRecIndex++;
    }
    else
    {
        qCWarning(logSqlScriptMain) << "Record index" << recIndex << ". Record not processed: " << rec.values.join(", ");
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
        if (childJoin->parent->scriptTable->name.toLower() != parentJoinTable->scriptTable->name.toLower())
            continue;
        qCInfo(logSqlScriptMain) << "There is a child join" << childJoin->parent->scriptTable->name  << childJoin->child->scriptTable->name;

        //Есть связанные записи
        JoinIndex::iterator it = childJoin->indexUpToDown.find(recIndex);
        if (it == childJoin->indexUpToDown.end())
            continue;

        qCInfo(logSqlScriptMain) << "There are child records" << childJoin->parent->scriptTable->name  << childJoin->child->scriptTable->name;

        QStringList ParentValuesByIndex = childJoin->getValuesByIndex(childJoin->parentForeignFields, childJoin->parent->scriptTable->records[recIndex]);
        //Обрадотка дочерних записей
        qCInfo(logSqlScriptMain)
                << "Parent primary keys =" << childJoin->getValuesByIndex(childJoin->parentForeignFields, childJoin->parent->scriptTable->records[recIndex])
                << ". Child records count =" << childJoin->indexUpToDown[recIndex].count();
        for (int childIndex: childJoin->indexUpToDown[recIndex])
            buildStatement(os, childJoin->child, sql, childIndex, childJoin, ParentValuesByIndex);
    }
    return nextRecIndex;
}

const JoinTable* getParent(JoinTable* joinTable)
{
    for (const Join* join: qAsConst(joinTable->joinList))
        if (join->child->scriptTable->name.toLower() == joinTable->scriptTable->name.toLower())
            return join->parent;
}

void SqlScriptMain::checkDatFldsCount(QStringList& sql, JoinTable* joinTable)
{
    auto CheckFlds = [&sql](const ScriptTable &table)
    {
        if (table.realFields.count() != table.fields.count())
        {
            sql.append(QString("WARNING: Для таблицы %1 набор полей в *.dat файле не соответствует набору в fmt словаре.")
                       .arg(table.name.toUpper()));
            sql.append(QString("Необходимо актуализировать *.dat файл"));
        }
    };

    CheckFlds(*joinTable->scriptTable);

    for (Join* childJoin: joinTable->joinList)
        CheckFlds(*childJoin->child->scriptTable);
}

int SqlScriptMain::build(QTextStream& os, JoinTable* joinTable)
{
    int stat = 0;
    if (!joinTable)
        return -1;

    QTextStream StdErr(stderr);
    StdErr.setCodec("IBM 866");

    QStringList variables = makeVariables(joinTable);
    QStringList sql;

    QStringList errors;
    checkDatFldsCount(errors, joinTable);

    for (const QString &err : errors)
    {
        sql.append(QString("-- %1").arg(err));
        StdErr << err << Qt::endl;
        stat = SqlBuildError_FieldComposition;
    }

    if (_dbSpelling->functionDeclare() == DbSpelling::FunctionBeforeBlocks)
        sql << makeInsertFunctions(joinTable);

    sql << _dbSpelling->getDeclare()
        << variables;

    auto AddPadding = [=](QString &str) -> void
    {
        str = Padding(1) + str;
    };

    if (_dbSpelling->functionDeclare() == DbSpelling::FunctionInDeclareBlock)
    {
        QStringList funcs = makeInsertFunctions(joinTable);
        std::for_each(funcs.begin(), funcs.end(), AddPadding);
        sql << QString() << funcs;
    }

    sql << _dbSpelling->getBegin();

    if (_dbSpelling->functionDeclare() == DbSpelling::FunctionInBeginBlock)
    {
        QStringList funcs = makeInsertFunctions(joinTable);
        std::for_each(funcs.begin(), funcs.end(), AddPadding);
        sql << funcs;
    }

    auto DisEnableAutoIncTrigger = [=](ScriptTable* datTable, QStringList &sql, bool enable = false)
    {
        for (const DatIndex &idx : qAsConst(datTable->indexes))
        {
            if (idx.hasAutoinc())
            {
                if (!enable)
                {
                    QString call = QString("rsb_ainc.disable_trigger('%1');")
                            .arg(datTable->name.toUpper());

                    sql.append(Padding(1) + _dbSpelling->callProcedure(call));
                }
                else
                {
                    QString call = QString("rsb_ainc.enable_trigger('%1');")
                            .arg(datTable->name.toUpper());

                    sql.append(Padding(1) + _dbSpelling->callProcedure(call));

                    call = QString("rsb_ainc.restore_seq('%1');")
                            .arg(datTable->name.toUpper());
                    sql.append(Padding(1) + _dbSpelling->callProcedure(call));
                }
                break;
            }
        }
    };

    DisEnableAutoIncTrigger(joinTable->scriptTable, sql);

    for (const Join *idx : joinTable->joinList)
    {
        DisEnableAutoIncTrigger(idx->child->scriptTable, sql);
        sql.append(QString());
    }


    for (int recno = 0; recno < joinTable->scriptTable->records.count(); )
    {
        int oldSize = sql.size();
        recno = buildStatement(os, joinTable, sql, recno);
        if (sql.size() != oldSize)
            sql.append(QString());
    }

    DisEnableAutoIncTrigger(joinTable->scriptTable, sql, true);

    for (const Join *idx : joinTable->joinList)
        DisEnableAutoIncTrigger(idx->child->scriptTable, sql, true);

    sql << _dbSpelling->getEnd() << "";

    if (_dbSpelling->needDropFunctions())
    {
        QMapIterator<QString, InsertFunction> iter(m_InsertFunctions);
        while (iter.hasNext())
        {
            iter.next();
            sql << _dbSpelling->dropFunction(iter.value().name,
                                             iter.value().fullname,
                                             iter.value().returnType)
                << "/"
                << QString();
        }
    }
    os << sql.join("\n");

    return stat;
}

void SqlScriptMain::dateSpelling(const JoinTable *joinTable, DatRecord &rec)
{
    for (int i = 0; i < joinTable->scriptTable->realFields.count(); ++i)
    {
        DiffField fld = joinTable->scriptTable->field(joinTable->scriptTable->realFields[i]);
        qCInfo(logSqlScriptMain) << fld.name << joinTable->scriptTable->fields[i].typeName;

        if (fld.isDate())
            rec.values[i] = _dbSpelling->toDate(rec.values[i]);
    }
}

void SqlScriptMain::toScript(const JoinTable *joinTable, DatRecord &rec)
{
    for (QString& s: rec.values)
    {
        QString tmp = "'"; tmp += QChar(1); tmp += "'";
        if (s == tmp) {
            s = "CHR(1)";
            continue;
        }
        tmp = "'"; tmp += QChar(2); tmp += "'";
        if (s == tmp) {
            s = "CHR(0)";
            continue;
        }
        if (s == QChar(1)) {
            s = "CHR(1)";
            continue;
        }
        if (s == QChar(2)) {
            s = "CHR(0)";
            continue;
        }
    }
}

void SqlScriptMain::stringSpelling(const JoinTable *joinTable, DatRecord &rec)
{
    for (int i = 0; i < joinTable->scriptTable->realFields.count(); ++i)
    {
        DiffField fld = joinTable->scriptTable->field(joinTable->scriptTable->realFields[i]);
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














