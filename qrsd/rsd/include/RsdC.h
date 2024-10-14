//-*--------------------------------------------------------------------------*-
//
// File Name   : RsdC.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// February, 2001  Sergey Khorev, Pavlukov Denis  - Create file
//-*--------------------------------------------------------------------------*-

// Вспомогательные классы и функции для классов доступа из C++

#ifndef __RSDC_H
#define __RSDC_H

#include "RsdCBuildOption.h"

#define NVL( val )  (val != NULL ? val : "" )

#define ASSERT_NOT_OPENED( src ) if ( m_fOpened )\
   throw XRsdError ( src, RSD_E_READONLY, this, false );

#define ASSERT_OPENED( src ) if ( !m_fOpened ) throw XRsdError ( src, "Object is not opened.", this, false );

#define ASSERT_NOT_OPENED_( src ) if ( m_fOpened )\
   throw XRsdError ( src, RSD_E_READONLY, (CRsdEnvironment *)NULL, false );

#define ASSERT_OPENED_( src ) if ( !m_fOpened ) throw XRsdError ( src, "Object is not opened.", (CRsdEnvironment *)NULL, false );

//------------------ Для проверки правильности запуска методов.

#ifndef __RSDC_PRECOMPILE
    #include "rsdcpre.h"
#endif

#include "RsdErrMsg.h"
#include "RSDRGlobParms.h"
#include "IniUtil.h"

#ifndef __RSLOCALE_H
    #include "rslocale.h"
#endif

#include "strmoney.h"
#include "RSDDefs.h"
#include <assert.h>
#include <memory>

#include "traceinit.h"
//#include <boost/optional.hpp>
namespace not_boost{
  template<class T>
  class optional_referenceable{
  protected:
    T val_;
    bool is_valid_;
  public:
    optional_referenceable() : is_valid_(false) {}
    //~optional_referenceable() { }
    //optional_referenceable(const optional_referenceable<T> & t) { initFromCopy(t); }
    //optional_referenceable<T> & operator=(const optional_referenceable<T> & t) { clear(); initFromCopy(t); return *this;}

    T & operator*() { assert(is_valid_); return val_; }
    const T & operator*() const { assert(is_valid_); return val_; }
    operator bool() const { return is_valid_; }
    void emplace(const T & val) { val_ = val; is_valid_ = true; }
    void clear() { is_valid_ = false; }
  };
}


#pragma warning ( push )

#pragma warning ( disable : 4251 )
//'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'


#pragma warning ( disable : 4275 )
//non – DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'

#ifdef _MSC_VER
#define stricmp(s1,s2)     _stricmp((s1),(s2))
#endif

// Максимальный размер буфера
#define  dMAXSIZE_VALTEXT   256

class XRsdError;

class ParamList : public std::map< std::string, std::string >
   // маленький такой классик, чтобы парсить строку паметров типа
   // "dsn=database;user id=username;password=pwd". Если в параметре
   // используются символы ';', '=' или '"', то он заключается в кавычки
   // ставить пробелы вокруг '=' не рекомендуется ;))
{
public:
   ParamList ( std::string& s );        // можно передать строку параметров
};


template< typename Ptr_t >
class PtrVector : public std::vector< Ptr_t >
   // исключительно для целей безопасности
   // а все из-за этого RSL.... ;))
{
public:
   PtrVector (){}
   ~PtrVector (){ clear (); }

   typedef std::vector< Ptr_t > Parent;

   void clear ()
   {
      for ( iterator i=begin (); i!=end (); i++ )
         delete (*i);
      Parent::clear ();
   }

   long getIndex ( const char* name )
   {
      for ( iterator i = begin (); i != end (); i++ )
      {
         if ( !stricmp ( (*i)->getName (), name) )
            return (long)(i-begin());
      }
      return -1;
   }

   Ptr_t operator[] ( long index )
   {
      if ( index < 0 || (size_t)index >= size () )
         return NULL;
      return Parent::operator[] ( index );
   }

   Ptr_t operator[] ( const char* name )
   {
      long indx=getIndex (name);
      if ( indx==-1 ) return NULL;
      return operator[] ( indx );
   }

   void erase ( long index )
   {
      delete *(begin ()+index);
      Parent::erase ( begin ()+index );
   }
};

class CRsdEnvironment;
class CRsdConnection;
class CRsdCommand;
class CRsdRecordset;

class RSDDLL XRsdError : public std::runtime_error
   // наш класс исключения. Слегка позаимствовано у Федосова.
   // все классы CRsd... если и будут кидать исключения,
   // то только этого типа!!! Возможно, перед throw они еще
   // будут заполнять коллекцию ошибок у Environment'а.
{
public:
   //   typedef std::runtime_error Parent;
   // место возникновения ошибки
   // etNoInfo если нельзя получить дополнительную информацию
   enum Source { etNoInfo, etEnv, etConnect, etCommand, etResultSet, etMultiRSet, etData };

   XRsdError (const Source source, const std::string& what_arg, const CRsdEnvironment *env, bool fSQL);
   XRsdError (const Source source, const std::string& what_arg, const CRsdRecordset *rs, bool fSQL);
   XRsdError (const Source source, const std::string& what_arg, const CRsdCommand *cmd, bool fSQL);
   XRsdError (const Source source, const std::string& what_arg, const CRsdConnection *con, bool fSQL);

   Source GetSource () const { return errSource; }
   bool IsSQL () const {return m_fSQL;}

   const CRsdEnvironment *getEnv () { return m_env; }
private:
   Source errSource;
   bool m_fSQL;
   const CRsdEnvironment *m_env;
};

class RSDDLL CRsdEncBase
   // вспомогательный класс исключительно для использования в CRsdBase.
   // к сожалению, прямо в CRsdBase определять эти методы не удается
   // из-за его шаблонности (см. приведение в конструкторе CRsdBase : ( (
{
public:
   RSDEncoding_t getServerEncoding () const { return m_ServerEnc; }
   void setServerEncoding ( RSDEncoding_t Enc );    
   RSDEncoding_t getClientEncoding () const { return m_ClientEnc; }
   void setClientEncoding ( RSDEncoding_t Enc );    
   bool IsOpened () const {  return m_fOpened;}

protected:  

   CRsdEncBase () : m_fOpened (false), m_ServerEnc (RSDENC_OEM), m_ClientEnc (RSDENC_OEM) {}

   RSDEncoding_t m_ServerEnc, m_ClientEnc; // кодировки сервера, клиента
   // наследуются от вышележащего объекта, но могут переопределиться в подчиненном

   bool m_fOpened; // т.к. используем "ленивую" активизацию
   // нужен индикатор того, что драйвер уже
   // задействован
};


template < typename Handle_t, typename PtrIntf_t, typename PtrOwner_t = CRsdEncBase* >
class CRsdBase : public CRsdEncBase
   // простой базовый класс. Введен (пока) только для обобщения,
   // чтобы не описывать члены типа m_fOpened,...
   // никаких виртуальных функций...
{

public:
   PtrOwner_t GetOwner () const{ return m_pOwner; }
   Handle_t GetHandle () const { return m_handle; }
   PtrIntf_t GetIntf () const { return m_pIntf; }

protected:

   CRsdBase ( PtrOwner_t pOwner=NULL ) : 
         m_pIntf (NULL), m_handle (NULL), m_pOwner (pOwner)
         {
            if ( pOwner )
            {
               //              CRsdEncBase* pEncOwner = reinterpret_cast<CRsdEncBase*> (pOwner);
               CRsdEncBase* pEncOwner = static_cast<CRsdEncBase*> (pOwner);
               CRsdEncBase::setServerEncoding ( pEncOwner->getServerEncoding () );
               CRsdEncBase::setClientEncoding ( pEncOwner->getClientEncoding () );
            }
         }

         Handle_t m_handle;      // handle соответствующего объекта драйвера
         PtrIntf_t   m_pIntf;    //указатель на интерфейс
         PtrOwner_t m_pOwner;

         /*
         void makeopen () throw ( XRsdError ){} 
         // "внутренняя" функция открытия - вызывается только
         // методами класса. Все остальный должны использовать
         // открытый интерфейс - функцию open (). А уж ее прототип
         // может быть для каждого класса свой.
         */
};



template< class T >
class CRslBrother;  // будет производным от CBrother
// будет определен для RSL-wrapper'ов
// предназначен для классов, который кое-что знают
// о типе своих будущих братьев.


class CRsdError;
class CRsdField;
class CRsdParameter;

//template< class T >
class RSDDLL CBrother
   // Вспомогательный класс для нормальной работы со ссылками
   // предполагается использовать для связей C++-класс <-> его RSL-оболочка.
   // они хранят ссылки друг на друга. Имеется возможность сказать
   // владельцу ссылки на себя "сейчас я умру, так что твоя ссылка
   // на меня будет инвалидной" ;))
   // автоматически при вызове деструктора убивает ссылку на себя
   // в принципе, он только для этого и нужен
{
protected:
   CBrother ( CBrother* pBrother=NULL ) : m_pBrother (pBrother)
   {
      // предполагаем, что если у нас уже есть "старший" брат
      // (т.е. pBrother != NULL), то он сам позаботится о том, 
      // чтобы помнить нас.
   }

   virtual ~CBrother ()
   {
      if ( m_pBrother != NULL )
         m_pBrother->Forget ();   // прощай, братишка
   }

   // просит класс добавить ссылку на caller'а (аргумент)
   // если объект уже на кого-то ссылается, return false 
   // (чисто отладочная мера, надеюсь)
public:
   bool Remember (CBrother* pBrother)
   {
      RSASSERT ( !m_pBrother );

      if ( m_pBrother )
         return false;   // непорядок - пытается набиться в братья еще 
      // один, а мы можем помнить только одного

      // здравствуй, брат. Теперь я тебя буду помнить.
      m_pBrother = pBrother;
      return true;
   }

   // вызывается "братом", чтобы сообщить, что он "при смерти"
   // если ни на кого не ссылаемся, то return false (for debug only)
   bool Forget ()
   {
      RSASSERT ( m_pBrother );

      if ( !m_pBrother )
         return false;   // кто-то пытается нам сообщить, чтобы
      // бы его забыли. А мы и так никого не помним!
      // забываем про брата
      m_pBrother = NULL;  
      return true;
   }
   
   CBrother* getBrother ()
   {
      return m_pBrother;
   }
   
private:
   //  спрятали этот член, дабы у потомков не было соблазна пойти по указателю!!!
   CBrother* m_pBrother;

   friend class CRslBrother< CRsdError >;
   friend class CRslBrother< CRsdField >;
   friend class CRslBrother< CRsdParameter >;
   // в стандарте С++ возможен вариант
   //  template <class T> friend class CRslBrother;
   // но это не для нас : ( ( ( ( ( ( ( ( ( ( ( (

};

//=============================================================================
class RSDDLL CRsdError : public CBrother
{
public:
   CRsdError () : CBrother (NULL), m_AllowDelete (false){}

   CRsdError (  const char* descr,
      long    code,
      const char* source,
      const char* SQLState = "",
      long    nativeError = 0,
      const char* helpFile = "",
      const char* helpContextID = "" ) :
   CBrother (NULL),
      m_strDescr (descr), m_lCode (code), m_strSource (source),
      m_strSQLState (SQLState), m_lNativeError (nativeError),
      m_strHelpFile (helpFile), m_strContextID (helpContextID), m_AllowDelete (false)
   {}

   CRsdError ( RSDErrorInfo& eInfo):
   m_lCode ( eInfo.code), m_AllowDelete (false)
   {
      m_strDescr = NVL ( eInfo.description );
      m_strSQLState = eInfo.SQLState;
      m_lNativeError = eInfo.nativeError;
      /* остались еще поля:
      RSDULONG    severity; 
      RSDULONG    level;
      RSDULONG    subsystem;
      */
   }

   ~CRsdError () {}

   const char* getDescr () { return m_strDescr.c_str (); }
   inline const char* getDescr () const { return m_strDescr.c_str (); }
   long getCode () { return m_lCode; }
   const char* getSource () { return m_strSource.c_str (); }
   const char* getSQLState () { return m_strSQLState.c_str (); }
   long getNativeError () { return m_lNativeError; }
   const char* getHelpFile () { return m_strHelpFile.c_str (); }
   const char* getHelpContextID () { return m_strContextID.c_str (); }

   void MarkDeleted (){ m_AllowDelete=true; }
   bool CanDelete () { return m_AllowDelete; }
protected:

   friend class CRsdEnvironment;
   //  friend class XRsdError;

