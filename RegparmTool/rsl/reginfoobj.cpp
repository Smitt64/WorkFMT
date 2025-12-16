#include "RegInfoObj.h"
#include <toolsruntime.h>
#include <QDebugStateSaver>
#include <QRegularExpression>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QApplication>

RegInfoObj::RegInfoObj(QObject *parent)
    : QObject(parent)
{
    // Инициализируем typeName на основе type по умолчанию
    updateTypeNameFromType();
}

RegInfoObj::RegInfoObj(const RegInfoObj &other, QObject *parent)
    : QObject(parent)
    , m_fullName(other.m_fullName)
    , m_typeName(other.m_typeName)
    , m_type(other.m_type)
    , m_isGlobal(other.m_isGlobal)
    , m_isNode(other.m_isNode)
    , m_isSecurity(other.m_isSecurity)
    , m_comment(other.m_comment)
    , m_defaultValue(other.m_defaultValue)
    , m_RegPath(other.m_RegPath)
{
}

RegInfoObj::~RegInfoObj()
{
}

// Геттеры
QString RegInfoObj::fullName() const { return m_fullName; }
QString RegInfoObj::typeName() const { return m_typeName; }
qint16 RegInfoObj::type() const { return m_type; }
bool RegInfoObj::isGlobal() const { return m_isGlobal; }
bool RegInfoObj::isNode() const { return m_isNode; }
bool RegInfoObj::isSecurity() const { return m_isSecurity; }
QString RegInfoObj::comment() const { return m_comment; }
QVariant RegInfoObj::defaultValue() const { return m_defaultValue; }

// Сеттеры
void RegInfoObj::setFullName(const QString &fullName)
{
    if (m_fullName != fullName)
    {
        // Заменяем все виды разделителей на стандартный
        QString processedName = fullName;

        // Заменяем "\\", "//" и "/" на "\"
        processedName.replace("\\\\", "\\");
        processedName.replace("//", "\\");
        processedName.replace("/", "\\");

        // Разделяем строку по разделителю "\"
        QStringList parts = processedName.split('\\', Qt::SkipEmptyParts);

        // Выполняем trim для каждой части и фильтруем пустые
        QStringList trimmedParts;
        for (const QString &part : qAsConst(parts)) {
            QString trimmed = part.trimmed();
            if (!trimmed.isEmpty()) {
                trimmedParts.append(trimmed);
            }
        }

        // Собираем обратно в строку с разделителем "\"
        m_fullName = trimmedParts.join('\\');

        emit fullNameChanged();
    }
}

void RegInfoObj::setTypeName(const QString &typeName)
{
    if (m_typeName != typeName) {
        m_typeName = typeName;
        updateTypeFromTypeName();
        emit typeChanged();
    }
}

void RegInfoObj::setType(qint16 type)
{
    if (m_type != type) {
        m_type = type;
        updateTypeNameFromType();

        // При изменении типа конвертируем текущее значение по умолчанию
        if (m_defaultValue.isValid() && !m_defaultValue.isNull()) {
            setDefaultValue(m_defaultValue);
        }

        emit typeChanged();
    }
}

void RegInfoObj::setIsGlobal(bool isGlobal)
{
    if (m_isGlobal != isGlobal) {
        m_isGlobal = isGlobal;
        emit isGlobalChanged();
    }
}

void RegInfoObj::setIsNode(bool isNode)
{
    if (m_isNode != isNode) {
        m_isNode = isNode;
        emit isNodeChanged();
    }
}

void RegInfoObj::setIsSecurity(bool isSecurity)
{
    if (m_isSecurity != isSecurity) {
        m_isSecurity = isSecurity;
        emit isSecurityChanged();
    }
}

void RegInfoObj::setComment(const QString &comment)
{
    if (m_comment != comment) {
        m_comment = toolReplaceUnicodeSymToOem(comment);
        emit commentChanged();
    }
}

