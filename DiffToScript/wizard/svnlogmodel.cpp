#include "svnlogmodel.h"
#include "fmtcore.h"
#include <QProcess>
#include <QDomDocument>
#include <QFileIconProvider>

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
    QAbstractTableModel(parent)
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

    QProcess proc;
    proc.setWorkingDirectory(m_Path);

    QStringList args = {"log", "--xml"};

    /*if (!m_Limit.isNull())
    {
        args.append("--limit");
        args.append(m_Limit.toString());
    }*/
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

    endResetModel();
}

// ----------------------------------------------------------------------

SvnLogItemsModel::SvnLogItemsModel(QObject *parent) :
    QAbstractTableModel(parent)
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

            if (!action.compare("A", Qt::CaseInsensitive))
                return "Added";
            else if (!action.compare("M", Qt::CaseInsensitive))
                return "Modified";
            else if (!action.compare("D", Qt::CaseInsensitive))
                return "Deleted";
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

void SvnLogItemsModel::setPath(const QString &path, const QString &url)
{
    m_Path = path;
    m_Url = url;
}

void SvnLogItemsModel::refresh(const QString &revision)
{
    beginResetModel();
    m_Elements.clear();

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

    endResetModel();
}
