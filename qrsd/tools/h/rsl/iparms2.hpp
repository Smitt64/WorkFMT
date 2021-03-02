//-*-----------------------------------------------------------------------*-
//
// File Name   : iparms2.hpp
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
// August 11,2004  Sergei Kubrin (K78) - Create file
//-*-----------------------------------------------------------------------*-
#ifndef __IPARMS2_HPP
#define __IPARMS2_HPP

// -----------------------------------------------------------------------------
class TRSLValServerImpl2 : public TRSLValServerIntf
{
 public:
   // ----------------------------------------
   virtual void makeValue(VALUE *v)
   {
    ValueMake(v);
   }

   // ----------------------------------------
   virtual void copyValue(const VALUE *from, VALUE *to)
   {
    ValueCopy((VALUE *)from, to);
   }

   // ----------------------------------------
   virtual void clearValue(VALUE *v)
   {
    ValueClear(v);
   }

   // ----------------------------------------
   virtual void setValue(VALUE *v, VALTYPE v_type, const void *ptr )
   {
    ValueSet(v, v_type, (void *)ptr);
   }

   // ----------------------------------------
   virtual bool setValueAs(VALUE *v, VALTYPE needType, VALTYPE v_type, const void *ptr)
   {
    return ValueSetAs(v, needType, v_type, (void *)ptr);
   }

   // ----------------------------------------
   virtual void setValueId(int id, VALTYPE v_type, const void *ptr )
   {
    SetParm(id, v_type, (void *)ptr);
   }

   // ----------------------------------------
   virtual bool setValueAsId(int id, VALTYPE needType, VALTYPE v_type, const void *ptr)
   {
    return SetParmAs(id, needType, v_type, (void *)ptr);
   }

   // ----------------------------------------
   virtual bool getValue(VALUE *v, VALTYPE tp, void *data)
   {
    return ValueGet(v, tp, data);
   }

   // ----------------------------------------
   virtual bool cvtToValue(VALUE *v, VALTYPE tp)
   {
    return CnvType(v, tp);
   }
};

// -----------------------------------------------------------------------------
class TRSLPrm : public TRSLValServerImpl2, public TRSLValArrayBase
{
 public:
   // --------------------------------------------
   TRSLPrm(VALUE *prop = 0, bool reserveStringCvtBuf = true, int maxCount = 0) : propVal(prop)
   {
    int  count = GetNumParm();


    if(maxCount > count)
      count = maxCount;

    init(count + 2, reserveStringCvtBuf);
   }

   // --------------------------------------------
  ~TRSLPrm()
   {
    clear();
   }

   // --------------------------------------------
   TRSLValRef operator [] (int index)
   {
    VALUE  *v;


    GetParm(index, &v);

    return TRSLValRef(this, v, getCvtBuff(index + 2), index);
   }

   // --------------------------------------------
   size_t getCount() const
   {
    return m_size - 2;
   }

   // --------------------------------------------
   TRSLValRef retVal() { return TRSLValRef(this, GetReturnVal(), getCvtBuff(0), -1); }
   TRSLValRef putVal() { return TRSLValRef(this, propVal, getCvtBuff(1), -1); }
   TRSLValRef getVal() { return TRSLValRef(this, propVal, getCvtBuff(1), -1); }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   VALUE  *propVal;
};

// -----------------------------------------------------------------------------
class TRSLVal : public TRSLValServerImpl2
{
 public:
   // --------------------------------------------
   TRSLVal() : m_val(0), cvtBuff(0) {}

   // --------------------------------------------
   TRSLVal(VALUE *valPtr) : m_val(valPtr), cvtBuff(0) {}

   // --------------------------------------------
   TRSLVal(const TRSLVal &obj) : cvtBuff(0), m_val(obj.m_val) {}

   // --------------------------------------------
  ~TRSLVal()
   {
    if(cvtBuff)
      free(cvtBuff);
   }

   // --------------------------------------------
   TRSLVal &operator = (const TRSLVal &obj)
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
   TRSLVal &operator = (VALUE *valPtr)         
   {
    m_val = valPtr;

    if(cvtBuff)
      free(cvtBuff);

    cvtBuff = 0;

    return *this;
   }

   // --------------------------------------------
   operator VALUE * ()
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
#endif // __IPARMS2_HPP

/* <-- EOF --> */