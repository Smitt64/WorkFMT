#include <memory>
#include "toolsruntime.h"
#include "join.h"
#include "task.h"
#include "fmttable.h"
#include "scripttable.h"
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
#include <QTemporaryFile>

#include <QTextStream>
#include <QFile>
#include <QApplication>
#include "qloggingcategory.h"

Task::Task(QObject *parent) :
    QObject(parent),
    m_Result(0)
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

void getFmtInfo(QTextStream& os, const ScriptTable& datTable)
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

    for (;it != last && it->lineType == ltUpdate; ++it)
    {
        if (it->lineUpdateType == lutOld)
        {
            DatRecords::iterator next = it + 1;

            if (next != last && next->lineUpdateType == lutNew)
            {
                if (it->values[indexPrimaryKey] != next->values[indexPrimaryKey])
                {
                    it->lineType = ltDelete;
                    it->lineUpdateType = lutOld;

                    next->lineType = ltInsert;
                    next->lineUpdateType = lutNew;
                    it ++;
                }
            }
        }
    }
    /*for(;it != last && it->lineType == ltUpdate; ++it);
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
    }*/

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
    if (!optns[ctoDiffInfoMode].isSet)
        runScriptTask();
    else
        runDiffTask();

    emit finished();
}

int Task::result() const
{
    return m_Result;
}

void Task::runDiffTask()
{
    QString rules = getRules(optns);
    QLoggingCategory::setFilterRules(rules);

    // Определение потоков вывода
    StreamControl sc;
    QTextStream os(sc.makeOutputDevice(optns[ctoOutput].value));
    QTextStream *is = sc.getInput(optns[ctoInput].value);
    is->setCodec("IBM 866");
    os.setCodec("IBM 866");

    // normalize input for new files
    QTemporaryFile tmp;
    if (tmp.open())
    {
        const QString end_pattern = "+BEGINDATA";
        QTextStream tmpStream(&tmp);
        tmpStream.setCodec("IBM 866");

        QString lines = is->readAll();

        int begin_pos = lines.indexOf("+LOAD DATA");
        int end_pos = lines.indexOf(end_pattern);

        if (begin_pos != -1 && end_pos != -1)
        {
            lines = lines.remove(begin_pos, (end_pos - begin_pos) + end_pattern.size());
            tmpStream.seek(0);
            tmpStream << lines;
            qDebug() << tmp.fileName();
            tmp.close();

            is = sc.getInput(tmp.fileName());
            is->setCodec("IBM 866");
        }
        else
        {
            is->seek(0);
            tmp.close();
        }
    }

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

    is->device()->waitForReadyRead(-1);
    while (!is->atEnd())
    {
        linesParser.parseDoc(*is);

        qCInfo(logTask) << "Diff parsed. Lines:"
                        << "insert -" << linesParser.linesCount({ltInsert})
                        << "delete -" << linesParser.linesCount({ltDelete})
                        << "update -" << linesParser.linesCount({ltUpdate});

        is->device()->waitForReadyRead(-1);
    }

    linesParser.serializeLines(os, "json");
}

// --delete --insert --update --cs "CONNSTRING=dsn=THOR_DB12DEV1;user id=SERP_3188;password=SERP_3188" --input diff.txt
void Task::runScriptTask()
{
    QString rules = getRules(optns);
    QLoggingCategory::setFilterRules(rules);

    // Определение потоков вывода
    StreamControl sc;
    QTextStream os(sc.makeOutputDevice(optns[ctoOutput].value));
    QTextStream *is = sc.getInput(optns[ctoInput].value);
    is->setCodec("IBM 866");
    os.setCodec("IBM 866");

    // normalize input for new files
    QTemporaryFile tmp;
    if (tmp.open())
    {
        const QString end_pattern = "+BEGINDATA";
        QTextStream tmpStream(&tmp);
        tmpStream.setCodec("IBM 866");

        QString lines = is->readAll();

        int begin_pos = lines.indexOf("+LOAD DATA");
        int end_pos = lines.indexOf(end_pattern);

        if (begin_pos != -1 && end_pos != -1)
        {
            lines = lines.remove(begin_pos, (end_pos - begin_pos) + end_pattern.size());
            tmpStream.seek(0);
            tmpStream << lines;
            qDebug() << tmp.fileName();
            tmp.close();

            is = sc.getInput(tmp.fileName());
            is->setCodec("IBM 866");
        }
        else
        {
            is->seek(0);
            tmp.close();
        }
    }

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

    QVector<ScriptTable> datTables;
    QVector<TableLinks> tableLinks;

    QSharedPointer<DiffConnection> conn;

    if (optns[ctoConnectionString].isSet)
    {
        conn.reset(new DiffConnection(optns[ctoConnectionString].value,
                                      optns[ctoConnectionUnicode].isSet));
    }
    else
        conn.reset(new DiffConnection());

    if (!conn->isConnected())
        return;

    qCInfo(logTask) << "Connection success. User =" << conn->getUser() << ", dsn =" << conn->getDsn();

    is->device()->waitForReadyRead(-1);
    while (!is->atEnd())
    {
        linesParser.parseDoc(*is);
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

        datTables.append(ScriptTable());

        ScriptTable& datTable= datTables.back();

        FmtTable fmtTable(conn->getConnection());
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

        datTable.parseUpdateRecords(datTable);

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
        QString fileName = toolFullFileNameFromDir(QString("relations/%1.json")
                                                   .arg(datTable.name.toLower()));

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
        is->device()->waitForReadyRead(-1);
    }

    JoinTables joinTables;
    for (int i = 0; i < datTables.count(); ++i)
    {
        joinTables.add(&datTables[i], tableLinks[i]);
        qCInfo(logTask) << "Added " << datTables[i].name << "to JoinTables list.";
    }

    QSharedPointer<DbSpelling> dbSpelling;
    if (optns[ctoPostgres].isSet)
        dbSpelling.reset(new DbSpellingPostgres());
    else
        dbSpelling.reset(new DbSpellingOracle());

    SqlScriptMain ssm(dbSpelling, conn);

    qInfo(logTask) << "Start sql building.";
    m_Result = ssm.build(os, joinTables.getRoot());
}

