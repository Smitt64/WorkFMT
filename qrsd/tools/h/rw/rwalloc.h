#ifndef __RW_OSALLOC_H__
#define __RW_OSALLOC_H__ 1

/*
 * rwalloc : declaration for Objectstore merge 
 *
 * $Id: rwalloc.h@#/main/7  03/18/97 15:07:00  griswolf (TOOLS707_WIN32_971021)
 *
 * (c) Copyright 1989-1997 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 *******************************************************************
 *
 * $Log: rwalloc.h,v $
 * Revision 7.7  1996/10/19 23:58:16  kevinj
 * Bug #5100:  Finish the job.
 *
 * Revision 7.6  1996/10/18 21:59:40  griswolf
 * Scopus #5100: clean up RW_USE_OBJECTSTORE stuff.
 *
 * Revision 7.5  1996/10/08 23:46:26  kyle
 * ObjectStore update
 *
 * Revision 7.4  1996/09/09 21:05:57  griswolf
 * Quite warning about extraneous semicolon in generic classes.
 *
 * Revision 7.3  1996/08/28 19:27:48  hart
 * ODI change
 *
 * Revision 7.2  1996/07/20 03:20:37  hart
 * add RWExport to RWAlloc class
 *
 * Revision 7.1  1996/07/18 04:23:34  hart
 * ObjectStore changes
 *
 *
 */ 

#ifndef __RWDEFS_H__
#  include "rw/defs.h"
#endif /*  __RWDEFS_H__ */

/***
 *** standard functions needed for both compile time and link time;
 *** and for Tools.h++ both with and without ObjectStore. Scoped with class.
 ***/
class RWExport RWAlloc {
public:
  static void *of(const void *);
  static void *typespec_char();
  static void *typespec_uchar();
  static void *typespec_ushort();
  static void *typespec_pointer();
  static void *typespec_uint();
  static void *typespec_RWTimeFmtList();
  static int  is_same_allocator(const void *p1, const void *p2);
  static int  is_transient_allocation(const void *p1);
};

/***
 *** Ensure sanity
 ***/
#ifdef RW_USE_OBJECTSTORE_PSE
#  ifndef RW_USE_OBJECTSTORE
#  define RW_USE_OBJECTSTORE 1
#  endif
#endif

/***
 ***  get appropriate "general purpose" ObjectStore headers here.
 ***/
#if defined(RW_USE_OBJECTSTORE)
#  if defined(RW_USE_OBJECTSTORE_PSE)
#    include <os_pse/ostore.hh>
     typedef os_database os_segment;
#  else /* not _PSE */
/* os_typespec class is needed for Generic class instantiations */
#  include <ostore/portypes.hh>
#  include <ostore/except.hh>
#  include <ostore/client/client.hh>
#  endif /* _PSE or not */
#endif /*  RW_USE_OBJECTSTORE */


/***
 *** Get macros etc to ensure access from heterogenious OSs works
 ***/
#ifdef RW_OBJECTSTORE_HETERO
#include <ostore/os_types/virtfunc.hh> /* padding macros */
/* 
 * class os_virtual_behavior forces vtable ptr to be in a 
 * known location. Must always be the _first_ base class.
 */
#  define RWBehavior   : public os_virtual_behavior
#  define RWBehavior2    public os_virtual_behavior,

/* forces enums to be 4 bytes long */
#  define RWOS_HETERO_ENUM_PAD  ,rw_os_hetero_enum_pad=1<<30 

/* fix the lengths of integer types */
#  define RW4Byte             unsigned int
#  define RW4ByteSigned       int

#else /* not RW_OBJECTSTORE_HETERO */
#  define RWBehavior
#  define RWBehavior2
#  define RWOS_HETERO_ENUM_PAD 
#  define RW4Byte              unsigned long
#  define RW4ByteSigned        long
#endif /* RW_OBJECTSTORE_HETERO */

/***
 *** define the TYPESPEC macros
 ***/ 
#if defined(RW_USE_OBJECTSTORE)
/*
 * Typespec macro. This macro is inserted into all non-templated
 * classes that can be stored persistently in an ObjectStore database.
 */
#  define RW_TYPESPEC public: \
     static os_typespec *get_os_typespec();
/*
 * Provide a template-class variant, in case we want it in another
 * version or release.
 */
#  define RW_T_TYPESPEC RW_TYPESPEC
#else
#  define RW_TYPESPEC
#  define RW_T_TYPESPEC
#endif /* RW_USE_OBJECTSTORE */


/***
 *** Memory allocation details
 ***/
#if defined(RW_USE_OBJECTSTORE)
/* Use a dummy class to distinguish our new from the global operator new */
extern RWExport RWAlloc __rwa;

/* this is the meat of the whole operation */
RWExport void *operator new (size_t, void *, void *, RWAlloc&);
RWExport void *operator new RW_BRACKETS (size_t, void *, void *, int, RWAlloc&);
#endif /* RW_USE_OBJECTSTORE */

#if defined(COMPILER_SUPPORTS_VECTOR_NEW)
#define RW_BRACKETS []
#else
#define RW_BRACKETS
#endif

