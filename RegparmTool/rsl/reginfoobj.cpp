#include "RegInfoObj.h"
#include <toolsruntime.h>
#include <QDebugStateSaver>

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
    if (m_defaultValue != defaultValue) {
        m_defaultValue = defaultValue;
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
    if (typeName.trimmed() == "INTEGER") return INTEGER;
    if (typeName.trimmed() == "DOUBLE") return DOUBLE;
    if (typeName.trimmed() == "STRING") return STRING;
    if (typeName.trimmed() == "BINARY") return BINARY;
    if (typeName.trimmed() == "FLAG") return FLAG;
    return INTEGER; // По умолчанию
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
                    << ")";
    return debug;
}

QDebug operator<<(QDebug debug, RegInfoObj *obj)
{
    return operator<<(debug, *obj);
}
