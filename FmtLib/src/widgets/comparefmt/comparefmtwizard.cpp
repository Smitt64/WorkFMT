#include "comparefmtwizard.h"
#include "compareparamspage.h"
#include "comparecppstructpage.h"
#include "compareselecttablepage.h"
#include "codeeditor/codeeditor.h"
#include <QIcon>

CompareFmtWizard::CompareFmtWizard(ConnectionInfo *connection, QWidget *parent) :
    QWizard(parent)
{
    m_Connection = connection;
    m_ParamsPage = new CompareParamsPage(this);
    m_CodePage = new CompareCppStructPage(this);
    m_pSeltablePage = new CompareSelectTablePage(this);

    addPage(m_ParamsPage);
    addPage(m_CodePage);
    addPage(m_pSeltablePage);

    setWindowIcon(QIcon(":/img/SychronizeListHS.png"));
    setWindowTitle(tr("Мастер сравнения записей словаря"));
    setFixedSize(800, 600);
}

CompareFmtWizard::~CompareFmtWizard()
{

}

QString CompareFmtWizard::structString() const
{
    return field("struct").toString();
}

ConnectionInfo *CompareFmtWizard::connection()
{
    return m_Connection;
}
