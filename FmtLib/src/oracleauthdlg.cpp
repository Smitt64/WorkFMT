#include "oracleauthdlg.h"
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include <QScopedPointer>
#include "connectioninfo.h"
#include "oracletnslistmodel.h"
#include "fmtcore.h"
#include "fmtapplication.h"
#include "loggingcategories.h"

QDataStream &operator <<(QDataStream &stream, const RecentList &i)
{
    stream << i.database
           << i.host
           << i.pass
           << i.port
           << i.service
           << i.user
           << i.dsn;

    return stream;
}

QDataStream &operator >>(QDataStream &stream, RecentList &i)
{
    stream  >> i.database
            >> i.host
            >> i.pass
            >> i.port
            >> i.service
            >> i.user
            >> i.dsn;

    return stream;
}

bool operator ==(RecentList &list, const RecentList &other)
{
    return (list.database == other.database &&
            list.host == other.host &&
            list.pass == other.pass &&
            list.port == other.port &&
            list.service == other.service &&
            list.user == other.user);
}

const QString sOracleDatabaseString ="(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=@host@)(PORT=@port@)))(CONNECT_DATA=(SERVER=DEDICATED)(SERVICE_NAME=@service@)))";

OracleAuthDlg::OracleAuthDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

    QByteArray oradir = qgetenv("");
    sConfigOraFilePath = ConfigOraFilePath();

    model = ((FmtApplication*)qApp)->getOracleTnsModel();

    ui.comboBox->setModel(model);
    ui.comboBox->setCurrentIndex(-1);

    LoadPrefs();
    qDebug() << sConfigOraFilePath;
}

OracleAuthDlg::~OracleAuthDlg()
{

}

QSqlDatabase OracleAuthDlg::OraCreateConnection(const QString &username,
                                                const QString &passw,
                                                const QString &DSN,
                                                ConnectionInfo **info)
{
    OracleTnsListModel *pModel = ((FmtApplication*)qApp)->getOracleTnsModel();
    quint32 index = pModel->getIndexByDsn(DSN);
    QString host = pModel->index(index, OracleTnsListModel::mtns_Host).data().toString();
    QString service = pModel->index(index, OracleTnsListModel::mtns_ServiceName).data().toString();
    int port = pModel->index(index, OracleTnsListModel::mtns_Port).data().toInt();

    return OraCreateConnection(username, passw, host, service, username, DSN, port, info);
}

QSqlDatabase OracleAuthDlg::OraCreateConnection(const QString &username,
                                                const QString &passw,
                                                const QString &host,
                                                const QString &service,
                                                const QString &sheme,
                                                const QString &DSN,
                                                const int &port,
                                                ConnectionInfo **info)
{
    QString _connectionname = QString("%1@%2#%3").arg(username, service, QDateTime::currentDateTime().toString(Qt::RFC2822Date));
    QSqlDatabase db =
#ifndef HELPER_SQLITE
            QSqlDatabase::addDatabase("QOCI", _connectionname);
#else
            QSqlDatabase::addDatabase("QSQLITE", _connectionname);
#endif

#ifndef HELPER_SQLITE
    QString sOracleDBString = sOracleDatabaseString;
    sOracleDBString.replace("@host@", host);
    sOracleDBString.replace("@port@", QString::number(port));
    sOracleDBString.replace("@service@", service);

    db.setUserName(username);
    db.setPassword(passw);

    db.setDatabaseName(sOracleDBString);
#else
    db.setDatabaseName("./fmt_emulator.sqlite");
#endif

    if (info)
    {
        *info = new ConnectionInfo(_connectionname);
        (*info)->m_Host = host;
        (*info)->m_Service = service;
        (*info)->m_SchemeName = sheme;
        (*info)->m_User = username;
        (*info)->m_Password = passw;
        (*info)->m_DSN = DSN;
        (*info)->m_Port = port;

        (*info)->_db = db;
    }

    if (db.open())
    {
        qCInfo(logCore()) << "Connected to " << sOracleDBString << "";
    }
    else
        qCInfo(logCore()) << "" << sOracleDBString << ": " << db.lastError().text();

    return db;
}

