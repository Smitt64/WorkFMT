#ifndef FMTSEGMENTFLAGSDLG_H
#define FMTSEGMENTFLAGSDLG_H

#include <QDialog>

namespace Ui {
class FmtSegmentFlagsDlg;
}

class FmtSegment;
class FmtSegmentFlagsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FmtSegmentFlagsDlg(FmtSegment *Segment, QWidget *parent = nullptr);
    ~FmtSegmentFlagsDlg();

private slots:
    void UpdateFlagValue();
    void ApplyFlags();

private:
    void CheckBySegment();
    Ui::FmtSegmentFlagsDlg *ui;
    FmtSegment *pSegment;
};

#endif // FMTSEGMENTFLAGSDLG_H
