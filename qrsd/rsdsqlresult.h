#ifndef RSDSQLRESULT_H
#define RSDSQLRESULT_H

#include <QSqlResult>
#include <QScopedPointer>
#include <QSqlRecord>
#include <RsdC.h>
#include "rsdcommandex.h"
#include <BaseErrorSetter.hpp>

class RsdDriver;
class RsdSqlResult Q_DECL_FINAL: public QSqlResult, public BaseErrorSetter<RsdSqlResult>
{
    friend class RsdDriver;
    friend class BaseErrorSetter<RsdSqlResult>;
public:
    RsdSqlResult(const QSqlDriver *db);
    virtual ~RsdSqlResult();

    QVariant data(int index) Q_DECL_OVERRIDE;
    bool isNull(int index) Q_DECL_OVERRIDE;
    int	numRowsAffected() Q_DECL_OVERRIDE;
    bool reset(const QString &query) Q_DECL_OVERRIDE;
    int	size() Q_DECL_OVERRIDE;

    /*void bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType paramType) Q_DECL_OVERRIDE;
    void bindValue(int index, const QVariant &val, QSql::ParamType paramType) Q_DECL_OVERRIDE;*/

    bool exec() Q_DECL_OVERRIDE;

    bool fetch(int index) Q_DECL_OVERRIDE;
    bool fetchFirst() Q_DECL_OVERRIDE;
    bool fetchLast() Q_DECL_OVERRIDE;
    bool fetchNext() Q_DECL_OVERRIDE;
    bool fetchPrevious() Q_DECL_OVERRIDE;

    QSqlRecord record() const Q_DECL_OVERRIDE;

private:
    QVariant GetValueFromField(const CRsdField &cfld);
    QSqlField MakeField(const CRsdField &fld);
    RsdDriver *m_Driver;
    QScopedPointer<RsdCommandEx> m_Cmd;
    QScopedPointer<CRsdRecordset> m_RecSet;
};

#endif // RSDSQLRESULT_H
