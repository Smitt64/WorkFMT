#include "fmtribbonmainwindow.h"
#include "connectioninfo.h"
#include "fmttable.h"
#include "fmttablelistdelegate.h"
#include "fmtworkwindow.h"
#include "options/externaltoolspage.h"
#include "options/fmtoptionsdlg.h"
#include "oracleauthdlg.h"
#include "src/core/MDIProxyStyle.h"
#include "src/debugconnect.h"
#include "src/widgets/guiconverterdlg.h"
#include "src/widgets/sqlconvertordlg.h"
#include "stringlistdlg.h"
#include "subwindowsmodel.h"
#include "tablesdock.h"
#include "recentconnectionlist.h"
#include "treecombobox.h"
#include <QStatusBar>
#include <toolsruntime.h>
#include <QSettings>
#include <QMenu>
#include <QDebug>
#include <QPaintEvent>

FmtRibbonMainWindow::FmtRibbonMainWindow(QWidget *parent) :
    SARibbonMainWindow(parent)
{
    setWindowIcon(QIcon(":/img/icon128.png"));
    setWindowTitle(tr("WorkFMT"));
    setMinimumSize(800, 600);

    pTablesDock = new TablesDock(tr("Таблицы"), this);
    addDockWidget(Qt::LeftDockWidgetArea, pTablesDock);

    pTableListDelegate = new FmtTableListDelegate(this);
    pTablesDock->setItemDelegate(pTableListDelegate);

    m_pMdiStyle = new MDIProxyStyle(qApp->style());
    setStyle(m_pMdiStyle);

    pMdi = new QMdiArea(this);
    pMdi->setDocumentMode(true);
    pMdi->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pMdi->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //pMdi->setStyle(m_pMdiStyle);
    setCentralWidget(pMdi);

    m_pStatusBar = new QStatusBar();
    setStatusBar(m_pStatusBar);

    InitWindowsCombo();
    InitMainRibbonTab();
    /*SARibbonCategory *viewPage = new SARibbonCategory("Вид");
    ribbon->addCategoryPage(viewPage);*/

    InitQuickAccessBar();

    connect(pTablesDock, &TablesDock::tableDbClicked, this, &FmtRibbonMainWindow::TableClicked);
}

FmtRibbonMainWindow::~FmtRibbonMainWindow()
{

}


