/*************************************************************************
   File Name      : objlink.h
   Programmer     : Alexey Lebedev

   Creation       : 12 Feb 2002
   Description    : TObjectLink, TListLink

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __OBJLINK_H
#define __OBJLINK_H

#include "rslist.h"


/******************************************************************************
   struct TBaseObjectLink<TParentObject, TLinkedObject>

    ��������� �������:
      TParentObject - ������-�������� ������
      TLinkedObject - ������, �� ������� ��������� TParentObject
******************************************************************************/
template <class TParentObject, class TLinkedObject>
struct TBaseObjectLink
     {
        typedef TBaseObjectLink<TParentObject, TLinkedObject>  TBaseForwardLink;
        typedef TBaseObjectLink<TLinkedObject, TParentObject>  TBaseReverseLink;

        // ������������
        TBaseObjectLink()                            { _parent = NULL;    }
        TBaseObjectLink(TParentObject &parent)       { _parent = &parent; }

        // ����������
        virtual ~TBaseObjectLink()                   {}

        // ����������� �����������
        TBaseObjectLink(const TBaseForwardLink &x)   { _parent = x._parent; }

        // �������� ������������
        const TBaseForwardLink &operator = (const TBaseForwardLink &x)
        {
         _parent = x._parent;

         return *this;
        }

        // ��������/���������� ������-��������
        TParentObject *getParent() const             { return _parent;    }
        void setParent(TParentObject &parent)        { _parent = &parent; }

        // ��� ������ ����������� � �����������
        virtual bool link(TBaseReverseLink &rlnk)          { return false;}
        virtual bool unlink(const TBaseReverseLink &rlnk)  { return false;}

      private:
        TParentObject  *_parent;
     };


/******************************************************************************
   class TObjectLink
******************************************************************************/
template <class TParentObject, class TLinkedObject>
class TObjectLink : public TBaseObjectLink<TParentObject, TLinkedObject>
    {
       typedef TBaseObjectLink<TParentObject, TLinkedObject>  TBaseForwardLink;
       typedef TBaseObjectLink<TLinkedObject, TParentObject>  TBaseReverseLink;

       typedef TObjectLink<TParentObject, TLinkedObject>  TForwardLink;
       typedef TObjectLink<TLinkedObject, TParentObject>  TReverseLink;

     public:
       // ������������
       inline TObjectLink();
       inline TObjectLink(TParentObject &parent);
       inline TObjectLink(TParentObject &parent, TBaseReverseLink &rlnk);

       // ����������
       inline ~TObjectLink();

       // ����������� �����������
       inline TObjectLink(const TForwardLink &lnk);

       // ����������� �����������, ���������� ��������� �� ������-��������
       inline TObjectLink(const TForwardLink &lnk, TParentObject &parent);

       // �������� ������������
       const TForwardLink &operator = (const TForwardLink &lnk);

       // ������� ������
       inline bool createLink(TBaseReverseLink &rlnk);

       // ������� ������
       inline bool removeLink(TBaseReverseLink &rlnk);
       inline bool removeLink(const TLinkedObject &object);

       // ������� ������� ������
       inline bool removeLink();

       // ������ � ������� �� ���������
       TLinkedObject *getLink     () const { return _link ? _link->getParent() : NULL; }
       TLinkedObject *operator -> () const { return _link ? _link->getParent() : NULL; }

     protected:
       inline bool link(TBaseReverseLink &rlnk);
       inline bool unlink(const TBaseReverseLink &rlnk);

     private:
       TBaseReverseLink  *_link;
    };


