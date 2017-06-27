#include "toolbtncombo.h"
#include <QStylePainter>
#include <QDebug>
#include <QMenu>
#include <QToolBar>
#include <QPaintEvent>
#include <QApplication>

ToolBtnCombo::ToolBtnCombo(QWidget *parent)
    : QComboBox(parent)
{
    qDebug() << parent->metaObject()->className();

    m_Type = ParentType::PT_OTHER;
    m_Hovered = false;
    m_Open = false;

    if (dynamic_cast<QMenu*>(parent))
        m_Type = ParentType::PT_MENU;
    else if (dynamic_cast<QToolBar*>(parent))
        m_Type = ParentType::PT_TOOLBAR;
    setMouseTracking(true);
}

ToolBtnCombo::~ToolBtnCombo()
{

}

void ToolBtnCombo::paintEvent(QPaintEvent *event)
{
    /*QStylePainter p(this);
    QStyle *s = QApplication::style();

    if (m_Type = ParentType::PT_MENU)
    {
        QStyleOption opt;
        QStyleOptionComboBox combo;
        QStyleOptionFrame frame;

        frame.init(this);
        combo.init(this);

        combo.currentText = currentText();
        combo.state = QStyle::State_Enabled | QStyle::State_Active;
        frame.lineWidth = s->pixelMetric(QStyle::PM_ComboBoxFrameWidth);

        if (m_Hovered || m_Open)
        {
            p.drawPrimitive(QStyle::PE_FrameMenu, frame);
            //btn.state |= QStyle::State_Selected;
        }
        opt.rect = s->subControlRect(QStyle::CC_ComboBox, &combo, QStyle::SC_ComboBoxArrow);
        //p.drawText(event->rect(), currentText()); QStyle::CE_ComboBoxLabel
        p.drawControl(QStyle::CE_ComboBoxLabel, combo);
        p.drawPrimitive(QStyle::PE_IndicatorButtonDropDown, opt);
    }
    else
    {*/
        QComboBox::paintEvent(event);
    //}
}

void ToolBtnCombo::mouseMoveEvent(QMouseEvent *event)
{
    m_Hovered = true;
    QComboBox::mouseMoveEvent(event);
}

void ToolBtnCombo::leaveEvent(QEvent *event)
{
    m_Hovered = false;
    QComboBox::leaveEvent(event);
}

void ToolBtnCombo::showPopup()
{
    m_Open = true;
    QComboBox::showPopup();
}

void ToolBtnCombo::hidePopup()
{
    m_Open = false;
    QComboBox::hidePopup();
}
