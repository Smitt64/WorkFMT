#include "svnlogmodel.h"
#include "fmtcore.h"
#include <QProcess>
#include <QDomDocument>
#include <QFileIconProvider>
#include <QRegularExpression>

SvnInfoMap SvnGetRepoInfo(const QString &path)
{
    SvnInfoMap info;

    QProcess proc;
    proc.setWorkingDirectory(path);

    CoreStartProcess(&proc, "svn.exe", {"info", "--xml"}, true, true, 30000, true);

    QByteArray data = proc.readAllStandardOutput();
    QDomDocument doc;
    doc.setContent(data);

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChildElement("entry").firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if (e.tagName() != "wc-info")
            info.insert(e.tagName(), e.text());

        n = n.nextSibling();
    }

    return info;
}

// -----------------------------------------

SvnLogModel::SvnLogModel(QObject *parent) :
    QAbstractTableModel(parent), m_VcsType(SvnSatatusModel::VcsType::Auto)
{
    m_Limit = 100;
}

SvnLogModel::~SvnLogModel()
{

}

int SvnLogModel::columnCount(const QModelIndex &parent) const
{
    return fld_Count;
}

QModelIndex SvnLogModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex SvnLogModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int SvnLogModel::rowCount(const QModelIndex &parent) const
{
    return m_Elements.size();
}

QVariant SvnLogModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == fld_revision)
            return m_Elements[index.row()].revision;
        else if (index.column() == fld_author)
            return m_Elements[index.row()].author;
        else if (index.column() == fld_date)
            return m_Elements[index.row()].date.toString();
        else if (index.column() == fld_scr)
            return m_Elements[index.row()].scr;
        else if (index.column() == fld_message)
            return m_Elements[index.row()].message;
    }
    else if (role == Qt::SizeHintRole)
    {
        return QSize(0, 16);
    }
    return QVariant();
}

QVariant SvnLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList lst
    {
        tr("Ревизия"), tr("Автор"), tr("Дата"), tr("Запрос"), tr("Описание")
    };

    if (role == Qt::DisplayRole)
        return lst[section];

    return QAbstractItemModel::headerData(section, orientation, role);
}

const SvnLogElement &SvnLogModel::element(const int &row) const
{
    return m_Elements[row];
}

void SvnLogModel::setVcsType(SvnSatatusModel::VcsType type)
{
    m_VcsType = type;
}

void SvnLogModel::setPath(const QString &path, const QString &url)
{
    m_Path = path;
    m_Url = url;
}

void SvnLogModel::setDateRange(const QDate &from, const QDate &to)
{
    m_From = from;
    m_To = to;
}

void SvnLogModel::refresh()
{
    beginResetModel();
    m_Elements.clear();

    SvnSatatusModel::VcsType actualVcsType = m_VcsType;

    // Если установлен авторежим, определяем тип VCS автоматически
    if (m_VcsType == SvnSatatusModel::VcsType::Auto)
    {
        actualVcsType = detectVcsType(m_Path);
    }

    if (actualVcsType == SvnSatatusModel::VcsType::Svn)
    {
        refreshSvn();
    }
    else if (actualVcsType == SvnSatatusModel::VcsType::Git)
    {
        refreshGit();
    }

    endResetModel();
}

SvnSatatusModel::VcsType SvnLogModel::detectVcsType(const QString &path)
{
    QDir dir(path);

    // Проверяем наличие каталога .git
    if (dir.exists(".git"))
    {
        return SvnSatatusModel::VcsType::Git;
    }

    // Проверяем наличие каталога .svn
    if (dir.exists(".svn"))
    {
        return SvnSatatusModel::VcsType::Svn;
    }

    // Рекурсивно проверяем родительские каталоги для Git
    QString currentPath = path;
    while (!currentPath.isEmpty() && QDir(currentPath).exists())
    {
        QDir currentDir(currentPath);
        if (currentDir.exists(".git"))
        {
            return SvnSatatusModel::VcsType::Git;
        }

        // Для SVN проверяем только текущий каталог (не рекурсивно)
        if (currentPath == path && currentDir.exists(".svn"))
        {
            return SvnSatatusModel::VcsType::Svn;
        }

        // Поднимаемся на уровень выше
        QString parentPath = QDir(currentPath).absolutePath();
        if (parentPath == currentPath) // Достигли корня
            break;

        currentPath = QDir(currentPath).absoluteFilePath("..");
        if (currentPath == parentPath) // Достигли корня
            break;
    }

    return SvnSatatusModel::VcsType::None;
}

