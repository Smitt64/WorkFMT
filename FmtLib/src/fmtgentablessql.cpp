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
    stream << QString("-- %1").arg(pTable->comment()) << endl;
    stream << pTable->generateCreateTableSql().toLocal8Bit() << ";";
    stream << endl << endl;
    stream << pTable->getCommentSql();
    stream << ";" << endl;

    for (qint16 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *fld = pTable->field(i);
        stream << fld->getCommentSql() << ";" << endl;
    }
    stream.flush();

    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(pTable->name())), FormatType});

    return data;
}

FmtGenHighlightingRuleList FmtGenTablesSql::highlightingRuleList() const
{
    return m_HighlightingRuleList;
}
