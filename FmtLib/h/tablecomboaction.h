#ifndef TABLECOMBOACTION_H
#define TABLECOMBOACTION_H

#include <QObject>
#include <QWidgetAction>

class QAbstractItemModel;
class TableComboAction : public QWidgetAction
{
    Q_OBJECT
public:
    TableComboAction(QObject *parent = NULL);
    void setModel(QAbstractItemModel *model);
    void setDisplayColumn(const quint32 &col);

signals:
    void activated(const int &id);

protected:
    void deleteWidget(QWidget *widget);
    QWidget *createWidget(QWidget *parent);

private:
    QAbstractItemModel *pModel;
    quint32 m_Col;
};

#endif // TABLECOMBOACTION_H
