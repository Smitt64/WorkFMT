#include "fmtribbonmainwindow.h"
#include "connectioninfo.h"
#include "errordlg.h"
#include "errorsmodel.h"
#include "fmtfromrichtext.h"
#include "fmtimpexpwrp.h"
#include "fmttable.h"
#include "fmttablelistdelegate.h"
#include "fmtworkwindow.h"
#include "options/externaltoolspage.h"
#include "options/fmtoptionsdlg.h"
#include "oracleauthdlg.h"
#include "selectconnectiondlg.h"
#include "selectfolderdlg.h"
#include "src/debugconnect.h"
#include "src/widgets/guiconverterdlg.h"
#include "src/widgets/sqlconvertordlg.h"
#include "stringlistdlg.h"
#include "subwindowsmodel.h"
#include "tablesdock.h"
#include "recentconnectionlist.h"
#include "tablesdockwidget.h"
#include "treecombobox.h"
#include <QStatusBar>
#include <toolsruntime.h>
#include <QSettings>
#include <QMenu>
#include <QDebug>
#include <QPaintEvent>
#include <QTreeView>

FmtRibbonMainWindow::FmtRibbonMainWindow(QWidget *parent) :
    SARibbonMainWindow(parent),
    m_LastActiveWindow(nullptr),
    pTablesDock(nullptr)
{
    setWindowIcon(QIcon("://app-icon.svg"));
    setWindowTitle(tr("WorkFMT"));
    setMinimumSize(800, 600);

    pTablesDock = new TablesDock(tr("Таблицы"), this);
    addDockWidget(Qt::LeftDockWidgetArea, pTablesDock);

    pTableListDelegate = new FmtTableListDelegate(this);
    pTablesDock->setItemDelegate(pTableListDelegate);
    pTablesDock->setEventFilter(this);

    //m_pMdiStyle = new MDIProxyStyle(qApp->style());
    //setStyle(m_pMdiStyle);
    //qApp->setStyle(m_pMdiStyle);

    pMdi = new QMdiArea(this);
    pMdi->setDocumentMode(true);
    pMdi->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pMdi->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pMdi->setBackground(QColor("#EBEFF2"));
    //pMdi->setStyle(m_pMdiStyle);
    setCentralWidget(pMdi);

    m_pStatusBar = new QStatusBar();
    setStatusBar(m_pStatusBar);

    InitWindowsCombo();
    InitMainRibbonTab();
    /*SARibbonCategory *viewPage = new SARibbonCategory("Вид");
    ribbon->addCategoryPage(viewPage);*/

    InitContextCategoryes();
    InitQuickAccessBar();

    connect(pTablesDock, &TablesDock::tableDbClicked, this, &FmtRibbonMainWindow::TableClicked);
    connect(pTablesDock, &TablesDock::selectionChanged, this, &FmtRibbonMainWindow::UpdateActions);

    connect(pMdi, &QMdiArea::subWindowActivated, [=](QMdiSubWindow *window)
    {
        // subWindowActivated
        if (!window)
        {
            if (m_LastActiveWindow)
            {
                MdiSubInterface *lastwnd = dynamic_cast<MdiSubInterface*>(m_LastActiveWindow->widget());

                if (lastwnd)
                {
                    QList<QWidget*> status = lastwnd->statusBarSections();
                    lastwnd->clearRibbonTabs();

                    for (auto widget : qAsConst(status))
                        m_pStatusBar->removeWidget(widget);
                }
            }

            QList<SARibbonContextCategory*> allCategoryes = ribbonBar()->contextCategoryList();
            for (auto all : qAsConst(allCategoryes))
            {
                if (all->categoryCount())
                    ribbonBar()->showContextCategory(all);
                else
                    ribbonBar()->hideContextCategory(all);
            }

            m_LastActiveWindow = nullptr;

            return;
        }

        if (m_LastActiveWindow == window)
        {
            return;
        }

        MdiSubInterface *lastwnd = nullptr;
        MdiSubInterface *wnd = qobject_cast<MdiSubInterface*>(window->widget());

        if (m_LastActiveWindow)
            lastwnd = dynamic_cast<MdiSubInterface*>(m_LastActiveWindow->widget());

        if (!wnd)
        {
            /*m_ToolBoxDock->setModel(nullptr);
            m_PropertyDock->setPropertyModel(nullptr);
            m_PropertyDock->setStructModel(nullptr);*/

            /*m_pActionUndo->setSource(nullptr);
            m_pActionRedo->setSource(nullptr);
            m_pUndoActionWidget->setUndoStack(nullptr);*/

            if (lastwnd)
            {
                QList<QWidget*> status = lastwnd->statusBarSections();
                lastwnd->clearRibbonTabs();

                for (auto widget : qAsConst(status))
                    m_pStatusBar->removeWidget(widget);
            }

            m_LastActiveWindow = nullptr;
        }
        else
        {
            /*m_ToolBoxDock->setModel(wnd->toolBox());
            m_PropertyDock->setPropertyModel(wnd->propertyModel());
            m_PropertyDock->setStructModel(wnd->structModel());*/

            /*m_pActionUndo->setSource(wnd->undoAction());
            m_pActionRedo->setSource(wnd->redoAction());
            m_pUndoActionWidget->setUndoStack(wnd->undoStack());*/

            QModelIndex index = pWindowsModel->findWindow(wnd->connection(), window);
            pWindowsComboBox->setCurrentIndex(index);

            m_LastRibbonTabName.lock();

            ribbonBar()->setUpdatesEnabled(false);
            if (lastwnd)
            {
                QList<QWidget*> status = lastwnd->statusBarSections();
                lastwnd->clearRibbonTabs();

                for (auto widget : qAsConst(status))
                    m_pStatusBar->removeWidget(widget);
            }

            wnd->updateRibbonTabs();

            if (!m_LastRibbonTabName.get().isEmpty())
                ribbonBar()->raiseCategory(ribbonBar()->categoryByName(m_LastRibbonTabName));

            QList<SARibbonContextCategory*> allCategoryes = ribbonBar()->contextCategoryList();
            for (auto all : qAsConst(allCategoryes))
            {
                if (all->categoryCount())
                    ribbonBar()->showContextCategory(all);
                else
                    ribbonBar()->hideContextCategory(all);
            }
            ribbonBar()->setUpdatesEnabled(true);

            QList<QWidget*> status = wnd->statusBarSections();
            for (auto widget : qAsConst(status))
            {
                m_pStatusBar->addPermanentWidget(widget);
                widget->show();
            }

            m_LastActiveWindow = window;
            m_LastRibbonTabName.unlock();
        }
        /*if (wnd)
        {
            QModelIndex index = pWindowsModel->findWindow(wnd->connection(), window);
            pWindowsComboBox->setCurrentIndex(index);
        }*/
    });

    UpdateActions();
}

