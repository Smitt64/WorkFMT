#include "massopenfucnparams.h"
#include "ui_massopenfucnparams.h"
#include <codeeditor/codeeditor.h>
#include <codeeditor/codehighlighter.h>
#include "massoperationwizard.h"
#include <QRegularExpression>
#include <QTextStream>
#include <QMapIterator>
#include "fmtgencpptemplate.h"

const QString struct_template = "${name}_FILES";
const QString flags_template = "${name}_Flags";

MassOpenFucnParams::MassOpenFucnParams(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassOpenFucnParams)
{
    ui->setupUi(this);
    pEditor = new CodeEditor(this);
    pEditor->setReadOnly(true);

    setTitle(tr("Результат"));

    //m_Highlighter = new Highlighter(pEditor->document());
    ToolApplyHighlighter(pEditor, HighlighterCpp);
    ui->verticalLayout->addWidget(pEditor);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
}

MassOpenFucnParams::~MassOpenFucnParams()
{
    delete ui;
}

void MassOpenFucnParams::initializePage()
{
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
    m_template.clear();
    params.clear();

    QRegularExpression re("d(([\\w\\d])+)_\\w{3}");
    QStringList tables = wzrd->tables();
    foreach(const QString &table, tables) {
        QRegularExpressionMatch match = re.match(table);
        if (match.hasMatch())
        {
            QString structName = match.captured(1).toUpper();
            params[table] = structName;
        }
    }

    QTextStream stream(&m_template);
    stream << QString("// bfopctrl.c") << Qt::endl;
    stream << QString("typedef struct {") << Qt::endl;

    QMapIterator<QString, QString> iter(params);
    while (iter.hasNext())
    {
        iter.next();
        stream << tab << QString("OPF_UNI open%1;").arg(iter.value()) << Qt::endl;
    }

    stream << tab << QString("void* prev;") << Qt::endl;
    stream << QString("} %1;").arg(struct_template) << Qt::endl;

    stream << Qt::endl;

    stream << QString("typedef struct {") << Qt::endl;
    stream << tab << QString("void *prev; // ссылка на предыдущий вызов открывашки") << Qt::endl;
    stream << tab << QString("%1 flag;").arg(struct_template) << Qt::endl;
    stream << QString("} %1;").arg(flags_template) << Qt::endl;

    stream << Qt::endl;
    stream << QString("static %1 *currFiles%2;").arg(flags_template).arg(struct_template) << Qt::endl;
    stream << Qt::endl;

    stream << QString("void Close%1()").arg(struct_template) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("%1 *old;").arg(flags_template) << Qt::endl;
    stream << tab << QString("if (currFiles%1)").arg(struct_template) << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << QString("TBfErrorInfo BfError;") << Qt::endl;
    stream << tab << tab << QString("BfSaveError(&BfError);") << Qt::endl << Qt::endl;
    stream << tab << tab << QString("if (bfstat == 0)") << Qt::endl;
    stream << tab << tab << tab << QString("BfRestoreError(&BfError);") << Qt::endl << Qt::endl;

    iter.toFront();
    while (iter.hasNext())
    {
        iter.next();
        stream << tab << tab <<
                  QString("BankCloseFileUNI(&File%1, &currFiles%2->flag.open%1);")
               .arg(iter.value())
               .arg(struct_template) << Qt::endl;
    }

    stream << Qt::endl;
    stream << tab << tab << QString("old = currFiles%1;").arg(struct_template) << Qt::endl;
    stream << tab << tab << QString("currFiles%1 = (%2*)currFiles%1->prev;").arg(struct_template).arg(flags_template) << Qt::endl;
    stream << tab << tab << QString("FreeMem((void**)&old);") << Qt::endl;
    stream << tab << "}" << Qt::endl;
    stream << "}" << Qt::endl;

    stream << Qt::endl;
    stream << QString("int Open%1(int fmode, int WhatToSave)").arg(struct_template) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;
    stream << tab << QString("%1 *newF;").arg(flags_template) << Qt::endl;
    stream << tab << QString("newF = (%1*)Allocate(sizeof(%1));").arg(flags_template) << Qt::endl << Qt::endl;

    stream << tab << QString("if (!newF)") << Qt::endl;
    stream << tab << tab << QString("return OUT_OF_MEMORY;") << Qt::endl << Qt::endl;

    stream << tab << QString("newF->prev = (void*)currFiles%1;").arg(struct_template) << Qt::endl;
    stream << tab << QString("currFiles%1 = newF;").arg(struct_template) << Qt::endl << Qt::endl;

    iter.toFront();
    while (iter.hasNext())
    {
        iter.next();
        stream << tab << QString("if (!stat) stat = BankOpenFileUNI(");
        stream << QString("File%1, iOpen%1, fmode, OPF_SaveFilters, &newF->flag.open%1);").arg(iter.value()) << Qt::endl;
    }

    stream << Qt::endl << tab << "if (stat)" << Qt::endl;
    stream << tab << tab << QString("Close%1();").arg(struct_template) << Qt::endl << Qt::endl;

    stream << tab << "return stat;" << Qt::endl;
    stream << "}" << Qt::endl;

    pEditor->setPlainText(QString::fromLocal8Bit(m_template));
}

void MassOpenFucnParams::textChanged(const QString &text)
{
    QString value = QString::fromLocal8Bit(m_template);
    pEditor->setPlainText(value.replace("${name}", text));
}
