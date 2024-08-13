#ifndef GENERALOPTIONS_H
#define GENERALOPTIONS_H

#include <QWidget>
#include <optionsdlg/OptionsPage.h>
#include <QMap>

namespace Ui {
class GeneralOptions;
}

typedef QMap<QString, int> UsesMap;
class StdFolderListHandler;
class GeneralOptions : public OptionsPage
{
    Q_OBJECT

public:
    GeneralOptions(QWidget *parent = nullptr);
    ~GeneralOptions();

protected:
    virtual void restore() Q_DECL_OVERRIDE;
    virtual int save() Q_DECL_OVERRIDE;

private:
    Ui::GeneralOptions *ui;

    StdFolderListHandler *m_StdFolderListHandler;
    QMap<int, UsesMap> m_DirUses;
};

#endif // GENERALOPTIONS_H