/******************************************************************************
   class TStaticLink
******************************************************************************/
template <class TParentObject, class TLinkedObject>
class TStaticLink : public TObjectLink<TParentObject, TLinkedObject>
    {
       typedef TBaseObjectLink<TParentObject, TLinkedObject>  TBaseForwardLink;
       typedef TBaseObjectLink<TLinkedObject, TParentObject>  TBaseReverseLink;

       typedef TStaticLink<TParentObject, TLinkedObject>  TForwardLink;
       typedef TStaticLink<TLinkedObject, TParentObject>  TReverseLink;

     public:
       // �����������
       TStaticLink(TParentObject &parent, TBaseReverseLink &rlnk) : TObjectLink(parent, rlnk)
       {
       }

       // ����������
       ~TStaticLink()
       {
       }

       // ������ � ������� �� ������
       TLinkedObject &operator * () const { return *getLink(); }
    };


/******************************************************************************
   class TLinkList
******************************************************************************/
template <class TParentObject, class TLinkedObject>
class TLinkList : public TBaseObjectLink<TParentObject, TLinkedObject>
    {
       typedef TBaseObjectLink<TParentObject, TLinkedObject>  TBaseForwardLink;
       typedef TBaseObjectLink<TLinkedObject, TParentObject>  TBaseReverseLink;

       typedef TLinkList<TParentObject, TLinkedObject>  TForwardLink;
       typedef TLinkList<TLinkedObject, TParentObject>  TReverseLink;

       typedef RSListT<TBaseReverseLink *>  TListImpl;

     public:
       // ������������
       inline TLinkList();
       inline TLinkList(TParentObject &parent);
       inline TLinkList(TParentObject &parent, TBaseReverseLink &rlnk);

       // ����������
       inline ~TLinkList();

       // ����������� �����������
       inline TLinkList(const TForwardLink &lnk);

       // ����������� �����������, ���������� ��������� �� ������-��������
       inline TLinkList(const TForwardLink &lnk, TParentObject &parent);

       // �������� ������������
       const TForwardLink &operator = (const TForwardLink &lnk);

       // ������� ������
       inline bool createLink(TBaseReverseLink &rlnk);

       // ������� ������
       inline bool removeLink(TBaseReverseLink &rlnk);
       inline bool removeLink(const TLinkedObject &object);

       // ������� ������� ������
       inline bool removeLink();


       // ������ � ������ ��������
       typedef TListImpl                            list;
       typedef RSTPNAME TListImpl::iterator         iterator;
       typedef RSTPNAME TListImpl::const_iterator   const_iterator;

       const list &getList() const { return _list; }

     protected:
       inline bool link(TBaseReverseLink &rlnk);
       inline bool unlink(const TBaseReverseLink &rlnk);

     private:
       TListImpl  _list;
    };


/******************************************************************************
   class TObjectLink
******************************************************************************/
template <class TParentObject, class TLinkedObject>
TObjectLink<TParentObject, TLinkedObject>::TObjectLink() : TBaseForwardLink()
{
 _link = NULL;
}

template <class TParentObject, class TLinkedObject>
TObjectLink<TParentObject, TLinkedObject>::TObjectLink(TParentObject &parent) : TBaseForwardLink(parent)
{
 _link = NULL;
}

template <class TParentObject, class TLinkedObject>
TObjectLink<TParentObject, TLinkedObject>::
TObjectLink(TParentObject &parent, TBaseReverseLink &rlnk) : TBaseForwardLink(parent)
{
 _link = NULL;

 createLink(rlnk);
}

template <class TParentObject, class TLinkedObject>
TObjectLink<TParentObject, TLinkedObject>::~TObjectLink()
{
 if(_link)
   _link->unlink(*this);
}

template <class TParentObject, class TLinkedObject>
TObjectLink<TParentObject, TLinkedObject>::TObjectLink(const TForwardLink &lnk) : TBaseForwardLink(lnk)
{
 _link = NULL;

 // ��������� ������
 if(lnk._link)
   createLink(*lnk._link);
}

template <class TParentObject, class TLinkedObject>
const TObjectLink<TParentObject, TLinkedObject> &TObjectLink<TParentObject, TLinkedObject>::
operator = (const TForwardLink &lnk)
{
 // ��������� ������
 if(lnk._link ? createLink(*lnk._link) : removeLink())
 {
  // ������������ ��� �������� ������
  TBaseForwardLink::operator = (lnk);
 }

 return *this;
}

