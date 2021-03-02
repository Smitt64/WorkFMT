/*-----------------------------------------------------------------------*-
 File Name      : StreamNumData.h
 Date           : 1998-2000
 Programmer     : M.Golovanov, V.Markin, A.Dobroslavskiy

 $Revision: 2 $

 Subsystem      : 
 Description    : Вспомогательные поточные функции для классов 
                  чисел с фиксированной точкой

 Copyright (c) 1998-2000 R-Style Software Lab.
 All Rights Reserved.
-*-----------------------------------------------------------------------*-
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/FDecimal.h $
//                  $Revision: 26 $
//                  $Date: 19.09.00 16:48 $
//                  $Author: Lebedev $
//                  $Modtime: 19.09.00 16:21 $
-*-----------------------------------------------------------------------*/
#ifndef __STREAMNUMDATA_H
#define __STREAMNUMDATA_H

#include "rsiostrm.h"
#include "fdecimal.h"

// -----------------------------------------------------------------------------
// выводит в поток
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const FDecimal &dec)
{
 char  pStr[48];

 return os << dec.FormatRawStr(pStr, 48);
}

// -----------------------------------------------------------------------------
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const BaseFDecimal &basedec)
{
 char  pStr[48];

 return os << basedec.FormatRawStr(pStr, 48);
}
// -----------------------------------------------------------------------------
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const DBBaseFDecimal &dbbasedec)
{
 char  pStr[48];

 return os << dbbasedec.FormatRawStr(pStr, 48);
}

#ifdef FDECIMAL_WCHAR
    // -----------------------------------------------------------------------------
    // выводит в поток
    inline RSL_STD::wostream &operator << (RSL_STD::wostream &os, const FDecimal &dec)
    {
     wchar_t  pStr[48];

     return os << dec.FormatRawStr(pStr, 48);
    }

    // -----------------------------------------------------------------------------
    inline RSL_STD::wostream &operator << (RSL_STD::wostream &os, const BaseFDecimal &basedec)
    {
     wchar_t  pStr[48];

     return os << basedec.FormatRawStr(pStr, 48);
    }

    // -----------------------------------------------------------------------------
    inline RSL_STD::wostream &operator << (RSL_STD::wostream &os, const DBBaseFDecimal &dbbasedec)
    {
     wchar_t  pStr[48];

     return os << dbbasedec.FormatRawStr(pStr, 48);
    }
#endif

// -----------------------------------------------------------------------------
// вводит из потока
inline RSL_STD::istream &operator >> (RSL_STD::istream &is, FDecimal &dec)
{
 char  pStr[48];

 is >> pStr;
 dec = FDecimal(pStr);

 return is;
}

// -----------------------------------------------------------------------------
inline RSL_STD::istream &operator >> (RSL_STD::istream &is, BaseFDecimal &basedec)
{
 char  pStr[48];

 is >> pStr;
 basedec = FDecimal(pStr);

 return is;
}

// -----------------------------------------------------------------------------
inline RSL_STD::istream &operator >> (RSL_STD::istream &is, DBBaseFDecimal &dbbasedec)
{
 char  pStr[48];

 is >> pStr;
 dbbasedec = FDecimal(pStr);

 return is;
}

#ifdef FDECIMAL_WCHAR
    // -----------------------------------------------------------------------------
    // вводит из потока
    inline RSL_STD::wistream &operator >> (RSL_STD::wistream &is, FDecimal &dec)
    {
     wchar_t  pStr[48];

     is >> pStr;
     dec = FDecimal(pStr);

     return is;
    }

    // -----------------------------------------------------------------------------
    inline RSL_STD::wistream &operator >> (RSL_STD::wistream &is, BaseFDecimal &basedec)
    {
     wchar_t  pStr[48];

     is >> pStr;
     basedec = FDecimal(pStr);

     return is;
    }

    // -----------------------------------------------------------------------------
    inline RSL_STD::wistream &operator >> (RSL_STD::wistream &is, DBBaseFDecimal &dbbasedec)
    {
     wchar_t  pStr[48];

     is >> pStr;
     dbbasedec = FDecimal(pStr);

     return is;
    }
#endif

// -----------------------------------------------------------------------------
// выводит в поток
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const FMoney &mon)
{
 char  pStr[48];

 return os << mon.FormatRawStr(pStr, 48);
}

// -----------------------------------------------------------------------------
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const BaseFMoney &basemon)
{
 char  pStr[48];

 return os << basemon.FormatRawStr(pStr, 48);
}

// -----------------------------------------------------------------------------
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const DBBaseFMoney &dbbasemon)
{
 char  pStr[48];

 return os << dbbasemon.FormatRawStr(pStr, 48);
}

#ifdef FDECIMAL_WCHAR
    // -----------------------------------------------------------------------------
    // выводит в поток
    inline RSL_STD::wostream &operator << (RSL_STD::wostream &os, const FMoney &mon)
    {
     wchar_t  pStr[48];

     return os << mon.FormatRawStr(pStr, 48);
    }

    // -----------------------------------------------------------------------------
    inline RSL_STD::wostream &operator << (RSL_STD::wostream &os, const BaseFMoney &basemon)
    {
     wchar_t  pStr[48];

     return os << basemon.FormatRawStr(pStr, 48);
    }

    // -----------------------------------------------------------------------------
    inline RSL_STD::wostream &operator << (RSL_STD::wostream &os, const DBBaseFMoney &dbbasemon)
    {
     wchar_t  pStr[48];

     return os << dbbasemon.FormatRawStr(pStr, 48);
    }
#endif

// -----------------------------------------------------------------------------
// вводит из потока
inline RSL_STD::istream &operator >> (RSL_STD::istream &is, FMoney &mon)
{
 char  pStr[48];

 is >> pStr;
 mon = FMoney(pStr);

 return is;
}

// -----------------------------------------------------------------------------
inline RSL_STD::istream &operator >> (RSL_STD::istream &is, BaseFMoney &basemon)
{
 char  pStr[48];

 is >> pStr;
 basemon = FMoney(pStr);

 return is;
}

// -----------------------------------------------------------------------------
inline RSL_STD::istream &operator >> (RSL_STD::istream &is, DBBaseFMoney &dbbasemon)
{
 char  pStr[48];

 is >> pStr;
 dbbasemon = FMoney(pStr);

 return is;
}

#ifdef FDECIMAL_WCHAR
    // -----------------------------------------------------------------------------
    // вводит из потока
    inline RSL_STD::wistream &operator >> (RSL_STD::wistream &is, FMoney &mon)
    {
     wchar_t  pStr[48];

     is >> pStr;
     mon = FMoney(pStr);

     return is;
    }

    // -----------------------------------------------------------------------------
    inline RSL_STD::wistream &operator >> (RSL_STD::wistream &is, BaseFMoney &basemon)
    {
     wchar_t  pStr[48];

     is >> pStr;
     basemon = FMoney(pStr);

     return is;
    }

    // -----------------------------------------------------------------------------
    inline RSL_STD::wistream &operator >> (RSL_STD::wistream &is, DBBaseFMoney &dbbasemon)
    {
     wchar_t  pStr[48];

     is >> pStr;
     dbbasemon = FMoney(pStr);

     return is;
    }
#endif

#endif // __STREAMNUMDATA_H

/* <-- EOF --> */