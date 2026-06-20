#include "importobject.h"
#include "export/exporterfactory.h"
#include "export/exporterbase.h"
#include <connectioninfo.h>
#include <fmtcore.h>
#include <toolsruntime.h>
#include <QTextStream>
#include <QFileInfo>

ImportObject::ImportObject(QObject *parent) :
    QObject(parent),
    _unicode(false)
{
}

ImportObject::~ImportObject()
{
}

void ImportObject::setConnectionInfo(const QString &user, const QString &pswd, const QString &dsn, bool unicode)
{
    _user = user;
    _pswd = pswd;
    _dsn = dsn;
    _unicode = unicode;
}

void ImportObject::importTables(const QStringList &datFiles, const QDir &workDir)
{
    for (const QString &datFile : datFiles)
        importTable(datFile, workDir);
}

void ImportObject::importTable(const QString &datFile, const QDir &workDir)
{
    QTextStream stdOutput(stdout);
    stdOutput.setCodec("IBM 866");

    QString filePath = workDir.isRoot() || !workDir.exists()
            ? datFile
            : workDir.absoluteFilePath(datFile);

    QFileInfo info(filePath);
    QString table = info.baseName().toUpper();

    emit importTableStart(table);

    if (!m_conn || !m_conn->isOpen())
    {
        m_conn.reset(new ConnectionInfo());
        QString options;
        if (_unicode)
            options = RSD_UNICODE;

        if (!m_conn->open(QRSD_DRIVER, _user, _pswd, _dsn, options))
        {
            emit procError(QString("Failed to open connection for user %1@%2").arg(_user, _dsn));
            return;
        }
    }

    QScopedPointer<ExporterBase> exporter(ExporterFactory::createExporter(m_conn.data(), this));
    if (!exporter)
    {
        emit procError(QString("Unsupported database type for import: %1").arg(m_conn->type()));
        return;
    }

    exporter->setConnectionInfo(m_conn.data());
    exporter->setOutputDirectory(workDir);

    connect(exporter.data(), &ExporterBase::procMessage, this, &ImportObject::procMessage);
    connect(exporter.data(), &ExporterBase::error, this, [this](const QString &msg) {
        emit procError(msg);
    });
    connect(exporter.data(), &ExporterBase::importProgress, this, [this](int current, int total) {
        emit progress(current, total);
    });

    exporter->importTable(filePath);
}
