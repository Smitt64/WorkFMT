#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tablesdock.h"
#include "oracleauthdlg.h"
#include "connectioninfo.h"
#include "fmtworkwindow.h"
#include "fmttable.h"
#include "fmtcore.h"
#include "fmtfromrichtext.h"
#include "treecombobox.h"
#include "subwindowsmodel.h"
#include "impexpparams.h"
#include "fmtimpexpwrp.h"
#include "fmterrors.h"
#include "aboutdlg.h"
#include "tablesdockwidget.h"
#include "exporttoxmlwizard.h"
#include "tablesgroupprovider.h"
#include "windowslistdlg.h"
#include "tablesgroupsdlg.h"
#include "selectconnectiondlg.h"
#include "stringlistdlg.h"
#include "fmttablelistdelegate.h"
#include "massoperationwizard.h"
#include "logsettingsdlg.h"
#include <QRegExp>
#include <QRegularExpression>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("FMT"));
    setWindowIcon(QIcon(":/img/icon128.png"));
    pTablesDock = new TablesDock(tr("Таблицы"), this);
    addDockWidget(Qt::LeftDockWidgetArea, pTablesDock);
    //pTablesDock->tablesWidget()->installEventFilter(this);
    pTablesDock->setEventFilter(this);

    pTableListDelegate = new FmtTableListDelegate(this);
    pTablesDock->setItemDelegate(pTableListDelegate);

    pMdi = new QMdiArea(this);
    pMdi->setDocumentMode(true);
    pMdi->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pMdi->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(pMdi);

    ui->actionConnect->setShortcut(QKeySequence(QKeySequence::Open));
    ui->actionCreate->setShortcut(QKeySequence(QKeySequence::New));
    ui->actionCreateFromText->setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_N));

    m_ConnectionsGroup = new QActionGroup(this);
    pLogButton = new QPushButton(this);
    pLogButton->setToolTip(tr("Параметры трассы"));
    pLogButton->setIcon(QIcon(":/img/book_notebook.png"));
    pLogButton->setFlat(true);
    ui->statusBar->addPermanentWidget(pLogButton);

    CreateWindowsCombo();
    CreateMainToolBar();
    CreateWindowFunctional();
    CreateSearchToolBar();
    CreateViewMenu();

    ui->tabToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QSettings *s = settings();
    restoreGeometry(s->value("Geometry").toByteArray());
    restoreState(s->value("State").toByteArray());

    actionEdit = new QAction(QIcon(":/img/edittable.png"), tr("Редактировать"), this);
    actionExport = new QAction(QIcon(":/img/savexml.png"), tr("Экспорт в xml файл"), this);
    actionDeleteTable = new QAction(QIcon(":/img/removetable.png"), tr("Удалить запись"), this);

    connect(ui->actionConnect, SIGNAL(triggered(bool)), SLOT(actionConnectTriggered()));
    connect(ui->actionDisconnect, SIGNAL(triggered(bool)), SLOT(actionDisconnectTriggered()));
    connect(pTablesDock, SIGNAL(tableDbClicked(quint32)), SLOT(tableClicked(quint32)));
    connect(m_ConnectionsGroup, SIGNAL(triggered(QAction*)), SLOT(conActionTriggered(QAction*)));
    connect(ui->actionCreate, SIGNAL(triggered(bool)), SLOT(actionCreate()));
    connect(ui->actionRebuildOffset, SIGNAL(triggered(bool)), SLOT(rebuidOffsets()));
    connect(ui->actionCreateFromText, SIGNAL(triggered(bool)), SLOT(createFromText()));

    connect(ui->actionImpExpPrm, SIGNAL(triggered(bool)), SLOT(ImpExpSettings()));
    connect(ui->actionImportDir, SIGNAL(triggered(bool)), SLOT(ImpDirAction()));
    connect(ui->actionImport, SIGNAL(triggered(bool)), SLOT(ImportAction()));
    connect(ui->actionExport, SIGNAL(triggered(bool)), SLOT(ExportXmlAction()));

    connect(actionEdit, SIGNAL(triggered(bool)), SLOT(actionEditFmt()));
    connect(actionExport, SIGNAL(triggered(bool)), SLOT(actionExportTableXml()));
    connect(ui->actionInit, SIGNAL(triggered(bool)), SLOT(actionInit()));
    connect(pMdi, SIGNAL(subWindowActivated(QMdiSubWindow*)), SLOT(subWindowActivated(QMdiSubWindow*)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), SLOT(about()));
    connect(ui->actionSql, SIGNAL(triggered(bool)), SLOT(CreateTableSql()));
    connect(ui->actionEditGroups, SIGNAL(triggered(bool)), SLOT(EditGroups()));
    connect(ui->actionCopyTable, SIGNAL(triggered(bool)), SLOT(CopyTable()));
    connect(ui->actionCopyTableAs, SIGNAL(triggered(bool)), SLOT(CopyTableTo()));
    connect(ui->actionRsexpDir, SIGNAL(triggered(bool)), SLOT(RsExpExportDir()));
    connect(ui->actionUnloadDbf, SIGNAL(triggered(bool)), SLOT(UnloadDbf()));
    connect(ui->actionLoadDbf, SIGNAL(triggered(bool)), SLOT(LoadDbf()));
    connect(ui->action_FMT_sqlite, SIGNAL(triggered(bool)), SLOT(UnloadSqlite()));
    connect(ui->actionOpenConnection, SIGNAL(triggered(bool)), SLOT(OpenConnection()));
    connect(actionDeleteTable, SIGNAL(triggered(bool)), SLOT(RemoveFmtTable()));
    connect(pLogButton, SIGNAL(clicked(bool)), SLOT(LoggingSettings()));
    connect(ui->actionEditContent, SIGNAL(triggered(bool)), SLOT(EditContent()));
    connect(ui->actionGenCreateTbSql, SIGNAL(triggered(bool)), SLOT(GenCreateTableScript()));
    connect(ui->actionGenModifyScript, SIGNAL(triggered(bool)), SLOT(GenModifyTableFields()));
    connect(ui->actionGenAddScript, SIGNAL(triggered(bool)), SLOT(GenAddFiledsScript()));
    connect(ui->actionGenDelScript, SIGNAL(triggered(bool)), SLOT(GenDeleteFiledsScript()));
    connect(ui->actionMassOp, SIGNAL(triggered(bool)), SLOT(OnMassOpAction()));
    connect(ui->actionConfluence, SIGNAL(triggered(bool)), SLOT(OnConfluence()));

