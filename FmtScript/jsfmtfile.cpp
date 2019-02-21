#include <QtScript>
#include <stdlib.h>
#include "jsfmtfile.h"
#include "jsfmtfileprototype.h"

Q_DECLARE_METATYPE(FmtFile*)
Q_DECLARE_METATYPE(FmtFile)
Q_DECLARE_METATYPE(JsFmtFile*)

/*
 * https://www.mcom.nl/backup/harmattan/docs/html/qt4/script-customclass-bytearrayclass-h.html
https://www.mcom.nl/backup/harmattan/docs/html/qt4/script-customclass-bytearrayclass-cpp.html
http://doc.qt.io/qt-4.8/qt-script-customclass-example.html
 * */
void initializeJsFmtFile(QScriptEngine *engine)
{
    JsFmtFile *ptr = new JsFmtFile(engine);
    engine->globalObject().setProperty("FmtFile", ptr->constructor());
}

// ===================================================================================
JsFmtFile::JsFmtFile(QScriptEngine *engine) :
    QObject(engine),
    QScriptClass(engine)
{
    qScriptRegisterMetaType<FmtFile>(engine, toScriptValue, fromScriptValue);

    proto = engine->newQObject(new JsFmtFilePrototype(this), QScriptEngine::QtOwnership,
                               QScriptEngine::SkipMethodsInEnumeration
                               | QScriptEngine::ExcludeSuperClassMethods
                               | QScriptEngine::ExcludeSuperClassProperties);

    QScriptValue global = engine->globalObject();
    proto.setPrototype(global.property("Object").property("prototype"));

    ctor = engine->newFunction(construct, proto);
    ctor.setData(engine->toScriptValue(this));
}

JsFmtFile::~JsFmtFile()
{

}

QScriptValue JsFmtFile::newInstance(const FmtFile &file)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(file));
    return engine()->newObject(this, data);
}

QScriptValue JsFmtFile::newInstance()
{
    return newInstance(FmtFile());
}

QScriptValue JsFmtFile::constructor()
{
    return ctor;
}

QScriptValue JsFmtFile::construct(QScriptContext *ctx, QScriptEngine *)
{
    JsFmtFile *cls = qscriptvalue_cast<JsFmtFile*>(ctx->callee().data());

    if (!cls)
        return QScriptValue();

    QScriptValue arg = ctx->argument(0);
    if (arg.instanceOf(ctx->callee()))
        return cls->newInstance(qscriptvalue_cast<FmtFile>(arg));

    return cls->newInstance();
}

QString JsFmtFile::name() const
{
    return QLatin1String("FmtFile");
}

QScriptValue JsFmtFile::prototype() const
{
    return proto;
}

QScriptValue JsFmtFile::toScriptValue(QScriptEngine *eng, const FmtFile &ba)
{
    QScriptValue ctor = eng->globalObject().property("JsFmtFile");
    JsFmtFile *cls = qscriptvalue_cast<JsFmtFile*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(ba));
    return cls->newInstance(ba);
}

void JsFmtFile::fromScriptValue(const QScriptValue &obj, FmtFile &ba)
{
    ba = qvariant_cast<FmtFile>(obj.data().toVariant());
}

QScriptValue::PropertyFlags JsFmtFile::propertyFlags(const QScriptValue &/*object*/, const QScriptString &name, uint /*id*/)
{
    return QScriptValue::Undeletable;
}

QScriptClass::QueryFlags JsFmtFile::queryProperty(const QScriptValue &object, const QScriptString &name, QScriptClass::QueryFlags flags, uint *id)
{
    FmtFile *file = qscriptvalue_cast<FmtFile*>(object.data());

    if (!file)
        return 0;
}
