//-*--------------------------------------------------------------------------*-
//
// File Name   : rslocale.h
//
// Copyright (c) 1991 - 2006 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// June 5,2006  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __RSLOCALE_H
#define __RSLOCALE_H


#define LINK_RSLOCALE_DLL
#include "rslocdll.h"



enum RslCodePageType
   {
    RSL_CP_RSOEM    = 0,
    RSL_CP_RSANSI   = 1,
    RSL_CP_LCOEM    = 2,
    RSL_CP_LCANSI   = 3
   };

extern TRsLocaleExportTable GlobalRslLocale;
extern bool      RslLCUnicode;
extern bool      IsRsLocaleLoaded;
extern unsigned  RslDefProgEncode;


void InitRsLocale();
void DoneRsLocale();

char *LCCnvChar(unsigned dstCp, char *dst, int dstSz, unsigned srcCp, const char *src, int srcSz);

// Use only if  RslLCUnicode == true!
int   LCIsCharAlpha(int ch, unsigned cp);
char *LCStingUpper(char *str, int len, unsigned cp);
char *LCStingLower(char *str, int len, unsigned cp);
int   LCCompareStringN(const char *s1, const char *s2, int len, unsigned cp);
int   LCCompareString(const char *s1, int nCh1, const char *s2, int nCh2, unsigned cp);
const char *LCstristrR (const char *str1, const char *str2, unsigned cp);
int   LCToDnChar(int val, unsigned cp);
int   LCToUpChar(int val, unsigned cp);


// -----------------------------------------------
template<class CharType>
inline void RsAllocCvtBuffer(CharType **ppBuffer, int len, CharType *pFixedBuffer, int fixedLen)
{
 if(len > fixedLen)
   *ppBuffer = static_cast<CharType *>(calloc(len, sizeof(CharType)));
 else
  {
   memset((void *)pFixedBuffer, 0, sizeof(CharType)*fixedLen);

   *ppBuffer = pFixedBuffer;
  }

 if(!*ppBuffer)
   throw RSException("RsAllocCvtBuffer error");
}

// -----------------------------------------------
template<class CharType>
inline void RsFreeCvtBuffer(CharType *pBuffer, CharType *pFixedBuffer, int /*fixedLen*/)
{
 if(pBuffer != pFixedBuffer)
   free(pBuffer);
}

// -----------------------------------------------
template<class CharType, int nBufferLength = 128>
class RslTmpBuffer
    {
     public:

       RslTmpBuffer(size_t len)
       {
        RsAllocCvtBuffer(&m_pBuffer, (int)len, m_fixedBuffer, nBufferLength);
       }

      ~RslTmpBuffer() throw()
       {
        RsFreeCvtBuffer(m_pBuffer, m_fixedBuffer, nBufferLength);
       }

       operator const CharType *() const throw ()
       {
        return m_pBuffer;
       }

       operator CharType *() const throw ()
       {
        return m_pBuffer;
       }

       void init(size_t len)
       {
        RsFreeCvtBuffer(m_pBuffer, m_fixedBuffer, nBufferLength);
        RsAllocCvtBuffer(&m_pBuffer, (int)len, m_fixedBuffer, nBufferLength);
       }

     private:

       CharType  *m_pBuffer;
       CharType   m_fixedBuffer[nBufferLength];


       RslTmpBuffer(const RslTmpBuffer &) throw();

       RslTmpBuffer &operator = (const RslTmpBuffer&) throw();
    };

// -----------------------------------------------
template<int nBufferLength = 128>
class W2RSCHEX
    {
     public:

       W2RSCHEX(const wchar_t *psz, unsigned cpId)
       {
        if(!psz)
         {
          m_pBuffer = NULL;
          return;
         }

        int  nLength = GlobalRslLocale.WideCharToMultiBytePtr(GlobalRslLocale.cp[cpId], 0, psz, -1, NULL, 0, NULL, NULL);

        RsAllocCvtBuffer(&m_pBuffer, nLength, m_fixedBuffer, nBufferLength);

        if(!GlobalRslLocale.WideCharToMultiBytePtr(GlobalRslLocale.cp[cpId], 0, psz, -1, m_pBuffer, nLength, NULL, NULL))
          throw RSException("WideCharToMultiByte");
       }

      ~W2RSCHEX() throw()
       {
        RsFreeCvtBuffer(m_pBuffer, m_fixedBuffer, nBufferLength);
       }

       operator const char * () const throw ()
       {
        return m_pBuffer;
       }

     private:

       char  *m_pBuffer;
       char   m_fixedBuffer[nBufferLength];


       W2RSCHEX(const W2RSCHEX &) throw();

       W2RSCHEX &operator = (const W2RSCHEX &) throw();
    };