#ifdef QT_NO_DEBUG
    connect(ui->actionHotFix, SIGNAL(triggered(bool)), SLOT(HotFixCreate()));
    ui->actionHotFix->setVisible(false);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateMainToolBar()
{
    toolConnectMenu = new QMenu(this);
    toolConnect = new QToolButton(this);
    toolConnect->setPopupMode(QToolButton::MenuButtonPopup);
    toolConnect->setIcon(ui->actionConnect->icon());
    toolConnect->setText(ui->actionConnect->text());
    toolConnect->setDefaultAction(ui->actionConnect);
    toolConnect->setMenu(toolConnectMenu);

    QStringList lst;
    QList<RecentList> recent;
    OracleAuthDlg::loadRecentList(lst, recent);

    if (recent.size())
    {
        toolConnectMenu->addAction(ui->actionConnect);
        toolConnect->setDefaultAction(ui->actionConnect);
        toolConnectMenu->setDefaultAction(ui->actionConnect);
        toolConnectMenu->addSeparator();

        foreach (const RecentList &item, recent)
        {
            QAction *action = toolConnectMenu->addAction(OracleAuthDlg::OraMakeConnectionName(item));
            action->setData(QVariant::fromValue(item));
            action->setToolTip(OracleAuthDlg::OraRecentConnectionToolTip(item));
            connect(action, SIGNAL(triggered(bool)), SLOT(OpenRecentConnection()));
        }
    }

    ui->mainToolBar->addWidget(toolConnect);
    ui->mainToolBar->addAction(ui->actionOpenConnection);
    ui->mainToolBar->addAction(ui->actionCreate);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionImport);
    ui->mainToolBar->addAction(ui->actionImportDir);
    ui->mainToolBar->addAction(ui->actionExport);

    UpdateActions();
}