void RegInfoObj::setDefaultValue(const QVariant &defaultValue)
{
    if (!defaultValue.isValid() || defaultValue.isNull()) {
        // Если значение пустое, устанавливаем соответствующее значение по умолчанию для типа
        QVariant newValue;
        switch (m_type) {
        case INTEGER:
            newValue = 0;
            break;
        case DOUBLE:
            newValue = 0.0;
            break;
        case STRING:
            newValue = QString();
            break;
        case BINARY:
            newValue = QByteArray();
            break;
        case FLAG:
            newValue = false;
            break;
        }

        if (m_defaultValue != newValue) {
            m_defaultValue = newValue;
            emit defaultValueChanged();
        }
        return;
    }

    // Конвертируем значение в правильный тип
    QVariant convertedValue = convertToType(defaultValue);

    if (m_defaultValue != convertedValue) {
        m_defaultValue = convertedValue;
        emit defaultValueChanged();
    }
}

// Вспомогательные методы (статичные)
QString RegInfoObj::getTypeNameFromType(qint16 type)
{
    switch (type) {
    case INTEGER: return "INTEGER";
    case DOUBLE: return "DOUBLE";
    case STRING: return "STRING";
    case BINARY: return "BINARY";
    case FLAG: return "FLAG";
    default: return "INTEGER";
    }
}

qint16 RegInfoObj::getTypeFromTypeName(const QString &typeName)
{
    QString normalizedName = typeName.trimmed().toUpper();
    if (normalizedName == "INTEGER") return INTEGER;
    if (normalizedName == "DOUBLE") return DOUBLE;
    if (normalizedName == "STRING") return STRING;
    if (normalizedName == "BINARY") return BINARY;
    if (normalizedName == "FLAG") return FLAG;
    return INTEGER; // По умолчанию
}

QStringList RegInfoObj::getAvailableTypes()
{
    return {"INTEGER", "DOUBLE", "STRING", "BINARY", "FLAG"};
}

QString RegInfoObj::getDefaultValueAsString() const
{
    if (!m_defaultValue.isValid() || m_defaultValue.isNull()) {
        return QString();
    }

    switch (m_type) {
    case INTEGER:
        return QString::number(m_defaultValue.toInt());
    case DOUBLE:
        return QString::number(m_defaultValue.toDouble());
    case STRING:
        return m_defaultValue.toString();
    case BINARY:
        return QString::fromUtf8(m_defaultValue.toByteArray().toHex());
    case FLAG:
        return m_defaultValue.toBool() ? "true" : "false";
    default:
        return m_defaultValue.toString();
    }
}

bool RegInfoObj::validateDefaultValue(const QVariant &value) const
{
    return isValidForType(value);
}

// Приватные методы
void RegInfoObj::updateTypeNameFromType()
{
    m_typeName = getTypeNameFromType(m_type);
}

void RegInfoObj::updateTypeFromTypeName()
{
    m_type = getTypeFromTypeName(m_typeName);
}

QVariant RegInfoObj::convertToType(const QVariant &value) const
{
    if (!value.isValid() || value.isNull()) {
        return QVariant();
    }

    switch (m_type) {
    case INTEGER: {
        bool ok;
        int intValue = value.toInt(&ok);
        if (ok) return intValue;

        // Попробуем конвертировать из строки
        QString str = value.toString().trimmed();
        intValue = str.toInt(&ok);
        if (ok) return intValue;

        // Если не удалось, вернем 0
        return 0;
    }

    case DOUBLE: {
        bool ok;
        double doubleValue = value.toDouble(&ok);
        if (ok) return doubleValue;

        // Попробуем конвертировать из строки
        QString str = value.toString().trimmed();
        doubleValue = str.toDouble(&ok);
        if (ok) return doubleValue;

        // Если не удалось, вернем 0.0
        return 0.0;
    }

    case STRING:
        return value.toString();

    case BINARY: {
        if (value.type() == QVariant::ByteArray) {
            return value;
        }

        QString str = value.toString().trimmed();
        // Если строка в hex формате, конвертируем из hex
        if (str.length() % 2 == 0 && QRegularExpression("^[0-9A-Fa-f]+$").match(str).hasMatch()) {
            return QByteArray::fromHex(str.toUtf8());
        }

        // Иначе просто в байтовый массив
        return str.toUtf8();
    }

    case FLAG: {
        if (value.type() == QVariant::Bool) {
            return value;
        }

        QString str = value.toString().trimmed().toLower();
        if (str == "true" || str == "1" || str == "on" || str == "yes") {
            return true;
        }
        if (str == "false" || str == "0" || str == "off" || str == "no") {
            return false;
        }

        // Попробуем конвертировать в число
        bool ok;
        int intValue = value.toInt(&ok);
        if (ok) return intValue != 0;

        // По умолчанию false
        return false;
    }

    default:
        return value;
    }
}

