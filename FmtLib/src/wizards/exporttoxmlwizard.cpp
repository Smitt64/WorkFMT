#include "exporttoxmlwizard.h"
#include "tablesdockwidget.h"
#include "fmttablesmodel.h"
#include "fmtapplication.h"
#include "fmtimpexpwrp.h"
#include "expsettingspage.h"
#include "expresultpage.h"
#include <QFileDialog>
#include <QProgressDialog>

// ==================================

ExportToXmlWizard::ExportToXmlWizard(ConnectionInfo *connection, QWidget *parent)
    : QWizard(parent)
{
    FmtApplication *app = (FmtApplication*)qApp;
    m_pPrm = app->settings();

    pConnection = connection;

    pExp = new FmtImpExpWrp(pConnection, this);

    addPage(firstPage());
    addPage(resultPage());

    setMinimumSize(700, 590);

    connect(this, SIGNAL(currentIdChanged(int)), SLOT(pageIdChanged(int)));
    connect(button(QWizard::CancelButton), SIGNAL(clicked(bool)), pExp, SLOT(cancel()));
}

void ExportToXmlWizard::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter)
        event->ignore();
}

QWizardPage *ExportToXmlWizard::firstPage()
{
    QWizardPage *page = new ExpSettingsPage(this);
    page->setTitle(tr("Настройки экспорта"));
    return page;
}

QWizardPage *ExportToXmlWizard::resultPage()
{
    QWizardPage *page = new ExpResultPage(this);
    page->setTitle(tr("Экспорт"));
    return page;
}

void ExportToXmlWizard::pageIdChanged(int id)
{
    if (id == 1)
    {
        QWizardPage *p = page(id);
        connect(pExp, SIGNAL(finished(int)), p, SLOT(exportFinished(int)));
        m_Errors.appendMessage(tr("Начат экспорт %1 таблиц").arg(pExp->tablesCount()));
        pExp->exportTable(field("dir").toString());
    }
}

