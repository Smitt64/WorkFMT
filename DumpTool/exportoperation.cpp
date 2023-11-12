#include "exportoperation.h"
#include "fmtcore.h"
#include <QProcess>
#include <QFileInfo>
#include <QLockFile>

ExportOperation::ExportOperation(DumpToolWizard *wizzard) :
    BaseOperation(wizzard)
{

}

ExportOperation::~ExportOperation()
{

}

void ExportOperation::run()
{
    DumpToolWizard *wzrd = wizzard();

    QString ExportService = wzrd->field("ExportService").toString();
    QString ExportFrom = wzrd->field("ExportFrom").toString();
    QString ExportPass = wzrd->field("ExportPass").toString();
    QString ExportPath = wzrd->field("ExportPath").toString();

    QDir fileDir(ExportPath);
    QString fileName = QString("%1.dmp").arg(ExportFrom);
    QString absoluteFilePath = QDir::toNativeSeparators(fileDir.absoluteFilePath(fileName));

    if (QFile::exists(absoluteFilePath))
    {
        emit stageChanged(QString("Удаление дампа"));
        emit message(QString("Файл %1 уже существует в каталоге %2, поэтому будет удален и создан заново")
                     .arg(fileName, ExportPath));

        emit progressDialog(tr("Удаление файла %1\n").arg(fileName), -1);

        if (!QFile::remove(absoluteFilePath))
        {
            emit errormsg(QString("Ошибка удаления файла %1\n").arg(fileName));

            QLockFile lf(absoluteFilePath);
            if (lf.isLocked())
                emit errormsg(QString("Файл занят другим процессом\n"));
        }

        emit progressDialogEnd();
    }

    emit stageChanged(QString("Экспорт дампа"));
    m_Proc.reset(new QProcess());

    QStringList params = QStringList()
            << "expdp"
            << QString("%1/%2@%3").arg(ExportFrom, ExportPass, ExportService)
            << QString("directory=%1").arg(wzrd->userValName(ExportPath))
            << QString("logfile=%1.log").arg(ExportFrom)
            << QString("dumpfile=%1.dmp").arg(ExportFrom);

    connect(m_Proc.data(), &QProcess::readyReadStandardOutput, [this]() -> void
    {
        readyReadStandardOutput();
    });

    CoreStartProcess(m_Proc.data(), "fmtdatapumpwrp.exe",
                     params, true, true,
                     std::numeric_limits<int>::max());

    emit finished();
}

void ExportOperation::readyReadStandardOutput()
{
    qint64 bytesAvailable = m_Proc->bytesAvailable();
    QString data = m_Proc->read(bytesAvailable);

    StandardRead(data);
}

void ExportOperation::StandardRead(QString &data)
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
