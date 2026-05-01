#ifndef EXPORTERFACTORY_H
#define EXPORTERFACTORY_H

#include <QScopedPointer>
#include "exporterbase.h"
#include "export/oraexporter.h"
#include "export/pgexporter.h"
#include <connectioninfo.h>

class ExporterFactory
{
public:
    static ExporterBase* createExporter(ConnectionInfo *conn, QObject *parent = nullptr)
    {
        if (!conn) return nullptr;

        switch (conn->type()) {
            case ConnectionInfo::CON_ORA:
                return new OracleExporter(parent);

            case ConnectionInfo::CON_POSTGRESQL:
                return new PostgresExporter(parent);

            default:
                qWarning() << "Unsupported database type:" << conn->type();
                return nullptr;
        }
    }
};

#endif // EXPORTERFACTORY_H