   std::string m_strDescr;         // Описывает ошибку.
   long    m_lCode;            // Код ошибки.
   std::string m_strSource;            // Источник ошибки - имя приложения или объекта.
   std::string m_strSQLState;      // Сообщение об ошибке ANSI SQL стандарта, 5 симв.
   long    m_lNativeError; // Специфичный для драйвера код ошибки.
   std::string m_strHelpFile;      // Путь к файлу справки об ошибке.
   std::string m_strContextID; // Контекст статьи в файле справки.

   bool m_AllowDelete;
};


//=============================================================================
class RSDDLL CRsdProperty
{
   /*
   public:
   CRsdProperty (): prop (NULL) {}
   ~CRsdProperty () {}

   //---------------------------
   const char* getName (void);

   long        getType (void);

   const char* getValue (void);
   bool        setValue (const char *val);

   long        getAttributes (void);
   bool        setAttributes (long attr);

   protected:
   friend class CRsdEnvironment;
   friend class CRsdConnection;
   friend class CRsdCommand;
   friend class CRsdRecordset;

   std::string name;         // Название дополнительного свойства, строка.
   long        type;         // Тип операции и тип данных этого свойства.
   std::string value;  // !!!// Значение, связанное со свойством. !!!!!!!!
   long        attributes;   // Одна или более характеристик свойства.
   */
};

template< typename T > class RsdFldBase;

double RSDDLL DecimalToDouble (RSDDECIMAL *);
RSDULONG DecimalToUlong (const RSDDECIMAL *pd);
RSDLONG DecimalToLong( const RSDDECIMAL *pd );

#ifdef USE_NUMERIC
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDBYTE& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDCHAR& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDSHORT& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDUSHORT& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDLONG& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDBIGINT& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDULONG& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDUBIGINT& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDFLOAT& x);
RSDDLL void GetFromNumeric (RSDNUMERIC&, RSDDOUBLE& x);
#endif

//
// Базовый класс для CRsdField и CRsdParameter
//
template< typename ParentItf_t, typename ParentHandle_t, typename ParentObject_t >
class  CRsdFldBase : public CBrother
   {
public:
   typedef ParentItf_t ParentItf_type;
   typedef ParentHandle_t ParentHandle_type;
   typedef ParentObject_t ParentObject_type;
protected:
   std::string       m_strName;      
   RSDValType_t      m_Type;
   long              m_dataSize;
   void*             m_data;       //указатель на данные,

   ParentItf_t*      m_ParentItf;
   ParentHandle_t    m_ParentHandle;
   ParentObject_t*   m_Parent;
   RSDLONG           m_nOrdinal;

   bool              m_fReadOnly;
   bool              m_fModified;

   long*             m_pLenOrInd;
   long              m_lenOrInd;

   int               m_index;  // Если используется массив параметров, то это индекс в массиве
   int               m_arraySize;

   bool              m_indAlloced;
   bool              m_dataAlloced;
   bool              m_wideCharFlag;

   CRsdFldBase (ParentObject_t* Parent, const char* name, const RSDValType_t type,
         void* buf, long len = 0, long* lenOrInd = NULL, bool isWide = false) : 
            CBrother (NULL), 
            m_strName (name), 
            m_Type (type), 
            m_dataSize (len),
            m_data (buf), 
            m_pLenOrInd (lenOrInd), 
            m_fReadOnly (false), 
            m_Parent (Parent), 
            m_fModified(false), 
            m_nOrdinal (0), 
            m_indAlloced (false),
            m_dataAlloced (false),
            m_index (0),
            m_lenOrInd (RSDBS_OK),
            m_arraySize (1),
            m_wideCharFlag (isWide)
      {
      }

   void initFieldData (RSDValType_t type, void* buf, long len, long* lenOrInd, size_t arraySize, bool isWide)
      {
      deallocateBuffer ();
      deallocateLenOrInd ();

      m_Type         = type;
      m_wideCharFlag = isWide;
      m_data         = buf;
      m_dataSize     = (long)getRsdTypeSize (m_Type, len);
      m_pLenOrInd    = lenOrInd;
      m_arraySize    = (long)arraySize;

      if (!m_pLenOrInd && m_Type != RSDPT_VALNAME)
         {
         if (arraySize == 1)
            m_pLenOrInd = &m_lenOrInd;
         else
            allocateLenOrInd (arraySize);
         }

      if (!m_data && m_dataSize && m_Type != RSDPT_VALNAME)
         {
         if (m_Type & (RSDPT_CFILE | RSDPT_BFILE))
            {
            allocateBuffer ((m_dataSize + len) * m_arraySize);
            RSDBLOB* ptr = (RSDBLOB*)m_data;
            char*    data = (char*)(ptr + m_arraySize);
            for (int i = 0; i < m_arraySize; ++i, ++ptr)
               {
               memset (ptr, 0, sizeof (RSDBLOB));
               if (len)
                  {
                  ptr->data = data;
                  ptr->size = len;
                  data += len;
                  }
               }
            }
         else
            allocateBuffer (m_dataSize * m_arraySize);
         }
      }


   virtual int setIndex (int ind) 
      { 
      RSASSERT (ind < m_arraySize);
      int oldIndex = m_index;
      m_index = ind; 
      return oldIndex;
      }

   int  getIndex () const  
      { 
      return m_index; 
      }

   void* allocateBuffer (size_t sz) 
      {
      deallocateBuffer();
      m_dataAlloced = true;
      return m_data = operator new (sz);
      }

   void deallocateBuffer () 
      {
      if (m_dataAlloced)
         operator delete (m_data);
      m_data = NULL;
      m_dataAlloced = false;
      }

   void* allocateLenOrInd (size_t arraySize)
      {
      deallocateLenOrInd ();
      m_indAlloced = true;
      m_pLenOrInd = (long*)operator new (sizeof (long) * arraySize);
      memset (m_pLenOrInd, 0, sizeof (long) * arraySize);
      return m_pLenOrInd;
      }

   void deallocateLenOrInd ()
      {
      if (m_indAlloced)
         operator delete (m_pLenOrInd);
      m_pLenOrInd = NULL;
      m_indAlloced = false;
      }

public:

   virtual ~CRsdFldBase () 
      {
      deallocateBuffer ();
      deallocateLenOrInd ();
      }

   virtual void* getDataPtr () const
      {
      return m_index ? (char*)m_data + m_index * m_dataSize : m_data;
      }

   bool isNull () const 
      { 
      return getStatus () == RSDBS_NULL; 
      }

   virtual void setNull () 
      { 
      setStatus (RSDBS_NULL); 
      }

   bool isTrunc () const 
      { 
      return getStatus () == RSDBS_TRUNC ||  getValueLen () > getBufferLen ();
      }

   bool getReadOnly () const 
      { 
      return m_fReadOnly; 
      }

   virtual void setReadOnly (bool fReadOnly = true) 
      {
      m_fReadOnly = fReadOnly;
      }

   bool getModified () const 
      { 
      return m_fModified; 
      }

   virtual void setModified (bool fModified) 
      { 
      m_fModified = fModified; 
      }

   RSDLONG getOrdinal () const 
      { 
      return m_nOrdinal; 
      }


   long* getStatusPtr () 
      { 
      return m_pLenOrInd;  
      }

   virtual void setStatus (RSDRSetBindStatus_t status)
      {
      if (m_pLenOrInd)
         m_pLenOrInd [m_index] = (long)status;
      }

   RSDRSetBindStatus_t getStatus () const
      {
      if (m_pLenOrInd && m_pLenOrInd [m_index] < 0)
         return (RSDRSetBindStatus_t)m_pLenOrInd [m_index];
      return RSDBS_OK;
      }

   virtual void setValueLen (long sz)
      {
      if (m_pLenOrInd)
         m_pLenOrInd [m_index] = sz;
      }

   virtual long getValueLen () const
      {
      if ( m_pLenOrInd && (m_Type == RSDPT_BFILE || m_Type == RSDPT_CFILE) ) 
         return m_pLenOrInd [m_index];
      return (long)getRsdTypeSize (m_Type, m_dataSize);
      }

   virtual long getBufferLen () const
      {
      return m_dataSize;
      }

   // Оставляем старую функцию для совместимости исходного кода
   virtual long getLen () const
      {
      return getBufferLen ();
      }

   virtual void SetExtraInfo (ParentItf_t* parentItf, ParentHandle_t parentHandle, ParentObject_t* parent, RSDLONG nOrdinal)
      {
      m_ParentItf    = parentItf;
      m_ParentHandle = parentHandle;
      m_Parent       = parent;
      m_nOrdinal     = nOrdinal;
      }

   const char* getName () const 
      { 
      return m_strName.c_str ();
      }

   virtual RSDValType_t getType () const 
      { 
      return m_Type; 
      }

   bool isUnicodeField ()
      {
      return m_wideCharFlag;
      }

   virtual void* getValPtr () const 
      { 
      return m_data; 
      }


   RSDLONG getVal (void* to, RSDLONG len) const 
      { 
      len = min (len, m_dataSize);
      memcpy (to, getDataPtr (), len); 
      return len;
      }

   static RSDValType_t getRsdType ( RSDBYTE& ) { return RSDPT_BYTE; }
   static RSDValType_t getRsdType ( RSDCHAR& ) { return RSDPT_CHAR; }
   static RSDValType_t getRsdType ( RSDSHORT& ) { return RSDPT_SHORT; }
   static RSDValType_t getRsdType ( RSDUSHORT& ) { return RSDPT_USHORT; }
   static RSDValType_t getRsdType ( RSDLONG& ) { return RSDPT_LONG; }
   static RSDValType_t getRsdType ( RSDULONG& ) { return RSDPT_ULONG; }
   static RSDValType_t getRsdType ( RSDBIGINT& ) { return RSDPT_BIGINT; }
   static RSDValType_t getRsdType ( RSDUBIGINT& ) { return RSDPT_UBIGINT; }
   static RSDValType_t getRsdType ( RSDFLOAT& ) { return RSDPT_FLOAT; }
   static RSDValType_t getRsdType ( RSDDOUBLE& ) { return RSDPT_DOUBLE; }
   static RSDValType_t getRsdType ( RSDLPSTR ) { return RSDPT_LPSTR; }
   static RSDValType_t getRsdType ( RSDLPWSTR ) { return RSDPT_LPWSTR; }
   static RSDValType_t getRsdType ( RSDBINARY ) { return RSDPT_BINARY; }
   static RSDValType_t getRsdType ( RSDDECIMAL& ) { return RSDPT_DECIMAL; }
#if defined (USE_NUMERIC) && !defined (NUMERIC_AS_MONEY)
   static RSDValType_t getRsdType ( RSDNUMERIC& ) { return RSDPT_NUMERIC; }
#endif
   static RSDValType_t getRsdType ( RSDBIT& ) { return RSDPT_BIT; }
   static RSDValType_t getRsdType ( RSDDATE& ) { return RSDPT_DATE; }
   static RSDValType_t getRsdType ( RSDTIME& ) { return RSDPT_TIME; }
   static RSDValType_t getRsdType ( RSDTIMESTAMP& ) { return RSDPT_TIMESTAMP; }

   static size_t getRsdTypeSize (RSDValType_t type, long len = 0)
      {
      switch (type)
         {
      case RSDPT_BYTE:    return sizeof (RSDBYTE); break;
      case RSDPT_CHAR:    return sizeof (RSDCHAR); break;
      case RSDPT_SHORT:   return sizeof (RSDSHORT); break;
      case RSDPT_LONG:    return sizeof (RSDLONG); break;
      case RSDPT_BIGINT:  return sizeof (RSDBIGINT); break;
      case RSDPT_USHORT:  return sizeof (RSDUSHORT); break;
      case RSDPT_ULONG:   return sizeof (RSDULONG); break;
      case RSDPT_UBIGINT: return sizeof (RSDUBIGINT); break;
      case RSDPT_FLOAT:   return sizeof (RSDFLOAT); break;
      case RSDPT_DOUBLE:  return sizeof (RSDDOUBLE); break;
      case RSDPT_BINARY:
      case RSDPT_LPSTR:   return len; break;
      case RSDPT_LPWSTR:  return len; break;
      case RSDPT_DECIMAL: return sizeof (RSDDECIMAL); break;
#if defined (USE_NUMERIC) && !defined (NUMERIC_AS_MONEY)
      case RSDPT_NUMERIC: return sizeof (RSDNUMERIC); break;
#endif
      case RSDPT_BIT:     return sizeof (RSDBIT); break;
      case RSDPT_DATE:    return sizeof (RSDDATE); break;
      case RSDPT_TIME:    return sizeof (RSDTIME); break;
      case RSDPT_TIMESTAMP:return sizeof (RSDTIMESTAMP); break;

      case RSDPT_VALNAME: return len; break;

      case RSDPT_CFILE:
      case RSDPT_BFILE:   return sizeof (RSDBLOB);

      default:
         throw XRsdError ( XRsdError::etResultSet, "Illegal type", (CRsdEnvironment *)NULL, false );
         }
      }


   void clear ()
      {
      m_fModified = false;

      if (!m_data)
         return;

      switch (getType ())
         {
      case RSDPT_BYTE:    *(RSDBYTE*)getDataPtr () = 0; break;
      case RSDPT_CHAR:    *(RSDCHAR*)getDataPtr () = 0; break;
      case RSDPT_SHORT:   *(RSDSHORT*)getDataPtr () = 0; break;
      case RSDPT_LONG:    *(RSDLONG*)getDataPtr () = 0; break;
      case RSDPT_BIGINT:  *(RSDBIGINT*)getDataPtr () = 0; break;
      case RSDPT_USHORT:  *(RSDUSHORT*)getDataPtr () = 0; break;
      case RSDPT_ULONG:   *(RSDULONG*)getDataPtr () = 0; break;
      case RSDPT_UBIGINT: *(RSDUBIGINT*)getDataPtr () = 0; break;
      case RSDPT_FLOAT:   *(RSDFLOAT*)getDataPtr () = 0; break;
      case RSDPT_DOUBLE:  *(RSDDOUBLE*)getDataPtr () = 0; break;
      case RSDPT_BINARY:  memset (getDataPtr (), 0, getBufferLen ()); break;
      case RSDPT_LPSTR:   *(RSDCHAR*)getDataPtr () = 0; break;
      case RSDPT_LPWSTR:  *(RSDWCHAR*)getDataPtr () = 0; break;
      case RSDPT_DECIMAL:  memset (getDataPtr (), 0, sizeof (RSDDECIMAL)); break;
#if defined (USE_NUMERIC) && !defined (NUMERIC_AS_MONEY)
      case RSDPT_NUMERIC:  *(RSDNUMERIC*)getDataPtr () = NUMERIC_ZERO;  break;
#endif
      case RSDPT_BIT:     *(RSDBIT*)getDataPtr () = 0; break;
      case RSDPT_DATE:    memset (getDataPtr (), 0, sizeof (RSDDATE)); break;
      case RSDPT_TIME:    memset (getDataPtr (), 0, sizeof (RSDTIME)); break;
      case RSDPT_TIMESTAMP: memset (getDataPtr (), 0, sizeof (RSDTIMESTAMP)); break;

      case RSDPT_VALNAME: break;

      case RSDPT_CFILE:
      case RSDPT_BFILE:   
         {
         RSDBLOB* src = (RSDBLOB*)getDataPtr ();
         if (src->data)
            memset (src->data, 0, src->size); 
         else
            memset (src, 0, getBufferLen ()); 
         }
      break;

      default:
         throw XRsdError (XRsdError::etResultSet, "Illegal type", (CRsdEnvironment *)NULL, false);
         }
      setStatus (RSDBS_OK);
      }

   // --------------------------------------------
   // Конвертируем значение в строку для вывода в трассу
   char *toTraceString(void)
   {
    static char  valTxt[dMAXSIZE_VALTEXT] = "";


    if(m_data)
     {
      char   buff[dMAXSIZE_VALTEXT];
      void  *from = getDataPtr();


      if(from)
        switch(m_Type)
             {
              case RSDPT_BIT:
                   sprintf(valTxt, "%d", (int)*(bool *)from);
                   break;

              case RSDPT_BYTE:
                   sprintf(valTxt, "%d", (int)*(unsigned char *)from);
                   break;

              case RSDPT_SHORT:
                   sprintf(valTxt, "%d", (int)*(short *)from);
                   break;

              case RSDPT_USHORT:
                   sprintf(valTxt, "%u", (int)*(unsigned short *)from);
                   break;

              case RSDPT_LONG:
                   sprintf(valTxt, "%ld", *(long *)from);
                   break;

              case RSDPT_ULONG:
                   sprintf(valTxt, "%lu", *(unsigned long *)from);
                   break;

              case RSDPT_UBIGINT:
                   {
                    RSDUBIGINT  src = *(RSDUBIGINT *)from;

                    sprintf(valTxt, "%I64u", src);
                   }
                   break;

              case RSDPT_BIGINT:
                   {
                    RSDBIGINT  src = *(RSDBIGINT *)from;

                    sprintf(valTxt, "%I64d", src);
                   }
                   break;

              case RSDPT_FLOAT:
                   sprintf(valTxt, "%f", (double)*(float *)from);
                   break;

              case RSDPT_DOUBLE:
                   sprintf(valTxt, "%lf", *(double *)from);
                   break;

              case RSDPT_CHAR:
                   sprintf(valTxt, "%d", (int)*(unsigned char *)from);
                   break;

              case RSDPT_LPSTR:
                   {
                    // Обрезаем выводимый текст по размеру буфера. Если тест превышает размер,
                    // выводим его в таком виде:
                    // 'ЧастьПоместившегосяТекста...', N
                    // где N - полный размер текста
                    int  buffSize = dMAXSIZE_VALTEXT,
                         textSize = from ? (int)strlen((const char *)from) : 0;


                    *valTxt = 0;

                    if(textSize > 0)
                     {
                      if(textSize <= (buffSize - 3))
                        sprintf(valTxt, "'%s'", (const char *)from);
                      else
                       {
                        // В принципе, размер можно уменьшить на 17 символов, но для простоты уменьшаем на 20.
                        char  tmpBuff[dMAXSIZE_VALTEXT - 20];


                        strncpy(tmpBuff, (const char *)from, dMAXSIZE_VALTEXT - 20);
                        tmpBuff[dMAXSIZE_VALTEXT - 21] = 0;

                        sprintf(valTxt, "'%s...', %d", tmpBuff, textSize);
                       }
                     }
                   }
                   break;

              case RSDPT_LPWSTR:
                   {
                    size_t           toLen    = dMAXSIZE_VALTEXT;
                    size_t           fromLen  = wcslen((RSDWCHAR *)from) + 1;
                    RslCodePageType  codePage = RSL_CP_RSOEM;


                    LCW2RSCH(buff, (int)toLen, (RSDWCHAR *)from, (int)min(toLen, fromLen), codePage);
                    buff[toLen - 1] = '\0';

                    sprintf(valTxt, "'%s'", buff);
                   }
                   break;


              case RSDPT_DECIMAL:
                   {
                    RSDDECIMAL  *src = (RSDDECIMAL *)from;
                    FDecimal     dec(*src);
                    long         valLen = getValueLen();


                    //RSDDECIMAL         
                    if(valLen != getRsdTypeSize(RSDPT_BIGINT) /*|| bRec.bufSize != RSDBIGINT_STRLEN*/)
                     {
                      decimal_to_str(dec, buff, 0, RSDDECIMAL_SCALE, 0);

                      sprintf(valTxt, "%s", buff);
                     }         
                    else  //RSDBIGINT  
                     {           
                      // так как SQL_C_SBIGINT и SQL_C_UBIGINT из спецификации ODBC 3.0,  в Oracle ODBC вплоть до 12-ой версии не поддерживаются
                      // RSDPT_BIGINT и RSDPT_UBIGINT биндятся в ODBC через SQL_C_NUMERIC c преобразованием в RSDDECIMAL внутри BigIntToOdbcCvt и BigIntFromOdbcCvt. 
                      decimal_to_str(dec, buff, 0, RSDBIGINT_SCALE, 0);

                      sprintf(valTxt, "%s", buff);
                     }
                   }
                   break;

       #ifdef USE_NUMERIC
              case RSDPT_NUMERIC:
                   numeric_to_str(*((Numeric *)from), buff, 0, RSDNUMERIC_SCALE, 0);

                   sprintf(valTxt, "%s", buff);
              break;
       #endif

              case RSDPT_DATE:
                   {
                    RSDDATE  &bnd = *(RSDDATE *)from;

                    sprintf(valTxt, "%d.%d.%d", (int)bnd.day, (int)bnd.mon, (int)bnd.year);
                   }
                   break;

              case RSDPT_TIME:
                   {
                    RSDTIME  &bnd = *(RSDTIME *)from;

                    sprintf(valTxt, "%d.%d.%d", (int)bnd.hour, (int)bnd.min, (int)bnd.sec);
                   }
                   break;

              case RSDPT_TIMESTAMP:
                   {
                    RSDTIMESTAMP  &bnd = *(RSDTIMESTAMP *)from;

                    sprintf(valTxt, "%d.%d.%d %d:%d:%d", (int)bnd.date.day, (int)bnd.date.mon, (int)bnd.date.year, (int)bnd.time.hour, (int)bnd.time.min, (int)bnd.time.sec);
                   }
                   break;

              case RSDPT_BINARY:
                   {
                    size_t  len = min(10, getValueLen());


                    buff[0] = '\0';

                    for(size_t  i = 0; i < len; ++i)
                      {
                       strcat(buff, " ");

                       sprintf(buff + strlen(buff), "%0X", ((char *)from)[i]);
                      }

                    sprintf(valTxt, "%s", buff);
                   }
                   break;

              case RSDPT_BFILE:
                   sprintf(valTxt, "<blob>");
                   break;

              case RSDPT_CFILE:
                   sprintf(valTxt, "<clob>");
                   break;
             }
     }

    return valTxt;
   }

   RSDLPSTR AsRSDLPSTR ()
      {
      RSDValType_t type = getType ();
      if (type != RSDPT_LPSTR && !(m_dataSize && (type == RSDPT_BFILE || type == RSDPT_CFILE)))
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment*)NULL, false);

      if (type == RSDPT_BFILE || type == RSDPT_CFILE)
         {
         RSDBLOB* src = (RSDBLOB*)getDataPtr ();
         reinterpret_cast<RSDLPSTR> (src->data);
         }

      return reinterpret_cast<RSDLPSTR> (getDataPtr ());
      }

   RSDBINARY AsRSDBINARY ()
      {
      RSDValType_t type = getType ();
      if (type != RSDPT_BINARY && !(m_dataSize && (type == RSDPT_BFILE || type == RSDPT_CFILE)))
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment*)NULL, false);

      if (type == RSDPT_BFILE || type == RSDPT_CFILE)
         {
         RSDBLOB* src = (RSDBLOB*)getDataPtr ();
         reinterpret_cast<RSDBINARY> (src->data);
         }

      return reinterpret_cast<RSDBINARY> (getDataPtr ());
      }

   RSDLPWSTR AsRSDLPWSTR ()
      {
      if (getType () != RSDPT_LPWSTR)
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);
      return reinterpret_cast<RSDLPWSTR> (getDataPtr ());
      }

