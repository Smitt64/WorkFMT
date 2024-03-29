#include "pgexportoperation.h"
#include "fmtcore.h"
#include "baseloghighlighter.h"
#include <QProcess>
#include <QDebug>
#include <QTemporaryDir>
#include <QFile>

PgExportOperation::PgExportOperation(DumpToolWizard *wizzard) :
    BaseOperation(wizzard)
{

}

PgExportOperation::~PgExportOperation()
{

}

void PgExportOperation::run()
{
    DumpToolWizard *wzrd = wizzard();

    QString PgExpServer = wzrd->field("PgExpServer").toString();
    quint32 PgExpPort = wzrd->field("PgExpPort").toInt();
    QString PgExpUser = wzrd->field("PgExpUser").toString();
    QString PgExpDatabase = wzrd->field("PgExpDatabase").toString();
    QString PgExpPath = wzrd->field("PgExpPath").toString();
    QString PgBinPath = wzrd->userField("PgBinPath").toString();

    emit stageChanged(QString("Экспорт дампа"));
    m_Proc.reset(new QProcess());

    connect(m_Proc.data(), &QProcess::readyReadStandardOutput, this, &PgExportOperation::readyReadStandardOutput);

    QStringList params = QStringList()
            << "pg_dump"
            << QString("-h")
            << PgExpServer
            << QString("-p")
            << QString::number(PgExpPort)
            << QString("-U")
            << PgExpUser
            << QString("-d")
            << PgExpDatabase
            << "--verbose"
            << "-b"
            << "-N"
            << "sqlj"
            << QString("-f")
            << PgExpPath;

    QDir dir(PgBinPath);
    CoreStartProcess(m_Proc.data(), "fmtdatapumpwrp.exe",
                     params, true, true,
                     std::numeric_limits<int>::max());
// pg_dump -h %1 -p %2 -U %3 -d %4 -b -N sqlj -f .\dump.sql >nul
    emit finished();
}

void PgExportOperation::readyReadStandardOutput()
{
    qint64 bytesAvailable = m_Proc->bytesAvailable();
    QByteArray data = m_Proc->read(bytesAvailable);

    StandardRead(data);
}

void PgExportOperation::StandardRead(QByteArray &data)
{
    QTextStream stream(&data, QIODevice::ReadOnly);
    stream.setCodec("IBM 866");
    while (!stream.atEnd())
    {
        QString msg = stream.readLine();

        if (msg.contains("Error at line ", Qt::CaseInsensitive) || msg.contains("ORA-"))
            emit errormsg(msg);
        else
            emit message(msg);
    }
}

void PgExportOperation::highlighter(BaseLogHighlighter **obj, QTextDocument *parent)
{
    *obj = new BaseLogHighlighter(parent);
    (*obj)->addMarkItem("\\bpg_dump\\b");
    (*obj)->addMarkItem("\\bDUMMY\\b");
    (*obj)->addMarkItem("\\bTYPE\\b");
    (*obj)->addMarkItem("\\bINDEX\\b");
    (*obj)->addMarkItem("\\bPOST\\-DATA\\b");
    (*obj)->addMarkItem("\\bPRE\\-DATA\\b");
    (*obj)->addMarkItem("\\bTABLE\\b");
    (*obj)->addMarkItem("\\bDATA\\b");
    (*obj)->addMarkItem("\\bBOUNDARY\\b");
    (*obj)->addMarkItem("\\bCONSTRAINT\\b");
    (*obj)->addMarkItem("\\bSCHEMA\\b");
    (*obj)->addMarkItem("\\bEXTENSION\\b");
    (*obj)->addMarkItem("\\bCOMMENT\\b");
    (*obj)->addMarkItem("\\bFUNCTION\\b");
    (*obj)->addMarkItem("\\bTRIGGER\\b");
    (*obj)->addMarkItem("\\bFK\\b");
    (*obj)->addMarkItem("\\bCONSTRAINT\\b");
    (*obj)->addMarkItem("\\bACL\\b");
    (*obj)->addMarkItem("\\bVIEW\\b");
    (*obj)->addErrorItem("предупреждение");

    QTextCharFormat format;
    format.setForeground(Qt::darkGray);
    (*obj)->addRule("подробности", format);
}
