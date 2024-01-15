#include "recentconnectionlist.h"
#include "fmtcore.h"
#include <QFile>
#include <QDataStream>

QDataStream &operator <<(QDataStream &out, const tagRecentListOld &rhs)
{
    out << rhs.user << rhs.pass << rhs.dsn;
    return out;
}

QDataStream &operator >>(QDataStream &in, tagRecentListOld &rhs)
{
    in >> rhs.user >> rhs.pass >> rhs.dsn;
    return in;
}

QDataStream &operator <<(QDataStream &out, const tagRecentList &rhs)
{
    const tagRecentListOld &ref = static_cast<const tagRecentListOld&>(rhs);
    out << ref;
    out << rhs.lastConnect;
    out << rhs.connectionType;
    return out;
}

QDataStream &operator >>(QDataStream &in, tagRecentList &rhs)
{
    tagRecentListOld &ref = static_cast<tagRecentListOld&>(rhs);
    in >> ref;
    in >> rhs.lastConnect;
    in >> rhs.connectionType;
    return in;
}

QDataStream &operator <<(QDataStream &out, const tagRecentList2 &rhs)
{
    const tagRecentList &ref = static_cast<const tagRecentList&>(rhs);
    out << ref;
    out << rhs.Options;
    return out;
}

QDataStream &operator >>(QDataStream &in, tagRecentList2 &rhs)
{
    tagRecentList &ref = static_cast<tagRecentList&>(rhs);
    in >> ref;
    in >> rhs.Options;
    return in;
}

// ==============================================================================
const char newFormatMagic [] = "FMTRL1";
const char newFormatMagic2[] = "FMTRL2";

RecentConnectionList::RecentConnectionList(QObject *parent)
    : QAbstractTableModel{parent}
{
    sConfigOraFilePath = ConfigOraFilePath();

    if (qMetaTypeId<RecentList>() == QMetaType::UnknownType)
        qRegisterMetaType<RecentList>("RecentList");

    if (qMetaTypeId<RecentListOld>() == QMetaType::UnknownType)
        qRegisterMetaType<RecentListOld>("RecentListOld");

    m_Oracle = QIcon(":/dblogo/Oracle.png");
    m_PostgreSQL = QIcon(":/dblogo/PostgreSQL.png");
}

RecentConnectionList::~RecentConnectionList()
{

}

bool RecentConnectionList::save()
{
    bool hr = true;

    QFile file(sConfigOraFilePath);
    hr = file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    if (hr)
    {
        file.write(newFormatMagic2, sizeof(newFormatMagic2));
        QDataStream stream(&file);

        qint16 size = static_cast<qint16>(m_List.size());
        stream << size;

        for (const RecentList2 &listitem : qAsConst(m_List))
            stream << listitem;

        file.close();
    }
    return hr;
}

bool RecentConnectionList::load()
{
    bool hr = true;

    QFile file(sConfigOraFilePath);
    hr = file.open(QIODevice::ReadOnly);

    if (hr)
    {
        QDataStream stream(&file);

        beginResetModel();
        char magic[7] = { 0 };
        file.read(magic, sizeof(magic));

        if (!memcmp(magic, newFormatMagic, sizeof(newFormatMagic)))
        {
            qint16 size = 0;
            stream >> size;

            for (int i = 0; i < size; i++)
            {
                RecentList rec;
                RecentList2 rec2;

                stream >> rec;
                rec2.user = rec.user;
                rec2.pass = rec.pass;
                rec2.dsn = rec.dsn;
                rec2.lastConnect = rec.lastConnect;
                rec2.connectionType = rec.connectionType;
                m_List.append(rec2);
            }
        }
        else if (!memcmp(magic, newFormatMagic2, sizeof(newFormatMagic2)))
        {
            qint16 size = 0;
            stream >> size;

            for (int i = 0; i < size; i++)
            {
                RecentList2 rec;
                stream >> rec;
                m_List.append(rec);
            }
        }
        else
        {
            QStringList last;
            QList<RecentListOld> recent;

            file.seek(0);
            stream >> last;
            stream >> recent;

            for (const auto &item : qAsConst(recent))
            {
                RecentList2 rec;
                rec.dsn = item.dsn;
                rec.user = item.user;
                rec.pass = item.pass;

                m_List.append(rec);
            }
        }
        endResetModel();
        file.close();
    }

    return hr;
}

RecentConnectionList::RecentListType::const_iterator RecentConnectionList::find(const QString &user, const QString &dsn) const
{
    RecentListType::const_iterator iter = std::find_if(m_List.begin(), m_List.end(), [=](const auto &item)
    {
        if (!user.compare(item.user, Qt::CaseInsensitive) && !dsn.compare(item.dsn, Qt::CaseInsensitive))
            return true;

        return false;
    });

    return iter;
}

