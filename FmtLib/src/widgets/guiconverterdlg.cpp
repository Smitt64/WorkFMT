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

    QModelIndex idxname = m_pRecentListFilter->index(index.row(), 0);
    QModelIndex idxdsn = m_pRecentListFilter->index(index.row(), 1);

    ui->NameEdit->setText(m_pRecentListFilter->data(idxname).toString());
    ui->dbName->setText(m_pRecentListFilter->data(idxname).toString());
    ui->PswdEdit->setText(m_pRecentListFilter->data(idxname, PswdRole).toString());

    int found = ui->comboBox_Dsn->findText(m_pRecentListFilter->data(idxdsn).toString(), Qt::MatchFixedString);

    if (found >= 0)
        ui->comboBox_Dsn->setCurrentIndex(found);
}


void GuiConverterDlg::on_comboBox_Dsn_currentIndexChanged(int index)
{
    QSettings params("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\ODBC\\ODBC.INI", QSettings::NativeFormat);
    params.beginGroup(ui->comboBox_Dsn->currentText());
    QString host = params.value("Servername").toString();
    int port = params.value("Port").toInt();

    ui->ipAddr->setText(QString("%1:%2").arg(host).arg(port));
    params.endGroup();
}

QString GuiConverterDlg::userNs() const
{
    return ui->userNs->text();
}

QString GuiConverterDlg::indxNs() const
{
    return ui->indxNs->text();
}

QString GuiConverterDlg::sysScheme() const
{
    return ui->sysScheme->text();
}

QString GuiConverterDlg::sysPwd() const
{
    return ui->sysPwd->text();
}

QString GuiConverterDlg::ipAddr() const
{
    return ui->ipAddr->text();
}

QString GuiConverterDlg::userScheme() const
{
    return ui->NameEdit->text();
}

QString GuiConverterDlg::userPwd() const
{
    return ui->PswdEdit->text();
}

QString GuiConverterDlg::dsn() const
{
    return ui->comboBox_Dsn->currentText();
}

QString GuiConverterDlg::dbName() const
{
    return ui->dbName->text();
}
