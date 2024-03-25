#include <memory>

#include "join.h"
#include "task.h"
#include "fmttable.h"
#include "dattable.h"
#include "dbspellingoracle.h"
#include "dbspellingpostgres.h"
#include "diffconnection.h"
#include "fmtfield.h"
#include "fmtcore.h"
#include "linesparsermain.h"
#include "streamcontrol.h"
#include "difflogging.h"
#include "fmtindex.h"
#include "fmtsegment.h"
#include "sqlscriptmain.h"

#include <QTextStream>
#include <QFile>
#include <QApplication>
#include "qloggingcategory.h"


Task::Task(QObject *parent) : QObject(parent)
{
    //снятие всех опций
    std::fill(optns.begin(), optns.end() - 1, TaskOption{ false, "" } );
}

void Task::showAppInfo(QTextStream& os) {
    os << "Application enviroment path:" << Qt::endl;
    QStringList pathList = QApplication::libraryPaths();
    for (int i = 0; i < pathList.count(); ++i)
        os << "\t" << pathList[i] << Qt::endl;

    os << "App dir: " << QApplication::applicationDirPath() << Qt::endl;
}

void getFmtInfo(QTextStream& os, const DatTable& datTable)
{
    FmtInit();
    os << "-- Table: " << datTable.name << Qt::endl;

    for (int i = 0; i < datTable.indexes.count(); ++i)
    {
        os << QString("-- index[%1].name = '%2' (%3)").arg(QString::number(i), datTable.indexes[i].name, datTable.indexes[i].isUnique?"unique":"") << Qt::endl;
        for (const IndexField& field: datTable.indexes[i].fields)
            os << "--\t" << field.name << Qt::endl;
    }

    os << "-- Fields:" << Qt::endl;
    for (int i = 0; i < datTable.fields.count(); ++i)
    {
        const DiffField& fld = datTable.fields[i];
        os << QString("--\t%1 %2 %3").arg(
                  QString::number(i + 1).rightJustified(2, '0'),
                  fld.name,
                  fld.typeName
                  );
        if (fld.isAutoinc)
            os << " (autoinc) ";
        os << Qt::endl;
    }    
}

QString getRules(TaskOptions optns)
{
    QStringList rules;

    bool state = optns[ctoVerbose].isSet;
    QString stateStr = state ? "true" : "false";
    rules.append("*.info=" + stateStr);
    rules.append("*.warning=" + stateStr);
    rules.append("*.debug=" + stateStr);

    rules << optns[ctoLog].value.split(",");

    return rules.join("\n");
}

QString getPrimaryKey(const DatIndexes& indexes)
{
    for (const DatIndex& index: indexes)
        if (index.isUnique && index.fields.count() == 1 && index.fields[0].isAutoinc)
            return index.fields[0].name;
    return "";
}

DatRecords::iterator parseUpdateBlock(int indexPrimaryKey, DatRecords::iterator first, DatRecords::iterator last)
{
    DatRecords::iterator it = first;

    for(;it != last && it->lineType == ltUpdate; ++it);
    last = it;

    DatRecords::iterator mid = first + (last - first) / 2;

    for (;mid != last; ++first, ++mid)
    {
        if (first->values[indexPrimaryKey] != mid->values[indexPrimaryKey])
        {
            first->lineType = ltDelete;
            first->lineUpdateType = lutOld;

            mid->lineType = ltInsert;
            mid->lineUpdateType = lutNew;

            qCInfo(logTask) << "Line type changed for primary key " << first->values[indexPrimaryKey];
        }
    }
    return last;
}

void parseUpdateRecords(DatTable& datTable)
{
    QString primaryKeyField = getPrimaryKey(datTable.indexes);
    if (primaryKeyField == "")
        return;

    int indexPrimaryKey = datTable.fields.indexByFieldName(primaryKeyField);
    if (indexPrimaryKey == -1)
        return;

    DatRecords::iterator it = datTable.records.begin();
    while (it != datTable.records.end())
    {
        if (it->lineType != ltUpdate)
            ++it;
        it = parseUpdateBlock(indexPrimaryKey, it, datTable.records.end());
    }
}

void Task::run()
{
    runTask();
    emit finished();
}