void MainWindow::CreateWindowsCombo()
{
    pWindowsComboBox = new TreeComboBox(this);
    pWindowsComboBox->setMinimumWidth(250);
    pWindowsModel = new SubWindowsModel(this);

    pWindowsComboBox->setModel(pWindowsModel);
    pWindowsComboBox->setMaxVisibleItems(15);

    connect(pWindowsModel, SIGNAL(windowsUpdated()), pWindowsComboBox->view(), SLOT(expandAll()));
    connect(pWindowsComboBox, SIGNAL(modelIndexChanged(QModelIndex)), SLOT(subWindowIndexChanged(QModelIndex)));
}

void MainWindow::subWindowActivated(QMdiSubWindow *window)
{
    if (!window)
        return;

    FmtWorkWindow *wnd = (FmtWorkWindow*)window->widget();
    QModelIndex index = pWindowsModel->findWindow(wnd->connection(), window);
    pWindowsComboBox->setCurrentIndex(index);
}

void MainWindow::subWindowIndexChanged(const QModelIndex &index)
{
    QMdiSubWindow *wnd = pWindowsModel->window(index);

    if (wnd)
        SetActiveFmtWindow(wnd);
}

void MainWindow::UpdateActions()
{
    //ConnectionInfo *cur = currentConnection();
}

void MainWindow::ImpExpSettings()
{
    ImpExpParams dlg(this);
    dlg.exec();
}

void MainWindow::ExportXmlAction()
{
    ConnectionInfo *cur = currentConnection();
    if (!cur)
        return;

#ifndef _DEBUG
    if (!CheckConnectionType(cur, ConnectionInfo::CON_ORA, true, this))
        return;
#endif

    ExportToXmlWizard wizard(cur, this);
    if (wizard.exec() == QDialog::Accepted)
    {

    }
}

void MainWindow::ImpDirAction()
{
    ConnectionInfo *current = currentConnection();
    if (!current)
        return;

#ifndef _DEBUG
    if (!CheckConnectionType(current, ConnectionInfo::CON_ORA, true, this))
        return;
#endif

    FmtImpExpWrp imp(current, this);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Импорт каталога"), imp.lastImportDir());

    if (dir.isEmpty())
        return;

    QProgressDialog dlg(tr("Загрузка xml файлов из каталога."), tr("Прервать"), 0, 0, this);
    QEventLoop loop;

    dlg.open(&imp, SIGNAL(finished()));
    connect(&imp, SIGNAL(finished(int)), &loop, SLOT(quit()));
    connect(&dlg, SIGNAL(canceled()), &imp, SLOT(cancel()));
    connect(&dlg, SIGNAL(canceled()), &loop, SLOT(quit()));
    imp.importDir(dir);

    if (imp.isRunning())
    {
        loop.exec();

        FmtErrors log;
        imp.parseProtocol(&log);
        ErrorDlg edlg(ErrorDlg::mode_Information, this);
        edlg.setErrors(&log);
        edlg.setMessage(tr("Протокол загрузки xml файлов: "));
        edlg.exec();
    }
}

void MainWindow::ImportAction()
{
    ConnectionInfo *current = currentConnection();
    if (!current)
        return;

#ifndef _DEBUG
    if (!CheckConnectionType(current, ConnectionInfo::CON_ORA, true, this))
        return;
#endif

    FmtImpExpWrp imp(current, this);
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Импорт файлов"), imp.lastImportDir(), "Xml файлы(*.xml)");

    if (files.isEmpty())
        return;

    imp.addTable(files);
    QProgressDialog dlg(tr("Загрузка xml файлов из каталога."), tr("Прервать"), 0, files.size(), this);
    QEventLoop loop;

    dlg.open(&imp, SIGNAL(finished()));
    connect(&imp, SIGNAL(finished(int)), &loop, SLOT(quit()));
    connect(&dlg, SIGNAL(canceled()), &imp, SLOT(cancel()));
    connect(&dlg, SIGNAL(canceled()), &loop, SLOT(quit()));

    int pos = 0;
    FmtErrors log;
    foreach (const QString &file, files)
    {
        imp.importFile(file);

        if (imp.isRunning())
        {
            dlg.setValue(pos);
            loop.exec();
            pos ++;
            imp.parseProtocol(&log);
        }
    }

    if (!log.isEmpty())
    {
        ErrorDlg edlg(ErrorDlg::mode_Information, this);
        edlg.setErrors(&log);
        edlg.setMessage(tr("Протокол загрузки xml файлов: "));
        edlg.exec();
    }
}