bool RegInfoObj::isValidForType(const QVariant &value) const
{
    if (!value.isValid() || value.isNull()) {
        return true; // Пустое значение допустимо
    }

    switch (m_type) {
    case INTEGER: {
        bool ok;
        value.toInt(&ok);
        if (ok) return true;

        QString str = value.toString().trimmed();
        str.toInt(&ok);
        return ok;
    }

    case DOUBLE: {
        bool ok;
        value.toDouble(&ok);
        if (ok) return true;

        QString str = value.toString().trimmed();
        str.toDouble(&ok);
        return ok;
    }

    case STRING:
        return true; // Любая строка допустима

    case BINARY:
        return true; // Любые бинарные данные допустимы

    case FLAG: {
        if (value.type() == QVariant::Bool) {
            return true;
        }

        QString str = value.toString().trimmed().toLower();
        if (str == "true" || str == "false" ||
            str == "1" || str == "0" ||
            str == "on" || str == "off" ||
            str == "yes" || str == "no") {
            return true;
        }

        bool ok;
        value.toInt(&ok);
        return ok;
    }

    default:
        return false;
    }
}

QDebug operator<<(QDebug debug, const RegInfoObj &obj)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "RegInfoObj("
                    << "fullName:" << obj.fullName()
                    << ", typeName:" << obj.typeName()
                    << ", type:" << obj.type()
                    << ", isGlobal:" << obj.isGlobal()
                    << ", isNode:" << obj.isNode()
                    << ", isSecurity:" << obj.isSecurity()
                    << ", comment:" << obj.comment()
                    << ", defaultValue:" << obj.defaultValue()
                    << ", defaultValueAsString:" << obj.getDefaultValueAsString()
                    << ")";
    return debug;
}

QDebug operator<<(QDebug debug, RegInfoObj *obj)
{
    return operator<<(debug, *obj);
}

// ----------------------------------------------------------------------

RegInfoObjModel::RegInfoObjModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int RegInfoObjModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_objects.count();
}

int RegInfoObjModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant RegInfoObjModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_objects.count() || index.row() < 0)
        return QVariant();

    QSharedPointer<RegInfoObj> obj = m_objects.at(index.row());
    if (!obj)
        return QVariant();

    Column column = static_cast<Column>(index.column());

    // Для булевых значений показываем только чекбокс, без текста
    if (role == Qt::DisplayRole && isBooleanColumn(column)) {
        return QVariant(); // Не показываем текст для булевых колонок
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (column) {
        case FullNameColumn: return obj->fullName();
        case TypeNameColumn: return obj->typeName();
        //case TypeColumn: return obj->type();
        case IsGlobalColumn: return obj->isGlobal();
        case IsNodeColumn: return obj->isNode();
        case IsSecurityColumn: return obj->isSecurity();
        case CommentColumn: return obj->comment();
        case DefaultValueColumn: return obj->getDefaultValueAsString();
        case ColumnCount: return QVariant(); // Не должно достигать
        }
    }

    // Для отображения булевых значений в виде чекбоксов
    if (role == Qt::CheckStateRole && isBooleanColumn(column)) {
        bool value = false;
        switch (column) {
        case IsGlobalColumn: value = obj->isGlobal(); break;
        case IsNodeColumn: value = obj->isNode(); break;
        case IsSecurityColumn: value = obj->isSecurity(); break;
        default: break;
        }
        return value ? Qt::Checked : Qt::Unchecked;
    }

    // ToolTip для длинного текста
    if (role == Qt::ToolTipRole) {
        switch (column) {
        case CommentColumn:
        case FullNameColumn:
            return data(index, Qt::DisplayRole);
        case IsGlobalColumn:
            return tr("Глобальный");
        case IsNodeColumn:
            return tr("Узел");
        case IsSecurityColumn:
            return tr("Безопасность");
        case DefaultValueColumn:
            return tr("Значение: %1\nТип: %2").arg(obj->getDefaultValueAsString()).arg(obj->typeName());
        default:
            break;
        }
    }

    // Выравнивание для разных типов данных
    if (role == Qt::TextAlignmentRole) {
        switch (column) {
        //case TypeColumn:
        case IsGlobalColumn:
        case IsNodeColumn:
        case IsSecurityColumn:
            return Qt::AlignCenter;
        default:
            return (int)Qt::AlignLeft | Qt::AlignVCenter;
        }
    }

    return QVariant();
}

QVariant RegInfoObjModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        Column column = static_cast<Column>(section);
        return columnName(column);
    }

    return QVariant();
}

bool RegInfoObjModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_objects.count() || index.row() < 0)
        return false;

    QSharedPointer<RegInfoObj> obj = m_objects.at(index.row());
    if (!obj)
        return false;

    Column column = static_cast<Column>(index.column());
    bool changed = false;

    // Обрабатываем изменение через чекбокс
    if (role == Qt::CheckStateRole && isBooleanColumn(column)) {
        bool boolValue = (value.toInt() == Qt::Checked);
        switch (column) {
        case IsGlobalColumn:
            obj->setIsGlobal(boolValue);
            changed = true;
            break;
        case IsNodeColumn:
            obj->setIsNode(boolValue);
            changed = true;
            break;
        case IsSecurityColumn:
            obj->setIsSecurity(boolValue);
            changed = true;
            break;
        default:
            break;
        }
    }
    else if (role == Qt::EditRole) {
        switch (column) {
        case FullNameColumn:
            obj->setFullName(value.toString());
            changed = true;
            break;
        case TypeNameColumn:
            obj->setTypeName(value.toString());
            changed = true;
            break;
        /*case TypeColumn:
            obj->setType(value.toInt());
            changed = true;
            break;*/
        case IsGlobalColumn:
            obj->setIsGlobal(value.toBool());
            changed = true;
            break;
        case IsNodeColumn:
            obj->setIsNode(value.toBool());
            changed = true;
            break;
        case IsSecurityColumn:
            obj->setIsSecurity(value.toBool());
            changed = true;
            break;
        case CommentColumn:
            obj->setComment(value.toString());
            changed = true;
            break;
        case DefaultValueColumn:
            // Проверяем валидность значения перед установкой
            if (obj->validateDefaultValue(value)) {
                obj->setDefaultValue(value);
                changed = true;
            } else {
                emit validationError(tr("Некорректное значение '%1' для типа '%2'")
                                    .arg(value.toString())
                                    .arg(obj->typeName()));
                return false;
            }
            break;
        case ColumnCount:
            break; // Не должно достигать
        }
    }

    if (changed) {
        emit dataChanged(index, index, {role});
        emit objectChanged(obj);
        return true;
    }

    return false;
}

Qt::ItemFlags RegInfoObjModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    Column column = static_cast<Column>(index.column());

    // Разрешаем редактирование только для редактируемых колонок
    if (isEditableColumn(column)) {
        flags |= Qt::ItemIsEditable;
    }

    // Для булевых колонок используем только чекбоксы
    if (isBooleanColumn(column)) {
        flags |= Qt::ItemIsUserCheckable;
        // Убираем возможность текстового редактирования для булевых колонок
        flags &= ~Qt::ItemIsEditable;
    }

    return flags;
}

