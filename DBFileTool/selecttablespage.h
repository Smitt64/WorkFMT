#ifndef SELECTTABLESPAGE_H
#define SELECTTABLESPAGE_H

#include <QWizardPage>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QFrame>

class ConnectionInfo;
class TablesSelectWidget;
class SelectTablesPage : public QWizardPage
{
    Q_OBJECT
public:
    SelectTablesPage(QWidget *parent = nullptr);
    virtual ~SelectTablesPage();

    virtual void initializePage();

    bool AddFunc(const QString &str);
    bool RemoveFunc(const QString &str);

    virtual bool isComplete() const;
    virtual bool validatePage();
    virtual int nextId() const;

private:
    QVBoxLayout *pLayout;
    TablesSelectWidget *m_pTables;

    QScopedPointer<ConnectionInfo> m_pConnection;
    QLabel *m_PathLabel;
    QLineEdit *m_pPathEdit;
    QToolButton *m_pSelectFolder;
    QHBoxLayout *m_pEditLayout;

    QFrame *m_pHorLine;
};

#endif // SELECTTABLESPAGE_H