void MainWindow::OpenConnection(const QString &connectionString)
{
    QString dsn;
    QString user;
    QString pswd;

    if (ParseConnectionString(connectionString, user, pswd, dsn))
    {
        ConnectionInfo *info = Q_NULLPTR;
        OracleAuthDlg::OraCreateConnection(user, pswd, dsn, &info);

        if (info->isOpen())
        {
            CreateConnectionActio(user, info);
            info->updateFmtList();
        }
        else
            delete info;
    }
}

void MainWindow::actionConnectTriggered()
{
    OracleAuthDlg dlg(this);

    if (dlg.exec() == QDialog::Accepted)
    {
        ConnectionInfo *info = dlg.createConnectionInfo();
        CreateConnectionActio(dlg.getConnectionSheme(), info);
        info->updateFmtList();
    }
}

QAction *MainWindow::CreateConnectionActio(const QString &ShemeName, ConnectionInfo *info)
{
    QAction *a = new QAction(this);
    a->setText(ShemeName);
    a->setCheckable(true);
    ui->tabToolBar->addAction(a);
    m_ConnectionsGroup->addAction(a);
    a->setChecked(true);

    a->setIcon(info->colorIcon());
    a->setData(reinterpret_cast<int>(info));
    pWindowsModel->addConnection(info);

    info->updateFmtList();
    m_pConnections.append(info);
    //pTablesDock->setModel(info->tablesModel());
    pTablesDock->setConnection(info);

    return a;
}

ConnectionInfo *MainWindow::currentConnection()
{
    ConnectionInfo *cur = Q_NULLPTR;

    foreach (QAction *act, m_ConnectionsGroup->actions()) {
        if (act->isChecked())
        {
            cur = reinterpret_cast<ConnectionInfo*>(act->data().toInt());
        }
    }
    return cur;
}

void MainWindow::actionEditFmt()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QString ntable = actionEdit->data().toString();
    QMdiSubWindow *wnd = hasTableWindow(ntable);

    if (wnd == Q_NULLPTR)
    {
        QSharedPointer<FmtTable> table(new FmtTable(current));
        if (table->load(ntable))
        {
            QMdiSubWindow *window = CreateDocument(table);
            window->show();
        }
    }
    else
       SetActiveFmtWindow(wnd);
}

void MainWindow::tableClicked(const quint32 &id)
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QMdiSubWindow *wnd = hasTableWindow(id);
    if (wnd == Q_NULLPTR)
    {
        QSharedPointer<FmtTable> table(new FmtTable(current));
        if (table->load(id))
        {
            CreateDocument(table)->show();
        }
    }
    else
       SetActiveFmtWindow(wnd);
}

void MainWindow::SetActiveFmtWindow(QMdiSubWindow *wnd)
{
    wnd->setWindowState(Qt::WindowNoState);
    pMdi->setActiveSubWindow(wnd);
}

void MainWindow::OnTableChangeUpdtList()
{
    TablesDockWidget *list = pTablesDock->tablesWidget();

    if (list->isFiltered())
        list->updateList();
}

QMdiSubWindow *MainWindow::CreateDocument(QSharedPointer<FmtTable> &table, FmtWorkWindow **pWindow)
{
    FmtWorkWindow *window = new FmtWorkWindow;
    m_Windows[table->connection()].push_back(window);

    QMdiSubWindow *wnd = pMdi->addSubWindow(window);
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    wnd->setWindowTitle(window->makeWindowTitle(table));
    wnd->setWindowIcon(QIcon(":/table"));
    window->setParentWnd(wnd);
    window->setFmtTable(table);

    QModelIndex index = pWindowsModel->addWindow(table->connection(), wnd);
    pWindowsComboBox->setCurrentIndex(index);
    connect(window, SIGNAL(accepted()), wnd, SLOT(deleteLater()));
    connect(window, SIGNAL(rejected()), wnd, SLOT(deleteLater()));
    connect(window, SIGNAL(destroyed(QObject*)), SLOT(WorkWindowDestroyed(QObject*)));
    connect(window, SIGNAL(needUpdateTableList()), SLOT(OnTableChangeUpdtList()));

    if (pWindow)
        *pWindow = window;

    return wnd;
}

