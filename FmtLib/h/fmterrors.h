#ifndef FMTERRORS_H
#define FMTERRORS_H

#include <QObject>
#include <QAbstractItemModel>
#include <QDateTime>
#include "fmtlib_global.h"

typedef struct
{
    qint16 type;
    QString text;
    QDateTime time;
}FmtErrorStruct;

class FMTLIBSHARED_EXPORT FmtErrors : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasErrors READ hasErrors)
    Q_PROPERTY(bool hasWarnings READ hasWarnings)
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(qint16 type READ type)
    Q_PROPERTY(qint32 errorsCount READ errorsCount)
    Q_PROPERTY(qint32 warningsCount READ warningsCount)
public:
    enum FmtErrorType
    {
        fmtet_Error = 0,
        fmtet_Warning,
        fmtet_Info
    };
    Q_ENUM(FmtErrorType)
    explicit FmtErrors(QObject *parent = Q_NULLPTR);
    virtual ~FmtErrors();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex &index) const;
    virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
    void errorsCountChanged(quint32);
    void warningsCountChanged(quint32);
    void infoCountChanged(quint32);

public slots:
    bool hasErrors();
    bool hasWarnings();
    bool isEmpty();

    void appendError(const QString &text, const qint16 &type = fmtet_Error, const QDateTime &dateTime = QDateTime());
    void appendMessage(const QString &text, const QDateTime &dateTime = QDateTime());

    int errorsCount();
    int warningsCount();
    int infoCount();
    qint16 type(const qint32 &row = -1) const;
    QString text() const;
    QString textAt(const int &i) const;

    bool next();
    bool first();
    void clear();

private:
    quint32 m_errorsCount, m_warningsCount, m_infoCount;
    QList<FmtErrorStruct> m_errors;
    QList<FmtErrorStruct>::iterator m_Iterator;
};

#endif // FMTERRORS_H
