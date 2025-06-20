#ifndef STRING_ADAPT_H
#define STRING_ADAPT_H

#include "SqlConverterBuildParm.h"

#include "dll_obj_adapt.hpp"
#include "string_members.h"

#ifdef SqlConverter_EXPORTS
  #include <string>
  #include <boost/ref.hpp>
  #include <iostream> 
#endif


class string_iterator_adapt;

class __SHARED_DIRECTION string_iterator_adapt_intf {
public:
  typedef char value_type;
  typedef int difference_type;
  virtual ~string_iterator_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(string_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION string_iterator_adapt : public obj_adapt<string_iterator_adapt_intf>
  , public std::iterator<std::random_access_iterator_tag, string_iterator_adapt_intf::value_type> {
public:
#ifdef SqlConverter_EXPORTS
  typedef std::string::iterator impl_t;
  typedef string_iterator_adapt_intf::difference_type difference_type;
  string_iterator_adapt(std::string::iterator && iter);
#endif
  typedef std::random_access_iterator_tag iterator_category;
  typedef string_iterator_adapt_intf::value_type value_type;
  typedef string_iterator_adapt_intf::difference_type difference_type;
  string_iterator_adapt();
  string_iterator_adapt(const string_iterator_adapt & t);
  GEN_DELEGATE_FMEMBER_LIST(string_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

//-----------------------------------------------------------------------
class string_const_iterator_adapt;

class __SHARED_DIRECTION string_const_iterator_adapt_intf {
public:
  typedef const char value_type;
  typedef int difference_type;
  virtual ~string_const_iterator_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(string_const_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION string_const_iterator_adapt : public obj_adapt<string_const_iterator_adapt_intf>
  , public std::iterator<std::random_access_iterator_tag, string_const_iterator_adapt_intf::value_type> {
public:
#ifdef SqlConverter_EXPORTS
  typedef std::string::const_iterator impl_t;
  typedef string_const_iterator_adapt_intf::difference_type difference_type;
  string_const_iterator_adapt(std::string::const_iterator && iter);
#endif
  typedef std::random_access_iterator_tag iterator_category;
  typedef string_const_iterator_adapt_intf::value_type value_type;
  typedef string_const_iterator_adapt_intf::difference_type difference_type;
  string_const_iterator_adapt();
  string_const_iterator_adapt(const string_const_iterator_adapt & t);
  GEN_DELEGATE_FMEMBER_LIST(string_const_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};


//------------------------------------------------------------------------
class string_adapt;

class __SHARED_DIRECTION string_adapt_intf {
public:
  typedef char value_type;
  typedef string_iterator_adapt iterator;
  typedef string_const_iterator_adapt const_iterator;
  virtual ~string_adapt_intf(){ }
  virtual string_adapt_intf * clone(string_adapt & parent) const = 0;
  GEN_VIRTUAL_FMEMBER_LIST(string_adapt, string_adapt_FMEMBER_LIST)
  //virtual const wchar_t * c_str() const = 0;
};

class __SHARED_DIRECTION string_adapt : public obj_adapt<string_adapt_intf> {
public:
#ifdef SqlConverter_EXPORTS
  typedef std::string impl_t;
  string_adapt(impl_t && obj);
  string_adapt(boost::reference_wrapper<impl_t> obj);
  GEN_DELEGATE_UNWRAP(string_adapt)
#endif
  typedef string_adapt_intf::value_type value_type;
  typedef string_adapt_intf::iterator iterator;
  typedef string_adapt_intf::const_iterator const_iterator;
  string_adapt();
  string_adapt(string_adapt && t);
  string_adapt(const string_adapt & t);
  string_adapt(value_type const * msg);

  GEN_DELEGATE_FMEMBER_LIST(string_adapt, string_adapt_FMEMBER_LIST)
protected:
  string_adapt_intf * clone(string_adapt & parent) const;
};

#ifdef SqlConverter_EXPORTS
  std::ostream & operator<<(std::ostream & ostm, const string_adapt & adapter);
#endif

#endif