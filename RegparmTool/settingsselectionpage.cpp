#include "settingsselectionpage.h"
#include "ui_settingsselectionpage.h"
#include "settingspathcompleter.h"
#include "regparmmodel/settingstablemodel.h"
#include "regparmmodel/settingsvaluedelegate.h"
#include "regparmmodel/regparmmodel.h"
#include "regparmmodel/regparmitem.h"
#include "rsl/reginfoobj.h"
#include "regparmwizard.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include <toolsruntime.h>
#include <QHeaderView>
#include <QMessageBox>
#include <QTreeView>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextCodec>
#include <QKeyEvent>

SettingsSelectionPage::SettingsSelectionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SettingsSelectionPage),
    m_pModel(new SettingsTableModel(this)),
    m_pTypeDelegate(new TypeComboBoxDelegate(this)),
    m_pValueDelegate(new SettingsValueDelegate(SettingsTableModel::TypeNameColumn, this)),
    m_pCompleter(nullptr),
    m_pCompleterTree(nullptr),
    m_pConnection(nullptr)
{
    ui->setupUi(this);

    setupTree();

    connect(ui->addBtn, &QPushButton::clicked, this, &SettingsSelectionPage::onAddClicked);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &SettingsSelectionPage::onDeleteClicked);
    // Enter обрабатывается в eventFilter (с учётом открытого completer)
    // connect(ui->nameEdit, &QLineEdit::returnPressed, this, &SettingsSelectionPage::onNameReturnPressed);

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SettingsSelectionPage::onSelectionChanged);
    connect(m_pModel, &QAbstractItemModel::rowsInserted, this, &QWizardPage::completeChanged);
    connect(m_pModel, &QAbstractItemModel::rowsRemoved, this, &QWizardPage::completeChanged);

    onSelectionChanged();
}

SettingsSelectionPage::~SettingsSelectionPage()
{
    closeDatabase();
    delete ui;
}

void SettingsSelectionPage::setupTree()
{
    ui->treeView->setModel(m_pModel);
    ui->treeView->setItemDelegateForColumn(SettingsTableModel::TypeNameColumn, m_pTypeDelegate);
    ui->treeView->setItemDelegateForColumn(SettingsTableModel::ValueColumn, m_pValueDelegate);

    QHeaderView *header = ui->treeView->header();
    header->setStretchLastSection(false);
    header->setSectionResizeMode(SettingsTableModel::FullNameColumn, QHeaderView::Stretch);
    header->setSectionResizeMode(SettingsTableModel::TypeNameColumn, QHeaderView::Fixed);
    header->setSectionResizeMode(SettingsTableModel::ValueColumn, QHeaderView::Stretch);

    ui->treeView->setColumnWidth(SettingsTableModel::TypeNameColumn, 100);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setItemsExpandable(false);
    ui->treeView->setExpandsOnDoubleClick(false);
}

void SettingsSelectionPage::setupCompleter()
{
    if (!m_pConnection || !m_pConnection->isOpen())
        return;

    delete m_pCompleter;
    m_pCompleter = nullptr;
    delete m_pCompleterTree;
    m_pCompleterTree = nullptr;

    RegParmModel *completerModel = new RegParmModel(m_pConnection->db(), this);

    m_pCompleter = new SettingsPathCompleter(this);
    m_pCompleter->setModel(completerModel);
    m_pCompleter->setCompletionColumn(RegParmItem::T_NAME);
    m_pCompleter->setCompletionRole(Qt::DisplayRole);
    m_pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_pCompleter->setCompletionMode(QCompleter::PopupCompletion);

    ui->nameEdit->setCompleter(m_pCompleter);
    ui->nameEdit->installEventFilter(this);
}

bool SettingsSelectionPage::openDatabase()
{
    closeDatabase();

    RegParmWizard *wzrd = qobject_cast<RegParmWizard*>(wizard());
    if (!wzrd)
        return false;

    m_pConnection = new ConnectionInfo(QString());
    QString error;

    QString options;
    if (wzrd->field("IsUnicode").toBool())
        options = "RSD_UNICODE";

    if (!m_pConnection->open(QRSD_DRIVER,
                              wzrd->field("User").toString(),
                              wzrd->field("Password").toString(),
                              wzrd->field("Service").toString(),
                              options,
                              &error))
    {
        QMessageBox::critical(this, tr("Ошибка подключения"),
                              tr("Не удалось подключиться к БД: %1").arg(error));
        delete m_pConnection;
        m_pConnection = nullptr;
        return false;
    }

    return true;
}

