//-*-----------------------------------------------------------------------*-
//
// File Name   : iparms.hpp
//
// Sybsystem   :   
// Description : 
//
// Source      : 
//
// Library(s)  : 
//
// Copyright (c) 1991 - 2002 by R-Style Softlab.
// All Rights Reserved.
//
//-*- History -------------------------------------------------------------*-
// August 2,2004  Sergei Kubrin (K78) - Create file
//-*-----------------------------------------------------------------------*-
#ifndef __IPARMS_HPP
#define __IPARMS_HPP

// -----------------------------------------------------------------------------
class TRSLValArray;
class TRSLValHolder;

// -----------------------------------------------------------------------------
class TRSLValServerIntf
{
 public:
   virtual void makeValue   (VALUE *v)                                                   = 0;
   virtual void copyValue   (const VALUE *from, VALUE *to)                               = 0;
   virtual void clearValue  (VALUE *v)                                                   = 0;
   virtual void setValue    (VALUE *v,VALTYPE v_type, const void *ptr )                  = 0;
   virtual bool setValueAs  (VALUE *v,VALTYPE needType, VALTYPE v_type, const void *ptr) = 0;
   virtual void setValueId  (int id,VALTYPE v_type, const void *ptr )                    = 0;
   virtual bool setValueAsId(int id,VALTYPE needType, VALTYPE v_type, const void *ptr)   = 0;
   virtual bool getValue    (VALUE *v,VALTYPE tp,void *data)                             = 0;
   virtual bool cvtToValue  (VALUE *v, VALTYPE tp)                                       = 0;
};

// -----------------------------------------------------------------------------
class TRSLValRef
{
   // --------------------------------------------
   // --------------------------------------------
   VALUE               *m_val;
   TRSLValServerIntf   *host;
   char               **cvtBuff;
   int                  id;

 protected:
   // --------------------------------------------
   TRSLValRef(TRSLValServerIntf *host_,VALUE *val, char **cvtP, int id_)
     : host(host_), m_val(val), cvtBuff(cvtP), id(id_)
   {
   }

 public:
   // --------------------------------------------
   void clear()
   {
    host->clearValue(m_val);
   }

   // --------------------------------------------
   void set(VALTYPE v_type, const void *ptr)
   {
    if(cvtBuff && *cvtBuff)
     {
      free(*cvtBuff);

      *cvtBuff = NULL;
     }

    host->clearValue(m_val);

    if(id != -1)
      host->setValueId(id, v_type, ptr);
    else 
      host->setValue(m_val, v_type, ptr);
   }

   // --------------------------------------------
   void setAs(VALTYPE needType, VALTYPE v_type, const void *ptr)
   {
    if(cvtBuff && *cvtBuff)
     {
      free(*cvtBuff);

      *cvtBuff = NULL;
     }

    host->clearValue(m_val);

    bool  res = (id == -1 ? host->setValueAs(m_val, needType, v_type, (void *)ptr) :
                            host->setValueAsId(id, needType, v_type, (void *)ptr));

    if(!res)
      throw RslConvertException();
   }

   // --------------------------------------------
   VALUE *getValue()
   {
    return m_val;
   }
       
   // --------------------------------------------
   VALTYPE getType()
   {
    return m_val->v_type;
   }

   // --------------------------------------------
   bool cvtTo(VALTYPE tp)
   {
    return host->cvtToValue(m_val, tp);
   }

   // --------------------------------------------
   TRSLValRef &operator= (const TRSLValRef &val)         
   {           
    host->copyValue(val.m_val, m_val);

    return *this;
   } 

   // --------------------------------------------
   TRSLValRef &operator= (const VALUE *val)         
   {           
    host->copyValue(val, m_val);

    return *this;
   }

