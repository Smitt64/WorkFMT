#include "fmteditcontentfilter.h"
#include "ui_fmteditcontentfilter.h"
#include "codeeditor.h"
#include "fmtfildsmodel.h"
#include "highlighter.h"
#include "connectioninfo.h"
#include "fmtfield.h"
#include "datelineedit.h"
#include "dataselectdialog.h"
#include <QtSql>
#include <QDebug>

#define BETWEEN_CONDITION "BETWEEN"

FieldSortFilterProxyModel::FieldSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

FieldSortFilterProxyModel::~FieldSortFilterProxyModel()
{

}

bool FieldSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent);
    if (sourceRow == sourceModel()->rowCount() - 1)
        return false;
    return true;
}

Qt::ItemFlags FieldSortFilterProxyModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

// ========================================================================================
void ListItemSwap(QListView *list, const QString &direction)
{
   int currentIndex = list->currentIndex().row();
   QAbstractItemModel *model = list->model();
   if (currentIndex != -1)
   {
      if (direction.toLower() == "up") //moves the item up
      {
         if (currentIndex > 0)
         {
            QVariant value1 = model->data(model->index(currentIndex, 0));
            QVariant value2 = model->data(model->index(currentIndex - 1, 0));

            model->setData(model->index(currentIndex, 0), value2);
            model->setData(model->index(currentIndex - 1, 0), value1);
            list->setCurrentIndex(model->index(currentIndex - 1, 0));
         }
      }
      else  //moves the item down
      {
         if (currentIndex < model->rowCount() - 1)
         {
             QVariant value1 = model->data(model->index(currentIndex, 0));
             QVariant value2 = model->data(model->index(currentIndex + 1, 0));

             model->setData(model->index(currentIndex, 0), value2);
             model->setData(model->index(currentIndex + 1, 0), value1);
             list->setCurrentIndex(model->index(currentIndex + 1, 0));
         }
      }
   }
}
// ========================================================================================

FmtEditContentFilter::FmtEditContentFilter(FmtSharedTablePtr table, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FmtEditContentFilter)
{
    ui->setupUi(this);
    pEditor = new CodeEditor(this);
    pHighlighter = new SqlHighlighter(pEditor->document());

    pTable = table;
    ui->verticalLayout_3->addWidget(pEditor);

    m_DateValidator.setRegExp(QRegExp(DateLineEdit::regexpPattern()));
    m_TimeValidator.setRegExp(QRegExp("([012]?[0-9]|2[0-3]):[0-5][0-9]:\\d{1,3}"));

    pProxy = new FieldSortFilterProxyModel(this);
    pProxy->setSourceModel(table->fieldsModel());

    ui->fldListView->setModel(pProxy);
    ui->fldListView->setModelColumn(FmtFildsModel::fld_DbName);
    ui->orderListView->setModel(&m_OrderModel);

    pClearFilterBtn = ui->buttonBox->addButton(tr("Очистить фильтр"), QDialogButtonBox::ResetRole);
    pClearFilterBtn->setDefault(false);
    pClearFilterBtn->setAutoDefault(false);

    ui->fldComboBox->setModel(pProxy);
    ui->fldComboBox->setModelColumn(FmtFildsModel::fld_DbName);
    OnCondCurrentTextChanged(ui->condComboBox->currentIndex());
    OnFieldComboIndexChanged(ui->fldComboBox->currentIndex());
    OnSelectionChanged(QModelIndex());

    connect(ui->condComboBox, SIGNAL(currentIndexChanged(int)), SLOT(OnCondCurrentTextChanged(qint32)));
    connect(ui->fldComboBox, SIGNAL(currentIndexChanged(int)), SLOT(OnFieldComboIndexChanged(qint32)));
    connect(ui->pushAnd, SIGNAL(clicked(bool)), SLOT(OnPushAnd()));
    connect(ui->pushOr, SIGNAL(clicked(bool)), SLOT(OnPushAnd()));
    connect(ui->addAsc, SIGNAL(clicked(bool)), SLOT(OnAddOrder()));
    connect(ui->addDesc, SIGNAL(clicked(bool)), SLOT(OnAddOrder()));
    connect(ui->fldListView, SIGNAL(clicked(QModelIndex)), SLOT(OnSelectionChanged(QModelIndex)));
    connect(ui->condToolButton1, SIGNAL(clicked(bool)), SLOT(OnSelectValues()));
    connect(ui->condToolButton2, SIGNAL(clicked(bool)), SLOT(OnSelectValues()));
    connect(ui->removeFromOrder, SIGNAL(clicked(bool)), SLOT(OnRemoveFromOrder()));
    connect(ui->orderBtnDown, SIGNAL(clicked(bool)), SLOT(OnMoveOrder()));
    connect(ui->orderBtnUp, SIGNAL(clicked(bool)), SLOT(OnMoveOrder()));
    connect(pClearFilterBtn, SIGNAL(clicked(bool)), SLOT(OnClearFilter()));
}

