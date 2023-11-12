#include "impfileselectpage.h"
#include "ui_impfileselectpage.h"
#include "dumptoolwizard.h"
#include "tnsnames.h"
#include "filelistdialog.h"
#include "selserverdirdlg.h"
#include <QFileDialog>
#include <QFileInfo>

ImpFileSelectPage::ImpFileSelectPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ImpFileSelectPage)
{
    ui->setupUi(this);

    ui->fileButton->setVisible(false);

    setTitle(tr("Параметры файла дампа"));
    registerField("Location", ui->locationBox, "currentIndex");
    registerField("LocationName", ui->locationBox, "currentText");
    registerField("PathToDump", ui->pathEdit);
    registerField("DumpFile", ui->dumpEdit);
    registerField("SchemeOwner", ui->ownerEdit);
    registerField("ImportPath", ui->importEdit);

    connect(ui->locationBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ImpFileSelectPage::LoacationChanged);

    connect(ui->locationBox, SIGNAL(currentTextChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->pathEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->dumpEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->ownerEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->importEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
}

ImpFileSelectPage::~ImpFileSelectPage()
{
    delete ui;
}

void ImpFileSelectPage::initializePage()
{
    DumpToolWizard *wzrd = qobject_cast<DumpToolWizard*>(wizard());

    QString IMPDP_DIR1 = wzrd->userField("IMPDP_DIR1").toString();

    if (!IMPDP_DIR1.isEmpty())
        ui->importEdit->setText(IMPDP_DIR1);
    else
        ui->importEdit->setText(wzrd->userField("EXPDP_DIR1").toString());

    ui->importLabel->setText(QString("Каталог <b><i>импорта</i></b> на сервере <b>%1</b>:").arg(field("Service").toString()));
}

void ImpFileSelectPage::LoacationChanged(int index)
{
    DumpToolWizard *wzrd = qobject_cast<DumpToolWizard*>(wizard());
    if (index == Local)
    {
        ui->infoEdit->setText(tr("Файл на моей машине:"));
        ui->pathEdit->setText(QString());
        ui->pathEdit->setReadOnly(true);
        ui->dumpEdit->setText(QString());
        ui->fileButton->setVisible(false);
        ui->expSelectButton->setVisible(true);

        ui->expSelectButton->setIcon(QIcon(":/img/openfolderHS.png"));
        ui->expSelectButton->setToolTip(tr("Выбор файла"));
    }
    else if (index == SameServer || index == ImpdpServer)
    {
        if (index == SameServer)
        {
            ui->infoEdit->setText(tr("Файл в каталоге <b><i>экспорта</i></b> на сервере <b>%1</b>:").arg(field("Service").toString()));
            ui->pathEdit->setText(wzrd->userField("EXPDP_DIR1").toString());
        }
        else
        {
            ui->infoEdit->setText(tr("Файл в каталоге <b><i>импорта</i></b> на сервере <b>%1</b>:").arg(field("Service").toString()));
            ui->pathEdit->setText(wzrd->userField("IMPDP_DIR1").toString());
        }

        ui->pathEdit->setReadOnly(false);
        ui->dumpEdit->setText(QString());
        ui->expSelectButton->setVisible(false);
        ui->fileButton->setVisible(true);
    }
    else if (index == OtherServer)
    {
        ui->infoEdit->setText(tr("Файл на другом сервере БД:"));
        ui->pathEdit->setText(QString());
        ui->pathEdit->setReadOnly(false);
        ui->dumpEdit->setText(QString());
        ui->fileButton->setVisible(true);
        ui->expSelectButton->setVisible(true);
        ui->expSelectButton->setIcon(QIcon(":/img/dpplg.dll_I0095_0409.ico"));
        ui->expSelectButton->setToolTip(tr("Выбор каталога сервера"));
    }
}

void ImpFileSelectPage::on_expSelectButton_clicked()
{
    int location = ui->locationBox->currentIndex();

    if (location == Local)
    {
        QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(),
                                                        tr("Файл дампа (*.dmp)"));

        if (!fileName.isEmpty())
        {
            QFileInfo fi(fileName);
            ui->pathEdit->setText(fi.canonicalPath());
            ui->dumpEdit->setText(fi.fileName());
            PumpOwner();
        }
    }
    else if (location == OtherServer)
    {
        SelServerDirDlg dlg(this);

        if (dlg.exec() == QDialog::Accepted)
        {
            ui->pathEdit->setText(dlg.path());
            ui->infoEdit->setText(QString("Файл на сервере <b>%1</b>").arg(dlg.service()));
        }
    }
}

void ImpFileSelectPage::PumpOwner()
{
    QDir path(ui->pathEdit->text());
    QFile dmpFile(path.absoluteFilePath(ui->dumpEdit->text()));

    if (dmpFile.open(QIODevice::ReadOnly))
    {
        QByteArray data = dmpFile.read(500);
        int pos = data.indexOf("\".\"SYS_EXPORT_SCHEMA");
        int lastpos = data.lastIndexOf('\"', pos - 1);

        ui->ownerEdit->setText(data.mid(lastpos + 1, pos - 1 - lastpos));
        dmpFile.close();
    }
}

void ImpFileSelectPage::on_fileButton_clicked()
{
    int location = ui->locationBox->currentIndex();

    if (location == SameServer || location == ImpdpServer || location == OtherServer)
    {
        FileListDialog dlg(ui->pathEdit->text(), this);
        if (dlg.exec() == QDialog::Accepted)
        {
            ui->dumpEdit->setText(dlg.fileName());

            PumpOwner();
        }
    }
}

bool ImpFileSelectPage::validatePage()
{
    return true;
}

bool ImpFileSelectPage::isComplete() const
{
    if (ui->pathEdit->text().isEmpty() ||
            ui->dumpEdit->text().isEmpty() ||
            ui->ownerEdit->text().isEmpty() ||
            ui->importEdit->text().isEmpty())
        return false;

    return true;
}
