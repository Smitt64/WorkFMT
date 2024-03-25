#include <QTest>

#include "difftoscripttest.h"
#include "linesparsermain.h"
#include "linespareser.h"
#include "recordparser.h"
#include "difffield.h"
#include "dattable.h"
#include "dbspellingoracle.h"
#include "join.h"
#include "fmttable.h"
#include "diffconnection.h"
#include "sqlscriptmain.h"

DiffToScriptTest::DiffToScriptTest(QObject *parent) : QObject(parent)
{

}

void DiffToScriptTest::initTestCase()
{
    QLoggingCategory::setFilterRules(QStringLiteral("Fmt=false\nSql=false\nCore=false\n*.info=false\n*.warning=true\nDiffToScript.Join=true\nDiffToScript.SqlScriptMain=true"));
}

void DiffToScriptTest::cleanupTestCase()
{
    QLoggingCategory::setFilterRules(QStringLiteral("Fmt=false\nSql=false\nCore=false\n*.info=true"));
}

void DiffToScriptTest::caseLinesType()
{
    QStringList input;
    input << "Index: table1.dat"
          << "+++n"
          << "+i"
          << "---n"
          << "-d"
          << "-u"
          << "+u"
          << "Index: table2.dat"
          << "+++n"
          << "+i"
          << "---n"
          << "-d"
          << "-u"
          << "+u"
             ;

    QString buff = input.join("\n");
    QTextStream is(&buff);

    LinesParserMain pm;
    pm.setTableParser(new LinesTablePareser("Index: "));
    pm.setDeleteParser(new LinesDeleteParser("-"));
    pm.setInsertParser(new LinesInsertParser("+"));
    pm.setUpdateParser(new LinesUpdateParser("-"));
    pm.parseDoc(is);

    QCOMPARE(pm.getLines({ltTable}).count(),1);
    QCOMPARE(pm.getLines({ltTable}).back(), "table1");

    QCOMPARE(pm.getLines({ltTable}).count(),1);
    QCOMPARE(pm.getLines({ltDelete}).count(),1);
    QCOMPARE(pm.getLines({ltInsert}).count(),1);
    QCOMPARE(pm.getLines({ltUpdate}).count(),2);

    pm.parseDoc(is);

    QCOMPARE(pm.getLines({ltTable}).count(),1);
    QCOMPARE(pm.getLines({ltTable}).back(), "table2");

    QCOMPARE(pm.getLines({ltDelete}).count(),1);
    QCOMPARE(pm.getLines({ltInsert}).count(),1);
    QCOMPARE(pm.getLines({ltUpdate}).count(),2);
}

