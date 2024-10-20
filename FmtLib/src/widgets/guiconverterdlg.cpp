#include "guiconverterdlg.h"
#include "ui_guiconverterdlg.h"
#include "recentconnectionlist.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include <QSettings>

GuiConverterDlg::GuiConverterDlg(QSettings *settings, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GuiConverterDlg),
    m_pRecentListFilter(new FilterRecentConnectionList()),
    m_pRecentList(new RecentConnectionList()),
    m_DataSources(new OdbcTableModel()),
    m_m_DataSourceDelegate(new OdbcTableModelDelegate())
{
    ui->setupUi(this);

    m_pRecentListFilter->setAcceptOracle(false);
    m_pRecentListFilter->setSourceModel(m_pRecentList.data());
    ui->connectionsTree->setModel(m_pRecentListFilter.data());
    ui->comboBox_Dsn->setModel(m_DataSources.data());
    ui->comboBox_Dsn->setItemDelegate(m_m_DataSourceDelegate.data());

    m_pRecentList->load();

    settings->beginGroup("GuiConverter");
    ui->userNs->setText(settings->value("userNs", "users").toString());
    ui->indxNs->setText(settings->value("indxNs", "indx").toString());
    ui->sysScheme->setText(settings->value("sysScheme", "postgres").toString());
    ui->sysPwd->setText(settings->value("sysPwd", "system").toString());
    settings->endGroup();
}

GuiConverterDlg::~GuiConverterDlg()
{
    delete ui;
}

void GuiConverterDlg::on_connectionsTree_clicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QModelIndex idxname = m_pRecentList->index(index.row(), 0);
    QModelIndex idxdsn = m_pRecentList->index(index.row(), 1);

    ui->NameEdit->setText(m_pRecentList->data(idxname).toString());
    ui->PswdEdit->setText(m_pRecentList->data(idxname, PswdRole).toString());

    int found = ui->comboBox_Dsn->findText(m_pRecentList->data(idxdsn).toString());

    if (found >= 0)
        ui->comboBox_Dsn->setCurrentIndex(found);
}