void MainWindow::WorkWindowDestroyed(QObject *wnd)
{
    const FmtWorkWindow *window = static_cast<const FmtWorkWindow*>(wnd);

    if (window)
    {
        ConnectionInfo *connection = window->connection();

        if (m_Windows[connection].contains((QWidget*)window))
            m_Windows[connection].removeOne((QWidget*)window);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings *s = settings();
    s->setValue("Geometry", saveGeometry());
    s->setValue("State", saveState());
    event->accept();
}

void MainWindow::conActionTriggered(QAction *action)
{
    ConnectionInfo *info = reinterpret_cast<ConnectionInfo*>(action->data().toInt());
    pTablesDock->setConnection(info);
            //->setModel(info->tablesModel());
}

void MainWindow::actionCreate()
{
    ConnectionInfo *current = currentConnection();

    if (current)
    {
        QSharedPointer<FmtTable> table(new FmtTable(current));
        CreateDocument(table)->show();
    }
}

void MainWindow::rebuidOffsets()
{
    QMdiSubWindow *wnd = pMdi->currentSubWindow();

    if (wnd)
    {
        FmtWorkWindow *doc = (FmtWorkWindow*)wnd->widget();
        doc->rebuildOffsets();
    }
}

void MainWindow::actionDisconnectTriggered()
{
    QAction *active = m_ConnectionsGroup->checkedAction();

    if (active)
    {
        ConnectionInfo *current = reinterpret_cast<ConnectionInfo*>(active->data().toInt());

        if (current)
        {
            bool NeedClose = true;
            if (!m_Windows[current].isEmpty())
            {
                if (QMessageBox::question(this, tr("Закрытие соеденения."), tr("Имеются незакрытые окна, работающие с соеденением. Завершить подключение?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
                    NeedClose = false;
            }

            if (NeedClose)
            {
                current->db().close();
                ui->tabToolBar->removeAction(active);
                m_ConnectionsGroup->removeAction(active);
                pTablesDock->closeConnection();
                //pTablesDock->setModel(nullptr);

                QList<QMdiSubWindow*> windows = pMdi->subWindowList();
                foreach (QMdiSubWindow *w, windows)
                {
                    FmtWorkWindow *doc = (FmtWorkWindow*)w->widget();
                    if (doc->connection() == current)
                        w->deleteLater();
                }

                QList<QAction*> actions = m_ConnectionsGroup->actions();
                if (!actions.isEmpty())
                {
                    actions[0]->setChecked(true);
                    conActionTriggered(actions[0]);
                }
                m_pConnections.removeOne(current);
                pWindowsModel->removeConnection(current);

                delete active;
                delete current;
            }
        }
    }
}

void MainWindow::createFromText()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    FmtFromRichText wizard(current, this);
    if (wizard.exec() == QDialog::Accepted)
    {
        QMdiSubWindow *wnd = CreateDocument(wizard.fmtTable());
        wnd->show();
    }
}

void MainWindow::actionExportTableXml()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    if (!CheckConnectionType(current, ConnectionInfo::CON_ORA, true, this))
        return;

    QString table = actionExport->data().toString();

    FmtImpExpWrp tmp(current);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Экспорт файла"), tmp.lastImportDir());

    if (dir.isEmpty())
        return;

    ExportFmtToXml(current, QStringList() << table, dir, true, true, this);
}

void MainWindow::actionInit()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    if (!CheckConnectionType(current, ConnectionInfo::CON_ORA, true, this))
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();

    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            InitFmtTable(table, this);
        }
    }
}

void MainWindow::CreateTableSql()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();

    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            SaveFmtTableSql(table, this);
        }
    }
}

void MainWindow::HotFixCreate()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();

    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            FmtHotFixCreate(table);
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == pTablesDock->tablesWidget())
    {
        bool hr = false;
        QListView *view = pTablesDock->tablesWidget()->listView();

        if (!view->isEnabled())
            return false;

        if (event->type() == QEvent::ContextMenu)
        {
            tablesContextMenu((QContextMenuEvent*)event, view);
            hr =  true;
        }

        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *e = (QKeyEvent*)event;

            if (e->key() == Qt::Key_Return)
            {
                if (view->selectionModel()->hasSelection())
                {
                    QString table = view->selectionModel()->selectedIndexes().at(0).data(Qt::UserRole).toString();
                    actionEdit->setData(table);
                    emit actionEdit->triggered();
                    hr = true;
                }
            }
        }

        return hr;
    }
    return false;
}

