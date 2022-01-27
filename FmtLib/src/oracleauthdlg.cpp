#include "oracleauthdlg.h"
#include "ui_oracleauthdlg.h"
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include <QScopedPointer>
#include <QtSql>
#include "connectioninfo.h"
#include "oracletnslistmodel.h"
#include "fmtcore.h"
#include "fmtapplication.h"
#include "loggingcategories.h"
#include "recentconnectionlist.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include <QTreeView>
#include <QCheckBox>

OracleAuthDlg::OracleAuthDlg(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::OracleAuthDlg()),
      m_pRecentList(new RecentConnectionList()),
      m_DataSources(new OdbcTableModel()),
      m_m_DataSourceDelegate(new OdbcTableModelDelegate()),
      pTmpInfo(nullptr)
{
    ui->setupUi(this);

    QByteArray oradir = qgetenv("");
    ui->connectionsTree->setModel(m_pRecentList.data());
    ui->comboBox_Dsn->setModel(m_DataSources.data());
    ui->comboBox_Dsn->setItemDelegate(m_m_DataSourceDelegate.data());

    LoadPrefs();
}

OracleAuthDlg::OracleAuthDlg(OracleTnsListModel *tnsmodel, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::OracleAuthDlg()),
      m_m_DataSourceDelegate(new OdbcTableModelDelegate()),
      pTmpInfo(nullptr)
{
    ui->setupUi(this);
    ui->connectionsTree->setModel(m_pRecentList.data());
    ui->comboBox_Dsn->setModel(m_DataSources.data());

    LoadPrefs();
}

OracleAuthDlg::~OracleAuthDlg()
{
    delete ui;
}

bool OracleAuthDlg::authirizate()
{
    bool hr = true;
    if (pTmpInfo)
        delete pTmpInfo;

    pTmpInfo = new ConnectionInfo();

    QString user = ui->lineEdit_username->text();
    QString pass = ui->lineEdit_passw->text();
    QString dsn = ui->comboBox_Dsn->currentText();

    hr = tryConnect(pTmpInfo, user, pass, dsn, this);

    if (hr)
        m_pRecentList->append(user, pass, dsn);
    else
    {
        delete pTmpInfo;
        pTmpInfo = nullptr;
    }

    return hr;
}

bool OracleAuthDlg::tryConnect(ConnectionInfo *pTmpInfo,
                           const QString &user,
                           const QString &pass,
                           const QString &dsn,
                           QWidget *parent)
{
    bool hr = true;
    QString error;

    if (user.isEmpty())
    {
        hr = false;
        error = tr("Не указано имя пользователя");
    }

    if (hr && pass.isEmpty())
    {
        hr = false;
        error = tr("Не указан пароль");
    }

    if (hr && dsn.isEmpty())
    {
        hr = false;
        error = tr("Не указана база данных");
    }

    if (hr)
    {
        hr = pTmpInfo->open(QRSD_DRIVER, user, pass, dsn);

        if (!hr)
        {
            error = pTmpInfo->db().lastError().text();

            if (error.contains("Driver not loaded"))
                error = tr("Не удалось загрузить драйвер RSD");
        }
    }

    if (!hr)
    {
        QMessageBox msgBox(parent);
        msgBox.setText(tr("Не удалось соедениться с базой данных:"));
        msgBox.setWindowTitle(tr("Ошибка"));
        msgBox.setInformativeText(error);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    return hr;
}

void OracleAuthDlg::LoadPrefs()
{
    m_pRecentList->load();
}

void OracleAuthDlg::SavePrefs(bool Append)
{
    Q_UNUSED(Append)
    m_pRecentList->save();
}

void OracleAuthDlg::acceptClicked()
{
    bool result = authirizate();

    if(result)
    {
        SavePrefs();
		accept();
	}
}

void OracleAuthDlg::on_connectionsTree_clicked(const QModelIndex &index)
{
    RecentList rec = m_pRecentList->record(index);

    ui->lineEdit_username->setText(rec.user);
    ui->lineEdit_passw->setText(rec.pass);
    ui->comboBox_Dsn->setCurrentText(rec.dsn);
}

void OracleAuthDlg::on_connectionsTree_doubleClicked(const QModelIndex &index)
{
    acceptClicked();
}

void OracleAuthDlg::on_delRecentButton_clicked()
{
    QSettings *s = settings();

    s->beginGroup("MessageBox");
    bool ShowDelMsg = s->value("ShowDeleteRecentConnection", true).toBool();
    bool DeleteFlag = true;

    QModelIndex index = ui->connectionsTree->currentIndex();
    if (!index.isValid())
        DeleteFlag = false;

    if (ShowDelMsg && DeleteFlag)
    {
        QCheckBox *check = new QCheckBox(tr("Больше не показывать"));
        RecentList recent = m_pRecentList->record(index);

        QMessageBox msg(this);
        msg.setWindowTitle(tr("Подтверждение удаления."));
        msg.setText(tr("Удалить соеденение <b>%1@%2</b> из закладок?").arg(recent.user, recent.dsn));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setCheckBox(check);

        if (msg.exec() == QMessageBox::No)
            DeleteFlag = false;

        s->setValue("ShowDeleteRecentConnection", !check->isChecked());
    }
    s->endGroup();

    if (DeleteFlag)
    {
        m_pRecentList->removeRow(index.row());

#ifndef _DEBUG
        SavePrefs();
#endif
    }
}

ConnectionInfo *OracleAuthDlg::getConnectionInfo()
{
    ConnectionInfo *info = pTmpInfo;
    pTmpInfo = nullptr;
    return info;
}

void OracleAuthDlg::on_pushButton_accept_clicked()
{
    acceptClicked();
}

