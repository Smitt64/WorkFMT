#ifndef MASSOPINTERFACE_H
#define MASSOPINTERFACE_H

#include <QObject>
#include "fmtlibfactory.h"
#include "loggingcategories.h"

class MassOperationWizard;
class QWizardPage;
class ConnectionInfo;
class MassOpInterface : public QObject
{
    Q_OBJECT
    friend class MassOperationWizard;
public:
    MassOpInterface(QObject *parent = nullptr);
    virtual ~MassOpInterface();

    virtual void initPages() = 0;
    virtual void deinitPages();

    ConnectionInfo *connectionInfo();
    void setConnectionInfo(ConnectionInfo *info);
    int addPage(QWizardPage *page);

signals:

public slots:

public:
    static FmtLibFactory<MassOpInterface,QString> m_pMassOpInterfaceFactory;

protected:
    QList<int> m_PageIds;
    void setWizard(MassOperationWizard *wzrd);
    MassOperationWizard *pWizard;
    ConnectionInfo *pInfo;
};

template<class T>
void registerMassOpInterface(const QString &alias, const QString &label)
{
    MassOpInterface::m_pMassOpInterfaceFactory.add<T>(alias, label);
    qInfo(logCore()) << "Operation interface registered: " << alias << ", Label: " << label;
}

#endif // MASSOPINTERFACE_H
