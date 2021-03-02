#include "rsdcachedresult.h"
#include "rsddriver.h"

class RsdCachedResultPrivate: public QSqlCachedResultPrivate
{
public:
    Q_DECLARE_PUBLIC(RsdCachedResult)
    RsdCachedResultPrivate(RsdCachedResult *q, const RsdDriver *drv) :
        QSqlCachedResultPrivate(q, drv)
    {

    }

    ~RsdCachedResultPrivate()
    {

    }
};

RsdCachedResult::RsdCachedResult(const RsdDriver *db) :
    QSqlCachedResult(*new RsdCachedResultPrivate(this, db))
{

}

QVariant RsdCachedResult::handle() const
{
    Q_D(const RsdCachedResult);
    return QVariant::fromValue(d->sql);
}

bool RsdCachedResult::reset(const QString &query)
{
    if (!prepare(query))
        return false;

    return exec();
}