template <class TParentObject, class TLinkedObject>
TObjectLink<TParentObject, TLinkedObject>::
TObjectLink(const TForwardLink &lnk, TParentObject &parent) : TBaseForwardLink(parent)
{
 _link = NULL;

 // ��������� ������
 if(lnk._link)
   createLink(*lnk._link);
}

template <class TParentObject, class TLinkedObject>
bool TObjectLink<TParentObject, TLinkedObject>::createLink(TBaseReverseLink &rlnk)
{
 // ������� ������������� ����� �����, � ����� ��� ��������� ������.
 // ����� ����� ������������, ��� ����������� ������������� ������
 // ��� �������� ����� ������, ��� ��� �� ��������.

 // ����� ��� �����������
 if(_link == &rlnk)
   return true;

 // ���� ����� �� �����������...
 if(!rlnk.link(*this))
   return false;

 // ���� ������ ����, �� ���� ���������
 if(_link && !_link->unlink(*this))
  {
   rlnk.unlink(*this);

   return false;
  }

 // ���� �� ���� ������, ������ ��� Ok!
 _link = &rlnk;

 return true;
}

template <class TParentObject, class TLinkedObject>
bool TObjectLink<TParentObject, TLinkedObject>::removeLink()
{
 if(!_link)
    return false;

 if(_link->unlink(*this))
  {
   _link = NULL;

   return true;
  }

 return false;
}

template <class TParentObject, class TLinkedObject>
bool TObjectLink<TParentObject, TLinkedObject>::removeLink(TBaseReverseLink &rlnk)
{
 if(_link != &rlnk)
   return false;

 if(_link->unlink(*this))
  {
   _link = NULL;
   return true;
  }

 return false;
}

template <class TParentObject, class TLinkedObject>
bool TObjectLink<TParentObject, TLinkedObject>::removeLink(const TLinkedObject &object)
{
 if(!_link || _link->getParent() != &object)
    return false;

 if(_link->unlink(*this))
  {
   _link = NULL;
   return true;
  }

 return false;
}

template <class TParentObject, class TLinkedObject>
bool TObjectLink<TParentObject, TLinkedObject>::link(TBaseReverseLink &rlnk)
{
 if(_link && !_link->unlink(*this))
   return false;

 _link = &rlnk;

 return true;
}

template <class TParentObject, class TLinkedObject>
bool TObjectLink<TParentObject, TLinkedObject>::unlink(const TBaseReverseLink &rlnk)
{
 return _link == &rlnk ? _link = NULL, true : false;
}


/******************************************************************************
   class TLinkList
******************************************************************************/
template <class TParentObject, class TLinkedObject>
TLinkList<TParentObject, TLinkedObject>::TLinkList() : TBaseForwardLink()
{
}

template <class TParentObject, class TLinkedObject>
TLinkList<TParentObject, TLinkedObject>::
TLinkList(TParentObject &parent) : TBaseForwardLink(parent)
{
}

template <class TParentObject, class TLinkedObject>
TLinkList<TParentObject, TLinkedObject>::
TLinkList(TParentObject &parent, TBaseReverseLink &rlnk) : TBaseForwardLink(parent)
{
 createLink(rlnk);
}

template <class TParentObject, class TLinkedObject>
TLinkList<TParentObject, TLinkedObject>::~TLinkList()
{
 TListImpl::iterator  first = _list.begin();
 TListImpl::iterator  last  = _list.end();

 while(first != last)
      (**first++).unlink(*this);
}

template <class TParentObject, class TLinkedObject>
TLinkList<TParentObject, TLinkedObject>::
TLinkList(const TForwardLink &lnk) : TBaseForwardLink(lnk)
{
 // ��������� ������
 TListImpl::const_iterator  first = lnk._list.begin();
 TListImpl::const_iterator  last  = lnk._list.end();

 while(first != last)
      createLink(**first++);
}