   // --------------------------------------------
   TRSLValRef &operator= (int val)         { return operator = ((long)val); }
   TRSLValRef &operator= (long val)        { set (V_INTEGER, &val); return *this; }
   TRSLValRef &operator= (int64_t val)     { set (V_BIGINT,  &val); return *this; }
   TRSLValRef &operator= (double val)      { set (V_DOUBLE,  &val); return *this; }
   TRSLValRef &operator= (long double val) { set (V_DOUBLEL, &val); return *this; }
   TRSLValRef &operator= (const char *val) { set (V_STRING,   val); return *this; }
   TRSLValRef &operator= (bool val)        { set (V_BOOL,    &val); return *this; }
   TRSLValRef &operator= (bdate val)       { set (V_DATE,    &val); return *this; }
   TRSLValRef &operator= (btime val)       { set (V_TIME,    &val); return *this; }

   // --------------------------------------------   
   TRSLValRef& operator= (TGenObject *val) 
   {
    if(val)
     {
      set(V_GENOBJ, val);

      ADDREF(val);
     }
    else
      clear();

    return *this; 
   }

   // --------------------------------------------
   void setAs(VALTYPE needType, int val)         { setAs(needType, (long)val); }
   void setAs(VALTYPE needType, long val)        { setAs(needType, V_INTEGER, &val); }
   void setAs(VALTYPE needType, int64_t val)     { setAs(needType, V_BIGINT,  &val); }
   void setAs(VALTYPE needType, double val)      { setAs(needType, V_DOUBLE,  &val); }
   void setAs(VALTYPE needType, long double val) { setAs(needType, V_DOUBLEL, &val); }
   void setAs(VALTYPE needType, char *val)       { setAs(needType, V_STRING,   val); }
   void setAs(VALTYPE needType, bool val)        { setAs(needType, V_BOOL,    &val); }
   void setAs(VALTYPE needType, bdate val)       { setAs(needType, V_DATE,    &val); }
   void setAs(VALTYPE needType, btime val)       { setAs(needType, V_TIME,    &val); }

#ifdef USE_NUMERIC
   // --------------------------------------------
   TRSLValRef &operator= (const Numeric   &val)  { set(V_NUMERIC, &val); return *this; }
   TRSLValRef &operator= (const Numeric_t &val)  { set(V_NUMERIC, &val); return *this; }

   // --------------------------------------------   
   void setAs(VALTYPE needType, const Numeric   &val) { setAs(needType, V_NUMERIC, &val); }
   void setAs(VALTYPE needType, const Numeric_t &val) { setAs(needType, V_NUMERIC, &val); }
#endif

#ifdef USE_FDECIMAL
   // --------------------------------------------
   TRSLValRef &operator= (const FDecimal_t &val)
   {
#ifdef SQLBUILD
    setAs(V_RSLMONEY, V_MONEY_FDEC, &val);
#else
    set(V_DECIMAL, &val);
#endif

    return *this;
   }

   // --------------------------------------------   
   TRSLValRef &operator= (const FMoney &val)
   {
    FDecimal_t  fdec;


    val.get(&fdec);

    setAs(V_RSLMONEY, V_MONEY_FDEC, &fdec);

    return *this;
   }

   // --------------------------------------------   
   TRSLValRef &operator= (const FDecimal &val)
   {
    FDecimal_t  fdec;


    val.get(&fdec);

#ifdef SQLBUILD
    setAs(V_RSLMONEY, V_MONEY_FDEC, &fdec);
#else
    set(V_DECIMAL, &fdec);
#endif

    return *this;
   }

   // --------------------------------------------   
   void setAs(VALTYPE needType, const FDecimal_t &val)
   {
    setAs(needType, V_DECIMAL, &val);
   }

   // --------------------------------------------   
   void setAs(VALTYPE needType, const FMoney &val)
   {
    FDecimal_t  fdec;


    val.get(&fdec);

    setAs(needType, V_MONEY_FDEC, &val);
   }

   // --------------------------------------------   
   void setAs(VALTYPE needType, const FDecimal &val)
   {
    FDecimal_t  fdec;


    val.get(&fdec);

#ifdef SQLBUILD
    setAs(needType, V_MONEY_FDEC, &fdec);
#else
    setAs(needType, V_DECIMAL, &fdec);
#endif
   }
#endif

