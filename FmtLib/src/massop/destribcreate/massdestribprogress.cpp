#include "massdestribprogress.h"
#include "ui_massdestribprogress.h"
#include "errordlg.h"
#include "ErrorsModel.h"
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
#include <QEventLoop>

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
    QStringList updScript;

    MassDestribParamModel *pModel = pInterface->model();
    try {
        QScopedPointer<ErrorsModel> tmp(new ErrorsModel());
        connect(tmp.data(), &ErrorsModel::newError, [=](const QString &msg)
        {
            emit error(msg);
        });

        connect(tmp.data(), &ErrorsModel::newMessage, [=](const QString &msg)
        {
            emit message(msg);
        });

        int size = pModel->rowCount(QModelIndex());
        for (int i = 0; i < size; i++)
        {
            tmp->clear();
            const MassDestribParamModelElement *element = pModel->getTableParam(i);
            QModelIndex index = pModel->index(i, MassDestribParamModel::fld_Name);
            QString table = pModel->data(index, Qt::DisplayRole).toString();
            emit message(QString("Начата обработка: %1").arg(table));
            QString tableUpper = table.toUpper();
            QString tableName = element->table->name();

            if (element->UnloadCreateTables) {
                FmtGenTablesSql gen;
                QMap<QString, QByteArray> data = gen.makeContent(element->table);
                QString code = QString::fromLocal8Bit(data.first());
                QString UnloadCreateTablesTitle = QString("Выгрузка CreateTables для %1").arg(tableName);
                emit message(UnloadCreateTablesTitle);
                writefile(m_CreateTables, QString("%1.sql").arg(tableUpper), code);
                code = FmtGenUpdateCreateTableScript(element->table);
                emit message(QString("Выгрузка скрипта создания таблицы"));
                writefile(m_UpdateScript, QString("%1_create.sql").arg(table), code);
            }

            if (element->UnloadFmt) {
                emit message(QString("Выгрузка fmt xml для %1").arg(tableName));

                QEventLoop loop;
                QScopedPointer<FmtImpExpWrp> imp(new FmtImpExpWrp(element->table->connection()));
                connect(imp.data(), &FmtImpExpWrp::finished, &loop, &QEventLoop::quit);

                imp->setDsn(m_dsn);
                imp->addTable(table);
                imp->exportTable(m_FmtDir.path());
                loop.exec();
                imp->parseProtocol(tmp.data());
            }

            if (element->UnloadDat) {
                emit message(QString("Выгрузка таблицы %1 в *.dat").arg(tableName));
                tmp->clear();

                QEventLoop loop;
                QScopedPointer<FmtDbfToolWrp> wrp(new FmtDbfToolWrp(element->table->connection()));
                connect(wrp.data()->errorsModel(), &ErrorsModel::newError, this, &MassDestribProgressRun::error);
                connect(wrp.data()->errorsModel(), &ErrorsModel::newMessage, this, &MassDestribProgressRun::message);
                connect(wrp.data(), &FmtDbfToolWrp::finished, &loop, &QEventLoop::quit);

                wrp->disconnect(wrp.data()->errorsModel(), &ErrorsModel::newError, Q_NULLPTR, Q_NULLPTR);
                wrp->disconnect(wrp.data()->errorsModel(), &ErrorsModel::newMessage, Q_NULLPTR, Q_NULLPTR);
                wrp->setDsn(m_dsn);
                wrp->unload(m_DatDir.path(), table);
                loop.exec();
                //dlg.setErrors(tmp.da);
            }

            QList<FmtField*> addFldList;
            QList<FmtField*> updFldList;
            QList<FmtField*> remFldList;

            for (int j = 0; j < pModel->rowCount(index); j++)
            {
                qint16 action = pModel->data(pModel->index(j, MassDestribParamModel::fld_Action, index), Qt::EditRole).value<qint16>();

                switch(action)
                {
                case MassDestribParamModel::ActionFldAdd:
                    addFldList.append(element->table->field(j));
                    break;
                case MassDestribParamModel::ActionFldEdit:
                    updFldList.append(element->table->field(j));
                    break;
                case MassDestribParamModel::ActionFldRemove:
                    remFldList.append(element->table->field(j));
                    break;
                }
            }

            if (!addFldList.isEmpty())
                updScript.append(FmtGenUpdateAddColumnScript(addFldList));

            if (!updFldList.isEmpty())
                updScript.append(FmtGenModifyColumnScript(updFldList));

            if (!remFldList.isEmpty())
                updScript.append(FmtGenUpdateDeleteColumnScript(remFldList));

            emit progress(i + 1);
        }

        if (!updScript.isEmpty())
        {
            QString updtFileName = m_UpdateScript.absoluteFilePath("updt.sql");
            emit message(QString("Создание скрипта обновления таблиц: %1").arg(updtFileName));
            QFile updtFile(updtFileName);

            if (updtFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream stream(&updtFile);
                stream.setCodec("IBM 866");
                for (const QString &sql : updScript)
                    stream << sql << endl;
                updtFile.close();
            }
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
    pErrors = new ErrorsModel(this);
    pErrDlg = new ErrorDlg(ErrorDlg::ModeWidget);
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

    connect(run, &MassDestribProgressRun::error, [=](QString msg)
    {
       pErrors->appendError(msg);
    });

    connect(run, &MassDestribProgressRun::message, [=](QString msg)
    {
       pErrors->appendMessage(msg);
    });

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
        dsn = element->table->connection()->dsn();
        run->setDsn(dsn);
    }
    QThreadPool::globalInstance()->start(run);
}

bool MassDestribProgress::isComplete() const
{
    return fIsComplete;
}
