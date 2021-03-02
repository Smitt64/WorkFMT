//-*--------------------------------------------------------------------------*-
//
// File Name   : property.h
//
// Copyright (c) 1991 - 2006 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// October 11,2007  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __RSPROPERTY_H
#define __RSPROPERTY_H

_TOOLEXP const char *ToolsGetProperty(const char *key);
_TOOLEXP bool        ToolsSetProperty(const char *key, const char *value);

#ifdef USE_TOOLS_PROPENUM
    _TOOLEXP PROPENUM    ToolsEnumProperty(PROPENUM id, TPropertyHolder *holder);
#endif


#endif // __RSPROPERTY_H

/* <-- EOF --> */