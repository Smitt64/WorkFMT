#ifndef RSDCOMMANDEX_H
#define RSDCOMMANDEX_H

#include <fdecimal.h>
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
        QByteArray nameBytes; // хранит toLocal8Bit() для передачи в addParam
        QByteArray byteArrayValue; // хранит сырые данные для RSDPT_BLOB/RSDPT_BINARY

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

    // Хранилище буферов для пакетной (array) вставки одного параметра.
    // value  - непрерывный буфер из count элементов с фиксированным шагом stride;
    // indLen - массив из count индикаторов длины/состояния (RSDBS_NULL для NULL).
    class BatchParam Q_DECL_FINAL
    {
    public:
        BatchParam() : value(nullptr), indLen(nullptr) {}
        ~BatchParam()
        {
            if (value)
                free(value);
            if (indLen)
                free(indLen);
        }

        void *value;
        long *indLen;
    };

    RsdCommandEx(CRsdConnection *con, RsdDriver *driver);
    virtual ~RsdCommandEx();

    void bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType paramType);
    void bindValue(int index, const QVariant &val, QSql::ParamType paramType);

    // Привязка массива значений одного параметра для пакетной вставки.
    // Перед серией вызовов должен быть выставлен setParamArraySize(values.size()).
    void bindBatch(const QString &placeholder, const QVariantList &values, QSql::ParamType paramType);

    // Освобождение буферов пакетной вставки (после/перед clearParams()).
    void clearBatch();

    RsdDriver *driver();

private:
    QVector<BindParam*> m_Params;
    QVector<BatchParam*> m_BatchParams;
    QVector<QByteArray> m_BatchParamNames; // хранит имена placeholder'ов для batch привязки
    RsdDriver *m_pDriver;
};

#endif // RSDCOMMANDEX_H
