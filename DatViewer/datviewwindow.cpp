#include "datviewwindow.h"
#include <QDir>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QApplication>
#include "recordparser.h"
#include "difftableinfo.h"
#include "linesparsermain.h"
#include <QLabel>

DatViewWindow::DatViewWindow(const QString &filePath, QWidget *parent)
    : QMdiSubWindow(parent)
    , m_filePath(filePath)
    , m_model(nullptr)
{
    setWindowTitle(QFileInfo(filePath).fileName());
    setAttribute(Qt::WA_DeleteOnClose);

    setupDatabase();
    setupUI();
    loadDatToDatabase();
}

DatViewWindow::~DatViewWindow()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

void DatViewWindow::setupDatabase()
{
    // Создаем уникальное имя для базы данных
    static int dbCounter = 0;
    QString dbName = QString("dat_view_%1").arg(++dbCounter);

    m_database = QSqlDatabase::addDatabase("QSQLITE", dbName);
    m_database.setDatabaseName(":memory:");

    if (!m_database.open()) {
        QMessageBox::critical(this, tr("Ошибка"),
            tr("Не удалось открыть базу данных: %1").arg(m_database.lastError().text()));
        return;
    }
}

void DatViewWindow::setupUI()
{
    QWidget *mainWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    // Панель управления
    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setContentsMargins(2, 2, 2, 2);

    m_tableComboBox = new QComboBox();
    m_tableComboBox->setMinimumWidth(200);

    m_refreshButton = new QPushButton(tr("Обновить"));

    controlLayout->addWidget(new QLabel(tr("Таблица:")));
    controlLayout->addWidget(m_tableComboBox);
    controlLayout->addWidget(m_refreshButton);
    controlLayout->addStretch();

    // Таблица для отображения данных
    m_tableView = new QTableView();
    m_model = new QSqlTableModel(this, m_database);
    m_tableView->setModel(m_model);

    // Настройка внешнего вида таблицы
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSortingEnabled(true);
    m_tableView->horizontalHeader()->setStretchLastSection(true);

    mainLayout->addWidget(controlWidget);
    mainLayout->addWidget(m_tableView);

    setWidget(mainWidget);

    // Подключаем сигналы
    connect(m_refreshButton, &QPushButton::clicked, this, &DatViewWindow::refreshData);
    connect(m_tableComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DatViewWindow::onTableChanged);
}

QString DatViewWindow::findFmtFile(const QString &datFilePath)
{
    QFileInfo datFileInfo(datFilePath);

    // Ищем в папке FMT относительно DAT файла
    QDir fmtDir = datFileInfo.absoluteDir();
    fmtDir.cdUp(); // Переходим из Data в DBFile
    if (fmtDir.cd("FMT")) {
        QString fmtFileName = datFileInfo.baseName() + ".xml";
        QString fmtFilePath = fmtDir.absoluteFilePath(fmtFileName);
        if (QFile::exists(fmtFilePath)) {
            return fmtFilePath;
        }
    }

    // Ищем в ресурсах
    QString resourcePath = QString("://xml/%1.xml").arg(datFileInfo.baseName().toLower());
    if (QFile::exists(resourcePath)) {
        return resourcePath;
    }

    return QString();
}

void DatViewWindow::loadDatToDatabase()
{
    QProgressDialog progress(tr("Загрузка данных..."), tr("Отмена"), 0, 100, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(10);
    QApplication::processEvents();

    // Ищем FMT файл
    QString fmtFilePath = findFmtFile(m_filePath);

    progress.setValue(30);

    if (fmtFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Предупреждение"),
            tr("FMT файл не найден для: %1").arg(QFileInfo(m_filePath).fileName()));
    }

    progress.setValue(50);

    // Создаем и настраиваем парсер
    LinesParserMain linesParser;
    VcsType vcsType = VcsType::Svn; // По умолчанию

    if (vcsType == VcsType::Svn)
        linesParser.setTableParser(new LinesTablePareser("Index: "));
    else
        linesParser.setTableParser(new LinesTablePareser("diff --git "));

    progress.setValue(70);

    // Загружаем таблицу
    QScopedPointer<DiffTable> table(new DiffTable());
    if (!fmtFilePath.isEmpty()) {
        table->loadFromFmtXml(fmtFilePath, m_filePath);
    }

    progress.setValue(90);

    // Загружаем данные в SQLite
    diffLoadDatToSqlite(m_filePath, m_database, table.data(), false);

    progress.setValue(100);

    // Обновляем список таблиц
    refreshData();
}

void DatViewWindow::refreshData()
{
    if (!m_database.isOpen()) {
        return;
    }

    // Получаем список таблиц
    QStringList tables = m_database.tables(QSql::Tables);

    // Сохраняем текущую выбранную таблицу
    QString currentTable = m_tableComboBox->currentData().toString();

    m_tableComboBox->clear();

    foreach (const QString &table, tables) {
        if (table.startsWith("sqlite_")) continue; // Пропускаем системные таблицы

        // Получаем количество записей в таблице
        QSqlQuery query(m_database);
        if (query.exec(QString("SELECT COUNT(*) FROM %1").arg(table)) && query.next()) {
            int rowCount = query.value(0).toInt();

            QString displayText = QString("%1 (%2 записей)").arg(table).arg(rowCount);
            m_tableComboBox->addItem(displayText, table);
        }
    }

    // Восстанавливаем выбранную таблицу
    int index = m_tableComboBox->findData(currentTable);
    if (index >= 0) {
        m_tableComboBox->setCurrentIndex(index);
    } else if (m_tableComboBox->count() > 0) {
        m_tableComboBox->setCurrentIndex(0);
    }
}

void DatViewWindow::onTableChanged(int index)
{
    if (index < 0 || !m_database.isOpen()) {
        return;
    }

    QString tableName = m_tableComboBox->itemData(index).toString();

    // Устанавливаем таблицу в модель
    m_model->setTable(tableName);
    m_model->setEditStrategy(QSqlTableModel::OnRowChange);

    if (!m_model->select()) {
        QMessageBox::warning(this, tr("Ошибка"),
            tr("Не удалось загрузить данные: %1").arg(m_model->lastError().text()));
        return;
    }

    // Обновляем заголовки столбцов
    m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    for (int i = 1; i < m_model->columnCount(); ++i) {
        QString header = m_model->headerData(i, Qt::Horizontal).toString();
        m_model->setHeaderData(i, Qt::Horizontal, header);
    }

    // Настраиваем ширину столбцов
    m_tableView->resizeColumnsToContents();

    // Обновляем заголовок окна
    setWindowTitle(QString("%1 - [%2]").arg(QFileInfo(m_filePath).fileName()).arg(tableName));
}
