#include "texteditpage.h"
#include "ui_texteditpage.h"
#include "tablemaptofmtwidget.h"
#include "importwizard.h"
#include <QTableView>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextTable>
#include <QTextCursor>
#include <QDebug>

TextEditPage::TextEditPage(FmtSharedTablePtr table, QStandardItemModel *model, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::TextEditPage)
{
    ui->setupUi(this);
    pTableModel = model;
    pTable = table;

    pTableWidget = new TableMapToFmtWidget(pTable, this);
    pTableWidget->tableWidget()->setModel(pTableModel);
    ui->verticalLayout->addWidget(pTableWidget);
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
}

TextEditPage::~TextEditPage()
{
    delete ui;
}

void TextEditPage::textChanged()
{
    QTextTable *table = Q_NULLPTR;
    QTextBlock block = ui->textEdit->document()->begin();

    pTableModel->clear();
    fieldsMap.clear();
    while(block.isValid())
    {
        QTextCursor blockCursor(block);
        table = blockCursor.currentTable();

        if (table)
        {
            QList<QStandardItem*> row;
            for (int i = 0; i < table->rows(); i++)
            {
                for (int j = 0; j < table->columns(); j++)
                {
                    QTextTableCell cell = table->cellAt(i, j);
                    QTextCursor fcur = cell.firstCursorPosition();
                    QTextCursor ecur = cell.lastCursorPosition();
                    QTextCursor cur(ui->textEdit->document());
                    cur.setPosition(fcur.position(), QTextCursor::MoveAnchor);
                    cur.setPosition(ecur.position(), QTextCursor::KeepAnchor);

                    QStandardItem *item = new QStandardItem();
                    item->setText(cur.selectedText());
                    row.append(item);
                }
                pTableModel->appendRow(row);
                row.clear();
            }
            break;
        }
        block = block.next();
    }

    for (int i = 0; i < pTableModel->rowCount(); i++)
    {
        QComboBox *combo = pTableWidget->columnFilter(i);
        qDebug() << combo->currentIndex() << combo->currentText();
        if (combo->currentIndex() != -1)
            fieldsMap[combo->currentIndex()] = i;
    }
}

int TextEditPage::nextId() const
{
    return page_MapFields;
}

int TextEditPage::getColumnForFmtField(const int &fieldId)
{
    if (!fieldsMap.contains(fieldId))
        return -1;
    return fieldsMap[fieldId];
}
