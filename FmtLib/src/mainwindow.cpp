#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tablesdock.h"
#include "oracleauthdlg.h"
#include "connectioninfo.h"
#include "fmtworkwindow.h"
#include "fmttable.h"
#include "fmtcore.h"
#include "options/fmtoptionsdlg.h"
#include "options/externaltoolspage.h"
#include "fmtfromrichtext.h"
#include "treecombobox.h"
#include "subwindowsmodel.h"
#include "fmtimpexpwrp.h"
#include "errorsmodel.h"
#include "aboutdlg.h"
#include "tablesdockwidget.h"
#include "exporttoxmlwizard.h"
#include "errordlg.h"
#include <fmtapplication.h>
#include "tablesgroupprovider.h"
#include "windowslistdlg.h"
#include "tablesgroupsdlg.h"
#include "selectconnectiondlg.h"
#include "stringlistdlg.h"
#include "fmttablelistdelegate.h"
#include "massoperationwizard.h"
#include "queryeditor/queryeditor.h"
#include "selectfolderdlg.h"
#include "recentconnectionlist.h"
#include "debugconnect.h"
#include "updatecheckermessagebox.h"
#include "windowactionsregistry.h"
#include "rslexecutors/toolbaractionexecutor.h"
#include "widgets/guiconverterdlg.h"
#include "widgets/sqlconvertordlg.h"
#include <QRegExp>
#include <QRegularExpression>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QWhatsThis>
#include <QWidgetAction>
#include <QInputDialog>
#include <QThreadPool>

class SearchActionWidget : public QWidgetAction
{
    FmtTableListDelegate *m_pDelegate;
    TablesDock *m_pTablesDock;
    QLineEdit *pSearchLine;
public:
    SearchActionWidget(FmtTableListDelegate *delegate, TablesDock *tablesDock, QWidget *parent = nullptr) :
        QWidgetAction(parent),
        m_pDelegate(delegate),
        m_pTablesDock(tablesDock),
        pSearchLine(nullptr)
    {
    }

    virtual ~SearchActionWidget() {}

    QLineEdit *edit()
    {
        return pSearchLine;
    }

    virtual QWidget *createWidget(QWidget *parent)
    {
        pSearchLine = new QLineEdit(parent);
        pSearchLine->setPlaceholderText(tr("Введите текст для поиска..."));
        pSearchLine->setFrame(false);
        pSearchLine->setMinimumWidth(350);
        pSearchLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        pSearchLine->setClearButtonEnabled(true);

        connect(pSearchLine, SIGNAL(textChanged(QString)), m_pDelegate, SLOT(setHighlightText(QString)));
        connect(pSearchLine, SIGNAL(textChanged(QString)), m_pTablesDock, SLOT(forceRepaint()));

        return pSearchLine;
    }

