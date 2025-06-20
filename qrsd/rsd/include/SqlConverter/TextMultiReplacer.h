#ifndef TEXTXMILTIREPLACER_H
#define TEXTXMILTIREPLACER_H

#include "Types.h"

#include "SharedInterfaceAdapt.h"

class SOraSpParam;

class __SHARED_DIRECTION TextMultiReplacer {
public:
  virtual void load(const string_adapt & fp_data, const string_adapt & sloc) = 0;
}; 

#endif