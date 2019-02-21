#ifndef FMTTABLELISTDELEGATE_H
#define FMTTABLELISTDELEGATE_H

#include <QStyledItemDelegate>

class FmtTableListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_PROPERTY(QString highlightText READ highlightText WRITE setHighlightText)
public:
    FmtTableListDelegate(QObject *parent = Q_NULLPTR);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QString highlightText() const;

public slots:
    void setHighlightText(const QString &text);

private:
    QString m_HighlightText;
};

#endif // FMTTABLELISTDELEGATE_H
