#include "pgimpparampage.h"
#include "ui_pgimpparampage.h"
#include "selectserverdlg.h"
#include "pgaddjarsdlg.h"
#include "dumptoolwizard.h"
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

PgImpParamPage::PgImpParamPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::PgImpParamPage)
{
    ui->setupUi(this);

    setTitle("Параметры импорта");

    registerField("PgImpServer", ui->serverImpEdit);
    registerField("PgImpPort", ui->portImpBox);
    registerField("PgImpAdmin", ui->adminEdit);
    registerField("PgImpOwner", ui->ownerEdit);
    registerField("PgImpPath", ui->dumpPath);
    registerField("PgImpDstUser", ui->userDestEdit);

    connect(ui->serverImpEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->portImpBox, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->adminEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->ownerEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->dumpPath, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->userDestEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
}

PgImpParamPage::~PgImpParamPage()
{
    delete ui;
}

void PgImpParamPage::on_selectServer_clicked()
{
    SelectServerDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        ui->serverImpEdit->setText(dlg.getHost());
        ui->portImpBox->setValue(dlg.getPort());
        ui->adminEdit->setText(dlg.getUserName());
    }
}


void PgImpParamPage::on_selfileButton_clicked()
{
    DumpToolWizard *wzrd = qobject_cast<DumpToolWizard*>(wizard());
    QDir dir = QDir::current();

    wzrd->settings()->beginGroup("PgImpParamPage");
    QString lastPath = wzrd->settings()->value("LastDir").toString();
    wzrd->settings()->endGroup();

    QString filename = QFileDialog::getOpenFileName(this, tr("Выбор файла, который загружать"),
                                                    QString(),
                                                    QString("Sql файл (*.sql);;Текстовый файл (*.txt);;Все файлы (*.*)"));

    if (!filename.isEmpty())
    {
        QFileInfo fi(filename);
        wzrd->settings()->beginGroup("PgImpParamPage");
        wzrd->settings()->setValue("LastDir", fi.absoluteDir().path());
        wzrd->settings()->endGroup();
        wzrd->settings()->sync();

        QFile f(filename);
        if (f.open(QIODevice::ReadOnly))
        {
            QString mask = "ALTER SCHEMA anydata OWNER TO ";
            QTextStream stream(&f);
            while (!stream.atEnd())
            {
                QString line = QString(stream.readLine()).trimmed();

                if (line.contains(mask))
                {
                    int size = mask.length();
                    QString owner = line.mid(size, line.indexOf(";") - size);
                    ui->ownerEdit->setText(owner);
                    ui->dumpPath->setText(filename);
                    break;
                }
            }
            f.close();

            if (ui->ownerEdit->text().isEmpty())
                QMessageBox::critical(this, tr("Выбор файла, который загружать"),
                                      tr("В файле не найдена информация о \"вледельце\" дампа"));
        }
    }
}

bool PgImpParamPage::isComplete() const
{
    if (ui->serverImpEdit->text().isEmpty() ||
            ui->portImpBox->text().isEmpty() ||
            ui->adminEdit->text().isEmpty() ||
            ui->ownerEdit->text().isEmpty() ||
            ui->dumpPath->text().isEmpty() ||
            ui->userDestEdit->text().isEmpty())
        return false;

    return true;
}

int PgImpParamPage::nextId() const
{
    return DumpToolWizard::eSummaryPage;
}

void PgImpParamPage::on_jarButton_clicked()
{
    DumpToolWizard *wzrd = qobject_cast<DumpToolWizard*>(wizard());

    PgAddJarsDlg dlg(wzrd->settings(), this);
    dlg.addStatic("PLXmlFunctions.jar");
    dlg.addList(wzrd->userField("PgJarFiles").toStringList());

    if (dlg.exec() == QDialog::Accepted)
    {
        QStringList shortNames;
        QStringList files = dlg.files();

        for (const QString &file : files)
        {
            QFileInfo fi(file);
            shortNames.append(fi.fileName());
        }

        files.removeFirst();

        wzrd->addUserField("PgJarFiles", files);
        ui->jarButton->setDescription(shortNames.join(";"));
    }
}

