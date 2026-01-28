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

QMap<QString, QByteArray> FmtGenTablesSql::makeContent(FmtSharedTablePtr pTable)
{
    QByteArray data;
    QTextStream stream(&data, QIODevice::WriteOnly);
    stream << QString("-- %1").arg(pTable->comment()) << endl;
    stream << pTable->generateCreateTableSql().toLocal8Bit() << ";";
    stream << endl << endl;

    QString Comment1 = pTable->getCommentSql();
    if (!Comment1.isEmpty())
        stream << Comment1;

    stream << ";" << endl;

    for (qint16 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *fld = pTable->field(i);
        QString Comment2 = fld->getCommentSql();

        if (!Comment2.isEmpty())
            stream << Comment2 << ";" << endl;
    }
    stream.flush();

    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(pTable->name())), FormatElemType});

    return QMap<QString, QByteArray>{{QString(), data}};
}

GenHighlightingRuleList FmtGenTablesSql::highlightingRuleList() const
{
    return m_HighlightingRuleList;
}
