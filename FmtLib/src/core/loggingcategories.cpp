#include "loggingcategories.h"

Q_LOGGING_CATEGORY(logEmpty,  "Empty")
Q_LOGGING_CATEGORY(logCore,   "Core")
Q_LOGGING_CATEGORY(logSql,    "Sql")
Q_LOGGING_CATEGORY(logFmt,    "Fmt")
Q_LOGGING_CATEGORY(logDbgHelp,"DbgHelp")

QStringList LoggingCategoryList()
{
    return QStringList()
            << "Core"
            << "Sql"
            << "Fmt"
            << "DbgHelp";
}

const QLoggingCategory &LoggingCategory(const QString &name)
{
    if (name == "Core")
        return logCore();
    if (name == "Sql")
        return logSql();
    if (name == "Fmt")
        return logFmt();
    if (name == "DbgHelp")
        return logDbgHelp();

    return logEmpty();
}
