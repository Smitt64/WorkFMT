#ifndef FMTINDEXTREEVIEW_H
#define FMTINDEXTREEVIEW_H

#include <QtWidgets>
#include "fmtlib_global.h"

class FmtTable;
class FmtIndecesDelegate;
class FMTLIBSHARED_EXPORT FmtIndexTreeView : public QTreeView
{
    Q_OBJECT
public:
    FmtIndexTreeView(QWidget *parent = NULL);
    void setFmtTable(FmtTable *table);

    void setButtonsVisible(bool value);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);

private slots:
    void RemoveButton();
    void InsertButton();

private:
    void MoveButton();
    QPoint m_Point;
    FmtTable *pFmtTable;
    QPushButton *pRemoveButton, *pInsertButton;
    FmtIndecesDelegate *pIndecesDelegate;
    bool m_fButtonsEnabled;

    QModelIndex m_Index;
};

#endif // FMTINDEXTREEVIEW_H