void SettingsSelectionPage::closeDatabase()
{
    if (m_pConnection) {
        m_pConnection->close();
        delete m_pConnection;
        m_pConnection = nullptr;
    }
}

bool SettingsSelectionPage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->nameEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        int key = keyEvent->key();
        if (key == Qt::Key_Return || key == Qt::Key_Enter) {
            bool completerVisible = m_pCompleter && m_pCompleter->popup() && m_pCompleter->popup()->isVisible();
            if (completerVisible) {
                // Enter в открытом completer: подставляем выбранный элемент, не добавляем настройку
                QModelIndex idx = m_pCompleter->currentIndex();
                if (idx.isValid()) {
                    QString completion = m_pCompleter->completionString(idx);
                    ui->nameEdit->setText(completion);
                }
                m_pCompleter->popup()->hide();
                return true; // подавляем returnPressed
            } else {
                // Enter без completer: добавляем текущий текст
                onAddClicked();
                return true;
            }
        }
    }
    return QWizardPage::eventFilter(watched, event);
}

bool SettingsSelectionPage::isComplete() const
{
    return m_pModel->rowCount() > 0;
}

void SettingsSelectionPage::initializePage()
{
    m_pModel->clear();
    ui->nameEdit->clear();

    if (openDatabase())
        setupCompleter();

    ui->nameEdit->setFocus();
}

void SettingsSelectionPage::cleanupPage()
{
    closeDatabase();
}

int SettingsSelectionPage::nextId() const
{
    const RegParmWizard *wzrd = qobject_cast<const RegParmWizard*>(wizard());
    if (wzrd && wzrd->selectedAction() == RegParmWizard::ActionAddRegToDat)
        return RegParmWizard::PageOperationWizard;

    return QWizardPage::nextId();
}

SettingsTableModel *SettingsSelectionPage::model() const
{
    return m_pModel;
}

qint64 SettingsSelectionPage::findKeyIdByParentAndName(qint64 parentId, const QString &name) const
{
    if (!m_pConnection || !m_pConnection->isOpen())
        return -1;

    QSqlQuery query(m_pConnection->db());
    query.prepare("SELECT t_keyid FROM dregparm_dbt WHERE t_parentid = :parentId AND UPPER(t_name) = UPPER(:name)");
    query.bindValue(":parentId", parentId);
    query.bindValue(":name", name);

    if (toolExecuteQuery(&query) != 0 || !query.next())
        return -1;

    return query.value(0).toLongLong();
}

