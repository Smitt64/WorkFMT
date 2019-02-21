#include "scriptextensionplugin.h"
#include "jsfmttable.h"
#include "jsconnectioninfo.h"
#include "jsfmtlib.h"
#include "jsfmtfile.h"
#include <fmtcore.h>
#include <QDebug>
#include <QUiLoader>

void initializeJsFmtUILoader(QScriptEngine *engine, QUiLoader *loader);

ScriptExtensionPlugin::ScriptExtensionPlugin(QObject *parent) :
    QScriptExtensionPlugin(parent),
    pUiLoader(Q_NULLPTR)
{
    FmtInit();
    pLib = new JsFmtLib(this);
}

ScriptExtensionPlugin::~ScriptExtensionPlugin()
{

}

void ScriptExtensionPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == "fmt")
    {
        QScriptValue objectValue = engine->newQObject(pLib);
        engine->globalObject().setProperty("FmtLib", objectValue);

        initializeJsFmtTable(engine);
        initializeJsConnectionInfo(engine);
        initializeJsFmtFile(engine);
    }

    if (key == "fmt.Ui")
    {
        if (pUiLoader == Q_NULLPTR)
            pUiLoader = new QUiLoader(this);
        initializeJsFmtUILoader(engine, pUiLoader);
    }
}

QStringList ScriptExtensionPlugin::keys() const
{
    return QStringList() << "fmt" << "fmt.FmtLib" << "fmt.Ui";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(FmtScript, ScriptExtensionPlugin)
#endif // QT_VERSION < 0x050000
