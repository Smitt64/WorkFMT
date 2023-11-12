#include "userconnectionpage.h"
#include "ui_userconnectionpage.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include "dumptoolwizard.h"
#include "tnsnames.h"
#include <QSqlQuery>
#include <QMessageBox>

UserConnectionPage::UserConnectionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::UserConnectionPage)
{
    ui->setupUi(this);
    m_pDsnModel = new OdbcTableModel(this);
    m_DataSourceDelegate = new OdbcTableModelDelegate(this);
    ui->serviceBox->setModel(m_pDsnModel);
    ui->serviceBox->setItemDelegate(m_DataSourceDelegate);

    setTitle(tr("Параметры пользователя"));
    registerField("UserName", ui->userEdit, "text", SIGNAL(textChanged(QString)));
    registerField("SystemName", ui->systemEdit, "text", SIGNAL(textChanged(QString)));
    registerField("SystemPass", ui->syspswdEdit, "text", SIGNAL(textChanged(QString)));
    registerField("Service", ui->serviceBox, "currentText", SIGNAL(currentTextChanged(QString)));
    registerField("DropUser", ui->dropUserBox);

    connect(ui->userEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->systemEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->syspswdEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(ui->serviceBox, SIGNAL(currentTextChanged(QString)), this, SIGNAL(completeChanged()));
}

UserConnectionPage::~UserConnectionPage()
{
    delete ui;
}

bool UserConnectionPage::validatePage()
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
            QString service = field("Service").toString();
            QString host = names.element(service)._Host;

            while (query.next())
            {
                QString directory_name = query.value("directory_name").toString();
                QString directory_path = query.value("directory_path").toString().mid(2);

                QString dir = QString("\\\\%1%2").arg(host, directory_path);
                wzrd->addUserField(directory_name, dir);
            }

            connection->close();

            return true;
        }
    }
    else
    {
        QMessageBox dlg(this);
        dlg.setWindowTitle(tr("Ошибка!"));
        dlg.setText(tr("Ошибка подключения к <b>%1@%2<\b>").arg(ui->systemEdit->text(), ui->serviceBox->currentText()));
        dlg.setDetailedText(error);
        dlg.setIcon(QMessageBox::Critical);
        dlg.exec();
    }

    return false;
}

bool UserConnectionPage::isComplete() const
{
    bool hr = !ui->userEdit->text().isEmpty() && !ui->systemEdit->text().isEmpty() && !ui->syspswdEdit->text().isEmpty()
            && !ui->serviceBox->currentText().isEmpty();

    return hr;
}
