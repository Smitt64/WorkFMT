#ifndef VECTOR_USTRING_ICASE_ADAPT_H
#define VECTOR_USTRING_ICASE_ADAPT_H

#include "SqlConverterBuildParm.h"

#include "dll_obj_adapt.hpp"
#include "vector_members.h"
#include "ustring_icase_adapt.h"

#ifdef SqlConverter_EXPORTS
#include <vector>
#include <boost/optional.hpp>
#endif


class vector_ustring_icase_iterator_adapt;

class __SHARED_DIRECTION vector_ustring_icase_iterator_adapt_intf {
public:
  typedef int difference_type;
  typedef ustring_icase_adapt value_type;
  virtual ~vector_ustring_icase_iterator_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(vector_ustring_icase_iterator_adapt, vector_ustring_iterator_adapt_FMEMBER_LIST)
};

class __SHARED_DIRECTION vector_ustring_icase_iterator_adapt : public obj_adapt<vector_ustring_icase_iterator_adapt_intf>, public std::iterator<std::random_access_iterator_tag, vector_ustring_icase_iterator_adapt_intf::value_type> {
public:
#ifdef SqlConverter_EXPORTS
  typedef std::vector<ustring_icase>::iterator impl_t;
  typedef std::vector<boost::optional<ustring_icase_adapt>>::iterator impl_adapt_t;
  typedef vector_ustring_icase_iterator_adapt_intf::difference_type difference_type;
  vector_ustring_icase_iterator_adapt(impl_t && iter, impl_adapt_t && iter_adapt);
#endif
  typedef std::random_access_iterator_tag iterator_category;
  typedef vector_ustring_icase_iterator_adapt_intf::value_type value_type;
  typedef vector_ustring_icase_iterator_adapt_intf::difference_type difference_type;
  vector_ustring_icase_iterator_adapt();
  vector_ustring_icase_iterator_adapt(const vector_ustring_icase_iterator_adapt & t);
  GEN_DELEGATE_FMEMBER_LIST(vector_ustring_icase_iterator_adapt, vector_ustring_iterator_adapt_FMEMBER_LIST)
};



//------------------------------------------------------------------------
class vector_ustring_icase_adapt;

class __SHARED_DIRECTION vector_ustring_icase_adapt_intf {
public:
  typedef ustring_icase_adapt value_type;
  typedef size_t size_type;
  typedef vector_ustring_icase_iterator_adapt iterator;
  virtual ~vector_ustring_icase_adapt_intf() {}
  GEN_VIRTUAL_FMEMBER_LIST(vector_ustring_icase_adapt, vector_ustring_adapt_FMEMBER_LIST)
  //virtual const wchar_t * c_str() const = 0;
};

class __SHARED_DIRECTION vector_ustring_icase_adapt : public obj_adapt<vector_ustring_icase_adapt_intf> {
public:
#ifdef SqlConverter_EXPORTS
  typedef std::vector<ustring_icase> impl_t;
  typedef vector_ustring_icase_adapt_intf::size_type size_type;
  vector_ustring_icase_adapt(impl_t && obj);
  GEN_DELEGATE_UNWRAP(vector_ustring_adapt)
#endif
  typedef vector_ustring_icase_adapt_intf::value_type value_type;
  typedef vector_ustring_icase_iterator_adapt iterator;
  typedef iterator const_iterator;
  typedef vector_ustring_icase_adapt_intf::size_type size_type;
  typedef const value_type& const_reference;
  vector_ustring_icase_adapt();
  vector_ustring_icase_adapt(vector_ustring_icase_adapt && t);
  vector_ustring_icase_adapt(const vector_ustring_icase_adapt & t);
  vector_ustring_icase_adapt(size_type n);
  GEN_DELEGATE_FMEMBER_LIST(vector_ustring_icase_adapt, vector_ustring_adapt_FMEMBER_LIST)
};



#endif