/***
 *** Template memory allocation using RW_NEW_TSPEC, RW_NEW_TSPEC_VEC
 ***    also rw_os_typespec template class (struct)
 ***/
#ifndef RW_NO_TEMPLATES
#ifdef RW_USE_OBJECTSTORE
/* allocator macros for templates.  The macros RW_NEW_TSPEC and
   RW_NEW_TSPEC_VEC are used in template code that does any memory
   allocation.  The template code must be compiled (not linked) into
   the application.  When the CPP macro RW_USE_OBJECTSTORE is set
   the allocation will done using the proper os_typespec for the
   class being allocated. */

/* The following class "specializations" are used by the RW_NEW_TSPEC
   macros so that get_os_typespec() calls for templated classes on basic
   types use the ObjectStore built-in-for-basic-types get_os_typespec()
   macros.  This trick is necessary because ObjectStore's schema
   generator only defines get_os_typespec() functions for non-builtin
   types. */

template <class T> struct rw_os_typespec;

/* specializations for rw_os_typespec::get(); */
struct rw_os_typespec<int> {
  inline static os_typespec *get() { return os_typespec::get_int();}
};
struct rw_os_typespec<char> {
  inline static os_typespec *get() { return os_typespec::get_char();}
};
struct rw_os_typespec<long> {
  inline static os_typespec *get() { return os_typespec::get_long();}
};
struct rw_os_typespec<short> {
  inline static os_typespec *get() { return os_typespec::get_short();}
};
struct rw_os_typespec<unsigned int> {
  inline static os_typespec *get() { return os_typespec::get_unsigned_int();}
};
struct rw_os_typespec<unsigned char> {
  inline static os_typespec *get() { return os_typespec::get_unsigned_char();}
};
struct rw_os_typespec<unsigned long> {
  inline static os_typespec *get() { return os_typespec::get_unsigned_long();}
};
struct rw_os_typespec<unsigned short> {
  inline static os_typespec *get() { return os_typespec::get_unsigned_short();}
};
struct rw_os_typespec<float> {
  inline static os_typespec *get() { return os_typespec::get_float();}
};
struct rw_os_typespec<double> {
  inline static os_typespec *get() { return os_typespec::get_double();}
};

template <class T> struct rw_os_typespec
{
public:
  inline static os_typespec *get() { return T::get_os_typespec(); }
};

/* finally, the macros themselves */
#  define RW_NEW_TSPEC(WHERE,CLASSNAME) \
     ::new(RWAlloc::of(WHERE), rw_os_typespec<CLASSNAME>::get(), __rwa)
#  define RW_NEW_TSPEC_VEC(WHERE,CLASSNAME,N) \
     ::new RW_BRACKETS (RWAlloc::of(WHERE), rw_os_typespec<CLASSNAME>::get(), (N), __rwa)

#else  /* RW_USE_OBJECTSTORE */

#  define RW_NEW_TSPEC(WHERE,CLASSNAME) new
#  define RW_NEW_TSPEC_VEC(WHERE,CLASSNAME,N) new
#endif /* RW_USE_OBJECTSTORE */
#endif /* RW_NO_TEMPLATES */

/***
 *** Generics memory allocation using RW_NEW_GENERIC, RW_NEW_GENERIC_VEC
 *** and a macro, of course, to help with generic typspec naming.
 ***/

#if defined(RW_USE_OBJECTSTORE)

#  define RW_GENERIC_TYPESPEC_DECLARE_NAME(TYPE) \
     extern os_typespec * RWNAME2(TYPE,_rwgeneric_typespec)();
#  define RW_NEW_GENERIC(WHERE,TYPE) \
     ::new(RWAlloc::of(WHERE),RWNAME2(TYPE,_rwgeneric_typespec)(),__rwa)
#  define RW_NEW_GENERIC_VEC(WHERE,TYPE,N) \
     ::new RW_BRACKETS (RWAlloc::of(WHERE),RWNAME2(TYPE,_rwgeneric_typespec)(),(N), __rwa)

#else /* RW_USE_OBJECTSTORE */

#  define RW_GENERIC_TYPESPEC_DECLARE_NAME(TYPE)
#  define RW_NEW_GENERIC(WHERE,TYPE) new
#  define RW_NEW_GENERIC_VEC(WHERE,TYPE,N) new

#endif /* RW_USE_OBJECTSTORE */

/***
 *** "Regular" memory allocation using RW_NEW and RW_NEW_VEC
 ***/
#if defined(RW_USE_OBJECTSTORE)
#  define RW_NEW(WHERE,TSPEC) \
     ::new(RWAlloc::of(WHERE),TSPEC,__rwa)
#  define RW_NEW_VEC(WHERE,TSPEC,N) \
     ::new RW_BRACKETS (RWAlloc::of(WHERE),TSPEC,(N),__rwa)

#else
#  define RW_NEW(WHERE,TSPEC) new
#  define RW_NEW_VEC(WHERE,TSPEC,N) new
#endif /* RW_USE_OBJECTSTORE */

#endif /* #ifndef __RW_OSALLOC_H__ */