FmtRibbonMainWindow::~FmtRibbonMainWindow()
{

}

void FmtRibbonMainWindow::OpenConnection(const QString &connectionString)
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

void FmtRibbonMainWindow::InitQuickAccessBar()
{
    SARibbonButtonGroupWidget* rightBar = ribbonBar()->rightButtonGroup();
    SARibbonQuickAccessBar* quickAccessBar = ribbonBar()->quickAccessBar();

    RecentConnectionList list;
    if (list.load() && !list.isEmpty())
    {
        QMenu* RecentMenu = new QMenu(tr("Недавние подключения"), this);
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

    pSearchLine = new SARibbonLineEdit(this);
    pSearchLine->setPlaceholderText(tr("Введите текст для поиска..."));
    pSearchLine->setFrame(false);
    pSearchLine->setMinimumWidth(350);
    pSearchLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    pSearchLine->setClearButtonEnabled(true);

    rightBar->addWidget(pSearchLine);

    connect(pSearchLine, SIGNAL(textChanged(QString)), pTableListDelegate, SLOT(setHighlightText(QString)));
    connect(pSearchLine, SIGNAL(textChanged(QString)), pTablesDock, SLOT(forceRepaint()));
}

void FmtRibbonMainWindow::InitMainRibbonTab()
{
    SARibbonBar* ribbon = ribbonBar();
    ribbon->applicationButton()->setText(tr("Файл"));
    ribbon->applicationButton()->setMinimumWidth(60);

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

    m_pActionEdit = createAction(tr("Редактировать"), "EditTableRow");
    m_pFakeActionInit = createAction(tr("Создать в БД"), "TableAdapter");
    m_pFakeUnloadToDbf = createAction(tr("Выгрузить в *.dat"), "UnloadDbf");
    m_pFakeLoadFromDbf = createAction(tr("Загрузить из *.dat"), "LoadDbf");
    m_pFakeCreateTablesSql = createAction(tr("Экспорт CreateTablesSql"), "CreateTablesSql");
    m_pFakeCreateDiffToScript = createAction(tr("Скрипт изменений (Diff To Script)"), "DiffToScript");
    m_pActionDeleteFmt = createAction(tr("Удалить запись"), "DeleteTable");

    auto FuncActionCreate = [=]()
    {
        ConnectionInfo *current = CurrentConnection();

        if (current)
        {
            QSharedPointer<FmtTable> table(new FmtTable(current));
            CreateDocument(table)->show();
        }
    };

    connect(m_pFakeActionInit, &QAction::triggered, [=](){ ExecuteCurrentFmtWindowSlot("InitDB"); });
    connect(m_pFakeUnloadToDbf, &QAction::triggered, [=](){ ExecuteCurrentFmtWindowSlot("UnloadToDbf"); });
    connect(m_pFakeLoadFromDbf, &QAction::triggered, [=](){ ExecuteCurrentFmtWindowSlot("LoadFromDbf"); });
    connect(m_pFakeCreateTablesSql, &QAction::triggered, [=](){ ExecuteCurrentFmtWindowSlot("CreateTableSql"); });
    connect(m_pFakeCreateDiffToScript, &QAction::triggered, [=](){ ExecuteCurrentFmtWindowSlot("DiffToScript", true); });

    connect(m_pActionCreateGroup, &QAction::triggered, FuncActionCreate);
    connect(m_pActionCreate, &QAction::triggered, FuncActionCreate);

    connect(m_pActionConnect, &QAction::triggered, this, &FmtRibbonMainWindow::ActionConnectTriggered);
    connect(m_pActionDisconnect, &QAction::triggered, this, &FmtRibbonMainWindow::DisconnectCurrent);
    connect(m_pOpenConnection, &QAction::triggered, this, &FmtRibbonMainWindow::OpenConnectionFile);
    connect(m_pActionGuiConverter, &QAction::triggered, this, &FmtRibbonMainWindow::StartGuiConverter);
    connect(m_pSqliteAction, &QAction::triggered, this, &FmtRibbonMainWindow::UnloadSqlite);
    connect(m_pConnectionsGallery, &SARibbonGallery::triggered, this, &FmtRibbonMainWindow::ConnectionActionSelected);
    connect(m_pActionImportDir, &QAction::triggered, this, &FmtRibbonMainWindow::ImpDirAction);
    connect(m_pActionImport, &QAction::triggered, this, &FmtRibbonMainWindow::ImportAction);
    connect(m_pActionExport, &QAction::triggered, this, &FmtRibbonMainWindow::ExportTableXml);
    connect(m_pActionEdit, &QAction::triggered, this, &FmtRibbonMainWindow::ActionEditFmt);

    connect(m_pActionCopyTable, &QAction::triggered, this, &FmtRibbonMainWindow::CopyTable);
    connect(m_pActionCopyTableTmp, &QAction::triggered, this, &FmtRibbonMainWindow::CopyTableToTmp);
    connect(m_pActionCopyTableTo, &QAction::triggered, this, &FmtRibbonMainWindow::CopyTableTo);

    QDir dir = QApplication::applicationDirPath();
    if (!QFile::exists(dir.absoluteFilePath("DumpTool.exe")))
        dir = QDir::current();

    connect(m_pActionDiffTool, &QAction::triggered, [=]()
    {
        QProcess::startDetached(dir.absoluteFilePath("DiffToScript.exe"), QStringList());
    });

    connect(m_pActionDeleteFmt, &QAction::triggered, [=]()
    {
        ConnectionInfo *current = CurrentConnection();

        if (!current)
            return;

        QListView *view = pTablesDock->tablesWidget()->listView();
        if (view->selectionModel()->hasSelection())
        {
            QModelIndex index = view->selectionModel()->selectedIndexes().at(0);
            QSharedPointer<FmtTable> table(new FmtTable(current));

            if (table->load(index.data(Qt::UserRole).toString()))
            {
                // Запрашиваем подтверждение перед удалением
                QString tableName = table->name();
                QMessageBox::StandardButton reply = QMessageBox::question(
                    this,
                    tr("Подтверждение удаления"),
                    tr("Вы действительно хотите удалить запись о таблице <b>%1</b> из FMT словаря?").arg(tableName),
                    QMessageBox::Yes | QMessageBox::No
                    );

                if (reply == QMessageBox::Yes)
                {
                    if (!table->removeFmtTable())  // Исправлено: ! для проверки неудачи
                    {
                        QMessageBox::information(this, tr("Успешно"), tr("Запись о таблице <b>%1</b> удалена из FMT словаря").arg(tableName));
                        current->updateFmtList();
                    }
                    else
                    {
                        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось удалить запись о таблице <b>%1</b> из FMT словаря").arg(tableName));
                    }
                }
            }
        }
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
        ConnectionInfo *connection = CurrentConnection();

        SqlConvertorDlg dlg(this);
        dlg.setWindowIcon(QIcon::fromTheme("TransferStoredProcedure"));

        if (connection)
            dlg.setUserName(connection->user());

        dlg.exec();
    });

    connect(m_pActionCreateText, &QAction::triggered, [=]()
    {
        ConnectionInfo *current = CurrentConnection();

        if (!current)
            return;

        FmtFromRichText wizard(current, this);
        if (wizard.exec() == QDialog::Accepted)
        {
            QSharedPointer<FmtTable> table = wizard.fmtTable();
            QMdiSubWindow *wnd = CreateDocument(table);
            wnd->show();
        }
    });

    connect(m_pActionCreateXml, &QAction::triggered, [=]()
    {
        ConnectionInfo *current = CurrentConnection();

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
    });
}

void FmtRibbonMainWindow::InitContextCategoryes()
{
    SARibbonContextCategory *rescat = ribbonBar()->addContextCategory(FMTTABLE_CONTEXTCATEGORY, QColor(0xBFFFBF), 0);
    //SARibbonContextCategory *controlcat = ribbon->addContextCategory(tr("Элемент"), QColor(0xC7FFFF), 0);
}

void FmtRibbonMainWindow::setupAction(QAction *act, const QString& text, const QString& iconname, const QKeySequence &key)
{
    act->setText(text);
    act->setIcon(QIcon::fromTheme(iconname));
    act->setObjectName(text);
    act->setShortcut(key);
}

void FmtRibbonMainWindow::CopyTable()
{
    ConnectionInfo *current = CurrentConnection();

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

void FmtRibbonMainWindow::CopyTableTo()
{
    ConnectionInfo *current = CurrentConnection();

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

void FmtRibbonMainWindow::CopyTableToTmp()
{
    ConnectionInfo *current = CurrentConnection();

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

ConnectionInfo *FmtRibbonMainWindow::CurrentConnection()
{
    QActionGroup* group = m_pConnectionsGalleryGroup->actionGroup();

    if (!m_pConnectionsGallery->currentViewGroup()->currentIndex().isValid())
        return nullptr;

    int index = m_pConnectionsGallery->currentViewGroup()->currentIndex().row();

    QAction *action = group->actions().at(index);
    ConnectionInfo *current = reinterpret_cast<ConnectionInfo*>(action->data().toInt());

    return current;
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

    UpdateActions();

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

    connect(pWindowsModel, &SubWindowsModel::windowsUpdated, (QTreeView*)pWindowsComboBox->view(), &QTreeView::expandAll);
    connect(pWindowsComboBox, &TreeComboBox::modelIndexChanged, [=](const QModelIndex &index)
    {
        // subWindowIndexChanged
        QMdiSubWindow *wnd = pWindowsModel->window(index);

        if (wnd)
            SetActiveFmtWindow(wnd);
    });
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

    QSettings *s = settings();
    restoreGeometry(s->value("Geometry").toByteArray());
    restoreState(s->value("State").toByteArray());

    update();
}

void FmtRibbonMainWindow::closeEvent(QCloseEvent *event)
{
    SARibbonMainWindow::closeEvent(event);

    QSettings *s = settings();
    s->setValue("Geometry", saveGeometry());
    s->setValue("State", saveState());
    //pUpdateChecker->requestInterruption();
    //pUpdateChecker->deleteLater();
    event->accept();
}

void FmtRibbonMainWindow::StartGuiConverter()
{
    QSettings *setting = settings();
    setting->beginGroup("GuiConverter");
    QString path = setting->value("path").toString();
    setting->endGroup();

    if (path.isEmpty())
    {
        FmtOptionsDlg dlg(CurrentConnection(), setting, this);
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

void FmtRibbonMainWindow::SetActiveFmtWindow(QMdiSubWindow *wnd)
{
    wnd->setWindowState(Qt::WindowNoState);
    pMdi->setActiveSubWindow(wnd);
}

QMdiSubWindow *FmtRibbonMainWindow::HasTableWindow(const QString &tableName)
{
    QMdiSubWindow *find = Q_NULLPTR;
    QList<QWidget*> lst = m_Windows[CurrentConnection()];

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
    QList<QWidget*> lst = m_Windows[CurrentConnection()];

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
    QMdiSubWindow *wnd = new QMdiSubWindow();
    wnd->setWidget(window);
    pMdi->addSubWindow(wnd);

    wnd->setAttribute(Qt::WA_DeleteOnClose);
    window->setParentWnd(wnd);
    window->setConnection(pConnection);
    window->setRibbonBar(ribbonBar());
    //window->setPalette(m_pMdiStyle->standardPalette());

    //window->setPalette(m_pMdiStyle->standardPalette());

    connect(window, &MdiSubInterface::destroyed, [=](QObject *wnd)
    {
        // WorkWindowDestroyed
        const FmtWorkWindow *window = static_cast<const FmtWorkWindow*>(wnd);

        if (window)
        {
            ConnectionInfo *connection = window->connection();

            if (m_Windows[connection].contains((QWidget*)window))
                m_Windows[connection].removeOne((QWidget*)window);
        }
    });

    QModelIndex index = pWindowsModel->addWindow(pConnection, wnd);
    pWindowsComboBox->setCurrentIndex(index);

    wnd->setMask(QRect());

    return wnd;
}

QMdiSubWindow *FmtRibbonMainWindow::CreateDocument(QSharedPointer<FmtTable> &table, FmtWorkWindow **pWindow)
{
    FmtWorkWindow *window = new FmtWorkWindow;
    //m_Windows[table->connection()].push_back(window);
    QMdiSubWindow *wnd = CreateMdiWindow(window, table->connection());

    window->setParentWnd(wnd);
    window->setFmtTable(table);
    wnd->setWindowTitle(window->makeWindowTitle());

    connect(window, &FmtWorkWindow::accepted, wnd, &FmtRibbonMainWindow::deleteLater);
    connect(window, &FmtWorkWindow::rejected, wnd, &FmtRibbonMainWindow::deleteLater);
    connect(window, &FmtWorkWindow::needUpdateTableList, [=]()
    {
        // OnTableChangeUpdtList
        TablesDockWidget *list = pTablesDock->tablesWidget();

        if (list->isFiltered())
            list->updateList();
    });

    if (pWindow)
        *pWindow = window;

    return wnd;
}

void FmtRibbonMainWindow::TableClicked(const quint32 &id)
{
    UpdateActions();

    ConnectionInfo *current = CurrentConnection();

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

void FmtRibbonMainWindow::UnloadSqlite()
{
    ConnectionInfo *current = CurrentConnection();

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


void FmtRibbonMainWindow::UpdateActions()
{
    ConnectionInfo *cur = CurrentConnection();

    bool HasConnection = cur ? true : false;
    bool CanSaveToXml = HasConnection && cur->hasFeature(ConnectionInfo::CanSaveToXml);
    bool HasSelectedTable = false;

    if (pTablesDock)
    {
        QListView *view = pTablesDock->tablesWidget()->listView();
        if (view && view->selectionModel() && view->selectionModel()->hasSelection())
            HasSelectedTable = true;
    }

    m_pActionDisconnect->setEnabled(HasConnection);
    m_pSqliteAction->setEnabled(HasConnection);
    m_pActionImport->setEnabled(HasConnection);
    m_pActionExport->setEnabled(CanSaveToXml && HasSelectedTable);
    m_pActionImportDir->setEnabled(HasConnection);

    m_pActionCreateGroup->setEnabled(HasConnection);
    m_pActionCopyTable->setEnabled(HasConnection && HasSelectedTable);
    m_pActionCopyTableTmp->setEnabled(HasConnection && HasSelectedTable);
    m_pActionCopyTableTo->setEnabled(HasConnection && HasSelectedTable);

    m_pActionDebug->setEnabled(HasConnection && cur->type() == ConnectionInfo::CON_ORA);

    QDir dir = QApplication::applicationDirPath();
    if (!QFile::exists(dir.absoluteFilePath("DumpTool.exe")))
        dir = QDir::current();

    if (!QFile::exists(dir.absoluteFilePath("DumpTool.exe")))
        m_pActionDumpTool->setEnabled(false);

    if (!QFile::exists(dir.absoluteFilePath("DiffToScript.exe")))
        m_pActionDiffTool->setEnabled(false);

    if (!cur)
        return;

    /*if (cur->type() != ConnectionInfo::CON_ORA)
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
    }*/
}

void FmtRibbonMainWindow::ConnectionActionSelected(QAction *action)
{
    ConnectionInfo *info = reinterpret_cast<ConnectionInfo*>(action->data().toInt());
    pTablesDock->setConnection(info);

    UpdateActions();
}

void FmtRibbonMainWindow::DisconnectCurrent()
{
    QActionGroup* group = m_pConnectionsGalleryGroup->actionGroup();
    int index = m_pConnectionsGallery->currentViewGroup()->currentIndex().row();

    QAction *action = group->actions().at(index);
    ConnectionInfo *current = reinterpret_cast<ConnectionInfo*>(action->data().toInt());

    if (!current)
        return;

    bool NeedClose = true;
    if (!m_Windows[current].isEmpty())
    {
        if (QMessageBox::question(this, tr("Закрытие соеденения."), tr("Имеются незакрытые окна, работающие с соеденением <b>%1</b>. Завершить подключение?")
                                  .arg(current->schemeName()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::No
        )
            NeedClose = false;
    }

    if (!NeedClose)
        return;

    current->db().close();
    pTablesDock->closeConnection();
    group->removeAction(action);

    SARibbonGalleryItem *GallaryItem = m_pConnectionsGallery->currentViewGroup()->groupModel()->take(index);
    delete GallaryItem;

    QList<QMdiSubWindow*> windows = pMdi->subWindowList();
    foreach (QMdiSubWindow *w, windows)
    {
        FmtWorkWindow *doc = (FmtWorkWindow*)w->widget();
        if (doc->connection() == current)
        {
            doc->clearRibbonTabs();
            w->deleteLater();
        }
    }

    QList<QAction*> actions = group->actions();
    if (!actions.isEmpty())
    {
        actions[0]->setChecked(true);
        ConnectionActionSelected(actions[0]);
    }

    pWindowsModel->removeConnection(current);
    m_pConnectionsGallery->update();
    m_pConnectionsGalleryGroup->update();

    delete action;
    delete current;

    UpdateActions();
}

void FmtRibbonMainWindow::ImpDirAction()
{
    ConnectionInfo *current = CurrentConnection();

    if (!current)
        return;

    if (!CheckConnectionType(current, { ConnectionInfo::CON_ORA, ConnectionInfo::CON_POSTGRESQL }, true, this))
        return;

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

void FmtRibbonMainWindow::ImportAction()
{
    ConnectionInfo *current = CurrentConnection();
    if (!current)
        return;

    if (!CheckConnectionType(current, { ConnectionInfo::CON_ORA, ConnectionInfo::CON_POSTGRESQL }, true, this))
        return;

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

void FmtRibbonMainWindow::ExportTableXml()
{
    ConnectionInfo *current = CurrentConnection();

    if (!current)
        return;

    if (!CheckConnectionType(current, { ConnectionInfo::CON_ORA, ConnectionInfo::CON_POSTGRESQL }, true, this))
        return;

    QString table = m_pActionExport->data().toString();

    SelectFolderDlg folder(RsFmtUnlDirContext, tr("Экспорт в xml файл"), this);
    if (folder.exec() == QDialog::Accepted)
    {
        ExportFmtToXml(current, QStringList()
                       << table, folder.selectedPath(), true, true, this);
    }
}

void FmtRibbonMainWindow::ActionEditFmt()
{
    ConnectionInfo *current = CurrentConnection();

    if (!current)
        return;

    QString ntable = m_pActionEdit->data().toString();
    QMdiSubWindow *wnd = HasTableWindow(ntable);

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

void FmtRibbonMainWindow::ExecuteCurrentFmtWindowSlot(const QString &Function, bool OpenWindow)
{
    ConnectionInfo *current = CurrentConnection();

    if (!current)
        return;

    QListView *view = pTablesDock->tablesWidget()->listView();
    if (view->selectionModel()->hasSelection())
    {
        QModelIndex index = view->selectionModel()->selectedIndexes().at(0);

        QString id = index.data(Qt::UserRole).toString();
        QMdiSubWindow *wnd = HasTableWindow(index.data(Qt::UserRole).toString());

        if (wnd)
        {
            FmtWorkWindow *_interface = qobject_cast<FmtWorkWindow*>(wnd->widget());

            if (_interface)
                QMetaObject::invokeMethod(_interface, Function.toLocal8Bit());

            if (OpenWindow)
                pMdi->setActiveSubWindow(wnd);
        }
        else
        {
            QSharedPointer<FmtTable> table(new FmtTable(current));
            if (table->load(id))
            {
                FmtWorkWindow *_interface = nullptr;
                wnd = CreateDocument(table, &_interface);

                if (_interface)
                    QMetaObject::invokeMethod(_interface, Function.toLocal8Bit());

                if (OpenWindow)
                {
                    wnd->show();
                    pMdi->setActiveSubWindow(wnd);
                }
                else
                    delete wnd;
            }
        }
    }
}

void FmtRibbonMainWindow::TablesContextMenu(QContextMenuEvent *event, QListView *view)
{
    UpdateActions();

    QMenu menu(this);
    menu.addAction(m_pActionEdit);
    menu.setActiveAction(m_pActionEdit);
    menu.setDefaultAction(m_pActionEdit);
    menu.addSeparator();
    menu.addAction(m_pFakeActionInit);
    menu.addSeparator();

    menu.addAction(m_pActionCopyTable);
    menu.addAction(m_pActionCopyTableTmp);
    menu.addAction(m_pActionCopyTableTo);

    menu.addSeparator();
    menu.addAction(m_pActionExport);
    menu.addAction(m_pFakeCreateTablesSql);
    menu.addSeparator();
    menu.addAction(m_pFakeUnloadToDbf);
    menu.addAction(m_pFakeLoadFromDbf);
    menu.addAction(m_pFakeCreateDiffToScript);
    menu.addSeparator();
    menu.addAction(m_pActionDeleteFmt);

    if (view->selectionModel()->hasSelection())
    {
        QString table = view->selectionModel()->selectedIndexes().at(0).data(Qt::UserRole).toString();
        m_pActionEdit->setEnabled(true);
        m_pActionEdit->setData(table);

        m_pActionExport->setEnabled(true);
        m_pActionExport->setData(table);
    }
    else
    {
        m_pActionEdit->setEnabled(false);
        m_pActionEdit->setData("");

        m_pActionExport->setEnabled(false);
        m_pActionExport->setData("");
    }

    menu.exec(event->globalPos());
}

bool FmtRibbonMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (pTablesDock && obj == pTablesDock->tablesWidget())
    {
        bool hr = false;
        QListView *view = pTablesDock->tablesWidget()->listView();

        if (!view->isEnabled())
            return false;

        if (event->type() == QEvent::ContextMenu)
        {
            TablesContextMenu((QContextMenuEvent*)event, view);
            hr =  true;
        }
        else if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *e = (QKeyEvent*)event;

            if (e->key() == Qt::Key_Return)
            {
                if (view->selectionModel()->hasSelection())
                {
                    QString table = view->selectionModel()->selectedIndexes().at(0).data(Qt::UserRole).toString();
                    m_pActionEdit->setData(table);
                    emit m_pActionEdit->triggered();
                    hr = true;
                }

                UpdateActions();
            }
        }

        return hr;
    }

    return SARibbonMainWindow::eventFilter(obj, event);
}
