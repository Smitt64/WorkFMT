#ifndef FMTEDITCONTENTDIALOG_H
#define FMTEDITCONTENTDIALOG_H

#include <QtWidgets>
#include "fmttable.h"
#include "fmteditcontentmodel.h"

class FmtField;
class FmtEditContentModel;
class FmtEditContentDialog : public QDialog
{
    Q_OBJECT
public:
    FmtEditContentDialog(FmtSharedTablePtr table, FmtEditContentModel *model, QWidget *parent);
    virtual ~FmtEditContentDialog();
    void setCurrentIndex(const QModelIndex &index);

    quint32 currentIndex() const;
    bool hasSubmit() const;

private slots:
    void OnFirstBtn();
    void OnPrevBtn();
    void OnNextBtn();
    void OnLastBtn();
    void OnIndexChanged(const int &index);
    void OnApplyChanges();
    void OnRevertChanges();
    void OnBigTextChanged();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    void CreateContainer();
    void CreateNavButtons();
    bool isModifyed(QList<qint16> *lst);
    bool CheckSave();
    int GetWidgetRow(QWidget *w);
    QWidget *CreateWidgetForField(FmtField *fld, QString &propertyName);
    FmtSharedTablePtr pTable;
    QScrollArea *pScrollArea;
    QWidget *pContainer;
    QGridLayout *pLayout;
    QVBoxLayout *pMainLayout;
    QSpacerItem *pSpacer, *pNavSpacer;
    QDialogButtonBox *pButtonBox;
    QDataWidgetMapper *pMapper;
    FmtEditContentModel *pModel;
    QHBoxLayout *pNavLayout;
    bool m_Submit;

    QToolButton *pFirstBtn, *pPrevBtn, *pNextBtn, *pLastBtn, *pSaveBtn, *pDiscard;
    QFrame *pLine;

    QList<QLabel*> pLabels;
    QList<QWidget*> pWidgets;

    QVariantList m_SaveData;
    quint32 m_CurrentIndex;
};

#endif // FMTEDITCONTENTDIALOG_H
