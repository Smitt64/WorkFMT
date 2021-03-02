#ifndef __RW_DCMLDEFS_H__
#define __RW_DCMLDEFS_H__

/***************************************************************************
 *
 * $Id: dcmldefs.h@#/main/spm_bldmdl/3  11/11/97 11:37:21  bass (MNY150_971113_WIN)
 *
 * (c) Copyright 1993, 1996-1997 Rogue Wave Software, Inc.
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
 **************************************************************************/

//#define RWDCML 0x0150      /* Version number */
#define RWDCML 0x0249      /* Version number */

#  if !defined(__RWDEFS_H__)
#     include "rw/defs.h"
#  endif

#include "limits.h"    // CHAR_BIT
#include "rw/rstream.h"

/* defining single variable to simplify later testing */ 
#if (defined(_AIX) && (defined(__IBMCPP__) && (__IBMCPP__ >= 500)))
#  ifndef RW_NO_LIBC_IN_STD
#    define RW_NO_LIBC_IN_STD 1
#  endif
#  define RW_MNY_AIX5XX 1
#endif
#if (defined(__sgi) && (defined(_COMPILER_VERSION) && (_COMPILER_VERSION >= 700)))
#  define RW_MNY_SGI72X 1
#  define RW_DCML_NO_FRIEND_TEMPLATE_FUNC 1
#endif
#if (defined(__SUNPRO_CC) &&(__SUNPRO_CC>0x420))
#  define RW_MNY_SPR42X 1
#endif
#if ( defined(__TURBOC__) && ( __TURBOC__ >= 0x540 )) 
#  define RW_MNY_TBC54X 1
#endif
#if (defined(__SUNPRO_CC) && (RWSTDLIBTYPE != 0) && (__SUNPRO_CC >= 0x500))
#  define RW_DCML_MATH_FUNCTIONS_IN_STD  1
#endif
#if (defined(__SunOS_5_6) && (defined(__SUNPRO_CC) && (__SUNPRO_CC>=0x520)))
#  define RW_MNY_SOL26_SPR52X 1
#  define RW_DCML_NO_FRIEND_TEMPLATE_FUNC 1
#endif
#if defined(__SUNPRO_CC) && (__SUNPRO_CC>=0x540)
#  define RW_DCML_NO_FRIEND_TEMPLATE_FUNC 1
#endif
#if (defined(__GNUG__) && (__GNUG__ == 2 && __GNUC_MINOR__ == 96))
#  define RW_DCML_NO_FRIEND_TEMPLATE_FUNC 1
#endif

#if defined(_MSC_VER) 
#  if (_MSC_VER >= 900) 
#    define RW_NO_LOCAL_STATIC_CONST 1
#  endif
#  if defined(_WIN64) || (_MSC_VER >= 1310)
#    define RW_DCML_NO_FRIEND_TEMPLATE_FUNC 1
#  endif
#endif

#if defined(__SUNPRO_CC)
#  define RW_MONEY_COMPILE_INSTANTIATE 1
#else
#  ifdef RW_COMPILE_INSTANTIATE
#    define RW_MONEY_COMPILE_INSTANTIATE 1
#  endif
#endif

// This works around a bug in the aix/gcc compiler.  The first
// set is used mainly in utlconst header files.  The other set 
// is used to allow initialization of method/group data members
//  which are normally const and initialized in the header files.
// In only aix/gcc builds, these data members still neede to be 
// initialized later, as can be seem in the examples.
#if defined(_AIX) && defined(__GNUG__) && (__GNUG__ == 2 && __GNU_MINOR__ < 96)
#  define RW_MNY_NO_EXPLICIT_INSTANTIATION 1
#  define RW_MNY_STATIC_CONST_DEL static
#  define RW_MNY_STATIC_CONST_DEF
#else
#  if defined(_MSC_VER) || defined(_AIX) || defined(__GNUG__) || (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x500)
#    define RW_MNY_STATIC_CONST_DEL static
#    define RW_MNY_STATIC_CONST_DEF
#  else
#    define RW_MNY_STATIC_CONST_DEL static const
#    define RW_MNY_STATIC_CONST_DEF const
#  endif
#endif

