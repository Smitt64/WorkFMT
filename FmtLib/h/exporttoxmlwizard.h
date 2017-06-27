#ifndef EXPORTTOXMLWIZARD_H
#define EXPORTTOXMLWIZARD_H

#include <QObject>
#include <QtWidgets>
#include "fmterrors.h"
#include "fmtlib_global.h"

class ConnectionInfo;
class TablesDock;
class TablesDockWidget;
class FmtTablesModel;
class FmtImpExpWrp;
class FMTLIBSHARED_EXPORT ExportToXmlWizard : public QWizard
{
    Q_OBJECT
    friend class ExpSettingsPage;
    friend class ExpResultPage;
public:
    ExportToXmlWizard(ConnectionInfo *connection, QWidget *parent = NULL);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void pageIdChanged(int id);

private:
    QWizardPage *firstPage();
    QWizardPage *resultPage();

    QSettings *m_pPrm;
    ConnectionInfo *pConnection;
    FmtImpExpWrp *pExp;

    QVBoxLayout *verticalLayout;
    QTableView *logTableView;
    QHBoxLayout *horizontalLayout;
    QProgressBar *progressBar;
    QToolButton *openFolderButton;
    QPushButton *cancelButton;

    FmtErrors m_Errors;
};

#endif // EXPORTTOXMLWIZARD_H
