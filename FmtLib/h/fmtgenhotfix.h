#ifndef FMTGENHOTFIX_H
#define FMTGENHOTFIX_H

#include "fmtgeninterface.h"

class FmtGenHotFix : public FmtGenInterface
{
    Q_OBJECT
public:
    FmtGenHotFix();

    virtual GenType getContentType() const { return FmtGenInterface::GenSql; }

    virtual QWidget *propertyEditor() { return NULL; }
    virtual bool hasPropertes() const { return false; }

protected:
    virtual QByteArray makeContent(QSharedPointer<FmtTable> pTable);

private:
    void WrapSqlBlock(QTextStream &stream, const QString &block);
    QString Simplify(QString line);
};

#endif // FMTGENHOTFIX_H
