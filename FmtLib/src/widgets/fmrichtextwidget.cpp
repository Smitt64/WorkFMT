#include "fmrichtextwidget.h"
#include "fmtcore.h"
#include "fmtfield.h"
#include <QSpacerItem>

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
            break;
        }
        block = block.next();
    }
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

                FmtField *fld = Q_NULLPTR;
                if (prm.insertBefore < 0)
                    fld = pTable->addField(fcur.selectedText().simplified(), fmtt_INT);
                else
                    fld = pTable->insertField(prm.insertBefore, fcur.selectedText().simplified(), fmtt_INT);

                int size = 0;
                QString comment = fcur3.selectedText().simplified();
                FmtFldType type = GetTypeSize(fcur2.selectedText().simplified(), &size, comment);

                if (pStringCheck->checkState() != Qt::Unchecked)
                    ++ size;

                fld->setType(type);
                fld->setComment(comment);
                fld->setSize(size);
            }
            break;
        }

        block = block.next();
    }
}

FmtFldType FmRichTextWidget::GetTypeSize(const QString &str, int *size, const QString &comment)
{
    FmtFldType type = fmtt_INT;
    *size = fmtTypeSize(type);

    if (str.contains("LONG", Qt::CaseInsensitive))
    {
        type = fmtt_LONG;
        *size = fmtTypeSize(type);
    }
    else if (str.contains("BIGINT", Qt::CaseInsensitive))
    {
        type = fmtt_BIGINT;
        *size = fmtTypeSize(type);
    }
    else if (str.contains("NUMBER", Qt::CaseInsensitive))
    {
        QRegExp rx("\\((\\d+)\\)");
        int pos = 0;
        if ((pos = rx.indexIn(str, pos)) != -1)
        {
            *size = rx.cap(1).toInt();
            if (*size == 10)
                type = fmtt_LONG;
            else if (*size == 19)
                type = fmtt_BIGINT;
            else
                type = fmtt_INT;
        }
        *size = fmtTypeSize(type);
    }
    else if (str.contains("FLOAT", Qt::CaseInsensitive))
    {
        type = fmtt_FLOAT;
        *size = fmtTypeSize(type);
    }
    else if (str.contains("DOUBLE", Qt::CaseInsensitive))
    {
        type = fmtt_DOUBLE;
        *size = fmtTypeSize(type);
    }
    else if (str.contains("MONEY", Qt::CaseInsensitive))
    {
        type = fmtt_MONEY;
        *size = fmtTypeSize(type);
    }
    else if (str.contains("STRING", Qt::CaseInsensitive))
    {
        type = fmtt_STRING;
        QRegExp rx("\\((\\d+)\\)");
        int pos = 0;
        if ((pos = rx.indexIn(str, pos)) != -1)
        {
            *size = rx.cap(1).toInt();
        }
    }
    else if (str.contains("VARCHAR2", Qt::CaseInsensitive))
    {
        type = fmtt_STRING;
        QRegExp rx("\\((\\d+)\\)");
        int pos = 0;
        if ((pos = rx.indexIn(str, pos)) != -1)
        {
            *size = rx.cap(1).toInt() + 1;
        }
    }
    else if (str.contains("SNR", Qt::CaseInsensitive))
    {
        type = fmtt_SNR;
        QRegExp rx("\\((\\d+)\\)");
        int pos = 0;
        if ((pos = rx.indexIn(str, pos)) != -1)
        {
            *size = rx.cap(1).toInt();
        }
    }
    else if (str.contains("DATE", Qt::CaseInsensitive))
    {
        type = fmtt_DATE;
        *size = fmtTypeSize(type);

        if (str.contains("TIME", Qt::CaseInsensitive) || str.contains("Время", Qt::CaseInsensitive) || comment.contains("TIME", Qt::CaseInsensitive) || comment.contains("Время", Qt::CaseInsensitive))
            type = fmtt_TIME;
    }
    else if (str.contains("TIME", Qt::CaseInsensitive))
    {
        type = fmtt_TIME;
        *size = fmtTypeSize(type);
    }
    else if (str.contains("CHR", Qt::CaseInsensitive) || str.contains("CHAR", Qt::CaseInsensitive))
    {
        type = fmtt_CHR;
        QRegExp rx("\\((\\d+)\\)");
        int pos = 0;
        if ((pos = rx.indexIn(str, pos)) != -1)
        {
            *size = rx.cap(1).toInt();
        }
    }
    else if (str.contains("FLAG", Qt::CaseInsensitive))
    {
        type = fmtt_CHR;
        *size = 1;
    }
    else if (str.contains("UCHR", Qt::CaseInsensitive))
    {
        type = fmtt_CHR;
        QRegExp rx("\\((\\d+)\\)");
        int pos = 0;
        if ((pos = rx.indexIn(str, pos)) != -1)
        {
            *size = rx.cap(1).toInt();
        }
    }
    else if (str.contains("NUMERIC", Qt::CaseInsensitive))
    {
        type = fmtt_NUMERIC;
        *size = fmtTypeSize(type);
    }
    return type;
}
