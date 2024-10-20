#include "debugconnect.h"
#include "ui_debugconnect.h"
#include "recentconnectionlist.h"
#include "odbctablemodel.h"
#include "odbctablemodeldelegate.h"
#include "errordlg.h"
#include "connectioninfo.h"
#include "loggingcategories.h"
#include <errorsmodel.h>
#include <QPushButton>
#include <QNetworkInterface>

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

    QList<QHostAddress> hosts = QNetworkInterface::allAddresses();
    for(int nIter = 0; nIter < hosts.count(); nIter++)
    {
        if(!hosts[nIter].isLoopback())
        {
            if (hosts[nIter].protocol() == QAbstractSocket::IPv4Protocol)
                ui->hostsBox->addItem(hosts[nIter].toString());
        }
    }

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

    if (found >= 0)
        ui->comboBox_Dsn->setCurrentIndex(found);
}

void DebugConnect::execQuery(ConnectionInfo *system,
                             ErrorsModel *log,
                             const QString &query,
                             const QString &templ,
                             const QString &beginmsg,
                             const QString &success)
{
    qInfo(logCore()) << beginmsg;

    QString err;
    QSqlQuery q1(system->db());
    q1.prepare(query);

    if (ExecuteQuery(&q1, &err))
    {
        qCritical(logCore()) << "Can't grant:" << err;
        log->addError(templ.arg(query) + tr(": Ошибка"));
        log->addError(err);
    }
    else
    {
        qInfo(logCore()) << "Ggrant debug any procedure success";
        log->addMessage(templ.arg(query) + tr(": Успешно"));
    }
}

void DebugConnect::apply()
{
    ErrorsModel log;
    QScopedPointer<ConnectionInfo> system(new ConnectionInfo());

    qInfo(logCore()) << "Begin apply debug grants...";
    if (system->open(QRSD_DRIVER, ui->sysNameEdit->text(),
                 ui->sysPswdEdit->text(),
                 ui->comboBox_Dsn->currentText()))
    {
        if (system->type() != ConnectionInfo::CON_ORA)
        {
            QString msg = tr("Схема %1@%2 не является базой данный Oracle")
                    .arg(ui->sysNameEdit->text())
                    .arg(ui->comboBox_Dsn->currentText());

            qCritical(logCore()) << msg;
            log.addError(msg);
            system->close();
        }
    }
    else
    {
        QString msg = tr("Не удалось подключиться к схеме %1@%2")
                .arg(ui->sysNameEdit->text())
                .arg(ui->comboBox_Dsn->currentText());

        qCritical(logCore()) << msg;
        log.addError(msg);
    }

    if (system->isOpen())
    {
        QString templ1 = tr("Предоставление прав на отладку любой процедуры [%1]");
        QString templ2 = tr("Предоставление прав отладочного сеанса подключения [%1]");
        QString templ3 = tr("Добавление записи в список управления доступом (ACL) сетевого узла [%1]");
        QString grant1 = QString("grant debug any procedure to %1").arg(ui->toUserEdit->text());
        QString grant2 = QString("grant debug connect session to %1").arg(ui->toUserEdit->text());

        execQuery(system.data(), &log, grant1, templ1,
                  "Begin grant debug connect session...",
                  "Ggrant debug any procedure success");

        execQuery(system.data(), &log, grant2, templ2,
                  "Begin grant debug any procedure...",
                  "Ggrant debug debug connect session success");


        QString query_template = ReadTextFileContent(":/append_host_ace");
        QString query = query_template.arg(ui->hostsBox->currentText()).arg(ui->toUserEdit->text());
        execQuery(system.data(), &log, query, templ3,
                  "Begin appends an access control entry (ACE) to the access control list (ACL) of a network host...",
                  "Appends an access control entry (ACE) to the access control list (ACL) of a network host success");
    }

    ErrorDlg dlg(ErrorDlg::ModeInformation, this);
    dlg.setErrors(&log);
    dlg.setMessage(QObject::tr("Протокол настройки схемы: "));
    dlg.exec();
}
