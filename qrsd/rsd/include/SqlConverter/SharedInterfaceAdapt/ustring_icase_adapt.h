#ifndef USTRING_ICASE_ADAPT_H
#define USTRING_ICASE_ADAPT_H

#include "SqlConverterBuildParm.h"

#include "dll_obj_adapt.hpp"
#include "string_members.h"
#include "ustring_adapt.h"

#ifdef SqlConverter_EXPORTS
  #include "Types.h"
  #include <boost/ref.hpp>
  #include <iostream> 
#endif

class ustring_icase_iterator_adapt;

class __SHARED_DIRECTION ustring_icase_iterator_adapt_intf {
public:
  typedef int difference_type;
  typedef wchar_t value_type;
  virtual ~ustring_icase_iterator_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(ustring_icase_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION ustring_icase_iterator_adapt : public obj_adapt<ustring_icase_iterator_adapt_intf>
  , public std::iterator<std::random_access_iterator_tag, ustring_icase_iterator_adapt_intf::value_type> {
public:
#ifdef SqlConverter_EXPORTS
  typedef ustring_icase::iterator impl_t;
  typedef ustring_icase_iterator_adapt_intf::difference_type difference_type;
  ustring_icase_iterator_adapt(impl_t && iter);
#endif
  typedef std::random_access_iterator_tag iterator_category;
  typedef ustring_icase_iterator_adapt_intf::value_type value_type;
  typedef ustring_icase_iterator_adapt_intf::difference_type difference_type;
  ustring_icase_iterator_adapt();
  ustring_icase_iterator_adapt(const ustring_icase_iterator_adapt & t);
  GEN_DELEGATE_FMEMBER_LIST(ustring_icase_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};


//------------------------------------------------------------------------
class ustring_icase_const_iterator_adapt;

class __SHARED_DIRECTION ustring_icase_const_iterator_adapt_intf {
public:
  typedef int difference_type;
  typedef const wchar_t value_type;
  virtual ~ustring_icase_const_iterator_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(ustring_icase_const_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION ustring_icase_const_iterator_adapt : public obj_adapt<ustring_icase_const_iterator_adapt_intf>
  , public std::iterator<std::random_access_iterator_tag, ustring_icase_const_iterator_adapt_intf::value_type> {
public:
#ifdef SqlConverter_EXPORTS
  typedef ustring_icase::const_iterator impl_t;
  typedef ustring_icase_const_iterator_adapt_intf::difference_type difference_type;
  ustring_icase_const_iterator_adapt(impl_t && iter);
#endif
  typedef std::random_access_iterator_tag iterator_category;
  typedef ustring_icase_const_iterator_adapt_intf::value_type value_type;
  typedef ustring_icase_const_iterator_adapt_intf::difference_type difference_type;
  ustring_icase_const_iterator_adapt();
  ustring_icase_const_iterator_adapt(const ustring_icase_const_iterator_adapt & t);
  GEN_DELEGATE_FMEMBER_LIST(ustring_icase_const_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

//------------------------------------------------------------------------
class ustring_icase_adapt;

class __SHARED_DIRECTION ustring_icase_adapt_intf {
public:
  typedef wchar_t value_type;
  typedef ustring_adapt base_t;
  typedef ustring_icase_iterator_adapt iterator;
  typedef ustring_icase_const_iterator_adapt const_iterator;
  virtual ~ustring_icase_adapt_intf() {}
  virtual ustring_icase_adapt_intf * clone(ustring_icase_adapt & parent) const = 0;
  
  GEN_VIRTUAL_FMEMBER_LIST(ustring_icase_adapt, string_adapt_FMEMBER_LIST)
  GEN_VIRTUAL_FMEMBER_LIST(ustring_icase_adapt, string_icase_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION ustring_icase_adapt : public obj_adapt<ustring_icase_adapt_intf> {
public:
#ifdef SqlConverter_EXPORTS
  typedef ustring_icase impl_t;
  ustring_icase_adapt(impl_t && obj);
  ustring_icase_adapt(boost::reference_wrapper<impl_t> obj);
  GEN_DELEGATE_UNWRAP(ustring_icase_adapt)
#endif
  typedef ustring_icase_adapt_intf::value_type value_type;
  typedef ustring_icase_adapt_intf::iterator iterator;
  typedef ustring_icase_adapt_intf::const_iterator const_iterator;
  ustring_icase_adapt();
  ustring_icase_adapt(ustring_icase_adapt && t);
  ustring_icase_adapt(const ustring_icase_adapt & t);
  ustring_icase_adapt(value_type const * msg);

  GEN_DELEGATE_FMEMBER_LIST(ustring_icase_adapt, string_adapt_FMEMBER_LIST)
  GEN_DELEGATE_FMEMBER_LIST(ustring_icase_adapt, string_icase_adapt_FMEMBER_LIST)
protected:
  ustring_icase_adapt_intf * clone(ustring_icase_adapt & parent) const;
};

#ifdef SqlConverter_EXPORTS
  std::wostream & operator<<( std::wostream & ostm, const ustring_icase_adapt & adapter );
#endif

#endif