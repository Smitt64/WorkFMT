/*******************************************************************************
 FILE         :   RSDIGIT.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   Integers types with arbitrary alignment and CPU architecture conversions

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   31 Jan 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsdigit.h $
                  $Revision: 3 $
                  $Date: 13.03.00 19:58 $
                  $Author: Lebedev $
                  $Modtime: 13.03.00 18:19 $
*******************************************************************************/

#ifndef __RSDIGIT_H
#define __RSDIGIT_H

#ifdef __cplusplus

#include "platform.h"

#include "int64.h"

#ifdef RSL_PL_WIN64
#include <string.h>
#endif

#include <systypes.h>
#include <swapends.h>

// -----------------------------------------------------------------------------
template <class Type, class Result>  class db_digit;

// -----------------------------------------------------------------------------
template <class Type, class Result>  class db_basedigit
{
 public:
   // --------------------------------------------
   operator Result() const
   {
    Type  val;
/* [!!!:x64]
    #ifdef RSL_PL_WIN64
        memcpy(&val, data, sizeof(Type));
    #else
*/
        swapEnds(val, *(Type *)data);
//    #endif

    return Result(val);
   }

   // --------------------------------------------
   db_basedigit<Type, Result> operator ++ ()
   {
    return *this = db_digit<Type, Result>(Type(*this) + 1);
   }

   // --------------------------------------------
   db_basedigit<Type, Result> operator -- ()
   {
    return *this = db_digit<Type, Result>(Type(*this) - 1);
   }

   // --------------------------------------------
   Result operator ++ (int)
   {
    Type  tmp = *this;

    *this = db_digit<Type, Result>(tmp + 1);

    return Result(tmp);
   }

   // --------------------------------------------
   Result operator -- (int)
   {
    Type  tmp = *this;

    *this = db_digit<Type, Result>(tmp - 1);

    return Result(tmp);
   }

   #define  RSL_DB_DIGIT_OPASSIGN(_OP) \
               db_basedigit<Type, Result> operator _OP##= (int val)           \
               {                                                              \
                return *this = db_digit<Type, Result>(Type(*this) _OP val);   \
               }

   RSL_DB_DIGIT_OPASSIGN(+)
   RSL_DB_DIGIT_OPASSIGN(-)
   RSL_DB_DIGIT_OPASSIGN(*)
   RSL_DB_DIGIT_OPASSIGN(/)
   RSL_DB_DIGIT_OPASSIGN(%)

   RSL_DB_DIGIT_OPASSIGN(|)
   RSL_DB_DIGIT_OPASSIGN(&)
   RSL_DB_DIGIT_OPASSIGN(^)
   RSL_DB_DIGIT_OPASSIGN(<<)
   RSL_DB_DIGIT_OPASSIGN(>>)

 protected:
   // --------------------------------------------
   // --------------------------------------------
   char  data[sizeof(Type)];
};

// -----------------------------------------------------------------------------
template <class Type, class Result>
class db_digit : public db_basedigit<Type, Result>
{
 public:
   // --------------------------------------------
   db_digit() {}

   // --------------------------------------------
   db_digit(Type val)
   {
/* [!!!:x64]
    #ifdef RSL_PL_WIN64
        memcpy(data, &val, sizeof(Type));
    #else
*/
        swapEnds((Type &)*this, val);
//    #endif
   }

   // --------------------------------------------
   db_digit(db_basedigit<Type, Result> base)
   {
    *this = *(db_digit<Type, Result> *)&base;
   }

   // --------------------------------------------
   db_digit<Type, Result> operator = (Type val)
   {
/* [!!!:x64]
    #ifdef RSL_PL_WIN64
        memcpy(data, &val, sizeof(Type));
    #else
*/
        swapEnds((Type&)*this, val);
//    #endif

    return *this;
   }

   // --------------------------------------------
   db_digit<Type, Result> operator = (db_basedigit<Type, Result> base)
   {
    *this = *(db_digit<Type, Result> *)&base;

    return *this;
   }
};

#define  DB_DIGIT_TYPES

typedef db_digit<int16,     signed short>         db_int16;
typedef db_digit<int32,     signed long>          db_int32;
typedef db_digit<int64_t,   signed __int64>       db_int64;
typedef db_digit<uint16,    unsigned short>       db_uint16;
typedef db_digit<uint32,    unsigned long>        db_uint32;
typedef db_digit<uint64_t,  unsigned __int64>     db_uint64;

typedef db_basedigit<int16,    signed short>      db_baseint16;
typedef db_basedigit<int32,    signed long>       db_baseint32;
typedef db_basedigit<int64_t,  signed __int64>    db_baseint64;
typedef db_basedigit<uint16,   unsigned short>    db_ubaseint16;
typedef db_basedigit<uint32,   unsigned long>     db_ubaseint32;
typedef db_basedigit<uint64_t, unsigned __int64>  db_ubaseint64;
         
#endif // __cplusplus

#endif // RSDIGIT.H

/* <-- EOF --> */