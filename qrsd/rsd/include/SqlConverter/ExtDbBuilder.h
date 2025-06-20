#ifndef EXTDBBUILDER_H
#define EXTDBBUILDER_H

#include "TypesExternalDb.h"
#include <boost/move/unique_ptr.hpp>

enum ECreateViewGttChecker {
  e_get_gtt_from_source = 0,
  e_get_gtt_from_dbms
};

class ExtDbBuilder {
public:
  typedef boost::movelib::unique_ptr<ExtDbBuilder> up_t;
  virtual ~ExtDbBuilder() {};

  virtual ExtDbBuilder* executor(DbmsExecutorBase::sp_t dbms_executor) = 0;
  virtual ExtDbBuilder* viewGttChecker(ECreateViewGttChecker checker) = 0;
  virtual void build() = 0;
};

#endif