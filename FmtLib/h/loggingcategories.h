#ifndef LOGGINGCATEGORIES_H
#define LOGGINGCATEGORIES_H

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logEmpty)
Q_DECLARE_LOGGING_CATEGORY(logCore)
Q_DECLARE_LOGGING_CATEGORY(logFmt)
Q_DECLARE_LOGGING_CATEGORY(logDbgHelp)

QStringList LoggingCategoryList();
const QLoggingCategory &LoggingCategory(const QString &name);

#endif // LOGGINGCATEGORIES_H
