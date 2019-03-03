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
    AbstractCreator(const QString &pAlias) { m_alias = pAlias; }
    virtual ~AbstractCreator() { }
    virtual Base *create() = 0;
    const QString alias() const { return m_alias; }

private:
    QString m_alias;
};

template <class C, class Base>
class Creator : public AbstractCreator<Base>
{
public:
    Creator(const QString &alias) :
        AbstractCreator<Base>(alias)
    { }
    virtual ~Creator() { }
    Base *create() { return static_cast<Base*>(new C()); }
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
    void add(const IdType &id, const QString &alias)
    {
        registerClass(id, new Creator<C, Base>(alias));
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

    QStringList getAliases() const
    {
        QStringList list;
        QMapIterator<typename FmtLibFactoryMap::key_type, typename FmtLibFactoryMap::mapped_type> iter(_FmtLibFactory);
        while (iter.hasNext())
        {
            iter.next();
            const typename FmtLibFactoryMap::mapped_type &value = iter.value();
            list.append(value->alias());
        }
        return list;
    }

    qint32 count() const
    {
        return _FmtLibFactory.count();
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
