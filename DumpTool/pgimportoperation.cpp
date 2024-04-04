#include "pgimportoperation.h"
#include "baseloghighlighter.h"
#include "fmtcore.h"
#include "toolsruntime.h"
#include <QTemporaryDir>
#include <QFileInfo>
#include <QRegExp>

PgImportOperation::PgImportOperation(DumpToolWizard *wizzard) :
    BaseOperation(wizzard)
{
    setAutoDelete(false);
}

PgImportOperation::~PgImportOperation()
{

}

#define ExecQuery(command) ExecQueryCommand(PgImpServer, PgImpPort, PgImpAdmin, QString(), command)
#define ExecQueryD(command) ExecQueryCommand(PgImpServer, PgImpPort, PgImpAdmin, PgImpDstUser, command)
void PgImportOperation::run()
{
    DumpToolWizard *wzrd = wizzard();

    QString PgImpServer = wzrd->field("PgImpServer").toString();
    quint32 PgImpPort = wzrd->field("PgImpPort").toInt();
    QString PgImpAdmin = wzrd->field("PgImpAdmin").toString();
    QString PgImpOwner = wzrd->field("PgImpOwner").toString();
    QString PgImpPath = wzrd->field("PgImpPath").toString();
    QString PgImpDstUser = wzrd->field("PgImpDstUser").toString();
    QStringList PgJarFiles = wzrd->userField("PgJarFiles").toStringList();

    qDebug() << PgJarFiles;
    emit stageChanged(QString("Экспорт дампа"));

    if (!toolGetPostgreSQLInstallLocation(m_PostgresDir))
    {
        emit errormsg(tr("Не найден установленный клиент PostgresqSQL"));
        emit finished();
        return;
    }

    int stat = 0;

    try
    {
        emit stageChanged(QString("Удаление предыдущей базы данных"));
        stat = ExecQuery(QString("DROP DATABASE IF EXISTS %1").arg(PgImpDstUser));

        if (stat)
        {
            ExecQuery(QString("SELECT pid, application_name, client_addr FROM pg_stat_activity WHERE datname = '%1'").arg(PgImpDstUser));
            throw tr("Ошибка удаления предыдущей базы данных [%1]").arg(PgImpDstUser);
        }

        emit stageChanged(QString("Создание новой базы данных"));
        stat = CreateDatabase(PgImpServer, PgImpPort, PgImpAdmin, PgImpDstUser);

        if (stat)
            throw tr("Ошибка создания новой базы данных \"%1\"").arg(PgImpDstUser);

        emit stageChanged(QString("Установка и активация расширения PL/Java"));
        stat = ExecQueryD(QString("CREATE EXTENSION pljava"));

        if (stat)
            throw tr("Ошибка установки и активации расширения PL/Java");

        emit stageChanged(QString("Установка и активация расширения pg_variables"));
        stat = ExecQueryD(QString("CREATE EXTENSION pg_variables"));

        if (stat)
            throw tr("Ошибка установки и активации расширения pg_variables");

        emit stageChanged(QString("Создание и настройка роли"));

        QString createRole = toolReadTextFileContent(":/res/CreateRole.sql");
        stat = ExecQueryD(createRole.arg(PgImpOwner));
        if (stat) throw stat;

        stat = ExecQueryD(QString("ALTER ROLE %1 WITH NOSUPERUSER INHERIT NOCREATEROLE NOCREATEDB LOGIN NOREPLICATION NOBYPASSRLS PASSWORD '%1' PROFILE \"default\"").arg(PgImpOwner));
        if (stat) throw stat;

        stat = ExecQueryD(QString("GRANT ALL ON DATABASE %1 TO %2").arg(PgImpDstUser, PgImpOwner));
        if (stat) throw stat;

        stat = ExecQueryD(QString("GRANT ALL ON TABLESPACE users TO %1").arg(PgImpOwner));
        if (stat) throw stat;

        stat = ExecQueryD(QString("GRANT ALL ON TABLESPACE indx TO %1").arg(PgImpOwner));
        if (stat) throw stat;

        stat = ExecQueryD(QString("GRANT ALL ON ALL TABLES IN SCHEMA public TO %1").arg(PgImpOwner));
        if (stat) throw stat;

        emit stageChanged(QString("Загрузка jar файлов"));
        stat = LoadJars(PgImpServer, PgImpPort, PgImpAdmin, PgImpDstUser, PgJarFiles);
        if (stat) throw stat;

        emit stageChanged(QString("Загрузка файла дампа"));
        stat = ExecQueryFile(PgImpServer, PgImpPort, PgImpAdmin, PgImpDstUser, PgImpPath);
        if (stat) throw stat;

        if (!stat)
            stat = ExecQueryD(QString("select * from sqlj.jar_repository"));
    }
    catch (QString e)
    {
        emit errormsg(e);
    }
    catch(...)
    {
        emit finished();
    }

    emit finished();
}

