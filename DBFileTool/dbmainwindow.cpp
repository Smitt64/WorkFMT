#include "dbmainwindow.h"
#include "ui_dbmainwindow.h"
#include <fmttablesmodel.h>
#include <oracletnslistmodel.h>
#include <tablesgroupprovider.h>
#include <oracleauthdlg.h>
#include <fmtcore.h>
#include <QFileDialog>
#include <QProgressDialog>
#include <QInputDialog>

DbMainWindow::DbMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DbMainWindow())
{
    loadTranslators();
    pObj = new DBFileObject(this);
    ui->setupUi(this);

    pTextLog = new QPlainTextEdit(this);
    logColor = new LogHighlighter(pTextLog->document());
    pTextLog->setReadOnly(true);
    ui->tabWidget->addTab(pTextLog, tr("Сообщения"));

    ui->toolButton->setIcon(QIcon("://img/openfolderHS.png"));

    pTnsModel = new OracleTnsListModel(this);
    ui->serviceBox->setModel(pTnsModel);

    ui->listView->setModel(&dbtModel);

    ui->userEdit->setText(pObj->lastUserName());
    ui->passEdit->setText(pObj->lastPassword());
    ui->serviceBox->setCurrentText(pObj->lastService());
    ui->dirEdit->setText(pObj->exportDir());

    ui->actionUnload->setShortcut(QKeySequence(Qt::Key_F2));
    ui->actionListAdd->setShortcut(QKeySequence(Qt::Key_Enter));

    ui->toolBar->addAction(ui->actionUnload);
    ui->toolBar->addAction(ui->actionLoad);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionListAdd);
    ui->toolBar->addAction(ui->actionListDelete);
    ui->toolBar->addAction(ui->actionListClear);

    pConnectionsToolBar = Q_NULLPTR;
    m_ConnectionsGroup = Q_NULLPTR;
    pAddMenu = new QMenu(tr("Добавить"), this);
    ui->addBtn->setMenu(pAddMenu);
    pAddTablesGroup = pAddMenu->addAction(tr("Добавить таблицы из группы"));

    pCompleter = new QCompleter(this);
    pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit->setCompleter(pCompleter);

    //
    ui->openConnection->setVisible(false);
    //

    connect(ui->unloadBtn, SIGNAL(clicked(bool)), SLOT(unload()));
    connect(ui->actionUnload, SIGNAL(triggered(bool)), SLOT(unload()));
    connect(ui->addBtn, SIGNAL(clicked(bool)), SLOT(addTable()));
    connect(ui->actionListAdd, SIGNAL(triggered(bool)), SLOT(addTable()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), SLOT(addTable()));
    connect(ui->toolButton, SIGNAL(clicked(bool)), SLOT(exportDir()));
    connect(ui->actionListClear, SIGNAL(triggered(bool)), SLOT(clear()));
    connect(ui->actionListDelete, SIGNAL(triggered(bool)), SLOT(remove()));
    connect(ui->listView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(listContextMenu(QPoint)));
    connect(pObj, SIGNAL(procMessage(QString)), pTextLog, SLOT(appendPlainText(QString)));
    connect(ui->actionLoad, SIGNAL(triggered(bool)), SLOT(importTables()));
    connect(pAddTablesGroup, SIGNAL(triggered(bool)), SLOT(addTablesGroup()));
    connect(ui->openConnection, SIGNAL(clicked(bool)), SLOT(openConnection()));
    connect(ui->actionSaveList, SIGNAL(triggered(bool)), SLOT(saveList()));
    connect(ui->actionLoadList, SIGNAL(triggered(bool)), SLOT(loadList()));
}

DbMainWindow::~DbMainWindow()
{
    delete ui;
}

void DbMainWindow::loadTranslators()
{
    QDir trDir(qApp->applicationDirPath());
    if (trDir.cd("translations"))
    {
        QString translatorFile = QString("qt_%1").arg("ru");
        if (qt_translator.load(translatorFile, trDir.absolutePath()))
            qApp->installTranslator(&qt_translator);
    }
}

void DbMainWindow::addTable()
{
    QString dbt = ui->lineEdit->text().simplified();

    if (dbt.isEmpty())
        return;

    if (dbtModel.match(dbtModel.index(0, 0), Qt::DisplayRole, dbt).size())
        return;

    int row = dbtModel.rowCount();
    dbtModel.insertRow(row);
    dbtModel.setData(dbtModel.index(row, 0), dbt, Qt::DisplayRole);

    ui->lineEdit->setText("");
}

void DbMainWindow::exportDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, QString(), ui->dirEdit->text());

    if (dir.isEmpty())
        return;

    ui->dirEdit->setText(dir);
    pObj->setExportDir(dir);
}

void DbMainWindow::clear()
{
    dbtModel.setStringList(QStringList());
}

void DbMainWindow::remove()
{
    //QStringList lst =
    ui->listView->setUpdatesEnabled(false);
    QModelIndexList indexes = ui->listView->selectionModel()->selectedIndexes();
    qSort(indexes.begin(), indexes.end());

    for(int i = indexes.count() - 1; i > -1; --i)
        dbtModel.removeRow(indexes.at(i).row());

    ui->listView->setUpdatesEnabled(true);
}

void DbMainWindow::listContextMenu(const QPoint &pos)
{
    QPoint item = ui->listView->mapToGlobal(pos);
    QMenu menu;
    menu.addAction(ui->actionListAdd);
    menu.addAction(ui->actionListDelete);
    menu.addAction(ui->actionListClear);
    menu.exec(item);
}

