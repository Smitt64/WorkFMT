#ifndef JSFMTFILE_H
#define JSFMTFILE_H

#include <QObject>
#include <QScriptClass>
#include "fmttable.h"
#include "jsfmttable.h"
#include "fmtfile.h"

class QScriptContext;
class JsFmtFile : public QObject,  public QScriptClass
{
    Q_OBJECT
public:
    JsFmtFile(QScriptEngine *engine);
    virtual ~JsFmtFile();

    QScriptValue newInstance(const FmtFile &file);
    QScriptValue newInstance();
    QScriptValue constructor();
    virtual QString name() const;
    QScriptValue prototype() const;

    QScriptValue::PropertyFlags propertyFlags(const QScriptValue &/*object*/, const QScriptString &name, uint /*id*/);
    QueryFlags queryProperty(const QScriptValue &object, const QScriptString &name, QueryFlags flags, uint *id);

private:
    static QScriptValue construct(QScriptContext *ctx, QScriptEngine *);
    static QScriptValue toScriptValue(QScriptEngine *eng, const FmtFile &ba);
    static void fromScriptValue(const QScriptValue &obj, FmtFile &ba);
    QSharedPointer<FmtTable> pTable;

    QScriptValue proto;
    QScriptValue ctor;
};

void initializeJsFmtFile(QScriptEngine *engine);
#endif // JSFMTFILE_H
