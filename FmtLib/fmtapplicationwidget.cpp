#include "fmtapplicationwidget.h"
#include "options/externaltoolspage.h"
#include "options/tablegroupoptions.h"
#include "ribbon/categorycontentwidget.h"
#include "ribbon/appoptionscontentwidget.h"
#include "options/generaloptions.h"
#include "fmtapplication.h"
#include "src/widgets/sqlconvertorcategory.h"
#include <QStandardItemModel>
#include <QtWidgets>

FmtApplicationWidget::FmtApplicationWidget(SARibbonMainWindow *parent) :
    ApplicationWidgetBase(parent),
    m_pOptionsWidget(nullptr),
    m_fileWidget(nullptr)

    //
    , m_homeWidget(nullptr)
    , m_insertWidget(nullptr)
    , m_viewWidget(nullptr)
    , m_TableGroupItem(nullptr)
{
    setMenuPanelColor(QColor(0x217346));

    m_pOptionsWidget = optionsWidget();
    SqlConvertorCategory *converter = new SqlConvertorCategory();

    m_pGeneralOptions = new GeneralOptions();
    m_pOptionsWidget->configureStringListEditors(m_pGeneralOptions);
    InitGeneralOptions();

    m_pExternalToolsPage  = new ExternalToolsPage();
    InitExternalToolsPage();

    m_pOptionsWidget->addPage(tr("Общие"), QIcon::fromTheme("IFrame"), m_pGeneralOptions);
    m_pOptionsWidget->addPage(tr("Внешние утилиты"), QIcon::fromTheme("Utility"), m_pExternalToolsPage); // m_TableGroupItem

    m_pTableGroupOptions = new TableGroupOptions(nullptr);
    int item = m_pOptionsWidget->addPage(tr("Внешние утилиты"), QIcon::fromTheme("Utility"), m_pTableGroupOptions);
    m_TableGroupItem = m_pOptionsWidget->listWidget()->item(item);

    m_pOptionsWidget->addRslPage(QIcon::fromTheme("RunTestDialog"));
    m_pOptionsWidget->addLogPage(QIcon::fromTheme("Log"), "WorkFmt");

    FmtApplication *app = qobject_cast<FmtApplication*>(qApp);
    m_pOptionsWidget->setSettings(app->settings());
    setupTabs();

    addTab(tr("Конвертировать SQL"), converter);
    addTab(tr("Параметры"), m_pOptionsWidget);
    m_pOptionsWidget->updateStyle();

    // Подключаем сигналы
    connect(this, &ApplicationWidgetBase::currentTabChanged,
            this, &FmtApplicationWidget::onTabChanged);
    connect(this, &ApplicationWidgetBase::backButtonClicked,
            this, &FmtApplicationWidget::onBackButtonClicked);
}

FmtApplicationWidget::~FmtApplicationWidget()
{
    delete m_pOptionsWidget;
}

void FmtApplicationWidget::setCurrentConnection(ConnectionInfo *info)
{

}

void FmtApplicationWidget::InitExternalToolsPage()
{
    QToolButton *toolButton = m_pExternalToolsPage->findChild<QToolButton*>("toolButton");
    toolButton->setIcon(QIcon::fromTheme("FolderOpened"));
    toolButton->setIconSize(QSize(24, 24));
}

void FmtApplicationWidget::InitGeneralOptions()
{
    QToolButton *newExpTableBtn = m_pGeneralOptions->findChild<QToolButton*>("newExpTableBtn");
    QToolButton *removeExpTableBtn = m_pGeneralOptions->findChild<QToolButton*>("removeExpTableBtn");

    newExpTableBtn->setIcon(QIcon::fromTheme("Create"));
    removeExpTableBtn->setIcon(QIcon::fromTheme("Remove_color"));

    newExpTableBtn->setIconSize(QSize(24, 24));
    removeExpTableBtn->setIconSize(QSize(24, 24));

    QTreeView *exportTableSettingsView = m_pGeneralOptions->findChild<QTreeView*>("exportTableSettingsView");
    exportTableSettingsView->setFrameShape(QFrame::NoFrame);
    exportTableSettingsView->setRootIsDecorated(false);
}

