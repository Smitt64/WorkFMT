/*******************************************************************************
 FILE         :   RSFLOAT.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   Floating types with arbitrary alignment

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   31 Jan 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsfloat.h $
                  $Revision: 4 $
                  $Date: 15.05.00 13:52 $
                  $Author: Lebedev $
                  $Modtime: 5.05.00 17:13 $
*******************************************************************************/

#ifndef __RSFLOAT_H
#define __RSFLOAT_H

#ifdef __cplusplus

#include "platform.h"

#ifdef RSL_PL_WIN64
#include <string.h>
#endif

template <class Type> class DBFloat;

template <class Type> class DBBaseFloat
{
 public:
   operator Type() const
   {
    Type  tmp;
/* [!!!:x64]
    #ifdef RSL_PL_WIN64
        memcpy (&tmp, data, sizeof(Type));
    #else
*/
        revers(&tmp, this, sizeof(Type));
//    #endif

    return tmp;
   }

   DBBaseFloat<Type> operator ++ ()
   {
    return *this = DBFloat<Type>(Type(*this) + 1);
   }

   DBBaseFloat<Type> operator -- ()
   {
    return *this = DBFloat<Type>(Type(*this) - 1);
   }

   Type operator++ (int)
   {
    Type  tmp = *this;

    *this = DBFloat<Type>(tmp + 1);

    return tmp;
   }

   Type operator -- (int)
   {
    Type  tmp = *this;

    *this = DBFloat<Type>(tmp - 1);

    return tmp;
   }


   #define  RSL_DB_FLOAT_OPASSIGN(_OP)  DBBaseFloat<Type> &operator _OP##= (Type val)          \
                                        {                                                      \
                                         return *this = DBFloat<Type>(Type(*this) _OP val);    \
                                        }

   RSL_DB_FLOAT_OPASSIGN(+)
   RSL_DB_FLOAT_OPASSIGN(-)
   RSL_DB_FLOAT_OPASSIGN(*)
   RSL_DB_FLOAT_OPASSIGN(/)

 protected:
   char  data[sizeof(Type)];

// [!!!:x64]
//   #ifndef RSL_PL_WIN64
       static void revers(void *dst, const void *src, int size)
       {
        char  *p = (char *)dst;
        const char  *q = (const char *)src + size - 1;
        int  i;

        for(i = 0; i < size; i++)
           p[i] = q[-i];
       }
//   #endif
};

// -----------------------------------------------
template <class Type>
class DBFloat : public DBBaseFloat<Type>
    {
     public:
       DBFloat<Type>() {}

       DBFloat<Type>(const Type &num)
       {
        #ifdef RSL_PL_WIN64
            memcpy (&data, &num, sizeof(Type));
        #else
            revers(this, &num, sizeof(Type));
        #endif
       }

       DBFloat<Type>(const DBBaseFloat<Type> &base)
       {
        *this = *(DBFloat<Type> *)&base;
       }
    };

#define DB_FLOAT_TYPES

typedef DBFloat<float>        db_float;
typedef DBFloat<double>       db_double;

typedef DBBaseFloat<float>    db_basefloat;
typedef DBBaseFloat<double>   db_basedouble;

#if defined(_MSC_VER) && (_MSC_VER > 1300)
    // error C2677: binary '+=' : no global operator found which takes type 'db_float'
    // (or there is no acceptable conversion)
    inline db_float __instance_db_float() { return db_float(0); }

    // error C2677: binary '+=' : no global operator found which takes type 'db_double'
    // (or there is no acceptable conversion)
    inline db_double __instance_db_double() { return db_double(0); }
#endif

#endif // __cplusplus

#endif // __RSFLOAT_H

/* <-- EOF --> */