FmtEditContentFilter::~FmtEditContentFilter()
{
    delete ui;
}

QString FmtEditContentFilter::filter() const
{
    return pEditor->toPlainText();
}

void FmtEditContentFilter::setFilter(const QString &fltr)
{
    pEditor->setPlainText(fltr);
}

void FmtEditContentFilter::OnCondCurrentTextChanged(const qint32 &index)
{
    bool enabled2 = true;
    if (index != CT_Between)
        enabled2 = false;

    ui->condLineEdit2->setVisible(enabled2);
    ui->condToolButton2->setVisible(enabled2);

    bool enabled1 = true;
    if (index == CT_NotNull || index == CT_Null)
        enabled1 = false;

    ui->condLineEdit1->setEnabled(enabled1);
    ui->condToolButton1->setEnabled(enabled1);
}

void FmtEditContentFilter::OnFieldComboIndexChanged(const qint32 &index)
{
    ui->condLineEdit1->setText("");
    ui->condLineEdit2->setText("");

    ui->condLineEdit1->setInputMask("");
    ui->condLineEdit2->setInputMask("");

    ui->condLineEdit1->setValidator(Q_NULLPTR);
    ui->condLineEdit2->setValidator(Q_NULLPTR);

    FmtField *pFld = pTable->field(index);
    if (pFld->isNumber() && !pFld->isRealNumber())
    {
        ui->condLineEdit1->setValidator(&m_IntValidator);
        ui->condLineEdit2->setValidator(&m_IntValidator);
    }
    else if (pFld->isRealNumber())
    {
        ui->condLineEdit1->setValidator(&m_DoubleValidator);
        ui->condLineEdit2->setValidator(&m_DoubleValidator);
    }
    else if (pFld->type() == fmtt_DATE)
    {
        ui->condLineEdit1->setValidator(&m_DateValidator);
        ui->condLineEdit2->setValidator(&m_DateValidator);

        ui->condLineEdit1->setInputMask(DateLineEdit::maskString());
        ui->condLineEdit2->setInputMask(DateLineEdit::maskString());
    }
    else if (pFld->type() == fmtt_TIME)
    {
        ui->condLineEdit1->setValidator(&m_TimeValidator);
        ui->condLineEdit2->setValidator(&m_TimeValidator);

        ui->condLineEdit1->setInputMask("99:99:99");
        ui->condLineEdit2->setInputMask("99:99:99");
    }
}

QString FmtEditContentFilter::FormatValue(const qint32 &fldindex, const QString &value)
{
    const QSqlDriver *driver = pTable->connection()->db().driver();

    QSqlField fld;
    FmtField *pFld = pTable->field(fldindex);
    if (pFld->isString())
    {
        fld.setType(QVariant::String);
        fld.setValue(value.mid(0, pFld->size() - 1));
    }
    else if (pFld->isNumber() && !pFld->isRealNumber())
    {
        fld.setType(QVariant::Int);
        fld.setValue(value.toInt());
    }
    else if (pFld->isRealNumber())
    {
        fld.setType(QVariant::Double);
        fld.setValue(value.toDouble());
    }
    else if (pFld->type() == fmtt_DATE)
    {
        fld.setType(QVariant::Date);
        fld.setValue(QDate::fromString(value, DateLineEdit::dateFormat()));
    }
    else if (pFld->type() == fmtt_TIME)
    {
        fld.setType(QVariant::DateTime);
        fld.setValue(QDateTime(QDate(1,1,1), QTime::fromString(value, "HH:mm:ss")));
    }
    else if (pFld->type() == fmtt_CHR || pFld->type() == fmtt_UCHR)
    {
        fld.setType(QVariant::Char);
        fld.setValue(value.at(0));
    }

    return driver->formatValue(fld);
}