template <class TParentObject, class TLinkedObject>
TLinkList<TParentObject, TLinkedObject>::
TLinkList(const TForwardLink &lnk, TParentObject &parent) : TBaseForwardLink(parent)
{
 // ��������� ������
 TListImpl::const_iterator  first = lnk._list.begin();
 TListImpl::const_iterator  last  = lnk._list.end();

 while(first != last)
      createLink(**first++);
}

template <class TParentObject, class TLinkedObject>
const TLinkList<TParentObject, TLinkedObject>& TLinkList<TParentObject, TLinkedObject>::
operator = (const TForwardLink &lnk)
{
 // ������������ ��� �������� ������
 TBaseForwardLink::operator= (lnk);

 // ������� ������
 removeLink();

 // ��������� ������
 TListImpl::const_iterator  first = lnk._list.begin();
 TListImpl::const_iterator  last  = lnk._list.end();

 while(first != last)
      createLink(*(*first++));

 return *this;
}

template <class TParentObject, class TLinkedObject>
bool TLinkList<TParentObject, TLinkedObject>::
createLink(TBaseReverseLink &rlnk)
{
 // ��������, ��� ����� ������ ��� ���...
 TListImpl::iterator  found = _list.find(&rlnk);

 if(found != _list.end())
   return false;

 // ���� ����� �� �����������...
 if(!rlnk.link(*this))
   return false;

 // ���� �� ���� ������, ������ ��� Ok!
 _list.push_back(&rlnk);

 return true;
}


template <class TParentObject, class TLinkedObject>
bool TLinkList<TParentObject, TLinkedObject>::removeLink()
{
 TListImpl::iterator  first = _list.begin();
 TListImpl::iterator  last  = _list.end();

 while(first != last)
     {
      TListImpl::iterator  next = first;

      ++next;

      if((**first).unlink(*this))
        _list.erase(first);

      first = next;
     }

 return _list.empty() ? true : false;
}

template <class TParentObject, class TLinkedObject>
bool TLinkList<TParentObject, TLinkedObject>::removeLink(TBaseReverseLink &rlnk)
{
 // ��������, ��� ����� ������ ����...
 TListImpl::iterator  found = _list.find(&rlnk);

 if(found == _list.end())
   return false;

 // ����������� �� �� ���
 if((**found).unlink(*this))
  {
   // �������...
   _list.erase(found);
   return true;
  }

 return false;
}

template <class TParentObject, class TLinkedObject>
bool TLinkList<TParentObject, TLinkedObject>::removeLink(const TLinkedObject &object)
{
 // ��������, ��� ����� ������ ����...
 TListImpl::iterator  found = _list.begin();
 TListImpl::iterator  last  = _list.end();


 while(found != last)
      if((**found++).getParent() == &object)
        break;

 if(found == last)
   return false;

 // ����������� �� �� ���
 if((**found).unlink(*this))
  {
   // �������...
   _list.erase(found);

   return true;
  }

 return false;
}

template <class TParentObject, class TLinkedObject>
bool TLinkList<TParentObject, TLinkedObject>::link(TBaseReverseLink &rlnk)
{
 // ��������, ��� ����� ������ ��� ���...
 TListImpl::iterator  found = _list.find(&rlnk);


 if(found != _list.end())
   return false;

 // ���� �� ���� ������, ������ ��� Ok!
 _list.push_back(&rlnk);

 return true;
}

template <class TParentObject, class TLinkedObject>
bool TLinkList<TParentObject, TLinkedObject>::unlink(const TBaseReverseLink &rlnk)
{
 // ��������, ��� ����� ������ ����...
 TListImpl::iterator  found = _list.find((TBaseReverseLink *)&rlnk);


 if(found == _list.end())
   return false;

 // �������...
 _list.erase(found);

 return true;
}

#endif // __OBJLINK_H

/* <-- EOF --> */