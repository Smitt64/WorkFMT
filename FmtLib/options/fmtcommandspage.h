#ifndef FMTCOMMANDSPAGE_H
#define FMTCOMMANDSPAGE_H

#include <optionsdlg/commandsoptions.h>

class UserCommandsGroup;
class FmtCommandsPage : public CommandsOptions
{
    Q_OBJECT
public:
    FmtCommandsPage(QWidget *parent = nullptr);
    virtual ~FmtCommandsPage();

private:
    UserCommandsGroup *m_pUserCmd;
};

#endif // FMTCOMMANDSPAGE_H
