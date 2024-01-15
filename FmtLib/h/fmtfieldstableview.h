#ifndef FMTFIELDSTABLEVIEW_H
#define FMTFIELDSTABLEVIEW_H

#include <QtWidgets>
#include "fmtlib_global.h"

class FmtFildsModel;
class FMTLIBSHARED_EXPORT FmtFieldsTableView : public QTableView
{
    Q_OBJECT
public:
    explicit FmtFieldsTableView(QWidget *parent = NULL);
    virtual ~FmtFieldsTableView();

    void setButtonsVisible(bool value);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *ev);
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void InsertClicked();
    void RemoveClicked();

private:
    void paintDeleteMark();
    QPushButton *pInsertButton, *pRemoveButton;
    QPoint m_Point;
    qint32 m_Index;
    FmtFildsModel *pFields;

    QPen penDeleteMark, penInsertMark;
    bool m_fDrawDeleteMark, m_fDrawInsertMark, m_fButtonsEnabled;
    void MoveButton();
};

#endif // FMTFIELDSTABLEVIEW_H
