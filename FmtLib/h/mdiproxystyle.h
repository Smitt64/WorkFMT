#ifndef MDIPROXYSTYLE_H
#define MDIPROXYSTYLE_H

#include <QProxyStyle>
#include <QPainter>
#include <QMdiSubWindow>
#include <QIcon>
#include "fmtlib_global.h"

class QPaintEvent;
class QStyleOptionButton;
class QStyleOptionTitleBar;
class MDIProxyStyle : public QProxyStyle
{
    Q_OBJECT

public:
    MDIProxyStyle(QStyle *style = nullptr);
    ~MDIProxyStyle() override = default;

    QPalette standardPalette() const;

    // Рисуем кастомную рамку для QMdiSubWindow
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = nullptr) const override;

    // Рисуем сложные элементы управления
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget = nullptr) const override;

    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget = nullptr) const override;

    // Возвращаем размеры элементов
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget = nullptr) const override;

    // Возвращаем стили пиксельных метрик
    int pixelMetric(PixelMetric metric, const QStyleOption *option,
                    const QWidget *widget = nullptr) const override;

    // Возвращаем прямоугольник для подэлементов
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget = nullptr) const override;

    SubControl hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex *opt,
                                                    const QPoint &pos, const QWidget *widget) const;

    int styleHint(QStyle::StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override;

    // Обрабатываем события рисования
    void polish(QWidget *widget) override;
    void unpolish(QWidget *widget) override;

    // Загрузка иконок из ресурсов SARibbon
    void loadSARibbonIcons();

private:
    // Иконки для кнопок (загружаются из SARibbon)
    QIcon closeIcon;
    QIcon minimizeIcon;
    QIcon maximizeIcon;
    QIcon restoreIcon;

    QColor mainAppColor;

    // Рисуем зеленую рамку для QMdiSubWindow
    void drawMdiSubWindowFrame(const QStyleOption *option,
                               QPainter *painter, const QWidget *widget = nullptr) const;

    // Рисуем заголовок QMdiSubWindow
    void drawMdiSubWindowTitleBar(const QStyleOptionComplex *option,
                                  QPainter *painter, const QWidget *widget = nullptr) const;

    // Рисуем кнопки управления MDI (в QMdiArea)
    void drawMdiControls(const QStyleOptionComplex *option,
                         QPainter *painter, const QWidget *widget = nullptr) const;

    // Рисуем кнопку управления MDI
    void drawMdiControlButton(const QStyleOptionButton *option,
                              QPainter *painter, const QWidget *widget,
                              SubControl button) const;

    // Рисуем кнопки заголовка окна
    void drawTitleBarButtons(const QStyleOptionTitleBar *option,
                             QPainter *painter, const QWidget *widget) const;

    void drawTitleBarButton(const QStyleOptionTitleBar *option,
                            QPainter *painter, const QWidget *widget,
                            SubControl control, const QIcon &icon,
                            bool isActive) const;

    // Вспомогательные функции для получения цветов
    QColor activeFrameColor() const { return QColor(33, 115, 70); }     // #217346
    QColor inactiveFrameColor() const { return QColor(168, 230, 189); } // #a8e6bd
    QColor titleBarActive() const { return QColor(255, 255, 255); }     // Белый
    QColor titleBarInactive() const { return QColor(248, 248, 248); }   // #f8f8f8
    QColor titleTextActive() const { return QColor(33, 115, 70); }      // #217346
    QColor titleTextInactive() const { return QColor(102, 102, 102); }  // #666666
    QColor borderColor() const { return QColor(197, 210, 224); }        // #c5d2e0
    QColor buttonHoverColor() const { return QColor(213, 240, 226); }   // #d5f0e2
    QColor buttonPressColor() const { return QColor(160, 220, 180); }   // #a0dcb4
    QColor closeButtonHover() const { return QColor(232, 17, 35); }     // #e81123
    QColor closeButtonPress() const { return QColor(200, 0, 20); }      // #c80014
};

#endif // MDIPROXYSTYLE_H
