#include "colormanager.h"

ColorManager::ColorManager(QObject *parent)
    : QObject{parent}
{
    initializePalette();
}

ColorManager::~ColorManager()
{

}

void ColorManager::initializePalette()
{
    // Акцентный цвет Office 2013 (#217346)
    QColor accentBase("#217346");
    QColor accentHsv = accentBase.toHsv();
    double accentHue = accentHsv.hueF();

    // Предопределённая палитра, гармонирующая с акцентным цветом
    m_colorPalette = {
        // Основные гармоничные цвета (триадная схема)
        QColor("#2E8B57"), // Морская зелень (близкий к акцентному)
        QColor("#4682B4"), // Стальной синий (120° от акцентного)
        QColor("#D2691E"), // Шоколадный (240° от акцентного)

        // Комплементарные цвета
        QColor("#9370DB"), // Средний фиолетовый
        QColor("#20B2AA"), // Светлый морской волны

        // Аналогичные цвета
        QColor("#3CB371"), // Средний морской зелёный (+30°)
        QColor("#228B22"), // Лесной зелёный (-30°)

        // Дополнительные контрастные цвета
        QColor("#CD5C5C"), // Индийский красный
        QColor("#DAA520"), // Золотистый
        QColor("#6495ED"), // Васильковый

        // Нейтральные акценты
        QColor("#DC143C"), // Малиновый
        QColor("#008B8B"), // Тёмный бирюзовый
        QColor("#4169E1"), // Королевский синий
        QColor("#6A5ACD"), // Аспидно-синий

        // Запасные цвета (если основные закончатся)
        QColor("#8B4513"), // Коричневый седла
        QColor("#2F4F4F"), // Тёмный аспидно-серый
        QColor("#B22222"), // Кирпичный
        QColor("#556B2F"), // Тёмный оливково-зелёный
        QColor("#9932CC"), // Тёмный орхидейный
        QColor("#FF8C00"), // Тёмный оранжевый
        QColor("#7CFC00"), // Зелёный газон
        QColor("#DB7093"), // Бледный фиолетово-красный
        QColor("#48D1CC"), // Средний бирюзовый
        QColor("#DA70D6")  // Орхидейный
    };

    // Сортируем палитру по гармонии с акцентным цветом
    std::sort(m_colorPalette.begin(), m_colorPalette.end(),
              [accentHue](const QColor& a, const QColor& b) {
                  // Сначала сортируем по гармоничному расстоянию от акцентного
                  QColor aHsv = a.toHsv();
                  QColor bHsv = b.toHsv();

                  double diffA = qAbs(aHsv.hueF() - accentHue);
                  double diffB = qAbs(bHsv.hueF() - accentHue);

                  diffA = qMin(diffA, 1.0 - diffA);
                  diffB = qMin(diffB, 1.0 - diffB);

                  // Предпочитаем цвета на гармоничном расстоянии (0.33, 0.5, 0.67)
                  auto harmonicScore = [accentHue](double hue) -> double {
                      double diff = qAbs(hue - accentHue);
                      diff = qMin(diff, 1.0 - diff);

                      if (qAbs(diff - 0.33) < 0.05) return 0; // Триадная - лучшие
                      if (qAbs(diff - 0.5) < 0.05) return 1;  // Комплементарная
                      if (qAbs(diff - 0.67) < 0.05) return 2; // Триадная вторая
                      if (qAbs(diff - 0.17) < 0.05) return 3; // Аналогичная
                      return 4 + diff; // Остальные по расстоянию
                  };

                  return harmonicScore(aHsv.hueF()) < harmonicScore(bHsv.hueF());
              });
}