void FmtApplicationWidget::setupTabs()
{
    // Создаем все вкладки
    createFileTab();
    createHomeTab();
    createInsertTab();
    createViewTab();

    // Добавляем вкладки в ApplicationWidgetBase
    if (m_fileWidget)
        addTab("Файл", m_fileWidget);

    if (m_homeWidget)
        addTab("Главная", m_homeWidget);

    if (m_insertWidget)
        addTab("Вставка", m_insertWidget);

    if (m_viewWidget)
        addTab("Вид", m_viewWidget);
}

void FmtApplicationWidget::createFileTab()
{
    m_fileWidget = new CategoryContentWidget(this);
    m_fileWidget->setLayoutType(CategoryContentWidget::LayoutListDetail);

    // Применяем Office-стиль
    m_fileWidget->updateOfficeStyle();

    // Добавляем поле поиска перед списком
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Поиск...");
    searchEdit->setMinimumHeight(30);
    searchEdit->setMaximumWidth(280);
    m_fileWidget->addWidgetBeforeList(searchEdit);

    // Добавляем кнопку "Обновить" перед списком
    QPushButton *refreshButton = new QPushButton("Обновить");
    refreshButton->setMinimumHeight(30);
    refreshButton->setMaximumWidth(280);
    m_fileWidget->addWidgetBeforeList(refreshButton);

    // Добавляем разделитель
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("QFrame { background-color: #E1E1E1; max-height: 1px; min-height: 1px; }");
    separator->setFixedHeight(1);
    m_fileWidget->addWidgetBeforeList(separator);

    // Создаем иконки для элементов списка
    QIcon folderIcon = style()->standardIcon(QStyle::SP_DirIcon);
    QIcon computerIcon = style()->standardIcon(QStyle::SP_ComputerIcon);
    QIcon driveIcon = style()->standardIcon(QStyle::SP_DriveHDIcon);
    QIcon desktopIcon = style()->standardIcon(QStyle::SP_DesktopIcon);
    QIcon documentIcon = style()->standardIcon(QStyle::SP_FileIcon);
    QIcon downloadIcon = style()->standardIcon(QStyle::SP_VistaShield);

    // Добавляем элементы списка с иконками
    m_fileWidget->addListGroup("Компьютер");
    m_fileWidget->addListItem("Текущая папка", "current_folder");
    m_fileWidget->addListItem("Добавление места", "add_location");

    m_fileWidget->addListGroup("Последние папки");
    m_fileWidget->addListItem("Telegram Desktop", "telegram");
    m_fileWidget->addListItem("D:", "d_drive");
    m_fileWidget->addListItem("Документы", "documents");
    m_fileWidget->addListItem("Downloads", "downloads");
    m_fileWidget->addListItem("debug", "debug");
    m_fileWidget->addListItem("Рабочий стол", "desktop");

    // Получаем указатель на список для установки иконок
    QListWidget *listWidget = m_fileWidget->listWidget();
    if (listWidget)
    {
        // Устанавливаем иконки для элементов
        // Индексы: 0 - группа "Компьютер", 1-2 - элементы компьютера, 3 - группа "Последние папки", 4-9 - элементы папок
        int index = 0;

        // Группа "Компьютер" (не выбирается, иконку не ставим)
        index++;

        // Текущая папка
        QListWidgetItem *currentFolderItem = listWidget->item(index++);
        if (currentFolderItem) currentFolderItem->setIcon(computerIcon);

        // Добавление места
        QListWidgetItem *addLocationItem = listWidget->item(index++);
        if (addLocationItem) addLocationItem->setIcon(driveIcon);

        // Группа "Последние папки" (не выбирается)
        index++;

        // Telegram Desktop
        QListWidgetItem *telegramItem = listWidget->item(index++);
        if (telegramItem) telegramItem->setIcon(folderIcon);

        // D:
        QListWidgetItem *dDriveItem = listWidget->item(index++);
        if (dDriveItem) dDriveItem->setIcon(driveIcon);

        // Документы
        QListWidgetItem *documentsItem = listWidget->item(index++);
        if (documentsItem) documentsItem->setIcon(documentIcon);

        // Downloads
        QListWidgetItem *downloadsItem = listWidget->item(index++);
        if (downloadsItem) downloadsItem->setIcon(downloadIcon);

        // debug
        QListWidgetItem *debugItem = listWidget->item(index++);
        if (debugItem) debugItem->setIcon(folderIcon);

        // Рабочий стол
        QListWidgetItem *desktopItem = listWidget->item(index++);
        if (desktopItem) desktopItem->setIcon(desktopIcon);

        // Устанавливаем размер иконок
        listWidget->setIconSize(QSize(24, 24));
    }

    // Правая часть - детальная область
    QWidget *detailWidget = new QWidget();
    QVBoxLayout *detailLayout = new QVBoxLayout(detailWidget);
    detailLayout->setContentsMargins(20, 10, 20, 20);

    QLabel *pathLabel = new QLabel("Текущая папка");
    pathLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333333;");
    detailLayout->addWidget(pathLabel);

    QLabel *pathValue = new QLabel("Excel\nC: » Users » smitt » AppData » Roaming » Microsoft » Excel");
    pathValue->setWordWrap(true);
    pathValue->setStyleSheet("color: #666666; padding: 10px 0;");
    detailLayout->addWidget(pathValue);

    QPushButton *browseButton = new QPushButton("Обзор");
    browseButton->setFixedWidth(100);
    detailLayout->addWidget(browseButton);
    detailLayout->addStretch();

    m_fileWidget->setDetailWidget(detailWidget);

    // Подключаем сигнал выбора элемента
    connect(m_fileWidget, &CategoryContentWidget::listItemSelected,
            this, [this](const QVariant &data) {
                qDebug() << "File tab: selected" << data.toString();
            });
}

