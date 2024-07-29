#ifndef FMTOPTIONSDLG_H
#define FMTOPTIONSDLG_H

#include <optionsdlg/optionsdlg.h>

class QSettings;
class FmtOptionsDlg : public OptionsDlg
{
    Q_OBJECT

public:
    FmtOptionsDlg(QSettings *settings, QWidget *parent = nullptr);
    virtual ~FmtOptionsDlg();
};

#endif // FMTOPTIONSDLG_H
