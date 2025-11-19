#include "MainWindow.h"
#include "fmtcore.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QStatusBar>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>
#include "datviewwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : SARibbonMainWindow(parent)
    , m_lastRepositoryPath(QDir::homePath())
{
    FmtInit();
    // Настройка настроек приложения
    m_settings = new QSettings("YourCompany", "MDIApp", this);

    // Чтение сохраненных настроек
    readSettings();

    // Создание MDI области
    setupMdiArea();

    // Настройка DockWidget
    setupDockWidget();

    // Настройка Ribbon панели
    setupRibbon();

    // Настройка статусной панели
    statusBar()->showMessage(tr("Готово"));
}

MainWindow::~MainWindow()
{
    writeSettings();
}

void MainWindow::setupMdiArea()
{
    m_mdiArea = new QMdiArea(this);
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(m_mdiArea);
}

void MainWindow::setupDockWidget()
{
    // Создаем DockWidget
    m_dockWidget = new QDockWidget(tr("Файлы данных"), this);
    m_dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Создаем основной виджет для DockWidget
    QWidget *dockContent = new QWidget();
    QVBoxLayout *dockLayout = new QVBoxLayout(dockContent);

    // Создаем панель фильтра
    QWidget *filterWidget = new QWidget();
    QHBoxLayout *filterLayout = new QHBoxLayout(filterWidget);
    filterLayout->setContentsMargins(2, 2, 2, 2);

    QLabel *filterLabel = new QLabel(tr("Фильтр:"));
    m_filterEdit = new QLineEdit();
    m_filterEdit->setPlaceholderText(tr("Введите текст для поиска..."));

    QPushButton *clearFilterBtn = new QPushButton(tr("X"));
    clearFilterBtn->setFixedSize(20, 20);
    clearFilterBtn->setToolTip(tr("Очистить фильтр"));

    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(m_filterEdit);
    filterLayout->addWidget(clearFilterBtn);

    // Создаем список файлов
    m_fileListWidget = new QListWidget();
    m_fileListWidget->setAlternatingRowColors(true);

    // Добавляем элементы в layout
    dockLayout->addWidget(filterWidget);
    dockLayout->addWidget(m_fileListWidget);

    m_dockWidget->setWidget(dockContent);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockWidget);

    // Настройка FileSystemWatcher
    m_fileWatcher = new QFileSystemWatcher(this);

    // Подключаем сигналы
    connect(m_fileListWidget, &QListWidget::itemClicked,
            this, &MainWindow::onFileItemClicked);
    connect(m_fileListWidget, &QListWidget::itemDoubleClicked,
            this, &MainWindow::onFileItemDoubleClicked);
    connect(m_filterEdit, &QLineEdit::textChanged,
            this, &MainWindow::onFilterTextChanged);
    connect(clearFilterBtn, &QPushButton::clicked,
            m_filterEdit, &QLineEdit::clear);
    connect(m_fileWatcher, &QFileSystemWatcher::directoryChanged,
            this, &MainWindow::onDirectoryChanged);
}

void MainWindow::setupRibbon()
{
    SARibbonBar* ribbon = ribbonBar();
    //ribbon->setApplicationButtonText(tr("Файл"));

    // Категория "Файл"
    SARibbonCategory* categoryFile = new SARibbonCategory();
    categoryFile->setCategoryName(tr("Файл"));
    ribbon->addCategoryPage(categoryFile);

    // Панель "Репозиторий"
    SARibbonPannel* pannelRepo = categoryFile->addPannel(tr("Репозиторий"));

    // Кнопка "Открыть репозиторий"
    QAction* actionOpenRepo = new QAction(QIcon(":/icons/open_repo.png"), tr("Открыть репозиторий"), this);
    actionOpenRepo->setStatusTip(tr("Открыть папку репозитория"));
    connect(actionOpenRepo, &QAction::triggered, this, &MainWindow::openRepository);
    pannelRepo->addLargeAction(actionOpenRepo);

    // Панель "Документы"
    SARibbonPannel* pannelDocs = categoryFile->addPannel(tr("Документы"));

    // Кнопка "Новый документ"
    QAction* actionNewDoc = new QAction(QIcon(":/icons/new_doc.png"), tr("Новый документ"), this);
    actionNewDoc->setStatusTip(tr("Создать новый документ"));
    connect(actionNewDoc, &QAction::triggered, this, &MainWindow::createNewDocument);
    pannelDocs->addLargeAction(actionNewDoc);

    // Категория "Вид"
    SARibbonCategory* categoryView = new SARibbonCategory();
    categoryView->setCategoryName(tr("Вид"));
    ribbon->addCategoryPage(categoryView);

    // Панель "Окна"
    SARibbonPannel* pannelWindows = categoryView->addPannel(tr("Окна"));

    // Действие для показа/скрытия DockWidget
    QAction* actionToggleDock = new QAction(QIcon(":/icons/files.png"), tr("Показать файлы"), this);
    actionToggleDock->setCheckable(true);
    actionToggleDock->setChecked(true);
    actionToggleDock->setStatusTip(tr("Показать/скрыть панель файлов"));
    connect(actionToggleDock, &QAction::toggled, m_dockWidget, &QDockWidget::setVisible);
    connect(m_dockWidget, &QDockWidget::visibilityChanged, actionToggleDock, &QAction::setChecked);
    pannelWindows->addLargeAction(actionToggleDock);
}