QStringList PgImportOperation::QueryCommandParams(const QString &server,
                   const quint32 &port,
                   const QString &admin,
                   const QString &database)
{
    QStringList args =
    {
        "-h",
        server,
        "-p",
        QString::number(port),
        "-U",
        admin,
        "--echo-queries",
        "--echo-hidden"
    };

    if (!database.isEmpty())
    {
        args.append("-d");
        args.append(database);
    }

    return args;
}

int PgImportOperation::ExecQueryCommand(const QString &server,
                                        const quint32 &port,
                                        const QString &admin,
                                        const QString &database,
                                        const QString &command)
{
    ResetProcess();

    QStringList args = QueryCommandParams(server, port, admin, database);
    args.append({"-c", QString("%1").arg(command)});

    int stat = CoreStartProcess(m_Proc.data(), m_PostgresDir.absoluteFilePath("psql.exe"),
                                args, true, true,
                                std::numeric_limits<int>::max(),
                                true);

    return stat;
}

int PgImportOperation::ExecQueryFile(const QString &server,
                     const quint32 &port,
                     const QString &admin,
                     const QString &database,
                     const QString &filename)
{
    ResetProcess();

    QStringList args = QueryCommandParams(server, port, admin, database);
    args.append({"-f", QString("%1").arg(filename)});

    return CoreStartProcess(m_Proc.data(), m_PostgresDir.absoluteFilePath("psql.exe"),
                     args, true, true,
                     std::numeric_limits<int>::max());
}

int PgImportOperation::CreateDatabase(const QString &server,
                                      const quint32 &port,
                                      const QString &admin,
                                      const QString &database)
{
    ResetProcess();

    QStringList args =
    {
        "-h",
        server,
        "-p",
        QString::number(port),
        "-U",
        admin,
        "-T",
        "template0",
        "-E",
        "UTF8",
        "-D",
        "users",
        "--lc-collate",
        "C",
        "--lc-ctype",
        "ru_RU.UTF-8",
        "--echo",
        "--locale-provider",
        "libc",
        database
    };

    return CoreStartProcess(m_Proc.data(), m_PostgresDir.absoluteFilePath("createdb.exe"),
                     args, true, true,
                     std::numeric_limits<int>::max());
}

int PgImportOperation::LoadJars(const QString &server,
                                const quint32 &port,
                                const QString &admin,
                                const QString &database,
                                const QStringList &PgJarFiles)
{
    QTemporaryDir temdir;

    QString PLXmlFunctions = temdir.filePath("PLXmlFunctions.jar");
    if (!toolSaveResourceToFile(":/res/PLXmlFunctions.jar", PLXmlFunctions))
    {
        emit errormsg(QString("Не удалось распаковать PLXmlFunctions.jar в %1").arg(temdir.path()));
        return 1;
    }

    int stat = LoadJarFile(server, port, admin, database, PLXmlFunctions);

    if (!stat)
    {
        QStringList::ConstIterator iter = PgJarFiles.begin();
        while (!stat && iter != PgJarFiles.end())
        {
            stat = LoadJarFile(server, port, admin, database, *iter);
            ++iter;
        }
    }

    return stat;
}

bool PgImportOperation::CanProcessJarFile()
{
    if (lo_import.isEmpty())
    {
        /*m_Proc->waitForReadyRead();*/
        QString str = QString::fromLocal8Bit(m_Proc->readAllStandardOutput());

        QRegExp rx("lo_import\\s+(\\d+)");

        int pos = rx.indexIn(lo_import);
        if (pos != -1)
            lo_import = rx.cap(1);

        qDebug() << m_Proc->readAllStandardError() << m_Proc->readAllStandardOutput();

        if (lo_import.isEmpty())
            return false;
    }

    return true;
}