void FmtApplicationWidget::createHomeTab()
{
    m_homeWidget = new CategoryContentWidget(this);
    m_homeWidget->setLayoutType(CategoryContentWidget::LayoutGroups);
    m_homeWidget->setGroupsColumns(3);  // 3 колонки для групп
    m_homeWidget->setGroupSpacing(15);

    // Группа "Буфер обмена"
    QWidget *clipboardWidget = new QWidget();
    QVBoxLayout *clipboardLayout = new QVBoxLayout(clipboardWidget);
    clipboardLayout->setSpacing(5);

    QPushButton *pasteBtn = new QPushButton("Вставить");
    pasteBtn->setFixedHeight(60);
    pasteBtn->setStyleSheet("QPushButton { background-color: #0078D7; color: white; border: none; border-radius: 3px; }");
    clipboardLayout->addWidget(pasteBtn);

    QHBoxLayout *copyLayout = new QHBoxLayout();
    QPushButton *copyBtn = new QPushButton("Копировать");
    QPushButton *cutBtn = new QPushButton("Вырезать");
    copyLayout->addWidget(copyBtn);
    copyLayout->addWidget(cutBtn);
    clipboardLayout->addLayout(copyLayout);

    m_homeWidget->addGroup("Буфер обмена", clipboardWidget, 1);

    // Группа "Шрифт"
    QWidget *fontWidget = new QWidget();
    QVBoxLayout *fontLayout = new QVBoxLayout(fontWidget);

    QComboBox *fontCombo = new QComboBox();
    fontCombo->addItems({"Calibri", "Arial", "Times New Roman", "Segoe UI"});
    fontLayout->addWidget(fontCombo);

    QComboBox *sizeCombo = new QComboBox();
    sizeCombo->addItems({"11", "12", "14", "16", "18", "20"});
    fontLayout->addWidget(sizeCombo);

    QHBoxLayout *styleLayout = new QHBoxLayout();
    QPushButton *boldBtn = new QPushButton("B");
    QPushButton *italicBtn = new QPushButton("I");
    QPushButton *underlineBtn = new QPushButton("U");
    boldBtn->setFixedWidth(30);
    italicBtn->setFixedWidth(30);
    underlineBtn->setFixedWidth(30);
    styleLayout->addWidget(boldBtn);
    styleLayout->addWidget(italicBtn);
    styleLayout->addWidget(underlineBtn);
    styleLayout->addStretch();
    fontLayout->addLayout(styleLayout);

    m_homeWidget->addGroup("Шрифт", fontWidget, 1);

    // Группа "Выравнивание"
    QWidget *alignWidget = new QWidget();
    QVBoxLayout *alignLayout = new QVBoxLayout(alignWidget);

    QHBoxLayout *alignButtons = new QHBoxLayout();
    QPushButton *leftAlign = new QPushButton("⬅");
    QPushButton *centerAlign = new QPushButton("⬅➡");
    QPushButton *rightAlign = new QPushButton("➡");
    leftAlign->setFixedWidth(40);
    centerAlign->setFixedWidth(40);
    rightAlign->setFixedWidth(40);
    alignButtons->addWidget(leftAlign);
    alignButtons->addWidget(centerAlign);
    alignButtons->addWidget(rightAlign);
    alignButtons->addStretch();
    alignLayout->addLayout(alignButtons);

    QHBoxLayout *verticalAlign = new QHBoxLayout();
    QPushButton *topAlign = new QPushButton("▲");
    QPushButton *middleAlign = new QPushButton("▼");
    QPushButton *bottomAlign = new QPushButton("⬚");
    topAlign->setFixedWidth(40);
    middleAlign->setFixedWidth(40);
    bottomAlign->setFixedWidth(40);
    verticalAlign->addWidget(topAlign);
    verticalAlign->addWidget(middleAlign);
    verticalAlign->addWidget(bottomAlign);
    verticalAlign->addStretch();
    alignLayout->addLayout(verticalAlign);

    m_homeWidget->addGroup("Выравнивание", alignWidget, 1);
}