    virtual void deleteWidget(QWidget *widget)
    {
        delete widget;
    }
};

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

    QDir dir = QApplication::applicationDirPath();
    if (!QFile::exists(dir.absoluteFilePath("DumpTool.exe")))
        dir = QDir::current();
    if (!QFile::exists(dir.absoluteFilePath("DumpTool.exe")))
        ui->actionDumpTool->setEnabled(false);

    if (!QFile::exists(dir.absoluteFilePath("DiffToScript.exe")))
        ui->actionDiffTool->setEnabled(false);

    m_ConnectionsGroup = new QActionGroup(this);

    pUpdateButton = new QPushButton(this);
    pUpdateButton->setToolTip(tr("Обновления"));
    pUpdateButton->setIcon(QIcon(":/img/base_globe_32.png"));
    pUpdateButton->setFlat(true);
    ui->statusBar->addPermanentWidget(pUpdateButton);

    CreateWindowsCombo();
    CreateMainToolBar();
    CreateWindowFunctional();
    CreateSearchToolBar();
    CreateViewMenu();
    CreateCheckUpdateRunnable();

    pActionExecutor = new ToolbarActionExecutor(this);
    windowActionsRegistry()->setRslExecutor(pActionExecutor);

    windowActionsRegistry()->scanActions(ui->menuFile);
    windowActionsRegistry()->scanActions(ui->menuService);
    windowActionsRegistry()->scanActions(ui->menuWindow);
    windowActionsRegistry()->scanActions(ui->menu);

    QList<QToolBar*> toolBars = windowActionsRegistry()->makeToolBars(((FmtApplication*)qApp)->settings(), "UserCommands", "ToolBars");
    for (QToolBar *toolbar : toolBars)
        addToolBar(toolbar);

    ui->tabToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->tabToolBar->setIconSize(QSize(20, 20));

    QSettings *s = settings();
    restoreGeometry(s->value("Geometry").toByteArray());
    restoreState(s->value("State").toByteArray());

    actionEdit = new QAction(QIcon(":/img/edittable.png"), tr("Редактировать"), this);
    actionExport = new QAction(QIcon(":/img/savexml.png"), tr("Экспорт в xml файл"), this);
    actionDeleteTable = new QAction(QIcon(":/img/removetable.png"), tr("Удалить запись"), this);

    pUserActionsMenu = nullptr;
    CreateUserCommandsMenu(&pUserActionsMenu, tr("Пользовательские действия"),
                           this, SLOT(onUserActionTriggered()));

    connect(ui->actionConnect, SIGNAL(triggered(bool)), SLOT(actionConnectTriggered()));
    connect(ui->actionDisconnect, SIGNAL(triggered(bool)), SLOT(actionDisconnectTriggered()));
    connect(pTablesDock, SIGNAL(tableDbClicked(quint32)), SLOT(tableClicked(quint32)));
    connect(m_ConnectionsGroup, SIGNAL(triggered(QAction*)), SLOT(conActionTriggered(QAction*)));
    connect(ui->actionCreate, SIGNAL(triggered(bool)), SLOT(actionCreate()));
    connect(ui->actionRebuildOffset, SIGNAL(triggered(bool)), SLOT(rebuidOffsets()));
    connect(ui->actionCreateFromText, SIGNAL(triggered(bool)), SLOT(createFromText()));

    connect(ui->actionImportDir, SIGNAL(triggered(bool)), SLOT(ImpDirAction()));
    connect(ui->actionImport, SIGNAL(triggered(bool)), SLOT(ImportAction()));

    connect(actionEdit, SIGNAL(triggered(bool)), SLOT(actionEditFmt()));
    connect(actionExport, SIGNAL(triggered(bool)), SLOT(actionExportTableXml()));
    connect(ui->actionInit, SIGNAL(triggered(bool)), SLOT(actionInit()));
    connect(pMdi, SIGNAL(subWindowActivated(QMdiSubWindow*)), SLOT(subWindowActivated(QMdiSubWindow*)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), SLOT(about()));
    connect(ui->actionSql, SIGNAL(triggered(bool)), SLOT(CreateTableSql()));
    connect(ui->actionCopyTable, SIGNAL(triggered(bool)), SLOT(CopyTable()));
    connect(ui->actionCopyTableAs, SIGNAL(triggered(bool)), SLOT(CopyTableTo()));
    connect(ui->actionCopyTableTmp, SIGNAL(triggered(bool)), SLOT(CopyTableToTmp()));
    connect(ui->actionUnloadDbf, SIGNAL(triggered(bool)), SLOT(UnloadDbf()));
    connect(ui->actionLoadDbf, SIGNAL(triggered(bool)), SLOT(LoadDbf()));
    connect(ui->action_FMT_sqlite, SIGNAL(triggered(bool)), SLOT(UnloadSqlite()));
    connect(ui->actionOpenConnection, SIGNAL(triggered(bool)), SLOT(OpenConnection()));
    connect(actionDeleteTable, SIGNAL(triggered(bool)), SLOT(RemoveFmtTable()));
    connect(ui->actionEditContent, SIGNAL(triggered(bool)), SLOT(EditContent()));
    connect(ui->actionGenCreateTbSql, SIGNAL(triggered(bool)), SLOT(GenCreateTableScript()));
    connect(ui->actionGenModifyScript, SIGNAL(triggered(bool)), SLOT(GenModifyTableFields()));
    connect(ui->actionGenAddScript, SIGNAL(triggered(bool)), SLOT(GenAddFiledsScript()));
    connect(ui->actionGenDelScript, SIGNAL(triggered(bool)), SLOT(GenDeleteFiledsScript()));
    connect(ui->actionMassOp, SIGNAL(triggered(bool)), SLOT(OnMassOpAction()));
    connect(ui->actionConfluence, SIGNAL(triggered(bool)), SLOT(OnConfluence()));
    connect(ui->actionCreateXml, SIGNAL(triggered(bool)), SLOT(CreateFromXml()));
    connect(ui->action_Diff_to_Script, SIGNAL(triggered(bool)), SLOT(GenDiffToScriptScript()));
    connect(ui->actionOptions, SIGNAL(triggered(bool)), SLOT(OptionsAction()));
    connect(ui->action_GuiConverter,SIGNAL(triggered(bool)), SLOT(StartGuiConverter()));
    connect(ui->actionDiffTables,SIGNAL(triggered(bool)), SLOT(CompareTables()));
    connect(ui->actionConvertSql,SIGNAL(triggered(bool)), SLOT(onSqlconerter()));

    ui->actionQuery->setVisible(false);
    connect(ui->actionQuery, SIGNAL(triggered(bool)), SLOT(OnCreateQuery()));

    connect(ui->actionDumpTool, &QAction::triggered, [=]()
    {
        QProcess::startDetached(dir.absoluteFilePath("DumpTool.exe"), QStringList());
    });

    connect(ui->actionDiffTool, &QAction::triggered, [=]()
    {
        QProcess::startDetached(dir.absoluteFilePath("DiffToScript.exe"), QStringList());
    });

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

    RecentConnectionList list;
    if (list.load())
    {
        if (!list.isEmpty())
        {
            toolConnectMenu->addAction(ui->actionConnect);
            toolConnect->setDefaultAction(ui->actionConnect);
            toolConnectMenu->setDefaultAction(ui->actionConnect);
            toolConnectMenu->addSeparator();

            int count = list.rowCount();
            for (int i = 0; i < count; i++)
            {
                RecentList item = list.record(i);
                QAction *action = toolConnectMenu->addAction(RecentConnectionList::connectionName(item));
                action->setData(QVariant::fromValue(item));
                action->setToolTip(RecentConnectionList::connectionToolTip(item));
                connect(action, SIGNAL(triggered(bool)), SLOT(OpenRecentConnection()));
            }
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

    MdiSubInterface *wnd = qobject_cast<MdiSubInterface*>(window->widget());

    if (wnd)
    {
        QModelIndex index = pWindowsModel->findWindow(wnd->connection(), window);
        pWindowsComboBox->setCurrentIndex(index);
    }
}

void MainWindow::subWindowIndexChanged(const QModelIndex &index)
{
    QMdiSubWindow *wnd = pWindowsModel->window(index);

    if (wnd)
        SetActiveFmtWindow(wnd);
}

void MainWindow::UpdateActions()
{
    ConnectionInfo *cur = currentConnection();

    if (!cur)
        return;

    if (cur->type() != ConnectionInfo::CON_ORA)
        ui->actionEditContent->setEnabled(false);

    if (cur->hasFeature(ConnectionInfo::CanCreateTable))
    {
        actionEdit->setText(tr("Редактировать"));
        actionDeleteTable->setEnabled(true);
        ui->actionCreate->setEnabled(true);
        ui->actionRebuildOffset->setEnabled(true);
        ui->actionCreateFromText->setEnabled(true);
        ui->actionInit->setEnabled(true);

        ui->actionCopyTable->setEnabled(true);
        ui->actionCopyTableAs->setEnabled(true);
        ui->actionCopyTableTmp->setEnabled(true);

        ui->actionMassOp->setEnabled(true);
        ui->actionCreateXml->setEnabled(true);
    }
    else
    {
        actionEdit->setText(tr("Просмотр"));
        actionDeleteTable->setEnabled(false);
        ui->actionCreate->setEnabled(false);
        ui->actionRebuildOffset->setEnabled(false);
        ui->actionCreateFromText->setEnabled(false);
        ui->actionInit->setEnabled(false);

        ui->actionCopyTable->setEnabled(false);
        ui->actionCopyTableAs->setEnabled(false);
        ui->actionCopyTableTmp->setEnabled(false);

        ui->actionMassOp->setEnabled(false);
        ui->actionCreateXml->setEnabled(false);
    }

    if (cur->hasFeature(ConnectionInfo::CanSaveToXml))
    {
        actionExport->setEnabled(true);
        ui->actionImpExpPrm->setEnabled(true);
        ui->actionImportDir->setEnabled(true);
        ui->actionImport->setEnabled(true);
    }
    else
    {
        actionExport->setEnabled(false);
        ui->actionImpExpPrm->setEnabled(false);
        ui->actionImportDir->setEnabled(false);
        ui->actionImport->setEnabled(false);
    }

    if (cur->hasFeature(ConnectionInfo::CanLoadUnloadDbf))
    {
        ui->actionUnloadDbf->setEnabled(true);
        ui->actionLoadDbf->setEnabled(true);
    }
    else
    {
        ui->actionUnloadDbf->setEnabled(false);
        ui->actionLoadDbf->setEnabled(false);
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

        ErrorsModel log;
        imp.parseProtocol(&log);
        ErrorDlg edlg(ErrorDlg::ModeInformation, this);
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
    ErrorsModel log;
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
        ErrorDlg edlg(ErrorDlg::ModeInformation, this);
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
        ConnectionInfo *info = new ConnectionInfo();

        if (OracleAuthDlg::tryConnect(info, user, pswd, dsn, QString(), this))
        {
            CreateConnectionActio(info);
            info->updateFmtList();
        }
        else
            delete info;
    }
}

void MainWindow::actionConnectTriggered()
{
    (void)openConnection();
}

ConnectionInfo* MainWindow::openConnection()
{
    OracleAuthDlg dlg(this);
    ConnectionInfo *info = nullptr;

    if (dlg.exec() == QDialog::Accepted)
    {
        info = dlg.getConnectionInfo();
        CreateConnectionActio(info);
        info->updateFmtList();
    }

    return info;
}

bool MainWindow::isExistsConnection(ConnectionInfo *connection)
{
    for (ConnectionInfo *info : m_pConnections)
    {
        if (info->connectionName() == connection->connectionName())
            return true;
    }

    return false;
}

bool MainWindow::addConnection(ConnectionInfo *connection)
{
    if (isExistsConnection(connection))
        return false;

    CreateConnectionActio(connection);
    connection->updateFmtList();

    return true;
}

QAction *MainWindow::CreateConnectionActio(ConnectionInfo *info)
{
    QAction *a = new QAction(this);
    a->setText(info->schemeName());
    a->setCheckable(true);
    ui->tabToolBar->addAction(a);
    m_ConnectionsGroup->addAction(a);
    a->setChecked(true);

    QSize toolBarIconSize = ui->tabToolBar->iconSize();
    a->setIcon(info->colorIcon(toolBarIconSize));
    a->setData(reinterpret_cast<qintptr>(info));
    pWindowsModel->addConnection(info);

    info->updateFmtList();
    m_pConnections.append(info);
    //pTablesDock->setModel(info->tablesModel());
    pTablesDock->setConnection(info);

    UpdateActions();

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

QMdiSubWindow *MainWindow::currentMdiWindow()
{
    return pMdi->activeSubWindow();
}

void MainWindow::OnTableChangeUpdtList()
{
    TablesDockWidget *list = pTablesDock->tablesWidget();

    if (list->isFiltered())
        list->updateList();
}

QMdiSubWindow *MainWindow::CreateMdiWindow(MdiSubInterface *window, ConnectionInfo *pConnection)
{
    m_Windows[pConnection].push_back(window);
    QMdiSubWindow *wnd = pMdi->addSubWindow(window);
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    window->setParentWnd(wnd);
    window->setConnection(pConnection);
    wnd->setWindowTitle(window->makeWindowTitle());
    connect(window, SIGNAL(destroyed(QObject*)), SLOT(WorkWindowDestroyed(QObject*)));

    QModelIndex index = pWindowsModel->addWindow(pConnection, wnd);
    pWindowsComboBox->setCurrentIndex(index);

    return wnd;
}

QMdiSubWindow *MainWindow::CreateDocument(QSharedPointer<FmtTable> &table, FmtWorkWindow **pWindow)
{
    FmtWorkWindow *window = new FmtWorkWindow;
    window->setFmtTable(table);
    //m_Windows[table->connection()].push_back(window);
    QMdiSubWindow *wnd = CreateMdiWindow(window, table->connection());
    wnd->setWindowIcon(QIcon(":/table"));

    connect(window, SIGNAL(accepted()), wnd, SLOT(deleteLater()));
    connect(window, SIGNAL(rejected()), wnd, SLOT(deleteLater()));
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
    pUpdateChecker->requestInterruption();
    pUpdateChecker->deleteLater();
    event->accept();
}

void MainWindow::conActionTriggered(QAction *action)
{
    ConnectionInfo *info = reinterpret_cast<ConnectionInfo*>(action->data().toInt());
    pTablesDock->setConnection(info);

    UpdateActions();
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
        QSharedPointer<FmtTable> table = wizard.fmtTable();
        QMdiSubWindow *wnd = CreateDocument(table);
        wnd->show();
    }
}

void MainWindow::actionExportTableXml()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

#ifndef QT_DEBUG
    if (!CheckConnectionType(current, ConnectionInfo::CON_ORA, true, this))
        return;
#endif

    QString table = actionExport->data().toString();

    SelectFolderDlg folder(RsFmtUnlDirContext, tr("Экспорт в xml файл"), this);
    if (folder.exec() == QDialog::Accepted)
    {
        ExportFmtToXml(current, QStringList()
                       << table, folder.selectedPath(), true, true, this);
    }
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
            InitFmtTable(table.data(), this);
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
    menu.addAction(ui->actionCopyTableTmp);
    menu.addSeparator();
    menu.addAction(actionExport);
    menu.addSeparator();

    menu.addAction(ui->actionEditContent);
    menu.addAction(ui->actionUnloadDbf);
    menu.addAction(ui->actionLoadDbf);
    menu.addSeparator();
    menu.setDefaultAction(actionEdit);
    menu.addAction(ui->actionSql);
    menu.addSeparator();
    menu.addAction(ui->action_Diff_to_Script);
    menu.addSeparator();
    menu.addAction(ui->actionDiffTables);
    menu.addSeparator();
    menu.addMenu(ui->menuUpdateScripts);

    if (pUserActionsMenu)
        menu.addMenu(pUserActionsMenu);

    menu.addSeparator();

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

    menu.setActiveAction(actionEdit);
    menu.exec(event->globalPos());
#endif
}

