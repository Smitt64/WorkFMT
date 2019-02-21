#ifndef FMTEDITCONTENTFILTER_H
#define FMTEDITCONTENTFILTER_H

#include <QDialog>
#include "fmttable.h"

namespace Ui {
class FmtEditContentFilter;
}

class FieldSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FieldSortFilterProxyModel(QObject *parent = Q_NULLPTR);
    virtual ~FieldSortFilterProxyModel();
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

// ========================================================================================

class CodeEditor;
class SqlHighlighter;
class FmtEditContentFilter : public QDialog
{
    Q_OBJECT

public:
    enum ConditionType
    {
        CT_Equal = 0,
        CT_NotEqual,
        CT_Greater,
        CT_GreaterEqual,
        CT_Less,
        CT_LessEqual,
        CT_Like,
        CT_Between,
        CT_NotNull,
        CT_Null,
        CT_In,
        CT_NotIn,
    };
    explicit FmtEditContentFilter(FmtSharedTablePtr table, QWidget *parent = 0);
    virtual ~FmtEditContentFilter();

    QString filter() const;
    void setFilter(const QString &fltr);

    QString sort() const;
    void setSort(const QString &str);

private slots:
    void OnCondCurrentTextChanged(const qint32 &index);
    void OnFieldComboIndexChanged(const qint32 &index);
    void OnPushAnd();
    void OnAddOrder();
    void OnSelectionChanged(const QModelIndex &index);
    void OnSelectValues();
    void OnRemoveFromOrder();
    void OnMoveOrder();
    void OnClearFilter();

private:
    QString FormatValue(const qint32 &fldindex, const QString &value);
    Ui::FmtEditContentFilter *ui;
    CodeEditor *pEditor;
    QSortFilterProxyModel *pProxy;
    FmtSharedTablePtr pTable;
    SqlHighlighter *pHighlighter;
    QIntValidator m_IntValidator;
    QDoubleValidator m_DoubleValidator;
    QRegExpValidator m_DateValidator, m_TimeValidator;
    QPushButton *pClearFilterBtn;

    QStringListModel m_OrderModel;
};

#endif // FMTEDITCONTENTFILTER_H
