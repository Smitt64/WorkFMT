#include "datfilecontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include "toolsruntime.h"
#include <QIcon>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryFile>
#include <QFile>
#include <algorithm>

DatFileContentTreeItem::DatFileContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem)
{

}

DatFileContentTreeItem::~DatFileContentTreeItem()
{

}

QVariant DatFileContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DecorationRole)
            return QIcon("://img/ext_sql.png");
        else if (role == Qt::DisplayRole)
        {
            QFileInfo fi(fileName());

            if (!parentItem()->isShowRowNumber())
                return fi.baseName().toLower() + ".sql";
            else
               return QString("%1_").arg(row() + 1, 2, 10, QChar('0')) + fi.baseName().toLower() + ".sql";
        }
        else if (role == Qt::EditRole)
        {
            QFileInfo fi(fileName());
            return fi.baseName().toLower();
        }
    }
    else if (column == HotfixContentModel::ColumnAction)
    {
        if (role == Qt::DisplayRole)
            return "diff to script";
    }

    return FileContentTreeItem::data(column, role);
}

void DatFileContentTreeItem::setChunks(const QStringList &lst)
{
    m_Chunks = lst;

    for (int i = 0; i < m_Chunks.size(); ++i)
        m_Chunks[i].remove('!');
}

MakeResult DatFileContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (m_Chunks.isEmpty())
        return ResultSuccess;

    MakeResult result = ResultFail;

    QStringList args = { "diff" };
    args.append(m_Chunks);
    /*
    params[PARAM_USER] = wzrd->field("User");
    params[PARAM_PASSWORD] = wzrd->field("Password");
    params[PARAM_SERVICE] = wzrd->field("Service");
    params[PARAM_IS_UNICODE] = wzrd->field("IsUnicode");

    params[PARAM_SOURCE_DIR] = wzrd->field("sourceEdit");
     */

    QScopedPointer<QProcess> process(new QProcess());
    process->setWorkingDirectory(params[PARAM_SOURCE_DIR].toString());
    toolStartProcess(process.data(), "svn.exe", args, true, true);

    QTemporaryFile tmp;
    if (!tmp.open())
    {
        msg = "Ошибка создания временного файла с результатом <b>svn diff</b>";
        return result;
    }

    tmp.write(process->readAllStandardOutput());
    tmp.close();

    QString connectionString = QString("CONNSTRING=dsn=%1;user id=%2;password=%3")
            .arg(params[PARAM_SERVICE].toString(), params[PARAM_USER].toString(), params[PARAM_PASSWORD].toString());

    QFileInfo fi(fileName());
    QDir d(fi.absolutePath());
    QString filename = d.absoluteFilePath(data(0, Qt::DisplayRole).toString());

    QString dat = m_Chunks[0];
    QStringList argtmpl;
    argtmpl << "--delete"
            << "--insert"
            << "--update"
            << "--cs"
            << connectionString
            << "--ora"
            << "--dat"
            << QDir::toNativeSeparators(dat.remove("!"))
            << "--input"
            << tmp.fileName()
            << "--output"
            << filename;

    if (params[PARAM_IS_UNICODE].toBool())
        argtmpl.append("--unicodedb");

    QScopedPointer<QProcess> proc(new QProcess);
    int ExitCode = toolStartProcess(proc.data(), "DiffToScript.exe", argtmpl, true, true);

    if (ExitCode)
    {
        QByteArray errdata = proc->readAllStandardError();
        QTextStream stream(&errdata);
        stream.setCodec("IBM 866");

        msg = stream.readAll();
    }
    else
    {
        msg = QString("Файл скриптов DiffToScript <b>%1</b> успешно создан")
                .arg(QDir::toNativeSeparators(filename));
        result = ResultSuccess;
    }

    return result;
}
