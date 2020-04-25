#include "fmterrors.h"
#include <QApplication>
#include <QStyle>

FmtErrors::FmtErrors(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_errorsCount = 0;
    m_warningsCount = 0;
    m_infoCount = 0;
    m_Iterator = QList<FmtErrorStruct>::iterator();
}

FmtErrors::~FmtErrors()
{

}

int FmtErrors::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant FmtErrors::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 1)
        {
            QString text = m_errors.at(index.row()).text;
            text = text.remove(QRegExp("<[^>]*>")).simplified();
            value = text;
        }
        else if (index.column() == 0)
            value = m_errors.at(index.row()).time.toString("hh:mm:ss.zzz dd.MM.yyyy");
    }
    if (role == Qt::DecorationRole)
    {
        if (index.column() == 0)
        {
            qint16 type = m_errors.at(index.row()).type;
            switch(type)
            {
            case fmtet_Error:
                value = QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
                break;
            case fmtet_Warning:
                value = QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
                break;
            case fmtet_Info:
                value = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
                break;
            }
        }
    }
    return value;
}

bool FmtErrors::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex FmtErrors::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex FmtErrors::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int FmtErrors::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_errors.size();
}

Qt::ItemFlags FmtErrors::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant FmtErrors::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QStringList headers = QStringList()
            << tr("Событие") << tr("Текст");
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return headers.at(section);
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

bool FmtErrors::hasErrors()
{
    bool result = false;
    if (m_errorsCount)
        result = true;
    return result;
}

bool FmtErrors::hasWarnings()
{
    bool result = false;
    if (m_warningsCount)
        result = true;
    return result;
}

int FmtErrors::errorsCount()
{
    return m_errorsCount;
}

int FmtErrors::warningsCount()
{
    return m_warningsCount;
}

int FmtErrors::infoCount()
{
    return m_infoCount;
}

bool FmtErrors::isEmpty()
{
    return m_errors.isEmpty();
}

void FmtErrors::appendError(const QString &text, const qint16 &type, const QDateTime &dateTime)
{
    FmtErrorStruct err;
    err.text = text;
    err.type = type;

    beginInsertRows(QModelIndex(), m_errors.size(), m_errors.size());
    if (dateTime.isNull())
        err.time = QDateTime::currentDateTime();
    else
        err.time = dateTime;

    m_errors.append(err);
    endInsertRows();
    emit newError(text);

    if (type == fmtet_Error)
    {
        m_errorsCount ++;
        emit errorsCountChanged(m_errorsCount);
    }
    else if (type == fmtet_Warning)
    {
        m_warningsCount ++;
        emit warningsCountChanged(m_warningsCount);
    }
    else
    {
        m_infoCount ++;
        emit infoCountChanged(m_infoCount);
    }
}

void FmtErrors::appendMessage(const QString &text, const QDateTime &dateTime)
{
    FmtErrorStruct err;
    err.text = text;
    err.type = FmtErrors::fmtet_Info;
    m_infoCount ++;

    if (dateTime.isNull())
        err.time = QDateTime::currentDateTime();
    else
        err.time = dateTime;

    beginInsertRows(QModelIndex(), m_errors.size(), m_errors.size());
    m_errors.append(err);
    endInsertRows();
    emit newMessage(text);
    emit infoCountChanged(m_infoCount);
}

bool FmtErrors::next()
{
    if (m_Iterator == QList<FmtErrorStruct>::iterator())
    {
        return first();
    }
    ++ m_Iterator;
    return m_Iterator != m_errors.end();
}

bool FmtErrors::first()
{
    m_Iterator = m_errors.begin();
    return m_Iterator != m_errors.end();
}

qint16 FmtErrors::type(const qint32 &row) const
{
    if (row == -1)
        return (*m_Iterator).type;

    return m_errors.at(row).type;
}

QString FmtErrors::text() const
{
    return (*m_Iterator).text;
}

QString FmtErrors::textAt(const int &i) const
{
    if (i < 0 || i >= m_errors.size())
        return QString();
    return m_errors.at(i).text;
}

void FmtErrors::clear()
{
    beginResetModel();
    m_errors.clear();
    m_errorsCount = 0;
    m_warningsCount = 0;
    m_infoCount = 0;
    endResetModel();
}