void MainWindow::CreateWindowFunctional()
{
    ui->windowToolBar->addAction(ui->actionOptions);
    ui->actionNextWnd->setShortcut(QKeySequence(QKeySequence::NextChild));
    ui->actionPrevWnd->setShortcut(QKeySequence(QKeySequence::PreviousChild));

    ui->windowToolBar->addAction(ui->actionWindowList);
    ui->windowToolBar->addWidget(pWindowsComboBox);
    ui->windowToolBar->addAction(ui->actionPrevWnd);
    ui->windowToolBar->addAction(ui->actionNextWnd);
    ui->windowToolBar->addSeparator();
    ui->windowToolBar->addAction(ui->actionCloseWnd);
    ui->windowToolBar->addAction(ui->actionCloseAllWnd);

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
    ui->menuView->addSeparator();
    ui->menuView->addAction(pTablesDock->toggleViewAction());
}

void MainWindow::CreateSearchToolBar()
{
    pSearch = new QMenu();
    pSearchWidget = new SearchActionWidget(pTableListDelegate, pTablesDock, menuBar());

    pSearch->addAction(pSearchWidget);
    ui->menuBar->setCornerWidget(pSearch);

    pSearchLine = pSearchWidget->edit();
    pFindShortcut = new QShortcut(QKeySequence::Find, this);
    connect(pFindShortcut, SIGNAL(activated()), pSearchLine, SLOT(setFocus()));
}

