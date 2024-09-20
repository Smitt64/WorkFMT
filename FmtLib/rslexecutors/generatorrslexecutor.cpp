#include "generatorrslexecutor.h"
#include <QTemporaryFile>
#include <QTextStream>

GeneratorRslExecutor::GeneratorRslExecutor(const QString &filename, FmtTable *pTable)
    : RslExecutor{}
{
    m_pTable = pTable;
    m_Filename = filename;
}

void GeneratorRslExecutor::PlayRepProc()
{
    QVariantList params;
    params << QVariant::fromValue<QObject*>((QObject*)m_pTable);

    (void)call("Execute", params);
}

void GeneratorRslExecutor::execute()
{
    QScopedPointer<QTemporaryFile> report(new QTemporaryFile());
    report->setAutoRemove(false);
    report->open();

    playRep(m_Filename, report->fileName().toLocal8Bit().data(), nullptr);

    QStringList err = errors();
    if (err.isEmpty())
    {
        QTextStream stream(report.data());
        stream.setCodec("IBM 866");

        report->flush();
        m_Data = stream.readAll().toLocal8Bit();
    }
    else
    {
        QTextStream stream(&m_Data, QIODevice::WriteOnly);
        for (const QString &s: err)
            stream << s << "\n";
    }
    report->close();
}

const QByteArray &GeneratorRslExecutor::data() const
{
    return m_Data;
}
