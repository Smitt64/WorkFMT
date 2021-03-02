/*******************************************************************************
 FILE         :   RSLDOUBL.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   Long double type with 10 bytes size.

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   31 Jan 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsldoubl.h $
                  $Revision: 11 $
                  $Date: 15.05.00 13:52 $
                  $Author: Lebedev $
                  $Modtime: 5.05.00 17:14 $
*******************************************************************************/

#ifndef __RSLDOUBL_H
#define __RSLDOUBL_H

#include "tooldef.h"

#ifdef __cplusplus
 extern "C" {
#endif

_TOOLEXP void   ToLDouble(double num, void *ptr); // void *ptr
_TOOLEXP double ToDouble(const void *ptr);        // и const void *ptr указатели на db_double10

#ifdef __cplusplus
 };
#endif


#if !defined(RSL_RESTRICT_LDBL)

    #if defined(__cplusplus)

        /*******************************************************************************
         DBDouble10
         DBBaseDouble10
        *******************************************************************************/
        class DBBaseDouble10
            {
             public:
               operator double() const
               {
                return ToDouble(this);
               }

               DBBaseDouble10 &operator ++ ()
               {
                ToLDouble(ToDouble(this) + 1, this);

                return *this;
               }

               DBBaseDouble10 &operator -- ()
               {
                ToLDouble(ToDouble(this) - 1, this);

                return *this;
               }

               double operator ++ (int)
               {
                double  tmp = ToDouble(this);

                ToLDouble(tmp + 1, this);

                return tmp;
               }

               double operator -- (int)
               {
                double  tmp = ToDouble(this);

                ToLDouble(tmp - 1, this);

                return tmp;
               }

               #define  RSL_DOUBLE10_OPASSIGN(_OP)  DBBaseDouble10 &operator _OP##= (double val)   \
                                                    {                                              \
                                                     ToLDouble(ToDouble(this) _OP val, this);      \
                                                     return *this;                                 \
                                                    }

               RSL_DOUBLE10_OPASSIGN(+)
               RSL_DOUBLE10_OPASSIGN(-)
               RSL_DOUBLE10_OPASSIGN(*)
               RSL_DOUBLE10_OPASSIGN(/)

             private:
               char  data[10];
            };

        // -----------------------------------------------------------------------------
        class DBDouble10 : public DBBaseDouble10
            {
             public:
               DBDouble10() {}

               DBDouble10(double num)
               {
                ToLDouble(num, this);
               }

               DBDouble10(const DBBaseDouble10 &base)
               {
                *this = *(DBDouble10 *)&base;
               }
            };

        #define DB_DBL10_TYPES

        typedef DBDouble10      db_double10;
        typedef DBBaseDouble10  db_basedouble10;

    #endif //#if defined(__cplusplus)

#endif //#if !defined(RSL_RESTRICT_LDBL)

#endif // __RSLDOUBL_H

/* <-- EOF --> */