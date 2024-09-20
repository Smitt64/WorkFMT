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
class QStandardItemModel;
class ExpTableModelDelegate;
class GeneralOptions : public OptionsPage
{
    Q_OBJECT

public:
    enum ClobMode
    {
        ClobMode_Simplified = 0,
        ClobMode_SimplifiedTrimmed,
        ClobMode_SplitFile,
    };
    GeneralOptions(QWidget *parent = nullptr);
    ~GeneralOptions();

private slots:
    void newExpTableBtnClicked();
    void removeExpTableBtnClicked();

protected:
    virtual void restore() Q_DECL_OVERRIDE;
    virtual int save() Q_DECL_OVERRIDE;

private:
    void setupExpTableModel();
    void updateExpOldFlag();
    Ui::GeneralOptions *ui;

    QStandardItemModel *m_pExpTableModel;
    ExpTableModelDelegate *m_pExpTableDelegate;

    StdFolderListHandler *m_StdFolderListHandler;
    QMap<int, UsesMap> m_DirUses;
};

#endif // GENERALOPTIONS_H
