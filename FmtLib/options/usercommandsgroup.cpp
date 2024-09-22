#include "usercommandsgroup.h"
#include "commandsoptions.h"
#include "commandsstorage.h"
#include "ui_usercommandsgroup.h"
#include "iconlibdlg.h"
#include <QStyledItemDelegate>
#include <QMessageBox>
#include <QMenu>
#include <QInputDialog>
#include <QFileDialog>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

UserCommandsGroup::UserCommandsGroup(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::UserCommandsGroup)
{
    ui->setupUi(this);

    m_Storage.reset(new ToolBarsStorage());
    GetMenuStyledItemDelegate(&m_pDelegate, this);

    ui->listView->setItemDelegate(m_pDelegate);
    ui->listView->setModel(m_Storage->model());

    updateButtonsState();

    m_pIcomMenu = new QMenu(this);

    QAction *addFromLib = m_pIcomMenu->addAction(tr("Добавить из библиотеки"));
    QAction *addFromfile = m_pIcomMenu->addAction(tr("Добавить из файла"));
    ui->buttonIcon->setMenu(m_pIcomMenu);

    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    connect(ui->buttonUp, SIGNAL(clicked()), this, SLOT(onButtonUp()));
    connect(ui->buttonDown, SIGNAL(clicked()), this, SLOT(onButtonDown()));
    connect(ui->buttonDelAction, SIGNAL(clicked()), this, SLOT(onRemoveCommand()));
    connect(ui->buttonName, SIGNAL(clicked()), this, SLOT(onChangeText()));

    connect(addFromLib, SIGNAL(triggered()), this, SLOT(onChangeLibIcon()));
    connect(addFromfile, SIGNAL(triggered()), this, SLOT(onChangeIconFromFile()));
}

UserCommandsGroup::~UserCommandsGroup()
{
    delete ui;
}

void UserCommandsGroup::updateButtonsState()
{
    ui->buttonAddMacro->setEnabled(true);

    QModelIndex index = ui->listView->currentIndex();
    if (index.isValid())
    {
        ui->buttonDelAction->setEnabled(true);

        if (ui->listView->currentIndex().row() == 0)
            ui->buttonUp->setEnabled(false);
        else
        {
            if (ui->listView->model()->rowCount() != 1)
                ui->buttonUp->setEnabled(true);
            else
                ui->buttonUp->setEnabled(false);
        }

        if (ui->listView->currentIndex().row() == ui->listView->model()->rowCount() - 1)
            ui->buttonDown->setEnabled(false);
        else
        {
            if (ui->listView->model()->rowCount() != 1)
                ui->buttonDown->setEnabled(true);
            else
                ui->buttonDown->setEnabled(false);
        }

        ToolBarsStorage *toolbar = m_Storage.data();

        if (toolbar->action(index.row())->ref)
        {
            ui->buttonIcon->setEnabled(false);
            ui->buttonName->setEnabled(false);
        }
        else
        {
            ui->buttonIcon->setEnabled(true);
            ui->buttonName->setEnabled(true);
        }
    }
    else
    {
        ui->buttonDelAction->setEnabled(false);
        ui->buttonUp->setEnabled(false);
        ui->buttonDown->setEnabled(false);
    }
}

void UserCommandsGroup::on_buttonAddMacro_clicked()
{
    CommandsOnAddMacro(m_Storage.data(), this);
}

void UserCommandsGroup::onItemClicked(const QModelIndex &index)
{
    updateButtonsState();
}

void UserCommandsGroup::onButtonUp()
{
    ToolBarsStorage *toolbar = m_Storage.data();

    QModelIndex index = ui->listView->currentIndex();
    toolbar->moveUp(index.row());

    index = ui->listView->model()->index(index.row() - 1, 0);
    ui->listView->setCurrentIndex(index);

    updateButtonsState();
}

void UserCommandsGroup::onButtonDown()
{
    ToolBarsStorage *toolbar = m_Storage.data();

    QModelIndex index = ui->listView->currentIndex();
    toolbar->moveDown(index.row());

    index = ui->listView->model()->index(index.row() + 1, 0);
    ui->listView->setCurrentIndex(index);

    updateButtonsState();
}

void UserCommandsGroup::onRemoveCommand()
{
    QModelIndex index = ui->listView->currentIndex();
    if (!index.isValid())
        return;

    if (QMessageBox::question(this, tr("Удалить команду"),
                              tr("Вы действительно хотите удалить команду <b>&#8220;%1&#8220;</b>?")
                                  .arg(index.data().toString())) == QMessageBox::Yes)
    {
        ToolBarsStorage *toolbar = m_Storage.data();
        toolbar->removeAction(index.row());

        updateButtonsState();
    }
}

void UserCommandsGroup::onChangeLibIcon()
{
    IconLibDlg dlg(this);
    QModelIndex index = ui->listView->currentIndex();
    if (dlg.exec() == QDialog::Accepted)
    {
        ToolBarsStorage *toolbar = m_Storage.data();
        toolbar->setIcon(ui->listView->currentIndex().row(), dlg.getIconPath());
    }
    ui->listView->setCurrentIndex(index);
}

void UserCommandsGroup::onChangeIconFromFile()
{
    QModelIndex index = ui->listView->currentIndex();
    QString path = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QString(),
                                                tr("Изображения (*.png *.jpeg *.bmp)"));

    if (path.isEmpty())
        return;

    ToolBarsStorage *toolbar = m_Storage.data();
    toolbar->setIcon(ui->listView->currentIndex().row(), path);
    ui->listView->setCurrentIndex(index);
}

void UserCommandsGroup::onChangeText()
{
    QModelIndex index = ui->listView->currentIndex();
    ToolBarsStorage *toolbar = m_Storage.data();

    QString oldname = toolbar->action(ui->listView->currentIndex().row())->title;
    QString newname = QInputDialog::getText(this, tr("Изменить название"), tr("Новое название: "), QLineEdit::Normal, oldname);

    if (newname.isEmpty())
        return;

    toolbar->setActionName(index.row(), newname);
    ui->listView->setCurrentIndex(index);
}

void UserCommandsGroup::save(QSettings *settings)
{
    QJsonDocument doc;

    QJsonObject root;
    QJsonArray actions = m_Storage->save();
    root.insert("UserActions", actions);
    doc.setObject(root);

    QByteArray data = doc.toJson();
    settings->beginGroup("UserCommands");
    settings->setValue("UserActions", QString::fromLocal8Bit(data.toBase64()));
    settings->endGroup();
}

void UserCommandsGroup::load(QSettings *settings)
{
    settings->beginGroup("UserCommands");
    QByteArray data = QByteArray::fromBase64(settings->value("UserActions").toString().toLocal8Bit());

    if (!data.isEmpty())
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject root = doc.object();

        QJsonArray actions = root["UserActions"].toArray();
        m_Storage->load(actions);
    }
    settings->endGroup();
}