int PgImportOperation::LoadJarFile(const QString &server,
                                   const quint32 &port,
                                   const QString &admin,
                                   const QString &database,
                                   const QString &filename)
{
    QFileInfo fi(filename);
    lo_import = "";

    //ResetProcess();
    QStringList args = QueryCommandParams(server, port, admin, database);
    args.append({"-c", QString("\\lo_import %1").arg(filename)});
    args.removeOne("--echo-queries");
    args.removeOne("--echo-hidden");

    /*int stat = CoreStartProcess(m_Proc.data(), m_PostgresDir.absoluteFilePath("psql.exe"),
                                args, true, true,
                                std::numeric_limits<int>::max(),
                                true);*/

    m_Proc.reset(new QProcess());
    m_Proc->start( m_PostgresDir.absoluteFilePath("psql.exe"), args);
    m_Proc->waitForStarted();
    m_Proc->waitForReadyRead();

    QString str = QString::fromLocal8Bit(m_Proc->readAllStandardOutput());
    QRegExp rx("lo_import\\s+(\\d+)");

    int pos = rx.indexIn(str);
    if (pos != -1)
        lo_import = rx.cap(1);

    emit message(QString("\\lo_import %1").arg(filename));
    emit message(str.simplified());
    m_Proc->waitForFinished();

    int stat = 0;
    if (!stat && CanProcessJarFile())
    {
        stat = ExecQueryCommand(server, port, admin, database,
                        QString("SELECT sqlj.install_jar(lo_get(%1), '%2', true)")
                                .arg(lo_import)
                                .arg(fi.baseName()));
    }
    else
        stat = 1;

    if (!stat && CanProcessJarFile())
    {
        stat = ExecQueryCommand(server, port, admin, database,
                        QString("select sqlj.set_classpath('public', '%1')")
                                .arg(fi.baseName()));
    }
    else
        stat = 1;

    if (!stat && CanProcessJarFile())
    {
        stat = ExecQueryCommand(server, port, admin, database,
                        QString("\\lo_unlink %1")
                                .arg(lo_import));
    }
    else
        stat = 1;

    return stat;
}

void PgImportOperation::ResetProcess()
{
    m_Proc.reset(new QProcess());
    m_Proc->setWorkingDirectory(m_PostgresDir.path());
    connect(m_Proc.data(), &QProcess::readyReadStandardOutput, this, &PgImportOperation::readyReadStandardOutput);
    connect(m_Proc.data(), &QProcess::readyReadStandardError, this, &PgImportOperation::readyReadStandardError);
}

void PgImportOperation::readyReadStandardOutput()
{
    //qDebug() << "readyReadStandardOutput";
    m_Proc->setReadChannel(QProcess::StandardOutput);
    qint64 bytesAvailable = m_Proc->bytesAvailable();
    QByteArray data = m_Proc->read(bytesAvailable);

    StandardRead(data);
}

void PgImportOperation::readyReadStandardError()
{
    //qDebug() << "readyReadStandardError";

    m_Proc->setReadChannel(QProcess::StandardError);
    qint64 bytesAvailable = m_Proc->bytesAvailable();
    QByteArray data = m_Proc->read(bytesAvailable);

    StandardRead(data, "System");
}

void PgImportOperation::StandardRead(QByteArray &data, const QString &encode)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec *system = QTextCodec::codecForLocale();
    QList<QByteArray> lines = data.split('\n');

    if (data.contains("lo_import"))
    {
        QString str = QString::fromLocal8Bit(data);

        QRegExp rx("lo_import\\s+(\\d+)");

        int pos = rx.indexIn(lo_import);
        if (pos != -1)
            lo_import = rx.cap(1);
    }

    QString text, full_msg;
    bool isFirst = true;
    for (const QByteArray &line : lines)
    {
        QString unicodetext = codec->toUnicode(line, line.size(), &state);
        if (state.invalidChars > 0)
        {
            QString systemtext = system->toUnicode(line, line.size(), &state);

            int pos_dot = unicodetext.indexOf(":");
            if (state.invalidChars > 0)
            {
                if (pos_dot != -1)
                {
                    text = systemtext.mid(0, pos_dot);
                    text += unicodetext.mid(pos_dot);
                }
                else
                    text = systemtext;
            }
            else
                text = systemtext;
        }
        else
            text = unicodetext;

        QString msg = text.remove('\r').replace("\\r", "  ");

        if (!isFirst)
            full_msg += "\n";

        full_msg += msg;
        isFirst = false;
    }

    emit message(full_msg);
}

void PgImportOperation::highlighter(BaseLogHighlighter **obj, QTextDocument *parent)
{
    *obj = new BaseLogHighlighter(parent);
    (*obj)->addMarkItem("ПОДРОБНОСТИ\\:");
    (*obj)->addMarkItem("ПОДСКАЗКА\\:");
    (*obj)->addErrorItem("ОШИБКА\\:");
    (*obj)->addErrorItem("ПРЕДУПРЕЖДЕНИЕ\\:");

    QTextCharFormat format;
    format.setForeground(Qt::darkGray);
    (*obj)->addRule("\\(\\d+ строки\\)", format);
}
