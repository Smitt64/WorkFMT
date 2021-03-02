/*******************************************************************************
 FILE         :   RSDBTYPE.HPP

 COPYRIGHT    :   R-Style SoftLab, 2004

 DESCRIPTION  :   Шаблоны классов для db-типов.

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   19 Jul 2004

 VERSION INFO :   

*******************************************************************************/

#ifndef __RSDBTYPE_HPP
#define __RSDBTYPE_HPP

#include <platform.h>


#ifdef __cplusplus

// -----------------------------------------------------------------------------
template <class Type> class db_type
{
 public:
   operator Type() const
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        return data;
    #else
        Type  val;

        memcpy(&val, data, sizeof (Type));

        return val;
    #endif
   }

   db_type<Type> operator = (const Type val)
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        data = val;
    #else
        memcpy(data, &val, sizeof(Type));
    #endif

      return *this;
   }

 protected:
   #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
       Type  data;
   #else
       char  data[sizeof(Type)];
   #endif
};

// -----------------------------------------------------------------------------
template <class Type> class db_reftype
{
 public:
   operator Type() const
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        return data;
    #else
        Type  val;

        memcpy(&val, data, sizeof(Type));

        return val;
    #endif
   }

   db_reftype<Type> operator = (const Type &val)
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        data = val;
    #else
        memcpy(data, &val, sizeof(Type));
    #endif

    return *this;
   }

protected:
  #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
      Type  data;
  #else
      char  data[sizeof (Type)];
  #endif
};

// -----------------------------------------------------------------------------
template <class Type, class SaveType> class db_2type
{
 public:
   operator Type() const
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        return data;
    #else
        SaveType  save;

        memcpy(&save, data, sizeof(SaveType));

        return save;
    #endif
   }

   db_2type<Type, SaveType> operator = (const Type val)
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        data = val;
    #else
        SaveType  save = val;

        memcpy(data, &save, sizeof(SaveType));
    #endif

    return *this;
   }

 protected:
   #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
       SaveType  data;
   #else
       char  data[sizeof(SaveType)];
   #endif
};

// -----------------------------------------------------------------------------
template <class Type, class SaveType> class db_2reftype
{
 public:
   operator Type() const
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        return data;
    #else
        SaveType  save;

        memcpy(&save, data, sizeof(SaveType));

        return save;
    #endif
   }

   db_2reftype<Type, SaveType> operator = (const Type &val)
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        data = val;
    #else
        SaveType  save = val;
        memcpy(data, &save, sizeof(SaveType));
    #endif

    return *this;
   }

 protected:
   #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
       SaveType  data;
   #else
       char  data[sizeof(SaveType)];
   #endif
};

// -----------------------------------------------------------------------------
template <class Type, class SaveType, class ConvType> class db_3type
{
 public:
   operator Type () const
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        return (ConvType)data;
    #else
        SaveType  save;

        memcpy(&save, data, sizeof(SaveType));

        return (ConvType)save;
    #endif
   }

   db_3type<Type, SaveType, ConvType> operator = (const Type val)
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        data = (ConvType)val;
    #else
        SaveType  save = (ConvType)val;

        memcpy(data, &save, sizeof(SaveType));
    #endif

    return *this;
   }

 protected:
   #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
       SaveType  data;
   #else
       char  data[sizeof(SaveType)];
   #endif
};

// -----------------------------------------------------------------------------
template <class Type, class SaveType, class ConvType> class db_3reftype
{
public:
   operator Type() const
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        return (ConvType)data;
    #else
        SaveType  save;

        memcpy(&save, data, sizeof(SaveType));

        return (ConvType)save;
    #endif
   }

   db_3reftype<Type, SaveType, ConvType> operator = (const Type &val)
   {
    #if defined (RSL_PL_WIN32) // [!!!:x64]  && !defined (RSL_PL_WIN64)
        data = (ConvType)val;
    #else
        SaveType  save = (ConvType)val;

        memcpy(data, &save, sizeof(SaveType));
    #endif

    return *this;
   }

 protected:
   #if defined (RSL_PL_WIN32) // [!!!:x64] && !defined (RSL_PL_WIN64)
       SaveType  data;
   #else
       char  data[sizeof(SaveType)];
   #endif
};

#endif //__cplusplus

#endif //__RSDBTYPE_HPP

/* <-- EOF --> */