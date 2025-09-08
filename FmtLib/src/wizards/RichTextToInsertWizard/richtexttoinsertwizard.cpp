#include "richtexttoinsertwizard.h"
#include "src/massop/massopselecttablespage.h"
#include "fmttable.h"
#include "src/wizards/RichTextToInsertWizard/fmtimportfldsourcepage.h"
#include "src/wizards/richtextpage.h"
#include <QTextDocument>

class RichTextToInsertSelectTablesPage : public MassOpSelectTablesPage
{
public:
    RichTextToInsertSelectTablesPage(ConnectionInfo *info, QWidget *parent) :
        MassOpSelectTablesPage(info, parent)
    {
        setSingleSelectionMode(true);
    }

    virtual bool AddFunc(const QString &str) Q_DECL_OVERRIDE
    {
        RichTextToInsertWizard *pWizard = qobject_cast<RichTextToInsertWizard*>(wizard());
        return pWizard->addTables(str);
    }

    virtual bool RemoveFunc(const QString &str) Q_DECL_OVERRIDE
    {
        RichTextToInsertWizard *pWizard = qobject_cast<RichTextToInsertWizard*>(wizard());
        return pWizard->removeTables(str);
    }
};

RichTextToInsertWizard::RichTextToInsertWizard(ConnectionInfo *info, QWidget *parent, Qt::WindowFlags flags) :
    QWizard(parent, flags),
    m_pTable(nullptr)
{
    //m_pTable = new FmtTable(info, this);
    Init(info);
}

RichTextToInsertWizard::RichTextToInsertWizard(FmtTable *Table, QWidget *parent, Qt::WindowFlags flags) :
    QWizard(parent, flags)
{
    m_pTable = Table;

    Init(m_pTable->connection());
}

void RichTextToInsertWizard::Init(ConnectionInfo *info)
{
    setMinimumSize(800, 600);
    setWindowTitle(tr("Создание скрипта загрузки содержимого"));

    if (!m_pTable)
    {
        m_pSelectTables = new RichTextToInsertSelectTablesPage(info, this);
        addPage(m_pSelectTables);
    }

    m_pRichText = new QTextDocument(this);
    m_pRichTextPage = new RichTextPage(this);
    m_pRichTextPage->setDocument(m_pRichText);
    m_pRichTextPage->setTitle(tr("Таблица с данными"));
    addPage(m_pRichTextPage);

    m_pSourcePage = new FmtImportFldSourcePage(this);
    addPage(m_pSourcePage);
}

bool RichTextToInsertWizard::addTables(const QString &str)
{
    if (m_Tables.contains(str))
        return false;

    m_Tables.append(str);

    return true;
}

bool RichTextToInsertWizard::removeTables(const QString &str)
{
    m_Tables.removeOne(str);
    return true;
}

FmtTable *RichTextToInsertWizard::table() const
{
    return m_pTable;
}

QTextDocument *RichTextToInsertWizard::document()
{
    return m_pRichText;
}
