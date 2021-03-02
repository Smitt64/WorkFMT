/*-----------------------------------------------------------------------*-

 File Name   : misc.hpp  

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 July 8,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __MISC_HPP
#define __MISC_HPP

// -----------------------------------------------------------------------------
#define  RSL_DELEGATE(tpname, parmlist, calllist)                   \
           class tpname                                             \
           {                                                        \
            public:                                                 \
              virtual void runEvent parmlist = 0;                   \
              virtual ~##tpname () {}                               \
           };                                                       \
           template <class Object>                                  \
           class tpname##Impl : public tpname                       \
           {                                                        \
            public:                                                 \
              typedef void (Object::*TMemberFun) parmlist;          \
              tpname##Impl (Object *ob, TMemberFun mem) :           \
                              obj(ob), member(mem) {}               \
              void runEvent parmlist { (obj->*member) calllist; }   \
           private:                                                 \
              Object        *obj;                                   \
              TMemberFun  member;                                   \
           } 

// -----------------------------------------------------------------------------
#define  RSL_DELEGATE_(tpname, retval, parmlist, calllist)                 \
           class tpname                                                    \
           {                                                               \
           public:                                                         \
              virtual retval runEvent parmlist = 0;                        \
              virtual ~##tpname () {}                                      \
           };                                                              \
           template <class Object>                                         \
           class tpname##Impl : public tpname                              \
           {                                                               \
           public:                                                         \
              typedef retval (Object::*TMemberFun) parmlist;               \
              tpname##Impl (Object *ob,TMemberFun mem) :                   \
                              obj(ob), member(mem) {}                      \
              retval runEvent parmlist { return (obj->*member) calllist; } \
           private:                                                        \
              Object        *obj;                                          \
              TMemberFun  member;                                          \
           }


// -----------------------------------------------------------------------------
class TObjHolder
    {
     public:

       TObjHolder(void *p) : refCount (1) { the_p = p; }

       unsigned AddRef()  { return ++refCount; }
       unsigned Release() { return --refCount; }

       void *getPtr() { return the_p; }

     private:
       void  *the_p;
       unsigned  refCount;
    };

// -----------------------------------------------------------------------------
template<class X>
class TRefPtr
    {
     public:
       explicit TRefPtr (X* p = 0) : the_p(0) { AssignObj (p); }
       TRefPtr(TRefPtr<X> &a) : the_p(0) { AssignObj(a); }
      ~TRefPtr() { DecRef(); }

       void operator = (TRefPtr<X> &a) { DecRef(); AssignObj(a); }
       void operator = (X *p)          { DecRef(); AssignObj(p); }


       X &operator *  () const { return *static_cast<X *>(the_p->getPtr()); }
       X *operator -> () const { return  static_cast<X *>(the_p->getPtr()); }
       X *get         () const { return  static_cast<X *>(the_p->getPtr()); }

     private:

       void AssignObj(X *p)
          {
           if(p)
             the_p = new TObjHolder(p);
          }

       void AssignObj(TRefPtr<X> &a)
          {
           if(a.the_p)
            {
             the_p = a.the_p;
             the_p->AddRef();
            }
          }

       void DecRef()
          {
           if(the_p && !the_p->Release())
            {
             delete static_cast<X *>(the_p->getPtr());
             delete the_p;

             the_p = 0;
            }
          }

       TObjHolder  *the_p;
    };

// -----------------------------------------------------------------------------
template<class X> class TXptr
{
 public:
   //
   // construct/copy/destroy
   //
   explicit TXptr(X *p = 0) : the_p(p) {}
   TXptr(TXptr<X> &a) : the_p(a.release()) {}

   void operator = (TXptr<X> &rhs) { reset(rhs.release()); }
   void operator = (X *rhs) { if(the_p) delete the_p; the_p = rhs; }

  ~TXptr()
   {
    if(the_p)
      delete the_p;
   }
   //
   // members
   //
   const X &operator * ()  const { return *the_p; }
   X *operator-> ()        const { return the_p; }
   X *get        ()        const { return the_p; }
   X *release    ()              { return reset(0); }
   X *reset      (X *p = 0)      { X *tmp = the_p; the_p = p; return tmp; }
   bool operator ! ()      const { return !the_p; }

 private:

   X  *the_p;
};

// -----------------------------------------------------------------------------
// Use free to delete memory.
template<class X> class TXptrC
{
 public:
   //
   // construct/copy/destroy
   //
   explicit TXptrC(X *p = 0) : the_p(p) {}
   TXptrC (TXptrC<X> &a) : the_p(a.release()) {}

   void operator = (TXptrC<X> &rhs) { reset(rhs.release()); }
   void operator = (X *rhs) { if(the_p) free(the_p); the_p = rhs; }

  ~TXptrC()
   {
    if(the_p)
      free(the_p);
   }
   //
   // members
   //
   const X &operator * ()  const { return *the_p; }
   X *operator-> ()        const { return the_p; }
   X *get        ()        const { return the_p; }
   X *release    ()              { return reset(0); }
   X *reset      (X *p = 0)      { X* tmp = the_p; the_p = p; return tmp; }
   bool operator ! ()      const { return !the_p; }

 private:

   X  *the_p;
};

// -----------------------------------------------------------------------------
class TEvent
    {
     public:
       TEvent(HANDLE h) : hd (h) {}

       TEvent(bool isMan = true, bool isSign = false, bool notCreate = false, const char *name = NULL)
       {
        if(notCreate)
          hd = 0;
        else
          hd = CreateEvent(NULL, (BOOL)isMan, (BOOL)isSign, name);
       }

       void create(bool isMan = true, bool isSign = false)
       {
        if(!hd)
          hd = CreateEvent(NULL, (BOOL)isMan, (BOOL)isSign, NULL);
       }

      ~TEvent() { if(hd) CloseHandle(hd); }

       void reset() { ResetEvent(hd); }
       void signal() { SetEvent(hd); }
       HANDLE getHandle() const { return hd; }
       PHANDLE getRef() { return &hd; }
       bool isValid() const { return (hd != NULL); }
       bool operator ! () const { return !isValid (); }
       operator HANDLE () const { return hd; }

     private:
       HANDLE  hd;
    };


// -----------------------------------------------------------------------------
class THandle
    {
     public: 
       THandle() : hd(INVALID_HANDLE_VALUE) {}
       THandle(HANDLE h) : hd(h) {}
      ~THandle() { close (); }
       THandle(THandle &x) : hd(x.release ()) {}

       void operator = (HANDLE h) { close (); hd = h; }
       HANDLE release() { HANDLE tmp = hd; hd = INVALID_HANDLE_VALUE; return tmp; }
       operator HANDLE () const { return hd; }
       bool isValid() const { return (hd != INVALID_HANDLE_VALUE); }
       bool operator ! () const { return !isValid (); }

       void close()
       {
        if(hd != INVALID_HANDLE_VALUE)
          CloseHandle(hd); 

        hd = INVALID_HANDLE_VALUE;
       }

       PHANDLE operator & () { return &hd; }

     private:
       HANDLE  hd;
    };

// -----------------------------------------------------------------------------
class THandleNull
    {
     public: 
       THandleNull() : hd(0) {}
       THandleNull(HANDLE h) : hd(h) {}
      ~THandleNull() { close(); }
       THandleNull(THandleNull &x) : hd(x.release()) {}

       void operator = (HANDLE h) { close (); hd = h; }
       HANDLE release() { HANDLE tmp = hd; hd = 0; return tmp; }
       operator HANDLE () const { return hd; }
       bool isValid() const { return hd != 0; }
       bool operator ! () const { return !isValid(); }
       void close() { if(hd) CloseHandle(hd); hd = 0; }
       PHANDLE operator & () { return &hd; }

     private:
       HANDLE  hd;
    };

// -----------------------------------------------------------------------------
class TXEvent : public TEvent
{
 public:
   TXEvent(bool isMan = true, bool isSign = false, bool notCreate = false, const char *name = NULL);
};

// -----------------------------------------------------------------------------
class TSocket
    {
     public: 
       TSocket() : sock(INVALID_SOCKET) {}
       TSocket(SOCKET h) : sock(h) {}
      ~TSocket() { if(sock != INVALID_SOCKET) closesocket(sock); }
       TSocket(TSocket &x) : sock (x.release ()) {}

       void operator = (SOCKET h) { sock = h; }
       SOCKET release() { SOCKET tmp = sock; sock = INVALID_SOCKET; return tmp; }
       operator SOCKET () const { return sock; }
       bool isValid() const { return (sock != INVALID_SOCKET); }
       bool operator ! () const { return !isValid(); }

     private:
       SOCKET  sock;
    };

// -----------------------------------------------------------------------------
enum THttpRequestType {REG_BAD, REG_UNSUPPORT, REG_GET, REG_HEAD, REG_POST};

class TCriticalSection
    {
     public:
       TCriticalSection();
      ~TCriticalSection();

       void Enter ();
       void Leave ();

       class Lock
           {
            public:
                Lock(const TCriticalSection &);
                Lock(CRITICAL_SECTION *);
               ~Lock();

            private:
                CRITICAL_SECTION *CritSecObj;
           };                                  

       operator LPCRITICAL_SECTION() const { return CONST_CAST(LPCRITICAL_SECTION, &CritSec); }

       friend Lock;               

     private:
       CRITICAL_SECTION  CritSec;
    };

// -----------------------------------------------------------------------------
inline TCriticalSection::TCriticalSection()
{
 ::InitializeCriticalSection(CONST_CAST(CRITICAL_SECTION *, &CritSec));
}

// -----------------------------------------------------------------------------
inline TCriticalSection::~TCriticalSection()
{
 ::DeleteCriticalSection(CONST_CAST(CRITICAL_SECTION *, &CritSec));
}

// -----------------------------------------------------------------------------
inline void TCriticalSection::Enter()
{
 ::EnterCriticalSection(CONST_CAST(CRITICAL_SECTION *, &CritSec));
}

// -----------------------------------------------------------------------------
inline void TCriticalSection::Leave()
{
 ::LeaveCriticalSection(CONST_CAST(CRITICAL_SECTION *, &CritSec));
}

// -----------------------------------------------------------------------------
inline TCriticalSection::Lock::Lock(const TCriticalSection &sec) : CritSecObj(CONST_CAST(CRITICAL_SECTION *, &sec.CritSec))
{
 ::EnterCriticalSection(CritSecObj);
}

// -----------------------------------------------------------------------------
inline TCriticalSection::Lock::Lock(CRITICAL_SECTION *sec) : CritSecObj(sec)
{
 ::EnterCriticalSection(CritSecObj);
}

// -----------------------------------------------------------------------------
inline TCriticalSection::Lock::~Lock()
{
 ::LeaveCriticalSection(CritSecObj);
}

#endif // __MISC_HPP

/* <-- EOF --> */