#ifndef RW_DECDEFS_H
#define RW_DECDEFS_H

/***************************************************************************
 * Macro definitions removed from decbase, decport and decin
 * but still needed for backward compability
 *
 * $Id: //money/ed6-rel/rw/currency/decdefs.h#1 $
 *
 * Copyright (c) 1993, 1996-2003 Rogue Wave Software, Inc.  All Rights Reserved.
 *
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software,
 * Inc. pursuant to a written license agreement and may be used, copied,
 * transmitted, and stored only in accordance with the terms of such
 * license and with the inclusion of the above copyright notice.  This
 * computer software or any other copies thereof may not be provided or
 * otherwise made available to any other person.
 *
 * U.S. Government Restricted Rights.  This computer software is provided
 * with Restricted Rights.  Use, duplication, or disclosure by the
 * Government is subject to restrictions as set forth in subparagraph (c)
 * (1) (ii) of The Rights in Technical Data and Computer Software clause
 * at DFARS 252.227-7013 or subparagraphs (c) (1) and (2) of the
 * Commercial Computer Software - Restricted Rights at 48 CFR 52.227-19,
 * as applicable.  Manufacturer is Rogue Wave Software, Inc., 5500
 * Flatiron Parkway, Boulder, Colorado 80301 USA.
 *
 **************************************************************************
 * Special stuff for Windows NT / Win32s (_WIN32/64).
 *
 * Under Windows, these header files can be used in two different modes:
 * as part of a DLL, or as part of a regular (static) library.
 * For each of these two ways, we can either be COMPILING the library,
 * or USING the library.  This makes a matrix of four possible situations.
 *
 *************************************************************************
 ******************************  DLL *************************************
 *************************************************************************
 *
 *                         Creating a DLL
 *
 * The macro __DLL__ should be define when compiling to create a DLL.
 *
 *     The Borland compilers automatically do this if either the -WD or
 *     -WDE switch is being used.  In this situation, the macros
 *     RWExport and rwexport expand to _export.
 *
 *     For other compilers, we must define __DLL__ where appropriate
 *     if the compiler doesn't.  See the example for Microsoft below.
 *
 *     RWExport and rwexport expand to _export by default.  Massage as
 *     necessary for your compiler; again see below for the Microsoft
 *     specific directives.
 *
 *                       Using the resultant DLL
 *
 * In order to use the resultant DLL, you must define the macro _RWCLASSDLL
 * when compiling.  This will communicate to the Rogue Wave header files
 * your intention to use a DLL version of the Rogue Wave library.
 *
 * If you intend to use a DLL version of the Borland RTL as well, then you
 * should also define the macro _CLASSDLL, as per Borland's instructions.
 *
 * You must also specify the -WS switch ("smart callbacks") for the
 * Borland Compiler.
 *
 *
 *************************************************************************
 ***********************  Static Windows Library **************************
 *************************************************************************
 *
 *
 *                Creating a RW static Windows library
 *
 * If your intention is to create a Rogue Wave library to be used as
 * as a static Windows library, then the macro _WIN32/64
 * must have been defined in compiler.h (or by the compiler directly).
 *
 * Borland:     _Windows will be defined automatically if
 *              any -W switch is used.  Borland defines _WIN32/64 automatically
 *              in their 32-bit compiler for Windows NT / Win32s.
 *
 * Microsoft:   Microsoft automatically defines _WINDOWS if you use the
 *              /GA (preferred) or /GW switch.
 *              _WIN32/64 will only be defined if either it, or WIN32,
 *              is explicitly defined on the cl386 command line.
 *
 *
 *                 Using a RW static Windows library
 *
 * Nothing special needs to be done.  Just link compile with the appropriate
 * compile switches and link in the RW static Windows library.
 *
 **************************************************************************/

//#include "rw/compiler.h"
//#include "rw/dcmldefs.h"

// Support for new product wide using DLL macro:
#ifdef RWDLL
#  ifndef _RWDCMLDLL
#    define _RWDCMLDLL 1
#  endif
#endif


/*
 * Check for Microsoft C/C++ and massage as necessary.
 */
#if defined(_MSC_VER)
#  if defined(_WINDLL) && !defined(__DLL__)
#    define __DLL__ 1
#  endif
#  if !defined(_export)
#    define _export __export
#  endif
#endif


#if defined(_RWDCMLBUILDDLL) || defined(_RWBUILDDLL) || defined(_RWDBBUILDDLL) 
#  ifndef _RWDCMLDLL
#    define _RWDCMLDLL 1
#  endif
#  ifndef _RWTOOLSDLL
#    define _RWTOOLSDLL 1
#  endif
#  ifndef _RWDBDLL
#    define _RWDBDLL 1
#  endif
#endif

// The Money DLL related defines.
#if defined(_RWDCMLDLL) || defined(_RWTOOLSDLL) || defined(_RWDBDLL)
#  if defined(__WIN16__) && !defined(__LARGE__)
#    error   Must use large memory model when compiling or using the Money DLL!
#  endif
#  if defined(_RWDCMLBUILDDLL) || defined(_RWBUILDDLL) || defined(_RWDBBUILDDLL) 
// Compiling the Money DLL.
#    if !defined(_MSC_VER) && !defined(__SC__) && !defined(__WATCOMC__)
#      define RWDCMLExport _export                  /* Mark classes as exported */
#      define rwdcmlexport _export                  /*    & functions as well   */
#      define RWDCMLTExport RWDCMLExport
#    else /* Microsoft: */
#      define RWDCMLExport __declspec(dllexport)
#      define rwdcmlexport __declspec(dllexport)
#      define RWDCMLTExport
#    endif
#  else
     // Using the Money DLL.
#    if defined(__WIN16__)
#      define RWDCMLExport _import  /* Mark classes as huge  */
#      define rwdcmlexport far      /*    & functions as far */
#    elif defined(_WIN32) || defined(_WIN64)
#      if !defined(_MSC_VER) && !defined(__WATCOMC__) && !defined(__IBMCPP__)
#        if defined(__SC__)
#          define RWDCMLExport
#          define rwdcmlexport
#        else 
#          define RWDCMLExport _import
#          define rwdcmlexport
#        endif
#      else
#        define RWDCMLExport  __declspec(dllimport)
#        define rwdcmlexport
#      endif  
#    endif
#  endif
#else
   // Neither compiling, nor using the Money DLL.
#  define RWDCMLExport
#  define rwdcmlexport
#endif

/*
 * If the programmer is building a DLL that uses the Money DLL,
 * code for the template and generic classes will actually find its
 * way into the programmer's DLL.  To have those classes exported,
 * define _RWDCMLEXPORT_INSTANTIATED (feature not available with Microsoft)
 */

#ifndef _RWDCMLBUILDDLL
#  ifdef _RWDCMLEXPORT_INSTANTIATED
#    if !defined(_MSC_VER) && !defined(__WATCOMC__) && !defined(__IBMCPP__)
#      define RWDCMLTExport _export
#    else
#      define RWDCMLTExport /* MS does not allow dllexport for templates */
#    endif
#  else
#    define RWDCMLTExport
#  endif
#endif

#endif
