#ifndef SQLSTRINGLIST_H
#define SQLSTRINGLIST_H

#include <QMap>
#include <QObject>

class SqlStringList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)          // Размер списка
    Q_PROPERTY(QStringList lines READ lines NOTIFY linesChanged)  // Содержимое списка
    Q_PROPERTY(QString sql READ sqlText CONSTANT)
    Q_PROPERTY(int groupCount READ groupCount CONSTANT)
    Q_PROPERTY(QStringList groups READ groups CONSTANT)
public:
    explicit SqlStringList(QObject *parent = nullptr);
    SqlStringList(QStringList* list, QObject *parent = nullptr);
    virtual ~SqlStringList();

    Q_INVOKABLE void append(const QString& str, int depth = 0);
    Q_INVOKABLE void append(const QStringList& strings, int depth = 0);

    Q_INVOKABLE void clearGroup(const QString& name);
    Q_INVOKABLE void applyGroup(const QString& name);
    Q_INVOKABLE void beginGroup(const QString& name);
    Q_INVOKABLE void endGroup();

    // Геттеры
    QStringList stringList() const;  // Возвращает копию списка
    int size() const;               // Размер списка
    QStringList lines() const;     // Содержимое списка (для QML)

    // Указатель на внутренний список
    QStringList* data() const;

    QString sqlText() const;

    int groupCount() const;
    QStringList groups() const;

signals:
    void sizeChanged(int newSize);
    void linesChanged();  // Сигнал при изменении содержимого

private:
    QStringList* m_list = nullptr;  // Указатель на внешний QStringList
    QMap<QString, QStringList> m_Groups;
    QString m_CurrentGroup;
    bool m_ownsList = false;        // Флаг владения
};

#endif // SQLSTRINGLIST_H
