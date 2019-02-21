#include "jsfmtlib.h"
#include <fmtcore.h>

/*!
    \class JsFmtLib
    \brief JsFmtLib класс, содержащий вспомогатеоьные мункции

    \ingroup tools
*/
JsFmtLib::JsFmtLib(QObject *parent) :
    QObject(parent),
    QScriptable()
{
    setObjectName("JsFmtLib");
}

/*!
    \fn QStringList JsFmtLib::allFmtTypes() const
    Возвращает наименований доступных типов полей
    \code
    var lst = FmtLib.allFmtTypes();
    for (var i = 0; i < lst.length; i++)
        print(lst[i]);
    \endcode
    Вывод:
    \code
    INT
    LONG
    BIGINT
    FLOAT
    DOUBLE
    MONEY
    STRING
    SNR
    DATE
    TIME
    CHR
    UCHR
    NUMERIC
    \endcode
*/
QStringList JsFmtLib::allFmtTypes() const
{
    return fmtTypes();
}
