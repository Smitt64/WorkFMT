/*******************************************************************************
 FILE         :   PCCCONV.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   Character code conversions (EBCDIC-ASCII, etc.)

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   10 Sep 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/pccconv.h $
                  $Revision: 6 $
                  $Date: 8.02.00 16:39 $
                  $Author: Lebedev $
                  $Modtime: 8.02.00 16:27 $
*******************************************************************************/

#ifndef __PCCCONV_H
#define __PCCCONV_H

#include <stddef.h>

/*******************************************************************************
                     class PCharConv

 Class defines one-way conversion between two coding layouts.
 Currently defined only for AS400
 DBCS is not supported
*******************************************************************************/
class PCharConv
    {
     public:
       PCharConv(int to, int from = 0);

       char cvt(char c) const;
       char *cvt(char *targ, const char *src = NULL) const;
       char *cvt(char *targ, const char *src, size_t size) const;

       unsigned short target() const
       {
        return _cp1;
       }

       unsigned short source() const
       {
        return _cp2;
       }


     private:
       unsigned short  _cp1; // First codepage ID
       unsigned short  _cp2; // Second codepage ID

       char  cvtbl[256] ;  // Conversion table cp1->cp2
    };


/*******************************************************************************
                     class PDualCConv

 Class defines two-way conversion between two coding layouts.
 Currently defined only for AS400
 DBCS is not supported
*******************************************************************************/
class PDualCConv
    {
     public:
       PDualCConv(int code1, int code2 = 0);

       char cvt21(char c) const;
       char cvt12(char c) const;

       char *cvt21(char *targ, const char *src = NULL) const;
       char *cvt12(char *targ, const char *src = NULL) const;

       char *cvt21(char *targ, const char *src, size_t size) const;
       char *cvt12(char *targ, const char *src, size_t size) const;

       unsigned short cp1() const
       {
        return _cvt21.target();
       }

       unsigned short cp2() const
       {
        return _cvt21.source();
       }

     private:
       PCharConv  _cvt21;
       PCharConv  _cvt12;
    };


/*******************************************************************************
 PCharConv
*******************************************************************************/
inline char PCharConv::cvt(char c) const
{
 return cvtbl[(unsigned char)c];
}

/*******************************************************************************
 PDualCConv
*******************************************************************************/
inline PDualCConv::PDualCConv(int code1, int code2) :
                   _cvt21(code1, code2), _cvt12(code2, code1)
{}

inline char PDualCConv::cvt12(char c) const
{
 return _cvt12.cvt(c);
}

inline char PDualCConv::cvt21(char c) const
{
 return _cvt21.cvt(c);
}

inline char *PDualCConv::cvt12(char *targ, const char *src) const
{
 return _cvt12.cvt(targ, src);
}

inline char *PDualCConv::cvt21(char *targ, const char *src) const
{
 return _cvt21.cvt(targ, src);
}

inline char *PDualCConv::cvt12(char *targ, const char *src, size_t size) const
{
 return _cvt12.cvt(targ, src, size);
}

inline char *PDualCConv::cvt21(char *targ, const char *src, size_t size) const
{
 return _cvt21.cvt(targ, src, size);
}

#endif // __PCCCONV_H

/* <-- EOF --> */
