#include "fmteditcontentdialog.h"
#include "fmtfield.h"
#include "fmtcore.h"
#include "datelineedit.h"
#include <limits>

FmtEditContentDialog::FmtEditContentDialog(FmtSharedTablePtr table, FmtEditContentModel *model, QWidget *parent) :
    QDialog(parent),
    m_Submit(false),
    m_CurrentIndex(-1)
{
    pTable = table;
    pModel = model;
    setFixedSize(640, 480);

    pMapper = new QDataWidgetMapper(this);
    pMainLayout = new QVBoxLayout;
    setLayout(pMainLayout);
    pScrollArea = new QScrollArea(this);
    pScrollArea->setWidgetResizable(true);
    pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pMainLayout->addWidget(pScrollArea);
    CreateNavButtons();

    pButtonBox = new QDialogButtonBox(this);
    pButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    pMainLayout->addWidget(pButtonBox);
    CreateContainer();
    setWindowTitle(tr("Редактирование записи таблицы '%1'").arg(pTable->name()));

    connect(pButtonBox, SIGNAL(accepted()), pMapper, SLOT(submit()));
    connect(pButtonBox, SIGNAL(rejected()), pMapper, SLOT(revert()));
    connect(pButtonBox, SIGNAL(accepted()), SLOT(accept()));
    connect(pButtonBox, SIGNAL(rejected()), SLOT(reject()));
    connect(pMapper, SIGNAL(currentIndexChanged(int)), SLOT(OnIndexChanged(int)));
}

FmtEditContentDialog::~FmtEditContentDialog()
{
    qDeleteAll(pLabels);
    qDeleteAll(pWidgets);
    pLabels.clear();
    pWidgets.clear();
}

void FmtEditContentDialog::setCurrentIndex(const QModelIndex &index)
{
    pMapper->setCurrentIndex(index.row());
}

quint32 FmtEditContentDialog::currentIndex() const
{
    return m_CurrentIndex;
}

void FmtEditContentDialog::CreateContainer()
{
    pSpacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout = new QGridLayout;
    pContainer = new QWidget(this);
    pContainer->setLayout(pLayout);
    pMapper->setModel(pModel);
    pMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    int i = 0;
    for (; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        QLabel *label = new QLabel(QString("%1:").arg(pFld->name().toUpper()));
        label->setToolTip(pFld->comment());
        label->setAlignment(Qt::AlignTop);
        pLabels.push_back(label);

        QString propertyName;
        QWidget *pEditWidget = CreateWidgetForField(pFld, propertyName);
        pEditWidget->setToolTip(pFld->comment());
        pLayout->addWidget(label, i, 0);
        pLayout->addWidget(pEditWidget, i, 1);

        pMapper->addMapping(pEditWidget, i, propertyName.toLocal8Bit());
    }
    pLayout->addItem(pSpacer, i + 1, 0);
    pLayout->setColumnStretch(0, 0);
    pLayout->setColumnStretch(1, 1);

    pScrollArea->setWidget(pContainer);

    QList<QAbstractSpinBox*> spinList = pContainer->findChildren<QAbstractSpinBox*>();
    foreach (QAbstractSpinBox *spin, spinList) {
        spin->installEventFilter(this);
        spin->setFocusPolicy(Qt::StrongFocus);
    }
}

bool FmtEditContentDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Wheel && qobject_cast<QAbstractSpinBox*>(watched))
    {
        event->ignore();
        return true;
    }
    return QDialog::eventFilter(watched, event);
}

