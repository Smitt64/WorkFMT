#include "fmtcore.h"
#include "fmtapplication.h"
#include "ErrorsModel.h"
#include "toolsruntime.h"
#include "fmttable.h"
#include "fmtfield.h"
#include "errordlg.h"
#include "dbinitdlg.h"
#include "oracletnslistmodel.h"
#include "loggingcategories.h"
#include "fmtdbftoolwrp.h"
#include "connectioninfo.h"
#include <Windows.h>
#include <QtCore>
#include <QSqlError>
#include <map>
#include <iostream>
#include <strstream>
#include <QProgressDialog>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QSqlDriver>
#include <QSqlField>

static QString Simplify(QString line)
{
    QString str = line;
    str = str.remove(QRegExp("(\\\"([\\w]+)\\\")\\."));
    line.replace("'", "''");
    str = str.remove("\"");
    return str.simplified();
}

void WrapSqlBlockObjectExists(QTextStream &stream, const QString &block, QList<FmtField*> flds, const QSharedPointer<FmtTable> &pTable)
{
    stream << "DECLARE " << Qt::endl;
    stream << "    e_object_exists EXCEPTION;" << Qt::endl;
    stream << "    PRAGMA EXCEPTION_INIT(e_object_exists, -955); " << Qt::endl;
    stream << "BEGIN" << Qt::endl;
    stream << "    EXECUTE IMMEDIATE " << Qt::endl;

    QStringList lines = block.split(QRegExp("\n|\r\n|\r"));
    for (int i = 0; i < lines.count(); i++)
    {
        QString str = Simplify(lines.at(i));

        if (!str.isEmpty())
        {
            stream << "        '" << str << "'";
            if (i != lines.count() - 1)
                stream << " || " << Qt::endl;
        }
    }
    stream << ";" << Qt::endl;

    if (!pTable.isNull())
    {
        stream << Qt::endl;

        QSqlDriver *driver = pTable->connection()->driver();

        QSqlField fld;
        fld.setType(QVariant::String);
        fld.setValue(pTable->comment());
        QString comment = QString("COMMENT ON TABLE %1 IS %2").arg(pTable->name().toUpper())
                .arg(driver->formatValue(fld, true));
        fld.setValue(comment);
        stream << QString("\tEXECUTE IMMEDIATE %1;")
                  .arg(driver->formatValue(fld, true))
               << Qt::endl;
    }

    if (!flds.isEmpty())
    {
        stream << Qt::endl;
        QString tableName = flds[0]->table()->name().toUpper();
        QSqlDriver *driver = flds[0]->table()->connection()->driver();
        foreach (const FmtField *pfld, flds) {
            QSqlField fld;
            fld.setType(QVariant::String);
            fld.setValue(pfld->comment());

            QString comment = QString("COMMENT ON COLUMN %1.%2 IS %3").arg(tableName)
                    .arg(pfld->name().toUpper())
                    .arg(driver->formatValue(fld, true));
            fld.setValue(comment);
            stream << QString("\tEXECUTE IMMEDIATE %1;")
                      .arg(driver->formatValue(fld, true))
                   << Qt::endl;
        }
    }

    stream << Qt::endl;
    stream << "EXCEPTION " << Qt::endl;
    stream << "    WHEN e_object_exists THEN NULL; " << Qt::endl;
    stream << "END;" << Qt::endl;
    stream << "/" << Qt::endl;
}

static void FmtGenUpdateDeleteColumnScriptField(QTextStream &stream, FmtField *fld)
{
    FmtTable *pTable = fld->table();
    stream << "DECLARE" << Qt::endl;
    stream << "\te_col_not_exist EXCEPTION;" << Qt::endl;
    stream << "\tPRAGMA EXCEPTION_INIT (e_col_not_exist, -904);" << Qt::endl;
    stream << "BEGIN" << Qt::endl;

    stream << QString("\tEXECUTE IMMEDIATE 'ALTER TABLE %1 DROP (%2)';")
              .arg(pTable->name().toUpper())
              .arg(fld->name().toUpper())
           << Qt::endl;

    stream << "EXCEPTION WHEN e_col_not_exist THEN NULL;" << Qt::endl;
    stream << "END;" << Qt::endl;
    stream << "/" << Qt::endl;
}

