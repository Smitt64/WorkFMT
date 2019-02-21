#ifndef DATELINEEDIT_H
#define DATELINEEDIT_H

#include <QtWidgets>

class QRegExpValidator;
class DateLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QDate date READ date WRITE setDate)
public:
    DateLineEdit(QWidget *parent = Q_NULLPTR);
    virtual ~DateLineEdit();

    QDate date() const;
    static QString maskString();
    static QString regexpPattern();
    static QString dateFormat();

public slots:
    void setDate(const QDate &value);

private:
    QRegExpValidator *pValidator;
};

#endif // DATELINEEDIT_H
