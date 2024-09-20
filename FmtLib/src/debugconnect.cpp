#include "debugconnect.h"
#include "ui_debugconnect.h"
#include "recentconnectionlist.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include "errordlg.h"
#include <errorsmodel.h>
#include <QPushButton>

DebugConnect::DebugConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugConnect),
    m_pRecentList(new RecentConnectionList()),
    m_DataSources(new OdbcTableModel()),
    m_m_DataSourceDelegate(new OdbcTableModelDelegate())
{
    ui->setupUi(this);

    ui->connectionsTree->setModel(m_pRecentList.data());
    ui->comboBox_Dsn->setModel(m_DataSources.data());
    ui->comboBox_Dsn->setItemDelegate(m_m_DataSourceDelegate.data());

    m_pRecentList->load();

    QPushButton *pApply = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(pApply, &QPushButton::clicked, this, &DebugConnect::apply);
}

DebugConnect::~DebugConnect()
{
    delete ui;
}

void DebugConnect::on_connectionsTree_clicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QModelIndex idx = m_pRecentList->index(index.row(), 0);
    QModelIndex idxdsn = m_pRecentList->index(index.row(), 1);
    ui->toUserEdit->setText(m_pRecentList->data(idx).toString());

    int found = ui->comboBox_Dsn->findText(m_pRecentList->data(idxdsn).toString());
    ui->comboBox_Dsn->setCurrentIndex(found);
}

void DebugConnect::apply()
{
    ErrorsModel log;
    ErrorDlg dlg(ErrorDlg::ModeInformation, this);
    dlg.setErrors(&log);
    dlg.setMessage(QObject::tr("Протокол настройки схемы: "));
    dlg.exec();
}
