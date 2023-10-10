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
    connect(ui->otherFieldsBtn, &QPushButton::clicked, this, &GenSqlTemplateDlg::otherFieldsBtnClick);
    connect(ui->templateBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &GenSqlTemplateDlg::currentIndexChanged);
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

void GenSqlTemplateDlg::currentIndexChanged(const int &index)
{
    if (index < sqlUpdateWithDefaults)
    {
        ui->otherFields->setEnabled(false);
        ui->otherFieldsBtn->setEnabled(false);
    }
    else
    {
        ui->otherFields->setEnabled(true);
        ui->otherFieldsBtn->setEnabled(true);
    }
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
                stream << ", ";
        }

        stream << ") values(";
    };

    auto baseInsertPlaceholders = [&stream, baseInsertFunc](QSharedPointer<FmtTable> Table, QList<FmtField*> FldList) -> void
    {
        baseInsertFunc(Table, FldList);

        foreach (const FmtField *fld, FldList) {
            stream << QString("?");
            if (fld != FldList.last())
                stream << ", ";
        }

        stream << ")";
    };

    auto baseUpdateFunc = [&stream](QSharedPointer<FmtTable> Table, const QList<FmtField*> &FldList, const QList<FmtField*> &OtherFldList, bool UseDefaults = true) -> void
    {
        stream << "UPDATE " << Table->name().toUpper() << " SET ";

        foreach (const FmtField *fld, FldList) {
            if (UseDefaults)
            {
                QString OraDefaultVal = fld->getOraDefaultVal();
                OraDefaultVal = OraDefaultVal.replace("''", "'");

                stream << QString("%1").arg(fld->name().toUpper()) << " = " << OraDefaultVal;
            }
            else
                stream << QString("%1").arg(fld->name().toUpper()) << " = ?";

            if (fld != FldList.last())
                stream << ", ";
        }

        if (!OtherFldList.isEmpty())
        {
            stream << " WHERE ";

            foreach (const FmtField *fld, OtherFldList) {
                if (UseDefaults)
                {
                    QString OraDefaultVal = fld->getOraDefaultVal();
                    OraDefaultVal = OraDefaultVal.replace("''", "'");

                    stream << QString("%1").arg(fld->name().toUpper()) << " = " << OraDefaultVal;
                }
                else
                    stream << QString("%1").arg(fld->name().toUpper()) << " = ?";

                if (fld != OtherFldList.last())
                    stream << " AND ";
            }
        }
    };

    auto baseRsdParamsFunc = [&stream](QSharedPointer<FmtTable> Table, const QList<FmtField*> &FldList, const QString &pref = QString()) -> void
    {
        QString preffix;

        if (!pref.isEmpty())
            preffix = QString("%1_").arg(pref);

        foreach (const FmtField *fld, FldList) {
            if (fld->isStringType())
            {
                stream << QString("%1 rsd_%5%2[klen(%3,%4)];")
                          .arg(fld->getRsdType())
                          .arg(fld->undecorateName())
                          .arg(FmtTableStructName(Table->name()))
                          .arg(fld->undecorateName())
                          .arg(preffix)
                       << Qt::endl;

                stream << QString("RSDLONG rsd_%2%1_len = sizeof(rsd_%2%1);")
                          .arg(fld->undecorateName())
                          .arg(preffix)<< Qt::endl;

                stream << QString("strncpyz(rsd_%4%1, '\\0', klen(%2,%3));")
                          .arg(fld->undecorateName())
                          .arg(FmtTableStructName(Table->name()))
                          .arg(fld->undecorateName())
                          .arg(preffix)
                       << Qt::endl;

                stream << Qt::endl;
            }
            else
            {
                stream << QString("%2 rsd_%3%1;")
                          .arg(fld->undecorateName())
                          .arg(fld->getRsdType())
                          .arg(preffix)<< Qt::endl;

                stream << QString("RSDLONG rsd_%2%1_len = sizeof(rsd_%2%1);")
                          .arg(fld->undecorateName())
                          .arg(preffix)<< Qt::endl;

                stream << Qt::endl;
            }
        }
    };

    auto baseRsdParamsAddFunc = [&stream](QSharedPointer<FmtTable> Table, const QList<FmtField*> &FldList, const QString &pref = QString()) -> void
    {
        QString preffix;

        if (!pref.isEmpty())
            preffix = QString("%1_").arg(pref);

        foreach (const FmtField *fld, FldList) {
            if (fld->isStringType())
                stream << QString("cmd.addParam(\"\", %1, rsd_%3%2, &rsd_%3%2_len, rsd_%3%2_len);")
                          .arg(fld->getRsdConstant())
                          .arg(fld->undecorateName())
                          .arg(preffix)
                       << Qt::endl;
            else
                stream << QString("cmd.addParam(\"\", %1, &rsd_%3%2, &rsd_%3%2_len, rsd_%3%2_len);")
                          .arg(fld->getRsdConstant())
                          .arg(fld->undecorateName())
                          .arg(preffix)
                       << Qt::endl;
        }
    };

    if (templ == sqlInsertWithDefaults)
    {
        baseInsertFunc(pTable, m_FldList);

        foreach (const FmtField *fld, m_FldList) {
            QString OraDefaultVal = fld->getOraDefaultVal();
            OraDefaultVal = OraDefaultVal.replace("''", "'");

            stream << QString("%1").arg(OraDefaultVal);
            if (fld != m_FldList.last())
                stream << ", ";
        }

        stream << ")";
    }
    else if (templ == sqlInsertWithPlaceholders)
        baseInsertPlaceholders(pTable, m_FldList);
    else if (templ == sqlInsertForRsdCommand)
    {
        baseRsdParamsFunc(pTable, m_FldList);

        stream << "std::string strSQL = \"";
        baseInsertPlaceholders(pTable, m_FldList);
        stream << "\");" << Qt::endl;
        stream << QString("CRsdCommand cmd(*getDefConnection(), strSQL.c_str());") << endl << endl;

        baseRsdParamsAddFunc(pTable, m_FldList);

        stream << Qt::endl;
        stream <<  "stat = AL_SQLCmdExecute(cmd);" << Qt::endl << "if ( stat == RSDRES_NODATA ) stat = 0;";
    }
    else if (templ == sqlUpdateWithDefaults)
    {
        baseUpdateFunc(pTable, m_FldList, m_OtherFldList);
    }
    else if (templ == sqlUpdateWithPlaceholders)
    {
        baseUpdateFunc(pTable, m_FldList, m_OtherFldList, false);
    }
    else if (templ == sqlUpdateForRsdCommand)
    {
        baseRsdParamsFunc(pTable, m_FldList);
        baseRsdParamsFunc(pTable, m_OtherFldList, "where");

        stream << "std::string strSQL = \"";
        baseUpdateFunc(pTable, m_FldList, m_OtherFldList, false);
        stream << "\");" << Qt::endl;
        stream << QString("CRsdCommand cmd(*getDefConnection(), strSQL.c_str());") << endl << endl;

        baseRsdParamsAddFunc(pTable, m_FldList);
        baseRsdParamsAddFunc(pTable, m_OtherFldList, "where");

        stream << Qt::endl;
        stream <<  "stat = AL_SQLCmdExecute(cmd);" << Qt::endl << "if ( stat == RSDRES_NODATA ) stat = 0;";
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

void GenSqlTemplateDlg::otherFieldsBtnClick()
{
    if (SelectTableFieldsDlg(pTable, tr("Дополнительные поля"), &m_OtherFldList, this))
    {
        if (m_OtherFldList.size() == pTable->fieldsCount())
            ui->otherFields->setText("Все");
        else
        {
            QString str;
            QTextStream stream(&str);
            for (QList<FmtField*>::iterator iter = m_OtherFldList.begin(); iter != m_OtherFldList.end(); ++iter)
            {
                if (iter != m_OtherFldList.begin())
                    stream << ",";

                stream << (*iter)->name();
            }

            ui->otherFields->setText(str);
        }
    }
}