bool OracleAuthDlg::authirizate()
{
    QSqlDatabase db = OraCreateConnection(ui.lineEdit_username->text(),
                                          ui.lineEdit_passw->text(),
                                          ui.lineEdit_host->text(),
                                          ui.lineEdit_service->text(),
                                          ui.lineEdit_base->text(),
                                          ui.comboBox->currentText(),
                                          ui.spinBox->value(),
                                          NULL);
    connectionname = db.connectionName();

    if(db.lastError().isValid())
    {
        QMessageBox::critical(this, tr("Не удалось подключиться"), db.lastError().text());
    }
    else
    {
        sheme = ui.lineEdit_base->text();
    }
    return db.isOpen();
}

bool OracleAuthDlg::loadRecentList(QStringList &last, QList<RecentList> &recent)
{
    bool result = false;
    QFile file(ConfigOraFilePath());
    if((result = file.open(QIODevice::ReadOnly)))
    {
        QDataStream stream(&file);
        stream >> last;
        stream >> recent;
        file.close();
    }

    return result;
}

void OracleAuthDlg::LoadPrefs()
{
    QStringList lst;
    if(loadRecentList(lst, recentList))
    {
        if(lst.count() >= 6)
        {
			QString sDatabase = lst.at(2);
			QString sHostname = lst.at(0);
			QString sPort = lst.at(1);
			QString sUsername = lst.at(3);
			QString sService = lst.at(4);
			
			ui.lineEdit_base->setText(sDatabase);
			ui.lineEdit_host->setText(sHostname);
			ui.lineEdit_username->setText(sUsername);
			ui.lineEdit_service->setText(sService);
			ui.spinBox->setValue(sPort.toInt());
            ui.comboBox->setCurrentText(lst.at(5));

            if(lst.count() == 7)
            {
                ui.lineEdit_passw->setText( lst.at(6) );
				ui.checkBox_savePass->setChecked(true);
			}
		}

        foreach (RecentList recent, recentList)
        {
            int row = ui.tableWidget->rowCount();
            ui.tableWidget->setRowCount(row + 1);

            QString toolTop = OraRecentConnectionToolTip(recent);

            QTableWidgetItem *userItem = new QTableWidgetItem(recent.user);
            QTableWidgetItem *serviceItem = new QTableWidgetItem(recent.service);
            userItem->setToolTip(toolTop);
            serviceItem->setToolTip(toolTop);

            ui.tableWidget->setItem(row, 0, userItem);
            ui.tableWidget->setItem(row, 1, serviceItem);
        }
	}
}

QString OracleAuthDlg::OraRecentConnectionToolTip(const RecentList &item)
{
    QString toolTop = QString("<b>DSN: </b>%6<br><b>База данных: </b>%1<br><b>Хост: </b>%2<br><b>Порт: </b>%3<br><b>Пользователь: </b>%4<br><b>Сервис: </b>%5<br>")
            .arg(item.database, item.host)
            .arg(item.port)
            .arg(item.user, item.service)
            .arg(item.dsn);
    return toolTop;
}

QString OracleAuthDlg::OraMakeConnectionName(const RecentList &item)
{
    return QString("%1@%2")
            .arg(item.database, item.service);
}

void OracleAuthDlg::PumpPrefs(const int &id)
{
    RecentList list = recentList[id];
    QString sDatabase = list.database;
    QString sHostname = list.host;
    qint16 sPort = list.port;
    QString sUsername = list.user;
    QString sService = list.service;

    ui.lineEdit_base->setText(sDatabase);
    ui.lineEdit_host->setText(sHostname);
    ui.lineEdit_username->setText(sUsername);
    ui.lineEdit_service->setText(sService);
    ui.spinBox->setValue(sPort);
    ui.comboBox->setCurrentText(list.dsn);

    if(!list.pass.isEmpty())
    {
        ui.lineEdit_passw->setText(list.pass);
    }
}

