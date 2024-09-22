#ifndef FMTCOMMANDSPAGE_H
#define FMTCOMMANDSPAGE_H

#include <optionsdlg/commandsoptions.h>

class QStyledItemDelegate;
class UserCommandsGroup;
class FmtCommandsPage : public CommandsOptions
{
    Q_OBJECT
public:
    FmtCommandsPage(QWidget *parent = nullptr);
    virtual ~FmtCommandsPage();

    virtual int save() Q_DECL_OVERRIDE;
    virtual void restore() Q_DECL_OVERRIDE;

private:
    UserCommandsGroup *m_pUserCmd;
    QStyledItemDelegate *m_pDelegate;
};

#endif // FMTCOMMANDSPAGE_H