#if defined(__hppa) && !defined(HP_ANSI)
#define RW_MONEY_NO_INLINE_ARRAYS
#endif

#if !defined(RW_MONEY_COMPILE_INSTANTIATE) || defined(__BORLANDC__) || (defined(__HP_aCC) && defined(HP_ANSI))
#define RW_MONEY_SPECIALIZATION_IN_LIBRARY
#endif

#if !defined(RW_NO_NEW_TEMPLATE_SYNTAX)
#  if defined(RW_MNY_AIX5XX) || defined(RW_MNY_SGI72X) || defined(RW_MNY_SPR42X) || defined(RW_MNY_TBC54X) || \
      defined(__DECCXX) || defined(__INTEL_COMPILER) || (defined(_MSC_VER) && (_MSC_VER >= 1310))
#    define RW_MONEY_NEW_TEMPLATE_SYNTAX template<>
#  else
#    define RW_MONEY_NEW_TEMPLATE_SYNTAX
#  endif
#else
#  define RW_MONEY_NEW_TEMPLATE_SYNTAX
#endif

#if defined(__GNUG__) || defined(RW_MNY_TBC54X)
#  define RW_NO_STATIC_TEMPLATE_ATTRIBUTES
#endif

#if defined(__GNUG__)  || defined(RW_MNY_TBC54X) || (defined(__SUNPRO_CC) &&(__SUNPRO_CC>=0x500) && (__SUNPRO_CC != 0x540) &&  !defined(__SunOS_5_6))
#  define FRIEND_TEMPLATE_SPECIFIER <>
#else
#  define FRIEND_TEMPLATE_SPECIFIER
#endif

#if defined(__DECCXX) || (defined(__SUNPRO_CC) && ((__SUNPRO_CC<=0x500) || (__SUNPRO_CC == 0x540))) || \
    defined(RW_MNY_SOL26_SPR52X) || defined(__hppa) || defined(RW_MNY_TBC54X) || defined(RW_MNY_SGI72X)  || \
    defined(RW_MNY_AIX5XX) || defined(_WIN64) || defined(__INTEL_COMPILER) || \
    (defined(__BORLANDC__) && (__BORLANDC__ < 0x0530)) || \
    (defined(_MSC_VER) && (_MSC_VER >= 1310))
#  define RW_NO_NONTYPE_TEMPLATE_FUNCTIONS 1
#endif

#if (defined(__SUNPRO_CC))
#  define RW_NO_STATIC_VAR_IN_STATIC_MEMBER 1
#endif

#if (defined(_MSC_VER) && (_MSC_VER == 1310)) || defined(__hppa)
#  define RW_NO_STATIC_TEMPLATE_FUNCTION_PTR_INITIALIZER 1
#endif

#if defined(RWMNY_NO_LONG_DOUBLE_MATH) && !(defined(_AIX43) && RWSTDLIBTYPE == 1)
// Some compilers (I won't specify names intentionally, but it is HP aCC++ 3.30)
// support only basic operations over long double. We need more complicated
// functionality. Convert long double to double.
#  define floorl(x) floor(double(x))
#  define fmodl(x, y) fmod(double(x), double(y))
#endif

#if defined RW_NO_NEW_HEADER
#  define RW_MNY_NO_NEW_HEADER 1
#endif

#ifdef RW_DCML_MATH_FUNCTIONS_IN_STD
# define RWMNY_STD std
#else
#  define RWMNY_STD
#endif

#if defined(RW_RWV2X_STDLIB) && defined(_RWSTD_NO_LIBC_IN_STD)
#  if !defined(RW_NO_LIBC_IN_STD)
#    define RW_NO_LIBC_IN_STD 1
#  endif
#endif

#endif /* ndef __RW_DCMLDEFS_H__ */
