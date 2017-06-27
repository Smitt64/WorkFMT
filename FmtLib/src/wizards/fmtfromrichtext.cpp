#include "fmtfromrichtext.h"
#include <QtWidgets>
#include "fmttable.h"
#include "fmtfield.h"
#include "fmtworkwindow.h"
#include "fmtcore.h"
#include "connectioninfo.h"

enum
{
    page_Document,
    page_FmtEdit
};

FmtFromRichText::FmtFromRichText(ConnectionInfo *connection, QWidget *parent, Qt::WindowFlags flags)
    :QWizard(parent, flags)
{
    pDocument = new QTextEdit;
    pWorkWnd = new FmtWorkWindow;

    pFmtTable.reset(new FmtTable(connection));
    pWorkWnd->setFmtTable(pFmtTable);
    pWorkWnd->setDialogButtonsVisible(false);
    pWorkWnd->setInitButtonVisible(false);
    pWorkWnd->adjustSize();

    pDocument->setMinimumSize(pWorkWnd->size());

    pNameLabel = new QLabel(tr("Наименование: "));
    pTypeLabel = new QLabel(tr("Тип: "));
    pCommentLabel = new QLabel(tr("Комментарий: "));

    pNameCombo = new QComboBox();
    pNameCombo->setModel(&m_ColumnsItems);

    pTypeCombo = new QComboBox();
    pTypeCombo->setModel(&m_ColumnsItems);

    pCommentCombo = new QComboBox();
    pCommentCombo->setModel(&m_ColumnsItems);

    addPage(createRichTextPage());
    addPage(createFmtWorkPage());
    //addPage(createFinishPage());

    connect(this, SIGNAL(currentIdChanged(int)), SLOT(onPageChanged(int)));
    connect(pDocument, SIGNAL(textChanged()), SLOT(onTextChanged()));
}

FmtFromRichText::~FmtFromRichText()
{
    //delete pFmtTable;
    //delete pRichTextPage;
}

QWizardPage *FmtFromRichText::createRichTextPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle(tr("Вставьте текст"));
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *grid = new QHBoxLayout;
    grid->addWidget(pNameLabel);
    grid->addWidget(pNameCombo);
    grid->addWidget(pTypeLabel);
    grid->addWidget(pTypeCombo);
    grid->addWidget(pCommentLabel);
    grid->addWidget(pCommentCombo);

    layout->addLayout(grid);
    layout->addWidget(pDocument);
    page->setLayout(layout);

    return page;
}

QWizardPage *FmtFromRichText::createFmtWorkPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle(tr("Редактировать таблицу"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(pWorkWnd);
    page->setLayout(layout);

    return page;
}

QWizardPage *FmtFromRichText::createFinishPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle(tr("Завершение"));

    OpenCheck = new QCheckBox(tr("Открыть в рабочей области"), this);
    CreateTableCheck = new QCheckBox(tr("Создать таблицу в базе"), this);
    CreateIndexCheck = new QCheckBox(tr("Создать индексы и триггеры"), this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(OpenCheck);
    layout->addWidget(CreateTableCheck);
    layout->addWidget(CreateIndexCheck);

    page->setLayout(layout);
    return page;
}

void FmtFromRichText::onPageChanged(const int &id)
{
    if (id == page_FmtEdit)
    {
        pFmtTable->clear();

        QTextTable *table = NULL;
        QTextBlock block = pDocument->document()->begin();

        while(block.isValid())
        {
            QTextCursor blockCursor(block);
            table = blockCursor.currentTable();

            int pos = 0;

            // (\"|\«)(\W+)(\"|\»)
            QString str = block.text().replace("«", "\"").replace("»", "\"");
            QRegExp rxComment("(\\\"(([^\"])*)\\\")");
            if ((pos = rxComment.indexIn(str, pos)) != -1)
            {
                pFmtTable->setComment(rxComment.cap(2));
            }

            pos = 0;
            QRegExp rx(QString::fromUtf8("\\b(\\w+)\\.(dbt|tmp|rec)"));
            if ((pos = rx.indexIn(str, pos)) != -1)
            {
                pFmtTable->setName(QString("d%1_%2").arg(rx.cap(1), rx.cap(2)));
            }

            if (table)
            {
                int i = 1;

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

                    FmtField *fld = pFmtTable->addField(fcur.selectedText().simplified(), fmtt_INT);
                    fld->setComment(fcur3.selectedText().simplified());

                    int size = 0;
                    fld->setType(GetTypeSize(fcur2.selectedText().simplified(), &size));
                    fld->setSize(size);
                }
                break;
            }
            block = block.next();
        }
    }
}

void FmtFromRichText::onTextChanged()
{
    QTextTable *table = NULL;
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

int FmtFromRichText::GetTypeSize(const QString &str, int *size)
{
    int type = fmtt_INT;
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
