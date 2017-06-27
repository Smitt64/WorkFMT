#ifndef TOOLBTNCOMBO_H
#define TOOLBTNCOMBO_H

#include <QWidget>
#include <QComboBox>

class ToolBtnCombo : public QComboBox
{
    Q_OBJECT
public:
    ToolBtnCombo(QWidget *parent = NULL);  
    virtual ~ToolBtnCombo();
    virtual void showPopup();
    virtual void hidePopup();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);

private:
    enum ParentType
    {
        PT_OTHER = 0,
        PT_MENU,
        PT_TOOLBAR,
    };

    ParentType m_Type;
    bool m_Hovered, m_Open;
};

#endif // TOOLBTNCOMBO_H
