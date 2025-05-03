#ifndef ITERABLEOBJECTBASE_H
#define ITERABLEOBJECTBASE_H

#include <QObject>
#include <QVariant>
#include <QDebug>

class IterableObjectBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ GetSize CONSTANT)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
public:
    IterableObjectBase(QObject *parent = nullptr);
    IterableObjectBase(IterableObjectBase&& other) noexcept;

    IterableObjectBase(const IterableObjectBase&) = delete;
    IterableObjectBase& operator=(const IterableObjectBase&) = delete;
    IterableObjectBase& operator=(IterableObjectBase&& other) noexcept;

    virtual ~IterableObjectBase();

    // Переместить итератор в начало
    Q_INVOKABLE void toFront();

    // Переместить итератор в конец
    Q_INVOKABLE void toBack();

    // Есть ли следующий элемент
    Q_INVOKABLE bool hasNext() const;

    // Есть ли предыдущий элемент
    Q_INVOKABLE bool hasPrevious() const;

    // Получить текущую позицию итератора
    Q_INVOKABLE int currentIndex() const;

    // Установить текущую позицию итератора
    Q_INVOKABLE void setCurrentIndex(const int &index);

    Q_INVOKABLE QVariant next();
    Q_INVOKABLE QVariant previous();

    Q_INVOKABLE QVariant record(int index) const;

protected:
    virtual int GetSize() const;
    virtual QVariant GetNext();
    virtual QVariant GetPrevious();
    virtual QVariant GetRecord(int index) const;

    bool _next();
    bool _previous();

    int m_currentIndex = -1; // Текущая позиция итератора
};

template<template<typename...> class Container, class T>
class IterableObject : public IterableObjectBase, public Container<T>
{
public:
    using Container<T>::Container;
    using iterator = typename Container<T>::iterator;
    using const_iterator = typename Container<T>::const_iterator;
    using reverse_iterator = typename Container<T>::reverse_iterator;
    using const_reverse_iterator = typename Container<T>::const_reverse_iterator;

    IterableObject() :
        IterableObjectBase(),
        Container<T>() { };

    IterableObject(const IterableObject& other)
            : IterableObjectBase(),
              Container<T>(other) { }

    IterableObject(IterableObject&& other) noexcept
            : IterableObjectBase(std::move(other)),
              Container<T>(std::move(other)) {}

    virtual ~IterableObject()
    {
        if constexpr (std::is_pointer_v<T>)
              qDeleteAll(*this);
    }

    IterableObject& operator=(const IterableObject& other)
    {
        if (this != &other)
            Container<T>::operator=(other);

        return *this;
    }

    IterableObject& operator=(IterableObject&& other) noexcept
    {
        if (this != &other)
        {
            IterableObjectBase::operator=(std::move(other));
            Container<T>::operator=(std::move(other));
        }

        return *this;
    }

    iterator begin() { return Container<T>::begin(); }
    const_iterator begin() const { return Container<T>::begin(); }
    const_iterator cbegin() const { return Container<T>::cbegin(); }

    iterator end() { return Container<T>::end(); }
    const_iterator end() const { return Container<T>::end(); }
    const_iterator cend() const { return Container<T>::cend(); }

    reverse_iterator rbegin() { return Container<T>::rbegin(); }
    const_reverse_iterator rbegin() const { return Container<T>::rbegin(); }
    const_reverse_iterator crbegin() const { return Container<T>::crbegin(); }

    reverse_iterator rend() { return Container<T>::rend(); }
    const_reverse_iterator rend() const { return Container<T>::rend(); }
    const_reverse_iterator crend() const { return Container<T>::crend(); }

protected:
    virtual int GetSize() const Q_DECL_OVERRIDE
    {
        return Container<T>::size();
    }

    virtual QVariant GetNext() Q_DECL_OVERRIDE
    {
        if (!_next())
            return QVariant();

        return GetRecord(m_currentIndex);
    }

    virtual QVariant GetPrevious() Q_DECL_OVERRIDE
    {
        if (!_previous())
            return QVariant();

        return GetRecord(m_currentIndex);
    }

    virtual QVariant GetRecord(int index) const Q_DECL_OVERRIDE
    {
        if (index < 0 || index >=  Container<T>::size())
            return QVariant();

        const T &rec = Container<T>::at(index);
        if constexpr (std::is_base_of_v<QObject, T> && !std::is_pointer_v<T>)
                return QVariant::fromValue<QObject*>(const_cast<T*>(&rec));

        return QVariant::fromValue<T>(rec);
    }
};

template<typename T>
using VectorIterableObject = IterableObject<QVector, T>;

template<typename T>
using ListIterableObject = IterableObject<QList, T>;

#endif // ITERABLEOBJECTBASE_H
