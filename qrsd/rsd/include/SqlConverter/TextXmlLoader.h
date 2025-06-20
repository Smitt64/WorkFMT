#ifndef TEXTXMLLOADER_H
#define TEXTXMLLOADER_H

#include "Types.h"

#include "SharedInterfaceAdapt.h"

class __SHARED_DIRECTION TextXmlLoaderSet {
public:
  virtual void load(const string_adapt & fp_data, const string_adapt & root_tag) = 0;
}; 

#endif