#include "expresultpage.h"
#include "exporttoxmlwizard.h"
#include "errordlg.h"
#include "fmtimpexpwrp.h"
#include <QDesktopServices>

ExpResultPage::ExpResultPage(ExportToXmlWizard *parent) :
    QWizardPage(parent)
{
    pWizard = parent;
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

    logTableView = new ErrorDlg(ErrorDlg::mode_Information);
    logTableView->setObjectName(QStringLiteral("logTableView"));
    logTableView->setErrors(&parent->m_Errors);
    logTableView->setMessage(tr("Протокол экспорта записей FMT словаря"));
    //logTableView->buttons()->setVisible(false);
    logTableView->buttons()->setStandardButtons(QDialogButtonBox::NoButton);

    verticalLayout->addWidget(logTableView);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

    progressBar = new QProgressBar(this);
    progressBar->setObjectName(QStringLiteral("progressBar"));
    progressBar->setMaximum(0);
    progressBar->setValue(-1);
    progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    progressBar->setTextVisible(false);

    horizontalLayout->addWidget(progressBar);

    openFolderButton = new QPushButton(this);
    openFolderButton->setObjectName(QStringLiteral("openFolderButton"));
    openFolderButton->setIcon(QIcon(":/img/openfolderHS.png"));
    openFolderButton->setText(tr("Открыть каталог экспорта"));
    logTableView->buttons()->addButton(openFolderButton, QDialogButtonBox::ActionRole);

    //horizontalLayout->addWidget(openFolderButton);

    /*cancelButton = new QPushButton(this);
    cancelButton->setText(tr("Отмена"));
    cancelButton->setObjectName(QStringLiteral("cancelButton"));*/

    //horizontalLayout->addWidget(cancelButton);
    verticalLayout->addLayout(horizontalLayout);

    fFinished = false;

    connect(openFolderButton, SIGNAL(clicked(bool)), SLOT(openExportFolder()));
}

bool ExpResultPage::isComplete() const
{
    return fFinished;
}

void ExpResultPage::prepare()
{
    fFinished = false;
    pWizard->m_Errors.clear();
    progressBar->setVisible(true);
    emit completeChanged();
}

void ExpResultPage::exportFinished(int code)
{
    fFinished = true;
    pWizard->pExp->parseProtocol(&pWizard->m_Errors);
    progressBar->setVisible(false);
    emit completeChanged();
}

void ExpResultPage::openExportFolder()
{
    QDesktopServices::openUrl(QUrl(field("dir").toString()));
}