void DbMainWindow::importTables()
{
    QStringList files = QFileDialog::getOpenFileNames(this, QString(), ui->dirEdit->text());

    int i = 0;
    if (!files.isEmpty())
    {
        pObj->setLastUserName(ui->userEdit->text());
        pObj->setLastPassword(ui->passEdit->text());
        pObj->setLastService(ui->serviceBox->currentText());
        pTextLog->clear();
        ui->tabWidget->setCurrentIndex(1);

        QProgressDialog dlg(tr("Выгрузка таблицы"), tr("Прервать"), 0, files.size(), this);
        connect(&dlg, SIGNAL(canceled()), pObj, SLOT(stop()));
        dlg.open();
        foreach (const QString &str, files) {
            if (dlg.wasCanceled())
                break;
            else
            {
                dlg.setValue(i);
                dlg.setLabelText(tr("Загрузка файла %1, %2 из %3")
                                 .arg(str)
                                 .arg(++i)
                                 .arg(files.size()));

                pObj->load(ui->userEdit->text(),
                             ui->passEdit->text(),
                             ui->serviceBox->currentText(),
                             str);
            }
        }
        dlg.close();
    }
}

void DbMainWindow::unload()
{
    if (!dbtModel.rowCount())
        return;

    int i = 0;
    pObj->setLastUserName(ui->userEdit->text());
    pObj->setLastPassword(ui->passEdit->text());
    pObj->setLastService(ui->serviceBox->currentText());
    pTextLog->clear();
    ui->tabWidget->setCurrentIndex(1);

    QStringList lst = dbtModel.stringList();
    QProgressDialog dlg(tr("Выгрузка таблицы"), tr("Прервать"), 0, lst.size(), this);
    connect(&dlg, SIGNAL(canceled()), pObj, SLOT(stop()));
    dlg.open();
    foreach (const QString &str, lst) {
        if (dlg.wasCanceled())
            break;
        else
        {
            dlg.setValue(i);
            dlg.setLabelText(tr("Выгрузка таблицы %1, %2 из %3")
                             .arg(str)
                             .arg(++i)
                             .arg(lst.size()));

            pObj->unload(ui->userEdit->text(),
                         ui->passEdit->text(),
                         ui->serviceBox->currentText(),
                         ui->dirEdit->text(), str);
        }
    }
    dlg.close();
}

void DbMainWindow::addTablesGroup()
{
    bool ok = false;
    QStringList groups = TablesGroupProvider::instance()->getGroupsList();
    QString name = QInputDialog::getItem(this, tr("Список групп"), tr("Добавить таблицы в список из: "), groups, 0, false, &ok);

    if (ok)
    {
        int index = groups.indexOf(name);
        QSharedPointer<QSqlQuery> query = TablesGroupProvider::instance()->getQueryForSeletGroup(index);

        if (query->exec())
        {
            while (query->next())
            {
                int row = dbtModel.rowCount();
                dbtModel.insertRow(row);
                dbtModel.setData(dbtModel.index(row, 0), query->value(1).toString(), Qt::DisplayRole);
            }
        }
    }
}

QAction *DbMainWindow::CreateConnectionAction(const QString &ShemeName, ConnectionInfo *info)
{
    if (m_ConnectionsGroup == Q_NULLPTR)
        m_ConnectionsGroup = new QActionGroup(this);

    if (pConnectionsToolBar == Q_NULLPTR)
    {
        pConnectionsToolBar = addToolBar(tr("Подключения"));
        pConnectionsToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        pConnectionsToolBar->setIconSize(QSize(16, 16));
    }

    QAction *a = new QAction(this);
    a->setText(ShemeName);
    a->setCheckable(true);
    pConnectionsToolBar->addAction(a);
    m_ConnectionsGroup->addAction(a);
    a->setChecked(true);

    a->setIcon(info->colorIcon());
    a->setData(reinterpret_cast<int>(info));

    info->updateFmtList();
    m_pConnections.append(info);

    return a;
}

void DbMainWindow::openConnection()
{
    OracleAuthDlg dlg(pTnsModel, this);

    if (dlg.exec() == QDialog::Accepted)
    {
        ConnectionInfo *info = dlg.createConnectionInfo();
        ui->userEdit->setText(info->user());
        ui->passEdit->setText(info->password());
        ui->serviceBox->setCurrentText(info->dsn());
        CreateConnectionAction(dlg.getConnectionSheme(), info);

        pCompleter->setModel(info->tablesModel());
        pCompleter->setCompletionColumn(fnc_Name);
    }
}

void DbMainWindow::saveList()
{
    QString name = QInputDialog::getText(this, tr("Cохранить список"), tr("Наименование списка: "));

    if (name.isEmpty())
        return;

    pSettrings->beginGroup("Lists");
    pSettrings->setValue(name, dbtModel.stringList());
    pSettrings->endGroup();
    pSettrings->sync();
}

void DbMainWindow::loadList()
{
    pSettrings->beginGroup("Lists");
    QStringList lists = pSettrings->allKeys();
    pSettrings->endGroup();

    bool ok = false;
    QString name = QInputDialog::getItem(this, tr("Список сохранений"), tr("Добавить таблицы в список из: "), lists, 0, false, &ok);

    if (!ok)
        return;

    pSettrings->beginGroup("Lists");
    QStringList tables = pSettrings->value(name).toStringList();
    pSettrings->endGroup();

    foreach (const QString &str, tables) {
        int row = dbtModel.rowCount();
        dbtModel.insertRow(row);
        dbtModel.setData(dbtModel.index(row, 0), str, Qt::DisplayRole);
    }
}
