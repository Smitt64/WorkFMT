#ifndef LINEEDITACTION_H
#define LINEEDITACTION_H

#include <QObject>
#include <QWidgetAction>

class FmtFildsModel;
class QLineEdit;
class LineEditAction : public QWidgetAction
{
    Q_OBJECT
public:
    LineEditAction(QObject *parent = Q_NULLPTR);
    virtual ~LineEditAction();

public slots:
    void setFocus();

signals:
    void textChanged(const QString &str);

protected:
    virtual QWidget *createWidget(QWidget *parent);
    virtual void deleteWidget(QWidget *widget);

private:
    FmtFildsModel *pModel;
    QLineEdit *pLineEdit;
};

#endif // LINEEDITACTION_H
