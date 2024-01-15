#include "selserverdirdlg.h"
#include "ui_selserverdirdlg.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include "tnsnames.h"
#include <QSqlQuery>
#include <QDialogButtonBox>
#include <QPushButton>

SelServerDirDlg::SelServerDirDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelServerDirDlg)
{
    ui->setupUi(this);

    m_pDsnModel = new OdbcTableModel(this);
    m_DataSourceDelegate = new OdbcTableModelDelegate(this);
    ui->comboBox->setModel(m_pDsnModel);
    ui->comboBox->setItemDelegate(m_DataSourceDelegate);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

SelServerDirDlg::~SelServerDirDlg()
{
    delete ui;
}

QString SelServerDirDlg::path() const
{
    return ui->comboBox_2->currentText();
}

QString SelServerDirDlg::service() const
{
    return ui->comboBox->currentText();
}

void SelServerDirDlg::on_toolButton_clicked()
{
    QScopedPointer<ConnectionInfo> connection(new ConnectionInfo());

    QString error;
    if (connection->open(QRSD_DRIVER, ui->lineEdit->text(), ui->lineEdit_2->text(), ui->comboBox->currentText(), "", &error))
    {
        ui->comboBox_2->clear();
        QSqlQuery query("select directory_name, directory_path from dba_directories where directory_name in ('EXPDP_DIR1', 'IMPDP_DIR1')", connection->db());

        if (!ExecuteQuery(&query))
        {
            QString OraHome = m_pDsnModel->data(m_pDsnModel->index(ui->comboBox->currentIndex(), OdbcTableModel::fld_OraHome)).toString();

            TnsNames names(OraHome);
            QString service = ui->comboBox->currentText();
            QString host = names.element(service)._Host;

            while (query.next())
            {
                QString directory_path = query.value("directory_path").toString().mid(2);
                ui->comboBox_2->addItem(QString("\\\\%1%2").arg(host, directory_path));
            }
        }
        connection->close();
    }
}


void SelServerDirDlg::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

