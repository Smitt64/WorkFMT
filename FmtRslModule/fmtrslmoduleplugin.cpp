#include "fmtrslmoduleplugin.h"
#include "fmtrslmodule.h"
#include "rsscript/registerobjlist.hpp"

QStringList FmtRslModulePlugin::staticModules() const
{
    return { "Fmt" };
}

void FmtRslModulePlugin::registerStaticModules()
{
    addStaticModule<FmtRslModule>(new FmtRslModule());
}
