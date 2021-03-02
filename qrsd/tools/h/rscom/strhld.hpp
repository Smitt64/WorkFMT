//-*--------------------------------------------------------------------------*-
//
// File Name   : strhld.hpp
//
// Copyright (c) 1991 - 2005 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// May 25,2005  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-
#ifndef __STRHLD_HPP
#define __STRHLD_HPP

#ifndef __RSCOM_HPP
# include "rscom/rscom.hpp"
#endif      

// -----------------------------------------------------------------------------
class TRsStrHolder
    {
     public:
       // ----------------------------------------
       TRsStrHolder(const char *ptr = 0)
       {
        strElem.pStr = 0;
        strElem.proc = 0;

        set(ptr);
       }

       // ----------------------------------------
       TRsStrHolder(const TRsStrHolder &p)
       {
        strElem.pStr = 0;
        strElem.proc = 0;

        set(p.strElem.pStr);
       }

       // ----------------------------------------
       TRsStrHolder &operator = (const TRsStrHolder &p)
       {
        if(&strElem != &(p.strElem))
          set(p.strElem.pStr);

        return *this;
       }

       // ----------------------------------------
      ~TRsStrHolder()
       {
        freeString();
       }

       // ----------------------------------------
       operator const char * () const
       {
        return strElem.pStr;
       }

       // ----------------------------------------
       bool isEmpty()
       {
        return !strElem.pStr;
       }

       // ----------------------------------------
       const char *append(const char *str, const char *separator = ";")
       {
        size_t  addLen = strlen(str) + sizeof(char) + (separator ? strlen(separator) : 0);
        size_t  curLen = strElem.pStr ? strlen(strElem.pStr) : 0;
        char   *ptr    = (char *)allocMem(addLen + curLen);


        if(ptr)
         {
          *ptr = '\0';

          if(strElem.pStr)
           {
            strcat(ptr, strElem.pStr);

            if(separator)
              strcat(ptr, separator);
           }

          strcat(ptr, str);

          freeString();

          strElem.pStr = ptr;
          strElem.proc = freeMem;
         }

        return ptr;
       }

       // ----------------------------------------
       void set(const char *ptr, int len)
       {
        freeString();

        if(ptr)
         {
          strElem.pStr = (char *)allocMem(len + 1);

          if(strElem.pStr)
           {
            strncpy(strElem.pStr, ptr, len);

            strElem.pStr[len] = '\0';
           }

          strElem.proc = freeMem;
         }
       }

     private:

       // ----------------------------------------
       static void *allocMem(size_t sz)
       {
        return malloc(sz);
       }

       // ----------------------------------------
       static void  RSCOMCALL freeMem(void *p)
       {
        free(p);
       }

       // ----------------------------------------
       void set(const char *ptr)
       {
        freeString();

        if(ptr)
         {
          strElem.pStr = (char *)allocMem(strlen(ptr) + 1);

          if(strElem.pStr)
            strcpy(strElem.pStr, ptr);

           strElem.proc = freeMem;
         }
       }

       // ----------------------------------------
       void freeString()
       {
        if(strElem.pStr && strElem.proc)
         {
          strElem.proc(strElem.pStr);
          strElem.pStr = 0;
         }
       }

       // ----------------------------------------
       TRsStrPtr  strElem;
    };

// -----------------------------------------------------------------------------
typedef TRsStrHolder  TRsString;

#endif  // __STRHLD_HPP

/* <-- EOF --> */