#ifdef NUMERIC_AS_MONEY

#define num_def_cast( type, dbtype )    \
   /*  в целях безопасности и хорошести operator type () заменен на методы вида As...*/ \
   type As##type ()         \
   {                       \
   if (getType () != CRsdFldBase::getRsdType (*(type*)0))\
   switch (getType ())\
   {\
   case RSDPT_BYTE:    \
   return (type)*(uint8*)getDataPtr ();   \
   case RSDPT_SHORT:   \
      return (type)*(int16*)getDataPtr ();  \
   case RSDPT_LONG:    \
      return (type)*(int32*)getDataPtr ();   \
   case RSDPT_BIGINT:    \
      return (type)*(int64_t*)getDataPtr ();   \
   case RSDPT_LPSTR:   \
   case RSDPT_CHAR:    \
      return (type)*(int8*)getDataPtr ();   \
   case RSDPT_USHORT:  \
      return (type)*(uint16*)getDataPtr (); \
   case RSDPT_ULONG:   \
      return (type)*(uint32*)getDataPtr ();  \
   case RSDPT_UBIGINT:   \
      return (type)*(uint64_t*)getDataPtr ();  \
   case RSDPT_FLOAT:   \
      return (type)*reinterpret_cast<RSDFLOAT*> (getDataPtr ());  \
   case RSDPT_DOUBLE:  \
      return (type)*reinterpret_cast<RSDDOUBLE*> (getDataPtr ()); \
   case RSDPT_NUMERIC:   \
      {   \
      type x = 0;   \
      GetFromNumeric (*reinterpret_cast<RSDNUMERIC*> (getDataPtr ()), x);   \
      return x;   \
      }   \
   default:    \
      throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment*)NULL, false);\
      }\
   else    \
      return *(dbtype*)getDataPtr ();\
   }

#elif !defined (USE_NUMERIC)

