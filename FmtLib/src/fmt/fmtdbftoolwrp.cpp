#include "fmtdbftoolwrp.h"
#include "connectioninfo.h"
#include "fmterrors.h"
#include "fmtcore.h"
#include <QTextStream>

FmtDbfToolWrp::FmtDbfToolWrp(ConnectionInfo *info, QObject *parent) :
    QObject(parent),
    proc(new QProcess())
{
    codec = QTextCodec::codecForName("IBM 866");
    pInfo = info;
    err = new FmtErrors(this);
    QDir d = QDir::current();
            //(qApp->applicationDirPath());
    proc->setProgram(d.absoluteFilePath("DBFileTool.exe"));
    connect(proc.data(), SIGNAL(readyReadStandardError()), SLOT(readyReadStandardError()));
    connect(proc.data(), SIGNAL(readyReadStandardOutput()), SLOT(readyReadStandardOutput()));
}

FmtDbfToolWrp::~FmtDbfToolWrp()
{
    /*disconnect(&proc, SIGNAL(readyReadStandardError()), Q_NULLPTR, Q_NULLPTR);
    disconnect(&proc, SIGNAL(readyReadStandardOutput()), Q_NULLPTR, Q_NULLPTR);*/
}

FmtErrors *FmtDbfToolWrp::fmterrors()
{
    return err;
}

void FmtDbfToolWrp::setDsn(const QString &dsn)
{
    m_dsn = dsn;
}

void FmtDbfToolWrp::stop()
{
    proc->kill();
}

void FmtDbfToolWrp::readyReadStandardError()
{
    err->appendError(codec->toUnicode(proc->readAllStandardError()));
}

void FmtDbfToolWrp::readyReadStandardOutput()
{
    QString str = codec->toUnicode(proc->readAllStandardOutput());
    err->appendMessage(str);
}

void FmtDbfToolWrp::unload(const QString &ExportDir, const QString &dbt)
{
    QString dsn = m_dsn;
    err->clear();
    QEventLoop loop;
    connect(proc.data(), SIGNAL(finished(int)), &loop, SLOT(quit()));

    if (dsn.isEmpty())
        dsn = DatasourceFromService(pInfo->service());

    QStringList args;
    if (!ExportDir.isEmpty())
        args << "--edir" << ExportDir;
    args << "--cs" << QString("dsn=%1;user id=%2;password=%3").arg(dsn, pInfo->user(), pInfo->password());
    args << "--e" << "--dbt" << dbt;

    proc->setArguments(args);
    CoreStartProcess(proc.data(), proc->program(), proc->arguments());
    //proc.start();

    if (proc->waitForStarted())
        emit started();
    else
    {
        QByteArray data;
        QTextStream stream(&data, QIODevice::WriteOnly);
        stream << proc->program() << endl;

        QString errstr = proc->errorString() + "\n" + proc->program() + "\n";
        foreach (const QString &arg, args) {
            errstr += arg + "\n";
        }
        err->appendError(errstr);
        emit startError();
    }
}

void FmtDbfToolWrp::load(const QString &dbt)
{
    err->clear();
    QEventLoop loop;
    connect(proc.data(), SIGNAL(finished(int)), &loop, SLOT(quit()));

    QStringList args;
    args << "--cs" << QString("dsn=%1;user id=%2;password=%3")
            .arg(DatasourceFromService(pInfo->service()),
                 pInfo->user(),
                 pInfo->password());
    args << "--i" << "--dbt" << dbt;

    proc->setArguments(args);
    CoreStartProcess(proc.data(), proc->program(), proc->arguments());
    //proc.start();

    if (proc->waitForStarted())
        emit started();
    else
    {
        QByteArray data;
        QTextStream stream(&data, QIODevice::WriteOnly);
        stream << proc->program() << endl;

        QString errstr = proc->errorString() + "\n" + proc->program() + "\n";
        foreach (const QString &arg, args) {
            errstr += arg + "\n";
        }
        err->appendError(errstr);
        emit startError();
    }
}

