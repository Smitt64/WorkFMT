#include "fmtimportfldsourcepage.h"
#include "fmttable.h"
#include "fmtcore.h"
#include "fmtfield.h"
#include "src/widgets/filteredtablewidget.h"
#include "src/wizards/RichTextToInsertWizard/fmtsourcemodel.h"
#include "src/wizards/RichTextToInsertWizard/richtexttoinsertwizard.h"
#include "ui_fmtimportfldsourcepage.h"
#include <QTextFrame>
#include <QTextTable>
#include <QTextDocumentFragment>
#include <QScrollBar>
#include <QTreeView>
#include <QLineEdit>

class FieldsTableViewFilterController: public FilteredControlHandler
{
public:
    FieldsTableViewFilterController()
    {

    }

    QWidget *create(const int &index) Q_DECL_OVERRIDE
    {
        QLineEdit *w = new QLineEdit();

        if (index == FmtSourceModel::col_Name)
        {
            //connect(w, &QLineEdit::textChanged, pDelegate, &FmtFieldsDelegate::setNameText);
            w->setPlaceholderText(QObject::tr("Значение для поиска"));
        }
        if (index == FmtSourceModel::col_Type)
            w->setEnabled(false);
        if (index == FmtSourceModel::col_Source)
            w->setEnabled(false);

        return w;
    }
};

// ----------------------------------------------------------------------------------------------

// Делегат для ComboBox
SourceComboBoxDelegate::SourceComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void SourceComboBoxDelegate::setSourceCount(const qint16 &Count)
{
    SourceCount = Count;
}

QWidget *SourceComboBoxDelegate::createEditor(QWidget *parent,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    const FmtSourceModel *model = qobject_cast<const FmtSourceModel*>(index.model());

    FmtField *fld = model->field(index.row());
    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(true);
    editor->addItems(sourceNames(fld->type(), fld->size(), SourceCount));
    return editor;
}

void SourceComboBoxDelegate::setEditorData(QWidget *editor,
                                           const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QVariant data = index.model()->data(index, Qt::EditRole);
    comboBox->setCurrentText(data.toString());
}

void SourceComboBoxDelegate::setModelData(QWidget *editor,
                                          QAbstractItemModel *model,
                                          const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void SourceComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

QStringList SourceComboBoxDelegate::sourceNames(const qint16 &Type, const int &size, const qint16 &Count)
{
    QStringList lst({"NULL"});

    if (Type == fmtt_CHR || Type == fmtt_UCHR)
        lst.prepend("CHR(88)");

    lst.prepend(fmtGetOraDefaultVal(Type, size, false));

    if (Count)
    {
        for (int i = 0; i < Count; i++)
            lst.append(QString("{column_%1}").arg(i + 1));
    }

    return lst;
}

// ----------------------------------------------------------------------------------------------

FmtImportFldSourcePage::FmtImportFldSourcePage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::FmtImportFldSourcePage)
{
    ui->setupUi(this);
    setTitle(tr("Параметры заполнения"));

    m_pTreeView = new QTreeView(this);
    m_pTreeView->setObjectName(QString::fromUtf8("treeView"));
    m_pTreeView->setRootIsDecorated(false);
    m_pTreeView->setUniformRowHeights(true);
    m_pTreeView->setItemsExpandable(false);
    m_pTreeView->setExpandsOnDoubleClick(false);
    m_pTreeView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    m_pFilteredWidget = new FilteredTableWidget();
    m_pFilteredWidgetController = new FieldsTableViewFilterController();
    m_pFilteredWidget->setView(m_pTreeView, m_pTreeView->header());
    m_pFilteredWidget->setController(m_pFilteredWidgetController);

    ((QVBoxLayout*)layout())->insertWidget(0, m_pFilteredWidget);

    m_pModel = new FmtSourceModel(m_pTreeView);
    m_pTreeView->setModel(m_pModel);

    m_pDelegate = new SourceComboBoxDelegate(m_pTreeView);
    m_pTreeView->setItemDelegateForColumn(FmtSourceModel::col_Source, m_pDelegate);

    registerField("FirstAsHeader", ui->checkBox);
}

FmtImportFldSourcePage::~FmtImportFldSourcePage()
{
    delete ui;
}

void FmtImportFldSourcePage::initializePage()
{
    RichTextToInsertWizard *wzrd = qobject_cast<RichTextToInsertWizard*>(wizard());
    m_pModel->setTable(wzrd->table());

    QTextDocument *document = wzrd->document();
    ui->textBrowser->clear();
    extractFirstThreeRowsWithHeader(document);

    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);
}

void FmtImportFldSourcePage::extractFirstThreeRowsWithHeader(QTextDocument *m_pRichText)
{
    QTextDocument *newDocument = ui->textBrowser->document();

    QTextTable *table = nullptr;
    QTextFrame::iterator it;
    for (it = m_pRichText->rootFrame()->begin(); it != m_pRichText->rootFrame()->end(); ++it)
    {
        QTextFrame *childFrame = it.currentFrame();
        if (childFrame)
        {
            QTextTable *foundTable = qobject_cast<QTextTable*>(childFrame);
            if (foundTable)
            {
                table = foundTable;
                break;
            }
        }
    }

    if (!table)
        return;

    QTextCursor cursor(newDocument);
    int columns = table->columns();
    int rowsToCopy = qMin(3, table->rows()); // Копируем не более 3 строк


    QTextTable *newTable = cursor.insertTable(rowsToCopy + 1, columns); // +1 для заголовка
    for (int col = 0; col < columns; ++col)
    {
        QTextTableCell headerCell = newTable->cellAt(0, col);
        QTextCursor headerCursor = headerCell.firstCursorPosition();

        // Форматируем заголовок
        QTextCharFormat headerFormat;
        headerFormat.setFontWeight(QFont::Bold);
        headerFormat.setBackground(QBrush(QColor(240, 240, 240)));
        headerCursor.setCharFormat(headerFormat);
        headerCell.setFormat(headerFormat);

        headerCursor.insertText(QString("{column_%1}").arg(col + 1));
    }

    for (int row = 0; row < rowsToCopy; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            QTextTableCell sourceCell = table->cellAt(row, col);
            QTextTableCell targetCell = newTable->cellAt(row + 1, col); // +1 из-за заголовка

            if (sourceCell.isValid() && targetCell.isValid())
            {
                QTextCursor sCursor(m_pRichText);
                sCursor.setPosition(sourceCell.firstCursorPosition().position());
                sCursor.setPosition(sourceCell.lastCursorPosition().position(), QTextCursor::KeepAnchor);
                QString cellText = sCursor.selectedText();

                // Вставляем текст в целевую ячейку
                QTextCursor targetCursor = targetCell.firstCursorPosition();
                targetCursor.insertText(cellText);
            }
        }
    }

    m_pDelegate->setSourceCount(columns);
}
