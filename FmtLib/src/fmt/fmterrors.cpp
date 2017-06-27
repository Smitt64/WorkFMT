#include "fmterrors.h"
#include <QApplication>
#include <QStyle>

FmtErrors::FmtErrors(QObject *parent)
    : QAbstractItemModel(parent)
{

}

int FmtErrors::columnCount(const QModelIndex &parent) const
{
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
    return createIndex(row, column);
}

QModelIndex FmtErrors::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int FmtErrors::rowCount(const QModelIndex &parent) const
{
    return m_errors.size();
}

Qt::ItemFlags FmtErrors::flags(const QModelIndex &index) const
{
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

    /*if (orientation == Qt::Vertical)
    {
        if (section >= pTable->m_pFields.size())
        {
            if (role == Qt::DisplayRole)
                return QString("✳");
            if (role == Qt::FontRole)
                return QFont("Arial", 10);
        }
    }*/

    return QAbstractItemModel::headerData(section, orientation, role);
}

bool FmtErrors::hasErrors()
{
    auto iter = std::find_if(m_errors.begin(), m_errors.end(), [](const FmtErrorStruct &err)
    {
        return err.type == FmtErrors::fmtet_Error;
    });

    return iter != m_errors.end();
    //return !m_errors.isEmpty();
}

bool FmtErrors::hasWarnings()
{
    auto iter = std::find_if(m_errors.begin(), m_errors.end(), [](const FmtErrorStruct &err)
    {
        return err.type == FmtErrors::fmtet_Warning;
    });

    return iter != m_errors.end();
}

int FmtErrors::errorsCount()
{
    return std::count_if(m_errors.begin(), m_errors.end(), [](const FmtErrorStruct &err)
    {
        return err.type == FmtErrors::fmtet_Error;
    });
}

int FmtErrors::warningsCount()
{
    return std::count_if(m_errors.begin(), m_errors.end(), [](const FmtErrorStruct &err)
    {
        return err.type == FmtErrors::fmtet_Warning;
    });
}

int FmtErrors::infoCount()
{
    return std::count_if(m_errors.begin(), m_errors.end(), [](const FmtErrorStruct &err)
    {
        return err.type == FmtErrors::fmtet_Info;
    });
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

    if (dateTime.isNull())
        err.time = QDateTime::currentDateTime();
    else
        err.time = dateTime;

    beginInsertRows(QModelIndex(), m_errors.size(), m_errors.size());
    m_errors.append(err);
    endInsertRows();
    QApplication::processEvents();
}

void FmtErrors::appendMessage(const QString &text, const QDateTime &dateTime)
{
    FmtErrorStruct err;
    err.text = text;
    err.type = FmtErrors::fmtet_Info;

    if (dateTime.isNull())
        err.time = QDateTime::currentDateTime();
    else
        err.time = dateTime;

    beginInsertRows(QModelIndex(), m_errors.size(), m_errors.size());
    m_errors.append(err);
    endInsertRows();
    QApplication::processEvents();
}

bool FmtErrors::next()
{
    if (m_Iterator == m_errors.end())
    {
        return first();
    }
    ++ m_Iterator;
    return true;
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
    endResetModel();
}
