#include "contenttreeitem.h"
#include "hotfixcontentmodel.h"
#include <QDebug>
#include <QFileIconProvider>
#include <QGlobalStatic>

Q_GLOBAL_STATIC(QFileIconProvider, pIconProvider)

ContentTreeItem::ContentTreeItem(ContentTreeItem *parentItem) :
    QObject(),
    m_fSkipSetDataChild(false),
    m_parentItem(parentItem),
    m_Check(Qt::Unchecked),
    m_Chackable(false),
    m_Tristate(false),
    m_fEnabled(true),
    m_fShowRowNumber(false),
    m_Order(0)
{

}

ContentTreeItem::~ContentTreeItem()
{

}

ContentTreeItem *ContentTreeItem::findItemByData(const QVariant& value, int column, int role)
{
    if (data(column, role) == value)
        return this;

    for (const auto& child : m_childItems)
    {
        ContentTreeItem* foundItem = child->findItemByData(value, column, role);

        if (foundItem)
            return foundItem;
    }

    return nullptr;
}

ContentTreeItem *ContentTreeItem::appendChild(std::unique_ptr<ContentTreeItem> &&child)
{
    child->m_parentItem = this;
    m_childItems.push_back(std::move(child));

    ContentTreeItem *item = m_childItems.back().get();
    item->setModel(model());

    connect(item, &ContentTreeItem::itemChanged, model(), &HotfixContentModel::contentItemChanged);
    return item;
}

ContentTreeItem *ContentTreeItem::child(int row)
{
    return row >= 0 && row < childCount() ? m_childItems.at(row).get() : nullptr;
}

ContentTreeItem *ContentTreeItem::child(int row) const
{
    return row >= 0 && row < childCount() ? m_childItems.at(row).get() : nullptr;
}

int ContentTreeItem::childCount() const
{
    return int(m_childItems.size());
}

int ContentTreeItem::totalChildCount() const
{
    int count = 0;

    for (const auto& child : m_childItems)
    {
        count += 1;
        count += child->totalChildCount();
    }

    return count;
}

QVariant ContentTreeItem::data(const int &column, const int &role) const
{
    if (column == 0 && role == Qt::CheckStateRole && m_Chackable)
        return m_Check;

    return QVariant();
}

bool ContentTreeItem::setData(const QVariant &value, const int &column, int role)
{
    if (column == 0 && role == Qt::CheckStateRole && m_Chackable)
    {
        m_Check = value.value<Qt::CheckState>();

        if (m_fSkipSetDataChild)
            return false;

        for (int i = 0; i < m_childItems.size(); i++)
        {
            if (!m_childItems[i]->isEnable())
                continue;

            m_childItems[i]->setData(value, column, role);
            emit m_childItems[i]->itemChanged(column, {role});
        }

        Qt::CheckState firstcheck;
        ContentTreeItem *prnt = parentItem();

        for (int i = 0; i < prnt->childCount(); i++)
        {
            if (!i)
                firstcheck = prnt->m_childItems[i]->m_Check;
            else
            {
                if (!prnt->m_childItems[i]->isEnable())
                    continue;

                if (prnt->m_childItems[i]->m_Check != firstcheck)
                {
                    if (prnt->m_Tristate)
                        firstcheck = Qt::PartiallyChecked;
                    else
                        firstcheck = Qt::Unchecked;
                }
            }
        }

        prnt->m_Check = firstcheck;
        emit prnt->itemChanged(column, {role});

        return true;
    }

    return false;
}

ContentTreeItem *ContentTreeItem::parentItem()
{
    return m_parentItem;
}

ContentTreeItem *ContentTreeItem::parentItem() const
{
    return m_parentItem;
}

int ContentTreeItem::row() const
{
    if (m_parentItem == nullptr)
        return 0;
    const auto it = std::find_if(m_parentItem->m_childItems.cbegin(), m_parentItem->m_childItems.cend(),
                                 [this](const std::unique_ptr<ContentTreeItem> &treeItem) {
                                     return treeItem.get() == this;
                                 });

    if (it != m_parentItem->m_childItems.cend())
        return std::distance(m_parentItem->m_childItems.cbegin(), it);
    //Q_ASSERT(false); // should not happen
    return -1;
}

QFileIconProvider *ContentTreeItem::iconProvider()
{
    return pIconProvider;
}

QFileIconProvider *ContentTreeItem::iconProvider() const
{
    return pIconProvider;
}

void ContentTreeItem::setCheckable(const bool &checkable)
{
    m_Chackable = checkable;
}

void ContentTreeItem::setTristate(const bool &state)
{
    m_Tristate = state;
}

void ContentTreeItem::setEnable(const bool &state)
{
    m_fEnabled = state;
}

void ContentTreeItem::setShowRowNumber(const bool &state)
{
    m_fShowRowNumber = state;
}

Qt::CheckState ContentTreeItem::checkState()
{
    return m_Check;
}

const bool &ContentTreeItem::checkable() const
{
    return m_Chackable;
}

const bool &ContentTreeItem::tristate() const
{
    return m_Tristate;
}

void ContentTreeItem::setModel(HotfixContentModel *model)
{
    m_pModel = model;
}

HotfixContentModel *ContentTreeItem::model()
{
    return m_pModel;
}

const bool &ContentTreeItem::isEnable() const
{
    return m_fEnabled;
}

const bool &ContentTreeItem::isShowRowNumber() const
{
    return m_fShowRowNumber;
}

void ContentTreeItem::setOrder(const qint16 &v)
{
    m_Order = v;
}

const qint16 &ContentTreeItem::order() const
{
    return m_Order;
}

MakeResult ContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (action == ActionMake)
    {
        msg = "Действие не реализовано";
        return ResultWarning;
    }

    return ResultSuccess;
}
