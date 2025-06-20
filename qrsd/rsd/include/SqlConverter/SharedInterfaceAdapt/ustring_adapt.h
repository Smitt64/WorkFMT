#ifndef USTRING_ADAPT_H
#define USTRING_ADAPT_H

#include "SqlConverterBuildParm.h"

#include "dll_obj_adapt.hpp"
#include "string_members.h"

#ifdef SqlConverter_EXPORTS
  #include "Types.h"
  #include <boost/ref.hpp>
  #include <iostream> 
#endif


class ustring_iterator_adapt;

class __SHARED_DIRECTION ustring_iterator_adapt_intf {
public:
  typedef int difference_type;
  typedef wchar_t value_type;
  virtual ~ustring_iterator_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(ustring_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION ustring_iterator_adapt : public obj_adapt<ustring_iterator_adapt_intf>
  , public std::iterator<std::random_access_iterator_tag, ustring_iterator_adapt_intf::value_type> {
public:
#ifdef SqlConverter_EXPORTS
  typedef ustring::iterator impl_t;
  typedef ustring_iterator_adapt_intf::difference_type difference_type;
  ustring_iterator_adapt(impl_t && iter);
#endif
  typedef std::random_access_iterator_tag iterator_category;
  typedef ustring_iterator_adapt_intf::value_type value_type;
  typedef ustring_iterator_adapt_intf::difference_type difference_type;
  ustring_iterator_adapt();
  ustring_iterator_adapt(const ustring_iterator_adapt & t);
  GEN_DELEGATE_FMEMBER_LIST(ustring_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

//------------------------------------------------------------------------
class ustring_const_iterator_adapt;

class __SHARED_DIRECTION ustring_const_iterator_adapt_intf {
public:
  typedef int difference_type;
  typedef const wchar_t value_type;
  virtual ~ustring_const_iterator_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(ustring_const_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION ustring_const_iterator_adapt : public obj_adapt<ustring_const_iterator_adapt_intf>
  , public std::iterator<std::random_access_iterator_tag, ustring_const_iterator_adapt_intf::value_type> {
public:
#ifdef SqlConverter_EXPORTS
  typedef ustring::const_iterator impl_t;
  typedef ustring_const_iterator_adapt_intf::difference_type difference_type;
  ustring_const_iterator_adapt(impl_t && iter);
#endif
  typedef std::random_access_iterator_tag iterator_category;
  typedef ustring_const_iterator_adapt_intf::value_type value_type;
  typedef ustring_const_iterator_adapt_intf::difference_type difference_type;
  ustring_const_iterator_adapt();
  ustring_const_iterator_adapt(const ustring_const_iterator_adapt & t);
  GEN_DELEGATE_FMEMBER_LIST(ustring_const_iterator_adapt, string_iterator_adapt_FMEMBER_LIST)
};



//------------------------------------------------------------------------
class ustring_adapt;

class __SHARED_DIRECTION ustring_adapt_intf {
public:
  typedef wchar_t value_type;
  typedef ustring_iterator_adapt iterator;
  typedef ustring_const_iterator_adapt const_iterator;
  virtual ~ustring_adapt_intf(){ }
  virtual ustring_adapt_intf * clone(ustring_adapt & parent) const = 0;

  GEN_VIRTUAL_FMEMBER_LIST(ustring_adapt, string_adapt_FMEMBER_LIST)
  //virtual const wchar_t * c_str() const = 0;
};

class __SHARED_DIRECTION ustring_adapt : public obj_adapt<ustring_adapt_intf> {
public:
#ifdef SqlConverter_EXPORTS
  typedef ustring impl_t;
  ustring_adapt(impl_t && obj);
  ustring_adapt(boost::reference_wrapper<impl_t> obj);
  GEN_DELEGATE_UNWRAP(ustring_adapt)
#endif
  typedef ustring_adapt_intf::value_type value_type;
  typedef ustring_adapt_intf::iterator iterator;
  typedef ustring_adapt_intf::const_iterator const_iterator;
  ustring_adapt();
  ustring_adapt(ustring_adapt && t);
  ustring_adapt(const ustring_adapt & t);
  ustring_adapt(value_type const * msg);

  GEN_DELEGATE_FMEMBER_LIST(ustring_adapt, string_adapt_FMEMBER_LIST)
protected:
  ustring_adapt_intf * clone(ustring_adapt & parent) const;
};

#ifdef SqlConverter_EXPORTS
  std::wostream & operator<<( std::wostream & ostm, const ustring_adapt & adapter );
  bool operator!=( const ustring & left, const ustring_adapt & right);
#endif


#endif