#define num_def_cast( type, dbtype )    \
   /*  в целях безопасности и хорошести operator type () заменен на методы вида As...*/ \
   type As##type ()         \
   {                       \
   if (getType () != CRsdFldBase::getRsdType (*(type*)0))\
   switch ( getType () )\
   {\
   case RSDPT_BYTE:    \
      return (type)*reinterpret_cast<RSDBYTE*> (getDataPtr ());   \
   case RSDPT_SHORT:   \
      return (type)*reinterpret_cast<RSDSHORT*> (getDataPtr ());  \
   case RSDPT_LONG:    \
      return (type)*reinterpret_cast<RSDLONG*> (getDataPtr ());   \
   case RSDPT_BIGINT:    \
      return (type)*reinterpret_cast<RSDBIGINT*> (getDataPtr ());   \
   case RSDPT_LPSTR:   \
   case RSDPT_CHAR:    \
      return (type)*reinterpret_cast<RSDCHAR*> (getDataPtr ());   \
   case RSDPT_USHORT:  \
      return (type)*reinterpret_cast<RSDUSHORT*> (getDataPtr ()); \
   case RSDPT_ULONG:   \
      return (type)*reinterpret_cast<RSDULONG*> (getDataPtr ());  \
   case RSDPT_UBIGINT:   \
      return (type)*reinterpret_cast<RSDUBIGINT*> (getDataPtr ());  \
   case RSDPT_FLOAT:   \
      return (type)*reinterpret_cast<RSDFLOAT*> (getDataPtr ());  \
   case RSDPT_DOUBLE:  \
      return (type)*reinterpret_cast<RSDDOUBLE*> (getDataPtr ()); \
   case RSDPT_DECIMAL: \
      {   \
      return (type)DecimalToDouble (reinterpret_cast<RSDDECIMAL*> (getDataPtr ()));\
      }   \
   default:    \
      throw XRsdError( XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false );\
      }\
   else    \
      return *reinterpret_cast<type*> (getDataPtr ());\
   }

#else /* USE_NUMERIC */

#define num_def_cast( type, dbtype )    \
   /*  в целях безопасности и хорошести operator type () заменен на методы вида As...*/ \
   type As##type ()         \
   {                       \
   if (getType () != CRsdFldBase::getRsdType (*(type*)0))\
   switch ( getType () )\
   {\
   case RSDPT_BYTE:    \
   return (type)*(db_uint8*)getDataPtr ();   \
   case RSDPT_SHORT:   \
      return (type)*(db_int16*)getDataPtr ();  \
   case RSDPT_LONG:    \
      return (type)*(db_int32*)getDataPtr ();   \
   case RSDPT_BIGINT:    \
      return (type)*(db_int64*)getDataPtr ();   \
   case RSDPT_LPSTR:   \
   case RSDPT_CHAR:    \
      return (type)*(db_int8*)getDataPtr ();   \
   case RSDPT_USHORT:  \
      return (type)*(db_uint16*)getDataPtr (); \
   case RSDPT_ULONG:   \
      return (type)*(db_uint32*)getDataPtr ();  \
   case RSDPT_UBIGINT:   \
      return (type)*(db_uint64*)getDataPtr ();  \
   case RSDPT_FLOAT:   \
      return (type)*reinterpret_cast<RSDFLOAT*> (getDataPtr ());  \
   case RSDPT_DOUBLE:  \
      return (type)*reinterpret_cast<RSDDOUBLE*> (getDataPtr ()); \
   case RSDPT_DECIMAL: \
      {   \
      return (type)DecimalToDouble (reinterpret_cast<RSDDECIMAL*> (getDataPtr ()));\
      }   \
   case RSDPT_NUMERIC:   \
      {   \
      type x = 0;   \
      GetFromNumeric (*reinterpret_cast<RSDNUMERIC*> (getDataPtr ()), x);   \
      return x;   \
      }   \
   default:    \
      throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);\
      }\
   else    \
      return (type)*(dbtype*)getDataPtr ();\
   }
#endif

#define def_cast( type )    \
   /*  в целях безопасности и хорошести operator type () заменен на методы вида As...*/ \
   type As##type ()         \
   {                       \
   if (getType () != CRsdFldBase::getRsdType (*(type*)0) )   \
      throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);\
   return *reinterpret_cast<type*> (getDataPtr ());\
   }

      num_def_cast ( RSDBYTE, db_uint8 )
      
      num_def_cast ( RSDSHORT, db_int16 )

      num_def_cast ( RSDLONG, db_int32 )

      num_def_cast ( RSDBIGINT, db_int64 )

      num_def_cast ( RSDCHAR, db_int8 )

      num_def_cast ( RSDUSHORT, db_uint16 )

      num_def_cast ( RSDULONG, db_uint32 )

      num_def_cast ( RSDUBIGINT, db_uint64 )

      num_def_cast ( RSDFLOAT, db_float )

      num_def_cast ( RSDDOUBLE, db_double )
      

      //    def_cast ( RSDDECIMAL )
#ifdef NUMERIC_AS_MONEY
#define AsRSDDECIMAL AsRSDNUMERIC
#else
   RSDDECIMAL AsRSDDECIMAL () const
   {                      
      FDecimal dec;
      switch ( getType () )
      {
      case RSDPT_BYTE:    
         dec = *reinterpret_cast<RSDBYTE*> (getDataPtr ());
         break;
      case RSDPT_SHORT:   
         dec = *reinterpret_cast<RSDSHORT*> (getDataPtr ());
         break;
      case RSDPT_LONG:    
         dec = *reinterpret_cast<RSDLONG*> (getDataPtr ());   
         break;
      case RSDPT_BIGINT:    
         dec = *reinterpret_cast<RSDBIGINT*> (getDataPtr ());   
         break;
      case RSDPT_CHAR:    
         dec = *reinterpret_cast<RSDCHAR*> (getDataPtr ());   
         break;
      case RSDPT_USHORT:  
         dec = *reinterpret_cast<RSDUSHORT*> (getDataPtr ()); 
         break;
      case RSDPT_ULONG:   
         dec = *reinterpret_cast<RSDULONG*> (getDataPtr ());  
         break;
      case RSDPT_UBIGINT:   
         dec = *reinterpret_cast<RSDUBIGINT*> (getDataPtr ());  
         break;
      case RSDPT_FLOAT:   
         dec = *reinterpret_cast<RSDFLOAT*> (getDataPtr ());  
         break;
      case RSDPT_DOUBLE:  
         dec = *reinterpret_cast<RSDDOUBLE*> (getDataPtr ()); 
         break;
      case RSDPT_DECIMAL: 
         return *reinterpret_cast<RSDDECIMAL*> (getDataPtr ());

#ifdef USE_NUMERIC
      case RSDPT_NUMERIC:
         // ???
#endif
      default:    
         throw XRsdError( XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false );
      }
      FDecimal_t d;
      dec.get (&d);
      return d;
   }
#endif

#ifdef USE_NUMERIC
   RSDNUMERIC AsRSDNUMERIC ()
      {                      
      Numeric num;
      switch (getType ())
         {
      case RSDPT_BYTE:    
         num = *reinterpret_cast<RSDBYTE*> (getDataPtr ());
         break;
      case RSDPT_SHORT:   
         num = *reinterpret_cast<RSDSHORT*> (getDataPtr ());
         break;
      case RSDPT_LONG:    
         num = *reinterpret_cast<RSDLONG*> (getDataPtr ());   
         break;
      case RSDPT_BIGINT:    
         num = *reinterpret_cast<RSDBIGINT*> (getDataPtr ());   
         break;        
      case RSDPT_CHAR:    
         num = *reinterpret_cast<RSDCHAR*> (getDataPtr ());   
         break;
      case RSDPT_USHORT:  
         num = *reinterpret_cast<RSDUSHORT*> (getDataPtr ()); 
         break;
      case RSDPT_ULONG:   
         num = *reinterpret_cast<RSDLONG*> (getDataPtr ());  
         break;
      case RSDPT_UBIGINT:    
         num = *reinterpret_cast<RSDBIGINT*> (getDataPtr ());   
         break;        
      case RSDPT_FLOAT:   
         num = Numeric::from (*reinterpret_cast<RSDFLOAT*> (getDataPtr ()));
         break;
      case RSDPT_DOUBLE:  
         num = Numeric::from (*reinterpret_cast<RSDDOUBLE*> (getDataPtr ())); 
         break;
      case RSDPT_DECIMAL:  
         num = *reinterpret_cast<RSDDECIMAL*> (getDataPtr ()); 
         break;
#ifndef NUMERIC_AS_MONEY
      case RSDPT_NUMERIC: 
         return *reinterpret_cast<RSDNUMERIC*> (getDataPtr ());
#endif
      default:    
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment*)NULL, false);
         }
      return num; 
      }
#endif

   //def_cast ( RSDBIT )
   //    def_cast ( RSDDATE )
   RSDDATE AsRSDDATE ()
      { 
      switch (getType ())
         {
      case RSDPT_DATE:
         return *reinterpret_cast<RSDDATE*> (getDataPtr ());
      case RSDPT_TIMESTAMP:
         return reinterpret_cast<RSDTIMESTAMP*> (getDataPtr ())->date;
      default:
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);
         }
      }

   RSDBIT AsRSDBIT ()
      { 
      switch (getType ())
         {
      case RSDPT_BYTE:
         return (*reinterpret_cast<RSDBYTE*> (getDataPtr ()) != 0);
      case RSDPT_SHORT:
         return (*reinterpret_cast<RSDSHORT*> (getDataPtr ()) != 0);
      case RSDPT_LONG:   
         return (*reinterpret_cast<RSDLONG*> (getDataPtr ()) != 0);
      case RSDPT_BIGINT:   
         return (*reinterpret_cast<RSDBIGINT*> (getDataPtr ()) != 0);
      case RSDPT_CHAR:  
         return (*reinterpret_cast<RSDCHAR*> (getDataPtr ()) != 0);
      case RSDPT_USHORT:
         return (*reinterpret_cast<RSDUSHORT*> (getDataPtr ()) != 0);
      case RSDPT_ULONG:
         return (*reinterpret_cast<RSDULONG*> (getDataPtr ()) != 0);
      case RSDPT_UBIGINT:
         return (*reinterpret_cast<RSDUBIGINT*> (getDataPtr ()) != 0);
      case RSDPT_FLOAT: 
         return (*reinterpret_cast<RSDFLOAT*> (getDataPtr ()) != 0);
      case RSDPT_DOUBLE:
         return (*reinterpret_cast<RSDDOUBLE*> (getDataPtr ()) != 0);
      case RSDPT_DECIMAL:
         {
#ifdef NUMERIC_AS_MONEY
         return ( (*reinterpret_cast<RSDNUMERIC*> (getDataPtr ()) != 0) != 0);
#else
         RSDDECIMAL *dec = reinterpret_cast<RSDDECIMAL*> (getDataPtr ());
         return ( dec->m_Hi !=0 || dec->m_Lo != 0);
#endif
         }
#if defined (USE_NUMERIC) && !defined (NUMERIC_AS_MONEY)
      case RSDPT_NUMERIC:
         return ( (*reinterpret_cast<RSDNUMERIC*> (getDataPtr ()) != 0) != 0);
#endif
      case RSDPT_BIT:    
         return *reinterpret_cast<RSDBIT*> (getDataPtr ());
      default:
         throw XRsdError (XRsdError::etResultSet, "Illegal value type",(CRsdEnvironment *)NULL , false);
         }
      }

   //    def_cast ( RSDTIME )
   RSDTIME AsRSDTIME ()
      {                  
      switch (getType ())
         {
      case RSDPT_TIME:
         return *reinterpret_cast<RSDTIME*> (getDataPtr ());
      case RSDPT_TIMESTAMP:
         return reinterpret_cast<RSDTIMESTAMP*> (getDataPtr ())->time;
      default:
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);
         }
      }
   //    def_cast ( RSDTIMESTAMP )
   RSDTIMESTAMP AsRSDTIMESTAMP ()
      {                    
      RSDTIMESTAMP ts;
      switch (getType ())
         {
      case RSDPT_TIMESTAMP:
         return *reinterpret_cast<RSDTIMESTAMP*> (getDataPtr ());
      case RSDPT_TIME:
         {
         memset (&ts, 0, sizeof (ts));
         memcpy (&ts.time, getDataPtr (), sizeof (RSDTIME));
         return ts;
         }
      case RSDPT_DATE:
         {
         memset (&ts, 0, sizeof (ts));
         memcpy (&ts.date, getDataPtr (), sizeof (RSDDATE));
         return ts;
         }
      default:
         throw XRsdError( XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false );
         }
      }

   void setVal (RSDLPSTR val)
      {
      RSDValType_t tp = getType ();
      
      if (tp != RSDPT_LPSTR && tp != RSDPT_LPWSTR && tp != RSDPT_BFILE && tp != RSDPT_CFILE)
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);

      m_fModified = true;

      void*    bPtr = getDataPtr ();
      size_t   bSize = m_dataSize;

      size_t   sLen = strlen (val) + 1;

      if (tp == RSDPT_BFILE || tp == RSDPT_CFILE)
         {
         RSDBLOB* src = (RSDBLOB*)bPtr;
         if (!src->data)
            throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);

         bPtr = src->data;
         bSize = src->size;
         }


      if (tp == RSDPT_LPSTR || tp == RSDPT_BFILE || tp == RSDPT_CFILE)
         {
         if (sLen > bSize)
            sLen = bSize;

         strncpy ((RSDLPSTR)bPtr, val, sLen - 1);
         ((RSDLPSTR)bPtr) [sLen - 1] = '\0'; 

         setValueLen ((long)sLen);
         }
      else
         {
         if (sLen * sizeof (wchar_t) > bSize)
            sLen = (bSize / 2);

         LCRSCH2W ((RSDLPWSTR)bPtr, (int)(sLen - 1), val, (int)(sLen - 1), RSL_CP_RSOEM);
         ((RSDLPWSTR)bPtr) [sLen - 1] = L'\0'; 

         setValueLen ((long)sLen * sizeof (wchar_t));
         }

      m_Parent->PutData (m_nOrdinal);
      }

   void setVal (RSDLPWSTR val)
      {
      m_fModified = true;

      if (getType () != RSDPT_LPWSTR)
         throw XRsdError( XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false );

      void*    bPtr = getDataPtr ();

      size_t   sLen = wcslen (val) + 1;

      if (sLen * sizeof (wchar_t) > (size_t)m_dataSize)
         sLen = ((size_t)m_dataSize / 2);


      wcsncpy ((RSDLPWSTR)bPtr, val, sLen - 1);
      ((RSDLPWSTR)bPtr) [sLen - 1] = L'\0'; 

      setValueLen ((long)sLen * sizeof (wchar_t));

      m_Parent->PutData (m_nOrdinal);
      }

   void setVal (RSDBINARY val, RSDLONG size)
      {
      m_fModified = true;
      
      if (getType () != RSDPT_BINARY)
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);

      long toCopy =  min (m_dataSize, size);

      memset (getDataPtr (), 0, m_dataSize);
      memcpy (getDataPtr (), val, toCopy);

      setValueLen (toCopy);

      m_Parent->PutData (m_nOrdinal);
      }


