#ifndef SETTINGSVALUEDELEGATE_H
#define SETTINGSVALUEDELEGATE_H

#include <QStyledItemDelegate>

class SettingsValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SettingsValueDelegate(int typeColumn, QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

private:
    int m_typeColumn;
};

#endif // SETTINGSVALUEDELEGATE_H
