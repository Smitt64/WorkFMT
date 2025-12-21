#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include <SARibbon.h>
#include "fmtlib_global.h"

class QTreeView;
class FMTLIBSHARED_EXPORT TreeComboBox : public SARibbonComboBox
{
    Q_OBJECT
public:
    TreeComboBox(QWidget *parent = NULL);

    virtual bool eventFilter(QObject *object, QEvent *event);
    virtual void showPopup();
    virtual void hidePopup();

signals:
    void modelIndexChanged(const QModelIndex &index);

public slots:
    void setCurrentIndex(const QModelIndex &index);

private:
    QTreeView* pView;
    bool skipNextHide;
};

#endif // TREECOMBOBOX_H