#define def_setVal( type )       \
   void setVal (type val)        \
      {                          \
      m_fModified = true;        \
      if (getType () != CRsdFldBase::getRsdType (*(type*)0))   \
         throw XRsdError( XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false );\
      *(type*)getDataPtr () = val;        \
      setStatus (RSDBS_OK);      \
      m_Parent->PutData (m_nOrdinal);  \
      }

#define def_setValNum( type )  \
   void setVal (type val)   \
      {                       \
      m_fModified = true; \
      setStatus (RSDBS_OK);\
      switch (getType ())   \
      {      \
      case RSDPT_BYTE:   \
      *(RSDBYTE*)getDataPtr () = (RSDBYTE)val; \
      break;         \
      case RSDPT_SHORT:   \
         *(RSDSHORT*)getDataPtr () = (RSDSHORT)val; \
         break;         \
      case RSDPT_LONG:   \
         *(RSDLONG*)getDataPtr () = (RSDLONG)val; \
         break;         \
      case RSDPT_BIGINT:   \
         *(RSDBIGINT*)getDataPtr () = (RSDBIGINT)val; \
         break;         \
      case RSDPT_CHAR:   \
         *(RSDCHAR*)getDataPtr () = (RSDCHAR)val; \
         setValueLen (1); \
         break;         \
      case RSDPT_USHORT:   \
         *(RSDUSHORT*)getDataPtr () = (RSDUSHORT)val; \
         break;         \
      case RSDPT_ULONG:   \
         *(RSDULONG*)getDataPtr () = (RSDULONG)val; \
         break;         \
      case RSDPT_UBIGINT:   \
         *(RSDUBIGINT*)getDataPtr () = (RSDUBIGINT)val; \
         break;         \
      case RSDPT_FLOAT:   \
         *(RSDFLOAT*)getDataPtr () = (RSDFLOAT)val; \
         break;         \
      case RSDPT_DOUBLE:   \
         *(RSDDOUBLE*)getDataPtr () = (RSDDOUBLE)val; \
         break;         \
      default:   \
         throw XRsdError (XRsdError::etResultSet, "Illegal value type", (CRsdEnvironment *)NULL, false);\
         }      \
      m_Parent->PutData (m_nOrdinal);  \
      }

   def_setValNum ( RSDBYTE )
   def_setValNum ( RSDSHORT )
   def_setValNum ( RSDLONG )
   def_setValNum ( RSDBIGINT )
   def_setValNum ( RSDCHAR )
   def_setValNum ( RSDUSHORT )
   def_setValNum ( RSDULONG )
   def_setValNum ( RSDUBIGINT )
   def_setValNum ( RSDFLOAT )
   def_setValNum ( RSDDOUBLE )

   def_setVal ( RSDDECIMAL )

#if defined(USE_NUMERIC) && !defined(NUMERIC_AS_MONEY)
   def_setVal ( RSDNUMERIC )
#endif

   def_setVal ( RSDBIT )
   def_setVal ( RSDDATE )
   def_setVal ( RSDTIME )
   def_setVal ( RSDTIMESTAMP )

   
    

protected:



   friend class CRsdRecordset;
   friend class CRsdCommand;

   friend class RsdFldBase<CRsdParameter>;
   friend class RsdFldBase<CRsdField>; 
   friend class RsdField;
   friend class RsdParameter;
   friend class CoRsdRecordset;

   };


class RSDDLL CRsdField : public CRsdFldBase<IRSRecordSet, RSDHRS, CRsdRecordset>
   {
public:
   typedef CRsdFldBase<IRSRecordSet, RSDHRS, CRsdRecordset> Parent_t;

   ~CRsdField () 
      { 
      }

   //  BLOB methods
   RSDLONG read (RSDVALUEPTR pData, RSDLONG nCount);
   RSDLONG write (RSDVALUEPTR pData, RSDLONG nCount);

   class RsdLpstrReader{
   public:
     inline RsdLpstrReader( CRsdField & parent) : parent_(parent), byte_cx_(0), length_(strlen(parent_.AsRSDLPSTR())) {}
     inline RSDLONG read(RSDVALUEPTR pData, RSDLONG nCount){
       size_t reading_len = (size_t)nCount < length_ - byte_cx_ ? nCount : length_ - byte_cx_;
       memcpy(pData, parent_.AsRSDLPSTR() + byte_cx_, reading_len);
       byte_cx_ += reading_len;
       return (RSDLONG)reading_len;
     }
   protected:
     CRsdField & parent_;
     size_t byte_cx_, length_;
   };

   void setBlobFilename (RSDLPSTR Filename);
   void getBlobFilename (RSDLPSTR Filename);

protected:

   CRsdField (CRsdRecordset* Parent, const char* name, const RSDValType_t type,
            void* buf, long len = 0, long* status = NULL ) : 
            // реально len нужен только для RSDPT_LPSTR
         CRsdFldBase<IRSRecordSet, RSDHRS, CRsdRecordset> (Parent, name, type, buf, len, status )
      { 
      }

   friend class CRsdRecordset;
   friend class CRsdFieldNumber10;

   std::shared_ptr<RsdLpstrReader> rsdlpstr_reader_;
   };

class CRsdRecordset;

//=============================================================================
namespace RsdConv{
   inline RSDULONG& operator << (RSDULONG &dest, const RSDDECIMAL &sour)
   {
      dest = DecimalToUlong(&sour);
      return dest;
   }
   inline RSDLONG& operator << (RSDLONG &dest, const RSDDECIMAL &sour)
   {
      dest = DecimalToLong(&sour);
      return dest;
   }
}

class RSDDLL CRsdFieldNumber10 : public CRsdField
{
   friend class CRsdRecordset;
public:

   ~CRsdFieldNumber10 () 
   { 
   }

   virtual void* getDataPtr () const
   {
      synchronize();
      return fld_to_.getDataPtr();
      //return CRsdField::getDataPtr();
   }

   virtual void* getValPtr () const 
   { 
      synchronize();
      return fld_to_.getValPtr();
      //return CRsdField::getValPtr();
   }

   virtual void setNull () 
   { 
      fld_to_.setNull();
      CRsdField::setNull(); 
   }

   virtual void setReadOnly (bool fReadOnly = true) 
   {
      fld_to_.setReadOnly(fReadOnly);
      CRsdField::setReadOnly(fReadOnly);
   }

   virtual void setModified (bool fModified) 
   { 
      fld_to_.setModified(fModified);
      CRsdField::setModified(fModified);
   }

   virtual long getBufferLen () const
   {
      synchronize();
      return fld_to_.getBufferLen();
      //return CRsdField::getBufferLen();
   }

   virtual long getLen () const
   {
      synchronize();
      return fld_to_.getLen();
      //return CRsdField::getLen();
   }

   virtual void setStatus (RSDRSetBindStatus_t status)
   {
      fld_to_.setStatus(status);
      CRsdField::setStatus(status);
   }

   virtual void setValueLen (long sz)
   {
      fld_to_.setValueLen(sz);
      CRsdField::setValueLen(sz);
   }

   virtual long getValueLen () const
   {
      synchronize();
      return fld_to_.getValueLen();
      //return CRsdField::getValueLen();
   }

   virtual void SetExtraInfo (ParentItf_type* parentItf, ParentHandle_type parentHandle, ParentObject_type* parent, RSDLONG nOrdinal)
   {
      fld_to_.SetExtraInfo(parentItf, parentHandle, parent, nOrdinal);
      CRsdField::SetExtraInfo(parentItf, parentHandle, parent, nOrdinal);
   }

   virtual RSDValType_t getType () const 
   { 
      synchronize();
      return fld_to_.getType();
      //return CRsdField::getType();
   }

protected:

   CRsdFieldNumber10 (CRsdRecordset* Parent, const char* name, const RSDValType_t type,
            void* buf, long len = 0, long* status = NULL) 
      : CRsdField (Parent, name, type, buf, len, status )
      , fld_to_(Parent, name, RSDPT_UNDEFINED, NULL)
      , isSyncNow_(false)
   {
      //fld_to_.initFieldData(to_type, NULL, 0, m_pLenOrInd, m_arraySize, m_wideCharFlag);
   }

   void synchronize() const
   {
      if(isSyncNow_) return;
      isSyncNow_ = true;
      FDecimal val(*reinterpret_cast<FDecimal_t*>( CRsdField::getDataPtr() ));
      if( val > FDecimal(2147483647ll) || val < FDecimal(-2147483648ll)) // val > FDecimal(4294967295ll)      
         throw XRsdError ( XRsdError::etResultSet, "NUMBER(10) type overflow", (CRsdEnvironment *)NULL, false );

      //if(val > FDecimal(2147483647ll) )
      //   convert<RSDULONG, RSDDECIMAL>();
      //else
      convert<RSDLONG, RSDDECIMAL>();
      isSyncNow_ = false;
   }


   template<class TO, class FROM>
   void convert() const
   {
      using namespace RsdConv;
      TO dummy;
      fld_to_.initFieldData( getRsdType( dummy ), NULL, 0, m_pLenOrInd, m_arraySize, m_wideCharFlag);
      for(int i = 0; i < m_arraySize; ++i)
      {
         *reinterpret_cast<TO*>((char*)fld_to_.m_data + i * fld_to_.m_dataSize)
            << *reinterpret_cast<FROM*>((char*)m_data + i * m_dataSize);
      }
   }

   virtual int setIndex (int ind) 
   { 
      fld_to_.setIndex(ind);
      return CRsdField::setIndex(ind);
   }

   mutable CRsdField fld_to_;
   mutable bool isSyncNow_;
};

