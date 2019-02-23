#include "gencppsettings.h"
#include "ui_gencppsettings.h"
#include "fmtcore.h"
#include <QButtonGroup>

GenCppSettings::GenCppSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenCppSettings)
{
    ui->setupUi(this);
    pSetttings = settings();

    m_pUnionCommentGroup = new QButtonGroup(this);
    m_pUnionCommentGroup->addButton(ui->unionNoComment, uc_NoComment);
    m_pUnionCommentGroup->addButton(ui->unionCommentFld, uc_CommentFld);
    m_pUnionCommentGroup->addButton(ui->unionCommentSeg, uc_Commentseg);

    m_pFfDefaultCommentGroup = new QButtonGroup(this);
    m_pFfDefaultCommentGroup->addButton(ui->ffDefPkKey, ffdf_PkKey);
    m_pFfDefaultCommentGroup->addButton(ui->ffDefAinc, ffdf_AutoInk);

    GenCppSettingsParams Prm;
    ReadGenSettings(&Prm);

    ui->structGroupBox->setChecked(Prm.fGenStruct);
    ui->alignNamesStruct->setChecked(Prm.GenStruct.fAlignNames);
    ui->structComments->setChecked(Prm.GenStruct.fUseComments);

    ui->structGroupBox->setChecked(Prm.fGenUnion);
    ui->unionNameMask->setText(Prm.GenUnion.sNameMask);
    ui->uAlignNamesStruct->setChecked(Prm.GenUnion.fAlignNames);
    ui->segmentNameType->setCurrentIndex(Prm.GenUnion.iSegmentNameType);

    ui->skfGroupBox->setChecked(Prm.fGenSkf);
    ui->skfNameType->setCurrentIndex(Prm.SkfFunc.iNameType);
    ui->skfUniqKeys->setChecked(Prm.SkfFunc.fAllSkf);
    ui->skfDefault->setChecked(Prm.SkfFunc.fDefaultSkf);

    ui->enumGroupBox->setChecked(Prm.fGenEnum);
    ui->enumNameMask->setText(Prm.GenEnum.sNameMask);
    ui->enumNameType->setCurrentIndex(Prm.GenEnum.iNameType);
    ui->genBTRVFILE->setChecked(Prm.fGenBTRVFILE);

    ui->genFindFunctions->setChecked(Prm.fGenFindFunctions);
    ui->ffNameType->setCurrentIndex(Prm.FindFunc.iNameType);

    switch(Prm.GenUnion.iUseComments)
    {
    case uc_NoComment:
        ui->unionNoComment->setChecked(true);
        break;
    case uc_CommentFld:
        ui->unionCommentFld->setChecked(true);
        break;
    case uc_Commentseg:
        ui->unionCommentSeg->setChecked(true);
        break;
    }

    switch(Prm.FindFunc.iDefaultType)
    {
    case ffdf_PkKey:
        ui->ffDefPkKey->setChecked(true);
        break;
    case ffdf_AutoInk:
        ui->ffDefAinc->setChecked(true);
        break;
    }

    connect(this, SIGNAL(accepted()), SLOT(onAccept()));
}

GenCppSettings::~GenCppSettings()
{
    delete ui;
}

void GenCppSettings::onAccept()
{
    pSetttings->beginGroup("FmtGenCppTemplate");
    pSetttings->setValue("GenStruct", ui->structGroupBox->isChecked());
    pSetttings->setValue("GenStruct.AlignNames", ui->alignNamesStruct->isChecked());
    pSetttings->setValue("GenStruct.UseComments", ui->structComments->isChecked());

    pSetttings->setValue("GenUnion", ui->unionBox->isChecked());
    pSetttings->setValue("GenUnion.NameMask", ui->unionNameMask->text());
    pSetttings->setValue("GenUnion.AlignNames", ui->uAlignNamesStruct->isChecked());
    pSetttings->setValue("GenUnion.Comments", m_pUnionCommentGroup->checkedId());
    pSetttings->setValue("GenUnion.SegmentNameType", ui->segmentNameType->currentIndex());

    pSetttings->setValue("GenEnum", ui->enumGroupBox->isChecked());
    pSetttings->setValue("GenEnum.NameMask", ui->enumNameMask->text());
    pSetttings->setValue("GenEnum.NameType", ui->enumNameType->currentIndex());

    pSetttings->setValue("GenBTRVFILE", ui->genBTRVFILE->isChecked());
    pSetttings->setValue("GenOpenFunc", ui->genOpener->isChecked());
    pSetttings->setValue("GenFindFunc", ui->genFindFunctions->isChecked());

    pSetttings->setValue("GenFindFunc.NameType", ui->ffNameType->currentIndex());
    pSetttings->setValue("GenFindFunc.DefaultType", m_pFfDefaultCommentGroup->checkedId());

    pSetttings->setValue("GenSkf", ui->skfGroupBox->isChecked());
    pSetttings->setValue("GenSkf.AllSkf", ui->skfUniqKeys->isChecked());
    pSetttings->setValue("GenSkf.DefaultSkf", ui->skfDefault->isChecked());
    pSetttings->setValue("GenSkf.NameType", ui->skfNameType->currentIndex());

    pSetttings->endGroup();
}

void GenCppSettings::ReadGenSettings(GenCppSettingsParams *Prm)
{
    if (!Prm)
        return;

    QSettings *Setttings = settings();

    Setttings->beginGroup("FmtGenCppTemplate");
    Prm->fGenStruct = Setttings->value("GenStruct", true).toBool();
    Prm->GenStruct.fAlignNames = Setttings->value("GenStruct.AlignNames", true).toBool();
    Prm->GenStruct.fUseComments = Setttings->value("GenStruct.UseComments", true).toBool();

    Prm->fGenUnion = Setttings->value("GenUnion", true).toBool();
    Prm->GenUnion.sNameMask = Setttings->value("GenUnion.NameMask", "${StructName}_KEYS").toString();
    Prm->GenUnion.fAlignNames = Setttings->value("GenUnion.AlignNames", true).toBool();
    Prm->GenUnion.iSegmentNameType = Setttings->value("GenUnion.SegmentNameType", usn_Short).toInt();
    Prm->GenUnion.iUseComments = Setttings->value("GenUnion.Comments", 0).toInt();

    Prm->fGenEnum = Setttings->value("GenEnum", true).toBool();
    Prm->GenEnum.iNameType = Setttings->value("GenEnum.NameType", usn_Short).toInt();
    Prm->GenEnum.sNameMask = Setttings->value("GenEnum.NameMask", "${StructName}_KEYNUM").toString();

    Prm->fGenBTRVFILE = Setttings->value("GenBTRVFILE", true).toBool();
    Prm->fGenOpenFunc = Setttings->value("GenOpenFunc", true).toBool();
    Prm->fGenFindFunctions = Setttings->value("GenFindFunc", true).toBool();

    Prm->FindFunc.iNameType = Setttings->value("GenFindFunc.NameType", ffdf_PkKey).toInt();
    Prm->FindFunc.iDefaultType = Setttings->value("GenFindFunc.DefaultType", 0).toInt();

    Prm->fGenSkf = Setttings->value("GenSkf", true).toBool();
    Prm->SkfFunc.fAllSkf = Setttings->value("GenSkf.AllSkf", true).toBool();
    Prm->SkfFunc.fDefaultSkf = Setttings->value("GenSkf.DefaultSkf", true).toBool();
    Prm->SkfFunc.iNameType = Setttings->value("GenSkf.NameType", 0).toInt();
    Setttings->endGroup();
}