void SvnLogModel::refreshSvn()
{
    QProcess proc;
    proc.setWorkingDirectory(m_Path);

    QStringList args = {"log", "--xml"};

    if (m_From.isValid() && m_To.isValid())
    {
        args.append("-r");
        args.append(QString("{%1}:{%2}").arg(m_From.toString("yyyy-MM-dd"), m_To.toString("yyyy-MM-dd")));
    }
    else
    {
        args.append("--limit");
        args.append(m_Limit.toString());
    }

    args.append(m_Url);

    CoreStartProcess(&proc, "svn.exe", args, true, true, 30000, true);

    QByteArray data = proc.readAllStandardOutput();
    QDomDocument doc;
    doc.setContent(data);

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if (e.nodeName() == "logentry")
        {
            SvnLogElement element;
            element.revision = e.attribute("revision").toInt();
            element.author = e.firstChildElement("author").text();
            element.date = QDateTime::fromString(e.firstChildElement("date").text(),
                                                 Qt::ISODateWithMs);
            element.message = e.firstChildElement("msg").text();

            if (element.message.contains("SCR#"))
            {
                int pos = element.message.indexOf("\n");

                element.scr = element.message.mid(5, pos - 5);
                element.message = element.message.mid(pos + 1);
            }
            m_Elements.append(element);
        }
        n = n.nextSibling();
    }
}

void SvnLogModel::refreshGit()
{
    QProcess proc;
    proc.setWorkingDirectory(m_Path);

    QStringList args = {"log", "--pretty=format:%H|%an|%ad|%s", "--date=iso"};

    // Добавляем ограничение по количеству коммитов
    if (!m_From.isValid() || !m_To.isValid())
    {
        args.append("--max-count");
        args.append(m_Limit.toString());
    }

    // Добавляем фильтр по дате если указан
    if (m_From.isValid() && m_To.isValid())
    {
        args.append("--since");
        args.append(m_From.toString("yyyy-MM-dd"));
        args.append("--until");
        args.append(m_To.toString("yyyy-MM-dd 23:59:59"));
    }

    CoreStartProcess(&proc, "git.exe", args, true, true, 30000, true);

    QByteArray data = proc.readAllStandardOutput();
    QString output = QString::fromUtf8(data);
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    int revisionCounter = 1;

    for (const QString &line : lines)
    {
        if (line.trimmed().isEmpty()) continue;

        QStringList parts = line.split('|');
        if (parts.size() >= 4)
        {
            SvnLogElement element;
            // Для Git используем последовательные номера как ревизии
            element.revision = revisionCounter++;
            element.author = parts[1];
            element.date = QDateTime::fromString(parts[2], Qt::ISODate);
            element.message = parts[3];

            // Извлекаем SCR номер из сообщения если есть
            QRegularExpression scrRegex("SCR#(\\d+)");
            QRegularExpressionMatch match = scrRegex.match(element.message);
            if (match.hasMatch())
            {
                element.scr = match.captured(1);
            }

            m_Elements.append(element);
        }
    }
}

// ----------------------------------------------------------------------

SvnLogItemsModel::SvnLogItemsModel(QObject *parent) :
    QAbstractTableModel(parent), m_VcsType(SvnSatatusModel::VcsType::Auto)
{
}

SvnLogItemsModel::~SvnLogItemsModel()
{

}

int SvnLogItemsModel::columnCount(const QModelIndex &parent) const
{
    return fld_Count;
}

QModelIndex SvnLogItemsModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex SvnLogItemsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int SvnLogItemsModel::rowCount(const QModelIndex &parent) const
{
    return m_Elements.size();
}

QVariant SvnLogItemsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == fld_name)
            return m_Elements[index.row()].path;
        else if (index.column() == fld_action)
        {
            const QString action = m_Elements[index.row()].action;

            if (!action.compare("A", Qt::CaseInsensitive) || !action.compare("added", Qt::CaseInsensitive))
                return "Added";
            else if (!action.compare("M", Qt::CaseInsensitive) || !action.compare("modified", Qt::CaseInsensitive))
                return "Modified";
            else if (!action.compare("D", Qt::CaseInsensitive) || !action.compare("deleted", Qt::CaseInsensitive))
                return "Deleted";
            else if (!action.compare("R", Qt::CaseInsensitive) || !action.compare("renamed", Qt::CaseInsensitive))
                return "Renamed";
            else
                return action;
        }
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == fld_name)
        {
            QDir path(m_Path);
            QFileIconProvider provider;

            if (m_Elements[index.row()].kind == "dir")
                return provider.icon(QFileIconProvider::Folder);

            return provider.icon(QFileInfo(path.absoluteFilePath(m_Elements[index.row()].path)));
        }
    }
    else if (role == Qt::SizeHintRole)
    {
        return QSize(0, 16);
    }
    return QVariant();
}

QVariant SvnLogItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList lst
    {
        tr("Действие"), tr("Файл")
    };

    if (role == Qt::DisplayRole)
        return lst[section];

    return QAbstractItemModel::headerData(section, orientation, role);
}