void MainWindow::tablesContextMenu(QContextMenuEvent *event, QListView *view)
{
#ifndef QT_NO_CONTEXTMENU
    QMenu menu(this);
    menu.addAction(actionEdit);
    menu.addAction(ui->actionInit);
    menu.addSeparator();
    menu.addAction(ui->actionCopyTable);
    menu.addAction(ui->actionCopyTableAs);
    menu.addSeparator();
    menu.addAction(actionExport);

    menu.addAction(ui->actionEditContent);
    menu.addAction(ui->actionUnloadDbf);
    menu.addAction(ui->actionLoadDbf);
    menu.addSeparator();
    menu.setDefaultAction(actionEdit);
    menu.addAction(ui->actionSql);
    menu.addMenu(ui->menuUpdateScripts);

    menu.addSeparator();
    menu.addAction(ui->actionCreate);
    menu.addAction(actionDeleteTable);

    if (view->selectionModel()->hasSelection())
    {
        QString table = view->selectionModel()->selectedIndexes().at(0).data(Qt::UserRole).toString();
        actionEdit->setEnabled(true);
        actionEdit->setData(table);

        actionExport->setEnabled(true);
        actionExport->setData(table);
    }
    else
    {
        actionEdit->setEnabled(false);
        actionEdit->setData("");

        actionExport->setEnabled(false);
        actionExport->setData("");
    }

    menu.exec(event->globalPos());
#endif
}

void MainWindow::CreateWindowFunctional()
{
    ui->actionNextWnd->setShortcut(QKeySequence(QKeySequence::NextChild));
    ui->actionPrevWnd->setShortcut(QKeySequence(QKeySequence::PreviousChild));

    ui->windowToolBar->addAction(ui->actionWindowList);
    ui->windowToolBar->addWidget(pWindowsComboBox);
    ui->windowToolBar->addAction(ui->actionPrevWnd);
    ui->windowToolBar->addAction(ui->actionNextWnd);

    connect(ui->actionNextWnd, SIGNAL(triggered(bool)), pMdi, SLOT(activateNextSubWindow()));
    connect(ui->actionPrevWnd, SIGNAL(triggered(bool)), pMdi, SLOT(activatePreviousSubWindow()));
    connect(ui->actionCascadeWnd, SIGNAL(triggered(bool)), pMdi, SLOT(cascadeSubWindows()));
    connect(ui->actionTileWnd, SIGNAL(triggered(bool)), pMdi, SLOT(tileSubWindows()));
    connect(ui->actionCloseWnd, SIGNAL(triggered(bool)), pMdi, SLOT(closeActiveSubWindow()));
    connect(ui->actionCloseAllWnd, SIGNAL(triggered(bool)), pMdi, SLOT(closeAllSubWindows()));
    connect(ui->actionWindowList, SIGNAL(triggered(bool)), SLOT(showWindowList()));
}

void MainWindow::CreateViewMenu()
{
    ui->menuView->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuView->addAction(ui->tabToolBar->toggleViewAction());
    ui->menuView->addAction(ui->windowToolBar->toggleViewAction());
    ui->menuView->addAction(pSearch->toggleViewAction());
    ui->menuView->addSeparator();
    ui->menuView->addAction(pTablesDock->toggleViewAction());
}

void MainWindow::CreateSearchToolBar()
{
    pSearch = addToolBar(tr("Поиск"));
    pSearch->setObjectName("SearchToolBar");
    pSearchLine = new QLineEdit(pSearch);
    pSearchLine->setPlaceholderText(tr("Введите текст для поиска..."));
    pSearchLine->setMaximumWidth(350);
    pSearchLine->setClearButtonEnabled(true);

    pSearch->addWidget(pSearchLine);

    pFindShortcut = new QShortcut(QKeySequence::Find, this);

    connect(pSearchLine, SIGNAL(textChanged(QString)), pTableListDelegate, SLOT(setHighlightText(QString)));
    connect(pSearchLine, SIGNAL(textChanged(QString)), pTablesDock, SLOT(forceRepaint()));
    connect(pFindShortcut, SIGNAL(activated()), pSearchLine, SLOT(setFocus()));
}