void RegInfoObjModel::setRegInfoObjects(const RegInfoObjList &objects)
{
    beginResetModel();

    // Отключаем старые соединения
    for (auto &obj : m_objects) {
        if (obj) {
            disconnect(obj.data(), &RegInfoObj::fullNameChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::typeChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::isGlobalChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::isNodeChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::isSecurityChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::commentChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::defaultValueChanged, this, &RegInfoObjModel::onObjectDataChanged);
        }
    }

    m_objects = objects;

    // Подключаем новые соединения
    for (auto &obj : m_objects) {
        if (obj) {
            connect(obj.data(), &RegInfoObj::fullNameChanged, this, &RegInfoObjModel::onObjectDataChanged);
            connect(obj.data(), &RegInfoObj::typeChanged, this, &RegInfoObjModel::onObjectDataChanged);
            connect(obj.data(), &RegInfoObj::isGlobalChanged, this, &RegInfoObjModel::onObjectDataChanged);
            connect(obj.data(), &RegInfoObj::isNodeChanged, this, &RegInfoObjModel::onObjectDataChanged);
            connect(obj.data(), &RegInfoObj::isSecurityChanged, this, &RegInfoObjModel::onObjectDataChanged);
            connect(obj.data(), &RegInfoObj::commentChanged, this, &RegInfoObjModel::onObjectDataChanged);
            connect(obj.data(), &RegInfoObj::defaultValueChanged, this, &RegInfoObjModel::onObjectDataChanged);
        }
    }

    endResetModel();
}

void RegInfoObjModel::addRegInfoObject(QSharedPointer<RegInfoObj> obj)
{
    if (!obj)
        return;

    beginInsertRows(QModelIndex(), m_objects.count(), m_objects.count());
    m_objects.append(obj);

    // Подключаем сигналы изменения объекта
    connect(obj.data(), &RegInfoObj::fullNameChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::typeChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::isGlobalChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::isNodeChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::isSecurityChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::commentChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::defaultValueChanged, this, &RegInfoObjModel::onObjectDataChanged);

    endInsertRows();

    emit objectAdded(obj);
}

void RegInfoObjModel::insertRegInfoObject(int row, QSharedPointer<RegInfoObj> obj)
{
    if (!obj || row < 0 || row > m_objects.count())
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_objects.insert(row, obj);

    // Подключаем сигналы изменения объекта
    connect(obj.data(), &RegInfoObj::fullNameChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::typeChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::isGlobalChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::isNodeChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::isSecurityChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::commentChanged, this, &RegInfoObjModel::onObjectDataChanged);
    connect(obj.data(), &RegInfoObj::defaultValueChanged, this, &RegInfoObjModel::onObjectDataChanged);

    endInsertRows();

    emit objectAdded(obj);
}

void RegInfoObjModel::removeRegInfoObject(int row)
{
    if (row < 0 || row >= m_objects.count())
        return;

    QSharedPointer<RegInfoObj> obj = m_objects.at(row);

    beginRemoveRows(QModelIndex(), row, row);
    m_objects.removeAt(row);

    // Отключаем сигналы
    if (obj) {
        disconnect(obj.data(), &RegInfoObj::fullNameChanged, this, &RegInfoObjModel::onObjectDataChanged);
        disconnect(obj.data(), &RegInfoObj::typeChanged, this, &RegInfoObjModel::onObjectDataChanged);
        disconnect(obj.data(), &RegInfoObj::isGlobalChanged, this, &RegInfoObjModel::onObjectDataChanged);
        disconnect(obj.data(), &RegInfoObj::isNodeChanged, this, &RegInfoObjModel::onObjectDataChanged);
        disconnect(obj.data(), &RegInfoObj::isSecurityChanged, this, &RegInfoObjModel::onObjectDataChanged);
        disconnect(obj.data(), &RegInfoObj::commentChanged, this, &RegInfoObjModel::onObjectDataChanged);
        disconnect(obj.data(), &RegInfoObj::defaultValueChanged, this, &RegInfoObjModel::onObjectDataChanged);
    }

    endRemoveRows();

    emit objectRemoved(obj);
}

