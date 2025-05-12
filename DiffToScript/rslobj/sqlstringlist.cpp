#include "sqlstringlist.h"
#include "sqlscriptmain.h"
#include <QDebug>

SqlStringList::SqlStringList(QObject *parent)
    : QObject(parent),
      m_list(new QStringList()),
      m_ownsList(true)
{

}

SqlStringList::SqlStringList(QStringList* list, QObject *parent)
    : QObject(parent),
      m_list(list),
      m_ownsList(false)
{
    if (!m_list)
    {
        qWarning() << "SqlStringList: Передан nullptr, создан новый QStringList";
        m_list = new QStringList();
        m_ownsList = true;
    }
}

SqlStringList::~SqlStringList()
{
    if (m_ownsList && m_list)
    {
        delete m_list;
    }
}

/*SqlStringList& SqlStringList::append(const QString& str)
{
    if (m_list)
    {
        m_list->append(str);
        emit sizeChanged(m_list->size());
        emit linesChanged();
    }

    return *this;
}

SqlStringList& SqlStringList::append(const QStringList& strings)
{
    if (m_list)
    {
        m_list->append(strings);
        emit sizeChanged(m_list->size());
        emit linesChanged();
    }

    return *this;
}*/

void SqlStringList::append(const QString& str, int depth)
{
    if (m_list)
    {
        if (depth <= 0)
            m_list->append(str);
        else
            m_list->append(Padding(depth) + str);

        emit sizeChanged(m_list->size());
        emit linesChanged();
    }
}

void SqlStringList::append(const QStringList& strings, int depth)
{
    if (m_list)
    {
        if (depth <= 0)
            m_list->append(strings);
        else
        {
            auto AddPadding = [=](QString &str) -> void
            {
                str = Padding(depth) + str;
            };

            QStringList tmp = strings;
            std::for_each(tmp.begin(), tmp.end(), AddPadding);
            m_list->append(tmp);
        }

        emit sizeChanged(m_list->size());
        emit linesChanged();
    }
}

/*SqlStringList& SqlStringList::operator<<(const QString& str)
{
    return append(str);
}

SqlStringList& SqlStringList::operator<<(const QStringList& strings)
{
    return append(strings);
}*/

QStringList SqlStringList::stringList() const
{
    return m_list ? *m_list : QStringList();
}

int SqlStringList::size() const
{
    return m_list ? m_list->size() : 0;
}

QStringList SqlStringList::lines() const
{
    QStringList result;

    if (m_list)
    {
        for (const QString& str : *m_list)
            result << str;
    }

    return result;
}

QString SqlStringList::sqlText() const
{
    return m_list->join('\n');
}

QStringList* SqlStringList::data() const
{
    return m_list;
}