QStringList FmtGenGetTriggers(ConnectionInfo *connection, const QString &table)
{
    if (connection->type() != ConnectionInfo::CON_ORA)
        return QStringList();

    QStringList triggers;

    QSqlQuery query(connection->db());
    query.prepare("select trigger_name from USER_TRIGGERS where table_name = upper(?) and INSTR (trigger_name, 'AINC') = 0");
    query.addBindValue(table);

    if (!query.exec())
        return QStringList();

    while (query.next())
        triggers.append(query.value(0).toString());

    return triggers;
}

QString FmtGenTriggersScrip(ConnectionInfo *connection, const QString &table, bool disable)
{
    QString str;
    QTextStream stream(&str, QIODevice::WriteOnly);
    QStringList triggers = FmtGenGetTriggers(connection, table);

    if (triggers.isEmpty())
        return QString();

    std::transform(triggers.begin(), triggers.end(), triggers.begin(),
                   [](const QString &s) { return "'" + s + "'"; });

    QString filename = disable ? "://addfld_disable_trigger_tmpl.sql" : "://addfld_enable_trigger_tmpl.sql";
    QString tmpl = toolReadTextFileContent(filename);

    QString ora = triggers.join(",\n\t\t");
    QString pg = triggers.join(",\n--\t\t");
    str = tmpl.arg(ora, pg, table.toUpper()) + "\n";

    return str;
}

QString FmtGenTriggersScrip(QList<FmtField*> flds, bool disable)
{
    QString str;
    QTextStream stream(&str, QIODevice::WriteOnly);

    if (flds.isEmpty())
        return QString();

    return FmtGenTriggersScrip(flds.first()->table()->connection(),
                                             flds.first()->table()->name(),
                                             disable);
}

QString FmtGenUpdateDeleteColumnScript(QList<FmtField*> flds)
{
    QString str;
    QTextStream stream(&str, QIODevice::WriteOnly);

    foreach (const FmtField *fld, flds) {
        FmtGenUpdateDeleteColumnScriptField(stream, (FmtField*)fld);
        stream << Qt::endl;
    }
    return str;
}

QString FmtGenUpdateAddColumnScript(QList<FmtField*> flds)
{
    if (flds.isEmpty())
        return QString();

    QString str, tableName = flds[0]->table()->name().toUpper();
    QTextStream stream(&str, QIODevice::WriteOnly);

    stream << FmtGenTriggersScrip(flds, true) << Qt::endl;

    stream << "DECLARE" << Qt::endl;
    stream << "\te_col_exist EXCEPTION;" << Qt::endl;
    stream << "\tPRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
    stream << "BEGIN" << Qt::endl;

    stream << QString("\t-- Такой вариант намного быстрее, чем делать update") << Qt::endl;
    stream << QString("\tEXECUTE IMMEDIATE 'ALTER TABLE %1 ADD ( ' || ").arg(tableName) << Qt::endl;

    foreach (const FmtField *fld, flds) {
        stream << "\t\t\t" << QString("'%1 %2 DEFAULT %3").arg(fld->name().toUpper(), fld->getOraDecl(), fld->getOraDefaultVal());

        if (fld != flds.last())
            stream << ",";
        stream << "' ||" << Qt::endl;
    }
    stream << "\t\t')';" << Qt::endl;
    stream << Qt::endl;

    stream << QString("\tEXECUTE IMMEDIATE 'ALTER TABLE %1 MODIFY (' || ").arg(tableName) << Qt::endl;

    foreach (const FmtField *fld, flds) {
        stream << "\t\t\t" << QString("'%1 DEFAULT NULL").arg(fld->name().toUpper());

        if (fld != flds.last())
            stream << ",";
        stream << "' ||" << Qt::endl;
    }
    stream << "\t\t')';" << Qt::endl;
    stream << Qt::endl;

    QSqlDriver *driver = flds[0]->table()->connection()->driver();
    foreach (const FmtField *pfld, flds) {
        QSqlField fld;
        fld.setType(QVariant::String);
        fld.setValue(pfld->comment());

        QString comment = QString("COMMENT ON COLUMN %1.%2 IS %3").arg(tableName)
                .arg(pfld->name().toUpper())
                .arg(driver->formatValue(fld, true));
        fld.setValue(comment);
        stream << QString("\tEXECUTE IMMEDIATE %1;")
                  .arg(driver->formatValue(fld, true))
               << Qt::endl;
    }
    stream << "" << Qt::endl;

    stream << "EXCEPTION WHEN e_col_exist THEN NULL;" << Qt::endl;
    stream << "END;" << Qt::endl;
    stream << "/" << Qt::endl;

    QString triggers = FmtGenTriggersScrip(flds, false);

    if (!triggers.isEmpty())
        stream << Qt::endl << FmtGenTriggersScrip(flds, false) << Qt::endl;
        
    return str;
}

