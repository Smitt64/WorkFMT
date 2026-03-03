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
    FmtIndexTreeView(QWidget *parent = nullptr);
    ~FmtIndexTreeView();

    void setFmtTable(FmtTable *table);
    void setButtonsVisible(bool value);

    // Переопределенные методы
    virtual bool eventFilter(QObject *obj, QEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private slots:
    void InsertButton();
    void RemoveButton();

private:
    void MoveButton();
    void paintMarkers();

    QPoint m_Point;
    FmtTable *pFmtTable;
    QPushButton *pInsertButton;
    QPushButton *pRemoveButton;  // Возвращаем кнопку удаления
    FmtIndecesDelegate *pIndecesDelegate;
    bool m_fButtonsEnabled;

    QModelIndex m_Index;

    // Визуальные элементы
    QScopedPointer<QPixmap> pattern;  // Паттерн для штриховки
    QPen penDeleteMark;    // Перо для рамки удаления
    QPen penInsertMark;    // Перо для маркера вставки

    // Флаги отрисовки
    bool m_fDrawInsertMark;
    bool m_fDrawDeleteMark;
};

#endif // FMTINDEXTREEVIEW_H