QWidget *FmtEditContentDialog::CreateWidgetForField(FmtField *fld, QString &propertyName)
{
    QWidget *pWidget = Q_NULLPTR;

    qint16 type = fld->type();
    if (type == fmtt_INT || type == fmtt_LONG || type == fmtt_BIGINT)
    {
        QSpinBox *pSpin = new QSpinBox(pContainer);

        if (type == fmtt_INT)
        {
            pSpin->setMinimum(std::numeric_limits<qint16>::min());
            pSpin->setMaximum(std::numeric_limits<qint16>::max());
        }
        else
        {
            pSpin->setMinimum(std::numeric_limits<qint32>::min());
            pSpin->setMaximum(std::numeric_limits<qint32>::max());
        }

        pSpin->setValue(0);
        pWidget = pSpin;
        propertyName = "value";
    }
    else if (type == fmtt_FLOAT || type == fmtt_DOUBLE || type == fmtt_MONEY || type == fmtt_NUMERIC)
    {
        QDoubleSpinBox *pSpin = new QDoubleSpinBox(pContainer);
        pSpin->setMinimum(-922337203685477.5808);
        pSpin->setMaximum(922337203685477.5807);
        pSpin->setValue(0);
        pSpin->setDecimals(3);
        pWidget = pSpin;
        propertyName = "value";
    }
    else if (type == fmtt_STRING || type == fmtt_SNR || type == fmtt_CHR || type == fmtt_UCHR)
    {
        if (fld->size() <= 256)
        {
            QLineEdit *pLineEdit = new QLineEdit(pContainer);
            pLineEdit->setMaxLength(qMax(fld->size() - 1, 1));
            pLineEdit->setPlaceholderText(tr("<empty>"));
            pWidget = pLineEdit;
            propertyName = "text";
        }
        else
        {
            QPlainTextEdit *pTextEdit = new QPlainTextEdit(pContainer);
            pTextEdit->setMinimumHeight(100);
            pTextEdit->setPlaceholderText(tr("<empty>"));
            pWidget = pTextEdit;
            propertyName = "plainText";
            connect(pTextEdit, SIGNAL(textChanged()), SLOT(OnBigTextChanged()));
        }
    }
    else if (type == fmtt_DATE)
    {
        DateLineEdit *pDateEdit = new DateLineEdit(pContainer);
        pWidget = pDateEdit;
        propertyName = "date";
    }
    else if (type == fmtt_TIME)
    {
        QTimeEdit *pTimeEdit = new QTimeEdit(pContainer);
        pTimeEdit->setDisplayFormat("HH:mm:ss");
        propertyName = "time";
        pWidget = pTimeEdit;
    }

    pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    return pWidget;
}

void FmtEditContentDialog::CreateNavButtons()
{
    pNavLayout = new QHBoxLayout;
    pNavSpacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

    pLine = new QFrame(this);
    pLine->setFrameShape(QFrame::VLine);
    pLine->setFrameShadow(QFrame::Sunken);

    pSaveBtn = new QToolButton(this);
    pSaveBtn->setIcon(QIcon(":/img/base_checkmark_32.png"));

    pDiscard = new QToolButton(this);
    pDiscard->setIcon(QIcon(":/img/DiscardChanges.png"));

    pFirstBtn = new QToolButton(this);
    pFirstBtn->setIcon(QIcon(":/img/DataContainer_MoveFirstHS.png"));

    pPrevBtn = new QToolButton(this);
    pPrevBtn->setIcon(QIcon(":/img/DataContainer_MovePreviousHS.png"));

    pNextBtn = new QToolButton(this);
    pNextBtn->setIcon(QIcon(":/img/DataContainer_MoveNextHS.png"));

    pLastBtn = new QToolButton(this);
    pLastBtn->setIcon(QIcon(":/img/DataContainer_MoveLastHS.png"));

    pNavLayout->addWidget(pSaveBtn);
    pNavLayout->addWidget(pDiscard);
    pNavLayout->addWidget(pLine);
    pNavLayout->addWidget(pFirstBtn);
    pNavLayout->addWidget(pPrevBtn);
    pNavLayout->addWidget(pNextBtn);
    pNavLayout->addWidget(pLastBtn);
    pNavLayout->addItem(pNavSpacer);
    pMainLayout->addLayout(pNavLayout);

    connect(pFirstBtn, SIGNAL(clicked(bool)), SLOT(OnFirstBtn()));
    connect(pPrevBtn, SIGNAL(clicked(bool)), SLOT(OnPrevBtn()));
    connect(pNextBtn, SIGNAL(clicked(bool)), SLOT(OnNextBtn()));
    connect(pLastBtn, SIGNAL(clicked(bool)), SLOT(OnLastBtn()));
    connect(pSaveBtn, SIGNAL(clicked(bool)), SLOT(OnApplyChanges()));
    connect(pDiscard, SIGNAL(clicked(bool)), SLOT(OnRevertChanges()));
}