//=============================================================================
class RSDDLL CRsdEnvironment : public CRsdBase< RSDHEN, IRSEnv*, CRsdEnvironment* >,
   public RSDErrorSink
{
public:
   CRsdEnvironment::CRsdEnvironment () 
      : m_hLib (NULL), m_fCreator (true), OdbcInterface (NULL), m_hParentSink (NULL)
   { 
      vtbl = &MethTable;
      m_errors.clear (); /*    props.clear ();*/
      m_hErrorSink = this;
   }

   CRsdEnvironment ( const char* drv, const char* lib = NULL );
   CRsdEnvironment (RSDHEN hen, HRSDERRSINK hErrorSink ) ;

   ~CRsdEnvironment () { close (); }

   void close ();
   void open () throw ( XRsdError ) { makeopen (); }

   void check ( RSDRESULT res, XRsdError::Source source ) throw ( XRsdError );

   //---------------------- Работа со свойствами.
   const char* getDriver () const { return m_strDriver.c_str (); }

   void setDriver ( const char* drv );

   const char* getLibrary () const { return m_strLibrary.c_str (); }
   void setLibrary ( const char* lib );

   long getState () const { return (long)m_fOpened; }

   long getErrorCount () const { return (long)m_errors.size(); }
   void clearErrors () { m_errors.clear (); };

   CRsdError& getError ( long n );
   inline const CRsdError& getError( long n ) const  { return const_cast<CRsdEnvironment*>(this)->getError(n);  }
   CRsdError& getErrorWithoutDelete ( long n );
   inline const CRsdError& getErrorWithoutDelete ( long n ) const  { return const_cast<CRsdEnvironment*>(this)->getErrorWithoutDelete(n);  }

   HRSDERRSINK getErrSink () { return m_hErrorSink; }
   void setErrSink (HRSDERRSINK sink) { m_hErrorSink = sink; }

   void* GetDriverInterface ( const char* IName );

   static void* GetRecordsetModule ();
   static void SetRecordsetModule (void*);

   static void* GetRecordsetInterface ();
   static void SetRecordsetInterface (void*);

   static void* GetMRecordsetInterface ();
   static void SetMRecordsetInterface (void*);

   void* GetHLib () const { return m_hLib; }

   void* GetOdbcInterface ();
   void SetOdbcInterface (void *itf);

protected:

   friend class CRsdConnection;
   friend class RsdEnvironment;
   friend class RsdConnection;

   void makeopen ();

   //---------------------- Свойства.
   std::string m_strDriver;
   std::string m_strLibrary;

   //----------------------
   void* m_hLib;       // Дескриптор библиотеки драйвера. На самом деле HMODULE
   HRSDERRSINK m_hErrorSink;
   HRSDERRSINK m_hParentSink;

   typedef PtrVector< CRsdError* > ErrorsCollection;
   ErrorsCollection m_errors;

   static IRSDErrorSink MethTable;
   static void pAddError ( HRSDERRSINK, RSDErrorInfo* eInfo );

   void AddError ( RSDErrorInfo* eInfo );   
   bool m_fCreator;    // false в случае, когда handle передается при создании,
   // то есть создаем и удаляем handle не мы

   static void* RecordsetModule;
   static void* RecordsetInterface;
   static void* MRecordsetInterface;

   void* OdbcInterface;
};

//=============================================================================
class RSDDLL CRsdConnection : public CRsdBase< RSDHCN, IRSCon*, CRsdEnvironment* >
{
public:

   typedef CRsdBase< RSDHCN, IRSCon*, CRsdEnvironment* > CParent;

   CRsdConnection () : m_lTimeout (0), m_lState (0), m_fCreator (true), m_fIsInTrans (false)
   { /* props.clear (); */ }

   CRsdConnection ( CRsdEnvironment& env, const char* conStr = NULL,
      const char* usr = NULL, const char* pwd = NULL, long t = 0 );

   CRsdConnection ( CRsdEnvironment& env, RSDHCN hcn );

   ~CRsdConnection () {close (); }


   void close ();
   void open () { makeopen (); }

   void cancel ();
   void beginTrans ();
   void commitTrans ();
   void rollbackTrans ();
   bool isInTrans () { return m_fIsInTrans; }

   //---------------------- Работа со свойствами.
   CRsdEnvironment* getEnvironment () const {   return m_pOwner; }
   void setEnvironment ( CRsdEnvironment* env );

   const char* getConString () const { return m_strConString.c_str (); }
   void setConString ( const char* conStr );

   const char* getUser () const { return m_strUser.c_str (); }
   void setUser ( const char* usr );

   const char* getPassword () const {   return m_strPassword.c_str (); }
   void setPassword ( const char* pwd );

   const char* getCustomConString () const {    return m_strCustomConnStr.c_str (); }
   void  setCustomConString ( const char* custConnStr );

   long getTimeout () const { return m_lTimeout; }
   void setTimeout ( long t );

   long getState () const { return m_lState; }
   std::string getDbmsName ();
   bool isPostgres();

   bool TestTable (const char* TableName);
   void cancelCurrentQuery ();

   // Метод возвращает ODBC хэндл для соединения, в параметре hEnv возвращает ODBC хэнд окружения.
   RSDHANDLE getOdbcHandle(RSDPHANDLE hEnv = NULL);


protected:
   friend class CRsdCommand;
   friend class RsdConnection;

   void makeopen ();

   std::string m_strConString;
   std::string m_strUser;
   std::string m_strPassword;
   std::string m_strCustomConnStr;

   long        m_lTimeout;
   long        m_lState;

   bool m_fCreator;    // false в случае, когда handle передается при создании,
   // то есть создаем и удаляем handle не мы
   bool m_fIsInTrans;

};

//=============================================================================

//=============================================================================
class RSDDLL CRsdParameter : public CRsdFldBase< IRSCmd, RSDHCM, CRsdCommand >
   {
protected:
   RSDBindParamIO_t     m_lDirection;

   std::string          valName;
   RSDBindParamVer_t    m_ver;

   std::vector<std::string>   blobFileNames;

   CRsdParameter (CRsdCommand* Parent, const char* name, const RSDValType_t type,
            void* buf, long* indlen, long len = 0,
            const RSDBindParamIO_t dir = RSDBP_IN, bool m_wideCharFlag = false) :
         CRsdFldBase<IRSCmd, RSDHCM, CRsdCommand> (Parent, name, type, buf, len, indlen, m_wideCharFlag), 
         m_lDirection (dir),
         valName (""), 
         m_ver (RSDRVER_NONE)
      { 
      reInitParmType (type, buf, len, indlen, m_wideCharFlag);
      }

   long* getIndLenPtr () {return getStatusPtr (); }

public:

   void reInitParmType (RSDValType_t type, void* buf, long len, long* lenOrInd, bool isWide);

   ~CRsdParameter () 
      {
      }

   RSDBindParamIO_t getDirection () { return m_lDirection;   }
   void setDirection (RSDBindParamIO_t dir) { m_lDirection=dir; }

   void setValName (const char* val) { valName = val ? val : ""; }
   const char* getValName () { return valName.c_str (); }

   void setBlobFilename (RSDLPSTR Filename);
   const char* getBlobFilename ();


   void setVersion (RSDBindParamVer_t ver) { m_ver = ver; }
   RSDBindParamVer_t getVersion () { return m_ver; }

protected:

   friend class CRsdCommand;
   friend class CRsdRecordset;
   friend class RsdParameter;

   };

class CRsdParamAccessor;


enum CmdType_t
{
   RsdCmdText = 0,
   RsdCmdTable,
   RsdCmdStoredProc
};

class CRsdNameAccessor;

//=============================================================================
class RSDDLL CRsdCommand : public CRsdBase< RSDHCM, IRSCmd*, CRsdConnection* >
{
public:
   typedef CRsdBase< RSDHCM, IRSCmd*, CRsdConnection* > CParent;

   CRsdCommand () : m_lTimeout (0), m_CmdType (RsdCmdText), m_fPrepared (false),
      m_lOptions (0), m_lState (0), m_lRecsAffected (0), m_pacc (NULL),
      m_CurType (RSDVAL_FORWARD_ONLY), m_BlockSize (1),
      m_pOrderAcc (NULL), m_pWhereAcc (NULL), m_NullConversion (false),
      m_pacc_size (1), m_createBindWhenOpen (true), is_disableOraToPgConverter_(false)
   {
      m_strCmdText = "";
      m_params.clear ();
      //  props.clear ();
   }

   CRsdCommand ( CRsdConnection& con, const char *text = NULL, RSDCursorType_t CursorType = RSDVAL_FORWARD_ONLY,
      const long t = 0, const bool prep = false, const long opts = 0 );

   ~CRsdCommand () { m_params.clear (); close (); }

   void close ();

   RSDRESULT execute ();
   RSDRESULT executeDirect ();

   void cancel ();
   //---------------------- Работа со свойствами.
   CRsdConnection* getConnection () const { return m_pOwner; }
   bool setConnection ( CRsdConnection*con );

   const char* getCmdText () const { return m_strCmdText.c_str (); }
   bool setCmdText ( const char* text );

   long getBlockSize () const { return m_BlockSize; }
   void setBlockSize ( long BlockSize );

   CmdType_t getCmdType () const { return m_CmdType; }
   bool setCmdType ( CmdType_t type );

   bool getPrepared () const { return m_fPrepared; }
   bool setPrepared ( bool prep );

   long getTimeout () const {   return m_lTimeout; }
   bool setTimeout ( long t );

   long getOptions () const {   return m_lOptions; }
   bool setOptions ( long opts );

   long getState () const { return m_lState; }

   RSDCursorType_t getCursorType () const { return m_CurType; }
   bool setCursorType (RSDCursorType_t CursorType);

   long getRecsAffected () { return m_lRecsAffected; }

   void setClientEncoding ( RSDEncoding_t Enc );

   //---------------------- Работа с коллекциями.
   long getParamCount () const { return (long)m_params.size (); }
   long getParamIndex ( const char* name );  
   long getParamIndexEx ( const char* name ); // без exception
   CRsdParameter& getParam ( const long n );
   CRsdParameter& getParam ( const char* name );

   void refreshParams ();

   CRsdParameter* addParam ( const char* name, RSDValType_t type, void* value=0, long* indlen=0, long len=0, RSDBindParamIO_t dir = RSDBP_IN, bool isWide = false);
   CRsdParameter* addParam ( const char* paramName,  const char* valName, RSDBindParamVer_t ver, RSDBindParamIO_t dir = RSDBP_IN, bool isWide = false );
   CRsdParameter* insertParam ( long pos, const char* name, RSDValType_t type, void* value, long* indlen, long len=0, RSDBindParamIO_t dir = RSDBP_IN, bool isWide = false);

   void deleteParam ( int n );
   void deleteParam ( const char* name );

   void clearParams () { m_params.clear (); }

   void setParamAccessor ( CRsdParamAccessor* pa, size_t acc_size = 1 )
   { m_pacc = pa; m_pacc_size = acc_size; }

   size_t getParamArraySize () { return m_pacc_size; }

   void setParamArraySize (size_t p_size)
   { 
      m_pacc_size = p_size;
      if (m_fOpened && m_pacc_size != p_size)
         rebind (); 
   }

   void setOrderAccessor ( CRsdNameAccessor* oa )
   { m_pOrderAcc = oa; }

   void setWhereAccessor ( CRsdNameAccessor* wa )
   { m_pWhereAcc = wa; }

   void check ( RSDRESULT res, XRsdError::Source source )
   {
      GetEnvironment ()->check ( res, source ); 
   }

   void makeopen (bool isDirect);
   void makeopen ();

   void reset ();
   void rebind ()
   {      
      reset ();
      createBinding ();
   }
   
   void PutData (RSDLONG nOrdinal);

   void setNullConversion (bool bEnable);
   bool getNullConversion () { return m_NullConversion; }

   void setDisableOraToPgConverter(bool bEnable);
   bool getDisableOraToPgConverter();

   CRsdEnvironment* GetEnvironment () const
   {
      if (m_pOwner)
         return m_pOwner->GetOwner ();
      else
         return NULL;
   }

protected:
   friend class CRsdRecordset;
   friend class RsdCommand;
   friend class RsdRecordset;

   void createBinding ();
   RSDRESULT executeImpl(bool isDirect);

   std::string m_strCmdText;
   long    m_lTimeout;
   CmdType_t m_CmdType;
   bool    m_fPrepared;
   long    m_lOptions; //!!!
   long    m_lState;
   long    m_lRecsAffected;

   bool    m_createBindWhenOpen;

   RSDLONG m_BlockSize;

   RSDCursorType_t m_CurType;

   typedef PtrVector< CRsdParameter* > ParamsCollection;
   ParamsCollection m_params;

   CRsdParamAccessor* m_pacc;
   CRsdNameAccessor* m_pOrderAcc, *m_pWhereAcc;
   RSDBIND m_bindID;
   size_t m_pacc_size;

   bool m_NullConversion;
   RSDBYTE isDirect_;
   RSDLONG is_disableOraToPgConverter_;

};

class CRsdAccessor;
class CRsdDynAccessor;

class CRSetOverride
{
public:
   virtual void makeopen () = 0;
};

class RSDDLL CRsdRecordset : public CRsdBase< RSDHRS, IRSRecordSet*, CRsdCommand* >
{
public:
   typedef CRsdBase< RSDHRS, IRSRecordSet*, CRsdCommand* > CParent;

   CRsdRecordset ();
   CRsdRecordset ( CRsdCommand& cmd,
      RSDCursorLocation_t CursorLocation = RSDVAL_CLIENT,
      RSDCursorType_t CursorType = RSDVAL_FORWARD_ONLY,
      long PageSize = 10, long MaxPages = -1
      //long cursloc = 0, long lock = 0
      );

