#include "fmtdbftoolwrp.h"
#include "connectioninfo.h"
#include "fmterrors.h"
#include "fmtcore.h"

FmtDbfToolWrp::FmtDbfToolWrp(ConnectionInfo *info, QObject *parent) : QObject(parent)
{
    codec = QTextCodec::codecForName("IBM 866");
    pInfo = info;
    err = new FmtErrors(this);
    QDir d(qApp->applicationDirPath());
    proc.setProgram(d.absoluteFilePath("DBFileTool.exe"));

    qDebug() << d.absoluteFilePath("DBFileTool.exe");
    connect(&proc, SIGNAL(readyReadStandardError()), SLOT(readyReadStandardError()));
    connect(&proc, SIGNAL(readyReadStandardOutput()), SLOT(readyReadStandardOutput()));
}

FmtDbfToolWrp::~FmtDbfToolWrp()
{

}

FmtErrors *FmtDbfToolWrp::fmterrors()
{
    return err;
}

void FmtDbfToolWrp::stop()
{
    proc.kill();
}

void FmtDbfToolWrp::readyReadStandardError()
{
    err->appendError(codec->toUnicode(proc.readAllStandardError()));
}

void FmtDbfToolWrp::readyReadStandardOutput()
{
    QString str = codec->toUnicode(proc.readAllStandardOutput());
    err->appendMessage(str);
}

void FmtDbfToolWrp::unload(const QString &ExportDir, const QString &dbt)
{
    err->clear();
    QEventLoop loop;
    connect(&proc, SIGNAL(finished(int)), &loop, SLOT(quit()));

    QStringList args;
    if (!ExportDir.isEmpty())
        args << "--edir" << ExportDir;
    args << "--cs" << QString("dsn=%1;user id=%2;password=%3")
            .arg(DatasourceFromService(pInfo->service()),
                 pInfo->user(),
                 pInfo->password());
    args << "--e" << "--dbt" << dbt;

    proc.setArguments(args);
    proc.start();

    if (proc.waitForStarted())
    {
        emit started();
        //loop.exec();
    }
}
