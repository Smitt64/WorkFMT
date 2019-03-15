#ifndef MASSOPSELECTTABLESPAGE_H
#define MASSOPSELECTTABLESPAGE_H

#include <QWizardPage>

class TablesSelectWidget;
class ConnectionInfo;
class QVBoxLayout;
class MassOpSelectTablesPage : public QWizardPage
{
    Q_OBJECT
public:
    MassOpSelectTablesPage(ConnectionInfo *info, QWidget *parent = Q_NULLPTR);
    virtual ~MassOpSelectTablesPage();

    bool TablesSelectAddFunc(const QString &str);

private:
    TablesSelectWidget *pSeletTables;
    QVBoxLayout *pLayout;
};

#endif // MASSOPSELECTTABLESPAGE_H
