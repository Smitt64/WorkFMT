#include "importoperation.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include "impfileselectpage.h"
#include <limits>
#include <QFile>
#include <QProcess>
#include <QSqlQuery>
#include <QTemporaryDir>
#include <QFileSystemWatcher>

#define CHUNK_SIZE (1024 * 5)

ImportOperation::ImportOperation(DumpToolWizard *wizzard) :
    BaseOperation(wizzard)
{

}

ImportOperation::~ImportOperation()
{

}

void ImportOperation::run()
{
    bool RecreateUser = true;
    DumpToolWizard *wzrd = wizzard();
//   emit message(tr("<H1>Начало импорта</H1><br>"));

    QString Service = wzrd->field("Service").toString();
    QString SystemName = wzrd->field("SystemName").toString();
    QString SystemPass = wzrd->field("SystemPass").toString();
    QString UserName = wzrd->field("UserName").toString();
    bool DropUser = wzrd->field("DropUser").toBool();
    int Location = wzrd->field("Location").toInt();

    QString PathToDump = wzrd->field("PathToDump").toString();
    QString DumpFile = wzrd->field("DumpFile").toString();
    QString ImportPath = wzrd->field("ImportPath").toString();

    QScopedPointer<ConnectionInfo> connection(new ConnectionInfo());

    QString error;
    if (connection->open(QRSD_DRIVER, SystemName, SystemPass, Service, &error))
    {
        QSqlQuery query(connection->db());
        query.prepare("SELECT count(*) FROM ALL_USERS WHERE USERNAME=UPPER(:1)");
        query.addBindValue(UserName);

        if (!ExecuteQuery(&query) && query.next())
        {
            if (query.value(0).toInt() && !DropUser)
              RecreateUser = false;
        }
    }
    else
    {
        emit errormsg(tr("Ошибка подключения к %1\n").arg(SystemName));
        emit finished();
        return;
    }

    if (RecreateUser)
    {
        try
        {
            RecreateUserProc(Service, SystemName, SystemPass, UserName);
        }
        catch (QString &what)
        {
            emit errormsg(what);
            emit finished();
            return;
        }
    }

    if (Location != ImpFileSelectPage::ImpdpServer)
    {
        try
        {
            CopyDumpFile(PathToDump, DumpFile, ImportPath);
            emit succeeded(QString("Файл %1 успешно скопирован в %2\n")
                           .arg(DumpFile)
                           .arg(ImportPath));
        }
        catch (QString &what)
        {
            emit errormsg(what);
            emit finished();
            return;
        }
    }

    try
    {
        ImportFile();
    }
    catch (QString &what)
    {
        emit errormsg(what);
        emit finished();
        return;
    }


    emit finished();
}

void ImportOperation::RecreateUserProc(const QString &Service,
                                       const QString &SystemName,
                                       const QString &SystemPass,
                                       const QString &UserName)
{
    emit stageChanged(QString("Создание пользователя"));

    QTemporaryDir tmpdir;
    QString cmd = ReadTextFileContent(":/opr/CrUser_V6.cmd")
            .arg(SystemName, SystemPass, Service, UserName);

    if (!UnpackFile(":/opr/10g-recreateUser.sql", tmpdir.filePath("10g-recreateUser.sql")))
        throw QString("Ошибка создания временного файла 10g-recreateUser.sql\n");

    if (cmd.isEmpty())
        throw QString("Ошибка создания временного файла CrUser_V6.cmd\n");

    QFile cmdFile(tmpdir.filePath("CrUser_V6.cmd"));
    if (cmdFile.open(QIODevice::WriteOnly))
    {
        cmdFile.write(cmd.toLocal8Bit());
        cmdFile.close();
    }
    else
        throw QString("Ошибка создания временного файла CrUser_V6.cmd\n");

    m_Proc.reset(new QProcess());
    m_Proc->setWorkingDirectory(tmpdir.path());

    connect(m_Proc.data(), &QProcess::readyReadStandardOutput, [this]() -> void
    {
        readyReadStandardOutput();
    });

    CoreStartProcess(m_Proc.data(), tmpdir.filePath("CrUser_V6.cmd"),
                     QStringList(), true, true,
                     std::numeric_limits<int>::max());
}

