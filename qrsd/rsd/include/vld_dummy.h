#ifndef VLD_DUMMY_H
#define VLD_DUMMY_H

#include <boost/stacktrace.hpp>

inline void VLDDisable(){}
inline void VLDReportLeaks(){}
inline void VLDRefreshModules(){}
inline void VLDGetModulesList(WCHAR*, int){}
inline void VLDEnable(){}
inline std::string VLDGetCallstackNow(bool) {
  std::stringstream stm;
  stm << boost::stacktrace::stacktrace();
  return stm.str();
}

#endif