void MainWindow::showWindowList()
{
    WindowsListDlg dlg(pWindowsModel, pMdi, this);
    dlg.exec();
}

void MainWindow::about()
{
    AboutDlg dlg(":/AboutDlg", this);
    dlg.exec();
}

void MainWindow::OpenRecentConnection()
{
    QAction *action = qobject_cast<QAction*>(sender());

    if (action)
    {
        RecentList2 item = qvariant_cast<RecentList2>(action->data());

        ConnectionInfo *info = new ConnectionInfo();
        QString options = OracleAuthDlg::OptionsMapToOptions(item.Options);
        if (OracleAuthDlg::tryConnect(info, item.user, item.pass, item.dsn, options, this))
        {
            CreateConnectionActio(info);
            info->updateFmtList();
        }
        else
            delete info;
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
                QList<ConnectionInfo*> checkedItems = dlg.checkedItems();
                foreach (ConnectionInfo *info, checkedItems) {
                    QSharedPointer<FmtTable> cTable(new FmtTable(info));
                    table->copyTo(cTable);
                    CreateDocument(cTable)->show();
                }
            }
        }
    }
}

void MainWindow::CopyTableToTmp()
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
            table->copyToAsTmp(cTable);
            CreateDocument(cTable)->show();
        }
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
        QSettings *settings = ((FmtApplication*)qApp)->settings();
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QString table = index.data(Qt::UserRole).toString();

        StartUnloadDbf(current, table, this, settings);
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
        StartLoadDbfSelectFile(current, table, this);
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
                CreateConnectionActio(info);
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