static QSharedPointer<RegInfoObj> loadSettingFromDatabase(QSqlDatabase &db, const QString &fullName)
{
    QStringList parts = fullName.split('\\', Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return nullptr;

    qint64 parentId = 0;
    for (int i = 0; i < parts.size() - 1; ++i)
    {
        QSqlQuery parentQuery(db);
        parentQuery.prepare("SELECT t_keyid FROM dregparm_dbt WHERE t_parentid = :parentId AND UPPER(t_name) = UPPER(:name)");
        parentQuery.bindValue(":parentId", parentId);
        parentQuery.bindValue(":name", parts.at(i));

        if (toolExecuteQuery(&parentQuery) != 0 || !parentQuery.next())
            return nullptr;

        parentId = parentQuery.value(0).toLongLong();
    }

    QSqlQuery query(db);
    query.prepare("SELECT p.t_type, p.t_global, p.t_security, p.t_isbranch, "
                  "v.t_lintvalue, v.t_ldoublevalue, v.t_fmtblobdata_xxxx "
                  "FROM dregparm_dbt p "
                  "LEFT JOIN dregval_dbt v ON p.t_keyid = v.t_keyid AND v.t_regkind = 0 AND v.t_objectid = 0 "
                  "WHERE p.t_parentid = :parentId AND UPPER(p.t_name) = UPPER(:name)");
    query.bindValue(":parentId", parentId);
    query.bindValue(":name", parts.last());

    if (toolExecuteQuery(&query) != 0 || !query.next())
        return nullptr;

    qint16 type = static_cast<qint16>(query.value(0).toInt());
    bool isGlobal = query.value(1).toString().trimmed() == QLatin1String("X");
    bool isSecurity = query.value(2).toString().trimmed() == QLatin1String("X");
    bool isBranch = query.value(3).toString().trimmed() == QLatin1String("X") ||
                    query.value(3).toString().trimmed() == QLatin1String("1");
    int lintValue = query.value(4).toInt();
    double doubleValue = query.value(5).toDouble();
    QByteArray blobBytes = query.value(6).toByteArray(); // QRSD возвращает BLOB как сырые байты

    if (isBranch)
        return nullptr; // Узел, а не листовая настройка

    QSharedPointer<RegInfoObj> obj(new RegInfoObj());
    obj->setFullName(fullName);
    obj->setType(type);
    obj->setIsGlobal(isGlobal);
    obj->setIsSecurity(isSecurity);

    QVariant value;
    switch (type)
    {
    case RegInfoObj::INTEGER:
        value = lintValue;
        break;
    case RegInfoObj::DOUBLE:
        value = doubleValue;
        break;
    case RegInfoObj::FLAG:
        value = lintValue != 0;
        break;
    case RegInfoObj::STRING:
    case RegInfoObj::BINARY:
    {
        QByteArray bytes = blobBytes;
        while (!bytes.isEmpty() && bytes.endsWith('\0'))
            bytes.chop(1);

        if (type == RegInfoObj::STRING) {
            QTextCodec *codec = QTextCodec::codecForName("IBM 866");
            value = codec ? codec->toUnicode(bytes) : QString::fromLocal8Bit(bytes);
        } else {
            value = bytes;
        }
        break;
    }
    default:
        break;
    }

    obj->setDefaultValue(value);
    return obj;
}

void SettingsSelectionPage::onAddClicked()
{
    QString fullName = ui->nameEdit->text();
    if (validateAndAddName(fullName)) {
        ui->nameEdit->clear();
        ui->nameEdit->setFocus();
    }
}

void SettingsSelectionPage::onDeleteClicked()
{
    QItemSelectionModel *selection = ui->treeView->selectionModel();
    QModelIndex current = selection->currentIndex();
    if (!current.isValid())
        return;

    m_pModel->removeSetting(current.row());
}

void SettingsSelectionPage::onSelectionChanged()
{
    bool hasSelection = ui->treeView->selectionModel()->currentIndex().isValid();
    ui->deleteBtn->setEnabled(hasSelection);
}

static QString normalizeSettingPath(const QString &input)
{
    // Разделители уровней: '\', '/', '.' - приводим к одному '\'.
    // Убираем пробелы вокруг разделителей (в ТЗ часто пишут "CB \ PARTY")
    // и схлопываем повторные разделители, собирая путь заново.
    QString simplified = input;
    simplified.replace('/', '\\');
    simplified.replace('.', '\\');

    QStringList parts = simplified.split('\\', Qt::SkipEmptyParts);
    for (QString &part : parts)
        part = part.trimmed();
    parts.removeAll(QString()); // уровни из одних пробелов

    QString result = parts.join('\\');

    QSharedPointer<RegInfoObj> tempObj(new RegInfoObj());
    tempObj->setFullName(result);
    return tempObj->fullName();
}

bool SettingsSelectionPage::validateAndAddName(const QString &fullName)
{
    QString normalizedName = normalizeSettingPath(fullName);
    if (normalizedName.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не задано полное имя настройки"));
        return false;
    }

    if (m_pModel->containsFullName(normalizedName)) {
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Настройка '%1' уже присутствует в списке").arg(normalizedName));
        return false;
    }

    if (!m_pConnection || !m_pConnection->isOpen()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Подключение к БД не установлено"));
        return false;
    }

    QSharedPointer<RegInfoObj> dbObj = loadSettingFromDatabase(m_pConnection->db(), normalizedName);
    if (!dbObj) {
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Настройка '%1' не найдена в подключенной схеме").arg(normalizedName));
        return false;
    }

    m_pModel->addSetting(dbObj);
    return true;
}