void MainWindow::showWindowList()
{
    WindowsListDlg dlg(pWindowsModel, pMdi, this);
    dlg.exec();
}

void MainWindow::about()
{
//#ifdef FMTLIB_ABOUT_EXCEPTION
    /*int* p = NULL;
    p[0] = 10;*/
//#endif

    AboutDlg dlg(this);
    dlg.exec();
}

void MainWindow::EditGroups()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    TablesGroupsDlg dlg(current, this);
    dlg.exec();
}

void MainWindow::OpenRecentConnection()
{
    QAction *action = qobject_cast<QAction*>(sender());

    if (action)
    {
        RecentList item = qvariant_cast<RecentList>(action->data());

        ConnectionInfo *info = Q_NULLPTR;
        QSqlDatabase db = OracleAuthDlg::OraCreateConnection(item.user,
                                           item.pass,
                                           item.host,
                                           item.service,
                                           item.database,
                                           item.dsn,
                                           item.port,
                                           &info);
        if (db.isOpen() && info)
            CreateConnectionActio(item.database, info);
    }
}

void MainWindow::CopyTable()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            QSharedPointer<FmtTable> cTable(new FmtTable(current));
            table->copyTo(cTable);
            CreateDocument(cTable)->show();
        }
    }
}

void MainWindow::CopyTableTo()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            SelectConnectionDlg dlg(m_pConnections, this);
            dlg.setWindowTitle(tr("Копировать в..."));
            if (dlg.exec() == QDialog::Accepted)
            {
                QList<ConnectionInfo*> checkedItems = dlg.сheckedItems();
                foreach (ConnectionInfo *info, checkedItems) {
                    QSharedPointer<FmtTable> cTable(new FmtTable(info));
                    table->copyTo(cTable);
                    CreateDocument(cTable)->show();
                }
            }
        }
    }
}

void MainWindow::RsExpExportDir()
{
    QString path = QFileDialog::getExistingDirectory(this);

    if (!path.isEmpty())
    {
        settings()->setValue("RsExpUnlDir", path);
    }
}

void MainWindow::UnloadDbf()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QString table = index.data(Qt::UserRole).toString();

        StartUnloadDbf(current, table, this);
    }
}

void MainWindow::LoadDbf()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QString table = index.data(Qt::UserRole).toString();

        QString RsExpUnlDir = settings()->value("RsExpUnlDir").toString();
        QString file = QFileDialog::getOpenFileName(this, QString(), RsExpUnlDir, QString("%1.dat (%1.dat)").arg(table));

        if (!file.isEmpty())
            StartLoadDbf(current, file, this);
    }
}

void MainWindow::UnloadSqlite()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QSqlDatabase sqliteDB = QSqlDatabase::addDatabase("QSQLITE", "sqliteDBexport");
    QString fname = QFileDialog::getSaveFileName(this, QString(), QString(), QString("Sqlite db (*.sqlite *.db)"));

    if (!fname.isEmpty())
    {
        sqliteDB.setDatabaseName(fname);
        if (sqliteDB.open())
        {
            if (!CloneFmtFromConnection(current->db(), sqliteDB, this))
            {

            }
            sqliteDB.close();
            QSqlDatabase::removeDatabase(fname);
        }
    }
}

void MainWindow::OpenConnection()
{
    QSettings *s = settings();
    QString filename = QFileDialog::getOpenFileName(this, tr("Открыть файл подключения"), s->value("LastOpenConnectionDir", QString()).toString(),
                                                    tr("Sqlite db, rsreq.ini (*.sqlite *.db rsreq.ini);;Sqlite db(*.sqlite *.db);;rsreq.ini(rsreq.ini)"));

    if (!filename.isEmpty())
    {
        ConnectionInfo *info = new ConnectionInfo();

        QFileInfo finfo(filename);
        QString suffix = finfo.suffix();
        if (suffix == "db" || suffix == "sqlite")
        {
            if (!info->openSqlite(filename))
                delete info;
            else
            {
                CreateConnectionActio(info->schemeName(), info);
                info->updateFmtList();
                s->setValue("LastOpenConnectionDir", finfo.absolutePath());
                s->sync();
            }
        }
        else if (suffix == "ini")
        {
            QString constrtype1regexp = ConstrType1RegExp();
            QString rsreq = ReadTextFileContent(filename);
            QRegExp rx(constrtype1regexp);

            QStringList list;
            int pos = 0;

            while ((pos = rx.indexIn(rsreq, pos)) != -1) {
                list << rx.cap(0);
                pos += rx.matchedLength();
            }

            if (!list.isEmpty())
            {
                StringListDlg dlg(this);
                dlg.setWindowTitle(tr("Открыть подключение"));
                dlg.setList(list);
                if (dlg.exec() == QDialog::Accepted)
                    OpenConnection(dlg.selected());
            }

            s->setValue("LastOpenConnectionDir", finfo.absolutePath());
            s->sync();
        }
    }
}

