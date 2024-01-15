#include "fmtsegmentflagsdlg.h"
#include "ui_fmtsegmentflagsdlg.h"
#include "fmtcore.h"
#include "fmtsegment.h"

#define FLAG_VALUE_PROP "FlagValue"

FmtSegmentFlagsDlg::FmtSegmentFlagsDlg(FmtSegment *Segment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FmtSegmentFlagsDlg),
    pSegment(Segment)
{
    ui->setupUi(this);

    ui->checkDuplicates->setProperty(FLAG_VALUE_PROP, fmtkf_Duplicates);
    ui->checkAlternate->setProperty(FLAG_VALUE_PROP, fmtkf_Alternate);
    ui->checkModifiable->setProperty(FLAG_VALUE_PROP, fmtkf_Modifiable);
    ui->checkDescending->setProperty(FLAG_VALUE_PROP, fmtkf_Descending);
    ui->checkBinary->setProperty(FLAG_VALUE_PROP, fmtkf_Binary);
    ui->checkSupplement->setProperty(FLAG_VALUE_PROP, fmtkf_Supplement);
    ui->checkNullVal->setProperty(FLAG_VALUE_PROP, fmtkf_NullVal);
    ui->checkExtended->setProperty(FLAG_VALUE_PROP, fmtkf_Extended);
    ui->checkSegment->setProperty(FLAG_VALUE_PROP, fmtkf_Segment);
    ui->checkManual->setProperty(FLAG_VALUE_PROP, fmtkf_Manual);
    ui->checkLocal->setProperty(FLAG_VALUE_PROP, fmtkf_Local);

    QList<QCheckBox*> widgetList = ui->checkContainer->findChildren<QCheckBox*>();
    foreach(QCheckBox *check, widgetList) {
        connect(check, SIGNAL(stateChanged(int)), SLOT(UpdateFlagValue()));
    }

#if QT_VERSION >= 0x050900
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
#endif

    CheckBySegment();

    connect(this, SIGNAL(accepted()), SLOT(ApplyFlags()));
}

FmtSegmentFlagsDlg::~FmtSegmentFlagsDlg()
{
    delete ui;
}

void FmtSegmentFlagsDlg::setReadOnly(bool value)
{
    ui->checkDuplicates->setEnabled(!value);
    ui->checkAlternate->setEnabled(!value);
    ui->checkModifiable->setEnabled(!value);
    ui->checkDescending->setEnabled(!value);
    ui->checkBinary->setEnabled(!value);
    ui->checkSupplement->setEnabled(!value);
    ui->checkNullVal->setEnabled(!value);
    ui->checkExtended->setEnabled(!value);
    ui->checkSegment->setEnabled(!value);
    ui->checkManual->setEnabled(!value);
    ui->checkLocal->setEnabled(!value);
}

void FmtSegmentFlagsDlg::UpdateFlagValue()
{
    QList<QCheckBox*> widgetList = ui->checkContainer->findChildren<QCheckBox*>();

    qint32 Flags = 0;
    foreach(QCheckBox *check, widgetList) {
        if (check->isChecked())
            Flags |= check->property(FLAG_VALUE_PROP).toInt();
    }

    ui->spinFlags->setValue(Flags);
}

void FmtSegmentFlagsDlg::CheckBySegment()
{
    if (pSegment)
    {
        quint32 Flags = pSegment->segmentFlags();

        if (Flags & fmtkf_Duplicates)
            ui->checkDuplicates->setChecked(true);

        if (Flags & fmtkf_Modifiable)
            ui->checkModifiable->setChecked(true);

        if (Flags & fmtkf_Binary)
            ui->checkBinary->setChecked(true);

        if (Flags & fmtkf_NullVal)
            ui->checkNullVal->setChecked(true);

        if (Flags & fmtkf_Segment)
            ui->checkSegment->setChecked(true);

        if (Flags & fmtkf_Alternate)
            ui->checkAlternate->setChecked(true);

        if (Flags & fmtkf_Descending)
            ui->checkDescending->setChecked(true);

        if (Flags & fmtkf_Supplement)
            ui->checkSupplement->setChecked(true);

        if (Flags & fmtkf_Extended)
            ui->checkExtended->setChecked(true);

        if (Flags & fmtkf_Manual)
            ui->checkManual->setChecked(true);

        if (Flags & fmtkf_Local)
            ui->checkLocal->setChecked(true);
    }
}

void FmtSegmentFlagsDlg::ApplyFlags()
{
    pSegment->setFlags(ui->spinFlags->value());
}
