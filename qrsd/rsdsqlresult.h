#ifndef RSDSQLRESULT_H
#define RSDSQLRESULT_H

#include <fdecimal.h>
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

    bool exec() Q_DECL_OVERRIDE;
    bool prepare(const QString &query) Q_DECL_OVERRIDE;

    bool fetch(int index) Q_DECL_OVERRIDE;
    bool fetchFirst() Q_DECL_OVERRIDE;
    bool fetchLast() Q_DECL_OVERRIDE;
    bool fetchNext() Q_DECL_OVERRIDE;
    bool fetchPrevious() Q_DECL_OVERRIDE;

    QSqlRecord record() const Q_DECL_OVERRIDE;

    void setQuery(const QString &query) Q_DECL_OVERRIDE;

    static QVariant rsBinaryToVariantQByteArray(Qt::HANDLE fld);
    static QVariant rsCharToVariantQChar(Qt::HANDLE fld);

protected:
    void onBeforeCloseConnection();

private:
    // /*@ DYNAMIC_SAMPLING(e 3)*/
    QStringList GetSqlHints(const QString &sql);
    QVariant HintValue(const QString &hintparam);
    bool HasHint(const QStringList &hints, const QString &hint, QStringList *params = nullptr);

    bool setCmdText(const QString &sql);
    bool makeRecordSetFromCmd(QScopedPointer<RsdCommandEx> &cmd);

    QVariant GetValueFromField(Qt::HANDLE fld, const int &loblen);
    QSqlField MakeField(Qt::HANDLE fld, const bool &isnull, const int &loblen);
    void MakeRecord();

    RsdDriver *m_Driver;
    Qt::HANDLE m_RecSet;
    QScopedPointer<RsdCommandEx> m_Cmd;
    QScopedPointer<QSqlRecord> m_Record;
    QString m_QueryString;
};

Q_DECLARE_METATYPE(long double);

#endif // RSDSQLRESULT_H