void OracleAuthDlg::SavePrefs(bool Append)
{
	QFile f(sConfigOraFilePath);
    if(f.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&f);
		QStringList lstValues;
		lstValues << ui.lineEdit_host->text() 
			<< QString::number(ui.spinBox->value())
			<< ui.lineEdit_base->text()
            << ui.lineEdit_username->text()
            << ui.lineEdit_service->text()
            << ui.comboBox->currentText();

        if(ui.checkBox_savePass->isChecked())
            lstValues << ui.lineEdit_passw->text();

        RecentList list;
        list.database = ui.lineEdit_base->text();
        list.host = ui.lineEdit_host->text();
        list.pass = ui.lineEdit_passw->text();
        list.port = ui.spinBox->value();
        list.service = ui.lineEdit_service->text();
        list.user = ui.lineEdit_username->text();
        list.dsn = ui.comboBox->currentText();

        if (!recentList.contains(list))
        {
            recentList.append(list);
        }

        stream << lstValues;
        stream << recentList;
		f.close();
    }
    else
    {
		QMessageBox::information(this, "", tr("Failed to open config file!") );
	}
}

void OracleAuthDlg::on_pushButton_accept_clicked()
{
    if(authirizate())
    {
		SavePrefs();
		accept();
	}
}

void OracleAuthDlg::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui.lineEdit_base->setText(recentList[index.row()].database);
    ui.lineEdit_host->setText(recentList[index.row()].host);
    ui.lineEdit_username->setText(recentList[index.row()].user);
    ui.lineEdit_service->setText(recentList[index.row()].service);
    ui.spinBox->setValue(recentList[index.row()].port);
    ui.comboBox->setCurrentText(recentList[index.row()].dsn);

    if(ui.checkBox_savePass->isChecked())
    {
        ui.lineEdit_passw->setText(recentList[index.row()].pass);
    }

    on_pushButton_accept_clicked();
}

void OracleAuthDlg::on_comboBox_currentIndexChanged(int index)
{
    ui.lineEdit_host->setText(model->index(index, OracleTnsListModel::mtns_Host).data().toString());
    ui.lineEdit_service->setText(model->index(index, OracleTnsListModel::mtns_ServiceName).data().toString());
    ui.spinBox->setValue(model->index(index, OracleTnsListModel::mtns_Port).data().toInt());
}

QString OracleAuthDlg::getConnectionSheme()
{
    return sheme;
}

void OracleAuthDlg::on_delRecentButton_clicked()
{
    QSettings *s = settings();

    s->beginGroup("MessageBox");
    bool ShowDelMsg = s->value("ShowDeleteRecentConnection", true).toBool();
    bool DeleteFlag = true;
    if (ShowDelMsg)
    {
        if (ui.tableWidget->currentRow() < 0)
            DeleteFlag = false;

        if (DeleteFlag)
        {
            QCheckBox *check = new QCheckBox(tr("Больше не показывать"));
            RecentList recent = recentList.at(ui.tableWidget->currentRow());
            QMessageBox msg(this);
            msg.setWindowTitle(tr("Подтверждение удаления."));
            msg.setText(tr("Удалить соеденение <b>%1@%2</b> из закладок?").arg(recent.user, recent.service));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setCheckBox(check);
            if (msg.exec() == QMessageBox::No)
            {
                DeleteFlag = false;
            }
            s->setValue("ShowDeleteRecentConnection", !check->isChecked());
        }
    }
    s->endGroup();

    if (DeleteFlag)
    {
        recentList.removeAt(ui.tableWidget->currentRow());
        ui.tableWidget->removeRow(ui.tableWidget->currentRow());

        if(ui.tableWidget->rowCount())
            PumpPrefs(ui.tableWidget->rowCount() - 1);
        SavePrefs();
    }
}

ConnectionInfo *OracleAuthDlg::createConnectionInfo()
{
    ConnectionInfo *info = new ConnectionInfo(connectionname);
    info->m_Host = ui.lineEdit_host->text();
    info->m_Service = ui.lineEdit_service->text();
    info->m_SchemeName = sheme;
    info->m_User = ui.lineEdit_username->text();
    info->m_Password = ui.lineEdit_passw->text();
    info->m_DSN = ui.comboBox->currentText();
    info->m_Port = ui.spinBox->value();

    return info;
}
