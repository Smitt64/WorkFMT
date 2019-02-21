#include "jsfmttable.h"
#include <fmttable.h>
#include <fmtfield.h>
#include <fmtindex.h>
#include <fmtsegment.h>
#include <fmterrors.h>
#include <connectioninfo.h>
#include <QtScript>
#include <QFile>
#include <QUiLoader>
#include <QtWidgets>

QScriptValue myLoadUIFunction(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue calleeData = context->callee().data();
    QUiLoader *loader = qobject_cast<QUiLoader*>(calleeData.toQObject());

    QWidget *w = Q_NULLPTR;
    if (context->argumentCount() >= 1)
    {
        QFile f(context->argument(0).toString());
        if (f.open(QIODevice::ReadOnly))
        {
            w = loader->load(&f);
            f.close();
        }
    }

    QScriptValue ret = engine->undefinedValue();
    if (w)
        ret = engine->newQObject(w);

    return ret;
}

QScriptValue myAddToComboBoxFunction(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue ret = engine->undefinedValue();
    if (context->argumentCount() > 1)
    {
        QComboBox *box = qscriptvalue_cast<QComboBox*>(context->argument(0));

        if (!box)
            return context->throwError(QScriptContext::TypeError, "AddToComboBox: first param is not QComboBox object.");

        for (int i = 1; i < context->argumentCount(); ++i)
        {
            if (context->argument(i).isArray())
            {
                qint32 length = context->argument(i).property("length").toInt32();
                for(int j = 0; j < length; j++)
                    box->addItem(context->argument(i).property(j).toString());
            }
            else
                box->addItem(context->argument(i).toString());
        }
    }
    return ret;
}

void initializeJsFmtUILoader(QScriptEngine *engine, QUiLoader *loader)
{
    QScriptValue LoadUIfun = engine->newFunction(myLoadUIFunction);
    LoadUIfun.setData(engine->newQObject(loader));
    engine->globalObject().setProperty("LoadUiXml", LoadUIfun);

    QScriptValue AddToComboBoxfun = engine->newFunction(myAddToComboBoxFunction);
    engine->globalObject().setProperty("AddToComboBox", AddToComboBoxfun);
}

Q_DECLARE_METATYPE(QComboBox*)