   // --------------------------------------------
   void getDataVal(VALTYPE tp, void *data)
   {
    if(!host->getValue(m_val, tp, data))
      throw RslConvertException();
   }

   // --------------------------------------------
   operator VALUE *()
   {
    return getValue();
   }

   // --------------------------------------------
   operator bool ()
   {
    bool  retVal;


    getDataVal(V_BOOL, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator short int ()
   {
    return (short int)operator long ();
   }

   // --------------------------------------------   
   operator unsigned short ()
   {
    return (unsigned short)operator long ();
   }

   // --------------------------------------------
   operator int ()
   {
    return operator long ();
   }

   // --------------------------------------------
   operator unsigned int ()
   {
    return operator unsigned long ();
   }

   // --------------------------------------------
   operator long ()
   {
    long  retVal;


    getDataVal(V_INTEGER, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator unsigned long ()
   {
    return operator long ();
   }

   // --------------------------------------------
   operator int64_t ()
   {
    int64_t  retVal;


    getDataVal(V_BIGINT, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator uint64_t ()
   {
    return operator int64_t ();
   }

   // --------------------------------------------
   operator double ()
   {
    double  retVal;


    getDataVal(V_DOUBLE, &retVal);

    return retVal;
   }

   // --------------------------------------------
   operator const char *()
   {
    if(getType() == V_STRING)
      return m_val->value.string;

    char  *retVal = "";


    if(cvtBuff)
     {
      if(*cvtBuff)
        retVal = *cvtBuff;
      else
       {
        VALUE  val;


        host->makeValue(&val);
        host->copyValue(m_val, &val);

        if(host->cvtToValue(&val, V_STRING))
         {
          *cvtBuff = (char *)malloc(strlen(val.value.string) + 1);

          if(*cvtBuff)
           {
            strcpy(*cvtBuff, val.value.string);

            retVal = *cvtBuff;
           }
         }

        host->clearValue(&val);
       }
     }

    return retVal;
   }

   // --------------------------------------------
   operator TGenObject *()
   {
    TGenObject  *obj;


    getDataVal(V_GENOBJ, &obj);

    return obj;
   }

   // --------------------------------------------
   operator bdate ()
   {
    bdate  bd;


    memset(&bd, 0, sizeof(bdate));

    getDataVal(V_DATE, &bd);

    return bd;
   }

   // --------------------------------------------
   operator btime ()
   {
    btime  tm;


    memset(&tm, 0, sizeof(btime));

    getDataVal(V_TIME, &tm);

    return tm;
   }

   // --------------------------------------------   
   operator btimestamp ()
   {
    btimestamp  tm;


    memset(&tm, 0, sizeof(btimestamp));

    getDataVal(V_DTTM, &tm);

    return tm;
   }

#ifdef USE_FDECIMAL
   // --------------------------------------------
   operator FDecimal_t ()
   {
    FDecimal_t  tm;


    memset(&tm, 0, sizeof(FDecimal_t));

    getDataVal(V_DECIMAL, &tm);

    return tm;
   }

   // --------------------------------------------
   operator FDecimal ()
   {
    FDecimal_t  tm;

    
    memset(&tm, 0, sizeof(FDecimal_t));

    getDataVal(V_DECIMAL, &tm);

    return FDecimal(tm);
   }

   // --------------------------------------------
   operator FMoney ()
   {
    FDecimal_t  tm;


    memset(&tm, 0, sizeof(FDecimal_t));

    getDataVal(V_MONEY_FDEC, &tm);

    return FMoney(tm);
   }
#endif

#ifdef USE_NUMERIC
   // --------------------------------------------
   operator Numeric_t ()
   {
    Numeric_t  val;


    getDataVal(V_NUMERIC, &val);

    return val;
   }

   // --------------------------------------------
   operator Numeric ()
   {
    Numeric  val;


    getDataVal(V_NUMERIC, &val);

    return val;
   }
#endif

   friend class TRSLValArray;
   friend class TRSLValHolder;

   friend class TRSLPrm;
   friend class TRSLVal;
};


// @H -------------------------------------------------------------------

class TRSLValArrayBase
{
 protected:
   // --------------------------------------------
   TRSLValArrayBase() : cvtBuff(0)
   {
   }

   // --------------------------------------------
  ~TRSLValArrayBase()
   {
    if(cvtBuff)
      free(cvtBuff);
   }

   // --------------------------------------------
   void init(size_t size, bool reserveStringCvtBuf)
   {
    if(cvtBuff)
     {
      free(cvtBuff);

      cvtBuff = NULL;
     }

    m_size = size;

    if(reserveStringCvtBuf)
     {
      cvtBuff = (char **)malloc(sizeof(char **)*(m_size));

      if(cvtBuff)
        for(size_t  i = 0; i < m_size; ++i)
           cvtBuff[i] = 0;
     }
   }

   // --------------------------------------------
   void clear()
   {
    if(cvtBuff)
      for(size_t  i = 0; i < m_size; ++i)
         if(cvtBuff[i])
           free(cvtBuff[i]);
   }


   // --------------------------------------------
   char **getCvtBuff(size_t index)
   {
    if(cvtBuff && index < m_size)
      return cvtBuff + index;

    return 0;
   }


   // --------------------------------------------
   // --------------------------------------------
   char         **cvtBuff;
   size_t         m_size;
};

// -----------------------------------------------------------------------------
class TRSLValServerImpl1 : public TRSLValServerIntf
{
 public:
   // --------------------------------------------
   TRSLValServerImpl1(HRSLINST inst) : m_inst(inst)
   {
    m_intf = GetInterpreterInterface()->GetValueInterface();
   }

   // --------------------------------------------
   virtual void makeValue(VALUE *v)
   {
    m_intf->Make(m_inst, v);
   }

   // --------------------------------------------   
   virtual void copyValue(const VALUE *from, VALUE *to)
   {
    m_intf->Copy(m_inst, (VALUE *)from, to);
   }

   // --------------------------------------------   
   virtual void clearValue(VALUE *v)
   {
    m_intf->Clear(m_inst, v);
   }

   // --------------------------------------------   
   virtual void setValue(VALUE *v, VALTYPE v_type, const void *ptr)
   {
    m_intf->Set(m_inst, v, v_type, (void *)ptr);
   }

   // --------------------------------------------   
   virtual bool setValueAs(VALUE *v, VALTYPE needType, VALTYPE v_type, const void *ptr)
   {
    return m_intf->SetAs(m_inst, v, needType, v_type, (void *)ptr);
   }

   // --------------------------------------------   
   virtual void setValueId(int id, VALTYPE v_type, const void *ptr )
   {
   }

   // --------------------------------------------   
   virtual bool setValueAsId(int id, VALTYPE needType, VALTYPE v_type, const void *ptr)
   {
    return false;
   }

   // --------------------------------------------   
   virtual bool getValue(VALUE *v, VALTYPE tp, void *data)
   {
    return m_intf->Get(m_inst, v, tp, data);
   }

   // --------------------------------------------   
   virtual bool cvtToValue(VALUE *v, VALTYPE tp)
   {
    return m_intf->CvtTo(m_inst, v, tp);
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   HRSLINST        m_inst;
   TRSLValueIntf  *m_intf;
};

// -----------------------------------------------------------------------------
class TRSLValArray : public TRSLValServerImpl1, public TRSLValArrayBase
{
 public:
   // --------------------------------------------
   TRSLValArray(HRSLINST inst, size_t sz, bool reserveStringCvtBuf = true, bool reverse = true) throw (NoMemException)
     : TRSLValServerImpl1(inst), m_ptrArray(0)
   {
    init(sz, reserveStringCvtBuf, reverse);
   }

   // --------------------------------------------
  ~TRSLValArray()
   {
    clear();

    if(m_ptrArray)
      free(m_ptrArray);
   }

   // --------------------------------------------
   void clear()
   {
    if(m_ptrArray)
      for(size_t  i = 0; i < m_size; ++i)
         m_intf->Clear(m_inst, &m_ptrArray[i]);

    TRSLValArrayBase::clear();
   }

   // --------------------------------------------
   void init(size_t size, bool reserveStringCvtBuf = true, bool reverse = true)
   {
    clear();

    if(m_ptrArray)
      free(m_ptrArray);

    TRSLValArrayBase::init(size + 1, reserveStringCvtBuf);

    isRevers   = reverse;
    m_ptrArray = (VALUE *)malloc(sizeof(VALUE)*(m_size));

    if(!m_ptrArray)
      throw NoMemException();

    for(size_t  i = 0; i < m_size; ++i)
       m_intf->Make(m_inst, &m_ptrArray[i]);

   }

   // --------------------------------------------
   TRSLValRef operator [] (int index)
   {
    size_t  id = isRevers ? m_size - index - 1 : index + 1;

    return TRSLValRef(this, getValue(id), getCvtBuff(id), -1);
   }

   // --------------------------------------------
   VALUE *getValue(size_t index) throw (RslIndexException)
   {
    if(index < 0 || index >= m_size)  // the 0 element is retOrPutVal;
      throw RslIndexException();

    return m_ptrArray + index;
   }

   // --------------------------------------------
   operator VALUE *()
   {
    return m_ptrArray + 1;
   }

   // --------------------------------------------   
   VALUE *retOrPut()
   {
    return m_ptrArray;
   }

   // --------------------------------------------
   size_t getCount() const
   {
    return m_size - 1;
   }

   // --------------------------------------------
   TRSLValRef retVal() { return TRSLValRef(this, m_ptrArray, getCvtBuff(0), -1); }
   TRSLValRef putVal() { return TRSLValRef(this, m_ptrArray, getCvtBuff(0), -1); }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   VALUE   *m_ptrArray;
   bool     isRevers;
};

// -----------------------------------------------------------------------------
class TRSLValHolder : public TRSLValServerImpl1
{
 public:
   // --------------------------------------------
   TRSLValHolder(HRSLINST inst) : TRSLValServerImpl1(inst), m_val(0), cvtBuff(0)
   {
   }

   // --------------------------------------------
   TRSLValHolder(HRSLINST inst, VALUE *valPtr) : TRSLValServerImpl1(inst), m_val(valPtr), cvtBuff(0)
   {
   }

   // --------------------------------------------
   TRSLValHolder(const TRSLValHolder &obj) : TRSLValServerImpl1(obj), cvtBuff(0), m_val(obj.m_val)
   {
   }

   // --------------------------------------------
  ~TRSLValHolder()
   {
    if(cvtBuff)
      free(cvtBuff);
   }

   // --------------------------------------------
   TRSLValHolder &operator= (const TRSLValHolder &obj)
   {
    if(&obj != this)
     {
      m_val = obj.m_val;

      if(cvtBuff)
        free(cvtBuff);

      cvtBuff = 0;
     }

    return *this;
   }

   // --------------------------------------------
   TRSLValHolder &operator= (VALUE *valPtr)         
   {
    m_val = valPtr;

    if(cvtBuff)
      free(cvtBuff);

    cvtBuff = 0;

    return *this;
   }

   // --------------------------------------------
   operator VALUE *()
   {
    return m_val;
   }

   // --------------------------------------------
   TRSLValRef val()
   {
    return TRSLValRef(this, m_val, &cvtBuff, -1);
   }

   // --------------------------------------------   
   TRSLValRef operator [] (int index)
   {
    return val();
   }

   // --------------------------------------------
   TRSLValRef operator () ()
   {
    return val();
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   VALUE  *m_val;
   char   *cvtBuff;
};
#endif // __IPARMS_HPP

/* <-- EOF --> */