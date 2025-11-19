#ifndef REGPARMITEM_H
#define REGPARMITEM_H

#include <QVariant>
#include <QVector>
#include <QString>

class RegParmItem
{
public:
    // Enum для удобного доступа к полям
    enum Field {
        T_KEYID = 0,
        T_PARENTID,
        T_NAME,
        T_TYPE,
        T_GLOBAL,
        T_DESCRIPTION,
        T_SECURITY,
        T_ISBRANCH,
        T_TEMPLATE,
        FIELD_COUNT
    };

    explicit RegParmItem(const QVector<QVariant> &data, RegParmItem *parent = nullptr);
    ~RegParmItem();

    void appendChild(RegParmItem *child);
    RegParmItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    RegParmItem *parentItem();

    // Методы доступа по номеру поля
    qint64 keyId() const;
    qint64 parentId() const;
    QString name() const;
    int type() const;
    QString global() const;
    QString description() const;
    QString security() const;
    bool isBranch() const;
    QString templateStr() const;

    // Универсальный метод доступа по enum
    QVariant field(Field field) const;

    // Методы доступа по имени поля
    QVariant fieldByName(const QString &fieldName) const;
    static int fieldIndexByName(const QString &fieldName);

    bool hasChildren() const;

    void setChildrenLoaded(bool loaded);
    bool childrenLoaded() const;

private:
    QVector<QVariant> m_itemData;
    QVector<RegParmItem *> m_childItems;
    RegParmItem *m_parentItem;
    bool m_childrenLoaded = false;

    static QHash<QString, int> initFieldMap();
    static const QHash<QString, int> FIELD_MAP;
};

Q_DECLARE_OPAQUE_POINTER(RegParmItem)

#endif // REGPARMITEM_H