void FmtApplicationWidget::createInsertTab()
{
    m_insertWidget = new CategoryContentWidget(this);
    m_insertWidget->setLayoutType(CategoryContentWidget::LayoutLeftPanel);

    // Левая панель с кнопками навигации
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(10, 10, 10, 10);
    leftLayout->setSpacing(10);

    // Заголовок
    QLabel *titleLabel = new QLabel("Параметры вставки");
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333333; padding-bottom: 10px;");
    leftLayout->addWidget(titleLabel);

    // Комбобокс выбора типа вставки
    QLabel *typeLabel = new QLabel("Тип объекта:");
    typeLabel->setStyleSheet("color: #666666;");
    leftLayout->addWidget(typeLabel);

    QComboBox *objectTypeCombo = new QComboBox();
    objectTypeCombo->addItems({"Таблица", "Диаграмма", "Рисунок", "Текстовое поле", "Символ"});
    objectTypeCombo->setMinimumHeight(30);
    leftLayout->addWidget(objectTypeCombo);

    // Поле ввода для имени
    QLabel *nameLabel = new QLabel("Имя объекта:");
    nameLabel->setStyleSheet("color: #666666; margin-top: 10px;");
    leftLayout->addWidget(nameLabel);

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Введите имя объекта...");
    nameEdit->setMinimumHeight(30);
    leftLayout->addWidget(nameEdit);

    // Спинбокс для размера
    QLabel *sizeLabel = new QLabel("Размер (px):");
    sizeLabel->setStyleSheet("color: #666666; margin-top: 10px;");
    leftLayout->addWidget(sizeLabel);

    QSpinBox *sizeSpinBox = new QSpinBox();
    sizeSpinBox->setRange(10, 500);
    sizeSpinBox->setValue(100);
    sizeSpinBox->setMinimumHeight(30);
    leftLayout->addWidget(sizeSpinBox);

    // Чекбоксы опций
    QCheckBox *borderCheckBox = new QCheckBox("Добавить рамку");
    borderCheckBox->setChecked(true);
    leftLayout->addWidget(borderCheckBox);

    QCheckBox *shadowCheckBox = new QCheckBox("Добавить тень");
    leftLayout->addWidget(shadowCheckBox);

    // Кнопка вставки
    QPushButton *insertButton = new QPushButton("Вставить");
    leftLayout->addWidget(insertButton);
    insertButton->setAttribute(Qt::WA_StyledBackground, true);
    insertButton->setMouseTracking(true);

    leftLayout->addStretch();

    m_insertWidget->setLeftPanelWidget(leftPanel, 280);

    // Правая панель с превью
    QWidget *previewWidget = new QWidget();
    QVBoxLayout *previewLayout = new QVBoxLayout(previewWidget);
    previewLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *previewTitle = new QLabel("Предварительный просмотр");
    previewTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #333333;");
    previewLayout->addWidget(previewTitle);

    // Область для отображения превью
    QFrame *previewArea = new QFrame();
    previewArea->setStyleSheet(
        "QFrame {"
        "    background-color: white;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 4px;"
        "}"
        );
    previewArea->setMinimumHeight(300);
    previewArea->setFrameShape(QFrame::Box);

    QVBoxLayout *previewAreaLayout = new QVBoxLayout(previewArea);
    previewAreaLayout->setAlignment(Qt::AlignCenter);

    QLabel *previewLabel = new QLabel("Здесь будет отображаться\nпредварительный просмотр\nвыбранного объекта");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet("color: #999999; font-size: 12px;");
    previewAreaLayout->addWidget(previewLabel);

    previewLayout->addWidget(previewArea);
    previewLayout->addStretch();

    m_insertWidget->setDetailWidget(previewWidget);

    // Подключаем сигналы для демонстрации
    connect(insertButton, &QPushButton::clicked, this, [this, objectTypeCombo, nameEdit, sizeSpinBox, borderCheckBox, shadowCheckBox, previewLabel]() {
        QString objectType = objectTypeCombo->currentText();
        QString objectName = nameEdit->text();
        int size = sizeSpinBox->value();
        bool hasBorder = borderCheckBox->isChecked();
        bool hasShadow = shadowCheckBox->isChecked();

        QString previewText = QString("Вставлен объект:\n\nТип: %1\nИмя: %2\nРазмер: %3 px\nРамка: %4\nТень: %5")
                                  .arg(objectType)
                                  .arg(objectName.isEmpty() ? "Без имени" : objectName)
                                  .arg(size)
                                  .arg(hasBorder ? "Да" : "Нет")
                                  .arg(hasShadow ? "Да" : "Нет");

        previewLabel->setText(previewText);
        previewLabel->setStyleSheet("color: #333333; font-size: 12px;");

        qDebug() << "Insert object:" << objectType << objectName << size << hasBorder << hasShadow;
    });

    m_insertWidget->updateOfficeStyle();

    connect(objectTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [previewLabel](int index) {
        QStringList previews = {
            "📊 Предварительный просмотр таблицы\n\n5x5 таблица с данными",
            "📈 Предварительный просмотр диаграммы\n\nГрафик зависимости данных",
            "🖼️ Предварительный просмотр рисунка\n\nИзображение будет вставлено",
            "📝 Предварительный просмотр текстового поля\n\nТекст будет размещен в поле",
            "🔧 Предварительный просмотр символа\n\nСпециальный символ будет вставлен"
        };
        if (index >= 0 && index < previews.size()) {
            previewLabel->setText(previews[index]);
        }
    });

    m_insertWidget->updateStyle();
    qDebug() << insertButton->style();
}

void FmtApplicationWidget::createViewTab()
{
    m_viewWidget = new CategoryContentWidget(this);
    m_viewWidget->setLayoutType(CategoryContentWidget::LayoutFullWidget);

    // Полноэкранная таблица для просмотра данных
    QTableWidget *spreadsheet = new QTableWidget(50, 10);
    spreadsheet->setHorizontalHeaderLabels({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"});
    spreadsheet->setAlternatingRowColors(true);
    spreadsheet->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Заполняем тестовыми данными
    for (int row = 0; row < 50; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString("Данные %1-%2").arg(row+1).arg(col+1));
            spreadsheet->setItem(row, col, item);
        }
    }

    m_viewWidget->setMainWidget(spreadsheet);
}

void FmtApplicationWidget::onTabChanged(int index)
{
    qDebug() << "Tab changed to index:" << index << "title:" << tabTitle(index);
}

void FmtApplicationWidget::onBackButtonClicked()
{
    qDebug() << "Back button clicked - widget will be hidden";
}