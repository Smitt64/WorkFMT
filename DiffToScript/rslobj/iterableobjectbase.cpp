#include "iterableobjectbase.h"

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
