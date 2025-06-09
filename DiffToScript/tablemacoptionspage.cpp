#include "tablemacoptionspage.h"
#include "ui_tablemacoptionspage.h"
#include "rslobj/tablefilesmodel.h"
#include "task.h"
#include <rsscript/registerobjlist.hpp>
#include <QtDebug>

TableMacOptionsPage::TableMacOptionsPage(QWidget *parent) :
    OptionsPage(parent),
    ui(new Ui::TableMacOptionsPage)
{
    //TableFilesModel model;
    //    model.loadFromDirectories(QStringList() << "path/to/dir1" << "path/to/dir2");

    pSettings = diffGetSettings();
    m_pModel.reset(new TableFilesModel(pSettings.data()));

    rslObjList()->applyMacroDirs();
    QStringList inc = rslObjList()->incDirs().split(";");
    m_pModel->loadFromDirectories(inc);

    ui->setupUi(this);

    ui->filesModel->setModel(m_pModel.data());
}

TableMacOptionsPage::~TableMacOptionsPage()
{
    delete ui;
}
