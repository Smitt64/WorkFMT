#include "fmtcore.h"
#include "fmtapplication.h"
#include "fmterrors.h"
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
    stream << "DECLARE " << endl;
    stream << "    e_object_exists EXCEPTION;" << endl;
    stream << "    PRAGMA EXCEPTION_INIT(e_object_exists, -955); " << endl;
    stream << "BEGIN" << endl;
    stream << "    EXECUTE IMMEDIATE " << endl;

    QStringList lines = block.split(QRegExp("\n|\r\n|\r"));
    for (int i = 0; i < lines.count(); i++)
    {
        QString str = Simplify(lines.at(i));

        if (!str.isEmpty())
        {
            stream << "        '" << str << "'";
            if (i != lines.count() - 1)
                stream << " || " << endl;
        }
    }
    stream << ";" << endl;

    if (!pTable.isNull())
    {
        stream << endl;

        QSqlDriver *driver = pTable->connection()->driver();

        QSqlField fld;
        fld.setType(QVariant::String);
        fld.setValue(pTable->comment());
        QString comment = QString("COMMENT ON TABLE %1 IS %2").arg(pTable->name().toUpper())
                .arg(driver->formatValue(fld, true));
        fld.setValue(comment);
        stream << QString("\tEXECUTE IMMEDIATE %1;")
                  .arg(driver->formatValue(fld, true))
               << endl;
    }

    if (!flds.isEmpty())
    {
        stream << endl;
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
                   << endl;
        }
    }

    stream << endl;
    stream << "EXCEPTION " << endl;
    stream << "    WHEN e_object_exists THEN NULL; " << endl;
    stream << "END;" << endl;
    stream << "/" << endl;
}

static void FmtGenUpdateDeleteColumnScriptField(QTextStream &stream, FmtField *fld)
{
    FmtTable *pTable = fld->table();
    stream << "DECLARE" << endl;
    stream << "\te_col_not_exist EXCEPTION;" << endl;
    stream << "\tPRAGMA EXCEPTION_INIT (e_col_not_exist, -904);" << endl;
    stream << "BEGIN" << endl;

    stream << QString("\tEXECUTE IMMEDIATE 'ALTER TABLE %1 DROP (%2)';")
              .arg(pTable->name().toUpper())
              .arg(fld->name().toUpper())
           << endl;

    stream << "EXCEPTION WHEN e_col_not_exist THEN NULL;" << endl;
    stream << "END;" << endl;
    stream << "/" << endl;
}

QString FmtGenUpdateDeleteColumnScript(QList<FmtField*> flds)
{
    QString str;
    QTextStream stream(&str, QIODevice::WriteOnly);

    foreach (const FmtField *fld, flds) {
        FmtGenUpdateDeleteColumnScriptField(stream, (FmtField*)fld);
        stream << endl;
    }
    return str;
}

QString FmtGenUpdateAddColumnScript(QList<FmtField*> flds)
{
    if (flds.isEmpty())
        return QString();

    QString str, tableName = flds[0]->table()->name().toUpper();
    QTextStream stream(&str, QIODevice::WriteOnly);

    stream << "DECLARE" << endl;
    stream << "\te_col_exist EXCEPTION;" << endl;
    stream << "\tPRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << endl;
    stream << "BEGIN" << endl;
    stream << QString("\tEXECUTE IMMEDIATE 'ALTER TABLE %1 ADD ( ' || ").arg(tableName) << endl;

    foreach (const FmtField *fld, flds) {
        stream << "\t\t\t" << QString("'%1 %2").arg(fld->name().toUpper(), fld->getOraDecl());
        if (fld != flds.last())
            stream << ",";
        stream << "' ||" << endl;
    }
    stream << "\t\t')';" << endl;
    stream << endl;
    stream << QString("\tEXECUTE IMMEDIATE 'UPDATE %1 SET ' || ").arg(tableName) << endl;

    foreach (const FmtField *fld, flds) {
        stream << "\t\t\t" << QString("'%1 = %2").arg(fld->name().toUpper(), fld->getOraDefaultVal());
        if (fld != flds.last())
        {
            stream << ",";
            stream << "' ||" << endl;
        }

    }
    stream << "';" << endl << endl;

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
               << endl;
    }
    stream << "" << endl;

    stream << "EXCEPTION WHEN e_col_exist THEN NULL;" << endl;
    stream << "END;" << endl;
    stream << "/" << endl;
    return str;
}

QString FmtGenModifyColumnScript(QList<FmtField*> flds)
{
    if (flds.isEmpty())
        return QString();

    QString str, tableName = flds[0]->table()->name().toUpper();
    QTextStream stream(&str, QIODevice::WriteOnly);

    stream << "DECLARE" << endl;
    stream << "\te_col_exist EXCEPTION;" << endl;
    stream << "\tPRAGMA EXCEPTION_INIT (e_col_exist, -01430);" << endl;
    stream << "BEGIN" << endl;
    stream << QString("\tEXECUTE IMMEDIATE 'ALTER TABLE %1 MODIFY ( ' || ").arg(tableName) << endl;

    foreach (const FmtField *fld, flds) {
        stream << "\t\t\t" << QString("'%1 %2").arg(fld->name().toUpper(), fld->getOraDecl());
        if (fld != flds.last())
            stream << ",";
        stream << "' ||" << endl;
    }
    stream << "\t\t')';" << endl << endl;

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
               << endl;
    }
    stream << "" << endl;

    stream << "EXCEPTION WHEN e_col_exist THEN NULL;" << endl;
    stream << "END;" << endl;
    stream << "/" << endl;
    return str;
}

QString FmtGenUpdateCreateTableScript(QSharedPointer<FmtTable> pTable)
{
    QString str;
    QTextStream stream(&str, QIODevice::WriteOnly);

    QList<FmtField*> flds;
    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
        flds.append(pTable->field(i));

    WrapSqlBlockObjectExists(stream, pTable->generateCreateTableSql(), flds, pTable);
    return str;
}
