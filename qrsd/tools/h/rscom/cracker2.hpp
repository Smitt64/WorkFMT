// -------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Класс-обёртка для обхода конструкции (RsComSpecVal)p1).isOpt()
// при сборке под MSVS 2017
class CCastValRefToSpecVal
{
 private:
   RsComSpecVal  m_SpecVal;
   bool          m_isVal;

 public:
   // --------------------------------------------
   CCastValRefToSpecVal(TValRef &rValRef) : m_SpecVal(0)
   {
    m_isVal = false;

    if(rValRef.getType() == RSCOM_SPECVAL)
      if(rValRef.getDataVal(RSCOM_SPECVAL, &m_SpecVal) == RSL_STAT_OK)
        m_isVal = true;
   }

   // --------------------------------------------
   bool IsOpt(void)
   {
    return m_isVal ? m_SpecVal.isOpt() : false;
   }
};

// -----------------------------------------------------------------------------
template <class T, class P1, class P2 >
class Cracker2Def
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2);

   // --------------------------------------------
   Cracker2Def(T *objPtr, pmf_t p, P2 d1) : obj(objPtr), proc(p), def1(d1)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    int  count = ptr->getCount() - 1;
    TValRef  p1(ptr, 1);

    TRsStat  stat = (obj->*proc)(TValRef(ptr, 0), 
                     count < 1 || (p1.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p1).IsOpt()) ? def1 : (P2)p1);

    return stat;
   }

 private:
   pmf_t  proc;
   T     *obj;
   P2     def1;
};

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class D1>
inline Cracker2Def<T, P1, P2> RsCrackerDef(T *obj, TRsStat (T::*f)(P1 par, P2 par2), D1 def1)
{
 return Cracker2Def<T, P1, P2>(obj, f, def1);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3 >
class Cracker3Def
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3);

   // --------------------------------------------
   Cracker3Def(T *objPtr, pmf_t p, P2 d1, P3 d2) : obj(objPtr), proc(p), def1(d1), def2(d2) {}

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    int  count = ptr->getCount() - 1;
    TValRef  p1(ptr, 1);
    TValRef  p2(ptr, 2);
    TRsStat  stat = (obj->*proc)(TValRef(ptr, 0),
                     count < 1 || (p1.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p1).IsOpt()) ? def1 : (P2)p1,
                     count < 2 || (p2.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p2).IsOpt()) ? def2 : (P3)p2);

    return stat;
   }

 private:
   pmf_t  proc;
   T     *obj;
   P2     def1;
   P3     def2;
};


// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class D1, class D2>
inline Cracker3Def<T, P1, P2, P3> RsCrackerDef(T *obj, TRsStat (T::*f)(P1 par, P2 par2, P3 par3), D1 def1, D2 def2)
{
 return Cracker3Def<T, P1, P2, P3>(obj, f, def1, def2);
}


// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3>
class Cracker3Def1
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3);

   // --------------------------------------------
   Cracker3Def1(T *objPtr, pmf_t p, P3 d1) : obj(objPtr), proc(p), def1(d1)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    int  count = ptr->getCount() - 1;
    TValRef  p1(ptr, 1);
    TValRef  p2(ptr, 2);

    TRsStat  stat = (obj->*proc)(TValRef(ptr, 0), (P2)p1,
                                         count < 2 || ((p2.getType() == RSCOM_SPECVAL) && (CCastValRefToSpecVal(p2).IsOpt())) ? def1 : (P3)p2);

    return stat;
   }

 private:
   pmf_t  proc;
   T     *obj;
   P3     def1;
};

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class D1>
inline Cracker3Def1<T, P1, P2, P3> RsCrackerDef(T *obj, TRsStat (T::*f)(P1 par, P2 par2, P3 par3), D1 def1)
{
 return Cracker3Def1<T, P1, P2, P3>(obj, f, def1);
}


// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4>
class Cracker4Def
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3, P4 par4);

   // --------------------------------------------
   Cracker4Def(T *objPtr, pmf_t p, P2 d1, P3 d2, P4 d3) : obj(objPtr), proc(p), def1(d1), def2(d2), def3(d3)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    int  count = ptr->getCount () - 1;

    TValRef  p1(ptr, 1);
    TValRef  p2(ptr, 2);
    TValRef  p3(ptr, 3);

    TRsStat  stat = (obj->*proc)(TValRef(ptr, 0),
                     count < 1 || (p1.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p1).IsOpt()) ? def1 : (P2)p1,
                     count < 2 || (p2.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p2).IsOpt()) ? def2 : (P3)p2,
                     count < 3 || (p3.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p3).IsOpt()) ? def3 : (P4)p3);

    return stat;
   }

 private:
   pmf_t  proc;
   T     *obj;
   P2     def1;
   P3     def2;
   P4     def3;
};

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class D1, class D2, class D3>
inline Cracker4Def<T, P1, P2, P3, P4> RsCrackerDef(T *obj, TRsStat (T::*f)(P1 par, P2 par2, P3 par3, P4 par4), D1 def1, D2 def2, D3 def3)
{
 return Cracker4Def<T, P1, P2, P3, P4>(obj, f, def1, def2, def3);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5>
class Cracker5Def
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3, P4 par4, P5 par5);

   // --------------------------------------------
   Cracker5Def(T *objPtr, pmf_t p, P2 d1, P3 d2, P4 d3, P5 d4) :
              obj(objPtr), proc(p), def1(d1), def2(d2), def3(d3), def4(d4)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    int  count = ptr->getCount() - 1;
    TValRef  p1(ptr, 1);
    TValRef  p2(ptr, 2);
    TValRef  p3(ptr, 3);
    TValRef  p4(ptr, 4);

    TRsStat  stat = (obj->*proc)(TValRef(ptr, 0),
                     count < 1 || (p1.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p1).IsOpt()) ? def1 : (P2)p1,
                     count < 2 || (p2.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p2).IsOpt()) ? def2 : (P3)p2,
                     count < 3 || (p3.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p3).IsOpt()) ? def3 : (P4)p3,
                     count < 4 || (p4.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p4).IsOpt()) ? def4 : (P5)p4);

    return stat;
   }

 private:
   pmf_t  proc;
   T     *obj;
   P2     def1;
   P3     def2;
   P4     def3;
   P5     def4;
};

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5, class D1, class D2, class D3, class D4>
inline Cracker5Def<T, P1, P2, P3, P4, P5> RsCrackerDef(T *obj, TRsStat (T::*f)(P1 par, P2 par2, P3 par3, P4 par4, P5 par5), D1 def1, D2 def2, D3 def3, D4 def4)
{
 return Cracker5Def<T, P1, P2, P3, P4, P5>(obj, f, def1, def2, def3, def4);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5, class P6>
class Cracker6Def
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3, P4 par4, P5 par5, P6 par6);

   // --------------------------------------------
   Cracker6Def(T *objPtr, pmf_t p, P2 d1, P3 d2, P4 d3, P5 d4, P6 d5) :
              obj(objPtr), proc(p), def1(d1), def2(d2), def3(d3), def4(d4), def5(d5)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    int  count = ptr->getCount() - 1;
    TValRef  p1(ptr, 1);
    TValRef  p2(ptr, 2);
    TValRef  p3(ptr, 3);
    TValRef  p4(ptr, 4);
    TValRef  p5(ptr, 5);
    TRsStat  stat = (obj->*proc)(TValRef(ptr, 0),
                     count < 1 || (p1.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p1).IsOpt()) ? def1 : (P2)p1,
                     count < 2 || (p2.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p2).IsOpt()) ? def2 : (P3)p2,
                     count < 3 || (p3.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p3).IsOpt()) ? def3 : (P4)p3,
                     count < 4 || (p4.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p4).IsOpt()) ? def4 : (P5)p4,
                     count < 5 || (p5.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p5).IsOpt()) ? def5 : (P6)p5);

    return stat;
   }

 private:
   pmf_t  proc;
   T     *obj;
   P2     def1;
   P3     def2;
   P4     def3;
   P5     def4;
   P6     def5;
};

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class D1, class D2, class D3, class D4, class D5>
inline Cracker6Def<T, P1, P2, P3, P4, P5, P6> RsCrackerDef(T *obj, TRsStat (T::*f)(P1 par, P2 par2, P3 par3, P4 par4, P5 par5, P6 par6), D1 def1, D2 def2, D3 def3, D4 def4, D5 def5)
{
 return Cracker6Def<T, P1, P2, P3, P4, P5, P6>(obj, f, def1, def2, def3, def4, def5);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
class Cracker7Def
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3, P4 par4, P5 par5, P6 par6, P7 par7);

   // --------------------------------------------
   Cracker7Def(T *objPtr, pmf_t p, P2 d1, P3 d2, P4 d3, P5 d4, P6 d5, P7 d6) :
              obj(objPtr), proc(p), def1(d1), def2(d2), def3(d3), def4(d4), def5(d5), def6(d6)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    int  count = ptr->getCount() - 1;
    TValRef  p1(ptr, 1);
    TValRef  p2(ptr, 2);
    TValRef  p3(ptr, 3);
    TValRef  p4(ptr, 4);
    TValRef  p5(ptr, 5);
    TValRef  p6(ptr, 6);

    TRsStat  stat = (obj->*proc)(TValRef(ptr, 0),
                     count < 1 || (p1.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p1).IsOpt()) ? def1 : (P2)p1,
                     count < 2 || (p2.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p2).IsOpt()) ? def2 : (P3)p2,
                     count < 3 || (p3.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p3).IsOpt()) ? def3 : (P4)p3,
                     count < 4 || (p4.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p4).IsOpt()) ? def4 : (P5)p4,
                     count < 5 || (p5.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p5).IsOpt()) ? def5 : (P6)p5,
                     count < 6 || (p6.getType() == RSCOM_SPECVAL && CCastValRefToSpecVal(p6).IsOpt()) ? def6 : (P7)p6);

    return stat;
   }

 private:
   pmf_t  proc;
   T     *obj;
   P2     def1;
   P3     def2;
   P4     def3;
   P5     def4;
   P6     def5;
   P7     def6;
};

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7,
                   class D1, class D2, class D3, class D4, class D5, class D6>
inline Cracker7Def<T, P1, P2, P3, P4, P5, P6, P7> RsCrackerDef(T *obj, TRsStat (T::*f)(P1 par, P2 par2, P3 par3, P4 par4, P5 par5, P6 par6, P7 par7), D1 def1, D2 def2, D3 def3, D4 def4, D5 def5, D6 def6)
{
 return Cracker7Def<T, P1, P2, P3, P4, P5, P6, P7>(obj, f, def1, def2, def3, def4, def5, def6);
}

/* <-- EOF --> */