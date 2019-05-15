#include "massdestribprogress.h"
#include "ui_massdestribprogress.h"
#include "errordlg.h"
#include "fmterrors.h"
#include "fmttable.h"
#include "massoperationwizard.h"
#include "massdestribcreate.h"
#include "massdestribparammodel.h"
#include "fmtapplication.h"
#include "fmtcore.h"
#include "fmtgentablessql.h"
#include "fmtimpexpwrp.h"
#include "fmtdbftoolwrp.h"
#include <QAbstractButton>
#include <QFile>
#include <QTextStream>
#include <QThreadPool>
#include <QThread>

MassDestribProgressRun::MassDestribProgressRun(MassDestribCreate *Interface, QObject *parent) :
    QObject(parent),
    QRunnable (),
    pInterface(Interface)
{
    setAutoDelete(true);
}

MassDestribProgressRun::~MassDestribProgressRun()
{

}

QDir MassDestribProgressRun::getDir(const QDir &top, const QString &dirpath)
{
    QDir dir = top;
    if (!dir.cd(dirpath))
    {
        if (dir.mkdir(dirpath))
        {
            dir.cd(dirpath);
            emit message(QString("Создан каталог %1").arg(dir.path()));
        }
    }
    return dir;
}

void MassDestribProgressRun::writefile(const QDir &dir, const QString &filename, const QString &data)
{
    QString fullFileName = dir.absoluteFilePath(filename);
    QFile file(fullFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit error(tr("Не удалось создать файл %1. %2")
                   .arg(fullFileName)
                   .arg(file.errorString()));
    }
    else {
        QTextStream stream(&file);
        stream.setCodec("IBM 866");
        stream << data;
    }
    file.close();
}

void MassDestribProgressRun::setDsn(const QString &dsn)
{
    m_dsn = dsn;
}

void MassDestribProgressRun::run()
{
    FmtApplication *app = (FmtApplication*)qApp;
    m_pPrm = app->settings();

    m_pPrm->beginGroup("MassDestrib");
    QDir m_Dir(m_pPrm->value("unloaddir", QDir::currentPath()).toString());
    if (!m_Dir.exists()) {
        emit message(QString("Каталог %1 не существует, попытка создать...").arg(m_Dir.path()));
        if (!m_Dir.mkpath(m_Dir.path())) {
            emit error(tr("Не удалось создать каталог %1")
                       .arg(m_Dir.path()));
            emit finished();
            return;
        }
        else {
            emit message(QString("Каталог создан  %1").arg(m_Dir.path()));
        }
    }
    m_pPrm->endGroup();

    QDir m_CreateTables = getDir(m_Dir, "CreateTablesSql");
    QDir m_UpdateScript = getDir(m_Dir, "Update");
    QDir m_FmtDir = getDir(m_Dir, "fmt");
    QDir m_DatDir = getDir(m_Dir, "dbfile");
    MassDestribParamModel *pModel = pInterface->model();
    try {
        QScopedPointer<FmtErrors> tmp(new FmtErrors());
        connect(tmp.data(), &FmtErrors::newError, this, &MassDestribProgressRun::error);
        connect(tmp.data(), &FmtErrors::newMessage, this, &MassDestribProgressRun::message);

        int size = pModel->rowCount(QModelIndex());
        for (int i = 0; i < size; i++)
        {
            tmp->clear();
            const MassDestribParamModelElement *element = pModel->getTableParam(i);
            QString table = pModel->data(pModel->index(i, MassDestribParamModel::fld_Name), Qt::DisplayRole).toString();
            emit message(QString("Начата обработка: %1").arg(table));
            QString tableUpper = table.toUpper();

            if (element->UnloadCreateTables) {
                FmtGenTablesSql gen;
                QString code = QString::fromLocal8Bit(gen.makeContent(element->table));
                emit message(QString("Выгрузка CreateTables"));
                writefile(m_CreateTables, QString("%1.sql").arg(tableUpper), code);
                code = FmtGenUpdateCreateTableScript(element->table);
                emit message(QString("Выгрузка скрипта создания таблицы"));
                writefile(m_UpdateScript, QString("%1_create.sql").arg(table), code);
            }

            if (element->UnloadFmt) {
                emit message(QString("Выгрузка fmt"));
                QScopedPointer<FmtImpExpWrp> imp(new FmtImpExpWrp(element->table->connection()));
                imp->setDsn(m_dsn);
                imp->addTable(table);
                imp->exportTable(m_FmtDir.path());
                imp->parseProtocol(tmp.data());
            }

            if (element->UnloadDat) {
                emit message(QString("Выгрузка *.dat"));
                tmp->clear();

                QScopedPointer<FmtDbfToolWrp> wrp(new FmtDbfToolWrp(element->table->connection()));
                connect(wrp.data()->fmterrors(), &FmtErrors::newError, this, &MassDestribProgressRun::error);
                connect(wrp.data()->fmterrors(), &FmtErrors::newMessage, this, &MassDestribProgressRun::message);
                wrp->disconnect(wrp.data()->fmterrors(), &FmtErrors::newError, Q_NULLPTR, Q_NULLPTR);
                wrp->disconnect(wrp.data()->fmterrors(), &FmtErrors::newMessage, Q_NULLPTR, Q_NULLPTR);
                wrp->setDsn(m_dsn);
                wrp->unload(m_DatDir.path(), table);
                //dlg.setErrors(tmp.da);
            }

            //if (element->)
            emit progress(i + 1);
        }
    } catch (...) {
        emit error(tr("Exception handled"));
    }
    emit finished();
}

// ============================================================================

MassDestribProgress::MassDestribProgress(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassDestribProgress)
{
    ui->setupUi(this);
    setTitle(tr("Выгрузка"));
    pErrors = new FmtErrors(this);
    pErrDlg = new ErrorDlg(ErrorDlg::mode_Widget);
    pErrDlg->setErrors(pErrors);
    ui->verticalLayout->insertWidget(0, pErrDlg);
}

MassDestribProgress::~MassDestribProgress()
{
    delete ui;
}

void MassDestribProgress::initializePage()
{
    fIsComplete = false;
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
    MassDestribCreate *pInterface = qobject_cast<MassDestribCreate*>(wzrd->getInterface());
    wzrd->button(QWizard::BackButton)->setEnabled(false);

    ui->progressBar->setMaximum(wzrd->tables().size());
    MassDestribProgressRun *run = new MassDestribProgressRun(pInterface);

    pErrors->clear();
    connect(run, &MassDestribProgressRun::progress, ui->progressBar, &QProgressBar::setValue);
    connect(run, SIGNAL(message(QString)), pErrors, SLOT(appendMessage(QString)));
    connect(run, SIGNAL(error(QString)), pErrors, SLOT(appendError(QString)));
    connect(run, &MassDestribProgressRun::finished, [=]()
    {
        fIsComplete = true;
        emit completeChanged();
    });

    MassDestribParamModel *pModel = pInterface->model();
    if (pModel->rowCount())
    {
        QString dsn;
        const MassDestribParamModelElement *element = pModel->getTableParam(0);
        dsn = DatasourceFromService(element->table->connection()->service());
        run->setDsn(dsn);
    }
    QThreadPool::globalInstance()->start(run);
}

bool MassDestribProgress::isComplete() const
{
    return fIsComplete;
}