void FmtRibbonMainWindow::OpenConnection(const QString &connectionString)
{
 // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void FmtRibbonMainWindow::InitQuickAccessBar()
{
    SARibbonQuickAccessBar* quickAccessBar = ribbonBar()->quickAccessBar();

    RecentConnectionList list;
    if (list.load() && !list.isEmpty())
    {
        QMenu* RecentMenu = new QMenu(tr("Недавние файлы"), this);
        RecentMenu->setIcon(QIcon::fromTheme("History"));

        toolAddActionWithTooltip(RecentMenu, tr("Список последних открытых подключений"));

        int count = list.rowCount();
        for (int i = 0; i < count; i++)
        {
            RecentList item = list.record(i);
            QAction *action = RecentMenu->addAction(RecentConnectionList::connectionName(item));
            action->setData(QVariant::fromValue(item));
            action->setToolTip(RecentConnectionList::connectionToolTip(item));
            //connect(action, SIGNAL(triggered(bool)), SLOT(OpenRecentConnection()));
        }

        quickAccessBar->addMenu(RecentMenu, Qt::ToolButtonIconOnly, QToolButton::InstantPopup);
    }
}

void FmtRibbonMainWindow::InitMainRibbonTab()
{
    SARibbonBar* ribbon = ribbonBar();

    SARibbonCategory *mainPage = new SARibbonCategory("Главная");
    ribbon->addCategoryPage(mainPage);

    SARibbonPannel* connectionPannel = new SARibbonPannel(tr("Подключение"));
    mainPage->addPannel(connectionPannel);

    m_pActionConnect = createAction(tr("Новое подключение"), "ConnectToDatabase", QKeySequence::Open);
    connectionPannel->addLargeAction(m_pActionConnect);

    m_pOpenConnection = createAction(tr("Открыть подключение"), "OpenDatabase");
    connectionPannel->addSmallAction(m_pOpenConnection);

    m_pActionDisconnect = createAction(tr("Закрыть подключение"), "DatabaseOffline");
    connectionPannel->addSmallAction(m_pActionDisconnect);

    m_pSqliteAction = createAction(tr("Выгрузить в sqlite базу"), "Export");
    connectionPannel->addSmallAction(m_pSqliteAction);

    SARibbonPannel* ImportExportPannel = new SARibbonPannel(tr("Импорт/Экспорт"));
    mainPage->addPannel(ImportExportPannel);

    m_pActionImport = createAction(tr("Импорт из xml файлов"), "ImportPackage");
    ImportExportPannel->addSmallAction(m_pActionImport);

    m_pActionImportDir = createAction(tr("Импорт xml из каталога"), "ImportCatalogPart");
    ImportExportPannel->addSmallAction(m_pActionImportDir);

    m_pActionExport = createAction(tr("Экспорт в xml файл"), "ExportTableToFile");
    ImportExportPannel->addSmallAction(m_pActionExport);

    // Создать запись
    SARibbonPannel* TablePannel = new SARibbonPannel(tr("Таблицы"));
    mainPage->addPannel(TablePannel);

    m_pMenuCreate = new QMenu(this);
    m_pMenuCreate->setIcon(QIcon::fromTheme("CreateTable"));
    m_pMenuCreate->setTitle(tr("Создать запись"));

    m_pActionCreate = createAction(tr("Создать запись"), "CreateTable", QKeySequence::New);
    m_pMenuCreate->addAction(m_pActionCreate);
    m_pMenuCreate->setDefaultAction(m_pActionCreate);

    m_pActionCreateText = createAction(tr("Создать из текста"), "CreateTableFromText", Qt::SHIFT + Qt::CTRL + Qt::Key_N);
    m_pMenuCreate->addAction(m_pActionCreateText);

    m_pActionCreateXml = createAction(tr("Cоздать из xml"), "CreateTableFromXml");
    m_pMenuCreate->addAction(m_pActionCreateXml);

    m_pActionCreateGroup = TablePannel->addLargeMenu(m_pMenuCreate, QToolButton::MenuButtonPopup);

    m_pActionCopyTable = createAction(tr("Копировать таблицу"), "CopyTable");
    TablePannel->addSmallAction(m_pActionCopyTable);

    m_pActionCopyTableTmp = createAction(tr("Копировать как временную"), "CopyTableTmp");
    TablePannel->addSmallAction(m_pActionCopyTableTmp);

    m_pActionCopyTableTo = createAction(tr("Копировать таблицу в..."), "CopyTableTo");
    TablePannel->addSmallAction(m_pActionCopyTableTo);

    SARibbonPannel *ConnectionsGallary = mainPage->addPannel(tr("Активные подключения"));
    m_pConnectionsGallery = ConnectionsGallary->addGallery();
    m_pConnectionsGalleryGroup = m_pConnectionsGallery->addCategoryActions(tr("Подключения"), {});
    m_pConnectionsGalleryGroup->setGridMinimumWidth(128);
    m_pConnectionsGallery->currentViewGroup()->setGridMinimumWidth(128);

    SARibbonPannel* ToolsPannel = new SARibbonPannel(tr("Инструменты"));
    mainPage->addPannel(ToolsPannel);

    m_pActionDiffTool = createAction(tr("Запустить Diff to Script"), "DiffToScript");
    ToolsPannel->addLargeAction(m_pActionDiffTool);

    m_pActionDebug = createAction(tr("Настроить отлаку на схеме"), "DebugXSLT");
    ToolsPannel->addMediumAction(m_pActionDebug);

    m_pActionGuiConverter = createAction(tr("Запустить GuiConverter"), "GuiConverter");
    ToolsPannel->addMediumAction(m_pActionGuiConverter);

    m_pActionDumpTool = createAction(tr("Обработка дампов"), "DumpTool");
    ToolsPannel->addMediumAction(m_pActionDumpTool);

    m_pActionConvertScript = createAction(tr("Конвертировать скрипт"), "TransferStoredProcedure");
    ToolsPannel->addMediumAction(m_pActionConvertScript);

    connect(m_pActionConnect, &QAction::triggered, this, &FmtRibbonMainWindow::ActionConnectTriggered);
    connect(m_pOpenConnection, &QAction::triggered, this, &FmtRibbonMainWindow::OpenConnectionFile);
    connect(m_pActionGuiConverter, &QAction::triggered, this, &FmtRibbonMainWindow::StartGuiConverter);

    QDir dir = QApplication::applicationDirPath();
    if (!QFile::exists(dir.absoluteFilePath("DumpTool.exe")))
        dir = QDir::current();

    if (!QFile::exists(dir.absoluteFilePath("DumpTool.exe")))
        m_pActionDumpTool->setEnabled(false);

    if (!QFile::exists(dir.absoluteFilePath("DiffToScript.exe")))
        m_pActionDiffTool->setEnabled(false);

    connect(m_pActionDiffTool, &QAction::triggered, [=]()
    {
        QProcess::startDetached(dir.absoluteFilePath("DiffToScript.exe"), QStringList());
    });

    connect(m_pActionDumpTool, &QAction::triggered, [=]()
    {
        QProcess::startDetached(dir.absoluteFilePath("DumpTool.exe"), QStringList());
    });

    connect(m_pActionDebug, &QAction::triggered, [=]()
    {
        DebugConnect dlg(this);
        dlg.setWindowIcon(QIcon::fromTheme("DebugXSLT"));
        dlg.exec();
    });

    connect(m_pActionConvertScript, &QAction::triggered, [=]()
    {
        ConnectionInfo *connection = currentConnection();

        SqlConvertorDlg dlg(this);
        dlg.setWindowIcon(QIcon::fromTheme("TransferStoredProcedure"));

        if (connection)
            dlg.setUserName(connection->user());

        dlg.exec();
    });
}

void FmtRibbonMainWindow::setupAction(QAction *act, const QString& text, const QString& iconname, const QKeySequence &key)
{
    act->setText(text);
    act->setIcon(QIcon::fromTheme(iconname));
    act->setObjectName(text);
    act->setShortcut(key);
}

void FmtRibbonMainWindow::ActionConnectTriggered()
{
    (void)openConnection();
}

ConnectionInfo* FmtRibbonMainWindow::openConnection()
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

QAction *FmtRibbonMainWindow::CreateConnectionActio(ConnectionInfo *info)
{
    QAction *a = new QAction(this);
    a->setText(info->schemeName());
    a->setCheckable(true);
    //ui->tabToolBar->addAction(a);
    //m_ConnectionsGroup->addAction(a);
    a->setChecked(true);

    m_pConnectionsGalleryGroup->addActionItem(a);
    int id = m_pConnectionsGalleryGroup->actionGroup()->actions().indexOf(a);
    if (id != -1)
    {
        QModelIndex idx = m_pConnectionsGalleryGroup->model()->index(id, 0);
        m_pConnectionsGalleryGroup->setCurrentIndex(idx);
        m_pConnectionsGallery->currentViewGroup()->setCurrentIndex(idx);
    }
    //m_pConnectionsGalleryGroup->setCurrentIndex()

    //QSize toolBarIconSize = ui->tabToolBar->iconSize();
    a->setIcon(info->colorIcon(QSize(128, 128)));
    a->setData(reinterpret_cast<qintptr>(info));
    pWindowsModel->addConnection(info);

    info->updateFmtList();
    m_pConnections.append(info);
    pTablesDock->setModel(info->tablesModel());
    pTablesDock->setConnection(info);

    //UpdateActions();

    return a;
}

void FmtRibbonMainWindow::OpenConnectionFile()
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

void FmtRibbonMainWindow::InitWindowsCombo()
{
    SARibbonSystemButtonBar* wbar = windowButtonBar();

    pWindowsComboBox = new TreeComboBox(this);
    pWindowsComboBox->setMinimumWidth(250);
    pWindowsModel = new SubWindowsModel(this);

    pWindowsComboBox->setModel(pWindowsModel);
    pWindowsComboBox->setMaxVisibleItems(15);

    connect(pWindowsModel, SIGNAL(windowsUpdated()), pWindowsComboBox->view(), SLOT(expandAll()));
    connect(pWindowsComboBox, SIGNAL(modelIndexChanged(QModelIndex)), SLOT(subWindowIndexChanged(QModelIndex)));

    wbar->addWidget(pWindowsComboBox);
}

void FmtRibbonMainWindow::showEvent(QShowEvent *event)
{
    SARibbonBar* ribbon = ribbonBar();
    SARibbonMainWindow::showEvent(event);
    setRibbonTheme(SARibbonTheme::RibbonThemeOffice2013);

    QFile file;
    file.setFileName("://theme-office2013-green.qss");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString qss = QString::fromUtf8(file.readAll());
    setStyleSheet(qss);
    ribbon->setTabBarBaseLineColor(QColor(33, 115, 70));
    ribbon->setWindowTitleTextColor(QColor(33, 115, 70));

    setContentsMargins(2,2,2,2);

    update();
}

void FmtRibbonMainWindow::StartGuiConverter()
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

ConnectionInfo *FmtRibbonMainWindow::currentConnection()
{
    ConnectionInfo *cur = Q_NULLPTR;

    QList<QAction*> ConnectionsGroup = m_pConnectionsGalleryGroup->actionGroup()->actions();
    foreach (QAction *act, ConnectionsGroup)
    {
        if (act->isChecked())
            cur = reinterpret_cast<ConnectionInfo*>(act->data().toInt());
    }

    return cur;
}

void FmtRibbonMainWindow::SetActiveFmtWindow(QMdiSubWindow *wnd)
{
    wnd->setWindowState(Qt::WindowNoState);
    pMdi->setActiveSubWindow(wnd);
}

QMdiSubWindow *FmtRibbonMainWindow::HasTableWindow(const QString &tableName)
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

QMdiSubWindow *FmtRibbonMainWindow::HasTableWindow(const quint64 &tableID)
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

QMdiSubWindow *FmtRibbonMainWindow::CreateMdiWindow(MdiSubInterface *window, ConnectionInfo *pConnection)
{
    m_Windows[pConnection].push_back(window);
    QMdiSubWindow *wnd = new QMdiSubWindow();  OfficeMdiWindow
    wnd->setWidget(window);
    pMdi->addSubWindow(wnd);

    wnd->setAttribute(Qt::WA_DeleteOnClose);
    window->setParentWnd(wnd);
    window->setConnection(pConnection);
    wnd->setWindowTitle(window->makeWindowTitle());
    connect(window, SIGNAL(destroyed(QObject*)), SLOT(WorkWindowDestroyed(QObject*)));

    QModelIndex index = pWindowsModel->addWindow(pConnection, wnd);
    pWindowsComboBox->setCurrentIndex(index);

    wnd->setMask(QRect());

    return wnd;
}

QMdiSubWindow *FmtRibbonMainWindow::CreateDocument(QSharedPointer<FmtTable> &table, FmtWorkWindow **pWindow)
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

void FmtRibbonMainWindow::TableClicked(const quint32 &id)
{
    ConnectionInfo *current = currentConnection();

    if (!current)
        return;

    QMdiSubWindow *wnd = HasTableWindow(id);
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
