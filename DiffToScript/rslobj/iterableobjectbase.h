#ifndef ITERABLEOBJECTBASE_H
#define ITERABLEOBJECTBASE_H

#include <QObject>

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

protected:
    virtual int GetSize() const;
    bool _next();
    bool _previous();

    int m_currentIndex = -1; // Текущая позиция итератора
};

template<template<typename...> class Container, class T>
class IterableObject : public IterableObjectBase, public Container<T>
{
public:
    IterableObject() :
        IterableObjectBase(),
        Container<T>() { };

    IterableObject(const IterableObject& other)
            : IterableObjectBase(other),
              Container<T>(other) { }

    IterableObject(IterableObject&& other) noexcept
            : IterableObjectBase(std::move(other)),
              Container<T>(std::move(other)) {}

    virtual ~IterableObject() = default;

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

    const T *doNext()
    {
        if (!_next())
            return nullptr;

        const T &rec = Container<T>::at(m_currentIndex);
        return &rec;
    }

    const T *doPrevious()
    {
        if (!_previous())
            return nullptr;

        const T &rec = Container<T>::at(m_currentIndex);
        return &rec;
    }

protected:
    virtual int GetSize() const Q_DECL_OVERRIDE
    {
        return Container<T>::size();
    }
};

#define INVOKABLE_NEXT(type) Q_INVOKABLE const type *next() { return doNext(); }
#define INVOKABLE_PREVIOUS(type) Q_INVOKABLE const type *previous() { return doPrevious(); }

template<typename T>
using VectorIterableObject = IterableObject<QVector, T>;

template<typename T>
using ListIterableObject = IterableObject<QList, T>;

#endif // ITERABLEOBJECTBASE_H