   CRsdRecordset (CRsdConnection&      con, 
      RSDLPSTR             TableName, 
      RSDRSNAMEBINDING*    PrimaryKey, 
      RSDRSNAMEBINDING**   keyDef,
      RSDRSNAMEBINDING*    recDef,
      RSDADVHINTS*         advHints,
      void*                pStatus,
      const char*          defFlds,
      const char*          depTabs,
      const char*          joinCond,
      const char*          whereCond,
      long                 masterCols,
      bool                 blob,
      long                 PageSize,
      RSDRSNAMEBINDING*    sort = NULL
      );

   ~CRsdRecordset();


   void open ()
   {
      if (!m_override)
         makeopen ();
      else
         m_override->makeopen ();
   }

   virtual void close ();

   bool move ( RSDLONG recNum, RSDOrientation_t orientation );
   bool moveToBookmark ( RSDBOOKMARK bmark );
   bool moveFirst ();
   bool moveLast ();
   bool moveNext ();
   bool movePrev ();
   bool moreResults ();

   RSDULONG getRecCount ();

   //---------------------- Работа со свойствами.

   void createDefaultAccessor ();

   CRsdCommand* getCommand () { return m_pOwner; }
   void setCommand ( CRsdCommand* cmd );

   bool getBOF () { open (); return RSD_SUCCEEDED (m_pIntf->pBOF (m_handle))==RSDRES_OK; }
   bool getEOF () { open (); return RSD_SUCCEEDED (m_pIntf->pEOF (m_handle))==RSDRES_OK; }

   long getPageSize () { return m_lPageSize; }
   void setPageSize ( long PageSize );

   long getMaxPages () { return m_lMaxPages; }
   void setMaxPages ( long MaxPages );

   RSDCursorLocation_t getCursorLocation () { return m_CursorLocation; }
   void setCursorLocation ( RSDCursorLocation_t CursorLocation );

   RSDCursorType_t getCursorType () { return m_CursorType; }
   void setCursorType ( RSDCursorType_t CursorType );

   bool getAutoTimestamp () const { return m_fAutoTimestamp; }
   void setAutoTimestamp ( bool fAuto );

   long getState () const { return m_lState; }

   long getLockState () const { return m_LockState; }

   long getStatus () const { return m_lStatus; }

   RSDBOOKMARK getBookmark ()
   {
      if ( !IsOpened () ) 
         return 0;
      RSDBOOKMARK bmark;
      m_pIntf->pGetBookmark (m_handle, &bmark);
      return bmark;
   }

   bool AddRefBookmark (RSDBOOKMARK);
   bool ReleaseBookmark (RSDBOOKMARK);

   //---------------------- Работа с коллекциями.
   long getFldCount () const { return (long)m_fields.size(); }
   long getFldIndex ( const char* name );

   CRsdField& getFld ( long n );
   CRsdField& getFld ( const char* name );

   void check ( RSDRESULT res, XRsdError::Source source );

   void addAccessor ( CRsdAccessor* acc );

   void delAccessor ( CRsdAccessor* acc )
   { 
      m_accessors.remove (acc); 
   }

   void updateAccessors ( bool fAll = false );  // обновлять ли и Manual Accessors

   bool updateAccessor ( CRsdAccessor* acc );

   RSDRESULT   Update ();
   void        Edit ();
   void        AddNew ( bool fCopy=false );
   RSDRESULT   Delete ();
   void        CancelEdit ();

   void beginBatch ();
   void endBatch ();

   bool Find ( CRsdNameAccessor& findCond, RSDRSetFindCond cond);
   bool Find (const char* findCond);
   void Lock (int mode);

   virtual void makeopen ();

   void setCachingFind (bool f);
   void setEmptyQuery (bool f);

   void setBackgroundFetch (bool f);

   void setClientEncoding ( RSDEncoding_t Enc );

   enum LockState_t
   {
      LockLocked, 
      LockFailed, 
      LockNolock
   };

   LockState_t getLockState () { return m_LockState; }

   void reset ();
   void refresh ();
   long getCurPos () const;

   typedef std::vector<RSDRSNAMEBINDING> NameBindVec_t;
   void GetPKInfo (NameBindVec_t& columns, NameBindVec_t& pseudo);
   void PutData (RSDLONG nOrdinal);

   void setNullConversion (bool bEnable);
   bool getNullConversion ();

   void setDisableOraToPgConverter(bool bEnable);
   bool getDisableOraToPgConverter();

   void SwitchRecordset (RSDKEY nKey, bool bUnique); //, RSDLONG KeyKind);
   void setTableName (RSDLPSTR TableName);
   void setPrimaryKey (RSDRSNAMEBINDING* primaryKey);
   void setKeyDef (RSDRSNAMEBINDING** nameBind);
   void setRecDef (RSDRSNAMEBINDING* recDef);

   void setDepFields (const char* s) { m_depfields = s; }
   void setDepTables (const char* s) { m_deptables = s; }
   void setJoinCond  (const char* s) { m_joincond = s;  }
   void setWhereCond (const char* s) { m_wherecond = s; }

   const char* getDepFields () { return m_depfields.c_str (); }
   const char* getDepTables () { return m_deptables.c_str (); }
   const char* getJoinCond  ()  { return m_joincond.c_str ();  }
   const char* getWhereCond () { return m_wherecond.c_str (); }
   const char* getToolsWhereCond () { return m_toolswherecond.c_str (); }
   const char* getSortString () { return m_sort.c_str (); }
   std::string getSortStringEx () { return m_sort_ex; }

   bool ifWasFirstMove () {return !fFirstMove;}

   char* getAllRecordsSql (char* sql);
   const char* getTableName (char* str);

   void setSort (const char* sort);
   void setFilter (const char* filter);
   void setFilter (int filter);
   const char* getFilter () { return m_filter.c_str (); }

   void setSortString (const char* sort);
   void setToolsWhereCond (const char* toolsWhereCond);
   void applyToolsWhereCond (bool bApply);
   int  getBlobLimitSize () {return blobLimit();}
   void setBlobLimitSize (int maxSize);
   void setLockType (int lockType);
   void setFullShiftMode (int mode);
   void EnablePassiveControl (bool val);

   bool setAutoincField(int fldNum);
   bool setAutoincField(const char* fldName);
   void setSequenceName(const char* seqName);

   void setRepairUpdatedCacheFieldsNullData (bool bEnable);
   bool getRepairUpdatedCacheFieldsNullData (); 

   int getAutoincField() {return m_aincFld;}
   const char* getSequenceName() {return m_seqName.c_str();}

   void addAdditionalParam (RSDValType_t type, void* val, long size);
   void getAdditionalParams (RSDBINDING** pBind, long* count);
   void resetAdditionalParams ();

   //  функции поиска по ключу и первичному ключу.
   bool PKFind (void* buffer, bool useCache);
   bool KeyFind (void* buffer, RSDRSetFindCond cond);

   virtual CRsdEnvironment* GetEnvironment () const
   {
      // так как наследуемся от Recordset , у которого Owner - это Command, то приходится хранить указатель
      // на Connection в виде указателя на Command.
      // пока так
      if (m_pOwner)
      {
         if (m_isMulti)
            return reinterpret_cast<CRsdConnection*> (m_pOwner)->GetOwner ();
         else
            return m_pOwner->GetEnvironment ();
      }
      else
         return NULL;
   }

   void addField (const char *name, RSDValType_t type, int len = 0, int pos = -1);

   bool setSpecField (long numf, int type);
   long getSpecField (int type);     

   // пользовательские команды для изменения,
   // для обновления данных и получения данных 
   // после вставки (автоинкремент, результат триггеров и т.п.)
   void setUserCmd (CRsdCommand* userCmd, RSDCmdType_RSet_t cmdType);
   void activateUserCmd (RSDCmdType_RSet_t cmdType);
   void activateEditUserCmd ();
   void activateDeleteUserCmd ();
   CRsdCommand* getUserCmd (RSDCmdType_RSet_t cmdType);
   void setAutoRefresh (bool bAuto);
   bool getAutoRefresh ();
   void refreshRecord ();

   bool IsEnableTrace(void);
   HTRACE GetODBCTraceHandle(void);
   HTRACE GetMODBCTraceHandle(void);

public:
   // Статика для нужд трассировки
   static HTRACE  st_hOdbcTrace;
   static HTRACE  st_hMOdbcTrace;
   static int     st_TraceCnt;
   static int     st_MTraceCnt;

protected:

   friend class CRsdEnvironment;
   friend class RsdRecordset;

   void _internalInit ();

   void AdjustCommand ();
   // "вправляет мозги" команде, если она малофункциональна для затребованного курсора
   void setLockState ();

   void makeopenMulti ();      
   virtual void closeMulti ();
   // Owner у Multi - это Connection, в отличие от Command у Recordset. Поэтому все ф-ии работающие
   // с Owner, и использующие методы Command, которых нет у Connection Должны быть переписаны
   bool moveMulti ( RSDLONG recNum, RSDOrientation_t orientation );
   bool moveFirstMulti ();
   bool moveLastMulti ();
   bool movePrevMulti ();

   // а какой смысл это имеет для Мулти?    
   void AdjustCommandMulti (){}
   void setCursorLocationMulti ( RSDCursorLocation_t CursorLocation ){}
   void setCursorTypeMulti ( RSDCursorType_t CursorType ){}

   CRsdField* make_CRsdField(CRsdRecordset* Parent, const char* name, const RSDValType_t type,
            void* buf, long len = 0, long* status = NULL, RSDLONG ciPrecision = 0);

   const int& blobLimit();
   void setBlobLimit(int val);
   const int& blobMode();
   void setBlobMode(int val);

   long m_lPageSize;
   long m_lMaxPages;
   long m_recSize;

   RSDCursorLocation_t  m_CursorLocation;
   RSDCursorType_t      m_CursorType;

   long     m_lState;
   long     m_lStatus;

   void*    m_hLib;       // Дескриптор библиотеки Recordset'а

   typedef PtrVector< CRsdField* > FieldsCollection;
   //  typedef std::vector< CRsdField* > FieldsCollection;
   FieldsCollection  m_fields;

   CRsdDynAccessor*  m_pAcc;    // указатель на аксессор по умолчанию.
   // используется для binding к fields
   RSDBYTE*          m_buffer;          // буфер для биндинга дефолтного аксессора,
   // включая поле статуса

   typedef std::list< CRsdAccessor* > AccessorsCollection;
   AccessorsCollection m_accessors;    // коллекция для хранения акссессоров.

   bool m_fAutoTimestamp;

   //  std::list<SRsdProperty> props;
   friend class CRsdAccessor;

   bool        fFictiveMove;  // был ли фиктивный Move
   bool        fFirstMove;
   RSDLock_t   m_LockSet;
   LockState_t m_LockState;

   bool        m_isMulti;

   // поля для multirecordset
   RSDLPSTR    m_TableName;

   RSDRSNAMEBINDING* m_PrimaryKey;

   RSDKEY      m_KeyNum;

   RSDRSNAMEBINDING** m_keyDef;
   RSDRSNAMEBINDING*  m_recDef; // запись, как она определена в словаре

   RSDRSNAMEBINDING*  m_SortDef;

   RSDULONG    m_maxPages;

   void*       m_hLib2; // для того чтобы получить инт-с команды
   IRSCmd*     m_pCmdIntf;

   void*       m_status;

   std::string m_depfields;
   std::string m_deptables;
   std::string m_joincond;
   std::string m_wherecond;
   std::string m_toolswherecond;

   std::string m_sort;
   std::string m_sort_ex;

   std::string m_filter;   

   RSDULONG    m_mastercols;

   RSDADVHINTS* m_advHints;

   bool        m_blobField;
   bool        m_blobWriteInMem;
   
   //int         m_blobLimit;
   //int         m_blobMode;
   not_boost::optional_referenceable<int>         m_blobLimit;
   not_boost::optional_referenceable<int>         m_blobMode;
   int         m_lockType;
   bool        m_bAutoRefresh;
   bool        m_NullConversion;
   int         m_aincFld;
   std::string      m_seqName;

   CRSetOverride* m_override;
   NameBindVec_t  columns,pseudo;

   CRsdCommand* m_userCmds [USERCMDSN];
   bool        checkNumber10_;
   std::string m_strTableName;
};


enum RsdCondition_t
{
   RsdCondEQ = RSDFC_EQ,
   RsdCondLT = RSDFC_LT,
   RsdCondGT = RSDFC_GT,

   RsdCondAsc = RsdCondGT,
   RsdCondDesc = RsdCondLT
};

struct NameBinding_t
{
   NameBinding_t 
   (
      const char*    _FieldName, 
      void*          _Value, 
      RSDValType_t   _Type, 
      RsdCondition_t _Cond, 
      RSDLONG        _Len
   ):
      FieldName (_FieldName), 
      Value (_Value), 
      Type (_Type), 
      Cond (_Cond), 
      Len (_Len)
{}

