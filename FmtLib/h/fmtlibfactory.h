#ifndef FMTLIBFmtLibFactory_H
#define FMTLIBFmtLibFactory_H

#include <QString>
#include <QHash>
#include <QMap>

#include <QDebug>

template <class Base>
class AbstractCreator
{
public:
    AbstractCreator() { }
    virtual ~AbstractCreator() { }
    virtual Base *create() = 0;
};

template <class C, class Base>
class Creator : public AbstractCreator<Base>
{
public:
    Creator() { }
    virtual ~Creator() { }
    Base *create() { return (Base*)new C(); }

};

template <class Base, class IdType>
class FmtLibFactory
{
protected:
    typedef AbstractCreator<Base> AbstractFmtLibFactory;
    typedef QMap<IdType, AbstractFmtLibFactory*> FmtLibFactoryMap;

public:
    FmtLibFactory() { }
    virtual ~FmtLibFactory() { }

    template <class C>
    void add(const IdType &id)
    {
        registerClass(id, new Creator<C, Base>());
    }

    Base *create(const IdType &id) const
    {
        typename FmtLibFactoryMap::const_iterator it = _FmtLibFactory.find(id);

        if (it != _FmtLibFactory.end())
            return it.value()->create();
    }

    Base *getInstance(const IdType &id) const
    {
        typename FmtLibFactoryMap::const_iterator it = _FmtLibFactory.find(id);
        if (it != _FmtLibFactory.end())
        {
            return it.value()->create();
        }
    }

    QList<IdType> getIds() const
    {
        return _FmtLibFactory.keys();
    }
protected:
    void registerClass(const IdType &id, AbstractFmtLibFactory *p)
    {
        typename FmtLibFactoryMap::iterator it = _FmtLibFactory.find(id);
        if (it == _FmtLibFactory.end())
            _FmtLibFactory[id] = p;
        else
            delete p;
    }

private:
    FmtLibFactory(const FmtLibFactory&);
    FmtLibFactory &operator = (const FmtLibFactory&);
    FmtLibFactoryMap _FmtLibFactory;
};

#endif // FMTLIBFmtLibFactory_H
