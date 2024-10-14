//-*--------------------------------------------------------------------------*-
//
// File Name   : RsdUtl.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// November 12, 2002  Khorev - Create file
//-*--------------------------------------------------------------------------*-


// Разные полезные функции для внутренних нужд RSD

#ifndef RSDUTL_H
#define RSDUTL_H

#include "RSDUtlBuildOption.h"

#ifdef USE_NUMERIC
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#endif

DLL_DIRECTION char* gtempnam ();

#ifdef USE_NUMERIC
DLL_DIRECTION bool  correctNumericValue (SQL_NUMERIC_STRUCT& num);
#endif

#endif