void ImportOperation::CopyDumpFile(const QString &PathToDump,
                                   const QString &DumpFile,
                                   const QString &ImportPath)
{
    emit stageChanged(QString("Копирование дампа"));
    QDir src(PathToDump);
    QDir dst(ImportPath);

    QString srcFile = QDir::toNativeSeparators(src.absoluteFilePath(DumpFile));
    QString dstFile = QDir::toNativeSeparators(dst.absoluteFilePath(DumpFile));

    emit message(QString("Исходный файл: %1\n").arg(srcFile));

    if (QFile::exists(dstFile))
    {
        emit message(QString("Файл %1 уже существует в каталоге %2 и будет удален\n")
                     .arg(DumpFile)
                     .arg(ImportPath));

        if (!QFile::remove(dstFile))
            throw QString("Ошибка удаления файла %1\n")
                .arg(dstFile);
    }

    QFile in(srcFile);
    QFile out(dstFile);

    if (!in.open(QIODevice::ReadOnly) || !out.open(QIODevice::WriteOnly))
        throw QString("Ошибка копирования файла %1 из %2 в %3\n")
            .arg(DumpFile, srcFile, dstFile);
    else
    {
        qreal coefficient = 1;
        qint64 inSize = in.size() - 1;
        qint32 newSize = 0;
        qint32 max32 = std::numeric_limits<qint32>::max();

        if (inSize > max32)
            coefficient = ((qreal)max32 / (qreal)inSize);

        emit progressDialog(tr("Копирование файла %1\n").arg(DumpFile), inSize * coefficient);

        while (!in.atEnd())
        {
            QByteArray line = in.readLine();
            out.write(line);

            newSize += line.size() * coefficient;
            emit progressValue(newSize);
        }

        emit progressDialogEnd();
    }

    in.close();
    out.close();
}

void ImportOperation::StandardRead(QString &data)
{
    QTextStream stream(&data, QIODevice::ReadOnly);
    while (!stream.atEnd())
    {
        QString msg = stream.readLine();

        if (msg.contains("Error at line ", Qt::CaseInsensitive) || msg.contains("ORA-"))
            emit errormsg(msg);
        else
            emit message(msg);
    }
}

void ImportOperation::readyReadStandardOutput()
{
    qint64 bytesAvailable = m_Proc->bytesAvailable();
    QString data = m_Proc->read(bytesAvailable);

    StandardRead(data);
}

void ImportOperation::ImportFile()
{
    emit stageChanged(QString("Импорт данных"));
    DumpToolWizard *wzrd = wizzard();

    QString Service = wzrd->field("Service").toString();
    QString SystemName = wzrd->field("SystemName").toString();
    QString SystemPass = wzrd->field("SystemPass").toString();
    QString UserName = wzrd->field("UserName").toString();
    QString SchemeOwner = wzrd->field("SchemeOwner").toString();

    QString DumpFile = wzrd->field("DumpFile").toString();
    QString ImportPath = wzrd->field("ImportPath").toString();

    QDir OraHome = wzrd->userField("OraHome").toString();
    QDir ImpHome(ImportPath);
    OraHome.cd("bin");

    m_Proc.reset(new QProcess());

    QFileInfo fi(DumpFile);
    QStringList params = QStringList()
            << "impdp"
            << QString("%1/%2@%3").arg(SystemName, SystemPass, Service)
            << QString("dumpfile=%1").arg(DumpFile)
            << QString("directory=%1").arg(wzrd->userValName(ImportPath))
            << QString("logfile=%1.log").arg(fi.baseName())
            << QString("remap_schema=%1:%2").arg(SchemeOwner, UserName)
            << QString("TABLE_EXISTS_ACTION=TRUNCATE")
            << QString("parallel=2")
            << QString("TRANSFORM=oid:n");

    connect(m_Proc.data(), &QProcess::readyReadStandardOutput, [this]() -> void
    {
        readyReadStandardOutput();
    });

    CoreStartProcess(m_Proc.data(), "fmtdatapumpwrp.exe",
                     params, true, true,
                     std::numeric_limits<int>::max());
}
