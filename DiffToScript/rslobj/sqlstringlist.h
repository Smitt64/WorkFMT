#ifndef SQLSTRINGLIST_H
#define SQLSTRINGLIST_H

#include <QObject>

class SqlStringList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)          // Размер списка
    Q_PROPERTY(QStringList lines READ lines NOTIFY linesChanged)  // Содержимое списка
    Q_PROPERTY(QString sql READ sqlText CONSTANT)
public:
    explicit SqlStringList(QObject *parent = nullptr);
    SqlStringList(QStringList* list, QObject *parent = nullptr);
    virtual ~SqlStringList();

    Q_INVOKABLE void append(const QString& str);
    Q_INVOKABLE void append(const QStringList& strings);

    // Основные методы
    //SqlStringList& append(const QString& str);
    //SqlStringList& append(const QStringList& strings);

    // Перегрузка операторов
    //SqlStringList& operator<<(const QString& str);
    //SqlStringList& operator<<(const QStringList& strings);

    // Геттеры
    QStringList stringList() const;  // Возвращает копию списка
    int size() const;               // Размер списка
    QStringList lines() const;     // Содержимое списка (для QML)

    // Указатель на внутренний список
    QStringList* data() const;

    QString sqlText() const;

signals:
    void sizeChanged(int newSize);
    void linesChanged();  // Сигнал при изменении содержимого

private:
    QStringList* m_list = nullptr;  // Указатель на внешний QStringList
    bool m_ownsList = false;        // Флаг владения
};

#endif // SQLSTRINGLIST_H
