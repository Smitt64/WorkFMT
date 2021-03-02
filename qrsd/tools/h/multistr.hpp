/*************************************************************************
   File Name      : multistr.hpp
   Programmer     : Alexey Lebedev

   Creation       : 15 Nov 2002
   Description    : class TMultiString

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __MULTISTR_HPP
#define __MULTISTR_HPP

#include <stddef.h>


/*************************************************************************
   class TMultiString
*************************************************************************/
class TMultiString
    {
     public:
       TMultiString()
       {
        m_nAlloced  = 256;
        m_szLastPtr = m_szFirstPtr = new char[m_nAlloced];

        *m_szFirstPtr = '\0';
       }

      ~TMultiString()
       {
        delete m_szFirstPtr;
       }

       // �������� ��ப� � ᯨ᮪
       bool AddStr(const char *pStr)
       {
        size_t sz = strlen(pStr) + 1;

        if(sz == 1)
          return false;

        if(m_nAlloced < sz + (m_szLastPtr - m_szFirstPtr) + 1)
         {
          size_t size = m_szLastPtr - m_szFirstPtr + 1;

          char  *old = m_szFirstPtr;


          // ��⠭�������� �ॡ㥬� ࠧ��� ����
          m_nAlloced = (unsigned)(sz + size);

          // �����塞 ��� �� �࠭��� � 256 ����
          m_nAlloced += 256 - m_nAlloced % 256;

          char  *ptr= new char [m_nAlloced];


          // �����㥬 �����
          m_szFirstPtr = (char *)memcpy(ptr, m_szFirstPtr, size);
          m_szLastPtr  = m_szFirstPtr + size - 1;

          // ����塞 ���� ����
          delete old;
         }

        memcpy(m_szLastPtr, pStr, sz);

        *(m_szLastPtr += sz) = '\0';

        return true;
       }

       // ������� ����� ��ப� �� ᯨ᪠
       bool RemoveFirst()
       {
        // �᫨ ᯨ᮪ ����
        if(!*m_szFirstPtr)
          return false;

        while(*m_szFirstPtr)
             m_szFirstPtr++;

        ++m_szFirstPtr;

        return true;
       }

       // ������� ��᫥���� ��ப� �� ᯨ᪠
       bool RemoveLast()
       {
        // �᫨ ᯨ᮪ ����
        if(!*m_szFirstPtr)
          return false;
        
        m_szLastPtr -= 2;

        while(*m_szLastPtr && m_szLastPtr > m_szFirstPtr)
             m_szLastPtr--;

        if(m_szLastPtr > m_szFirstPtr)
          m_szLastPtr++;

        *m_szLastPtr = '\0';

        return true;
       }

       // ������� �� ��ப� �� ᯨ᪠
       void RemoveAll()
       {
        m_szLastPtr   = m_szFirstPtr;
        *m_szFirstPtr = '\0';
       }

       // ����� � �����
       const char *GetData() const
       {
        return m_szFirstPtr;
       }

       // ������ ������ � �����
       size_t GetSize() const 
       {
        return (m_szLastPtr - m_szFirstPtr) + 1;
       }

       // true - �᫨ ᯨ᮪ ����
       bool IsEmpty() const
       {
        return !*m_szFirstPtr ? true : false;
       }

       // ���� ����� ᯨ᪠
       const char *First() const
       {
        return *m_szFirstPtr ? m_szFirstPtr : NULL;
       }

       // ���室 � ᫥���饬� ������
       const char *Next(const char *pStr) const
       {
        if(!(pStr >= m_szFirstPtr && pStr < m_szLastPtr))
          return NULL;

        // �饬 ����� ⥪�饩 ��ப�
        while(*pStr)
             pStr++;

        // �����頥� 㪠��⥫� �� ᫥������ ��ப� ��� NULL
        if(!*++pStr)
          return NULL;

        return pStr;
       }

       // ���室 � �।��饬� ������
       const char *Prev(const char *pStr) const
       {
        if(!(pStr > m_szFirstPtr && pStr <= m_szLastPtr))
          return NULL;

        // �饬 ����� �।��饩 ��ப�
        pStr--;
        while(*pStr && pStr > m_szFirstPtr)
             pStr--;

        if(pStr == m_szFirstPtr)
          return NULL;

        // �饬 ��砫� ��ப�
        pStr--;
        while(*pStr && pStr > m_szFirstPtr)
             pStr--;

        return pStr + 1;
       }

       // ��᫥���� ����� ᯨ᪠
       const char *Last() const
       {
        return Prev(m_szLastPtr);
       }

       // C++ operators
       operator const char * () const { return GetData(); }
       operator bool () const { return !IsEmpty(); }
       bool operator! () const { return IsEmpty(); }

     private:
       unsigned  m_nAlloced;
       char     *m_szLastPtr;
       char     *m_szFirstPtr;
    };

#endif // __MULTISTR_HPP

/* <-- EOF --> */