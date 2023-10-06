#include "gensqltemplatedlg.h"
#include "ui_gensqltemplatedlg.h"
#include "fmttable.h"
#include "fmtfield.h"
#include "fmtcore.h"
#include <QPushButton>

GenSqlTemplateDlg::GenSqlTemplateDlg(QSharedPointer<FmtTable> &table, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenSqlTemplateDlg)
{
    ui->setupUi(this);
    pTable = table;

    m_FldList = pTable->getFieldsList();

    connect(ui->mainFieldsBtn, &QPushButton::clicked, this, &GenSqlTemplateDlg::mainFieldsBtnClick);
}

GenSqlTemplateDlg::~GenSqlTemplateDlg()
{
    delete ui;
}

QString GenSqlTemplateDlg::templateName() const
{
    return ui->templateBox->currentText();
}

GenSqlTemplateDlg::SqlTemplate GenSqlTemplateDlg::templateID() const
{
    return (SqlTemplate)ui->templateBox->currentIndex();
}

QString GenSqlTemplateDlg::sqlTemplate() const
{
    QString sql;
    QTextStream stream(&sql);
    int templ = ui->templateBox->currentIndex();

    auto baseInsertFunc = [&stream](QSharedPointer<FmtTable> Table, QList<FmtField*> FldList) -> void
    {
        stream << "INSERT INTO " << Table->name().toUpper() << "(";

        foreach (const FmtField *fld, FldList) {
            stream << QString("%1").arg(fld->name().toUpper());
            if (fld != FldList.last())
                stream << ",";
        }

        stream << ") values(";
    };

    auto baseInsertPlaceholders = [&stream, baseInsertFunc](QSharedPointer<FmtTable> Table, QList<FmtField*> FldList) -> void
    {
        baseInsertFunc(Table, FldList);

        foreach (const FmtField *fld, FldList) {
            stream << QString("?");
            if (fld != FldList.last())
                stream << ",";
        }

        stream << ")";
    };

    if (templ == sqlInsertWithDefaults)
    {
        baseInsertFunc(pTable, m_FldList);

        foreach (const FmtField *fld, m_FldList) {
            QString OraDefaultVal = fld->getOraDefaultVal();
            OraDefaultVal = OraDefaultVal.replace("''", "'");

            stream << QString("%1").arg(OraDefaultVal);
            if (fld != m_FldList.last())
                stream << ",";
        }

        stream << ")";
    }
    else if (templ == sqlInsertWithPlaceholders)
        baseInsertPlaceholders(pTable, m_FldList);
    else if (templ == sqlInsertForRsdCommand)
    {
        foreach (const FmtField *fld, m_FldList) {
            if (fld->isStringType())
            {
                stream << QString("%1 rsd_%2[klen(%3,%4)];")
                          .arg(fld->getRsdType())
                          .arg(fld->undecorateName())
                          .arg(FmtTableStructName(pTable->name()))
                          .arg(fld->undecorateName())
                       << endl;

                stream << QString("RSDLONG rsd_%1_len = sizeof(rsd_%1);")
                          .arg(fld->undecorateName()) << endl;

                stream << QString("strncpyz(rsd_%1, '\\0', klen(%2,%3));")
                          .arg(fld->undecorateName())
                          .arg(FmtTableStructName(pTable->name()))
                          .arg(fld->undecorateName())
                       << endl;

                stream << endl;
            }
            else
            {
                stream << QString("%2 rsd_%1;")
                          .arg(fld->undecorateName())
                          .arg(fld->getRsdType()) << endl;

                stream << QString("RSDLONG rsd_%1_len = sizeof(rsd_%1);")
                          .arg(fld->undecorateName()) << endl;

                stream << endl;
            }
        }

        stream << "std::string strSQL = \"";
        baseInsertPlaceholders(pTable, m_FldList);
        stream << "\");" << endl;
        stream << QString("CRsdCommand cmd(*getDefConnection(), strSQL.c_str());") << endl << endl;

        foreach (const FmtField *fld, m_FldList) {
            if (fld->isStringType())
                stream << QString("cmd.addParam(\"\", %1, rsd_%2, &rsd_%2_len, rsd_%2_len);")
                          .arg(fld->getRsdConstant())
                          .arg(fld->undecorateName())
                       << endl;
            else
                stream << QString("cmd.addParam(\"\", %1, &rsd_%2, &rsd_%2_len, rsd_%2_len);")
                          .arg(fld->getRsdConstant())
                          .arg(fld->undecorateName())
                       << endl;
        }

        stream << endl;
        stream <<  "stat = AL_SQLCmdExecute(cmd);" << endl << "if ( stat == RSDRES_NODATA ) stat = 0;";
    }

    return sql;
}

void GenSqlTemplateDlg::mainFieldsBtnClick()
{
    if (SelectTableFieldsDlg(pTable, tr("Основные поля"), &m_FldList, this))
    {
        if (m_FldList.size() == pTable->fieldsCount())
            ui->mainFields->setText("Все");
        else
        {
            QString str;
            QTextStream stream(&str);
            for (QList<FmtField*>::iterator iter = m_FldList.begin(); iter != m_FldList.end(); ++iter)
            {
                if (iter != m_FldList.begin())
                    stream << ",";

                stream << (*iter)->name();
            }

            ui->mainFields->setText(str);
        }
    }
}
