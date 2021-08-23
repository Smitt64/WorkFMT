#ifndef RSDCOMMANDEX_H
#define RSDCOMMANDEX_H

#include <RsdC.h>
#include <QtSql>
#include <QVector>
#include "rsddriver.h"

class RsdCommandEx : public CRsdCommand
{
public:
    class BindParam Q_DECL_FINAL
    {
        friend class RsdCommandEx;

    public:
        BindParam(const QVariant &value, QSql::ParamType paramType, const QString &pname, RsdCommandEx *parent);
        ~BindParam();

        int typeSize(const QVariant::Type &type);

        void *value;
        RSDLONG valueSize;
        RSDValType_t valType;
        RSDBindParamIO_t dir;
        QString name;

    private:
        RsdCommandEx *m_pParent;
        template<class T> void setBuffer(const QVariant &value)
        {
            const T &tmpValue = value.value<T>();
            this->valueSize = sizeof(T);
            this->value = malloc(static_cast<size_t>(valueSize));

            memcpy(this->value, &tmpValue, this->valueSize);
        }

        template<class T> void copyBuffer(const T &value)
        {
            this->valueSize = sizeof(T);
            this->value = malloc(static_cast<size_t>(valueSize));
            memcpy(this->value, &value, this->valueSize);
        }

        void setBuffer(const QString &str);
        void setBuffer(const QChar &qchar);
        void setBuffer(const QDate &qdate);
        void setBuffer(const QTime &qtime);
        void setBuffer(const QDateTime &qdatetime);
    };

    RsdCommandEx(CRsdConnection *con, RsdDriver *driver);
    virtual ~RsdCommandEx();

    void bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType paramType);
    void bindValue(int index, const QVariant &val, QSql::ParamType paramType);

    RsdDriver *driver();

private:
    QVector<BindParam*> m_Params;
    RsdDriver *m_pDriver;
};

#endif // RSDCOMMANDEX_H
