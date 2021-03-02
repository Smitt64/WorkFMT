//-*--------------------------------------------------------------------------*-
//
// File Name   : rsnumerr.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// June 28, 2004 Alexey Lebedev - Create file
//-*--------------------------------------------------------------------------*-

//
// Классы-исключений для обработки ошибок в класс Numeric.
//

#ifndef __RSNUMERR_H
#define __RSNUMERR_H

#include <string>

#include "tooldef.h"
#include "rsexcept.h"
#include "rsnumeric.h"


/*******************************************************************************
 class Numeric96InexactErr
*******************************************************************************/

class _TOOLEXP Numeric96InexactErr : public RSException
    {
     public:
       Numeric96InexactErr(Numeric96::Op o, const Numeric96 &a, const Numeric96 &b, const char *msg);
       
       Numeric96        leftOperand()  const { return operandA_; }
       Numeric96        rightOperand() const { return operandB_; }
       Numeric96::Op    operation()    const { return op_; }
       RSL_STD::string  opString()     const;
       const char      *msg()          const { return what(); }

     private:
       Numeric96::Op  op_;
       Numeric96      operandA_, operandB_;
    };


/*******************************************************************************
 class Numeric96OverflowErr
*******************************************************************************/

class _TOOLEXP Numeric96OverflowErr : public RSException
    {
     public:
       Numeric96OverflowErr(Numeric96::Op o, const Numeric96 &a, const Numeric96 &b, const char *msg);

       Numeric96        leftOperand()  const { return operandA_; }
       Numeric96        rightOperand() const { return operandB_; }
       Numeric96::Op    operation()    const { return op_; }
       RSL_STD::string  opString()     const;
       const char      *msg()          const { return what(); }

     private:
       Numeric96::Op  op_;
       Numeric96      operandA_, operandB_;
    };


/*******************************************************************************
 class Numeric96DivByZeroErr
*******************************************************************************/

class _TOOLEXP Numeric96DivByZeroErr : public RSException
    {
     public:
       Numeric96DivByZeroErr(Numeric96::Op o, const Numeric96 &a, const Numeric96 &b, const char *msg);

       Numeric96        leftOperand()  const { return operandA_; }
       Numeric96        rightOperand() const { return operandB_; }
       Numeric96::Op    operation()    const { return op_; }
       RSL_STD::string  opString()     const;
       const char      *msg()          const { return what(); }

     private:
       Numeric96::Op op_;
       Numeric96     operandA_, operandB_;
    };

#endif //__RSNUMERR_H

/* <-- EOF --> */