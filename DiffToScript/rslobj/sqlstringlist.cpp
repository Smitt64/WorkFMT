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

void SqlStringList::clearGroup(const QString& name)
{
    m_Groups.remove(name);
}

void SqlStringList::beginGroup(const QString& name)
{
    m_CurrentGroup = name;

    if (!m_Groups.contains(m_CurrentGroup))
        m_Groups[m_CurrentGroup] = QStringList();
}

void SqlStringList::endGroup()
{
    m_CurrentGroup = QString();
}

void SqlStringList::applyGroup(const QString& name)
{
    if (!m_Groups.contains(name))
        return;

    QStringList lst = m_Groups[name];
    SqlStringList::append(lst);
}

void SqlStringList::append(const QString& str, int depth)
{
    QStringList *ptr = m_list;

    if (!m_CurrentGroup.isEmpty())
        ptr = &m_Groups[m_CurrentGroup];

    if (ptr)
    {
        if (depth <= 0)
            ptr->append(str);
        else
            ptr->append(Padding(depth) + str);

        emit sizeChanged(m_list->size());
        emit linesChanged();
    }
}

void SqlStringList::append(const QStringList& strings, int depth)
{
    QStringList *ptr = m_list;

    if (!m_CurrentGroup.isEmpty())
        ptr = &m_Groups[m_CurrentGroup];

    if (ptr)
    {
        if (depth <= 0)
            ptr->append(strings);
        else
        {
            auto AddPadding = [=](QString &str) -> void
            {
                str = Padding(depth) + str;
            };

            QStringList tmp = strings;
            std::for_each(tmp.begin(), tmp.end(), AddPadding);
            ptr->append(tmp);
        }

        emit sizeChanged(m_list->size());
        emit linesChanged();
    }
}

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
        for (const QString& str : std::as_const(*m_list))
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

int SqlStringList::groupCount() const
{
    return m_Groups.keys().size();
}

QStringList SqlStringList::groups() const
{
    return m_Groups.keys();
}
