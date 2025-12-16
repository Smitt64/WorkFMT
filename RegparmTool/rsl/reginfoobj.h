#ifndef REGINFOOBJ_H
#define REGINFOOBJ_H

#include <QObject>
#include <QVariant>
#include <QAbstractTableModel>
#include <QSharedPointer>
#include <QComboBox>
#include <QStyledItemDelegate>

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
    virtual ~RegInfoObj();

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
    Q_INVOKABLE void setFullName(const QString &fullName);
    Q_INVOKABLE void setTypeName(const QString &typeName);
    Q_INVOKABLE void setType(qint16 type);
    Q_INVOKABLE void setIsGlobal(bool isGlobal);
    Q_INVOKABLE void setIsNode(bool isNode);
    Q_INVOKABLE void setIsSecurity(bool isSecurity);
    Q_INVOKABLE void setComment(const QString &comment);
    Q_INVOKABLE void setDefaultValue(const QVariant &defaultValue);

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
    Q_INVOKABLE QString getDefaultValueAsString() const;
    Q_INVOKABLE bool validateDefaultValue(const QVariant &value) const;
    Q_INVOKABLE static QStringList getAvailableTypes();

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
    QVariant convertToType(const QVariant &value) const;
    bool isValidForType(const QVariant &value) const;

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

QDebug operator<<(QDebug debug, const RegInfoObj &obj);
QDebug operator<<(QDebug debug, RegInfoObj *obj);

// ----------------------------------------------------------------------

using RegInfoObjList = QList<QSharedPointer<RegInfoObj>>;

class RegInfoObjModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RegInfoObjModel(QObject *parent = nullptr);

    // Enum для колонок таблицы
    enum Column {
        FullNameColumn = 0,
        TypeNameColumn,
        //TypeColumn,
        IsGlobalColumn,
        IsNodeColumn,
        IsSecurityColumn,
        CommentColumn,
        DefaultValueColumn,

        ColumnCount // Должен быть последним
    };
    Q_ENUM(Column)

    // QAbstractTableModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Дополнительные методы для работы с данными
    void setRegInfoObjects(const RegInfoObjList &objects);
    void addRegInfoObject(QSharedPointer<RegInfoObj> obj);
    void insertRegInfoObject(int row, QSharedPointer<RegInfoObj> obj);
    void removeRegInfoObject(int row);
    void clear();
    QSharedPointer<RegInfoObj> getRegInfoObject(int row) const;
    RegInfoObjList getAllObjects() const;

    // Вспомогательные методы
    int findRowByFullName(const QString &fullName) const;
    QModelIndex findIndexByFullName(const QString &fullName) const;

    // Статические методы для работы с колонками
    static QString columnName(Column column);
    static bool isBooleanColumn(Column column);
    static bool isEditableColumn(Column column);
    static bool isTypeColumn(Column column);

signals:
    void objectAdded(QSharedPointer<RegInfoObj> obj);
    void objectRemoved(QSharedPointer<RegInfoObj> obj);
    void objectChanged(QSharedPointer<RegInfoObj> obj);
    void validationError(const QString &errorMessage);

private:
    RegInfoObjList m_objects;

private slots:
    void onObjectDataChanged();
};

// ----------------------------------------------------------------------

class TypeComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TypeComboBoxDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

// ----------------------------------------------------------------------

class DefaultValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit DefaultValueDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

private:
    QWidget *createIntegerEditor(QWidget *parent) const;
    QWidget *createDoubleEditor(QWidget *parent) const;
    QWidget *createStringEditor(QWidget *parent) const;
    QWidget *createBinaryEditor(QWidget *parent) const;
    QWidget *createFlagEditor(QWidget *parent) const;
};

#endif // REGINFOOBJ_H
