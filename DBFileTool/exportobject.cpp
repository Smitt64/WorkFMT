#include "exportobject.h"
#include <fmtcore.h>
#include <toolsruntime.h>
#include <connectioninfo.h>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>

ExportObject::ExportObject(QObject *parent) :
    QObject(parent)
{
    m_ClobMode = ClobMode_Simplified;//ClobMode_SplitFile;
}

ExportObject::~ExportObject()
{

}

void ExportObject::setConnectionInfo(const QString &user, const QString &pswd, const QString &dsn, bool unicode)
{
    _user = user;
    _pswd = pswd;
    _dsn = dsn;
    _unicode = unicode;
}

bool ExportObject::HasClobFields(const QString &table, ConnectionInfo *connection)
{
    QSqlQuery HasClobFieldsQuery(connection->db());
    HasClobFieldsQuery.prepare("SELECT count(1) FROM cols WHERE table_name = upper(?) and data_type = 'CLOB'");
    HasClobFieldsQuery.addBindValue(table.toUpper());

    if (ExecuteQuery(&HasClobFieldsQuery))
        return false;

    if (HasClobFieldsQuery.next())
        return HasClobFieldsQuery.value(0).toInt() > 0;

    return false;
}

void ExportObject::exportTable(const QStringList &table, const QDir &outdir)
{
    for (const QString &t : table)
        exportTable(t, outdir);
}

void ExportObject::setClobMode(const ClobMode &mode)
{
    m_ClobMode = mode;
}

