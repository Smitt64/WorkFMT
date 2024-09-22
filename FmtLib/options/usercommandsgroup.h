#ifndef USERCOMMANDSGROUP_H
#define USERCOMMANDSGROUP_H

#include <QGroupBox>

namespace Ui {
class UserCommandsGroup;
}

class QMenu;
class ToolBarsStorage;
class QStyledItemDelegate;
class QSettings;
class UserCommandsGroup : public QGroupBox
{
    Q_OBJECT

public:
    explicit UserCommandsGroup(QWidget *parent = nullptr);
    ~UserCommandsGroup();

    void save(QSettings *settings);
    void load(QSettings *settings);

private slots:
    void on_buttonAddMacro_clicked();
    void onItemClicked(const QModelIndex &index);
    void onButtonUp();
    void onButtonDown();
    void onRemoveCommand();
    void onChangeLibIcon();
    void onChangeIconFromFile();
    void onChangeText();

private:
    void updateButtonsState();
    Ui::UserCommandsGroup *ui;
    QStyledItemDelegate *m_pDelegate;
    QScopedPointer<ToolBarsStorage> m_Storage;
    QMenu *m_pIcomMenu;
};

#endif // USERCOMMANDSGROUP_H
