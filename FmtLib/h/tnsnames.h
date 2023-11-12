#ifndef TNSNAMES_H
#define TNSNAMES_H

#include <QObject>
#include <QDir>
#include <QMap>
#include "fmtlib_global.h"

class FMTLIBSHARED_EXPORT TnsNames
{
public:
    typedef struct
    {
        QString _Name, _Protocol, _ServiceName, _Host;
        qint32 _Port;

    } TnsElement;
    TnsNames(const QString &oraHome);
    virtual ~TnsNames();

    TnsElement element(const QString &str) const;

private:
    void load();
    QString clearString(QString str);
    int findClosePose(const QString &str, int pos);

    QDir _oraHome;
    QMap<QString, TnsElement> m_List;
};

#endif // TNSNAMES_H