// -----------------------------------------------------------------------------
template<int nBufferLength = 128>
class RSCH2WEX
    {
     public:

       RSCH2WEX(const char *psz, unsigned cpId)
       {
        if(!psz)
         {
          m_pBuffer = NULL;
          return;
         }

        int  nLength = GlobalRslLocale.MultiByteToWideCharPtr(GlobalRslLocale.cp[cpId], 0, psz, -1, 0, 0);

        RsAllocCvtBuffer(&m_pBuffer, nLength, m_fixedBuffer, nBufferLength);

        if(!GlobalRslLocale.MultiByteToWideCharPtr(GlobalRslLocale.cp[cpId], 0, psz, -1, m_pBuffer, nLength))
          throw RSException("MultiByteToWideChar");
       }

       RSCH2WEX(const char *psz, unsigned cpId, int sz)
       {
        if(!psz)
         {
          m_pBuffer = NULL;
          return;
         }

        int  nLength = GlobalRslLocale.MultiByteToWideCharPtr(GlobalRslLocale.cp[cpId], 0, psz, sz, 0, 0);

        RsAllocCvtBuffer(&m_pBuffer, nLength, m_fixedBuffer, nBufferLength);

        if(!GlobalRslLocale.MultiByteToWideCharPtr(GlobalRslLocale.cp[cpId], 0, psz, sz, m_pBuffer, nLength))
          throw RSException("MultiByteToWideChar");
       }


      ~RSCH2WEX() throw()
       {
        RsFreeCvtBuffer(m_pBuffer, m_fixedBuffer, nBufferLength);
       }

       operator const wchar_t *() const throw ()
       {
        return m_pBuffer;
       }

       operator wchar_t *() const throw ()
       {
        return m_pBuffer;
       }

     private:

       wchar_t  *m_pBuffer;
       wchar_t   m_fixedBuffer[nBufferLength];


       RSCH2WEX(const RSCH2WEX &) throw();

       RSCH2WEX &operator = (const RSCH2WEX &) throw();
    };

// -----------------------------------------------------------------------------
inline int LCW2RSCH(char *outStr, int outSz, const wchar_t *inStr, int inSz, unsigned cp)
{
 unsigned  realCp = cp >= RSL_CP_RSOEM && cp <= RSL_CP_LCANSI ? GlobalRslLocale.cp [cp] : cp;

 return GlobalRslLocale.WideCharToMultiBytePtr(realCp, 0, inStr, inSz, outStr, outSz, NULL, NULL);
}

// -----------------------------------------------------------------------------
inline int LCRSCH2W (wchar_t *outStr, int outSz, const char *inStr, int inSz, unsigned cp)
{
 unsigned  realCp = cp >= RSL_CP_RSOEM && cp <= RSL_CP_LCANSI ? GlobalRslLocale.cp [cp] : cp;

 return GlobalRslLocale.MultiByteToWideCharPtr(realCp, 0, inStr, inSz, outStr, outSz);
}

// -----------------------------------------------------------------------------
inline int LCW2RSOEM (char *outStr, int outSz, const wchar_t *inStr, int inSz)
{
 return GlobalRslLocale.WideCharToMultiBytePtr(GlobalRslLocale.cp[RSL_CP_RSOEM], 0, inStr, inSz, outStr, outSz, NULL, NULL);
}

// -----------------------------------------------------------------------------
inline int LCRSOEM2W (wchar_t *outStr, int outSz, const char *inStr, int inSz)
{
 return GlobalRslLocale.MultiByteToWideCharPtr(GlobalRslLocale.cp[RSL_CP_RSOEM], 0, inStr, inSz, outStr, outSz);
}

