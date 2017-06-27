#include "fmtfielddlg.h"
#include "ui_fmtfielddlg.h"
#include "fmtfildsmodel.h"
#include "fmtcore.h"
#include "fmttable.h"
#include <QDataWidgetMapper>

FmtFieldDlg::FmtFieldDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FmtFieldDlg)
{
    ui->setupUi(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    ui->typeCombo->setMaxVisibleItems(14);
    ui->typeCombo->addItems(fmtTypes());

    ui->nameEdit->setMaxLength(fmtm_FieldNameMaxSize);
    ui->commentEdit->setMaxLength(fmtm_FieldCommentMaxSize);

    connect(ui->firstButton, SIGNAL(clicked(bool)), mapper, SLOT(toFirst()));
    connect(ui->prevButton, SIGNAL(clicked(bool)), mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked(bool)), mapper, SLOT(toNext()));
    connect(ui->lastButton, SIGNAL(clicked(bool)), mapper, SLOT(toLast()));
    connect(mapper, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged(int)));
    connect(ui->typeCombo, SIGNAL(currentIndexChanged(int)), SLOT(typeChanged(int)));
}

FmtFieldDlg::~FmtFieldDlg()
{
    delete ui;
}

void FmtFieldDlg::setModel(FmtFildsModel *model)
{
    mapper->clearMapping();
    mapper->setModel(model);

    /*FmtTable *pTable = model->fmtTable();
    ui->toolUndo->setDefaultAction(pTable->undoAction());
    ui->toolRedo->setDefaultAction(pTable->redoAction());*/

    ResetMaping();
}

void FmtFieldDlg::setCurrentIndex(const int &index)
{
    mapper->setCurrentIndex(index);
}

void FmtFieldDlg::currentIndexChanged(const int &index)
{
    if (index == mapper->model()->rowCount() - 1)
    {
        setCurrentIndex(index - 1);
    }
    ui->commentEdit->home(false);
}

void FmtFieldDlg::ResetMaping()
{
    mapper->clearMapping();
    mapper->addMapping(ui->idEdit, FmtFildsModel::fld_Id);
    mapper->addMapping(ui->fmtIdEdit, FmtFildsModel::fld_FmtId);
    mapper->addMapping(ui->nameEdit, FmtFildsModel::fld_Name);
    mapper->addMapping(ui->sizeBox, FmtFildsModel::fld_Size);
    mapper->addMapping(ui->offsetBox, FmtFildsModel::fld_Offset);
    mapper->addMapping(ui->outlenBox, FmtFildsModel::fld_Outlen);
    mapper->addMapping(ui->decpointBox, FmtFildsModel::fld_Decpoint);
    mapper->addMapping(ui->commentEdit, FmtFildsModel::fld_Comment);
    mapper->addMapping(ui->ishiddenBox, FmtFildsModel::fld_Hidden);

    mapper->addMapping(ui->typeCombo, FmtFildsModel::fld_TypeIndex, "currentIndex");
}

void FmtFieldDlg::typeChanged(const int &index)
{
    /*int indx = mapper->currentIndex();
    QAbstractItemModel *model = mapper->model();
    mapper->setModel(NULL);

    mapper->setModel(model);
    ResetMaping();*/
    //ResetMaping();
    //mapper->model()->setData(mapper->model()->index(mapper->currentIndex(), FmtFildsModel::fld_Size), fmtTypeSize(fmtTypeFromIndex(index)));
    //ui->sizeBox->setValue(fmtTypeSize(v));
}
