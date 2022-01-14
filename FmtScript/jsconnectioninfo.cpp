#include "jsconnectioninfo.h"
#include "jsfmtlib.h"
#include <QDialog>
#include "oracleauthdlg.h"
#include <connectioninfo.h>

typedef QSharedPointer<ConnectionInfo> JsConnectionInfo;
Q_DECLARE_METATYPE(JsConnectionInfo)

QScriptValue JsCreateConnection(QScriptContext *context, QScriptEngine *eng)
{
    QString username;
    QString passw;
    QString DSN;
    ConnectionInfo *pInfo = Q_NULLPTR;

    if (context->argumentCount() < 3)
        return context->throwError(QScriptContext::SyntaxError, "not enough parameters");
    else if (context->argumentCount() == 3)
    {
        if (!context->argument(0).isString())
            return context->throwError(QScriptContext::SyntaxError, "Invalid parameter type: 1");
        if (!context->argument(1).isString())
            return context->throwError(QScriptContext::SyntaxError, "Invalid parameter type: 2");
        if (!context->argument(2).isString())
            return context->throwError(QScriptContext::SyntaxError, "Invalid parameter type: 3");

        username = context->argument(0).toString();
        passw = context->argument(1).toString();
        DSN = context->argument(2).toString();

        //OracleAuthDlg::OraCreateConnection(username, passw, DSN, &pInfo, pTnsListModel);
    }

    if (pInfo == Q_NULLPTR)
        return context->throwError(QScriptContext::SyntaxError, "Could not create a connection");

    //JsConnectionInfo pointer(pInfo);
    return eng->toScriptValue(pInfo);
}


void initializeJsConnectionInfo(QScriptEngine *engine)
{
    /*QScriptValue object = engine->newObject();
    object.setProperty("CreateConnection", engine->newFunction(JsCreateConnection), QScriptValue::Undeletable | QScriptValue::ReadOnly);*/
    //engine->globalObject().setProperty("CreateConnection", engine->newFunction(JsCreateConnection));
            //.property(, );
    engine->globalObject().setProperty("CreateConnection", engine->newFunction(JsCreateConnection), QScriptValue::Undeletable | QScriptValue::ReadOnly);
}
