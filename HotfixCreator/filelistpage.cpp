#include "filelistpage.h"
#include "ui_filelistpage.h"
#include "hotfixwizard.h"

FileListPage::FileListPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::FileListPage)
{
    QWizard *pWizard = qobject_cast<QWizard*>(parent);
    ui->setupUi(this);

    setSubTitle(tr("Измененные файлы"));

    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &QWizardPage::completeChanged);
    connect(pWizard, &QWizard::currentIdChanged, this, &FileListPage::currentIdChanged);
}

FileListPage::~FileListPage()
{
    delete ui;
}

bool FileListPage::isComplete() const
{
    return !ui->plainTextEdit->toPlainText().isEmpty();
}

bool FileListPage::validatePage()
{
    HotfixWizard *wzrd = qobject_cast<HotfixWizard*>(wizard());
    wzrd->setFileListText(ui->plainTextEdit->toPlainText());
    ui->plainTextEdit->setPlainText(wzrd->fileListText());

    return QWizardPage::validatePage();
}

void FileListPage::cleanupPage()
{
    HotfixWizard *wzrd = qobject_cast<HotfixWizard*>(wizard());
    wzrd->setFileListText(ui->plainTextEdit->toPlainText());
}

void FileListPage::initializePage()
{
    HotfixWizard *wzrd = qobject_cast<HotfixWizard*>(wizard());
    ui->plainTextEdit->setPlainText(wzrd->fileListText());
}

/*int FileListPage::nextId() const
{
    HotfixWizard *wzrd = qobject_cast<HotfixWizard*>(wizard());
    wzrd->setFileListText(ui->plainTextEdit->toPlainText());

    return QWizardPage::nextId();
}*/

void FileListPage::currentIdChanged(int id)
{
    Q_UNUSED(id)
    HotfixWizard *wzrd = qobject_cast<HotfixWizard*>(wizard());
    wzrd->setFileListText(ui->plainTextEdit->toPlainText());
    ui->plainTextEdit->setPlainText(wzrd->fileListText());
}
