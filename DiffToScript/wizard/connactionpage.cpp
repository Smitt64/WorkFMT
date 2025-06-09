#include "connactionpage.h"
#include "ui_connactionpage.h"
#include "recentconnectionlist.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include "connectioninfo.h"
#include <QMessageBox>

ConnactionPage::ConnactionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ConnactionPage),
    m_pRecentList(new RecentConnectionList()),
    m_DataSources(new OdbcTableModel()),
    m_m_DataSourceDelegate(new OdbcTableModelDelegate())
{
    ui->setupUi(this);
    setTitle(tr("Параметры, откуда брать структуру таблиц"));

    registerField("User", ui->userEdit);
    registerField("Password", ui->pswdEdit);
    registerField("Service", ui->serviceBox, "currentText");
    registerField("IsUnicode", ui->checkUnicode);

    ui->connectionsTree->setModel(m_pRecentList.data());
    ui->serviceBox->setModel(m_DataSources.data());
    ui->serviceBox->setItemDelegate(m_m_DataSourceDelegate.data());

    m_pRecentList->load();

    connect(ui->userEdit, &QLineEdit::textChanged, this, &ConnactionPage::completeChanged);
    connect(ui->pswdEdit, &QLineEdit::textChanged, this, &ConnactionPage::completeChanged);
    connect(ui->serviceBox, &QComboBox::currentTextChanged, this, &ConnactionPage::completeChanged);
}

ConnactionPage::~ConnactionPage()
{
    delete ui;
}

void ConnactionPage::on_connectionsTree_clicked(const QModelIndex &index)
{
    RecentList2 item = m_pRecentList->record(index);
    ui->userEdit->setText(item.user);
    ui->pswdEdit->setText(item.pass);
    ui->serviceBox->setCurrentText(item.dsn);
    ui->checkUnicode->setChecked(item.Options["RSD_UNICODE"].toBool());
}

bool ConnactionPage::isComplete() const
{
    if(ui->userEdit->text().isEmpty() || ui->pswdEdit->text().isEmpty() || ui->serviceBox->currentText().isEmpty())
        return false;

    return true;
}

bool ConnactionPage::validatePage()
{
    QString unicode;
    ConnectionInfo info;

    if (ui->checkUnicode->isChecked())
        unicode = "RSD_UNICODE";

    QString error;
    if (!info.open(QRSD_DRIVER, ui->userEdit->text(), ui->pswdEdit->text(), ui->serviceBox->currentText(), unicode, &error))
    {
        QMessageBox::critical(this, tr("Ошибка подключения"), error);
        return false;
    }

    info.close();
    return true;
}

void ConnactionPage::initializePage()
{
    ui->connectionsTree->setCurrentIndex(QModelIndex());
}
