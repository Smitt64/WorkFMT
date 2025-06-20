#ifndef TYPES_H
#define TYPES_H

#include "SqlConverterBuildParm.h"

//#include "SharedInterfaceAdapt.h"

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/locale.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/locale.hpp>
#include <boost/assign.hpp>

#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_comparison.hpp"
#include "boost/algorithm/cxx14/equal.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>

namespace pt = boost::property_tree;
using namespace boost::placeholders;

#include <string>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <list>

#define __PRETTY_FUNCTION__ __FUNCSIG__

//typedef std::basic_string<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char> > ustring;
typedef std::wstring ustring;

class ustring_adapt;
class ustring_icase : private ustring {
public:
  typedef ustring base_t;

  inline ustring_icase() : ustring() {}
  inline ustring_icase(const wchar_t * val) : ustring( boost::algorithm::to_upper_copy(ustring(val)) ) {  }
  inline ustring_icase(wchar_t val) : ustring( boost::algorithm::to_upper_copy(ustring(0, val)) ) {  }
  inline ustring_icase(const ustring & val) : ustring( boost::algorithm::to_upper_copy(val) ) {  }
  //inline ustring_icase(ustring & val) : ustring( boost::algorithm::to_upper_copy(val) ) {  }
  inline ustring_icase(const ustring_icase & val) : ustring( val.str() ) {  }
  inline ustring_icase(ustring_icase && val) : ustring( std::move(val) ) {  }
  using ustring::clear;
  using ustring::empty;
  using ustring::erase;
  using ustring::c_str;
  using ustring::size;
  using ustring::iterator;
  using ustring::const_iterator;
  using ustring::begin;
  using ustring::end;
  using ustring::cbegin;
  using ustring::cend;
  using ustring::assign;

  inline const ustring & str() const { return *this; }
  //explicit inline operator const ustring&() const { return str(); }

  ustring_icase & operator=(const ustring_adapt & adapter);
  ustring_icase & operator=(const ustring & adapter);
  ustring_icase & operator=(const wchar_t * msg);
  inline ustring_icase & operator=(ustring_icase && t) {
    ustring::operator=(std::move(t));
    return *this;
  }
  ustring_icase & operator=(const ustring_icase & t);

  //        || std::is_same<const ustring&, T>::value\
  std::is_same<const wchar_t *, T>::value\
    || std::is_same<wchar_t, T>::value\
  std::is_array<T>::value && std::is_same<const wchar_t, typename std::remove_extent<T>::type>::value\


/*
  template<class T>
  inline typename std::enable_if<std::is_same<const ustring_icase &, T>::value , bool>::type compare(T & r) {
    return this->str() == ustring_icase(r).str();  
  }*/


  #define USTRING_ICASE_CONSTR_TYPES \
     std::is_same<const ustring_adapt &, T>::value\
  || std::is_array<T>::value && std::is_same<const wchar_t, typename std::remove_extent<T>::type>::value\
  || std::is_same<const wchar_t *, T>::value\
  || std::is_same<wchar_t, T>::value\
  || std::is_same<const ustring_icase &, T>::value\
  || std::is_same<const ustring&, T>::value\

  template<class T>
  inline typename std::enable_if<USTRING_ICASE_CONSTR_TYPES , bool>::type operator==(T r) const {
    return this->str() == ustring_icase(r).str();  
  }

  template<class T>
  inline typename std::enable_if<USTRING_ICASE_CONSTR_TYPES, bool>::type  operator!=(T r) const {
    return this->str() != ustring_icase(r).str();  
  }
  
  inline bool operator==(const ustring_icase & r) const {
    return this->str() == ustring_icase(r).str();  
  }

  inline bool operator==(const ustring & r) const {
    return this->str() == ustring_icase(r).str();  
  }


  //template<class T>
  //inline friend typename std::enable_if<USTRING_ICASE_CONSTR_TYPES , bool>::type operator==(const ustring_icase & l, T & r) {
  //  return l.str() == ustring_icase(r).str();  
  //}

  //template<class T>
  //inline friend typename std::enable_if<USTRING_ICASE_CONSTR_TYPES, bool>::type  operator!=(const ustring_icase & l, T & r) {
  //  return l.str() != ustring_icase(r).str();  
  //}

  //inline bool operator==(const ustring_icase & r) {
  //  return this->str() == r.str();  
  //}

  //inline bool operator!=(const ustring_icase & r) {
  //  return this->str() != r.str();  
  //}
  

  template<class T>
  inline friend bool operator<(const ustring_icase & l, const T & r) {
    return l.str() < ustring_icase(r).str();
  }

  template<class T>
  inline friend ustring_icase & operator+=(ustring_icase & l, const T & r) {
    static_cast<ustring&>(l) += ustring_icase(r).str();  
    return l;
  }

  template<class T>
  inline friend ustring_icase & operator+=(ustring_icase && l, const T & r) {
    static_cast<ustring&>(l) += ustring_icase(r).str();  
    return l;
  }

  template<class T>
  inline friend ustring_icase operator+(const ustring_icase & l, const T & r) {
    return l.str() + ustring_icase(r).str();
  }

  inline friend std::wostream & operator<<(std::wostream & o, const ustring_icase & t) {
    o << t.str();
    return o;
  }
};

__SHARED_DIRECTION size_t getSqlConverterBuild();
__SHARED_DIRECTION char* getSqlConverterBuildStr();
__SHARED_DIRECTION size_t getSqlConverterMajorVer();
__SHARED_DIRECTION size_t getSqlConverterMinorVer();
__SHARED_DIRECTION size_t getSqlConverterPathVer();
__SHARED_DIRECTION char* getSqlConverterInfoVerStr();


namespace comparsions{

  bool operator<(const ustring & a, const ustring & b);

  inline bool nocase_less(const wchar_t& c1, const wchar_t& c2)
  {
        return tolower (c1) < tolower (c2); 
  }

  inline bool nocase_equal(const wchar_t& c1, const wchar_t& c2)
  {
        return tolower (c1) == tolower (c2); 
  }

  struct StrEqualNoCase{
    inline bool operator() (const ustring & s1, const ustring & s2) const 
    {
      return boost::algorithm::equal 
      (s1.begin (), s1.end (),   // source range
      s2.begin (), s2.end (),   // dest range
      boost::bind(nocase_equal, _1, _2) );  // comparison
    }
  };

  struct StrLessNoCase
  {
    inline bool operator() (const ustring & s1, const ustring & s2) const 
    {
      //return boost::locale::to_lower(s1, locUtf16_) < boost::locale::to_lower(s2, locUtf16_);
      return std::lexicographical_compare 
      (s1.begin (), s1.end (),   // source range
      s2.begin (), s2.end (),   // dest range
      boost::bind(nocase_less, _1, _2) );  // comparison
    }

    inline bool operator() (const boost::tuple<ustring, ustring> & s1, const boost::tuple<ustring, ustring> & s2) const
    {
      return StrLessNoCase()(s1.get<0>(), s2.get<0>()) 
        || StrEqualNoCase()(s1.get<0>(), s2.get<0>()) && StrLessNoCase()(s1.get<1>(), s2.get<1>());
    }

  };

  inline bool operator<(const ustring & a, const ustring & b)
  {
    return StrLessNoCase()(a, b);
  }

}

namespace conversions{

  template<class T>
  T to_lower(T val, const std::locale& loc = std::locale())
  {
    return boost::algorithm::detail::to_lowerF<T>(loc)(val);
  }

  template<class T>
  T to_upper(T val, const std::locale& loc = std::locale())
  {
    return boost::algorithm::detail::to_upperF<T>(loc)(val);
  }

}

#endif