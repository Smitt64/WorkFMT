#include "iterableobjectbase.h"
#include <QSet>

Q_GLOBAL_STATIC(QSet<Qt::HANDLE>, deletedObjects)

bool CachedObjectDeleter::CheckObjectInCache(Qt::HANDLE obj)
{
    return deletedObjects->contains(obj);
}

void CachedObjectDeleter::InsertObjectToCache(Qt::HANDLE obj)
{
    deletedObjects->insert(obj);
}

IterableObjectBase::IterableObjectBase(QObject *parent) :
    QObject(parent)
{

}

IterableObjectBase::IterableObjectBase(IterableObjectBase&& other) noexcept
    : QObject(other.parent()),
      m_currentIndex(other.m_currentIndex)
{
    // QObject не поддерживает перемещение, поэтому создаем новый объект
    other.m_currentIndex = -1;
}

IterableObjectBase::~IterableObjectBase()
{

}

IterableObjectBase& IterableObjectBase::operator=(IterableObjectBase&& other) noexcept
{
    if (this != &other)
    {
        // QObject не поддерживает перемещение, копируем только данные
        m_currentIndex = other.m_currentIndex;
        other.m_currentIndex = -1;
    }

    return *this;
}

bool IterableObjectBase::_next()
{
    if (!hasNext())
        return false;

    ++m_currentIndex;
    return true;
}

bool IterableObjectBase::_previous()
{
    if (!hasPrevious())
        return false;

    --m_currentIndex;
    return true;
}

int IterableObjectBase::GetSize() const
{
    return 0;
}

QVariant IterableObjectBase::GetRecord(int index) const
{
    return QVariant();
}

QVariant IterableObjectBase::GetNext()
{
    _next();
    return QVariant();
}

QVariant IterableObjectBase::GetPrevious()
{
    _previous();
    return QVariant();
}

void IterableObjectBase::toFront()
{
    m_currentIndex = -1;
}

void IterableObjectBase::toBack()
{
    m_currentIndex = GetSize();
}

bool IterableObjectBase::hasNext() const
{
    return m_currentIndex + 1 < GetSize();
}

bool IterableObjectBase::hasPrevious() const
{
    return m_currentIndex > 0;
}

int IterableObjectBase::currentIndex() const
{
    return m_currentIndex;
}

void IterableObjectBase::setCurrentIndex(const int &index)
{
    if (index >= -1 && index < GetSize() && index != m_currentIndex)
        m_currentIndex = index;
}

QVariant IterableObjectBase::next()
{
    return GetNext();
}

QVariant IterableObjectBase::previous()
{
    return GetPrevious();
}

QVariant IterableObjectBase::record(int index) const
{
    return GetRecord(index);
}
