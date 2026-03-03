#ifndef FMTFIELDSTABLEVIEW_H
#define FMTFIELDSTABLEVIEW_H

#include <QtWidgets>
#include "fmtlib_global.h"

class QPixmap;
class FmtFildsModel;
class FMTLIBSHARED_EXPORT FmtFieldsTableView : public QTableView
{
    Q_OBJECT
public:
    explicit FmtFieldsTableView(QWidget *parent = NULL);
    virtual ~FmtFieldsTableView();

    void setButtonsVisible(bool value);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *ev) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private slots:
    void InsertClicked();
    void RemoveClicked();

private:
    void paintDeleteMark();
    QPushButton *pInsertButton, *pRemoveButton;
    QPoint m_Point;
    qint32 m_Index;
    FmtFildsModel *pFields;
    QScopedPointer<QPixmap> pattern;

    QPen penDeleteMark, penInsertMark;
    bool m_fDrawDeleteMark, m_fDrawInsertMark, m_fButtonsEnabled;
    void MoveButton();
};

#endif // FMTFIELDSTABLEVIEW_H