QMdiSubWindow *MainWindow::hasTableWindow(const quint64 &tableID)
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

FmtWorkWindow *MainWindow::currentWorkWindow()
{
    QMdiSubWindow *wnd = pMdi->activeSubWindow();

    if (wnd)
        return qobject_cast<FmtWorkWindow*>(wnd->widget());

    return Q_NULLPTR;
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
        {
            FmtWorkWindow *window = qobject_cast<FmtWorkWindow*>(wnd->widget());
            SetActiveFmtWindow(wnd);

            if (window)
                window->EditContent();
        }
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

void MainWindow::CompareTables()
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
            window->CompareStruct();
        }
    }
}

void MainWindow::GenDiffToScriptScript()
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
            window->DiffToScript();
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

void MainWindow::CreateFromXml()
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), "Файлы Fmt (*.xml);");
    if (!fileName.isEmpty())
    {
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->loadFromXml(fileName))
        {
            QMdiSubWindow *wnd = CreateDocument(table);
            wnd->show();
        }
    }
}

void MainWindow::CreateCheckUpdateRunnable()
{
    FmtApplication *app = (FmtApplication*)qApp;

    pUpdateChecker = new UpdateChecker();
    pUpdateChecker->setProgramName("RsWorkMaintenanceTool.exe");
    pUpdateChecker->setSettings(app->settings());

    pUpdateChecker->setAutoDelete(false);
    QThreadPool::globalInstance()->start(pUpdateChecker);

    connect(pUpdateChecker, &UpdateChecker::checkFinished, this, &MainWindow::UpdateCheckFinished);
}