QColor ColorManager::getColorForObject(QObject* obj)
{
    if (!obj) {
        return QColor("#217346"); // Возвращаем акцентный цвет по умолчанию
    }

    // Проверяем, не зарегистрирован ли уже объект
    if (m_objectColors.contains(obj)) {
        return m_objectColors.value(obj);
    }

    // Очищаем разрушенные объекты
    cleanupDestroyedObjects();

    // Ищем свободный цвет в палитре
    QColor assignedColor;

    // Стратегия 1: Ищем первый свободный гармоничный цвет
    for (int i = 0; i < m_colorPalette.size(); ++i) {
        int index = (m_lastUsedIndex + i + 1) % m_colorPalette.size();
        QColor candidate = m_colorPalette[index];

        if (!m_usedColors.contains(candidate)) {
            assignedColor = candidate;
            m_lastUsedIndex = index;
            break;
        }
    }

    // Стратегия 2: Если все цвета заняты, находим наименее похожий на уже используемые
    if (!assignedColor.isValid()) {
        double bestScore = -1.0;

        for (const QColor& candidate : m_colorPalette) {
            // Вычисляем минимальное расстояние до уже используемых цветов
            double minDistance = 1.0;

            for (const QColor& usedColor : m_usedColors) {
                QColor candHsv = candidate.toHsv();
                QColor usedHsv = usedColor.toHsv();

                double hueDiff = qAbs(candHsv.hueF() - usedHsv.hueF());
                hueDiff = qMin(hueDiff, 1.0 - hueDiff);

                double satDiff = qAbs(candHsv.saturationF() - usedHsv.saturationF());
                double valDiff = qAbs(candHsv.valueF() - usedHsv.valueF());

                // Комбинированная метрика расстояния
                double distance = hueDiff * 0.6 + satDiff * 0.2 + valDiff * 0.2;

                if (distance < minDistance) {
                    minDistance = distance;
                }
            }

            if (minDistance > bestScore) {
                bestScore = minDistance;
                assignedColor = candidate;
            }
        }
    }

    // Стратегия 3: Если всё ещё нет цвета, создаём новый на основе последнего
    if (!assignedColor.isValid()) {
        QColor lastColor = m_colorPalette.isEmpty() ?
                               QColor("#217346") : m_colorPalette.last();
        QColor lastHsv = lastColor.toHsv();

        // Создаём новый оттенок, сдвигаясь на золотое сечение
        double newHue = fmod(lastHsv.hueF() + 0.618033988749895, 1.0);
        assignedColor = QColor::fromHsvF(newHue, 0.6, 0.8, 1.0);
    }

    // Регистрируем объект
    m_objectColors.insert(obj, assignedColor);
    m_usedColors.insert(assignedColor);

    // Подключаемся к сигналу destroyed для автоматической очистки
    connect(obj, &QObject::destroyed, this, &ColorManager::onObjectDestroyed);

    /*qDebug() << "Assigned color" << assignedColor.name() << "to object"
             << obj->objectName() << "(" << obj->metaObject()->className() << ")";*/

    return assignedColor;
}

void ColorManager::releaseColorForObject(QObject* obj)
{
    if (!obj || !m_objectColors.contains(obj)) {
        return;
    }

    QColor color = m_objectColors.value(obj);

    // Удаляем из всех структур
    m_objectColors.remove(obj);
    m_usedColors.remove(color);

    // Отключаем сигнал (автоматически при удалении объекта)

    /*qDebug() << "Released color" << color.name() << "from object"
             << obj->objectName();*/
}

void ColorManager::onObjectDestroyed(QObject* obj)
{
    // Объект уже разрушен, но указатель ещё валиден для поиска
    if (m_objectColors.contains(obj)) {
        QColor color = m_objectColors.value(obj);
        m_objectColors.remove(obj);
        m_usedColors.remove(color);

        /*qDebug() << "Auto-released color" << color.name()
                 << "from destroyed object";*/
    }
}

void ColorManager::cleanupDestroyedObjects()
{
    // Удаляем указатели на разрушенные объекты
    QList<QObject*> toRemove;

    for (auto it = m_objectColors.begin(); it != m_objectColors.end(); ++it) {
        if (!it.key()) { // Проверка нулевого указателя
            toRemove.append(it.key());
        }
    }

    for (QObject* obj : toRemove) {
        QColor color = m_objectColors.value(obj);
        m_objectColors.remove(obj);
        m_usedColors.remove(color);

        /*qDebug() << "Cleaned up color" << color.name()
                 << "from null object pointer";*/
    }
}
