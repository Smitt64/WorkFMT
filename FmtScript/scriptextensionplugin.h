#ifndef SCRIPTEXTENSIONPLUGIN_H
#define SCRIPTEXTENSIONPLUGIN_H

#include <QScriptExtensionPlugin>

class JsFmtLib;
class QUiLoader;
class ScriptExtensionPlugin : public QScriptExtensionPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QScriptExtensionInterface" FILE "FmtScript.json")
#endif // QT_VERSION >= 0x050000

public:
    ScriptExtensionPlugin(QObject *parent = Q_NULLPTR);
    virtual ~ScriptExtensionPlugin();
    virtual void initialize(const QString &key, QScriptEngine *engine);
    virtual QStringList keys() const;

private:
    JsFmtLib *pLib;
    QUiLoader *pUiLoader;
};

#endif // SCRIPTEXTENSIONPLUGIN_H
