#ifndef UNDOLISTPOPUP_H
#define UNDOLISTPOPUP_H

#include <QWidget>
#include <QUndoView>
#include <QWidgetAction>

class QUndoStack;
class UndoListPopup : public QWidgetAction
{
    Q_OBJECT
public:
    explicit UndoListPopup(QObject *parent = 0);
    virtual ~UndoListPopup();

    void setStack(QUndoStack *stack);

protected:
    virtual QWidget *createWidget(QWidget *parent);
    virtual void deleteWidget(QWidget *widget);

private:
    QUndoStack *pStack;
};

#endif // UNDOLISTPOPUP_H
