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

       // Добавить строку в список
       bool AddStr(const char *pStr)
       {
        size_t sz = strlen(pStr) + 1;

        if(sz == 1)
          return false;

        if(m_nAlloced < sz + (m_szLastPtr - m_szFirstPtr) + 1)
         {
          size_t size = m_szLastPtr - m_szFirstPtr + 1;

          char  *old = m_szFirstPtr;


          // Устанавливаем требуемый размер буфера
          m_nAlloced = (unsigned)(sz + size);

          // Расширяем его до границы в 256 байт
          m_nAlloced += 256 - m_nAlloced % 256;

          char  *ptr= new char [m_nAlloced];


          // Копируем данные
          m_szFirstPtr = (char *)memcpy(ptr, m_szFirstPtr, size);
          m_szLastPtr  = m_szFirstPtr + size - 1;

          // Удаляем старый буфер
          delete old;
         }

        memcpy(m_szLastPtr, pStr, sz);

        *(m_szLastPtr += sz) = '\0';

        return true;
       }

       // Удалить первую строку из списка
       bool RemoveFirst()
       {
        // Если список пуст
        if(!*m_szFirstPtr)
          return false;

        while(*m_szFirstPtr)
             m_szFirstPtr++;

        ++m_szFirstPtr;

        return true;
       }

       // Удалить последнюю строку из списка
       bool RemoveLast()
       {
        // Если список пуст
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

       // Удалить все строки из списка
       void RemoveAll()
       {
        m_szLastPtr   = m_szFirstPtr;
        *m_szFirstPtr = '\0';
       }

       // Доступ к данным
       const char *GetData() const
       {
        return m_szFirstPtr;
       }

       // Размер данных в байтах
       size_t GetSize() const 
       {
        return (m_szLastPtr - m_szFirstPtr) + 1;
       }

       // true - если список пуст
       bool IsEmpty() const
       {
        return !*m_szFirstPtr ? true : false;
       }

       // Первый элемент списка
       const char *First() const
       {
        return *m_szFirstPtr ? m_szFirstPtr : NULL;
       }

       // Переход к следующему элементу
       const char *Next(const char *pStr) const
       {
        if(!(pStr >= m_szFirstPtr && pStr < m_szLastPtr))
          return NULL;

        // Ищем конец текущей строки
        while(*pStr)
             pStr++;

        // Возвращаем указатель на следующую строку или NULL
        if(!*++pStr)
          return NULL;

        return pStr;
       }

       // Переход к предыдущему элементу
       const char *Prev(const char *pStr) const
       {
        if(!(pStr > m_szFirstPtr && pStr <= m_szLastPtr))
          return NULL;

        // Ищем конец предыдущей строки
        pStr--;
        while(*pStr && pStr > m_szFirstPtr)
             pStr--;

        if(pStr == m_szFirstPtr)
          return NULL;

        // Ищем начало строки
        pStr--;
        while(*pStr && pStr > m_szFirstPtr)
             pStr--;

        return pStr + 1;
       }

       // Последний элемент списка
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