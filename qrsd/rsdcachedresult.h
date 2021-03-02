#ifndef RSDCACHEDRESULT_H
#define RSDCACHEDRESULT_H

#include <QtSql/private/qsqlcachedresult_p.h>

class RsdDriver;
class RsdCachedResultPrivate;
class RsdCachedResult : public QSqlCachedResult
{
    Q_DECLARE_PRIVATE(RsdCachedResult)
public:
    RsdCachedResult(const RsdDriver *db);

    QVariant handle() const Q_DECL_OVERRIDE;
    bool reset(const QString &query);
};

#endif // RSDCACHEDRESULT_H
