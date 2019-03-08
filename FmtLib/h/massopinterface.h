#ifndef MASSOPINTERFACE_H
#define MASSOPINTERFACE_H

#include <QObject>
#include "fmtlibfactory.h"

class MassOpInterface : public QObject
{
    Q_OBJECT
public:
    MassOpInterface(QObject *parent = nullptr);
    virtual ~MassOpInterface();

signals:

public slots:

public:
    static FmtLibFactory<MassOpInterface,QString> m_pMassOpInterfaceFactory;
};

template<class T>
void registerMassOpInterface(const QString &alias, const QString &label)
{
    MassOpInterface::m_pMassOpInterfaceFactory.add<T>(alias, label);
    qInfo(logCore()) << "Operation interface registered: " << alias << ", Label: " << label;
}

#endif // MASSOPINTERFACE_H