void DiffToScriptTest::caseParseStringField()
{
    QString input =
            "0,"                                //0
            "1,"                                //1
            "1.1,"                              //2
            "4.4,"                              //4
            "'value ''of'' string1, string2',"  //7
            "01-01-0001:00:00:00,"              //9
            "0001-01-01 13:08:03.000,"          //10
            "\u0002,"                           //12
            "'0001',"                           //13
            "1234.56";                          //25
    QTextStream is(&input);

    DiffFields dfs;                                     //autoinc   string
    dfs.append(DiffField {"f_int",      0,  "INT",      true,       false});    //fmtt_INT = 0
    dfs.append(DiffField {"f_long",     1,  "LONG",     false,      false});    //fmtt_LONG = 1
    dfs.append(DiffField {"f_float",    2,  "FLOAT",    false,      false});    //fmtt_FLOAT = 2
    dfs.append(DiffField {"f_double",   4,  "DOUBLE",   false,      false});    //fmtt_DOUBLE = 4
//    dfs.append(DiffField {"f     ",     6,  "???",      false,      false});    //???             Сейчас в таблицах не используется
    dfs.append(DiffField {"f_string",   7,  "STRING",   false,      true });     //fmtt_STRING = 7
//    dfs.append(DiffField {"f_snr",      8,  "SNR",      false,      false});    //fmtt_SNR = 8    Сейчас в таблицах не используется
    dfs.append(DiffField {"f_date",     9,  "DATE",     false,      false});    //fmtt_DATE = 9
    dfs.append(DiffField {"f_time",     10, "TIME",     false,      false});    //fmtt_TIME = 10
    dfs.append(DiffField {"f_chr",      12, "CHR",      false,      true });     //fmtt_CHR = 12
    dfs.append(DiffField {"f_uchr",     13, "UCHR",     false,      true });     //fmtt_UCHR = 13
    dfs.append(DiffField {"f_numeric",  25, "NUMERIC",  false,      false});     //fmtt_NUMERIC = 25

    RecordParser rp(&dfs);
    QVERIFY(rp.parseRecord(input));
    QCOMPARE(rp.getValues().count(), 10);

    LinesParserMain linesParser;
    linesParser.setTableParser(new LinesTablePareser("Index: "));
    linesParser.setInsertParser(new LinesInsertParser("+"));
    linesParser.setDeleteParser(new LinesDeleteParser("-"));
    linesParser.setUpdateParser(new LinesUpdateParser("-"));

    input = "Index: table.dat\n"
            "+0,0,1.1,4.4,'string''0'', string2',01-01-0001:00:00:00,0001-01-01 13:08:03.000,\u0002,'0001',1234.56\n"   //insert
            "-1,1,1.1,4.4,'string''1'', string2',01-01-0001:00:00:00,0001-01-01 13:08:03.000,\u0002,'0001',1234.56\n"   //delete
            "-2,2,1.1,4.4,'string''2'', string2',01-01-0001:00:00:00,0001-01-01 13:08:03.000,\u0002,'0001',1234.56\n"   //update
            "+3,3,1.1,4.4,'string''3'', string2',01-01-0001:00:00:00,0001-01-01 13:08:03.000,\u0002,'0001',1234.56\n";  //update

    is.setString(&input);
    linesParser.parseDoc(is);

    QCOMPARE(linesParser.getLines({ltTable}).count(), 1);
    QCOMPARE(linesParser.getLines({ltTable})[0], "table");
    QCOMPARE(linesParser.getParsedLines().count(), 5);

    QTextStream os(stdout);

    DatTable dt;
    dt.name = "table";
    dt.fields = dfs;

    IndexFields fields;
    fields.append({"f_int", false});
    DatIndex index{"INX0", fields, true};

    dt.indexes.append(index);
    dt.loadData(linesParser.getParsedLines());

    QScopedPointer<DbSpelling> ora(new DbSpellingOracle);
    SqlScriptMain ssm(ora);

    TableLinks tableLinks;
    tableLinks.tableName = "table";

    JoinTable joinTable(&dt, tableLinks);

    QString buff;
    os.setString(&buff);
    ssm.build(os, &joinTable);
    os.flush();

    QStringList lst = buff.split("\n");
    QStringList result;
    result.append("DECLARE");
    result.append("  v_table__f_int table.f_int%TYPE;");
    result.append("BEGIN");
    result.append("INSERT INTO table (f_int, f_long, f_float, f_double, f_string, f_date, f_time, f_chr, f_uchr, f_numeric) VALUES(0, 0, 1.1, 4.4, 'string''0'', string2', TO_DATE('01-01-0001:00:00:00', 'DD-MM-YYY'), 0001-01-01 13:08:03.000, , '0001', 1234.56);");
    result.append("DELETE FROM table WHERE f_int = 1;");
    result.append("UPDATE table SET f_int = 3, f_long = 3, f_string = 'string''3'', string2' WHERE f_int = 2;");
    result.append("END;");

    QCOMPARE(lst[0], result[0]);
    QCOMPARE(lst[1], result[1]);
    QCOMPARE(lst[2], result[2]);

}


void DiffToScriptTest::caseJsonLoad()
{
    QStringList tableNames = QStringList() << "DACSRESTGROUP_DBT" << "DACSRESTRICT_DBT";
    TableLinks tableLinks[2];
    QStringList fileNames = QStringList()
            << "test\\" + tableNames[0].toLower() + ".json"
            << "test\\" + tableNames[1].toLower() + ".json"
            ;

    //save DACSRESTGROUP_DBT
    int indx = 0;
    tableLinks[indx].tableName = tableNames[indx];
    tableLinks[indx].index << "T_PRIVID" << "T_ITEMLEVEL" << "T_ITEMID";
    tableLinks[indx].saveLinks(fileNames[indx]);

    //save DACSRESTRICT_DBT
    indx = 1;
    tableLinks[indx].tableName = tableNames[indx];

    Link link;
    link.tableName = "DACSRESTGROUP_DBT";
    link.fields.append(LinkField{"T_RESTID", "T_RESTID"});

    tableLinks[indx].links.append(link);
    tableLinks[indx].saveLinks(fileNames[indx]);

    //check load
    TableLinks tablesFromFile[2];

    //load DACSRESTGROUP_DBT
    indx = 0;
    tablesFromFile[indx].loadLinks(fileNames[indx]);

    QCOMPARE(tableLinks[indx].index, tablesFromFile[indx].index);
    QCOMPARE(tableLinks[indx].links, tablesFromFile[indx].links);
    QCOMPARE(tableLinks[indx].tableName, tablesFromFile[indx].tableName);
    QCOMPARE(tableLinks[indx], tablesFromFile[indx]);

    //load DACSRESTRICT_DBT
    indx = 0;
    tablesFromFile[indx].loadLinks(fileNames[indx]);

    QCOMPARE(tableLinks[indx].index, tablesFromFile[indx].index);
    QCOMPARE(tableLinks[indx].links, tablesFromFile[indx].links);
    QCOMPARE(tableLinks[indx].tableName, tablesFromFile[indx].tableName);
    QCOMPARE(tableLinks[indx], tablesFromFile[indx]);
}



