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
    bool prepare(const QString &query) Q_DECL_OVERRIDE;

    bool fetch(int index) Q_DECL_OVERRIDE;
    bool fetchFirst() Q_DECL_OVERRIDE;
    bool fetchLast() Q_DECL_OVERRIDE;
    bool fetchNext() Q_DECL_OVERRIDE;
    bool fetchPrevious() Q_DECL_OVERRIDE;

    QSqlRecord record() const Q_DECL_OVERRIDE;

    void setQuery(const QString &query) Q_DECL_OVERRIDE;

    /*static QDate rsDateToQDate(const bdate &_rsDate);
    static QTime rsTimeToQTime(const btime &_rsTime);
    static QVariant rsDateToVariantQDate(CRsdField &fld);
    static QVariant rsTimeToVariantQTime(CRsdField &fld);
    static QVariant rsTimeStampToVariantQDateTime(CRsdField &fld);*/
    static QVariant rsBinaryToVariantQByteArray(CRsdField &fld);
    static QVariant rsCharToVariantQChar(CRsdField &fld);
    static QVariant rsNumericToVariant(CRsdField &fld);

protected:
    void onBeforeCloseConnection();

private:
    bool setCmdText(const QString &sql);

    bool checkWord(const QString &sql, const QString &word) const;
    bool checkInsert(const QString &sql) const;
    bool checkUpdate(const QString &sql) const;
    bool checkDelete(const QString &sql) const;
    bool checkMerge(const QString &sql) const;
    bool checkCOMMENT(const QString &sql) const;
    bool checkAlter(const QString &sql) const;
    bool checkSelect(const QString &sql) const;
    bool checkCall(const QString &sql) const;
    bool checkCreate(const QString &sql) const;
    bool checkDrop(const QString &sql) const;
    bool makeRecordSetFromCmd(QScopedPointer<RsdCommandEx> &cmd);

    QVariant GetValueFromField(const CRsdField &cfld, const int &loblen);
    QSqlField MakeField(const CRsdField &fld, const bool &isnull, const int &loblen);
    void MakeRecord();

    RsdDriver *m_Driver;
    QScopedPointer<RsdCommandEx> m_Cmd;
    QScopedPointer<CRsdRecordset> m_RecSet;
    QScopedPointer<QSqlRecord> m_Record;
    QString m_QueryString;
};

Q_DECLARE_METATYPE(long double);

#endif // RSDSQLRESULT_H