void ExportObject::exportTable(const QString &table, const QDir &outdir)
{
    QString datfile = QString("%1.dat").arg(table.toUpper());
    QString linesfile = QString("%1.rec").arg(table.toUpper());
    QFile datFile(outdir.absoluteFilePath(datfile));
    QFile linesFile(outdir.absoluteFilePath(linesfile));

    if (!datFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return;

    QTextStream stream(&datFile);
    stream.setCodec("IBM 866");

    QScopedPointer<ConnectionInfo> info(new ConnectionInfo());
    QString options;

    if (_unicode)
        options = RSD_UNICODE;

    if (!info->open(QRSD_DRIVER, _user, _pswd, _dsn, options))
        return;

    QSqlQuery alterSession(info->db());
    alterSession.prepare("alter session set nls_numeric_characters = '. '");
    if (ExecuteQuery(&alterSession))
        return;

    QSqlQuery checkExists(info->db());
    checkExists.prepare(QString("select * from %1 where rownum < 2").arg(table.toUpper()));
    if (ExecuteQuery(&checkExists))
        return;

    // Search primary or unique key or unique index for ORDER BY clause
    QString KeyQueryStr = toolReadTextFileContent(":/res/KeyQuery.sql").arg(table.toUpper());
    QSqlQuery KeyQuery(info->db());
    KeyQuery.setForwardOnly(true);
    KeyQuery.prepare(KeyQueryStr);

    if (ExecuteQuery(&KeyQuery))
        return;

    QString query_part3, query_part1, query_part2 = " ";
    while (KeyQuery.next())
    {
        query_part3 += KeyQuery.value(0).toString();
        query_part3 += KeyQuery.value(1).toString();
        query_part3 += KeyQuery.value(2).toString();
    }

    query_part1 = "SELECT ";

    QString query1_str = QString("SELECT column_name, data_type FROM cols WHERE table_name = '%1' order by column_id")
            .arg(table.toUpper());

    QSqlQuery query1(info->db());
    query1.setForwardOnly(false);
    query1.prepare(query1_str);

    if (ExecuteQuery(&query1))
        return;

    stream << "LOAD DATA" << Qt::endl;

    bool fHasClobFields = HasClobFields(table, info.data());
    if (!fHasClobFields || m_ClobMode == ClobMode_Simplified)
        stream << "INFILE *" << Qt::endl;
    else
        stream << QString("INFILE '%1.rec' \"str '<endrec>\\r\\n'\"").arg(table) << Qt::endl;

    stream << "INTO TABLE " << table.toUpper() << Qt::endl;
    stream << "FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY X'27'" << Qt::endl;
    stream << "TRAILING NULLCOLS" << Qt::endl;
    stream << "(";

    bool fSkipComma = false;
    int col_count = 0;
    QStringList clobs;

    while (query1.next())
    {
        QString col_name = query1.value(0).toString();
        QString col_type = query1.value(1).toString();

        col_count++;

        if (col_count > 1)
        {
            if (!fSkipComma)
            {
                stream << "," << Qt::endl;
                query_part2 += "||','||";
            }

            fSkipComma = false;
        }

        if (col_type != "CLOB")
            stream << col_name;

        if (col_type == "NUMBER" || col_type == "FLOAT")
            query_part2 += col_name;
        else if (col_type == "CHAR")
        {
            stream << " \"REPLACE(:" << col_name
                   << ", chr(2), chr(0))\"";

            query_part2 += "''''||DECODE(";
            query_part2 += col_name;
            query_part2 += ", chr(0), chr(2), ";
            query_part2 += col_name;
            query_part2 += ")||''''";
        }
        else if (col_type == "VARCHAR2" || col_type == "RAW")
        {
            stream << " CHAR (4000) \"REPLACE(:"
                   << col_name
                   << ",'chr(10)',chr(10))\"";

            query_part2 += "DECODE(";
            query_part2 += col_name;
            query_part2 += ",chr(1),chr(1),chr(0),chr(1),";
            query_part2 += "''''||REPLACE(REPLACE(";
            query_part2 += col_name;
            query_part2 += ",'''',''''''),chr(10),'chr(10)')||'''')";
        }
        else if (col_type == "CLOB")
        {
            clobs.append(col_name);
            fSkipComma = true;
        }
        else if (col_type == "DATE")
        {
            stream << " DATE 'DD-MM-YYYY:HH24:MI:SS'";
            query_part2 += "to_char(";
            query_part2 += col_name;
            query_part2 += ",'DD-MM-YYYY:HH24:MI:SS')";
        }
        else if (col_type == "BLOB")
        {
            QString file_name1 = table + "_BLOB.sql"; // outdir
            QFile blobfile(outdir.absoluteFilePath(file_name1));

            if (!blobfile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            {
            }
            else
            {
                QString blob = toolReadTextFileContent(":/res/BlobTemplate.sql").arg(table);
                QTextStream blobstream(&blobfile);
                blobstream.setCodec("IBM 866");
                blobstream << blob;

                blobstream.flush();
                blobfile.close();
            }

            stream << " CHAR (32000)";
            query_part2 += "''''||dbms_lob.substr(";
            query_part2 += col_name;
            query_part2 += ",dbms_lob.getlength(";
            query_part2 += col_name;
            query_part2 += "),1)||''''";
        }
    }

    QString query_str;
    query_part2 += " t_joined";

    for (const QString &clob : clobs)
    {
        stream << "," << Qt::endl;
        stream << clob;

        if (m_ClobMode == ClobMode_SplitFile)
        {
            stream << QString(" CHAR(%1) ENCLOSED BY '<startlob>' AND '<endlob>'")
                  .arg(RSD_BLOB_MAX_LEN);
        }
    }

    if (!fHasClobFields || m_ClobMode != ClobMode_SplitFile)
        stream << ")\n\n\nBEGINDATA\n";
    else
        stream << ")";

    if (!clobs.isEmpty())
        query_part2 += ", " + clobs.join(", ");

    query_part2 += " from " + table;
    query_str = query_part1 + query_part2;
    query_str += query_part3;

    QSqlQuery query(info->db());
    query.prepare(query_str);

    if (ExecuteQuery(&query))
        return;

    if (fHasClobFields && m_ClobMode == ClobMode_SplitFile)
    {
        if (!linesFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            return;

        stream.setDevice(&linesFile);
        stream.setCodec("IBM 866");
    }

    int i = 1;
    QTextStream StdOutput(stdout);
    StdOutput.setCodec("IBM 866");
    while(query.next())
    {
        stream << query.value(0).toString();

        for (const QString &clob : clobs)
        {
            QString value = query.value(clob).toString();
            if (m_ClobMode == ClobMode_SplitFile)
                stream << ",<startlob>"<< value << "<endlob>";
            else
            {
                value = value.simplified();

                if (m_ClobMode == ClobMode_SimplifiedTrimmed)
                    value = value.trimmed();

                QSqlField f;
                f.setType(QVariant::String);
                f.setValue(value);
                stream << ","
                       << info->db().driver()->formatValue(f, true) << "";
            }
        }

        if (fHasClobFields && m_ClobMode == ClobMode_SplitFile)
            stream << "<endrec>";

        stream << Qt::endl;

        StdOutput << QString("Processing... %1 rows").arg(i) << Qt::endl;
        i ++;
    }//dss_sheduler_dbt

    if (linesFile.isOpen())
    {
        linesFile.flush();
        linesFile.close();
    }
    datFile.flush();
    datFile.close();
}
