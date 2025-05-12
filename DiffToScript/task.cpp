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
#include <QInputDialog>
#include <QTextStream>
#include <QFile>
#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDomDocument>
#include <QDomElement>
#include "qloggingcategory.h"
#include "diffmodeparser.h"
#include "rslobj/tablefilesmodel.h"
#include "rslobj/difftoscripexecutor.h"
#include "rslobj/sqlstringlist.h"
#include <rsscript/registerobjlist.hpp>
#include "rslobj/taskoptionscontainer.h"
#include "rslobj/fmttableslist.h"
#include <QSettings>

QString serializeNormalPathsToJson(const QList<QStringList>& chunks)
{
    QJsonArray jsonArray;
    for (const QStringList& chunk : chunks)
    {
        QJsonArray innerArray;
        for (const QString& str : chunk)
            innerArray.append(str);

        jsonArray.append(innerArray);
    }

    QJsonDocument doc(jsonArray);
    return doc.toJson(QJsonDocument::Compact);
}

QString serializeNormalPathsToXml(const QList<QStringList>& chunks)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("chunks");
    doc.appendChild(root);

    for (const QStringList& chunk : chunks)
    {
        QDomElement chunkElement = doc.createElement("chunk");
        for (const QString& str : chunk)
        {
            QDomElement itemElement = doc.createElement("item");
            QDomText textNode = doc.createTextNode(str);
            itemElement.appendChild(textNode);
            chunkElement.appendChild(itemElement);
        }
        root.appendChild(chunkElement);
    }

    return doc.toString();
}

QSharedPointer<QSettings> diffGetSettings()
{
    QDir settingsDir = QDir(qApp->applicationDirPath());

    QSharedPointer<QSettings> pSettings(new QSettings(settingsDir.absoluteFilePath("difftoscript.ini"), QSettings::IniFormat));
    return pSettings;
}

// ------------------------------------------------------------------

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

