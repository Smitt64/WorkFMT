//-*--------------------------------------------------------------------------*-
//
// File Name   : appcfgdata.hpp
//
// Copyright (c) 1991 - 2005 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// December 9,2005  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-
#ifndef __APPCFGDATA_HPP
#define __APPCFGDATA_HPP

#ifdef __cplusplus

enum RslRegPlacement
   {
    RslRegAuto,
    RslRegHKLM,
    RslRegHKCU
   };


bool GetProductRegKey        (const char *appName, PHKEY usekey, char *buffer, RslRegPlacement root = RslRegAuto);
bool GetProductFileName      (const char *appName, const char *fileName, char *buffer);
bool GetProductFileNameCommon(const char *appName, const char *fileName, char *buffer);
bool GetLagestVerKey         (const char *productKey, PHKEY usekey, char *buffer, const char *subKey);

const char *GetPersonalFolder(char *path);

const char *RslGetProductName(void);
const char *RslGetProductVersion(void);
void InitProductConfig();

#endif

#endif // __APPCFGDATA_HPP

/* <-- EOF --> */