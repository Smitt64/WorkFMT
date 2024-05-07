#include "oraconnectionpage.h"
#include "ui_oraconnectionpage.h"
#include "dbttoolwizard.h"
#include "tnsnames.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include "connectioninfo.h"
#include "fmtcore.h"

OraConnectionPage::OraConnectionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::OraConnectionPage)
{
    ui->setupUi(this);

    setTitle(tr("Параметры пользователя"));
    m_pDsnModel = new OdbcTableModel(this);
    m_DataSourceDelegate = new OdbcTableModelDelegate(this);
    ui->serviceBox->setModel(m_pDsnModel);
    ui->serviceBox->setItemDelegate(m_DataSourceDelegate);

    registerField("Service", ui->serviceBox, "currentText", SIGNAL(currentTextChanged(QString)));
    registerField("User", ui->systemEdit, "text", SIGNAL(textChanged(QString)));
    registerField("Password", ui->syspswdEdit, "text", SIGNAL(textChanged(QString)));

    connect(ui->systemEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->syspswdEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->serviceBox, SIGNAL(currentTextChanged(QString)), this, SIGNAL(completeChanged()));

    connect(ui->copyButton, &QToolButton::clicked, [=]() -> void
    {
        ui->syspswdEdit->setText(ui->systemEdit->text());
    });
}

OraConnectionPage::~OraConnectionPage()
{
    delete ui;
}

int OraConnectionPage::nextId() const
{
    return DbtToolWizard::eSelectTablesPage;
}

bool OraConnectionPage::isComplete() const
{
    bool hr = !ui->systemEdit->text().isEmpty() && !ui->syspswdEdit->text().isEmpty()
            && !ui->serviceBox->currentText().isEmpty();

    return hr;
}

bool OraConnectionPage::validatePage()
{
    //DbtToolWizard *wzrd = qobject_cast<DbtToolWizard*>(wizard());
    QScopedPointer<ConnectionInfo> connection(new ConnectionInfo());

    QString error;
    if (connection->open(QRSD_DRIVER, ui->systemEdit->text(), ui->syspswdEdit->text(), ui->serviceBox->currentText(), "", &error))
    {
        connection->close();
        return true;
    }

    return false;
}