const SvnLogInfoElement &SvnLogItemsModel::element(const int &row) const
{
    return m_Elements[row];
}

void SvnLogItemsModel::setVcsType(SvnSatatusModel::VcsType type)
{
    m_VcsType = type;
}

void SvnLogItemsModel::setPath(const QString &path, const QString &url)
{
    m_Path = path;
    m_Url = url;
}

void SvnLogItemsModel::refresh(const QString &revision)
{
    beginResetModel();
    m_Elements.clear();

    SvnSatatusModel::VcsType actualVcsType = m_VcsType;

    if (m_VcsType == SvnSatatusModel::VcsType::Auto)
    {
        actualVcsType = detectVcsType(m_Path);
    }

    if (actualVcsType == SvnSatatusModel::VcsType::Svn)
    {
        refreshSvn(revision);
    }
    else if (actualVcsType == SvnSatatusModel::VcsType::Git)
    {
        refreshGit(revision);
    }

    endResetModel();
}

SvnSatatusModel::VcsType SvnLogItemsModel::detectVcsType(const QString &path)
{
    // Та же логика определения, что и в SvnLogModel
    QDir dir(path);

    if (dir.exists(".git")) return SvnSatatusModel::VcsType::Git;
    if (dir.exists(".svn")) return SvnSatatusModel::VcsType::Svn;

    QString currentPath = path;
    while (!currentPath.isEmpty() && QDir(currentPath).exists())
    {
        QDir currentDir(currentPath);
        if (currentDir.exists(".git")) return SvnSatatusModel::VcsType::Git;
        if (currentPath == path && currentDir.exists(".svn")) return SvnSatatusModel::VcsType::Svn;

        QString parentPath = QDir(currentPath).absolutePath();
        if (parentPath == currentPath) break;
        currentPath = QDir(currentPath).absoluteFilePath("..");
        if (currentPath == parentPath) break;
    }

    return SvnSatatusModel::VcsType::None;
}

void SvnLogItemsModel::refreshSvn(const QString &revision)
{
    QProcess proc;
    proc.setWorkingDirectory(m_Path);

    QStringList args = {"log", "--xml", "-v", "-r", revision};
    args.append(m_Url);

    CoreStartProcess(&proc, "svn.exe", args, true, true, 30000, true);

    QByteArray data = proc.readAllStandardOutput();
    QDomDocument doc;
    doc.setContent(data);

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChildElement("logentry").firstChildElement("paths").firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if (e.nodeName() == "path")
        {
            SvnLogInfoElement element;
            element.path = e.text().remove(m_Url).mid(1);
            element.kind = e.attribute("kind");
            element.action = e.attribute("action");

            if (!element.path.isEmpty())
                m_Elements.append(element);
        }
        n = n.nextSibling();
    }
}

void SvnLogItemsModel::refreshGit(const QString &revision)
{
    QProcess proc;
    proc.setWorkingDirectory(m_Path);

    // Для Git используем хеш коммита или относительную ссылку (HEAD~1 и т.д.)
    QString commitHash = revision;

    // Если передан числовой номер (как в SVN), конвертируем в Git хеш
    if (revision.toInt() > 0)
    {
        // Получаем хеш коммита по порядковому номеру
        QStringList args = {"log", "--reverse", "--pretty=format:%H", "--max-count", revision};
        QProcess hashProc;
        hashProc.setWorkingDirectory(m_Path);
        CoreStartProcess(&hashProc, "git.exe", args, true, true, 30000, true);

        QStringList hashes = QString::fromUtf8(hashProc.readAllStandardOutput()).split('\n', Qt::SkipEmptyParts);
        if (hashes.size() >= revision.toInt())
        {
            commitHash = hashes[revision.toInt() - 1];
        }
        else
        {
            commitHash = "HEAD"; // fallback
        }
    }

    // Получаем изменения для конкретного коммита
    QStringList args = {"show", "--name-status", "--pretty=format:", commitHash};
    CoreStartProcess(&proc, "git.exe", args, true, true, 30000, true);

    QByteArray data = proc.readAllStandardOutput();
    QString output = QString::fromUtf8(data);
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines)
    {
        if (line.trimmed().isEmpty()) continue;

        // Формат: "M\tfile.txt"
        QRegularExpression regex("^(\\w+)\\t(.+)$");
        QRegularExpressionMatch match = regex.match(line);

        if (match.hasMatch())
        {
            SvnLogInfoElement element;
            element.action = match.captured(1).toLower();
            element.path = match.captured(2);

            // Определяем тип (файл или директория)
            QFileInfo fi(QDir(m_Path).absoluteFilePath(element.path));
            element.kind = fi.isDir() ? "dir" : "file";

            m_Elements.append(element);
        }
    }
}
