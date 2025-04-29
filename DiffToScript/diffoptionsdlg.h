#ifndef DIFFOPTIONSDLG_H
#define DIFFOPTIONSDLG_H

#include <optionsdlg/optionsdlg.h>

class TableMacOptionsPage;
class DiffOptionsDlg : public OptionsDlg
{
    Q_OBJECT
public:
    DiffOptionsDlg(QSettings *settings, QWidget *parent = nullptr);
    virtual ~DiffOptionsDlg();

private:
    TableMacOptionsPage *m_TableMacOptionsPage;
};

#endif // DIFFOPTIONSDLG_H
