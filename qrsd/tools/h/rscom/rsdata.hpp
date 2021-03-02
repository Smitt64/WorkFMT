/*-----------------------------------------------------------------------*-

 File Name   : rsdata.hpp

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 January 29,2002  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __RSDATA_HPP
#define __RSDATA_HPP

#include "int64.h"
#include "bdate.h"

#include <packpshn.h>

// -----------------------------------------------------------------------------
enum RsDataTypes
   {
    RSCOM_UNDEF    = 0,
    RSCOM_CHR      = 1,
    RSCOM_UCHR     = 2,
    RSCOM_SHORT    = 3,
    RSCOM_USHORT   = 4,
    RSCOM_INT      = 5,
    RSCOM_UINT     = 6,

    RSCOM_DOUBLE   = 7,

    RSCOM_STR      = 8,
    RSCOM_OBJ      = 9,
    RSCOM_BOOL     = 10,

    RSCOM_DATE     = 11,
    RSCOM_TIME     = 12,
    RSCOM_DTTM     = 13,
    RSCOM_MONEY_T  = 14,
    RSCOM_FDEC     = 15,

    RSCOM_PARMAR   = 16, //  IParmInfo
    RSCOM_PTR      = 17,
    RSCOM_SPECVAL  = 18,
    RSCOM_ERROR    = 19,

    RSCOM_NUMERIC  = 20,

    RSCOM_STR_LONG = 21, // Transfer only type

    RSCOM_BIGINT   = 22,
    RSCOM_UBIGINT  = 23,

    RSCOM_NULL     = 31 // Indicate no return value
   };
      
// -----------------------------------------------------------------------------
#define  RV_VAR       "\x7F"  // Indicate val of any type as RSCOM_UNDEF
#define  RV_I1        "\x01"
#define  RV_UI1       "\x02"
#define  RV_I2        "\x03"
#define  RV_UI2       "\x04"
#define  RV_I4        "\x05"
#define  RV_UI4       "\x06"
#define  RV_DBL       "\x07"
#define  RV_STR       "\x08"
#define  RV_OBJ       "\x09"
#define  RV_BOOL      "\x0A"
#define  RV_DATE      "\x0B"
#define  RV_TIME      "\x0C"
#define  RV_DTTM      "\x0D"
#define  RV_MONEY_T   "\x0E"
#define  RV_FDEC      "\x0F"
#define  RV_SPEC      "\x13"
#define  RV_NUMERIC   "\x14"
#define  RV_I8        "\x15"
#define  RV_UI8       "\x16"


#define  RVO_VAR      "\xFF"
#define  RVO_I1       "\x81"
#define  RVO_UI1      "\x82"
#define  RVO_I2       "\x83"
#define  RVO_UI2      "\x84"
#define  RVO_I4       "\x85"
#define  RVO_UI4      "\x86"
#define  RVO_DBL      "\x87"
#define  RVO_STR      "\x88"
#define  RVO_OBJ      "\x89"
#define  RVO_BOOL     "\x8A"
#define  RVO_DATE     "\x8B"
#define  RVO_TIME     "\x8C"
#define  RVO_DTTM     "\x8D"
#define  RVO_MONEY_T  "\x8E"
#define  RVO_FDEC     "\x8F"
#define  RVO_SPEC     "\x93"
#define  RVO_NUMERIC  "\x94"
#define  RVO_I8       "\x95"
#define  RVO_UI8      "\x96"
                   

#ifdef NUMERIC_AS_MONEY
    #define  RSCOM_MONEY  RSCOM_NUMERIC
    #define  RV_MONEY     RV_NUMERIC  
    #define  RVO_MONEY    RVO_NUMERIC
#else
    #define  RSCOM_MONEY  RSCOM_MONEY_T
    #define  RV_MONEY     RV_MONEY_T  
    #define  RVO_MONEY    RVO_MONEY_T
#endif

#define  RS_OUTFLAG     0x80
#define  RS_VARTYPE     0x40
#define  RS_CHANGED     0x20  // This attr is not marshaled

#define  RS_FLAGMASK    0xC0
#define  RS_TYPEMASK    0x1F


// -----------------------------------------------------------------------------
typedef unsigned char  RsValType;
typedef unsigned char  RsAttrType;

typedef unsigned char  RsValAttrType;


// -----------------------------------------------------------------------------
class RsComSpecVal
{
 public:
   // --------------------------------------------
   RsComSpecVal(long val) : intVal(val)
   {
   }

   // --------------------------------------------
   bool isNull()
   {
    return intVal == 0;
   }

   // --------------------------------------------
   bool isOpt()
   {
    return intVal == 1;
   }

   // --------------------------------------------
   bool isEmpt()
   {
    return intVal == 2;
   }

   // --------------------------------------------
   bool isNullObj()
   {
    return intVal == 3;
   }

   // --------------------------------------------
   // --------------------------------------------
   long intVal;   
};

// -----------------------------------------------------------------------------
class RsComNullVal : public RsComSpecVal
{
 public:
   RsComNullVal() : RsComSpecVal(0) {}
};

// -----------------------------------------------------------------------------
class RsComNullObj : public RsComSpecVal
{
 public:
   RsComNullObj() : RsComSpecVal(3) {}
};

// -----------------------------------------------------------------------------
class RsComOptVal : public RsComSpecVal
{
 public:
   RsComOptVal() : RsComSpecVal(1) {}
};

// -----------------------------------------------------------------------------
class RsComEmptVal : public RsComSpecVal
{
 public:
   RsComEmptVal() : RsComSpecVal(2) {}
};


// -----------------------------------------------------------------------------
struct TStrVal
     {
      const char  *oem;
      const char  *ansi;
     };

// -----------------------------------------------------------------------------
struct TRsDataEx
     {
      RsValAttrType             typeAttr;

      unsigned char             r1;
      short int                 r2;
      long                      r3;

      union
          {
           bool                 boolVal;
           signed char          chrVal;
           unsigned char        uchrVal;

           short int            shortVal;
           unsigned short int   ushortVal;

           long                 intVal;
           unsigned long        uintVal;

           double               doubVal;

           TStrVal              strVal;

           IRsObj              *objVal;

           bdate                dateVal;
           btime                timeVal;
           btimestamp           dttmVal;

#ifdef USE_FDECIMAL
           FDecimal_t           fdecVal;
#endif

           IParmInfo           *parmVal;   
           void                *ptrVal;  // For local use only
           char                 buff[8];

           int64_t              bigintVal;
           uint64_t             ubigintVal;
          };
     };

// -----------------------------------------------------------------------------
struct TRsData
     {
      RsValType                 type;
      RsAttrType                attr;


      // --------------------------------------------
      union
          {
           bool                 boolVal;
           signed char          chrVal;
           unsigned char        uchrVal;

           short int            shortVal;
           unsigned short int   ushortVal;

           long                 intVal;
           unsigned long        uintVal;

           double               doubVal;

           TStrVal              strVal;

           IRsObj              *objVal;

           bdate                dateVal;
           btime                timeVal;
           btimestamp           dttmVal;

#ifdef USE_FDECIMAL
           FDecimal_t           fdecVal;
#endif

           IParmInfo           *parmVal;   
           void                *ptrVal;  // For local use only
           char                 buff[8];

           int64_t              bigintVal;
           uint64_t             ubigintVal;
          };
     };

// -----------------------------------------------------------------------------
#ifdef SQLBUILD
    #define  RSCOM_MONEY_CVT  1
#else
    #define  RSCOM_MONEY_CVT  0
#endif

// -----------------------------------------------------------------------------
class TValRef
{
 private:
   // --------------------------------------------
   // --------------------------------------------
   IParmInfo    *parms;
   int           index;
   TRsCodePage   codePage;

 public:
   // --------------------------------------------
   IParmInfo *getParmInfo() const
   {
    return parms;
   }

   // --------------------------------------------
   int getIndex() const 
   {
    return index;
   }

   // --------------------------------------------
   TRsCodePage getCodePage() const
   {
    return codePage;
   }

   // --------------------------------------------
   TValRef(IParmInfo *prm, int ind, TRsCodePage code = RSCOM_CODEPAGE) : parms(prm), index(ind), codePage(code)
   {
    RSASSERT(prm);
   }

   // --------------------------------------------
   TValRef & operator = (const TValRef &val)
   {
    if(parms)
      parms->setDataEx(index, val.getData(), val.codePage);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (RsComSpecVal &val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_SPECVAL, &val.intVal);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (bool val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_BOOL, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (signed char val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_CHR, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (unsigned char val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_UCHR, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (short val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_SHORT, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (unsigned short val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_USHORT, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (int v)
   {
    long  val = v;


    if(parms)
      parms->setDataVal(index, RSCOM_INT, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (unsigned int v)
   {
    unsigned long  val = v;


    if(parms)
      parms->setDataVal(index, RSCOM_UINT, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (long val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_INT, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (unsigned long val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_UINT, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (int64_t val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_BIGINT, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (uint64_t val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_UBIGINT, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (double val)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_DOUBLE, &val);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (const char *p)
   {
    if(parms)
      parms->setDataFromStringAs(index, RSCOM_STR, p, codePage);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (const IRsObj *p)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_OBJ, (void *)p);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (const bdate &p)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_DATE, (void *)&p);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (const btime &p)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_TIME, (void *)&p);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (const btimestamp &p)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_DTTM, (void *)&p);

    return *this;
   }

#ifdef USE_FDECIMAL
   // --------------------------------------------
   TValRef & operator = (const FDecimal_t &p)
   {     
#ifdef SQLBUILD
    if(parms)
      parms->setDataVal(index, RSCOM_MONEY_T, (void *)&p);

    return *this;
#else
    if(parms)
      parms->setDataVal(index, RSCOM_FDEC, (void *)&p);

    return *this;
#endif
   }

   // --------------------------------------------
   TValRef & operator = (const FDecimal &par)
   {
    if(parms)
     {
      FDecimal_t  fdec;


      par.get(&fdec);

#ifdef SQLBUILD
      parms->setDataVal(index, RSCOM_MONEY_T, (void *)&fdec);
#else
      parms->setDataVal(index, RSCOM_FDEC, (void *)&fdec); 
#endif
     }

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (const FMoney &par)
   {
    if(parms)
     {
      FDecimal_t  fdec;


      par.get(&fdec);

      parms->setDataVal(index, RSCOM_MONEY_T, (void *)&fdec);
     }

    return *this;
   }
#endif

#ifdef USE_NUMERIC
   // --------------------------------------------
   TValRef & operator = (const Numeric_t &p)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_NUMERIC, (void *)&p);

    return *this;
   }

   // --------------------------------------------
   TValRef & operator = (const Numeric &p)
   {
    if(parms)
      parms->setDataVal(index, RSCOM_NUMERIC, (void *)&p);

    return *this;
   }
#endif

   // --------------------------------------------
   const TRsDataEx *getData() const
   {
    return parms ? parms->getDataEx(index) : 0;
   }

   // --------------------------------------------
   TRsStat getDataVal(int tp, void *data) const
   {
    TRsStat  stat = parms ? parms->getDataValEx(index, tp, data, RSCOM_MONEY_CVT) : RSL_STAT_CNVDATA;


    if(stat != RSL_STAT_OK && parms)
      throw XRSComError(parms->getAppObj(), stat);

    return stat;
   }

   // --------------------------------------------
   operator bool () const
   {
    bool  retVal;


    getDataVal(RSCOM_BOOL, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator signed char () const
   {
    signed char  retVal;


    getDataVal(RSCOM_CHR, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator unsigned char () const
   {
    unsigned char  retVal;


    getDataVal(RSCOM_UCHR, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator short int () const
   {
    short int  retVal;


    getDataVal(RSCOM_SHORT, &retVal);

    return retVal;
   }

   // -------------------------------------------- 
   operator unsigned short () const
   {
    unsigned short  retVal;


    getDataVal(RSCOM_USHORT, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator int () const
   {
    return operator long();
   }

   // --------------------------------------------
   operator unsigned int () const
   {
    return operator unsigned long();
   }

   // --------------------------------------------
   operator long () const
   {
    long  retVal;


    getDataVal(RSCOM_INT, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator unsigned long () const
   {
    unsigned long  retVal;


    getDataVal(RSCOM_UINT, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator int64_t () const
   {
    int64_t  retVal;


    getDataVal(RSCOM_BIGINT, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator uint64_t () const
   {
    uint64_t  retVal;


    getDataVal(RSCOM_UBIGINT, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator double () const
   {
    double  retVal;


    getDataVal(RSCOM_DOUBLE, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator const char * () const
   {
    return parms ? parms->toString(index, codePage) : "";
   }

   // --------------------------------------------                         
   const char *toString(TRsCodePage cp) const
   {
    return parms ? parms->toString(index, cp) : "";
   }

   // --------------------------------------------
   operator IRsObj * () const
   {
    const TRsDataEx  *ptr = getData();


    if(ptr)
     {
      int  type = getType();


      if(type == RSCOM_OBJ || type == RSCOM_PARMAR)
        return ptr->objVal;
      else if(type == RSCOM_UNDEF || (type == RSCOM_INT && !ptr->intVal))
        return NULL;
     }

    if(parms)
      throw XRSComError(parms->getAppObj(), RSL_STAT_CNVDATA);

    return 0;
   }

   // --------------------------------------------
   operator IParmInfo * () const
   {
    const TRsDataEx  *ptr = getData();


    if(ptr)
     {
      int  type = getType();


      if(type == RSCOM_PARMAR)
        return ptr->parmVal;
      else if(type == RSCOM_UNDEF)
        return NULL;
     }

    if(parms)
      throw XRSComError(parms->getAppObj(), RSL_STAT_CNVDATA);

    return 0;
   }

   // --------------------------------------------
   operator bdate() const
   {
    bdate  bd;


    memset(&bd, 0, sizeof(bdate));

    getDataVal(RSCOM_DATE, &bd);

    return bd;
   }

   // --------------------------------------------
   operator btime() const
   {
    btime  tm;


    memset(&tm, 0, sizeof(btime));

    getDataVal(RSCOM_TIME, &tm);

    return tm;
   }

   // -------------------------------------------- 
   operator btimestamp() const
   {
    btimestamp  tm;


    memset(&tm, 0, sizeof(btimestamp));

    getDataVal(RSCOM_DTTM, &tm);

    return tm;
   }

   // --------------------------------------------
   operator RsComSpecVal() const
   {
    RsComSpecVal  val(0);


    getDataVal(RSCOM_SPECVAL, &val.intVal);

    return val;
   }

#ifdef USE_FDECIMAL
   // --------------------------------------------
   operator FDecimal_t() const
   {
    FDecimal_t  tm;


    memset(&tm, 0, sizeof(FDecimal_t));

    getDataVal(RSCOM_FDEC, &tm);

    return tm;
   }

   // --------------------------------------------
   operator FDecimal() const
   {
    FDecimal_t  tm;

    
    memset(&tm, 0, sizeof(FDecimal_t));

    getDataVal(RSCOM_FDEC, &tm);

    return FDecimal (tm);
   }

   // --------------------------------------------
   operator FMoney() const
   {
    FDecimal_t  tm;


    memset(&tm, 0, sizeof(FDecimal_t));

    getDataVal(RSCOM_MONEY_T, &tm);

    return FMoney (tm);
   }
#endif

#ifdef USE_NUMERIC
   // --------------------------------------------
   operator Numeric_t() const
   {
    Numeric_t  val;


    getDataVal(RSCOM_NUMERIC, &val);

    return val;
   }

   // --------------------------------------------
   operator Numeric() const
   {
    Numeric  val;


    getDataVal(RSCOM_NUMERIC, &val);

    return val;
   }
#endif

   // --------------------------------------------
   void setAttr(int attr) 
   {
    if(parms)
      parms->setAttr(index, attr);
   }

   // --------------------------------------------
   int getAttr() const
   {
    if(parms)
      return parms->getAttr(index);

    return 0;
   }

   // --------------------------------------------
   int getType() const
   {
    if(parms)
      return parms->getType(index);

    return RSCOM_UNDEF;
   }

   // --------------------------------------------
   void clear()
   {
    if(parms)
      parms->clearData(index);
   }

   // --------------------------------------------
   TRsStat setDataVal(int asType, int tp, const void *data)
   {
    TRsStat  stat = parms ? parms->setDataValAs(index, asType, tp, data, RSCOM_MONEY_CVT) : RSL_STAT_CNVDATA;


    if(stat != RSL_STAT_OK && parms)
      throw XRSComError(parms->getAppObj(), stat);

    return stat;
   }

   // --------------------------------------------
   void setAs(int asType, double val)
   {
    setDataVal(asType, RSCOM_DOUBLE, &val);
   }

   // --------------------------------------------
   void setAs(int asType, int val)
   {
    setDataVal(asType, RSCOM_INT, &val);
   }

#ifdef USE_NUMERIC
   // --------------------------------------------
   void setAs(int asType, const Numeric_t &p)
   {
    setDataVal(asType, RSCOM_NUMERIC, &p);
   }
#endif

#ifdef USE_FDECIMAL
   // --------------------------------------------
   void setAs(int asType, const FDecimal_t &p)
   {
#ifdef SQLBUILD
    setDataVal(asType, RSCOM_MONEY_T, &p);
#else
    setDataVal(asType, RSCOM_FDEC, &p);
#endif
   }

   // --------------------------------------------
   void setAs(int asType, const FDecimal &par)
   {
    FDecimal_t  fdec;


    par.get(&fdec);

#ifdef SQLBUILD
    setDataVal(asType, RSCOM_MONEY_T, &fdec);
#else
    setDataVal(asType, RSCOM_FDEC, &fdec);
#endif
   }

   // --------------------------------------------
   void setAs(int asType, const FMoney &par)
   {
    FDecimal_t  fdec;


    par.get(&fdec);

    setDataVal(asType, RSCOM_MONEY_T, &fdec);
   }
#endif


   friend class TParmArray;
   friend class TBindObj;   
};


// -----------------------------------------------------------------------------
class TParmArray
{
 private:
   // --------------------------------------------
   // --------------------------------------------
   IParmInfo    *parms;
   TRsCodePage   codePage;

 public:
   // --------------------------------------------
   TParmArray(IParmInfo *p = 0, TRsCodePage code = RSCOM_CODEPAGE, bool doAddRef = true) : parms(p), codePage(code)
   {
    if(parms && doAddRef)
      parms->addRef();
   }

   // --------------------------------------------
  ~TParmArray()
   {
    if(parms)
      parms->release();
   }

   // --------------------------------------------
   TParmArray(IProxyStubMgr *ps, int count, bool ret = false, TRsCodePage code = RSCOM_CODEPAGE) : parms(0), codePage(code)
   {
    init(ps, count, ret);
   }

   // --------------------------------------------
   TParmArray(TAppObj *obj, int count, bool ret = false, TRsCodePage code = RSCOM_CODEPAGE) : parms(0), codePage(code)
   {
    init(obj, count, ret);
   }

   // --------------------------------------------
   void release()
   {
    if(parms)
      parms->release();

    parms = 0;
   }

   // --------------------------------------------
   void attach(IParmInfo *p, bool doAddRef = true)
   {
    if(parms)
      parms->release();

    parms = p;

    if(parms && doAddRef)
      parms->addRef();
   }

   // --------------------------------------------
   IParmInfo *detach()
   {
    IParmInfo  *retVal = parms;


    parms = 0;

    return retVal;
   }

   // --------------------------------------------
   bool init(IProxyStubMgr *ps, int count, bool ret = false)
   {
    if(parms)
      parms->release();

    return ps->makeParmInfo(count, ret, codePage, &parms);
   }

   // --------------------------------------------
   bool init(TAppObj *obj, int count, bool ret = false)
   {
    IProxyStubMgr  *ps = fs_getPSmgrA(obj);

    return init(ps, count, ret);
   }

   // --------------------------------------------
   TValRef operator [] (int index)
   {
    // The 0 element is a retval
    return TValRef(parms, index + 1, codePage);
   }

   // --------------------------------------------
   TValRef retVal()
   {
    return TValRef(parms, 0, codePage);
   }

   // --------------------------------------------
   TValRef putVal()
   {
    return TValRef(parms, 0, codePage);
   }
   
   // --------------------------------------------
   int getCount()
   {
    return parms ? parms->getCount() - 1 : 0;
   }

   // --------------------------------------------
   operator IParmInfo * ()
   {
    return parms;
   }
};


// -----------------------------------------------------------------------------
class TBindObj : public TRsPtr<IDispBind>
{
   // --------------------------------------------
   // --------------------------------------------
   TRsCodePage  codePage;
 
 public:
   // --------------------------------------------
   TBindObj(TRsCodePage code = RSCOM_CODEPAGE) : codePage(code)
   {
   }

   // --------------------------------------------
   TBindObj(const char *names, IDispObj *obj, int autoGet = 0, TRsCodePage code = RSCOM_CODEPAGE) : codePage(code)
   {
    TRsPtrX<IGetBind, IID_RSGETBIND>  bndGet = obj;


    if(bndGet)
      bndGet->getBind(names, autoGet, &p);
   }

   // --------------------------------------------
   TBindObj(const char * const *names, int num, IDispObj *obj, int autoGet = 0, TRsCodePage code = RSCOM_CODEPAGE) : codePage(code)
   {
    TRsPtrX<IGetBind, IID_RSGETBIND>  bndGet = obj;


    if(bndGet)
      bndGet->getBind2(names, num, autoGet, &p);
   }

   // --------------------------------------------
   TValRef operator [] (int index)
   {
    return TValRef(p, index, codePage);
   }
};


// -----------------------------------------------------------------------------
inline TRsStat RsComHandleAdvise(TDispStubImpl *pObj, RSDISPID id, int cmdType, IParmInfo *pInfo, int level)
{
 if((id == RSDISP_ADVISE || id == RSDISP_UNADVISE) && level == RSCOM_SYS_LEVEL)
  {
   TParmArray                     prm(pInfo);
   TRsPtrX<IDispObj, IID_RSDISP>  p = (IRsObj *)prm[0];


   if(p)
     return id == RSDISP_ADVISE ? pObj->advise(p) : pObj->unAdvise(p);

   return RSL_STAT_GEN;
  }

 return RSL_STAT_UNKNOWNID;
}

#include <packpop.h>

#endif // __RSDATA_HPP

/* <-- EOF --> */