void RegInfoObjModel::clear()
{
    beginResetModel();

    // Отключаем все соединения
    for (auto &obj : m_objects) {
        if (obj) {
            disconnect(obj.data(), &RegInfoObj::fullNameChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::typeChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::isGlobalChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::isNodeChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::isSecurityChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::commentChanged, this, &RegInfoObjModel::onObjectDataChanged);
            disconnect(obj.data(), &RegInfoObj::defaultValueChanged, this, &RegInfoObjModel::onObjectDataChanged);
        }
    }

    m_objects.clear();
    endResetModel();
}

QSharedPointer<RegInfoObj> RegInfoObjModel::getRegInfoObject(int row) const
{
    if (row < 0 || row >= m_objects.count())
        return nullptr;
    return m_objects.at(row);
}

RegInfoObjList RegInfoObjModel::getAllObjects() const
{
    return m_objects;
}

int RegInfoObjModel::findRowByFullName(const QString &fullName) const
{
    for (int i = 0; i < m_objects.count(); ++i) {
        if (m_objects.at(i) && m_objects.at(i)->fullName() == fullName) {
            return i;
        }
    }
    return -1;
}

QModelIndex RegInfoObjModel::findIndexByFullName(const QString &fullName) const
{
    int row = findRowByFullName(fullName);
    if (row >= 0) {
        return index(row, 0);
    }
    return QModelIndex();
}

// Статические методы
QString RegInfoObjModel::columnName(RegInfoObjModel::Column column)
{
    switch (column) {
    case FullNameColumn: return tr("Полное имя");
    case TypeNameColumn: return tr("Тип");
    //case TypeColumn: return tr("Тип (код)");
    case IsGlobalColumn: return tr("Г");
    case IsNodeColumn: return tr("У");
    case IsSecurityColumn: return tr("Б");
    case CommentColumn: return tr("Комментарий");
    case DefaultValueColumn: return tr("Значение по умолчанию");
    case ColumnCount: return QString();
    }
    return QString();
}

bool RegInfoObjModel::isBooleanColumn(RegInfoObjModel::Column column)
{
    return column == IsGlobalColumn || column == IsNodeColumn || column == IsSecurityColumn;
}

bool RegInfoObjModel::isEditableColumn(RegInfoObjModel::Column column)
{
    // Все колонки редактируемые, кроме булевых (они редактируются только через чекбокс)
    return !isBooleanColumn(column);
}

bool RegInfoObjModel::isTypeColumn(RegInfoObjModel::Column column)
{
    return column == TypeNameColumn;
}

void RegInfoObjModel::onObjectDataChanged()
{
    RegInfoObj *obj = qobject_cast<RegInfoObj*>(sender());
    if (!obj)
        return;

    // Находим индекс измененного объекта
    for (int i = 0; i < m_objects.count(); ++i) {
        if (m_objects.at(i).data() == obj) {
            QModelIndex left = index(i, 0);
            QModelIndex right = index(i, ColumnCount - 1);
            emit dataChanged(left, right);
            emit objectChanged(m_objects.at(i));
            break;
        }
    }
}

// ----------------------------------------------------------------------
// Реализация TypeComboBoxDelegate
// ----------------------------------------------------------------------

TypeComboBoxDelegate::TypeComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *TypeComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QComboBox *comboBox = new QComboBox(parent);
    comboBox->setFrame(false);
    comboBox->addItems(RegInfoObj::getAvailableTypes());
    return comboBox;
}

void TypeComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    if (!comboBox)
        return;

    QString currentType = index.model()->data(index, Qt::EditRole).toString();
    int currentIndex = comboBox->findText(currentType);
    if (currentIndex >= 0) {
        comboBox->setCurrentIndex(currentIndex);
    }
}

void TypeComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    if (!comboBox)
        return;

    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void TypeComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

// ----------------------------------------------------------------------
// Реализация DefaultValueDelegate
// ----------------------------------------------------------------------

DefaultValueDelegate::DefaultValueDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *DefaultValueDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    Q_UNUSED(option)

    if (!index.isValid())
        return nullptr;

    // Получаем объект RegInfoObj из модели
    QAbstractItemModel *model = const_cast<QAbstractItemModel*>(index.model());
    RegInfoObjModel *regModel = qobject_cast<RegInfoObjModel*>(model);
    if (!regModel)
        return nullptr;

    QSharedPointer<RegInfoObj> obj = regModel->getRegInfoObject(index.row());
    if (!obj)
        return nullptr;

    // Создаем редактор в зависимости от типа
    switch (obj->type()) {
    case RegInfoObj::INTEGER:
        return createIntegerEditor(parent);
    case RegInfoObj::DOUBLE:
        return createDoubleEditor(parent);
    case RegInfoObj::STRING:
        return createStringEditor(parent);
    case RegInfoObj::BINARY:
        return createBinaryEditor(parent);
    case RegInfoObj::FLAG:
        return createFlagEditor(parent);
    default:
        return createStringEditor(parent);
    }
}

void DefaultValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    QString value = index.model()->data(index, Qt::EditRole).toString();

    if (QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor)) {
        bool ok;
        int intValue = value.toInt(&ok);
        if (ok) spinBox->setValue(intValue);
    }
    else if (QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        bool ok;
        double doubleValue = value.toDouble(&ok);
        if (ok) doubleSpinBox->setValue(doubleValue);
    }
    else if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        lineEdit->setText(value);
    }
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(editor)) {
        textEdit->setPlainText(value);
    }
    else if (QComboBox *comboBox = qobject_cast<QComboBox*>(editor)) {
        int currentIndex = comboBox->findText(value, Qt::MatchFixedString);
        if (currentIndex >= 0) {
            comboBox->setCurrentIndex(currentIndex);
        }
    }
}

void DefaultValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    if (!editor || !model)
        return;

    QVariant value;

    if (QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor)) {
        value = spinBox->value();
    }
    else if (QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        value = doubleSpinBox->value();
    }
    else if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        value = lineEdit->text();
    }
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(editor)) {
        value = textEdit->toPlainText();
    }
    else if (QComboBox *comboBox = qobject_cast<QComboBox*>(editor)) {
        value = comboBox->currentText();
    }

    if (value.isValid()) {
        model->setData(index, value, Qt::EditRole);
    }
}

void DefaultValueDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

QWidget *DefaultValueDelegate::createIntegerEditor(QWidget *parent) const
{
    QSpinBox *spinBox = new QSpinBox(parent);
    spinBox->setFrame(false);
    spinBox->setRange(-2147483647, 2147483647);
    //spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    return spinBox;
}

QWidget *DefaultValueDelegate::createDoubleEditor(QWidget *parent) const
{
    QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox(parent);
    doubleSpinBox->setFrame(false);
    doubleSpinBox->setRange(-1.7e308, 1.7e308);
    doubleSpinBox->setDecimals(6);
    //doubleSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    return doubleSpinBox;
}

QWidget *DefaultValueDelegate::createStringEditor(QWidget *parent) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(false);
    return lineEdit;
}

QWidget *DefaultValueDelegate::createBinaryEditor(QWidget *parent) const
{
    QTextEdit *textEdit = new QTextEdit(parent);
    textEdit->setFrameStyle(QFrame::NoFrame);
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    return textEdit;
}

QWidget *DefaultValueDelegate::createFlagEditor(QWidget *parent) const
{
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->setFrame(false);
    comboBox->addItems({"false", "true"});
    return comboBox;
}
