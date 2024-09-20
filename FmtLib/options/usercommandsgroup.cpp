#include "usercommandsgroup.h"
#include "ui_usercommandsgroup.h"

UserCommandsGroup::UserCommandsGroup(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::UserCommandsGroup)
{
    ui->setupUi(this);
}

UserCommandsGroup::~UserCommandsGroup()
{
    delete ui;
}
