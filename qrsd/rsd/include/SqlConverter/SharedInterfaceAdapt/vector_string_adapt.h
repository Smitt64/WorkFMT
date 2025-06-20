#ifndef vector_string_ADAPT_H
#define vector_string_ADAPT_H

#include "SqlConverterBuildParm.h"

#include "dll_obj_adapt.hpp"
#include "vector_members.h"
#include "string_adapt.h"

#include <string>
#include <iostream>

#ifdef SqlConverter_EXPORTS
#include <vector>
#include <boost/optional.hpp>
//typedef std::wstring ustring;
#endif


class vector_string_iterator_adapt;

class __SHARED_DIRECTION vector_string_iterator_adapt_intf {
public:
  typedef int difference_type;
  typedef string_adapt value_type;
  virtual ~vector_string_iterator_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(vector_string_iterator_adapt, vector_ustring_iterator_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION vector_string_iterator_adapt : public obj_adapt<vector_string_iterator_adapt_intf>, public std::iterator<std::random_access_iterator_tag, vector_string_iterator_adapt_intf::value_type> {
public:
#ifdef SqlConverter_EXPORTS
  typedef std::vector<std::string>::iterator impl_t;
  typedef std::vector<boost::optional<string_adapt>>::iterator impl_adapt_t;
  typedef vector_string_iterator_adapt_intf::difference_type difference_type;
  vector_string_iterator_adapt(impl_t && iter, impl_adapt_t && iter_adapt);
#endif
  typedef std::random_access_iterator_tag iterator_category;
  typedef vector_string_iterator_adapt_intf::value_type value_type;
  typedef vector_string_iterator_adapt_intf::difference_type difference_type;
  vector_string_iterator_adapt();
  vector_string_iterator_adapt(const vector_string_iterator_adapt & t);
  GEN_DELEGATE_FMEMBER_LIST(vector_string_iterator_adapt, vector_ustring_iterator_adapt_FMEMBER_LIST)
};



//------------------------------------------------------------------------
class vector_string_adapt;

class __SHARED_DIRECTION vector_string_adapt_intf {
public:
  typedef string_adapt value_type;
  typedef size_t size_type;
  typedef vector_string_iterator_adapt iterator;
  virtual ~vector_string_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(vector_string_adapt, vector_ustring_adapt_FMEMBER_LIST)
  //virtual const wchar_t * c_str() const = 0;
};

class __SHARED_DIRECTION vector_string_adapt : public obj_adapt<vector_string_adapt_intf> {
public:
#ifdef SqlConverter_EXPORTS
  typedef std::vector<std::string> impl_t;
  vector_string_adapt(impl_t && obj);
  GEN_DELEGATE_UNWRAP(vector_string_adapt)
#endif
  typedef vector_string_adapt_intf::value_type value_type;
  typedef vector_string_iterator_adapt iterator;
  typedef iterator const_iterator;
  typedef vector_string_adapt_intf::size_type size_type;
  typedef const value_type& const_reference;
  vector_string_adapt();
  vector_string_adapt(vector_string_adapt && t);
  vector_string_adapt(const vector_string_adapt & t);
  vector_string_adapt(size_type n);
  GEN_DELEGATE_FMEMBER_LIST(vector_string_adapt, vector_ustring_adapt_FMEMBER_LIST)
};



#endif