#include "fmtgentablessql.h"
#include "fmtfield.h"
#include <QTextStream>

FmtGenTablesSql::FmtGenTablesSql()
    : FmtGenInterface()
{

}

FmtGenTablesSql::~FmtGenTablesSql()
{

}

QByteArray FmtGenTablesSql::makeContent(FmtSharedTablePtr pTable)
{
    QByteArray data;
    QTextStream stream(&data, QIODevice::WriteOnly);
    stream << pTable->generateCreateTableSql().toLocal8Bit() << ";";
    stream << endl << endl;
    stream << pTable->getCommentSql();
    stream << ";" << endl;

    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *fld = pTable->field(i);
        stream << fld->getCommentSql() << ";" << endl;
    }
    stream.flush();

    return data;
}