void FmtEditContentFilter::OnPushAnd()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn)
        return;

    if (!pEditor->document()->isEmpty())
        pEditor->appendPlainText(QString(" %1 ").arg(btn->text().toUpper()));


    QString value1 = FormatValue(ui->fldComboBox->currentIndex(), ui->condLineEdit1->text());
    QString condition, fldName = ui->fldComboBox->currentText();
    qint32 contType = ui->condComboBox->currentIndex();

    if (contType == CT_Like)
    {
        condition = QString("%1 LIKE %2")
                .arg(fldName, value1);
    }
    else if (contType == CT_Between)
    {
        QString value2 = FormatValue(ui->fldComboBox->currentIndex(), ui->condLineEdit2->text());
        condition = QString("%1 BETWEEN %2 AND %3")
                .arg(fldName, value1, value2);
    }
    else if (contType == CT_NotNull)
    {
        condition = QString("%1 IS NOT NULL")
                .arg(fldName);
    }
    else if (contType == CT_Null)
    {
        condition = QString("%1 IS NULL")
                .arg(fldName);
    }
    else if (contType == CT_In)
    {
        condition = QString("%1 IN (%2)")
                .arg(fldName, value1);
    }
    else if (contType == CT_NotIn)
    {
        condition = QString("%1 NOT IN (%2)")
                .arg(fldName, value1);
    }
    else
    {
        condition = QString("%1 %2 %3").arg(ui->fldComboBox->currentText())
                                 .arg(ui->condComboBox->currentText())
                                 .arg(value1);
    }
    pEditor->appendPlainText(condition);
}


void FmtEditContentFilter::OnSelectionChanged(const QModelIndex &index)
{
    ui->addAsc->setEnabled(index.isValid());
    ui->addDesc->setEnabled(index.isValid());
}

QString FmtEditContentFilter::sort() const
{
    QString sortstr;
    for (int i = 0; i < m_OrderModel.rowCount(); i++)
    {
        QString str = m_OrderModel.data(m_OrderModel.index(i, 0), Qt::DisplayRole).toString();

        if (i != 0)
            sortstr += ",";
        sortstr += str;
    }
    return sortstr;
}

void FmtEditContentFilter::setSort(const QString &str)
{
    if (str.isEmpty())
        return;

    QStringList lst = str.split(',');

    foreach (const QString &s, lst) {
        int newRow = m_OrderModel.rowCount();
        m_OrderModel.insertRow(newRow);
        m_OrderModel.setData(m_OrderModel.index(newRow, 0), s);
    }
}

void FmtEditContentFilter::OnAddOrder()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    if (!btn)
        return;

    QRegExp rx("(\\w+)");
    int pos = rx.indexIn(btn->text());

    if (pos >= 0)
    {
        QString ord = rx.cap(1);
        QModelIndex index = ui->fldListView->selectionModel()->currentIndex();
        QString fldname = index.data().toString();

        bool found = false;
        for (int i = 0; i < m_OrderModel.rowCount(); i++)
        {
            QString str = m_OrderModel.data(m_OrderModel.index(i, 0), Qt::DisplayRole).toString();
            if (str.contains(fldname))
                found = true;
        }

        if (!found)
        {
            int newRow = m_OrderModel.rowCount();
            m_OrderModel.insertRow(newRow);
            m_OrderModel.setData(m_OrderModel.index(newRow, 0), QString("%1 %2").arg(fldname, ord));
        }
    }
}

void FmtEditContentFilter::OnSelectValues()
{
    DataSelectDialog dlg(this);
    dlg.setQuery(pTable->name(), ui->fldComboBox->currentText(), &pTable->connection()->db());
    if (dlg.exec() == QDialog::Accepted)
    {
        FmtField *pFld = pTable->field(ui->fldComboBox->currentIndex());
        QVariant value = dlg.selectedValue();
        QLineEdit *pLineEdit = ui->condLineEdit1;

        if (sender() == ui->condToolButton2)
            pLineEdit = ui->condLineEdit2;

        if (pFld->type() == fmtt_DATE)
        {
            QDate dt = value.toDate();
            if (dt.isValid())
                pLineEdit->setText(dt.toString(DateLineEdit::dateFormat()));
        }
        else if(pFld->type() == fmtt_TIME)
        {
            QTime time = value.toTime();
            if (time.isValid())
                pLineEdit->setText(time.toString("HH:mm:ss"));
        }
        else
            pLineEdit->setText(value.toString());
    }
}

void FmtEditContentFilter::OnRemoveFromOrder()
{
    QModelIndex index = ui->orderListView->currentIndex();

    if (!index.isValid())
        return;

    m_OrderModel.removeRow(index.row());
}

void FmtEditContentFilter::OnMoveOrder()
{
    if (sender() == ui->orderBtnUp)
        ListItemSwap(ui->orderListView, "up");
    else
        ListItemSwap(ui->orderListView, "down");
}

void FmtEditContentFilter::OnClearFilter()
{
    pEditor->clear();
}
