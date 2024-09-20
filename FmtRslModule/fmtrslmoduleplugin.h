#ifndef FMTRSLMODULEPLUGIN_H
#define FMTRSLMODULEPLUGIN_H

#include <QObject>
#include <rsscript/RslModulePluginInterface.h>

class FmtRslModulePlugin : public QObject,
        public RslStaticModuleInterface
{
    Q_OBJECT
public:
    Q_PLUGIN_METADATA(IID "org.toolsruntime.rsl.modules.RslStaticModuleInterface/1.0" FILE "RslToolsRuntimeModule.json")
    Q_INTERFACES(RslStaticModuleInterface)

public:
    virtual QStringList staticModules() const Q_DECL_OVERRIDE;
    virtual void registerStaticModules() Q_DECL_OVERRIDE;
};

#endif // FMTRSLMODULEPLUGIN_H
