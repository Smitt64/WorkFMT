#include "exppgparampage.h"
#include "ui_exppgparampage.h"
#include "selectserverdlg.h"
#include "dumptoolwizard.h"
#include "pgexportaddprmdlg.h"
#include <QFileDialog>
#include <QSettings>

ExpPgParamPage::ExpPgParamPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ExpPgParamPage)
{
    ui->setupUi(this);

    setTitle("Параметры экспорта");
    registerField("PgExpServer", ui->serverImpEdit);
    registerField("PgExpPort", ui->portImpBox);
    registerField("PgExpUser", ui->userImpEdit);
    registerField("PgExpDatabase", ui->databaseImpEdit);
    registerField("PgExpPath", ui->exportPath);

    ui->addParamsBtn->setDescription("test");

    connect(ui->selectServer, &QToolButton::clicked, this, &ExpPgParamPage::onSelectServer);

    connect(ui->serverImpEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->userImpEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->databaseImpEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->portImpBox, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
}

ExpPgParamPage::~ExpPgParamPage()
{
    delete ui;
}

bool ExpPgParamPage::validatePage()
{
    return true;
}

bool ExpPgParamPage::isComplete() const
{
    if (ui->serverImpEdit->text().isEmpty() ||
            ui->userImpEdit->text().isEmpty() ||
            ui->databaseImpEdit->text().isEmpty() ||
            !ui->portImpBox->value() ||
            !ui->exportPath->text().isEmpty())
        return false;

    DumpToolWizard *wzrd = qobject_cast<DumpToolWizard*>(wizard());
    wzrd->addUserField("PgBinPath", GetPostgreSQLInstallLocation().absolutePath());

    return true;
}

int ExpPgParamPage::nextId() const
{
    return DumpToolWizard::eSummaryPage;
}

void ExpPgParamPage::onSelectServer()
{
    SelectServerDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        ui->serverImpEdit->setText(dlg.getHost());
        ui->portImpBox->setValue(dlg.getPort());
        ui->userImpEdit->setText(dlg.getUserName());
        ui->databaseImpEdit->setText(dlg.getDatabase());

        if (!ui->exportPath->text().isEmpty())
        {
            QFileInfo info(ui->exportPath->text());
            QDir baseDir = info.absoluteDir();

            ui->exportPath->setText(baseDir.absoluteFilePath(QString("%1.%2")
                                                             .arg(ui->databaseImpEdit->text())
                                                             .arg(info.completeSuffix())));
        }
    }
}

void ExpPgParamPage::on_selfileButton_clicked()
{
    QDir dir = QDir::current();
    QString filename = QFileDialog::getSaveFileName(this, tr("Выбор файла, куда сохранять дамп"),
                                                    dir.absoluteFilePath(QString("%1.sql").arg(ui->databaseImpEdit->text())),
                                                    QString("Sql файл (*.sql);;Текстовый файл (*.txt)"));

    if (!filename.isEmpty())
        ui->exportPath->setText(filename);
}

QDir GetPostgreSQLInstallLocation()
{
    QDir result;
    //WOW6432Node
    QSettings settings64("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", QSettings::NativeFormat);
    QStringList childKeys = settings64.childGroups();

    for (const QString &key : childKeys)
    {
        if (key.contains("PostgreSQL"))
        {
            settings64.beginGroup(key);
            QString InstallLocation = settings64.value("InstallLocation").toString();

            if (!InstallLocation.isEmpty())
            {
                QDir tmp(InstallLocation);

                if (tmp.cd("bin"))
                {
                    if (QFile::exists(tmp.absoluteFilePath("pg_dump.exe")) &&
                            QFile::exists(tmp.absoluteFilePath("psql.exe")))
                    {
                        result = tmp;
                        break;
                    }
                }
            }
            settings64.endGroup();
        }
    }
    return result;
}

void ExpPgParamPage::on_addParamsBtn_clicked()
{
    PgExportAddPrmDlg dlg(this);
    dlg.setParams(ui->addParamsBtn->description());

    if (dlg.exec() == QDialog::Accepted)
    {
        QStringList lst = dlg.params();
        QString desc = lst.join(",");

        ui->addParamsBtn->setDescription(desc);

        DumpToolWizard *wzrd = qobject_cast<DumpToolWizard*>(wizard());
        wzrd->addUserField("PgExpAddParams", desc);

    }
}

