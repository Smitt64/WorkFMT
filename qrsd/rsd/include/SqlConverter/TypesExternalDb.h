#ifndef TYPESEXTERNALDB_H
#define TYPESEXTERNALDB_H

#include "Types.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "SharedInterfaceAdapt.h"

// for universal DBMS - user realisation
class SqlStatementBase : private boost::noncopyable
{
  friend class SqlStatementBuilder;
public:
  typedef boost::shared_ptr<SqlStatementBase> sp_t;
  //typedef boost::tuple<A0, A1, A2, A3> tuple_t;

  virtual bool fetch() = 0;
  virtual string_adapt getError() = 0;
  virtual void throw_last_err(const string_adapt & prefix) = 0;
protected:
  virtual void bindSpec(ustring_adapt & val) = 0;
  virtual sp_t execute(const ustring_adapt & sql) = 0;
//  virtual sp_t sharedFromThis() = 0;
};

class SqlStatementBuilder {
  public:
    //typedef boost::shared_ptr<SqlStatementBuilder> sp_t;
    inline SqlStatementBuilder(SqlStatementBase::sp_t sttm) : sttm_(sttm) {}

    template<class T>
    SqlStatementBuilder & bind(T & val) {
      sttm_->bindSpec(val);
      return *this;
    }
    inline SqlStatementBase::sp_t execute(const ustring_adapt& sql) {sttm_->execute(sql); return sttm_; }
protected:
  SqlStatementBase::sp_t sttm_;
  };

class DbmsExecutorBase : private boost::noncopyable
{
public:
  typedef boost::shared_ptr<DbmsExecutorBase> sp_t;
  virtual SqlStatementBuilder buildStatement() = 0;

  //helper
  virtual const ustring_adapt & curSchema() = 0;
};


#endif