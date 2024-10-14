#ifndef SYSLOGHELPER_H
#define SYSLOGHELPER_H

#ifdef SYS_LOG 

enum RsdSysLogClasses {
  e_slog_blob = 0,
  e_slog_sql_call_stack
};

#endif

#endif