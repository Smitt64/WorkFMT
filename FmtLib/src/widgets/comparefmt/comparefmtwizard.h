#ifndef COMPAREFMTWIZARD_H
#define COMPAREFMTWIZARD_H

#include <QWizard>

class CompareParamsPage;
class CompareCppStructPage;
class ConnectionInfo;
class CompareSelectTablePage;
class CompareFmtWizard : public QWizard
{
    Q_OBJECT
public:
    enum Action
    {
        CompareStruct = 0,
        CompareTable,
        CompareConnection
    };

    enum Pages
    {
        PageParams = 0,
        PageStruct,
        PageSelectTable
    };

    CompareFmtWizard(ConnectionInfo *connection, QWidget *parent = nullptr);
    virtual ~CompareFmtWizard();

    QString structString() const;
    ConnectionInfo *connection();

private:
    CompareParamsPage *m_ParamsPage;
    CompareCppStructPage *m_CodePage;
    CompareSelectTablePage *m_pSeltablePage;

    ConnectionInfo *m_Connection;
};

#endif // COMPAREFMTWIZARD_H