void FmtEditContentDialog::OnIndexChanged(const int &index)
{
    m_CurrentIndex = index;
    m_SaveData.clear();

    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        QModelIndex indx = pModel->index(index, i);

        if (m_CurrentIndex >= 0)
            m_SaveData.append(pModel->data(indx, Qt::EditRole));
    }
}

bool FmtEditContentDialog::isModifyed(QList<qint16> *lst)
{
    bool hr = false;
    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        QModelIndex indx = pModel->index(m_CurrentIndex, i);

        if (indx.isValid())
        {
            QWidget *widget = pMapper->mappedWidgetAt(i);
            QString prop = pMapper->mappedPropertyName(widget);

            if (m_SaveData[i] != widget->property(prop.toLocal8Bit().data()))
            {
                if (lst)
                    lst->append(i);
                hr = true;
            }
        }
    }
    return hr;
}

bool FmtEditContentDialog::hasSubmit() const
{
    return m_Submit;
}

bool FmtEditContentDialog::CheckSave()
{
    QList<qint16> lst;
    if (!isModifyed(&lst))
        return true;

    QString msgstr = tr("Сохранить внесенные в запись изменеия?");
    QString fieldlist;
    QMessageBox msg(this);
    msg.setText(msgstr);

    foreach (const qint16 &item, lst) {
        FmtField *fld = pTable->field(item);
        fieldlist += QString("%1 (%2)\n").arg(fld->name(), fld->comment());
    }
    msg.setDetailedText(fieldlist);
    msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Retry);

    bool canNext = true;
    int result = msg.exec();
    if (result == QMessageBox::Save)
    {
        pMapper->submit();
        m_Submit = true;
    }
    else if (result == QMessageBox::Discard)
        pMapper->revert();
    else
        canNext = false;

    return canNext;
}

void FmtEditContentDialog::OnFirstBtn()
{
    if (CheckSave())
        pMapper->toFirst();
}

void FmtEditContentDialog::OnPrevBtn()
{
    if (CheckSave())
        pMapper->toPrevious();
}
void FmtEditContentDialog::OnNextBtn()
{
    if (CheckSave())
        pMapper->toNext();
}

void FmtEditContentDialog::OnLastBtn()
{
    if (CheckSave())
        pMapper->toLast();
}

void FmtEditContentDialog::OnApplyChanges()
{
    pMapper->submit();
    m_Submit = true;
}

void FmtEditContentDialog::OnRevertChanges()
{
    pMapper->revert();
}

int FmtEditContentDialog::GetWidgetRow(QWidget *w)
{
    int row = -1;
    int index = pLayout->indexOf(w);
    if (index >= 0)
    {
        int column = 0, rowSpan = 0, columnSpan = 0;
        pLayout->getItemPosition(index, &row, &column, &rowSpan, &columnSpan);
    }

    return row;
}

void FmtEditContentDialog::OnBigTextChanged()
{
    QPlainTextEdit *pEdit = qobject_cast<QPlainTextEdit*>(sender());

    if (!pEdit)
        return;

    int fldId = GetWidgetRow(pEdit);
    if (fldId >= 0)
    {
        bool oldBlockState = pEdit->blockSignals(true);
        FmtField *pFld = pTable->field(fldId);
        quint32 maxTextEditLength = pFld->size() - 1;
        int diff = pEdit->toPlainText().length() - maxTextEditLength;
        QString newStr = pEdit->toPlainText();
        newStr.chop(diff);
        pEdit->setPlainText(newStr);
        QTextCursor cursor = pEdit->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        pEdit->setTextCursor(cursor);
        pEdit->blockSignals(oldBlockState);
    }
}
