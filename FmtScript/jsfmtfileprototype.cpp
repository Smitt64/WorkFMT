#include "jsfmtfileprototype.h"
#include "fmtfile.h"
#include <QtScript/QScriptEngine>

Q_DECLARE_METATYPE(FmtFile*)

JsFmtFilePrototype::JsFmtFilePrototype(QObject *parent) : QObject(parent)
{

}

JsFmtFilePrototype::~JsFmtFilePrototype()
{

}

FmtFile *JsFmtFilePrototype::thisByteArray() const
{
    return qscriptvalue_cast<FmtFile*>(thisObject().data());
}
