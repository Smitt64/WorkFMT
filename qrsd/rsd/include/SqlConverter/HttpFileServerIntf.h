#ifndef HTTPFILESERVERINTF_H
#define HTTPFILESERVERINTF_H

#include <string>

class HttpFileServerIntf {
public:
  virtual void mountDirectory(const char *path) = 0;
  virtual const char* getUrlRootPath() = 0;
  virtual void runAsync() = 0;
  virtual void stop() = 0;
  virtual const char* getLog() = 0;
};

#endif