void MainWindow::UpdateCheckFinished(bool hasUpdates, const CheckDataList &updatedata)
{
    if (!hasUpdates)
        pUpdateButton->setIcon(QIcon(":/img/base_globe_32.png"));
    else
    {
        pUpdateChecker->setCheckUpdateFlag(false);
        UpdateCheckerMessageBox dlg(this);
        dlg.setList(updatedata);
        dlg.exec();
        pUpdateChecker->setCheckUpdateFlag(true);
    }
}

void MainWindow::UpdateCheckStarted()
{
    pUpdateButton->setIcon(QIcon(":/img/base_globe_updates.png"));
}

/*void MainWindow::OnCreateQuery()
{
    QueryEditor *editor = new QueryEditor();
    QMdiSubWindow *wnd = CreateMdiWindow(editor, currentConnection());

    wnd->show();
}*/

void MainWindow::on_actionDebug_triggered()
{
    DebugConnect dlg(this);
    dlg.exec();
}

void MainWindow::OptionsAction()
{
    FmtApplication *app = (FmtApplication*)qApp;
    FmtOptionsDlg dlg(currentConnection(), app->settings(), this);
    dlg.exec();
}

void MainWindow::onUserActionTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());
    ConnectionInfo *current = currentConnection();

    if (!current || !action)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();

    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
        QSharedPointer<FmtTable> table(new FmtTable(current));

        if (table->load(index.data(Qt::UserRole).toString()))
        {
            QMdiSubWindow *sub = CreateDocument(table);
            sub->show();

            FmtWorkWindow *wnd = qobject_cast<FmtWorkWindow*>(sub->widget());
            wnd->execUserAction(action->data().toString());
        }
    }
}

