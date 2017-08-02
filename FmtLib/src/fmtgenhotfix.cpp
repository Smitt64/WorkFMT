#include "fmtgenhotfix.h"
#include "fmtfield.h"
#include "fmtindex.h"
#include "fmttable.h"
#include "connectioninfo.h"
#include "fmtsegment.h"
#include "fmtcore.h"

FmtGenHotFix::FmtGenHotFix()
{

}

QByteArray FmtGenHotFix::makeContent(QSharedPointer<FmtTable> pTable)
{
    QByteArray data;
    QTextStream stream(&data, QIODevice::WriteOnly);
    QString lowerTableName = pTable->name().toLower();

    stream << "DECLARE " << endl;
    stream << "    m_FmtID NUMBER;" << endl;
    stream << "    m_FmtFldID NUMBER; " << endl;
    stream << "BEGIN" << endl;
    stream << QString::fromUtf8("    -- удаляем имеющееся описание полей таблицы") << endl;
    stream << "    DELETE FROM fmt_fields ff  " << endl;
    stream << QString("    WHERE ff.t_FmtID IN (SELECT fn.t_ID FROM fmt_names fn WHERE LOWER(fn.t_Name) = '%1'); ").arg(lowerTableName) << endl;
    stream << endl;
    stream << QString::fromUtf8("    -- удаляем имеющееся описание ключей таблицы") << endl;
    stream << "    DELETE FROM fmt_keys fk " << endl;
    stream << QString("    WHERE fk.t_FmtID IN (SELECT fn.t_ID FROM fmt_names fn WHERE LOWER(fn.t_Name) = '%1');").arg(lowerTableName) << endl;

    stream << endl;
    stream << QString::fromUtf8("    -- удаляем имеющееся описание таблицы ") << endl;
    stream << QString("    DELETE FROM fmt_names fn WHERE LOWER(fn.t_Name) = '%1'; ").arg(lowerTableName) << endl;
    stream << endl;

    stream << "    SELECT MAX(t_ID) INTO m_FmtID FROM fmt_names;" << endl;
    stream << "    SELECT MAX(t_ID) INTO m_FmtFldID FROM fmt_fields;" << endl;
    stream << endl;

    stream << QString::fromUtf8("    -- добавляем информацию о таблице") << endl;
    stream << "    m_FmtID := m_FmtID + 1; " << endl;
    stream << "    INSERT INTO FMT_NAMES(T_ID, T_NAME, T_OWNER, T_CACHESIZE, T_PKIDX, T_BLOBTYPE, T_BLOBLEN, T_FLAGS, T_COMMENT) " << endl;
    stream << QString("    VALUES(m_FmtID, '%1', '%2', %3, %4, %5, %6, %7, '%8'); ")
              .arg(pTable->name())
              .arg(pTable->owner())
              .arg(pTable->cacheSize())
              .arg(pTable->pkIDx())
              .arg(pTable->blobType())
              .arg(pTable->blobLen())
              .arg(pTable->tableFlags())
              .arg(pTable->comment())
           << endl;

    stream << endl;
    stream << QString::fromUtf8("    -- добавляем информацию полях") << endl;

    for(int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *f = pTable->field(i);
        stream << "    m_FmtFldID := m_FmtFldID + 1; " << endl;
        stream << "    INSERT INTO FMT_FIELDS(T_ID, T_FMTID, T_NAME, T_TYPE, T_SIZE, T_OFFSET, T_OUTLEN, T_DECPOINT, T_HIDDEN, T_COMMENT) " << endl;
        stream << QString("    VALUES(m_FmtFldID, m_FmtID, '%1', %2, %3, %4, %5, %6, %7, '%8');")
                  .arg(f->name())
                  .arg(f->type())
                  .arg(f->size())
                  .arg(f->offset())
                  .arg(f->outlen())
                  .arg(f->decpoint())
                  .arg("CHR(0)")
                  .arg(f->comment())
               << endl;
        stream << endl;
    }

    stream << QString::fromUtf8("    -- добавляем информацию о ключах") << endl;
    for(int i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *f = pTable->tableIndex(i);
        for (int j = 0; j < f->segmentsCount(); j++)
        {
            FmtSegment *segment = f->segment(j);
            stream << QString("    SELECT t_ID INTO m_FmtFldID FROM fmt_fields WHERE t_FmtID = m_FmtID AND LOWER(t_Name) = '%1';").arg(segment->field()->name().toLower())
                   << endl;
            stream << "    INSERT INTO FMT_KEYS(T_FMTID, T_KEYNUM, T_SEGNUM, T_FMTFLDID, T_FLAGS, T_TYPE, T_NULLVAL, T_ISREAL, T_COMMENT)" << endl;
            stream << QString("    VALUES(m_FmtID, %1, %2, m_FmtFldID, %3, %4, %5, %6, '%7');")
                      .arg(i)
                      .arg(j)
                      .arg(segment->segmentFlags())
                      .arg(segment->type())
                      .arg(f->nullValue())
                      .arg(segment->isReal() ? 1 : 0)
                      .arg(segment->comment())
                   << endl;
            stream << endl;
        }
    }

    stream << endl;
    stream << "EXCEPTION " << endl;
    stream << "    WHEN OTHERS THEN NULL; " << endl;
    stream << "END; " << endl;
    stream << "/" << endl;

    stream << endl;
    stream << "COMMIT;" << endl;
    stream << endl;

    stream << QString::fromUtf8("-- блок создания таблицы ") << endl;
    WrapSqlBlock(stream, pTable->generateCreateTableSql());
    for(int i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *f = pTable->tableIndex(i);
        QSqlQuery q(pTable->connection()->db());
        q.prepare(QString("SELECT DBMS_METADATA.GET_DDL('INDEX','%1') from DUAL")
                  .arg(f->name()));

        if (!ExecuteQuery(&q) && q.next())
        {
            stream << endl;
            stream << QString::fromUtf8("-- блок создания ключа") << endl;
            QString str = q.value(0).toString().split(QRegExp("\n|\r\n|\r")).at(1);
            WrapSqlBlock(stream, str);
            stream << endl;
        }
    }

    for(int i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *f = pTable->tableIndex(i);

        if (f->isAutoInc())
        {
            QSqlQuery q(pTable->connection()->db());
            QString seqname = QString("%1_SEQ").arg(pTable->name()).toUpper();
            q.prepare(QString("SELECT DBMS_METADATA.GET_DDL('SEQUENCE','%1') from DUAL")
                      .arg(seqname));

            if (!ExecuteQuery(&q) && q.next())
            {
                stream << QString::fromUtf8("-- блок создания посоледовательностей") << endl;
                QString str = q.value(0).toString();
                str = str.mid(0, str.indexOf("START WITH"));
                WrapSqlBlock(stream, str);
                stream << endl;
            }
        }
    }


    QSqlQuery q(pTable->connection()->db());
    q.prepare("SELECT TRIGGER_NAME, DESCRIPTION, TRIGGER_BODY FROM all_triggers WHERE owner = ? AND table_name = ?");
    q.bindValue(0, pTable->connection()->user());
    q.bindValue(1, pTable->name().toUpper());
    if (!ExecuteQuery(&q) && q.next())
    {
        enum
        {
            fld_TRIGGER_NAME,
            fld_DESCRIPTION,
            fld_TRIGGER_BODY
        };
        stream << QString::fromUtf8("-- блок создания триггера") << endl;
        stream << QString("CREATE OR REPLACE TRIGGER %1").arg(q.value(fld_TRIGGER_NAME).toString()) << endl;
        QString str = q.value(fld_DESCRIPTION).toString();
        str = str.mid(q.value(fld_TRIGGER_NAME).toString().length()).simplified();
        stream << str << endl;
        stream << q.value(fld_TRIGGER_BODY).toString();
        stream << "/" << endl;
        stream << endl;
    }

    return data;
}

void FmtGenHotFix::WrapSqlBlock(QTextStream &stream, const QString &block)
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

    stream << endl;
    stream << "EXCEPTION " << endl;
    stream << "    WHEN e_object_exists THEN NULL; " << endl;
    stream << "END;" << endl;
    stream << "/" << endl;
}

QString FmtGenHotFix::Simplify(QString line)
{
    QString str = line;
    str = str.remove(QRegExp("(\\\"([\\w]+)\\\")\\."));
    line.replace("'", "''");
    str = str.remove("\"");
    return str.simplified();
}