   std::string FieldName;
   void* Value;
   RSDValType_t Type;
   RsdCondition_t Cond; 
   RSDLONG Len;
};

class NameBindList_t : public std::list< NameBinding_t >
{
public:
   NameBindList_t (){}
   bool IsPresented ( const std::string& name ) const
   {
      for ( const_iterator i = begin (); i!=end (); i++ )
      {
         if ( name==i->FieldName ) return true;
      }
      return false;
   }
};

class CRsdNameAccessor
{
public:
   CRsdNameAccessor (){}

   void AddBinding ( const char* FieldName, RsdCondition_t Cond )
   {
      m_Bindings.push_back (NameBinding_t (FieldName, NULL, RSDPT_LONG, Cond, 0));
   }

   void AddBinding ( const char* FieldName, void* Value, RSDValType_t Type, RsdCondition_t Cond, RSDLONG Len = 0 )
   {
      m_Bindings.push_back (NameBinding_t (FieldName, Value, Type, Cond, Len));
   }

   const NameBindList_t& getBindings () const { return m_Bindings; }
protected:
   NameBindList_t m_Bindings;
};


// класс аксессора, абстрактный
//=============================================================================
class RSDDLL CRsdAccessor
{
public:
   RSDACCDESC getHandle () { return m_handle; }

   void SetOwner (CRsdRecordset* pRS) { m_pOwner = pRS; }

   virtual RSDVALUEPTR  getBase ()=0;

   void AddNew ()
   {
      if(m_pOwner)
      {
         m_pOwner->activateEditUserCmd ();

         bool fSaveAuto = m_fAuto;
         m_fAuto = false;
         m_EditMode = RSDINSERT;
         if ( m_pOwner->getRecCount () == 0 )
         {
            try
            {
               m_pOwner->setEmptyQuery (true);
               if ( m_pOwner->moveNext () )
                  m_pOwner->fFictiveMove = true;
            }
            catch (...)
               // даже если что-то произошло, то ничего фатального быть не должно
            {
               m_pOwner->fFictiveMove = false;
            }
         }
         m_pOwner->setEmptyQuery (false);
         m_fAuto = fSaveAuto;
         m_pOwner->check ( m_pOwner->m_pIntf->pInsert (m_pOwner->m_handle, m_handle), XRsdError::etData );
      }
   }

   RSDRESULT Update ( /*const*/ RSDLONG Fields[] )
   {
      RSDRESULT res = m_pOwner->m_pIntf->pUpdate (m_pOwner->m_handle, m_handle,Fields);
      if (res != RSDRES_NODATA) m_pOwner->check ( res, XRsdError::etData );
      m_EditMode = RSDNONE;
      m_pOwner->updateAccessors ();
      return res;
   }

   void Edit ()
   {
      if(m_pOwner)
      {
         m_pOwner->activateEditUserCmd ();

         m_EditMode = RSDEDIT;
         if ( m_pOwner->getBOF () || m_pOwner->getEOF () )
            throw XRsdError ( XRsdError::etData, 
            "Can't update unpositioned cursor", m_pOwner, false );
         m_pOwner->check ( m_pOwner->m_pIntf->pEdit (m_pOwner->m_handle, m_handle), XRsdError::etData );
      }
   }

   void SetData (RSDLONG Fields[])
   {
      m_pOwner->check ( m_pOwner->m_pIntf->pSetData (m_pOwner->m_handle, m_handle,
         Fields), XRsdError::etData );
      m_EditMode = RSDNONE;
   }

   RSDRESULT  Delete ()
   {
      RSDRESULT res = RSDRES_NODATA;

      if(m_pOwner)
      {
         m_pOwner->activateDeleteUserCmd ();

         if ( m_pOwner->getBOF () || m_pOwner->getEOF () )
            throw XRsdError ( XRsdError::etData, 
            "Can't update unpositioned cursor", m_pOwner, false );

         res = m_pOwner->m_pIntf->pDelete (m_pOwner->m_handle);
         if (res != RSDRES_NODATA) 
            m_pOwner->check ( res, XRsdError::etData );

         m_EditMode = RSDNONE;
      }
      return res;
   }

   void CancelEdit ()
   {
      m_pOwner->check ( m_pOwner->m_pIntf->pCancelEdit (m_pOwner->m_handle, m_handle), XRsdError::etData );
      m_EditMode = RSDNONE;
   }

   RSDEDIT_MODE getEditMode () { return m_EditMode; }

protected:
   CRsdAccessor ( bool fAuto=true ) : m_fAuto (fAuto), m_handle (0), m_pOwner (NULL), m_EditMode (RSDNONE) {}   
   ~CRsdAccessor ()
   {
      if ( m_pOwner )
         // перед смертью удалим себя из списка Recordset'а
         m_pOwner->delAccessor ( this );
   }

   virtual RSDRSETBINDING* getBindings ()=0;

   // макросы для создания статических аксессоров.
   // offsetof требует stddef.h

#define RSD_TYPE(type, fld) CRsdFldBase< IRSRecordSet, RSDHRS, CRsdRecordset >::getRsdType ( ( (type*) (0))->fld)
#define RSD_SIZE(type, fld) sizeof ( ( (type*) (0))->fld)

#define BEGIN_RSD_BINDING( classname ) \
public: \
   classname## ( bool fAuto=true ): CRsdAccessor (fAuto){}\
   typedef classname RsdBindType;  \
   virtual RSDVALUEPTR getBase () {return this;}\
   virtual RSDRSETBINDING* getBindings ()   \
   {   \
   static RSDRSETBINDING rgRSBindings[] = {

#define RSD_SCALAR_ENTRY( ordinal, value, status )\
   {       \
   ordinal, RSD_TYPE (RsdBindType, value), 0, \
   offsetof (RsdBindType, value), \
   offsetof (RsdBindType, status)   \
   },

#define RSD_SCALAR_ENTRY_EX( ordinal, type, value, status )\
   {       \
   ordinal, type, 0, \
   offsetof (RsdBindType, value), \
   offsetof (RsdBindType, status)   \
   },

#define RSD_SCALAR_ENTRY2( ordinal, value )\
   {       \
   ordinal, RSD_TYPE (RsdBindType, value), 0, \
   offsetof (RsdBindType, value), \
   0   \
   },

#define RSD_SCALAR_ENTRY2_EX( ordinal, type, value )\
   {       \
   ordinal, type, 0, \
   offsetof (RsdBindType, value), \
   0   \
   },

#define RSD_VARIABLE_LENGTH_ENTRY( ordinal, type, length, value, status )\
   {       \
   ordinal, type, length, \
   offsetof (RsdBindType, value), \
   offsetof (RsdBindType, status)   \
   },

#define RSD_VARIABLE_LENGTH_ENTRY_EX( ordinal, value, status )\
   {       \
   ordinal, RSD_TYPE (RsdBindType, value), RSD_SIZE (RsdBindType, value), \
   offsetof (RsdBindType, value), \
   offsetof (RsdBindType, status)   \
   },

#define RSD_VARIABLE_LENGTH_ENTRY2( ordinal, type, length, value )\
   {       \
   ordinal, type, length, \
   offsetof (RsdBindType, value), \
   0   \
   },

#define RSD_VARIABLE_LENGTH_ENTRY2_EX( ordinal, value )\
   {       \
   ordinal, RSD_TYPE (RsdBindType, value), RSD_SIZE (RsdBindType, value), \
   offsetof (RsdBindType, value), \
   0   \
   },

#ifdef RSL_PL_WIN64
    #define END_RSD_BINDING() {0,0,0, (RSDBIGINT)this,0} };\
       return rgRSBindings;    \
       }
#else
    #define END_RSD_BINDING() {0,0,0, (RSDLONG)this,0} };\
       return rgRSBindings;    \
       }
#endif

   friend class CRsdRecordset;

   bool m_fAuto;
   RSDACCDESC m_handle;
   CRsdRecordset* m_pOwner;

   RSDEDIT_MODE m_EditMode;
   //  void* m_pOrigin;
};


// класс динамического аксессора.
//=============================================================================
class RSDDLL CRsdDynAccessor : public CRsdAccessor
{
public:
   CRsdDynAccessor ( bool fAuto=true )
      : CRsdAccessor (fAuto), m_rgRSBindings (NULL) { } 

      ~CRsdDynAccessor ()
      {
         if ( m_rgRSBindings ) delete[] m_rgRSBindings;
      }

      virtual RSDVALUEPTR  getBase () {return this;};

      virtual RSDRSETBINDING* getBindings ();

      struct AccBinding
      {
         RSDULONG       ordinal;
         RSDValType_t   type;
         long           length;
         void*          pVal;
         long*          pStatus;
      };


      int AddBinding ( RSDULONG ordinal, RSDValType_t type, void* pVal, long length, long* pStatus  )
      {
         AccBinding b = { ordinal, type, length, pVal, pStatus };
         m_bindings.push_back ( b );
         return (int)m_bindings.size();
      }

      int InsertBinding ( long pos, RSDULONG ordinal, RSDValType_t type, void* pVal, long length,  long* pStatus )
      {
         AccBinding b = { ordinal, type, length, pVal, pStatus };
         m_bindings.insert ( m_bindings.begin ()+pos, b );
         return pos++;
      }
      void RemoveBinding ( long pos )
      {
         m_bindings.erase ( m_bindings.begin ()+pos );
      }
protected:
   typedef std::vector< AccBinding > BindingsCollection;
   BindingsCollection m_bindings;
   RSDRSETBINDING* m_rgRSBindings;
   // коллекция bindings. Нужна только до момента открытия,
   // во время открытия из нее создается массив RSDBINDINGS,
   // который и используется для построения привязок.

};

class RSDDLL CRsdParamAccessor
{
public:

   virtual int getSize () = 0;
   virtual RSDBINDING* getBindings () = 0;

protected:
   CRsdParamAccessor (){}   
   ~CRsdParamAccessor () {}

#define BEGIN_RSD_PARAM_BINDING( classname ) \
public: \
   classname## (): CRsdParamAccessor (){}\
   typedef classname RsdBindType;  \
   virtual int getSize ()   \
   {   \
   return sizeof (*this);   \
   }   \
   virtual RSDBINDING* getBindings ()   \
   {   \
   static RSDBINDING rgBindings[] = {

#define RSD_SCALAR_PARAM_ENTRY( ordinal, value, indicator )\
   {       \
   ordinal, RSDBP_IN, RSD_TYPE (RsdBindType, value), \
   &value, & (indicator=sizeof (value)), \
   0, 0, 0               \
   },

#define RSD_SCALAR_PARAM_ENTRY_EX( ordinal, type, value, indicator )\
   {       \
   ordinal, RSDBP_IN, type, \
   &value, & (indicator=CRsdFldBase::getRsdTypeSize (type)), \
   0, 0, 0                \
   },

#define RSD_SCALAR_PARAM_ENTRY2( ordinal, value, indicator, dir )\
   {       \
   ordinal, dir, RSD_TYPE (RsdBindType, value), \
   &value, & (indicator=sizeof (value)), \
   0, 0, 0                \
   },

#define RSD_SCALAR_PARAM_ENTRY2_EX( ordinal, type, value, indicator, dir )\
   {       \
   ordinal, dir, type, \
   &value, & (indicator=CRsdFldBase::getRsdTypeSize (type)), \
   0, 0, 0                \
   },

#define RSD_VARIABLE_LENGTH_PARAM_ENTRY( ordinal, type, length, value, indicator )\
   {       \
   ordinal, RSDBP_IN, type, \
   &value, & (indicator=length),    \
   0, length, 0           \
   },

#define RSD_VARIABLE_LENGTH_PARAM_ENTRY_EX( ordinal, value, indicator )\
   {       \
   ordinal, RSDBP_IN, RSD_TYPE (RsdBindType, value), \
   &value, & (indicator=RSD_SIZE (RsdBindType, value)),  \
   0, RSD_SIZE (RsdBindType, value), 0     \
   },

#define RSD_VARIABLE_LENGTH_PARAM_ENTRY2( ordinal, type, length, value, indicator, dir )\
   {       \
   ordinal, dir, type,     \
   &value, & (indicator=length),    \
   0, length, 0           \
   },

#define RSD_VARIABLE_LENGTH_PARAM_ENTRY2_EX( ordinal, value, indicator, dir )\
   {       \
   ordinal, dir, RSD_TYPE (RsdBindType, value), \
   &value, & (indicator=RSD_SIZE (RsdBindType, value)),  \
   0, RSD_SIZE (RsdBindType, value), 0     \
   },

#define END_RSD_PARAM_BINDING() {0,0,0,0,0,0,0,0} };\
   return rgBindings;  \
   }

   friend class CRsdCommand;

};

#pragma warning ( pop )

#ifdef _MSC_VER
#undef stricmp
#endif

#endif