#include "fmrichtextwidget.h"
#include "fmtcore.h"
#include "fmtfield.h"
#include <QSpacerItem>
#include <QRegularExpression>

FmRichTextWidget::FmRichTextWidget(QWidget *parent) :
    QWizardPage(parent)
{
    pDocument = new QTextEdit;
    pNameLabel = new QLabel(tr("Наименование: "));
    pTypeLabel = new QLabel(tr("Тип: "));
    pCommentLabel = new QLabel(tr("Комментарий: "));

    pStringCheck = new QCheckBox();
    pStringCheck->setText(tr("String + 1"));

    pNameCombo = new QComboBox();
    pNameCombo->setMinimumWidth(150);
    pNameCombo->setModel(&m_ColumnsItems);

    pTypeCombo = new QComboBox();
    pTypeCombo->setMinimumWidth(150);
    pTypeCombo->setModel(&m_ColumnsItems);

    pCommentCombo = new QComboBox();
    pCommentCombo->setMinimumWidth(150);
    pCommentCombo->setModel(&m_ColumnsItems);

    setTitle(tr("Вставьте текст"));
    QVBoxLayout *layout = new QVBoxLayout;

    horizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    for (int i = 0; i < 2; i++)
        pLabelspaces[i] = new QSpacerItem(16, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

    grid = new QHBoxLayout;
    grid->addWidget(pNameLabel);
    grid->addWidget(pNameCombo);
    grid->addSpacerItem(pLabelspaces[0]);
    grid->addWidget(pTypeLabel);
    grid->addWidget(pTypeCombo);
    grid->addSpacerItem(pLabelspaces[1]);
    grid->addWidget(pCommentLabel);
    grid->addWidget(pCommentCombo);
    grid->addSpacerItem(horizontalSpacer);
    grid->addWidget(pStringCheck);

    layout->addLayout(grid);
    layout->addWidget(pDocument);
    setLayout(layout);

    connect(pDocument, SIGNAL(textChanged()), SLOT(onTextChanged()));
}

void FmRichTextWidget::setDocumentMinimumSize(const QSize &size)
{
    pDocument->setMinimumSize(size);
}

void FmRichTextWidget::AddCustomWidget(const QString &label, QWidget *widget)
{
    QLabel *pLabel = new QLabel(label);
    m_CustomLabels.append(pLabel);
    grid->insertWidget(0, pLabel);
    grid->insertWidget(1, widget);
}

void FmRichTextWidget::onTextChanged()
{
    QTextTable *table = Q_NULLPTR;
    QTextBlock block = pDocument->document()->begin();

    m_ColumnsItems.clear();
    while(block.isValid())
    {
        QTextCursor blockCursor(block);
        table = blockCursor.currentTable();

        if (table)
        {
            int i = 0;

            for (; i < table->columns(); i++)
            {
                QTextTableCell cell = table->cellAt(0, i);
                QTextCursor fcur = cell.firstCursorPosition();
                fcur.select(QTextCursor::LineUnderCursor);
                m_ColumnsItems.appendRow(new QStandardItem(fcur.selectedText()));
            }

            TryAutoMapFields();
            break;
        }
        block = block.next();
    }
}

void FmRichTextWidget::TryAutoMapFields()
{
    SetAutoMapField(pNameCombo, "Код");
    SetAutoMapField(pTypeCombo, "Тип");
    SetAutoMapField(pCommentCombo, "Комментарий");
}

void FmRichTextWidget::SetAutoMapField(QComboBox *pCombo, const QString &name)
{
    QList<QStandardItem*> result;
    result = m_ColumnsItems.findItems(name, Qt::MatchContains);

    if (!result.empty())
        pCombo->setCurrentIndex(result[0]->row());
}

void FmRichTextWidget::ReadConetent(FmtSharedTablePtr &pTable, const FmRichTextReadParam &prm)
{
    QTextTable *table = Q_NULLPTR;
    QTextBlock block = pDocument->document()->begin();

    while(block.isValid())
    {
        QTextCursor blockCursor(block);
        table = blockCursor.currentTable();

        QString str = block.text().replace("«", "\"").replace("»", "\"");
        if (prm.readComment)
        {
            if (str.contains("Сущность", Qt::CaseInsensitive))
            {
                int pos = 0;
                QRegExp rxComment("(\\\"(([^\"])*)\\\")");
                if ((pos = rxComment.indexIn(str, pos)) != -1)
                    pTable->setComment(rxComment.cap(2));
            }
        }

        if (prm.readTableName)
        {
            int pos = 0;
            QRegExp rx(QString::fromUtf8("\\b(\\w+)\\.(dbt|tmp|rec)"));
            if ((pos = rx.indexIn(str, pos)) != -1)
                pTable->setName(QString("d%1_%2").arg(rx.cap(1), rx.cap(2)));
        }

        if (table)
        {
            int i = prm.ignoreFirstRow ? 1 : 0;
            for (; i < table->rows(); i++)
            {
                QTextTableCell cell  = table->cellAt(i, pNameCombo->currentIndex());
                QTextTableCell cell2 = table->cellAt(i, pTypeCombo->currentIndex());
                QTextTableCell cell3 = table->cellAt(i, pCommentCombo->currentIndex());

                QTextCursor fcur  = cell.firstCursorPosition();
                QTextCursor fcur2 = cell2.firstCursorPosition();
                QTextCursor fcur3 = cell3.firstCursorPosition();
                fcur.select(QTextCursor::LineUnderCursor);
                fcur2.select(QTextCursor::LineUnderCursor);
                fcur3.select(QTextCursor::LineUnderCursor);

                int size = 0;
                QString comment = fcur3.selectedText().simplified();
                QString ttype = fcur2.selectedText().simplified();
                CreateFieldParamList fldsList = GetFieldsToCreate(ttype, comment);

                foreach(const FmRichTextCreateFieldParam &fldParam, fldsList)
                {
                    FmtField *fld = Q_NULLPTR;
                    size = fldParam.size;

                    if (prm.insertBefore < 0)
                        fld = pTable->addField(fcur.selectedText().simplified(), fldParam.type);
                    else
                        fld = pTable->insertField(prm.insertBefore, fcur.selectedText().simplified(), fldParam.type);

                    if (pStringCheck->checkState() != Qt::Unchecked)
                        ++ size;

                    fld->setComment(comment);
                    fld->setSize(size);
                }
            }
            break;
        }

        block = block.next();
    }
}

void FmRichTextWidget::AddToCreateFieldParamList(CreateFieldParamList &list, const FmtFldType &type, const int &size)
{
    FmRichTextCreateFieldParam param;
    param.type = type;
    param.size = size;

    if (size == CALC_SIZE)
        param.size = fmtTypeSize(param.type);

    list.append(param);
}

CreateFieldParamList FmRichTextWidget::GetFieldsToCreate(const QString &str, const QString &comment)
{
    CreateFieldParamList CreateList;

    QRegularExpressionMatch match;
    if (str.contains("LONG", Qt::CaseInsensitive))
        AddToCreateFieldParamList(CreateList, fmtt_LONG);
    else if (str.contains("BIGINT", Qt::CaseInsensitive))
        AddToCreateFieldParamList(CreateList, fmtt_BIGINT);
    else if (str.contains(QRegularExpression("NUMBER[\\n\\s\\t]*\\((\\d+)\\)", QRegularExpression::CaseInsensitiveOption), &match))
    {
        FmtFldType type = fmtt_INT;
        int size = match.captured(1).toInt();

        if (size == 10)
            type = fmtt_LONG;
        else if (size == 19)
            type = fmtt_BIGINT;
        else
            type = fmtt_INT;

        AddToCreateFieldParamList(CreateList, type);
    }
    else if (str.contains("FLOAT", Qt::CaseInsensitive))
        AddToCreateFieldParamList(CreateList, fmtt_FLOAT);
    else if (str.contains("DOUBLE", Qt::CaseInsensitive))
        AddToCreateFieldParamList(CreateList, fmtt_DOUBLE);
    else if (str.contains("MONEY", Qt::CaseInsensitive))
        AddToCreateFieldParamList(CreateList, fmtt_MONEY);
    else if (str.contains(QRegularExpression("(STRING|VARCHAR2|SNR|UCHR)[\\n\\s\\t]*\\((\\d+)\\)", QRegularExpression::CaseInsensitiveOption), &match))
    {
        FmtFldType type = fmtt_STRING;
        int size = match.captured(2).toInt();

        if (match.captured(1).contains("VARCHAR2", Qt::CaseInsensitive))
            size ++;

        AddToCreateFieldParamList(CreateList, type, size);
    }
    else if (str.contains("DATE", Qt::CaseInsensitive) || str.contains("DATA", Qt::CaseInsensitive))
    {
        bool created = false;
        FmtFldType type = fmtt_DATE;

        if (str.contains("Дата", Qt::CaseInsensitive) || comment.contains("Date", Qt::CaseInsensitive) || comment.contains("Дата", Qt::CaseInsensitive) || comment.contains("DATA", Qt::CaseInsensitive))
        {
            type = fmtt_DATE;
            AddToCreateFieldParamList(CreateList, type);
            created = true;
        }

        if (str.contains("TIME", Qt::CaseInsensitive) || str.contains("Время", Qt::CaseInsensitive) || comment.contains("TIME", Qt::CaseInsensitive) || comment.contains("Время", Qt::CaseInsensitive))
        {
            type = fmtt_TIME;
            AddToCreateFieldParamList(CreateList, type);
            created = true;
        }

        if (!created)
            AddToCreateFieldParamList(CreateList, type);
    }
    else if (str.contains("TIME", Qt::CaseInsensitive))
        AddToCreateFieldParamList(CreateList, fmtt_TIME);
    else if (str.contains(QRegularExpression("(CHR|CHAR)[\\n\\s\\t]*\\((\\d+)\\)*", QRegularExpression::CaseInsensitiveOption), &match))
    {
        FmtFldType type = fmtt_CHR;
        int size = match.captured(2).toInt();

        if (size > 1)
            type = fmtt_STRING;

        if (!size)
            size = 1;

        AddToCreateFieldParamList(CreateList, type, size);
    }
    else if (str.contains("FLAG", Qt::CaseInsensitive))
        AddToCreateFieldParamList(CreateList, fmtt_CHR, 1);
    else if (str.contains("NUMERIC", Qt::CaseInsensitive))
        AddToCreateFieldParamList(CreateList, fmtt_NUMERIC);
    // форматы ТЗ
    else if (str.contains(QRegularExpression("N(\\d+)"), &match))
    {
        if (match.captured(1) == "10")
            AddToCreateFieldParamList(CreateList, fmtt_LONG);
        else if (match.captured(1) == "5")
            AddToCreateFieldParamList(CreateList, fmtt_INT);
    }
    else if (str.contains(QRegularExpression("[AaАа](\\d+)"), &match))
    {
        bool ok = false;
        int size = match.captured(1).toInt(&ok) + 1;

        if (!ok)
            size = -1;

        AddToCreateFieldParamList(CreateList, fmtt_STRING, size);
    }
    else if (str.contains(QRegularExpression("(dd.mm.yyyy)[\\n\\s\\t]+(hh:mm:ss)"), &match))
    {
        AddToCreateFieldParamList(CreateList, fmtt_DATE);
        AddToCreateFieldParamList(CreateList, fmtt_TIME);
    }
    else if (str.contains(QRegularExpression("(dd.mm.yyyy)"), &match))
        AddToCreateFieldParamList(CreateList, fmtt_DATE);
    else if (str.contains(QRegularExpression("(hh:mm:ss)"), &match))
        AddToCreateFieldParamList(CreateList, fmtt_TIME);

    return CreateList;
}