void Task::runTask()
{
    QString rules = getRules(optns);
    QLoggingCategory::setFilterRules(rules);

    // Определение потоков вывода
    StreamControl sc;
    QTextStream os(sc.makeOutputDevice(optns[ctoOutput].value));
    QTextStream is(&sc.getInput(optns[ctoInput].value));

//    QTextStream os(stdout);
//    QTextStream is(stdin);

    //Распознование типа строк в исходном файле
    LinesParserMain linesParser;
    linesParser.setTableParser(new LinesTablePareser("Index: "));
    if (optns[ctoInsert].isSet)
    {
        linesParser.setInsertParser(new LinesInsertParser("+"));
        qCInfo(logTask) << "Include insert parser.";
    }
    if (optns[ctoDelete].isSet)
    {
        linesParser.setDeleteParser(new LinesDeleteParser("-"));
        qCInfo(logTask) << "Include delete parser.";
    }
    if (optns[ctoUpdate].isSet)
    {
        linesParser.setUpdateParser(new LinesUpdateParser("-"));
        qCInfo(logTask) << "Include update parser.";
    }

    QString curDir = QFileInfo(QCoreApplication::applicationFilePath()).path();

    QVector<DatTable> datTables;
    QVector<TableLinks> tableLinks;

    DiffConnection conn;
    if (!conn.isConnected())
        return;
    qCInfo(logTask) << "Connection success. User =" << conn.getUser() << ", dsn =" << conn.getDsn();

    while (!is.atEnd())
    {
        linesParser.parseDoc(is);
        qCInfo(logTask) << "Diff parsed. Lines:"
                        << "insert -" << linesParser.linesCount({ltInsert})
                        << "delete -" << linesParser.linesCount({ltDelete})
                        << "update -" << linesParser.linesCount({ltUpdate})
                           ;

        if (linesParser.getLines({ltTable}).count() != 1)
        {
            qWarning(logTask) << "Error. Table name was not parsed.";
            return;
        }

        datTables.append(DatTable());

        DatTable& datTable= datTables.back();

        FmtTable fmtTable(conn.getConnection());
        if (!fmtTable.load(linesParser.getLines({ltTable})[0].toLower()))
        {
            QString tableName = linesParser.getLines({ltTable})[0].toLower();
            qCWarning(logTask) << QString("Error. fmtTable was not loaded. Table name = '%1'").arg(linesParser.getLines({ltTable})[0]);
            return;
        }
        qCInfo(logTask) << "FMT loaded.";

        datTable.loadFromFmt(&fmtTable);
        qCInfo(logTask) << "Fields of" << datTable.name << "loaded. Count =" << datTable.fields.count();

        datTable.loadData(linesParser.getParsedLines());
        qCInfo(logTask) << "Records of" << datTable.name << "loaded. Count =" << datTable.records.count();

        parseUpdateRecords(datTable);
        qCInfo(logTask) << "Parsing update records is done.";

        if (optns[ctoTableInfo].isSet)
            getFmtInfo(os, datTable);

        if (datTable.errorCount() > 0)
        {
            if (!optns[ctoTableInfo].isSet) //чтобы повторно не выводилось
                getFmtInfo(os, datTable);
            for (const QString& err: datTable.getErrors())
                os << err << Qt::endl;
        }

        tableLinks.append(TableLinks());
        TableLinks& tableLink = tableLinks.back();
        QString fileName = curDir + "/" + datTable.name.toLower() + ".json";
        if (QFile::exists(fileName))
        {
            tableLink.loadLinks(fileName);
            qCInfo(logTask) << "Json from" << fileName << "loaded";
        }
        else
        {
            qCWarning(logTask) << "File not exist: " << fileName;
            tableLink.tableName = datTable.name.toLower();
        }
    }

    JoinTables joinTables;
    for (int i = 0; i < datTables.count(); ++i)
    {
        joinTables.add(&datTables[i], tableLinks[i]);
        qCInfo(logTask) << "Added " << datTables[i].name << "to JoinTables list.";
    }

    QScopedPointer<DbSpelling> dbSpelling;
    if (optns[ctoPostgres].isSet)
        dbSpelling.reset(new DbSpellingPostgres());
    else
        dbSpelling.reset(new DbSpellingOracle());

    SqlScriptMain ssm(dbSpelling);

    qInfo(logTask) << "Start sql building.";
    ssm.build(os, joinTables.getRoot());    

}

