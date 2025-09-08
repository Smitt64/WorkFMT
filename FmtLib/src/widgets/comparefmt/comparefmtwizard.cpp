#include "comparefmtwizard.h"
#include "compareparamspage.h"
#include "comparecppstructpage.h"
#include "compareselecttablepage.h"
#include "codeeditor/codeeditor.h"
#include "src/widgets/comparefmt/comparexmlpage.h"
#include "src/widgets/connectionwzardpage/connactionpage.h"
#include <QIcon>

CompareFmtWizard::CompareFmtWizard(ConnectionInfo *connection, QWidget *parent) :
    QWizard(parent)
{
    m_Connection = connection;
    m_ParamsPage = new CompareParamsPage(this);
    m_CodePage = new CompareCppStructPage(this);
    m_pSeltablePage = new CompareSelectTablePage(this);
    m_pConnectionPage = new ConnactionPage(this);
    m_XmlCodePage = new CompareXmlPage(this);

    addPage(m_ParamsPage);
    addPage(m_CodePage);
    addPage(m_pConnectionPage);
    addPage(m_pSeltablePage);
    addPage(m_XmlCodePage);

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

QString CompareFmtWizard::xmlDumpString() const
{
    return field("XmlDump").toString();
}

ConnectionInfo *CompareFmtWizard::connection()
{
    return m_Connection;
}
