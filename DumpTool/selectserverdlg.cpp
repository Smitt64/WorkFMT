#include "selectserverdlg.h"
#include "ui_selectserverdlg.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include "fmtcore.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSettings>

DsnFilterModel::DsnFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

DsnFilterModel::~DsnFilterModel()
{

}

bool DsnFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    OdbcTableModel *sourcemodel = (OdbcTableModel*)sourceModel();

    if (source_row <0 || source_row >= sourcemodel->rowCount())
        return false;

    const OdbcElement &elem = sourcemodel->element(sourcemodel->index(source_row, 0));
    return elem._db == DataBaseType::PostgreSQL;
}

SelectServerDlg::SelectServerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectServerDlg)
{
    ui->setupUi(this);

    m_pDsnModel = new OdbcTableModel(this);
    m_DataSourceDelegate = new OdbcTableModelDelegate(this);

    m_pFilter = new DsnFilterModel(this);
    m_pFilter->setSourceModel(m_pDsnModel);

    ui->listView->setModel(m_pFilter);
    ui->listView->setItemDelegate(m_DataSourceDelegate);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

SelectServerDlg::~SelectServerDlg()
{
    delete ui;
}

void SelectServerDlg::on_listView_clicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QSettings node(QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\ODBC\\ODBC.INI"),
                QSettings::NativeFormat);

    node.beginGroup(index.data().toString());
    m_Host = node.value("Servername", "").toString();
    m_UserName = node.value("Username", "").toString();
    m_Port = node.value("Port", 5432).toInt();
    m_Database = node.value("Database", "").toString();
    node.endGroup();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

const QString &SelectServerDlg::getUserName() const
{
    return m_UserName;
}

const QString &SelectServerDlg::getHost() const
{
    return m_Host;
}

const quint32 &SelectServerDlg::getPort() const
{
    return m_Port;
}

const QString &SelectServerDlg::getDatabase() const
{
    return m_Database;
}

void SelectServerDlg::on_listView_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    if (ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled())
        accept();
}

