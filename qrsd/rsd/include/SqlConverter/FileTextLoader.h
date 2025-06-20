#ifndef FILETEXTLOADER_H
#define FILETEXTLOADER_H

#include "Types.h"

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/locale.hpp>

#include "SharedInterfaceAdapt.h"

class __SHARED_DIRECTION FileTextLoader
{
public:
  typedef boost::shared_ptr<FileTextLoader> sp_t;
  static sp_t make();

  virtual ~FileTextLoader() {}
  virtual bool open(const string_adapt & path, const string_adapt & loc) = 0;
  virtual void setText(const ustring_adapt & text) = 0;
  virtual ustring_adapt& text() = 0;
};

#endif