const QList<ConnectionInfo*> &MainWindow::connections() const
{
    return m_pConnections;
}

const QMap<ConnectionInfo*, MainWindow::WorkWindowList> &MainWindow::windows() const
{
    return m_Windows;
}

MainWindow::WorkWindowList MainWindow::windows(ConnectionInfo* info) const
{
    if (m_Windows.contains(info))
        return m_Windows[info];

    return MainWindow::WorkWindowList();
}

void MainWindow::StartGuiConverter()
{
    QSettings *setting = settings();
    setting->beginGroup("GuiConverter");
    QString path = setting->value("path").toString();
    setting->endGroup();

    if (path.isEmpty())
    {
        FmtOptionsDlg dlg(currentConnection(), setting, this);
        OptionsPage *page = dlg.findPage<ExternalToolsPage*>();

        if (page)
        {
            int index = dlg.pageIndex(page);
            if (dlg.execPage(index) == QDialog::Accepted)
            {
                setting->beginGroup("GuiConverter");
                path = setting->value("path").toString();
                setting->endGroup();
            }
        }
    }

    if (path.isEmpty())
    {
        QMessageBox::critical(this, tr("GuiConverter"), tr("Не задан путь к GuiConverter.exe"));
        return;
    }

    GuiConverterDlg dlg(setting, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString err;
        if (StartGuiconverter(dlg.userScheme(),
                              dlg.userPwd(),
                              dlg.dbName(),
                              dlg.dsn(),
                              dlg.userNs(),
                              dlg.indxNs(),
                              dlg.sysScheme(),
                              dlg.sysPwd(),
                              dlg.ipAddr(),
                              &err))
        {
            QMessageBox::critical(this, tr("GuiConverter"), err);
        }
    }
}

void MainWindow::onSqlconerter()
{
    ConnectionInfo *connection = currentConnection();

    SqlConvertorDlg dlg(this);

    if (connection)
        dlg.setUserName(connection->user());

    dlg.exec();
}
