#include "MDIProxyStyle.h"
#include <QApplication>
#include <QPainterPath>
#include <QMdiSubWindow>
#include <QDebug>
#include <QStyleOptionTitleBar>
#include <QStyleFactory>
#include <QMdiArea>
#include <QPaintEvent>
#include <QStyleOptionButton>
#include <QFile>
#include <QPainterPath>
#include <QPixmap>
#include <QIcon>

// Иконки из SARibbon
MDIProxyStyle::MDIProxyStyle(QStyle *style)
    : QProxyStyle(style)
{
    mainAppColor = QColor::fromRgb(33, 115, 70);
    loadSARibbonIcons();
}

void MDIProxyStyle::loadSARibbonIcons()
{
    // Загрузка иконок из ресурсов SARibbon
    // Проверяем несколько возможных путей

    QStringList closeIconPaths = {
        ":/res/close.svg",
        ":/icon/close.svg",
        ":/images/close.svg",
        ":/SARibbon/res/close.svg"
    };

    QStringList minimizeIconPaths = {
        ":/res/minimize.svg",
        ":/icon/minimize.svg",
        ":/images/minimize.svg",
        ":/SARibbon/res/minimize.svg"
    };

    QStringList maximizeIconPaths = {
        ":/res/maximize.svg",
        ":/icon/maximize.svg",
        ":/images/maximize.svg",
        ":/SARibbon/res/maximize.svg"
    };

    QStringList restoreIconPaths = {
        ":/res/restore.svg",
        ":/icon/restore.svg",
        ":/images/restore.svg",
        ":/SARibbon/res/restore.svg"
    };

    // Загружаем иконку закрытия
    for (const QString &path : closeIconPaths) {
        if (QFile::exists(path)) {
            closeIcon = QIcon(path);
            if (!closeIcon.isNull()) break;
        }
    }

    // Fallback для иконки закрытия
    if (closeIcon.isNull()) {
        QPixmap closePixmap(16, 16);
        closePixmap.fill(Qt::transparent);
        QPainter painter(&closePixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::black, 2.0));
        painter.drawLine(3, 3, 13, 13);
        painter.drawLine(13, 3, 3, 13);
        closeIcon = QIcon(closePixmap);
    }

    // Загружаем иконку сворачивания
    for (const QString &path : minimizeIconPaths) {
        if (QFile::exists(path)) {
            minimizeIcon = QIcon(path);
            if (!minimizeIcon.isNull()) break;
        }
    }

    // Fallback для иконки сворачивания
    if (minimizeIcon.isNull()) {
        QPixmap minPixmap(16, 16);
        minPixmap.fill(Qt::transparent);
        QPainter painter(&minPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::black, 2.0));
        painter.drawLine(3, 8, 13, 8);
        minimizeIcon = QIcon(minPixmap);
    }

    // Загружаем иконку максимизации
    for (const QString &path : maximizeIconPaths) {
        if (QFile::exists(path)) {
            maximizeIcon = QIcon(path);
            if (!maximizeIcon.isNull()) break;
        }
    }

    // Fallback для иконки максимизации
    if (maximizeIcon.isNull()) {
        QPixmap maxPixmap(16, 16);
        maxPixmap.fill(Qt::transparent);
        QPainter painter(&maxPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::black, 1.5));
        painter.drawRect(3, 3, 10, 10);
        maximizeIcon = QIcon(maxPixmap);
    }

    // Загружаем иконку восстановления
    for (const QString &path : restoreIconPaths) {
        if (QFile::exists(path)) {
            restoreIcon = QIcon(path);
            if (!restoreIcon.isNull()) break;
        }
    }

    // Fallback для иконки восстановления
    if (restoreIcon.isNull()) {
        QPixmap restorePixmap(16, 16);
        restorePixmap.fill(Qt::transparent);
        QPainter painter(&restorePixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::black, 1.5));
        painter.drawRect(3, 3, 8, 8);
        painter.drawRect(5, 5, 8, 8);
        restoreIcon = QIcon(restorePixmap);
    }
}

void MDIProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                                  QPainter *painter, const QWidget *widget) const
{
    // Обрабатываем рамку для QMdiSubWindow
    if (element == PE_FrameWindow && widget && qobject_cast<const QMdiSubWindow*>(widget)) {
        drawMdiSubWindowFrame(option, painter, widget);
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void MDIProxyStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                       QPainter *painter, const QWidget *widget) const
{
    // Обработка заголовка для QMdiSubWindow
    if (control == CC_TitleBar && widget && qobject_cast<const QMdiSubWindow*>(widget)) {
        // Рисуем фон заголовка и текст
        drawMdiSubWindowTitleBar(option, painter, widget);

        // Также рисуем кнопки, если это QStyleOptionTitleBar
        if (const QStyleOptionTitleBar *titleBarOption = qstyleoption_cast<const QStyleOptionTitleBar*>(option)) {
            drawTitleBarButtons(titleBarOption, painter, widget);
        }
        return;
    }

    // Обработка кнопок управления MDI (в QMdiArea)
    /*if (control == CC_MdiControls) {
        drawMdiControls(option, painter, widget);
        return;
    }*/

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

void MDIProxyStyle::drawControl(ControlElement element, const QStyleOption *option,
                                QPainter *painter, const QWidget *widget) const
{
    // Вместо использования несуществующих констант, просто передаем управление базовому классу
    // Текст заголовка и кнопки рисуются в drawComplexControl для CC_TitleBar
    QProxyStyle::drawControl(element, option, painter, widget);
}

void MDIProxyStyle::drawTitleBarButton(const QStyleOptionTitleBar *option,
                                       QPainter *painter, const QWidget *widget,
                                       SubControl control, const QIcon &icon,
                                       bool isActive) const
{
    if (!(option->subControls & control)) return;

    // Получаем прямоугольник кнопки
    QRect buttonRect = subControlRect(CC_TitleBar, option, control, widget);
    if (!buttonRect.isValid()) return;

    // Определяем состояние кнопки
    bool isHovered = (option->activeSubControls & control) &&
                     (option->state & State_MouseOver);
    bool isPressed = isHovered && (option->state & State_Sunken);

    // Выбираем цвет фона
    QColor bgColor = Qt::transparent;

    if (isPressed) {
        bgColor = (control == SC_TitleBarCloseButton) ? closeButtonPress() : buttonPressColor();
    } else if (isHovered) {
        bgColor = (control == SC_TitleBarCloseButton) ? closeButtonHover() : buttonHoverColor();
    }

    // Рисуем фон (если нужно)
    if (bgColor != Qt::transparent) {
        QRect rc = buttonRect.adjusted(1, 1, -1, -1);
        rc.moveTop(2);
        painter->fillRect(rc, bgColor);
    }

    // Рисуем рамку только при наведении или нажатии
    /*if (isHovered || isPressed) {
        QPen borderPen(isPressed ? activeFrameColor() : borderColor(), 1);
        painter->setPen(borderPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(buttonRect.adjusted(1, 1, -2, -2));
    }*/

    // Рисуем иконку
    QIcon::Mode mode = isHovered ? QIcon::Active : QIcon::Normal;
    QIcon::State state = isPressed ? QIcon::On : QIcon::Off;

    // НОВОЕ: Увеличиваем отступ для иконки, так как кнопки теперь больше
    int titleBarHeight = pixelMetric(PM_TitleBarHeight, option, widget);
    int iconPadding = titleBarHeight / 4; // Отступ пропорционален высоте

    QRect iconRect = buttonRect.adjusted(iconPadding, iconPadding, -iconPadding, -iconPadding);
    icon.paint(painter, iconRect, Qt::AlignCenter, mode, state);
}

void MDIProxyStyle::drawTitleBarButtons(const QStyleOptionTitleBar *option,
                                        QPainter *painter, const QWidget *widget) const
{
    if (!painter || !option || !widget) return;

    const QMdiSubWindow *mdiSubWindow = qobject_cast<const QMdiSubWindow*>(widget);
    if (!mdiSubWindow) return;

    bool isActive = mdiSubWindow->isActiveWindow();
    bool isMinimized = mdiSubWindow->windowState().testFlag(Qt::WindowMinimized);
    bool isMaximized = mdiSubWindow->windowState().testFlag(Qt::WindowMaximized);

    // Определяем, какие кнопки нужно рисовать
    bool hasClose = option->titleBarFlags & Qt::WindowCloseButtonHint;
    bool hasMinimize = option->titleBarFlags & Qt::WindowMinimizeButtonHint;
    bool hasMaximize = option->titleBarFlags & Qt::WindowMaximizeButtonHint;

    // Рисуем кнопки в правильном порядке
    if (hasClose) {
        drawTitleBarButton(option, painter, widget, SC_TitleBarCloseButton,
                           closeIcon, isActive);
    }

    if (hasMaximize) {
        QIcon iconToUse = isMaximized ? restoreIcon : maximizeIcon;
        drawTitleBarButton(option, painter, widget, SC_TitleBarMaxButton,
                           iconToUse, isActive);
    }

    if (hasMinimize) {
        // Для свернутого окна рисуем кнопку восстановления
        if (isMinimized) {
            drawTitleBarButton(option, painter, widget, SC_TitleBarNormalButton,
                               restoreIcon, isActive);
        } else {
            drawTitleBarButton(option, painter, widget, SC_TitleBarMinButton,
                               minimizeIcon, isActive);
        }
    }
}

void MDIProxyStyle::drawMdiSubWindowTitleBar(const QStyleOptionComplex *option,
                                             QPainter *painter, const QWidget *widget) const
{
    if (!painter || !widget || !option) return;

    painter->save();

    const QMdiSubWindow *mdiSubWindow = qobject_cast<const QMdiSubWindow*>(widget);
    bool isActive = mdiSubWindow ? mdiSubWindow->isActiveWindow() : false;

    // Цвета заголовка
    QColor titleBarColor = isActive ? titleBarActive() : titleBarInactive();
    QColor titleTextColor = isActive ? titleTextActive() : titleTextInactive();
    QColor borderColorValue = isActive ? activeFrameColor() : borderColor();

    // Заливаем фон заголовка
    QRect titleRect = option->rect.adjusted(1, 1, 0, -1);
    painter->fillRect(titleRect, titleBarColor);

    // Рисуем границы заголовка
    QPen borderPen(borderColorValue, 2);
    painter->setPen(borderPen);

    // Верхняя граница
    painter->drawLine(titleRect.topLeft(), titleRect.topRight());

    // Боковые границы
    painter->drawLine(titleRect.topLeft(), QPoint(titleRect.left(), titleRect.top() + titleRect.height()));
    painter->drawLine(titleRect.topRight(), QPoint(titleRect.right(), titleRect.top() + titleRect.height()));

    // Получаем высоту заголовка и размер кнопок
    int titleBarHeight = pixelMetric(PM_TitleBarHeight, option, widget);
    int buttonSize = titleBarHeight; // Кнопки по высоте заголовка
    int frameWidth = pixelMetric(PM_MdiSubWindowFrameWidth, option, widget);

    // Количество кнопок
    int buttonCount = 0;
    const QStyleOptionTitleBar *titleBarOption = qstyleoption_cast<const QStyleOptionTitleBar*>(option);
    if (titleBarOption) {
        if (titleBarOption->titleBarFlags & Qt::WindowCloseButtonHint) buttonCount++;
        if (titleBarOption->titleBarFlags & Qt::WindowMaximizeButtonHint) buttonCount++;
        if (titleBarOption->titleBarFlags & Qt::WindowMinimizeButtonHint) buttonCount++;
    }

    // НОВОЕ: Ширина области для кнопок (кнопки вплотную друг к другу)
    int buttonsWidth = buttonCount * buttonSize;

    // Рисуем иконку окна (если есть)
    int iconOffset = 0;
    if (titleBarOption && !titleBarOption->icon.isNull()) {
        // Размер иконки - немного меньше высоты заголовка
        int iconSize = qMin(titleBarHeight - 8, 16);

        // Позиция иконки - от левой рамки с небольшим отступом
        int iconX = titleRect.left() + frameWidth + 4;
        int iconY = titleRect.top() + (titleRect.height() - iconSize) / 2;

        QRect iconRect(iconX, iconY, iconSize, iconSize);

        // Рисуем иконку
        QIcon::Mode mode = isActive ? QIcon::Active : QIcon::Normal;
        titleBarOption->icon.paint(painter, iconRect, Qt::AlignCenter, mode, QIcon::On);

        iconOffset = iconSize + 8;
    }

    // Текст заголовка
    painter->setPen(titleTextColor);
    QFont font = painter->font();
    font.setFamily("Segoe UI");
    font.setPointSize(9);
    font.setBold(isActive);
    painter->setFont(font);

    // НОВОЕ: Позиционируем текст
    // Левая граница: рамка + отступ + иконка
    int textLeft = titleRect.left() + frameWidth + 8 + iconOffset;
    // Правая граница: до начала кнопок (без дополнительных отступов)
    int textRight = titleRect.right() - buttonsWidth;

    QRect textRect(textLeft, titleRect.top(),
                   textRight - textLeft, titleRect.height());

    QString title;
    if (titleBarOption) {
        title = titleBarOption->text;
    } else {
        title = widget->windowTitle();
    }

    // Рисуем текст только если есть место
    if (textRect.width() > 20) {
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter,
                          painter->fontMetrics().elidedText(title, Qt::ElideRight, textRect.width()));
    }

    painter->restore();
}

void MDIProxyStyle::drawMdiControls(const QStyleOptionComplex *option,
                                    QPainter *painter, const QWidget *widget) const
{
    QStyleOptionButton btnOpt;
    btnOpt.QStyleOption::operator=(*option);
    btnOpt.state &= ~State_MouseOver;

    int bsx = 0;
    int bsy = 0;

    // Размер иконок кнопок
    const int buttonIconMetric = pixelMetric(PM_TitleBarButtonIconSize, &btnOpt, widget);
    const QSize buttonIconSize(buttonIconMetric, buttonIconMetric);

    // Кнопка закрытия MDI окна
    if (option->subControls & QStyle::SC_MdiCloseButton) {
        if (option->activeSubControls & QStyle::SC_MdiCloseButton && (option->state & State_Sunken)) {
            btnOpt.state |= State_Sunken;
            btnOpt.state &= ~State_Raised;
            bsx = pixelMetric(PM_ButtonShiftHorizontal, option);
            bsy = pixelMetric(PM_ButtonShiftVertical, option);
        } else {
            btnOpt.state |= State_Raised;
            btnOpt.state &= ~State_Sunken;
            bsx = 0;
            bsy = 0;
        }

        btnOpt.rect = subControlRect(CC_MdiControls, option, SC_MdiCloseButton, widget);

        // Рисуем фон кнопки в нашем стиле
        drawMdiControlButton(&btnOpt, painter, widget, SC_MdiCloseButton);

        // Рисуем иконку из ресурсов
        QIcon::Mode mode = (option->state & State_MouseOver) ? QIcon::Active : QIcon::Normal;
        QIcon::State state = (btnOpt.state & State_Sunken) ? QIcon::On : QIcon::Off;

        closeIcon.paint(painter, btnOpt.rect.translated(bsx, bsy), Qt::AlignCenter, mode, state);
    }

    // Кнопка восстановления MDI окна
    if (option->subControls & QStyle::SC_MdiNormalButton) {
        if (option->activeSubControls & QStyle::SC_MdiNormalButton && (option->state & State_Sunken)) {
            btnOpt.state |= State_Sunken;
            btnOpt.state &= ~State_Raised;
            bsx = pixelMetric(PM_ButtonShiftHorizontal, option);
            bsy = pixelMetric(PM_ButtonShiftVertical, option);
        } else {
            btnOpt.state |= State_Raised;
            btnOpt.state &= ~State_Sunken;
            bsx = 0;
            bsy = 0;
        }

        btnOpt.rect = subControlRect(CC_MdiControls, option, SC_MdiNormalButton, widget);

        // Рисуем фон кнопки в нашем стиле
        drawMdiControlButton(&btnOpt, painter, widget, SC_MdiNormalButton);

        // Рисуем иконку восстановления
        QIcon::Mode mode = (option->state & State_MouseOver) ? QIcon::Active : QIcon::Normal;
        QIcon::State state = (btnOpt.state & State_Sunken) ? QIcon::On : QIcon::Off;

        restoreIcon.paint(painter, btnOpt.rect.translated(bsx, bsy), Qt::AlignCenter, mode, state);
    }

    // Кнопка сворачивания MDI окна
    if (option->subControls & QStyle::SC_MdiMinButton) {
        if (option->activeSubControls & QStyle::SC_MdiMinButton && (option->state & State_Sunken)) {
            btnOpt.state |= State_Sunken;
            btnOpt.state &= ~State_Raised;
            bsx = pixelMetric(PM_ButtonShiftHorizontal, option);
            bsy = pixelMetric(PM_ButtonShiftVertical, option);
        } else {
            btnOpt.state |= State_Raised;
            btnOpt.state &= ~State_Sunken;
            bsx = 0;
            bsy = 0;
        }

        btnOpt.rect = subControlRect(CC_MdiControls, option, SC_MdiMinButton, widget);

        // Рисуем фон кнопки в нашем стиле
        drawMdiControlButton(&btnOpt, painter, widget, SC_MdiMinButton);

        // Рисуем иконку минимизации
        QIcon::Mode mode = (option->state & State_MouseOver) ? QIcon::Active : QIcon::Normal;
        QIcon::State state = (btnOpt.state & State_Sunken) ? QIcon::On : QIcon::Off;

        minimizeIcon.paint(painter, btnOpt.rect.translated(bsx, bsy), Qt::AlignCenter, mode, state);
    }
}

void MDIProxyStyle::drawMdiControlButton(const QStyleOptionButton *option,
                                         QPainter *painter, const QWidget *widget,
                                         SubControl button) const
{
    if (!painter || !option) return;

    painter->save();

    // Определяем состояние кнопки
    bool isPressed = option->state & State_Sunken;
    bool isHovered = option->state & State_MouseOver;

    // Цвет фона кнопки
    QColor bgColor = Qt::transparent;

    if (isPressed) {
        bgColor = buttonPressColor();
    } else if (isHovered) {
        // Для кнопки закрытия - красный при наведении
        if (button == SC_MdiCloseButton) {
            bgColor = closeButtonHover();
        } else {
            bgColor = buttonHoverColor();
        }
    }

    // Рисуем фон
    if (bgColor != Qt::transparent) {
        painter->fillRect(option->rect, bgColor);
    }

    // Рисуем рамку при hover или pressed
    if (isHovered || isPressed) {
        QPen borderPen(isPressed ? activeFrameColor() : borderColor(), 1);
        painter->setPen(borderPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(option->rect.adjusted(0, 2, -2, -2));
    }

    painter->restore();
}

void MDIProxyStyle::drawMdiSubWindowFrame(const QStyleOption *option,
                                          QPainter *painter, const QWidget *widget) const
{
    if (!painter || !widget) return;

    painter->save();

    const QMdiSubWindow *mdiSubWindow = qobject_cast<const QMdiSubWindow*>(widget);
    bool isActive = mdiSubWindow ? mdiSubWindow->isActiveWindow() : false;

    // Цвет рамки в зависимости от активности
    QColor frameColor = isActive ? activeFrameColor() : inactiveFrameColor();

    // Рисуем зеленую рамку (3 пикселя)
    int FarameWidth = pixelMetric(PM_MdiSubWindowFrameWidth, option, widget);
    QPen framePen(frameColor, FarameWidth + 1);
    painter->setPen(framePen);
    painter->setBrush(Qt::NoBrush);

    QRect frameRect = widget->rect().adjusted(FarameWidth, FarameWidth, -FarameWidth, -FarameWidth);
    painter->drawRect(frameRect);

    // Если окно активно, добавляем дополнительный акцент
    if (isActive) {
        QPen accentPen(activeFrameColor().lighter(120), 1);
        painter->setPen(accentPen);
        QRect accentRect = frameRect.adjusted(FarameWidth, FarameWidth, -FarameWidth, -FarameWidth);
        painter->drawRect(accentRect);
    }

    painter->restore();
}

QSize MDIProxyStyle::sizeFromContents(ContentsType type, const QStyleOption *option,
                                      const QSize &size, const QWidget *widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);

    // Добавляем место для рамки MDI окон
    if (widget && qobject_cast<const QMdiSubWindow*>(widget)) {
        s.rwidth() += 6;
        s.rheight() += 6;
    }

    return s;
}

int MDIProxyStyle::pixelMetric(PixelMetric metric, const QStyleOption *option,
                               const QWidget *widget) const
{
    // Настройка толщины рамки для QMdiSubWindow
    if (metric == PM_MdiSubWindowFrameWidth && widget && qobject_cast<const QMdiSubWindow*>(widget)) {
        return 1;
    }

    if (metric == PM_MdiSubWindowMinimizedWidth && widget && qobject_cast<const QMdiSubWindow*>(widget)) {
        return 300;
    }

    // НОВОЕ: Увеличиваем высоту заголовка для больших кнопок
    if (metric == PM_TitleBarHeight && widget && qobject_cast<const QMdiSubWindow*>(widget)) {
        return 34; // Увеличиваем для лучшего вида больших кнопок
    }

    // Размер иконок кнопок MDI
    if (metric == PM_TitleBarButtonIconSize) {
        return 16; // Увеличиваем иконки для больших кнопок
    }

    // Размер кнопок заголовка
    if (metric == PM_TitleBarButtonSize && widget && qobject_cast<const QMdiSubWindow*>(widget)) {
        int titleBarHeight = QProxyStyle::pixelMetric(PM_TitleBarHeight, option, widget);
        return titleBarHeight; // Кнопки по высоте заголовка
    }

    // Сдвиг для нажатых кнопок
    if (metric == PM_ButtonShiftHorizontal || metric == PM_ButtonShiftVertical) {
        return 1;
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

QRect MDIProxyStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                                    SubControl sc, const QWidget *widget) const
{
    // Для кнопок заголовка QMdiSubWindow
    if (cc == CC_TitleBar && widget && qobject_cast<const QMdiSubWindow*>(widget)) {
        if (const QStyleOptionTitleBar *titleBarOpt = qstyleoption_cast<const QStyleOptionTitleBar*>(opt)) {
            int titleBarHeight = pixelMetric(PM_TitleBarHeight, titleBarOpt, widget);
            // НОВОЕ: Размер кнопок РАВЕН высоте заголовка
            int buttonSize = titleBarHeight;
            int frameWidth = pixelMetric(PM_MdiSubWindowFrameWidth, titleBarOpt, widget);

            // НОВОЕ: Нет отступов сверху - кнопки по всей высоте заголовка
            int verticalOffset = 0; // Убираем отступ сверху

            // Определяем, свернуто ли окно
            const QMdiSubWindow *window = qobject_cast<const QMdiSubWindow*>(widget);
            bool isMinimized = window && window->windowState().testFlag(Qt::WindowMinimized);

            // НОВОЕ: кнопка закрытия вплотную к правой рамке БЕЗ отступов
            // Используем правую границу заголовка без дополнительных отступов
            int x = opt->rect.right(); // Правая граница заголовка
            int y = opt->rect.top() + verticalOffset; // Верх заголовка

            // ВАЖНО: всегда рисуем одинаковое количество кнопок в одинаковых позициях
            // Порядок (справа налево): Закрыть -> Во весь экран -> Свернуть/Восстановить

            // 1. Закрытие (вплотную к правой границе, без отступов)
            if (titleBarOpt->titleBarFlags & Qt::WindowCloseButtonHint) {
                QRect closeRect(x - buttonSize, y, buttonSize, buttonSize);
                if (sc == SC_TitleBarCloseButton) {
                    return closeRect;
                }
                x -= buttonSize; // Без отступа между кнопками
            }

            // 2. Во весь экран/Восстановить из максимизированного
            if (titleBarOpt->titleBarFlags & Qt::WindowMaximizeButtonHint) {
                QRect maxRect(x - buttonSize, y, buttonSize, buttonSize);
                if (sc == SC_TitleBarMaxButton) {
                    return maxRect;
                }
                x -= buttonSize; // Без отступа между кнопками
            }

            // 3. Свернуть/Восстановить из свернутого
            if (titleBarOpt->titleBarFlags & Qt::WindowMinimizeButtonHint) {
                QRect restoreRect(x - buttonSize, y, buttonSize, buttonSize);
                if (isMinimized) {
                    if (sc == SC_TitleBarNormalButton) {
                        return restoreRect;
                    }
                } else {
                    if (sc == SC_TitleBarMinButton) {
                        return restoreRect;
                    }
                }
            }

            // Для SC_TitleBarLabel возвращаем область заголовка без кнопок
            if (sc == SC_TitleBarLabel) {
                // Вычисляем общую ширину кнопок
                int totalButtonsWidth = 0;
                int buttonCount = 0;

                if (titleBarOpt->titleBarFlags & Qt::WindowCloseButtonHint) buttonCount++;
                if (titleBarOpt->titleBarFlags & Qt::WindowMaximizeButtonHint) buttonCount++;
                if (titleBarOpt->titleBarFlags & Qt::WindowMinimizeButtonHint) buttonCount++;

                totalButtonsWidth = buttonCount * buttonSize; // Без отступов между кнопками

                // Область для текста от левой рамки до начала кнопок
                return opt->rect.adjusted(frameWidth + 4, verticalOffset,
                                          -totalButtonsWidth, 0);
            }
        }
    }

    // Для кнопок управления MDI в QMdiArea
    /*if (cc == CC_MdiControls) {
        int buttonSize = 28;
        int spacing = 2;

        switch (sc) {
        case SC_MdiCloseButton:
            return QRect(opt->rect.width() - buttonSize, 0, buttonSize, buttonSize);

        case SC_MdiNormalButton:
            return QRect(opt->rect.width() - 2 * buttonSize - spacing, 0, buttonSize, buttonSize);

        case SC_MdiMinButton:
            return QRect(opt->rect.width() - 3 * buttonSize - 2 * spacing, 0, buttonSize, buttonSize);

        default:
            break;
        }
    }*/

    return QProxyStyle::subControlRect(cc, opt, sc, widget);
}

QStyle::SubControl MDIProxyStyle::hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex *opt,
                                                const QPoint &pos, const QWidget *widget) const
{
    // Для заголовка QMdiSubWindow
    const QMdiSubWindow *window = qobject_cast<const QMdiSubWindow*>(widget);
    if (cc == CC_TitleBar && widget && window) {
        if (const QStyleOptionTitleBar *titleBarOpt = qstyleoption_cast<const QStyleOptionTitleBar*>(opt)) {
            // Проверяем кнопки в правильном порядке (справа налево)

            // 1. Кнопка закрытия
            QRect closeRect = subControlRect(cc, opt, SC_TitleBarCloseButton, widget);
            if (closeRect.isValid() && closeRect.contains(pos)) {
                //qDebug() << "SC_TitleBarCloseButton";
                return SC_TitleBarCloseButton;
            }

            // 2. Кнопка максимизации/восстановления
            QRect maxRect = subControlRect(cc, opt, SC_TitleBarMaxButton, widget);
            if (maxRect.isValid() && maxRect.contains(pos)) {
                //qDebug() << "SC_TitleBarMaxButton";
                return SC_TitleBarMaxButton;
            }

            // 3. Кнопка минимизации/восстановления
            if (window->windowState().testFlag(Qt::WindowMinimized))
            {
                QRect normalRect = subControlRect(cc, opt, SC_TitleBarNormalButton, widget);
                if (normalRect.isValid() && normalRect.contains(pos)) {
                    //qDebug() << "SC_MdiNormalButton";
                    return SC_TitleBarNormalButton;
                }
            }
            else
            {
                QRect minRect = subControlRect(cc, opt, SC_TitleBarMinButton, widget);
                if (minRect.isValid() && minRect.contains(pos)) {
                    //qDebug() << "SC_TitleBarMinButton";
                    return SC_TitleBarMinButton;
                }
            }

            // Если клик не по кнопкам, но в области заголовка
            return SC_TitleBarLabel;
        }
    }

    // Для MDI контролов в QMdiArea
    if (cc == CC_MdiControls) {
        // Проверяем кнопки управления MDI
        QRect closeRect = subControlRect(cc, opt, SC_MdiCloseButton, widget);
        if (closeRect.isValid() && closeRect.contains(pos)) {
            return SC_MdiCloseButton;
        }

        QRect normalRect = subControlRect(cc, opt, SC_MdiNormalButton, widget);
        if (normalRect.isValid() && normalRect.contains(pos)) {

            return SC_MdiNormalButton;
        }

        QRect minRect = subControlRect(cc, opt, SC_MdiMinButton, widget);
        if (minRect.isValid() && minRect.contains(pos)) {
            return SC_MdiMinButton;
        }
    }

    // По умолчанию используем базовый класс
    return QProxyStyle::hitTestComplexControl(cc, opt, pos, widget);
    //QStyle::SubControl ctrl = QProxyStyle::hitTestComplexControl(cc, opt, pos, widget);

    /*if (cc == CC_TitleBar && qobject_cast<const QMdiSubWindow*>(widget))
        qDebug() << ctrl;*/

    //return ctrl;
}

int MDIProxyStyle::styleHint(QStyle::StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    if (hint == QStyle::SH_WindowFrame_Mask)
    {
        int result = QProxyStyle::styleHint(hint, option, widget, returnData);

        if (returnData)
        {
            QStyleHintReturnMask *frameMask = (QStyleHintReturnMask*)returnData;
            frameMask->region = QRegion();
        }

        return result;
    }

    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

void MDIProxyStyle::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);

    // Применяем дополнительные настройки для QMdiSubWindow
    if (qobject_cast<QMdiSubWindow*>(widget)) {
        widget->setAttribute(Qt::WA_Hover, true);
    }

    // Применяем настройки для QMdiArea
    if (QMdiArea *mdiArea = qobject_cast<QMdiArea*>(widget)) {
        mdiArea->setAttribute(Qt::WA_Hover, true);
    }
}

