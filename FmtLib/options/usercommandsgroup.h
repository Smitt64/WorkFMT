#ifndef USERCOMMANDSGROUP_H
#define USERCOMMANDSGROUP_H

#include <QGroupBox>

namespace Ui {
class UserCommandsGroup;
}

class UserCommandsGroup : public QGroupBox
{
    Q_OBJECT

public:
    explicit UserCommandsGroup(QWidget *parent = nullptr);
    ~UserCommandsGroup();

private:
    Ui::UserCommandsGroup *ui;
};

#endif // USERCOMMANDSGROUP_H