QString FmtGenModifyColumnScript(QList<FmtField*> flds)
{
    if (flds.isEmpty())
        return QString();

    QString str, tableName = flds[0]->table()->name().toUpper();
    QTextStream stream(&str, QIODevice::WriteOnly);

    stream << "DECLARE" << Qt::endl;
    stream << "\te_col_exist EXCEPTION;" << Qt::endl;
    stream << "\tPRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << Qt::endl;
    stream << "BEGIN" << Qt::endl;
    stream << QString("\tEXECUTE IMMEDIATE 'ALTER TABLE %1 MODIFY ( ' || ").arg(tableName) << Qt::endl;

    foreach (const FmtField *fld, flds) {
        stream << "\t\t\t" << QString("'%1 %2").arg(fld->name().toUpper(), fld->getOraDecl());
        if (fld != flds.last())
            stream << ",";
        stream << "' ||" << Qt::endl;
    }
    stream << "\t\t')';" << Qt::endl << Qt::endl;

    QSqlDriver *driver = flds[0]->table()->connection()->driver();
    foreach (const FmtField *pfld, flds) {
        QSqlField fld;
        fld.setType(QVariant::String);
        fld.setValue(pfld->comment());

        QString comment = QString("COMMENT ON COLUMN %1.%2 IS %3").arg(tableName)
                .arg(pfld->name().toUpper())
                .arg(driver->formatValue(fld, true));
        fld.setValue(comment);
        stream << QString("\tEXECUTE IMMEDIATE %1;")
                  .arg(driver->formatValue(fld, true))
               << Qt::endl;
    }
    stream << "" << Qt::endl;

    stream << "EXCEPTION WHEN e_col_exist THEN NULL;" << Qt::endl;
    stream << "END;" << Qt::endl;
    stream << "/" << Qt::endl;
    return str;
}

QString FmtGenUpdateCreateTableScript(QSharedPointer<FmtTable> pTable)
{
    QString str;
    QTextStream stream(&str, QIODevice::WriteOnly);

    QList<FmtField*> flds;
    for (qint16 i = 0; i < pTable->fieldsCount(); i++)
        flds.append(pTable->field(i));

    WrapSqlBlockObjectExists(stream, pTable->generateCreateTableSql(), flds, pTable);
    return str;
}

QString FmtGenInsertTemplateSqlScript(QList<FmtField*> flds)
{
    if (flds.isEmpty())
        return QString();

    QString str, tableName = flds[0]->table()->name().toUpper();
    QTextStream stream(&str, QIODevice::WriteOnly);
    stream << "INSERT INTO " << tableName << "(";

    foreach (const FmtField *fld, flds) {
        stream << QString("%1").arg(fld->name().toUpper());
        if (fld != flds.last())
            stream << ",";
    }

    stream << ") values(";

    foreach (const FmtField *fld, flds) {
        stream << "?";
        if (fld != flds.last())
            stream << ", ";
    }

    stream << ");";

    return str;
}
