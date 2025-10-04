#ifndef FMTTABLECOLLECTION_H
#define FMTTABLECOLLECTION_H

#include <QObject>
#include "fmtlib_global.h"

class FmtTable;
class FmtTableCollection : public QObject
{
    Q_OBJECT
public:
    explicit FmtTableCollection(QObject *parent = nullptr);

    virtual bool hasNext() const = 0;
    virtual QSharedPointer<FmtTable> next() = 0;
    virtual void reset() = 0;

    QString currentItemName() const { return m_currentItemName; }
    virtual QString collectionType() const = 0;

signals:
    void loadingFinished();
    void errorOccurred(const QString &error);

protected:
    QString m_currentItemName;
};

class FMTLIBSHARED_EXPORT FmtXmlTableCollection : public FmtTableCollection
{
    Q_OBJECT

public:
    FmtXmlTableCollection(const QString &directoryPath, QObject *parent = nullptr);
};

#endif // FMTTABLECOLLECTION_H
