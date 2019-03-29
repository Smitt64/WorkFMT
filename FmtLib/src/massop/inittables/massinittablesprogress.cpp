#include "massinittablesprogress.h"
#include "ui_massinittablesprogress.h"
#include "errordlg.h"
#include "fmterrors.h"
#include "fmttable.h"
#include "massoperationwizard.h"
#include <QAbstractButton>
#include <QThreadPool>
#include <QThread>

MassInitTablesProgressRun::MassInitTablesProgressRun(const QStringList &tables, QObject *parent) :
    QObject(parent),
    QRunnable (),
    m_Tables(tables)
{
    setAutoDelete(true);
}

MassInitTablesProgressRun::~MassInitTablesProgressRun()
{

}

void MassInitTablesProgressRun::run()
{
    try {
        int size = m_Tables.size();
        for (int i = 0; i < size; i++)
        {
            QString table = m_Tables[i];
            emit message(QString("Начата обработка: %1").arg(table));
            QThread::sleep(1);
            emit progress(i + 1);
        }
    } catch (...) {
        emit error(tr("Exception handled"));
    }
    emit finished();
}

// ============================================================================

MassInitTablesProgress::MassInitTablesProgress(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassInitTablesProgress)
{
    ui->setupUi(this);
    setTitle(tr("Инициализация"));
    pErrors = new FmtErrors(this);
    pErrDlg = new ErrorDlg(ErrorDlg::mode_Widget);
    pErrDlg->setErrors(pErrors);
    ui->verticalLayout->insertWidget(0, pErrDlg);
}

MassInitTablesProgress::~MassInitTablesProgress()
{
    delete ui;
}

void MassInitTablesProgress::initializePage()
{
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
    wzrd->button(QWizard::BackButton)->setEnabled(false);

    ui->progressBar->setMaximum(wzrd->tables().size());
    MassInitTablesProgressRun *run = new MassInitTablesProgressRun(wzrd->tables());

    pErrors->clear();
    connect(run, &MassInitTablesProgressRun::progress, ui->progressBar, &QProgressBar::setValue);
    connect(run, SIGNAL(message(QString)), pErrors, SLOT(appendMessage(QString)));
    connect(run, SIGNAL(error(QString)), pErrors, SLOT(appendError(QString)));
    QThreadPool::globalInstance()->start(run);
}
