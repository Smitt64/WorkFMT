#ifndef MASSOPERATIONWIZARD_H
#define MASSOPERATIONWIZARD_H

#include <QWizard>

class ConnectionInfo;
class MassOperationWizard : public QWizard
{
    Q_OBJECT
public:
    MassOperationWizard(ConnectionInfo *info, QWidget *parent = Q_NULLPTR);
    virtual ~MassOperationWizard();

    ConnectionInfo *connectionInfo();
    bool addTables(const QString &str);

private:
    QWizardPage *createSelectOperationPage();
    QWizardPage *createSelectTablesPage(ConnectionInfo *info);
    ConnectionInfo *pInfo;
    QStringList m_Tables;
};

#endif // MASSOPERATIONWIZARD_H