void MainWindow::openRepository()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Выберите папку репозитория"),
        m_lastRepositoryPath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (!dir.isEmpty()) {
        m_lastRepositoryPath = dir;

        // Формируем путь к папке с данными
        m_currentDataPath = QDir(dir).filePath("Distrib/DBFile/Data");

        // Проверяем существование папки
        if (QDir(m_currentDataPath).exists()) {
            // Останавливаем наблюдение за предыдущей папкой
            if (!m_fileWatcher->directories().isEmpty()) {
                m_fileWatcher->removePaths(m_fileWatcher->directories());
            }

            // Добавляем новую папку для наблюдения
            m_fileWatcher->addPath(m_currentDataPath);

            // Обновляем список файлов
            refreshFileList();

            statusBar()->showMessage(tr("Репозиторий открыт: ") + dir);
        } else {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Папка с данными не найдена:\n%1").arg(m_currentDataPath));
            statusBar()->showMessage(tr("Папка с данными не найдена"));
        }

        // Сохраняем путь в настройках
        writeSettings();
    }
}

void MainWindow::createNewDocument()
{
    QTextEdit* textEdit = new QTextEdit();
    QMdiSubWindow* subWindow = m_mdiArea->addSubWindow(textEdit);
    subWindow->setWindowTitle(tr("Новый документ"));
    subWindow->show();
}

void MainWindow::refreshFileList()
{
    m_fileListWidget->clear();

    if (m_currentDataPath.isEmpty() || !QDir(m_currentDataPath).exists()) {
        return;
    }

    // Ищем все .dat файлы в папке
    QDirIterator it(m_currentDataPath, {"*.dat"}, QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo = it.fileInfo();

        // Добавляем файл в список
        QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
        item->setData(Qt::UserRole, fileInfo.absoluteFilePath());
        item->setToolTip(fileInfo.absoluteFilePath());

        m_fileListWidget->addItem(item);
    }

    // Применяем текущий фильтр
    onFilterTextChanged(m_filterEdit->text());

    statusBar()->showMessage(tr("Найдено файлов: %1").arg(m_fileListWidget->count()));
}

void MainWindow::onFileItemClicked(QListWidgetItem *item)
{
    if (!item) return;

    QString filePath = item->data(Qt::UserRole).toString();
    // Показываем информацию о файле в статусной панели
    QFileInfo fileInfo(filePath);
    statusBar()->showMessage(tr("Файл: %1 (%2 байт)").arg(fileInfo.fileName()).arg(fileInfo.size()));
}

void MainWindow::onFileItemDoubleClicked(QListWidgetItem *item)
{
    if (!item) return;

    QString filePath = item->data(Qt::UserRole).toString();
    openDatFile(filePath);
}

void MainWindow::openDatFile(const QString &filePath)
{
    // Проверяем, не открыт ли уже этот файл
    foreach (QMdiSubWindow *window, m_mdiArea->subWindowList()) {
        DatViewWindow *datWindow = qobject_cast<DatViewWindow*>(window);
        if (datWindow && datWindow->windowTitle().contains(QFileInfo(filePath).fileName())) {
            m_mdiArea->setActiveSubWindow(window);
            return;
        }
    }

    // Создаем новое окно
    DatViewWindow *datWindow = new DatViewWindow(filePath, this);
    QMdiSubWindow *subWindow = m_mdiArea->addSubWindow(datWindow);
    subWindow->setWindowIcon(QIcon(":/icons/database.png"));
    subWindow->show();

    statusBar()->showMessage(tr("Открыт файл: %1").arg(QFileInfo(filePath).fileName()));
}

void MainWindow::onFilterTextChanged(const QString &text)
{
    for (int i = 0; i < m_fileListWidget->count(); ++i) {
        QListWidgetItem *item = m_fileListWidget->item(i);
        bool matches = text.isEmpty() ||
                      item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }

    // Показываем количество отфильтрованных элементов
    int visibleCount = 0;
    for (int i = 0; i < m_fileListWidget->count(); ++i) {
        if (!m_fileListWidget->item(i)->isHidden()) {
            visibleCount++;
        }
    }

    m_dockWidget->setWindowTitle(tr("Файлы данных (%1/%2)").arg(visibleCount).arg(m_fileListWidget->count()));
}

void MainWindow::onDirectoryChanged(const QString &path)
{
    Q_UNUSED(path)
    // Обновляем список файлов при изменении в папке
    refreshFileList();
}

void MainWindow::readSettings()
{
    m_settings->beginGroup("MainWindow");
    restoreGeometry(m_settings->value("geometry").toByteArray());
    restoreState(m_settings->value("windowState").toByteArray());
    m_lastRepositoryPath = m_settings->value("lastRepositoryPath", QDir::homePath()).toString();
    m_settings->endGroup();
}

void MainWindow::writeSettings()
{
    m_settings->beginGroup("MainWindow");
    m_settings->setValue("geometry", saveGeometry());
    m_settings->setValue("windowState", saveState());
    m_settings->setValue("lastRepositoryPath", m_lastRepositoryPath);
    m_settings->endGroup();
}