void getFmtInfo(QTextStream& os, ScriptTable *datTable)
{
    FmtInit();
    os << "-- Table: " << datTable->name << Qt::endl;

    for (int i = 0; i < datTable->indexes.count(); ++i)
    {
        os << QString("-- index[%1].name = '%2' (%3)").arg(QString::number(i), datTable->indexes[i]->name, datTable->indexes[i]->isUnique?"unique":"") << Qt::endl;
        for (IndexField* field: datTable->indexes[i]->fields)
            os << "--\t" << field->name << Qt::endl;
    }

    os << "-- Fields:" << Qt::endl;
    for (int i = 0; i < datTable->fields.count(); ++i)
    {
        const DiffField* fld = datTable->fields[i];
        os << QString("--\t%1 %2 %3").arg(
                  QString::number(i + 1).rightJustified(2, '0'),
                  fld->name,
                  fld->typeName
                  );

        if (fld->isAutoinc)
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
    for (DatIndex *index: indexes)
    {
        if (index->isUnique && index->fields.count() == 1 && index->fields[0]->isAutoinc)
            return index->fields[0]->name;
    }

    return "";
}

DatRecords::iterator parseUpdateBlock(int indexPrimaryKey, DatRecords::iterator first, DatRecords::iterator last)
{
    DatRecords::iterator it = first;

    for (;it != last && (*it)->lineType == ltUpdate; ++it)
    {
        if ((*it)->lineUpdateType == lutOld)
        {
            DatRecords::iterator next = it + 1;

            if (next != last && (*next)->lineUpdateType == lutNew)
            {
                if ((*it)->values[indexPrimaryKey] != (*next)->values[indexPrimaryKey])
                {
                    (*it)->lineType = ltDelete;
                    (*it)->lineUpdateType = lutOld;

                    (*next)->lineType = ltInsert;
                    (*next)->lineUpdateType = lutNew;
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
        if ((*it)->lineType != ltUpdate)
            ++it;

        it = parseUpdateBlock(indexPrimaryKey, it, datTable.records.end());
    }
}

void Task::run()
{
    if (optns[ctoDiffInfoMode].isSet)
        runDiffTask();
    else if (optns[ctoNormalFileList].isSet)
        runNormalPathsTask();
    else if (optns[ctoUpdateTable].isSet)
        runAlterTableTask();
    else
        runScriptTask();

    emit finished();
}

int Task::result() const
{
    return m_Result;
}

void Task::runAlterTableTask()
{
    QString rules = getRules(optns);
    QLoggingCategory::setFilterRules(rules);

    // Определение потоков вывода
    StreamControl sc;
    QTextStream os(sc.makeOutputDevice(optns[ctoOutput].value));
    QTextStream *is = sc.getInput(optns[ctoInput].value);
    is->setCodec("IBM 866");
    os.setCodec("IBM 866");

    DiffModeParser parser(*is);
    QVector<DiffLine> lines = parser.getResult();

    QByteArray json;
    QTextStream stream(&json);
    stream.setCodec("IBM 866");
    parser.serialize(stream, "json");
    stream.flush();

    generateUpdateScript(json, os);
    os.flush();
}

void Task::runNormalPathsTask()
{
    QString rules = getRules(optns);
    QLoggingCategory::setFilterRules(rules);

    StreamControl sc;
    QTextStream os(sc.makeOutputDevice(optns[ctoOutput].value));
    QTextStream *is = sc.getInput(optns[ctoInput].value);
    is->setCodec("IBM 866");
    os.setCodec("IBM 866");

    QString array;

    is->device()->waitForReadyRead(-1);
    while (!is->atEnd())
    {
        array += is->readAll();
        is->device()->waitForReadyRead(-1);
    }

    QStringList paths = array.split(";");

    QList<TableLinks*> tableLinks;
    QStringList filesCleared = GetClearedFiles(paths, tableLinks);

    QList<QStringList> chunks;

    if (!filesCleared.isEmpty())
        chunks.append(filesCleared);

    for (const TableLinks *tableLink : qAsConst(tableLinks))
    {
        QStringList::ConstIterator file = std::find_if(paths.begin(), paths.end(), [=](const QString &f)
        {
            return f.contains(tableLink->tableName, Qt::CaseInsensitive);
        });

        QStringList chunk;

        if (file != paths.cend())
            chunk.append("!" + (*file));

        for (const Link *childlnk : qAsConst(tableLink->links))
        {
            file = std::find_if(paths.begin(), paths.end(), [=](const QString &f)
            {
                return f.contains(childlnk->tableName, Qt::CaseInsensitive);
            });

            if (file != paths.cend())
                chunk.append("!" + *file);
        }

        if (!chunk.isEmpty())
            chunks.append(chunk);
    }

    QString result;
    if (optns[ctoXml].isSet)
        result = serializeNormalPathsToXml(chunks);
    else
        result = serializeNormalPathsToJson(chunks);

    os << result;
    os.flush();
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

    DiffModeParser parser(*is);
    QVector<DiffLine> lines = parser.getResult();
    parser.serialize(os, optns[ctoXml].isSet ? "xml" : "json");
}

// --delete --insert --update --input diff.txt --updtbl --output 1_alter.txt
// --delete --insert --update --cs "CONNSTRING=dsn=THOR_DB12DEV1;user id=SERP_3188;password=SERP_3188" --input diff.txt
// --delete --insert --update --input diff.txt --ora --cs "CONNSTRING=dsn=THOR_DB12DEV1;user id=SERP_MYA_2031;password=SERP_MYA_2031"
// --delete --insert --update --input diff.txt --ora --cs "CONNSTRING=dsn=THOR_DB12DEV1;user id=SERP_MYA_2031;password=SERP_MYA_2031" --dat D:\svn\UranRSBankV6\Main\Distrib\DBFile\Data\DPURPLE_DBT.dat
void Task::runScriptTask()
{
    // for debug pause, can attach
    //QInputDialog::getMultiLineText(nullptr, "", "", "");

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

    QVector<ScriptTable*> datTables;
    TableLinksList tableLinks;

    QScopedPointer<FmtTablesList> pFmtTablesList(new FmtTablesList());
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
        linesParser.parseTableName(*is);
        if (linesParser.getLines({ltTable}).count() != 1)
        {
            qWarning(logTask) << "Error. Table name was not parsed.";
            return;
        }

        tableLinks.append(new TableLinks());
        TableLinks *tableLink = tableLinks.back();
        QString fileName = toolFullFileNameFromDir(QString("relations/%1.json")
                                                       .arg(linesParser.getLines({ltTable})[0].toLower()));

        if (QFile::exists(fileName))
        {
            tableLink->loadLinks(fileName);
            qCInfo(logTask) << "Json from" << fileName << "loaded";
        }
        else
        {
            qCWarning(logTask) << "File not exist: " << fileName;
            tableLink->tableName = linesParser.getLines({ltTable})[0].toLower();
        }

        datTables.append(new ScriptTable());

        ScriptTable *datTable = datTables.back();

        FmtTable *fmtTable = new FmtTable(conn->getConnection());
        pFmtTablesList->append(fmtTable);

        if (!fmtTable->load(linesParser.getLines({ltTable})[0].toLower()))
        {
            QString tableName = linesParser.getLines({ltTable})[0].toLower();
            qCWarning(logTask) << QString("Error. fmtTable was not loaded. Table name = '%1'").arg(linesParser.getLines({ltTable})[0]);
            return;
        }
        qCInfo(logTask) << "FMT loaded.";

        QString datfilepath;

        if (optns[ctoDatFile].isSet)
        {
            QFileInfo fi(optns[ctoDatFile].value);

            QDir datdir = fi.absoluteDir();
            datfilepath = datdir.absoluteFilePath(QString("%1.dat").arg(fmtTable->name().toUpper()));
        }

        datTable->loadFromFmt(fmtTable, datfilepath);
        datTable->InitUniqFields(tableLink);
        qCInfo(logTask) << "Fields of" << datTable->name << "loaded. Count =" << datTable->fields.count();

        if (optns[ctoTableInfo].isSet)
            getFmtInfo(os, datTable);

        if (datTable->errorCount() > 0)
        {
            if (!optns[ctoTableInfo].isSet) //чтобы повторно не выводилось
                getFmtInfo(os, datTable);
            for (const QString& err: datTable->getErrors())
                os << err << Qt::endl;
        }

        linesParser.parseDoc(*is, datTable);
        qCInfo(logTask) << "Diff parsed. Lines:"
                        << "insert -" << linesParser.linesCount({ltInsert})
                        << "delete -" << linesParser.linesCount({ltDelete})
                        << "update -" << linesParser.linesCount({ltUpdate});

        datTable->loadData(linesParser.getParsedLines());
        qCInfo(logTask) << "Records of" << datTable->name << "loaded. Count =" << datTable->records.count();

        is->device()->waitForReadyRead(-1);
    }

    JoinTables joinTables;
    for (int i = 0; i < datTables.count(); ++i)
    {
        joinTables.add(datTables[i], tableLinks[i]);
        qCInfo(logTask) << "Added " << datTables[i]->name << "to JoinTables list.";
    }

    QSharedPointer<DbSpelling> dbSpelling;
    if (optns[ctoPostgres].isSet)
        dbSpelling.reset(new DbSpellingPostgres());
    else
        dbSpelling.reset(new DbSpellingOracle());

    SqlScriptMain ssm(dbSpelling, conn);
    QString macroname = tableMacro(joinTables.getRoot()->scriptTable->name);

    QStringList sql;
    if (macroname.isEmpty())
    {
        qInfo(logTask) << "Start sql building.";
        m_Result = ssm.build(sql, joinTables.getRoot());

        os << sql.join("\n");
    }
    else
    {
        QScopedPointer<SqlStringList> pSqlStringList(new SqlStringList(&sql));
        QScopedPointer<TaskOptionsContainer> pTaskOptionsContainer(new TaskOptionsContainer(optns));
        RslGlobalsMap Globals =
        {
            { "{Options}", QVariant::fromValue<QObject*>(pTaskOptionsContainer.data()) },
            { "{SqlStrings}", QVariant::fromValue<QObject*>(pSqlStringList.data()) },
            { "{Spelling}", QVariant::fromValue<QObject*>(dbSpelling.data()) },
            { "{Connection}", QVariant::fromValue<QObject*>(conn.data()) },
            { "{FmtTables}", QVariant::fromValue<QObject*>(pFmtTablesList.data()) },
            { "{Funcs}", QVariant::fromValue<QObject*>(&ssm) },
        };

        SqlStringList SqlList(&sql);
        DiffToScripExecutor executor;
        executor.setGlobalsVariables(Globals);
        executor.setJoinTables(&joinTables);
        executor.playRep(macroname);

        os << sql.join("\n");
    }

    qDeleteAll(datTables);

    // Удаляется деструктором объекта IterableObject
    //qDeleteAll(tableLinks);
}

QString Task::tableMacro(const QString &table)
{
    QSharedPointer<QSettings> settings = diffGetSettings();
    TableFilesModel model(settings.data());

    rslObjList()->applyMacroDirs();
    QStringList inc = rslObjList()->incDirs().split(";");
    model.loadFromDirectories(inc);

    return model.getSelectedFileForTable(table);
}
