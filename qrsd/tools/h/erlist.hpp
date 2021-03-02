/*************************************************************************
   File Name      : erlist.hpp         14.12.2000
   Programmer     : A.Tokmashev

   Subsystem      : Interpreter RSL
   Description    : Class for store error list of RSL-instance.

   Copyright(C) 1991 - 2000 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __ERLIST_HPP
#define __ERLIST_HPP

#include "rsl/common.h"

// -----------------------------------------------------------------------------
class _APPTLEXP TRSLErrorsList
    {
     public: 
       TRSLErrorsList();
       TRSLErrorsList(const TRSLErrorsList &errList);
       virtual ~TRSLErrorsList();
       
       TRSLErrorsList &operator = (const TRSLErrorsList &errList);

       static void FreeErr(void *err) { free(err); }
       int GetCountErr() const { return LobjNitems((CNTLIST *)&m_errList); }
       CNTLIST *GetErrList() { return &m_errList; }

       void SaveError(ERRINFO *erm);
       ERRLISTELEM* GetCurrentErr() { return m_curErr; }                    
       void SetCurrentErr(ERRLISTELEM *er) { m_curErr = er; }

       ERRINFO *ErrorFirst();
       ERRINFO *ErrorLast();
       ERRINFO *ErrorNext();
       ERRINFO *ErrorPrev();

       void ClearErrors();

     protected:
       CNTLIST       m_errList;
       ERRLISTELEM  *m_curErr;
    };

#endif // __ERLIST_HPP

/* <-- EOF --> */