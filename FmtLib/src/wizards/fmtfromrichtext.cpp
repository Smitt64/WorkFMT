#include "fmtfromrichtext.h"
#include <QtWidgets>
#include "fmttable.h"
#include "fmtfield.h"
#include "fmtworkwindow.h"
#include "fmtcore.h"
#include "connectioninfo.h"
#include "fmrichtextwidget.h"

enum
{
    page_Document,
    page_FmtEdit
};

FmtFromRichText::FmtFromRichText(ConnectionInfo *connection, QWidget *parent, Qt::WindowFlags flags)
    :QWizard(parent, flags)
{
    pWorkWnd = new FmtWorkWindow;

    pFmtTable.reset(new FmtTable(connection));
    pWorkWnd->setFmtTable(pFmtTable);
    pWorkWnd->setDialogButtonsVisible(false);
    pWorkWnd->setInitButtonVisible(false);
    pWorkWnd->adjustSize();

    addPage(createRichTextPage());
    addPage(createFmtWorkPage());

    connect(this, SIGNAL(currentIdChanged(int)), SLOT(onPageChanged(int)));
}

FmtFromRichText::~FmtFromRichText()
{
}

QWizardPage *FmtFromRichText::createRichTextPage()
{
    FmRichTextWidget *page = new FmRichTextWidget;
    page->setDocumentMinimumSize(pWorkWnd->size());

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
        FmRichTextWidget *richpage = qobject_cast<FmRichTextWidget*>(page(0));

        if (richpage)
        {
            pFmtTable->clear();

            FmRichTextReadParam prm;
            prm.insertBefore = -1;
            prm.readComment = true;
            prm.readTableName = true;
            prm.ignoreFirstRow = true;
            richpage->ReadConetent(pFmtTable, prm);
        }
    }
}
