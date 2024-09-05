#include "exportobject.h"
#include <fmtcore.h>
#include <toolsruntime.h>
#include <connectioninfo.h>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>

ExportObject::ExportObject(QObject *parent) :
    QObject(parent)
{

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

void ExportObject::exportTable(const QString &table, const QDir &outdir)
{
    QString datfile = QString("%1.dat").arg(table);
    QFile datFile(outdir.absoluteFilePath(datfile));

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
    //checkExists.setForwardOnly(true);
    checkExists.prepare(QString("select * from %1 where rownum < 2").arg(table));
    //checkExists.prepare(QString("SELECT column_name, data_type FROM USER_TAB_COLUMNS WHERE table_name = '%1' order by column_id").arg(table));

    if (ExecuteQuery(&checkExists))
        return;

    while (checkExists.next())
        qDebug() << checkExists.value(1);

    // Search primary or unique key or unique index for ORDER BY clause
    QString KeyQueryStr = toolReadTextFileContent(":/res/KeyQuery.sql").arg(table);
    QSqlQuery KeyQuery(info->db());
    //KeyQuery.setForwardOnly(true);
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
            .arg(table);

    QSqlQuery query1(info->db());
    query1.setForwardOnly(false);
    query1.prepare(query1_str);

    if (ExecuteQuery(&query1))
        return;

    int col_count = 0;
    while (query1.next())
    {
        QString col_name = query1.value(0).toString();
        QString col_type = query1.value(1).toString();

        col_count++;

        if (col_count > 1)
        {
            stream << Qt::endl;
            query_part2 += "||','||";
        }

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
                QTextStream blobstream(&datFile);
                blobstream.setCodec("IBM 866");
                blobstream << blob;

                blobstream.flush();
                blobfile.close();
            }

            stream << " CHAR(32000)";
            query_part2 += "''''||dbms_lob.substr(";
            query_part2 += col_name;
            query_part2 += ",dbms_lob.getlength(";
            query_part2 += col_name;
            query_part2 += "),1)||''''";
        }
    }

    stream << ")\n\n\nBEGINDATA\n";
    datFile.flush();
    datFile.close();
}
