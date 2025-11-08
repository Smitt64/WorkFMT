#include "actionpage.h"
#include "toolsruntime.h"
#include "regparmwizard.h"
#include "ui_actionpage.h"
#include <QButtonGroup>
#include <QFileDialog>
#include <QSettings>
#include <selectfolderdlg.h>
#include <QSpinBox>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>

ActionPage::ActionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ActionPage),
    m_pGroup(nullptr),
    fakeBtn(nullptr)
{
    ui->setupUi(this);

    setTitle(tr("Добавление настройки в DAT файл"));

    registerField("RepositoryPath", ui->pathEdit);

    fakeBtn = new QSpinBox();
    m_pGroup = new QButtonGroup();
    m_pGroup->addButton(ui->radioAddRegToDat, 0);
    registerField("Action", fakeBtn, "value");

    // Соединения
    //connect(ui->selFolderBtn, &QPushButton::clicked, this, &ActionPage::on_selFolderBtn_clicked);
    connect(ui->pathEdit, &QLineEdit::textChanged, this, &ActionPage::on_pathEdit_textChanged);
    connect(ui->pathEdit, &QLineEdit::textChanged, this, &ActionPage::completeChanged);

    // Изначально скрываем иконку VCS
    ui->versIcon->setVisible(false);

    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked)
    {
        if (checked)
            setField("Action", id);
    });

    ui->radioAddRegToDat->setChecked(true);
}

ActionPage::~ActionPage()
{
    delete fakeBtn;
    delete m_pGroup;
    delete ui;
}

void ActionPage::initializePage()
{
    // Скрываем кнопку рестарта на первой странице
    wizard()->button(QWizard::CustomButton2)->setVisible(false);

    // Сбрасываем состояние
    ui->pathEdit->clear();
    ui->versIcon->setVisible(false);
}

bool ActionPage::isComplete() const
{
    QString path = ui->pathEdit->text().trimmed();

    // Проверяем, что путь не пустой
    if (path.isEmpty()) {
        ui->pathEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
        return false;
    }

    // Проверяем, что путь существует и это директория
    QDir dir(path);
    if (!dir.exists()) {
        ui->pathEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
        return false;
    }

    // Проверяем наличие необходимых DAT файлов
    if (!checkRequiredFiles(path)) {
        ui->pathEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
        return false;
    }

    // Если все проверки пройдены, убираем красную рамку
    ui->pathEdit->setStyleSheet("");
    return true;
}

void ActionPage::on_selFolderBtn_clicked()
{
    QSettings settings("RegParmWizard.ini", QSettings::IniFormat);
    SelectFolderDlg folderDlg(&settings, "RepoList", this);

    if (folderDlg.exec() == QDialog::Accepted)
    {
        QString selectedPath = folderDlg.selectedPath();
        ui->pathEdit->setText(selectedPath);

        // Используем существующую функцию для определения типа VCS
        VcsType type = toolDetectVcsType(selectedPath);
        if (type != VcsType::None)
        {
            ui->versIcon->setVisible(true);
            if (type == VcsType::Git)
            {
                ui->versIcon->setPixmap(QPixmap("://res/gitlogo.svg"));
                ui->versIcon->setToolTip(tr("Git репозиторий"));
            }
            else
            {
                ui->versIcon->setPixmap(QPixmap("://res/svnlogo.svg"));
                ui->versIcon->setToolTip(tr("SVN репозиторий"));
            }
        }
        else
        {
            ui->versIcon->setVisible(false);
        }

        // Проверяем наличие необходимых файлов и показываем сообщение
        if (!checkRequiredFiles(selectedPath))
        {
            QMessageBox::warning(this,
                               tr("Ошибка"),
                               tr("Не найдены необходимые файлы DREGPARM_DBT.dat и/или DREGVAL_DBT.dat в каталоге Distrib/DBFile/Data"));
        }

        emit completeChanged();
    }
}

void ActionPage::on_pathEdit_textChanged(const QString &text)
{
    // Проверяем тип VCS при ручном вводе пути
    if (!text.isEmpty() && QDir(text).exists())
    {
        VcsType type = toolDetectVcsType(text);
        if (type != VcsType::None)
        {
            ui->versIcon->setVisible(true);
            if (type == VcsType::Git)
            {
                ui->versIcon->setPixmap(QPixmap("://res/gitlogo.svg"));
                ui->versIcon->setToolTip(tr("Git репозиторий"));
            }
            else
            {
                ui->versIcon->setPixmap(QPixmap("://res/svnlogo.svg"));
                ui->versIcon->setToolTip(tr("SVN репозиторий"));
            }
        }
        else
        {
            ui->versIcon->setVisible(false);
        }
    }
    else
    {
        ui->versIcon->setVisible(false);
    }

    // При изменении текста вызываем проверку валидности
    emit completeChanged();
}

QString ActionPage::selectedPath() const
{
    return ui->pathEdit->text().trimmed();
}

bool ActionPage::checkRequiredFiles(const QString &basePath) const
{
    QDir dir(basePath);

    // Проверяем наличие файлов в подкаталоге Distrib\DBFile\Data
    QString dataPath = dir.absoluteFilePath("Distrib/DBFile/Data");
    QDir dataDir(dataPath);

    if (!dataDir.exists()) {
        return false;
    }

    QString regParmFile = dataDir.absoluteFilePath("DREGPARM_DBT.dat");
    QString regValFile = dataDir.absoluteFilePath("DREGVAL_DBT.dat");

    bool hasRegParm = QFile::exists(regParmFile);
    bool hasRegVal = QFile::exists(regValFile);

    return hasRegParm && hasRegVal;
}
