#ifndef RSDCOMMANDEX_H
#define RSDCOMMANDEX_H

#include <RsdC.h>
#include <QtSql>
#include <QVector>

class RsdCommandEx : public CRsdCommand
{
public:
    class BindParam Q_DECL_FINAL
    {
        friend class RsdCommandEx;

    public:
        BindParam(const QVariant &value, QSql::ParamType paramType, const QString &pname);
        ~BindParam();

        int typeSize(const QVariant::Type &type);

        void *value;
        RSDLONG valueSize;
        RSDValType_t valType;
        RSDBindParamIO_t dir;
        QString name;

    private:
        template<class T> void setBuffer(const QVariant &value)
        {
            const T &tmpValue = value.value<T>();
            this->valueSize = sizeof(T);
            this->value = malloc(static_cast<size_t>(valueSize));

            memcpy(this->value, &tmpValue, this->valueSize);
        }
    };

    RsdCommandEx(CRsdConnection *con);
    virtual ~RsdCommandEx();

    void bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType paramType);
    void bindValue(int index, const QVariant &val, QSql::ParamType paramType);

private:
    QVector<BindParam*> m_Params;
};

#endif // RSDCOMMANDEX_H
