#ifndef REGINFOOBJ_H
#define REGINFOOBJ_H

#include <QObject>
#include <QVariant>

class RegInfoObj : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fullName READ fullName WRITE setFullName NOTIFY fullNameChanged)
    Q_PROPERTY(QString typeName READ typeName WRITE setTypeName NOTIFY typeChanged)
    Q_PROPERTY(qint16 type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool isGlobal READ isGlobal WRITE setIsGlobal NOTIFY isGlobalChanged)
    Q_PROPERTY(bool isNode READ isNode WRITE setIsNode NOTIFY isNodeChanged)
    Q_PROPERTY(bool isSecurity READ isSecurity WRITE setIsSecurity NOTIFY isSecurityChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
    Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)

public:
    Q_INVOKABLE RegInfoObj(QObject *parent = nullptr);
    Q_INVOKABLE RegInfoObj(const RegInfoObj &other, QObject *parent = nullptr);

    // Геттеры
    QString fullName() const;
    QString typeName() const;
    qint16 type() const;
    bool isGlobal() const;
    bool isNode() const;
    bool isSecurity() const;
    QString comment() const;
    QVariant defaultValue() const;

    // Сеттеры
    void setFullName(const QString &fullName);
    void setTypeName(const QString &typeName);
    void setType(qint16 type);
    void setIsGlobal(bool isGlobal);
    void setIsNode(bool isNode);
    void setIsSecurity(bool isSecurity);
    void setComment(const QString &comment);
    void setDefaultValue(const QVariant &defaultValue);

    // Константы для типов
    enum Type {
        INTEGER = 0,
        DOUBLE = 1,
        STRING = 2,
        BINARY = 3,
        FLAG = 4
    };
    Q_ENUM(Type)

    // Вспомогательные методы
    Q_INVOKABLE static QString getTypeNameFromType(qint16 type);
    Q_INVOKABLE static qint16 getTypeFromTypeName(const QString &typeName);

signals:
    void fullNameChanged();
    void typeChanged();
    void isGlobalChanged();
    void isNodeChanged();
    void isSecurityChanged();
    void commentChanged();
    void defaultValueChanged();

private:
    void updateTypeNameFromType();
    void updateTypeFromTypeName();

    QString m_fullName;
    QString m_typeName;
    qint16 m_type = INTEGER; // INTEGER по умолчанию
    bool m_isGlobal = false;
    bool m_isNode = false;
    bool m_isSecurity = false;
    QString m_comment;
    QVariant m_defaultValue;
    QString m_RegPath;
};

QDebug operator<<(QDebug debug, RegInfoObj *obj);
QDebug operator<<(QDebug debug, const RegInfoObj &obj);

#endif // REGINFOOBJ_H