RecentConnectionList::RecentListType::iterator RecentConnectionList::find(const QString &user, const QString &dsn)
{
    RecentListType::iterator iter = std::find_if(m_List.begin(), m_List.end(), [=](const auto &item)
    {
        if (!user.compare(item.user, Qt::CaseInsensitive) && !dsn.compare(item.dsn, Qt::CaseInsensitive))
            return true;

        return false;
    });

    return iter;
}

bool RecentConnectionList::contains(const QString &user, const QString &dsn) const
{
    RecentListType::const_iterator iter = find(user, dsn);

    return iter != m_List.end();
}

void RecentConnectionList::append(const QString &user, const QString &pass, const QString &dsn, const quint16 &type, const OptionsMap &Options)
{
    beginResetModel();
    RecentListType::iterator iter = find(user, dsn);

    if (iter == m_List.end())
    {
        RecentList2 rec;
        rec.dsn = dsn;
        rec.user = user;
        rec.pass = pass;
        rec.connectionType = type;
        rec.lastConnect = QDateTime::currentDateTime();
        rec.Options = Options;

        m_List.append(rec);
    }
    else
        (*iter).lastConnect = QDateTime::currentDateTime();

    sortPrivate();
    endResetModel();
}

void RecentConnectionList::sortlist()
{
    beginResetModel();
    sortPrivate();
    endResetModel();
}

void RecentConnectionList::sortPrivate()
{
    std::sort(m_List.begin(), m_List.end(), [=](const RecentList &item1, const RecentList &item2)
    {
        return item1.lastConnect > item2.lastConnect;
    });
}

QString RecentConnectionList::connectionTypeName(const quint16 &type)
{
    QMetaEnum metaenum;
    const QMetaObject &meta = RecentConnectionList::staticMetaObject;

    int enumeratorCount = meta.enumeratorCount();
    for (int i = 0; i < enumeratorCount; i++)
    {
        const char *enumName = meta.enumerator(i).enumName();
        if (!strcmp(enumName, "ConnectionType"))
        {
            metaenum = meta.enumerator(i);
            break;
        }
    }

    QString value;
    if (metaenum.isValid())
        value = metaenum.valueToKey(type);

    return value;
}

void RecentConnectionList::setConnectionType(const QString &user, const QString &dsn, const quint16 &type)
{
    RecentListType::iterator iter = find(user, dsn);

    if (iter != m_List.end())
        (*iter).connectionType = type;
}

int RecentConnectionList::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

int RecentConnectionList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_List.size();
}

QVariant RecentConnectionList::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        const RecentList &item = m_List[index.row()];

        if (index.column() == 0)
            return item.user;

        if (index.column() == 1)
            return item.dsn;
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == 0)
        {
            const RecentList &item = m_List[index.row()];

            if (item.connectionType == Oracle)
                return m_Oracle;
            else if (item.connectionType == PostgreSQL)
                return m_PostgreSQL;
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        const RecentList &item = m_List[index.row()];
        return connectionToolTip(item);
    }
    return QVariant();
}

QVariant RecentConnectionList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == Qt::Vertical)
        return QAbstractTableModel::headerData(section, orientation, role);

    if (role == Qt::DisplayRole)
    {
        if (section == 0)
            return tr("Пользователь");
        else
            return tr("База данных");
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool RecentConnectionList::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = row; i < row + count; i++)
        m_List.removeAt(i);
    endRemoveRows();
    return true;
}

RecentList2 RecentConnectionList::record(const int &item) const
{
    Q_ASSERT_X(item >= 0 && item < m_List.size(), "RecentConnectionList::record", "Invalid item index");
    return m_List[item];
}

RecentList2 RecentConnectionList::record(const QModelIndex &item) const
{
    return record(item.row());
}

QString RecentConnectionList::connectionName(const RecentList &rec)
{
    return QString("%1@%2").arg(rec.user, rec.dsn);
}

QString RecentConnectionList::connectionToolTip(const RecentList &rec)
{
    QString connType = connectionTypeName(rec.connectionType);
    QString lastConnect = rec.lastConnect.toString(Qt::SystemLocaleDate);

    return QString(tr("<b>Пользователь: </b>%1<br/><b>База данных: </b>%2<br/><b>Подключение: </b>%3<br/><b>Последний раз: </b>%4"))
            .arg(rec.user, rec.dsn, connType, lastConnect);
}

bool RecentConnectionList::isEmpty() const
{
    return m_List.isEmpty();
}
