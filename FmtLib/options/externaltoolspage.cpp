#include "externaltoolspage.h"
#include "ui_externaltoolspage.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>

ExternalToolsPage::ExternalToolsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::ExternalToolsPage)
{
    ui->setupUi(this);
}

ExternalToolsPage::~ExternalToolsPage()
{
    delete ui;
}

void ExternalToolsPage::on_toolButton_clicked()
{
    QString guiconverter = QFileDialog::getOpenFileName(this,
                                                        tr("Выбор расположения GuiConverter"),
                                                        QString(),
                                                        tr("GuiConverter (GuiConverter.exe)"));

    if (guiconverter.isEmpty())
        return;

    QFileInfo fi(guiconverter);
    ui->GuiConverterEdit->setText(fi.absolutePath());
}

int ExternalToolsPage::save()
{
    QSettings *setting = settings();

    setting->beginGroup("GuiConverter");
    setting->setValue("path", ui->GuiConverterEdit->text());
    setting->setValue("userNs", ui->userNs->text());
    setting->setValue("indxNs", ui->indxNs->text());
    setting->setValue("sysScheme", ui->sysScheme->text());
    setting->setValue("sysPwd", ui->sysPwd->text());
    setting->endGroup();

    return 0;
}

void ExternalToolsPage::restore()
{
    QSettings *setting = settings();

    setting->beginGroup("GuiConverter");
    ui->GuiConverterEdit->setText(setting->value("path").toString());
    ui->userNs->setText(setting->value("userNs", "users").toString());
    ui->indxNs->setText(setting->value("indxNs", "indx").toString());
    ui->sysScheme->setText(setting->value("sysScheme", "postgres").toString());
    ui->sysPwd->setText(setting->value("sysPwd", "system").toString());
    setting->endGroup();
}
