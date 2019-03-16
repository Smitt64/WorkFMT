#ifndef MASSOPERATIONWIZARD_H
#define MASSOPERATIONWIZARD_H

#include <QWizard>

class ConnectionInfo;
class MassOpInterface;
class MassOperationWizard : public QWizard
{
    Q_OBJECT
public:
    enum StandartPages
    {
        PageSelectOperation = 0,
        PageSelectTables,
        PageUser,
    };
    MassOperationWizard(ConnectionInfo *info, QWidget *parent = Q_NULLPTR);
    virtual ~MassOperationWizard();

    ConnectionInfo *connectionInfo();
    bool addTables(const QString &str);
    bool removeTables(const QString &str);

    QString selectedOpeation() const;
    void initInterface();

private:
    QWizardPage *createSelectOperationPage();
    QWizardPage *createSelectTablesPage(ConnectionInfo *info);
    ConnectionInfo *pInfo;
    QStringList m_Tables;
    MassOpInterface *pInterface;
};

#endif // MASSOPERATIONWIZARD_H