// -----------------------------------------------------------------------------
inline int LCW2RSANSI (char *outStr, int outSz, const wchar_t *inStr, int inSz)
{
 return GlobalRslLocale.WideCharToMultiBytePtr(GlobalRslLocale.cp[RSL_CP_RSANSI], 0, inStr, inSz, outStr, outSz, NULL, NULL);
}

// -----------------------------------------------------------------------------
inline int LCRSANSI2W (wchar_t *outStr, int outSz, const char *inStr, int inSz)
{
 return GlobalRslLocale.MultiByteToWideCharPtr(GlobalRslLocale.cp[RSL_CP_RSANSI], 0, inStr, inSz, outStr, outSz);
}


#if defined(UNICODE) || defined(USE_UNICVT)

    class W2RSOEM : public W2RSCHEX<>
        {
         public:
           W2RSOEM(const wchar_t *psz) : W2RSCHEX<>(psz, RSL_CP_RSOEM) {}
        };

    class W2RSANSI : public W2RSCHEX<>
        {
         public:
           W2RSANSI(const wchar_t *psz) : W2RSCHEX<>(psz, RSL_CP_RSANSI) {}
        };

    class RSANSI2W : public RSCH2WEX<>
        {
         public:
           RSANSI2W(const char *psz) : RSCH2WEX<>(psz, RSL_CP_RSANSI) {}
        };

    class RSOEM2W : public RSCH2WEX<>
        {
         public:
           RSOEM2W(const char *psz) : RSCH2WEX<>(psz, RSL_CP_RSOEM) {}
        };


    #define  RSOEM2WLEN(ptr, len)  RSCH2WEX<>(ptr, RSL_CP_RSOEM, len)
    #define  RSCH2W(ptr, cp)       RSCH2WEX<>(ptr, cp)
    #define  W2RSCH(ptr, cp)       W2RSCHEX<>(ptr, cp)

    #define  CPW2RSCH(outStr, outSz, inStr, inSz, cp)  LCW2RSCH(outStr, outSz, inStr, inSz, cp)
    #define  CPRSCH2W(outStr, outSz, inStr, inSz, cp)  LCRSCH2W(outStr, outSz, inStr, inSz, cp)
    #define  CPW2RSOEM(outStr, outSz, inStr, inSz)     LCW2RSOEM(outStr, outSz, inStr, inSz)
    #define  CPRSOEM2W(outStr, outSz, inStr, inSz)     LCRSOEM2W(outStr, outSz, inStr, inSz)
    #define  CPW2RSANSI(outStr, outSz, inStr, inSz)    LCW2RSANSI(outStr, outSz, inStr, inSz)
    #define  CPRSANSI2W(outStr, outSz, inStr, inSz)    LCRSANSI2W(outStr, outSz, inStr, inSz)

#else  // UNICODE

    #define  RSCH2W(ptr, cp)       ptr
    #define  W2RSCH(ptr, cp)       ptr
    #define  RSOEM2WLEN(ptr, len)  ptr

    #define  RSOEM2W(ptr)   ptr
    #define  W2RSOEM(ptr)   ptr
    #define  RSANSI2W(ptr)  ptr
    #define  W2RSANSI(ptr)  ptr

    #define  CPW2RSCH(outStr, outSz, inStr, inSz, cp)  strncpy(outStr, inStr, outSz)
    #define  CPRSCH2W(outStr, outSz, inStr, inSz, cp)  strncpy(outStr, inStr, outSz)
    #define  CPW2RSOEM(outStr, outSz, inStr, inSz)     strncpy(outStr, inStr, outSz)
    #define  CPRSOEM2W(outStr, outSz, inStr, inSz)     strncpy(outStr, inStr, outSz)
    #define  CPW2RSANSI(outStr, outSz, inStr, inSz)    strncpy(outStr, inStr, outSz)
    #define  CPRSANSI2W(outStr, outSz, inStr, inSz)    strncpy(outStr, inStr, outSz)

#endif // UNICODE

#endif // __RSLOCALE_H

/* <-- EOF --> */