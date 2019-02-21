#include "datelineedit.h"

DateLineEdit::DateLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    pValidator = new QRegExpValidator(QRegExp(regexpPattern()), this);
    setValidator(pValidator);
    setInputMask(maskString());
}

DateLineEdit::~DateLineEdit()
{

}

QString DateLineEdit::maskString()
{
    return QString("99\\.99\\.9999;_");
}

QString DateLineEdit::regexpPattern()
{
    return QString("((0[0-9]|[12]\\d|(3|0)[01])\\.(0[0-9]|(1|0)[0-2])\\.[012]\\d{3})");
}

QString DateLineEdit::dateFormat()
{
    return QString("dd.MM.yyyy");
}

void DateLineEdit::setDate(const QDate &value)
{
    if (value == QDate(1,1,1))
        setText("00.00.0000");
    else
        setText(value.toString(dateFormat()));
}

QDate DateLineEdit::date() const
{
    QDate ret;
    if (text() == "00.00.0000")
        ret = QDate(1,1,1);
    else
        ret = QDate::fromString(text(), dateFormat());

    return ret;
}
