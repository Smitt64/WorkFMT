#include "massinittablesprogress.h"
#include "ui_massinittablesprogress.h"
#include "errordlg.h"
#include "fmterrors.h"
#include "fmttable.h"
#include "massoperationwizard.h"
#include "massinittablesparammodel.h"
#include "../massinittableoperation.h"
#include "fmtcore.h"
#include <QAbstractButton>
#include <QThreadPool>
#include <QThread>

MassInitTablesProgressRun::MassInitTablesProgressRun(MassInitTableOperation *Interface, QObject *parent) :
    QObject(parent),
    QRunnable (),
    pInterface(Interface)
{
    setAutoDelete(true);
}

MassInitTablesProgressRun::~MassInitTablesProgressRun()
{

}

void MassInitTablesProgressRun::run()
{
    MassInitTablesParamModel *pModel = pInterface->model();
    try {
        int size = pModel->rowCount(QModelIndex());
        for (int i = 0; i < size; i++)
        {
            QString table = pModel->data(pModel->index(i, MassInitTablesParamModel::FieldTableName), Qt::DisplayRole).toString();
            emit message(QString("Начата обработка: %1").arg(table));

            bool InitTable =  pModel->data(pModel->index(i, MassInitTablesParamModel::FieldInitTable), Qt::CheckStateRole).toInt() == Qt::Checked;
            bool InitIndex =  pModel->data(pModel->index(i, MassInitTablesParamModel::FieldInitIndex), Qt::CheckStateRole).toInt() == Qt::Checked;

            if (InitTable || InitIndex)
            {
                FmtTable fmttable;
                if (!fmttable.load(table))
                    emit error(tr("Не удалось загрузить таблицу %1").arg(table));
                else
                {
                    qint16 stat = 0;
                    QString err;
                    if (InitTable)
                    {
                        if ((stat = fmttable.createDbTable(&err)))
                            emit error(tr("Не удалось создать таблицу: %1").arg(err));
                        else
                            emit message(QString("Таблица %1 создана").arg(table));
                    }

                    if (!stat && InitIndex)
                    {
                        stat = InitFmtTableExec(&fmttable, &err);

                        if (stat)
                            emit error(tr("<b>DbInit завершился с кодом [%1]: %2</b>")
                                       .arg(stat)
                                       .arg(DbInitTextError(stat)));
                        else
                            emit message(tr("<b>DbInit завершился с кодом [%1]: %2</b>")
                                       .arg(stat)
                                       .arg(DbInitTextError(stat)));
                    }
                }
            }
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
    MassInitTableOperation *pInterface = qobject_cast<MassInitTableOperation*>(wzrd->getInterface());
    wzrd->button(QWizard::BackButton)->setEnabled(false);

    ui->progressBar->setMaximum(wzrd->tables().size());
    MassInitTablesProgressRun *run = new MassInitTablesProgressRun(pInterface, wzrd->tables());

    pErrors->clear();
    connect(run, &MassInitTablesProgressRun::progress, ui->progressBar, &QProgressBar::setValue);
    connect(run, SIGNAL(message(QString)), pErrors, SLOT(appendMessage(QString)));
    connect(run, SIGNAL(error(QString)), pErrors, SLOT(appendError(QString)));
    QThreadPool::globalInstance()->start(run);
}