void MDIProxyStyle::unpolish(QWidget *widget)
{
    QProxyStyle::unpolish(widget);
}

QPalette MDIProxyStyle::standardPalette() const
{
    QPalette pal = QProxyStyle::standardPalette(); // Начинаем с палитры базового стиля

    QColor mainColor = mainAppColor; // Используем наш основной цвет #217346
    QColor lightColor = mainColor.lighter(240);  // #D5F0E2 - как в QSS SARibbonQuickAccessBar
    QColor lighterColor = mainColor.lighter(280); // Ещё светлее, напр. #E8F7F0
    QColor midColor = mainColor.darker(120);     // Средний тон для неактивных элементов
    QColor darkColor = mainColor.darker(180);    // Темнее для границ неактивных
    QColor highlightColor = mainColor;           // Основной цвет для активных элементов (Highlight) #217346
    QColor highlightedTextColor = Qt::white;     // Цвет текста на активных элементах
    QColor windowBgColor = QColor("#F8F8F8");    // Цвет фона окна из QSS
    QColor windowTextColor = QColor("#666666");  // Цвет текста окна из QSS
    QColor buttonBgColor = QColor("#FFFFFF");    // Цвет фона кнопки из QSS
    QColor buttonTextColor = QColor("#333333");  // Цвет текста кнопки из QSS
    QColor disabledTextColor = QColor("#B2B2B2"); // Цвет отключенного текста из QSS

    // --- Основные цвета окна ---
    pal.setColor(QPalette::Window, windowBgColor);
    pal.setColor(QPalette::WindowText, windowTextColor);

    // --- Цвета текста ---
    pal.setColor(QPalette::Text, windowTextColor);
    pal.setColor(QPalette::ButtonText, buttonTextColor);
    pal.setColor(QPalette::PlaceholderText, disabledTextColor);

    // --- Цвета фона ---
    pal.setColor(QPalette::Base, Qt::white); // Стандартный фон редактируемых полей
    pal.setColor(QPalette::AlternateBase, windowBgColor.lighter(110)); // Альтернативный фон списков

    // --- Цвета акцента (Highlight) ---
    // *** ИСПРАВЛЕНО: Устанавливаем цвет для Active и Inactive ***
    pal.setColor(QPalette::Active, QPalette::Highlight, highlightColor);      // #217346
    pal.setColor(QPalette::Inactive, QPalette::Highlight, midColor);          // Или buttonBgColor, но midColor тоже логично
    pal.setColor(QPalette::Active, QPalette::HighlightedText, highlightedTextColor); // #FFFFFF
    pal.setColor(QPalette::Inactive, QPalette::HighlightedText, Qt::black); // Или buttonTextColor

    // --- Цвета кнопок ---
    pal.setColor(QPalette::Button, buttonBgColor);
    pal.setColor(QPalette::Light, lightColor); // #D5F0E2 - светлый акцент
    pal.setColor(QPalette::Mid, midColor);     // Средний тон
    pal.setColor(QPalette::Dark, darkColor);   // Темный тон

    // --- Цвета для неактивного состояния (Window, Text, ButtonText уже установлены выше) ---
    pal.setColor(QPalette::Inactive, QPalette::Window, windowBgColor);
    pal.setColor(QPalette::Inactive, QPalette::WindowText, windowTextColor.darker(120)); // Немного темнее
    pal.setColor(QPalette::Inactive, QPalette::Text, windowTextColor.darker(120));
    pal.setColor(QPalette::Inactive, QPalette::ButtonText, buttonTextColor.darker(120));
    // Highlight и HighlightedText уже установлены выше для Inactive

    // --- Цвета для отключенных элементов ---
    pal.setColor(QPalette::Disabled, QPalette::WindowText, disabledTextColor);
    pal.setColor(QPalette::Disabled, QPalette::Text, disabledTextColor);
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, disabledTextColor);
    // Disabled Highlight часто делается как Base
    pal.setColor(QPalette::Disabled, QPalette::Highlight, pal.color(QPalette::Base));
    pal.setColor(QPalette::Disabled, QPalette::HighlightedText, Qt::black); // Или buttonTextColor

    // --- Цвета для рамок и деталей ---
    pal.setColor(QPalette::Midlight, lighterColor); // #E8F7F0 или аналогичный

    return pal;
}