void DiffToScriptTest::caseDoubleInsert()
{ 
    QFile inputFile("test\\opu_insert_in.txt");
    inputFile.open(QFile::ReadOnly);
    QTextStream is(inputFile.readAll());

    LinesParserMain mainParser;
    mainParser.setTableParser(new LinesTablePareser("Index: "));
    mainParser.setInsertParser(new LinesInsertParser("+"));
    mainParser.setDeleteParser(new LinesDeleteParser("-"));
    mainParser.setUpdateParser(new LinesUpdateParser("-"));

    QVector<DatTable> datTables;
    QVector<TableLinks> tableLinks;

    while (!is.atEnd())
    {
        mainParser.parseDoc(is);
        datTables.append(DatTable());

        DatTable& datTable = datTables.back();

        FmtTable fmtTable;
        fmtTable.loadFromXml("test\\" + mainParser.getLines({ltTable})[0].toLower() + ".xml");

        datTable.loadFromFmt(&fmtTable);
        if (datTable.name.toLower() == "dopusymhist_dbt")
        {
            datTables[0].fields.fieldByName("t_name").isString = true; // Почему-то из xml не правильно тип считывается.

            DatIndex indx;
            indx.isUnique = true;
            indx.fields.append({"T_OFRRECID", false});
            indx.fields.append({"T_STARTDATE", false});

            datTables[0].indexes.append(indx);
        }
        if (datTable.name.toLower() == "dopusymb_dbt")
        {
            datTables[1].fields.fieldByName("T_RECID").isAutoinc = true; //из xml не подгружается автоинеремент
        }

        datTable.loadData(mainParser.getParsedLines());
        QCOMPARE(datTable.errorCount(), 0);

        tableLinks.append(TableLinks());
        TableLinks& tableLink = tableLinks.back();
        QString fileName = "test\\" + datTable.name.toLower() + ".json";
        if (QFile::exists(fileName))
            tableLink.loadLinks(fileName);
        else
            tableLink.tableName = datTable.name.toLower();
    }

    QCOMPARE(datTables.count(), 2);
    QCOMPARE(datTables[0].name.toLower(), "dopusymhist_dbt");
    QCOMPARE(datTables[1].name.toLower(), "dopusymb_dbt");

    QCOMPARE(datTables[0].records.count(), 4);
    QCOMPARE(datTables[1].records.count(), 4);

    QCOMPARE(datTables[1].records.getRecords({ltInsert}).count(), 1);
    QCOMPARE(datTables[1].records.getRecords({ltDelete}).count(), 1);
    QCOMPARE(datTables[1].records.getRecords({ltUpdate}).count(), 2);

    QCOMPARE(datTables[0].records.getRecords({ltInsert}).count(), 1);
    QCOMPARE(datTables[0].records.getRecords({ltDelete}).count(), 1);
    QCOMPARE(datTables[0].records.getRecords({ltUpdate}).count(), 2);

    JoinTables joinTables;
    joinTables.add(&datTables[0], tableLinks[0]);
    joinTables.add(&datTables[1], tableLinks[1]);

    JoinTable *parentJoinTable = joinTables.tableByName("dopusymb_dbt");
    JoinTable *childJoinTable = joinTables.tableByName("dopusymhist_dbt");

    Join* join = parentJoinTable->joinList[0];
    join = childJoinTable->getParentJoin();

//    QCOMPARE(join->indexDownToUp.size(), 4);
//    QCOMPARE(join->indexUpToDown.size(), 3);

    QScopedPointer<DbSpelling> ora(new DbSpellingOracle);
    SqlScriptMain ssm(ora);

    QFile outFile("test\\opu_insert_out.txt");
    QCOMPARE(outFile.open(QFile::WriteOnly), true);
    QTextStream os(&outFile);
    ssm.build(os, joinTables.getRoot());
}





