#include "expparampage.h"
#include "ui_expparampage.h"
#include "dumptoolwizard.h"
#include "tnsnames.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include "connectioninfo.h"
#include "fmtcore.h"

ExpParamPage::ExpParamPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ExpParamPage)
{
    ui->setupUi(this);
    setTitle(tr("Параметры пользователя"));

    fakeDirEdit = new QLineEdit();
    m_pDsnModel = new OdbcTableModel(this);
    m_DataSourceDelegate = new OdbcTableModelDelegate(this);
    ui->serviceBox->setModel(m_pDsnModel);
    ui->serviceBox->setItemDelegate(m_DataSourceDelegate);

    registerField("ExportService", ui->serviceBox, "currentText", SIGNAL(currentTextChanged(QString)));
    registerField("ExportFrom", ui->systemEdit, "text", SIGNAL(textChanged(QString)));
    registerField("ExportPass", ui->syspswdEdit, "text", SIGNAL(textChanged(QString)));
    registerField("ExportPath", fakeDirEdit, "text", SIGNAL(textChanged(QString)));

    connect(ui->systemEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->syspswdEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->serviceBox, SIGNAL(currentTextChanged(QString)), this, SIGNAL(completeChanged()));

    connect(ui->copyButton, &QToolButton::clicked, [=]() -> void
    {
        ui->syspswdEdit->setText(ui->systemEdit->text());
    });
}

ExpParamPage::~ExpParamPage()
{
    delete ui;
    delete fakeDirEdit;
}

int ExpParamPage::nextId() const
{
    return DumpToolWizard::eSummaryPage;
}

bool ExpParamPage::isComplete() const
{
    bool hr = !ui->systemEdit->text().isEmpty() && !ui->syspswdEdit->text().isEmpty()
            && !ui->serviceBox->currentText().isEmpty();

    return hr;
}

bool ExpParamPage::validatePage()
{
    DumpToolWizard *wzrd = qobject_cast<DumpToolWizard*>(wizard());
    QScopedPointer<ConnectionInfo> connection(new ConnectionInfo());

    QString error;
    if (connection->open(QRSD_DRIVER, ui->systemEdit->text(), ui->syspswdEdit->text(), ui->serviceBox->currentText(), &error))
    {
        QSqlQuery query("select directory_name, directory_path from dba_directories where directory_name in ('EXPDP_DIR1', 'IMPDP_DIR1')", connection->db());

        if (!ExecuteQuery(&query))
        {
            QString OraHome = m_pDsnModel->data(m_pDsnModel->index(ui->serviceBox->currentIndex(), OdbcTableModel::fld_OraHome)).toString();
            wzrd->addUserField("OraHome", OraHome);

            TnsNames names(OraHome);
            QString service = field("ExportService").toString();
            QString host = names.element(service)._Host;

            while (query.next())
            {
                QString directory_name = query.value("directory_name").toString();
                QString directory_path = query.value("directory_path").toString().mid(2);

                QString dir = QString("\\\\%1%2").arg(host, directory_path);
                wzrd->addUserField(directory_name, dir);

                if (directory_name.contains("EXPDP_DIR1"))
                    setField("ExportPath", dir);
            }

            connection->close();

            return true;
        }
    }

    return false;
}
