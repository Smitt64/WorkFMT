#include "fmtindecesmodelitem.h"

FmtIndecesModelItem::FmtIndecesModelItem(FmtIndecesModelItem *parent, QObject *parentObject)
    : QObject(parentObject)
{
    pParentItem = parent;
}

FmtIndecesModelItem::~FmtIndecesModelItem()
{
    qDeleteAll(childItems);
    childItems.clear();
}

FmtIndecesModelItem *FmtIndecesModelItem::child(int number)
{
    if (!childItems.isEmpty() && childItems.size() > number)
        return childItems[number];
    return NULL;
}

FmtIndecesModelItem *FmtIndecesModelItem::parent()
{
    return pParentItem;
}

int FmtIndecesModelItem::childCount() const
{
    return childItems.count();
}

int FmtIndecesModelItem::childNumber() const
{
    if (pParentItem)
        return pParentItem->childItems.indexOf(const_cast<FmtIndecesModelItem*>(this));

    return 0;
}

void FmtIndecesModelItem::appendChild(FmtIndecesModelItem *intem)
{
    intem->pParentItem = this;
    childItems.append(intem);
}

void FmtIndecesModelItem::insertChild(const int &after, FmtIndecesModelItem *intem)
{
    intem->pParentItem = this;

    if (after == -1)
        childItems.prepend(intem);
    else
        childItems.insert(after, intem);
}

void FmtIndecesModelItem::setFlag(int f)
{
    m_Flag = f;
}

FmtIndecesModelItem *FmtIndecesModelItem::take(int i)
{
    return childItems.takeAt(i);
}

void FmtIndecesModelItem::insertItem(int after)
{
    Q_UNUSED(after);
    //childItems.insert();
}

quint32 FmtIndecesModelItem::indexOfChild(FmtIndecesModelItem *item) const
{
    return childItems.indexOf(item);
}

/*void FmtIndecesModelItem::insertChild(const int &after, FmtIndecesModelItem *item)
{
    childItems.insert(after, item);
}*/
