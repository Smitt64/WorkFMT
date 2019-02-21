#ifndef JSFMTLIB_H
#define JSFMTLIB_H

#include <QObject>
#include <QtScript>
#include <oracletnslistmodel.h>

Q_GLOBAL_STATIC(OracleTnsListModel, pTnsListModel)

class JsFmtLib : public QObject, public QScriptable
{
    Q_OBJECT
public:
    JsFmtLib(QObject *parent = Q_NULLPTR);

public slots:
    QStringList allFmtTypes() const;
};

#endif // JSFMTLIB_H
