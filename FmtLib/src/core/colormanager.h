#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QObject>
#include <QSet>
#include <QColor>
#include <QHash>

inline uint qHash(const QColor& key, uint seed = 0) noexcept {
    QColor rgb = key.toRgb();
    return qHash(qRgb(rgb.red(), rgb.green(), rgb.blue()), seed);
}

class ColorManager : public QObject
{
    Q_OBJECT
public:
    ColorManager(QObject *parent = nullptr);
    virtual ~ColorManager();

    QColor getColorForObject(QObject* obj);
    void releaseColorForObject(QObject* obj);

    int registeredObjectsCount() const { return m_objectColors.size(); }
    QList<QObject*> registeredObjects() const { return m_objectColors.keys(); }

signals:

private slots:
    void onObjectDestroyed(QObject* obj);

private:
    void initializePalette();
    void cleanupDestroyedObjects();

    // Основная палитра гармонирующих цветов с акцентным #217346
    QVector<QColor> m_colorPalette;

    // Карта привязки объектов к цветам
    QHash<QObject*, QColor> m_objectColors;

    // Множество занятых цветов (для быстрого поиска свободных)
    QSet<QColor> m_usedColors;

    // Индекс для последовательного распределения
    int m_lastUsedIndex = 0;
};

Q_GLOBAL_STATIC(ColorManager, ConnectionColors)

#endif // COLORMANAGER_H