void MainWindow::RemoveFmtTable()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            if (table->removeFmtTable())
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось удалить запись о таблице <b>%1</b> из FMT словаря")
                                      .arg(table->name()));
            else
                current->updateFmtList();
        }
    }
}

void MainWindow::LoggingSettings()
{
    LogSettingsDlg dlg(this);
    dlg.exec();
}

QMdiSubWindow *MainWindow::hasTableWindow(const QString &tableName)
{
    QMdiSubWindow *find = Q_NULLPTR;
    QList<QWidget*> lst = m_Windows[currentConnection()];

    QList<QWidget*>::Iterator iter = lst.begin();
    while(iter != lst.end())
    {
        FmtWorkWindow *wnd = dynamic_cast<FmtWorkWindow*>(*iter);

        if (wnd)
        {
            if (wnd->table()->name().toLower() == tableName.toLower())
            {
                find = wnd->mdiWindow();
                break;
            }
        }
        ++iter;
    }

    return find;
}

QMdiSubWindow *MainWindow::hasTableWindow(const FmtRecId &tableID)
{
    QMdiSubWindow *find = Q_NULLPTR;
    QList<QWidget*> lst = m_Windows[currentConnection()];

    QList<QWidget*>::Iterator iter = lst.begin();
    while(iter != lst.end())
    {
        FmtWorkWindow *wnd = dynamic_cast<FmtWorkWindow*>(*iter);

        if (wnd)
        {
            if (wnd->table()->id() == tableID)
            {
                find = wnd->mdiWindow();
                break;
            }
        }
        ++iter;
    }

    return find;
}

void MainWindow::EditContent()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    const QListView *view = pTablesDock->tablesWidget()->listView();
    QItemSelectionModel *selectionModel = view->selectionModel();
    if (selectionModel->hasSelection())
    {
        QModelIndex index = selectionModel->selectedIndexes().at(0);
        QString tableName = index.data(Qt::UserRole).toString();
        QMdiSubWindow *wnd = hasTableWindow(tableName);

        if (wnd == Q_NULLPTR)
        {
            QSharedPointer<FmtTable> table(new FmtTable(current));

            if (table->load(tableName))
            {
                FmtWorkWindow *window = Q_NULLPTR;
                CreateDocument(table, &window)->show();
                window->EditContent();
            }
        }
        else
            SetActiveFmtWindow(wnd);
    }
}

void MainWindow::GenCreateTableScript()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            FmtWorkWindow *window = Q_NULLPTR;
            CreateDocument(table, &window)->show();
            window->GenCreateTableScript();
        }
    }
}

void MainWindow::GenModifyTableFields()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            FmtWorkWindow *window = Q_NULLPTR;
            CreateDocument(table, &window)->show();
            window->GenModifyTableFields();
        }
    }
}

void MainWindow::GenAddFiledsScript()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            FmtWorkWindow *window = Q_NULLPTR;
            CreateDocument(table, &window)->show();
            window->GenAddFiledsScript();
        }
    }
}

void MainWindow::GenDeleteFiledsScript()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            FmtWorkWindow *window = Q_NULLPTR;
            CreateDocument(table, &window)->show();
            window->GenDeleteFiledsScript();
        }
    }
}

void MainWindow::OnMassOpAction()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    MassOperationWizard wizard(current, this);
    wizard.exec();
}

void MainWindow::OnConfluence()
{
    QDesktopServices::openUrl(QUrl("http://confluence/pages/viewpage.action?pageId=397967369"));
}
