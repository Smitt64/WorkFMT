/*************************************************************************
   File Name      : rslist.h
   Programmer     : Alexey Lebedev

   Creation       : 19 Feb 2002
   Description    : TEventReceiver, TEventLog, TEventLogEntry,
                    TReceiverCash,  TReceiverList,
                    TBtrvReceiver,  TBtrvLog,  TBtrvLogEntry,
                    TProgLogEntry, TToolLogEntry

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __RSLIST_H
#define __RSLIST_H

#include <stddef.h>
#include "rsassert.h"


#ifndef RSTPNAME
    #ifdef _MSC_VER
        #define  RSTPNAME  typename
    #else
        #define  RSTPNAME
    #endif
#endif

// Defined if nested templates is not supported by the compiler
#ifndef RSL_NO_NESTING_TEMPLATES
    #if defined(_MSC_VER)
        #define RSL_NO_NESTING_TEMPLATES
    #endif
#endif

// -----------------------------------------------------------------------------
template <class T>
class RSListT
    {
       typedef RSListT<T>   list;

     public:
       //
       // Types.
       //
       typedef T            value_type;
       typedef T           *pointer;
       typedef const T     *const_pointer;
       typedef T           &reference;
       typedef const T     &const_reference;
       typedef size_t       size_type;

       struct list_item;
       typedef list_item   *list_node;

       struct list_item
            {
             value_type  data;
             list_node   next;

             list_item(const value_type &value, list_node node) : data(value), next(node) {}

             reference         operator * ()         { return data; }
             const_reference   operator * () const   { return data; }
             operator          reference  ()         { return data; }
             operator    const_reference  () const   { return data; }

             void *operator new   (size_t sz) { return ::operator new(sz); }
             void  operator delete(void* p)   { ::delete(p); }
            };

     public:
       //
       // Iterators.
       //
       class iterator
           {
              friend class RSListT<T>;

            protected:
              list_node   node;
              iterator(list_node node) : node(node) {}

            public:
              iterator() : node(NULL) {}
              bool operator == (const iterator &x) const { return node == x.node; }
              bool operator != (const iterator &x) const { return node != x.node; }
              value_type &operator * () const            { return *node; }
              iterator   &operator ++ ()                 { node = node->next; return *this; }
              iterator    operator ++ (int)              { iterator tmp = *this; ++*this; return tmp; }
           };

       class const_iterator
           {
              friend class RSListT<T>;

            protected:
              list_node   node;
              const_iterator(const list_node node) : node(node) {}

            public:
              const_iterator() : node(NULL) {}
              bool operator == (const const_iterator &x) const { return node == x.node; }
              bool operator != (const const_iterator &x) const { return node != x.node; }
              const value_type &operator * () const            { return node->data; }
              const_iterator   &operator ++ ()                 { node = node->next; return *this; }
              const_iterator    operator ++ (int)              { const_iterator tmp = *this; ++*this; return tmp; }
           };

     private:
       //
       // members
       //
       list_node   first;
       list_node   last;
       size_type   length;

     public:
       //
       // construct/copy/destroy
       //
       RSListT()
       {
        first = last = NULL;
        length = 0;
       }

      ~RSListT()
       {
        while(length--)
            {
             list_node  node = first;

             if(length)
               first = first->next;

             delete node;
            }
       }

       RSListT(const RSListT<T> &x)
       {
        if(&x != this)
         {
          first = last = NULL;
          length = 0;

          const_iterator  first = x.begin();
          const_iterator  last  = x.end();

          while(first != last)
              {
               push_back(*first);

               ++first;
              }
         }
       }

       const RSListT<T> &operator = (const RSListT<T> &x)
       {
        if(&x != this)
         {
          erase(begin(), end());

          const_iterator  first = x.begin();
          const_iterator  last  = x.end();

          while(first != last)
              {
               push_back(*first);

               ++first;
              }
         }

         return *this;
       }

       //
       // Iterators.
       //
       iterator begin()              { return first; }
       const_iterator begin()  const { return first; }
       iterator end()                { return NULL; }
       const_iterator end()    const { return NULL; }
       //
       // Capacity.
       //
       bool empty()      const { return length ? false : true; }
       size_type size()  const { return length; }
       //
       // Element access.
       //
       value_type &front() { return *first; }
       value_type &back () { return *last; }

       const value_type &front() const { return *first; }
       const value_type &back () const { return *last; }
       //
       // Index access.
       //
       value_type &operator [] (size_t position)
       {
        RSASSERT(position < size());

        iterator  first = begin();

        while(position--)
             ++first;

        return *first;
       }

       const value_type &operator [] (size_t position) const
       {
        RSASSERT(position < size());

        const_iterator  first = begin();

        while(position--)
             ++first;

        return *first;
       }

       //
       // Modifiers.
       //
       //
       // Insert x at position.
       //
       iterator insert(iterator position, const_reference value);

       void insert(const_reference value)
       {
        push_front(value);
       }

       //
       // Erase
       //
       void erase(iterator position);

       void erase(size_t position)
       {
        RSASSERT(position < size());

        iterator  first = begin();

        while(position--)
             ++first;

        erase(first);
       }

       //
       // Pop/push element
       //
       void push_front(const_reference value);
       void push_back (const_reference value);
       void pop_front ();
       void pop_back  ();

       void erase(iterator first, iterator last)
       {
        while(first != last)
            {
             iterator  next = first;

             ++next;

             erase(first);

             first = next;
            }
       }

       const_iterator find(const_reference value) const
       {
        const_iterator  first = begin();
        const_iterator  last  = end();

        while(first != last && !(*first == value))
             ++first;

        return first;
       }

       iterator find(const_reference value)
       {
        iterator  first = begin();
        iterator  last  = end();

        while(first != last && !(*first == value))
             ++first;

        return first;
       }

    #ifdef RSL_NO_NESTING_TEMPLATES
       template<class Predicate> iterator find_if(Predicate pred)
       {
        iterator  first = begin();
        iterator  last  = end();

        while(first != last && !pred(*first))
             ++first;

        return first;
       }
    #endif

       void remove(const_reference value)
       {
        iterator  first = begin();
        iterator  last  = end();

        while(first != last)
        {
         iterator  next = first;

         ++next;

         if(*first == value)
           erase(first);

         first = next;
        }
       }

    #ifdef RSL_NO_NESTING_TEMPLATES
       template<class Predicate> void remove_if(Predicate pred)
       {
        iterator  first = begin();
        iterator  last  = end();

        while(first != last)
            {
             iterator  next = first;

             ++next;

             if(pred(*first))
               erase(first);

             first = next;
            }
       }
    #endif

       //
       // new/delete operators
       //
       void *operator new   (size_t sz)             { return ::operator new(sz); }
       void *operator new   (size_t sz, void *buf)  { return buf; }
       void  operator delete(void *p)               { ::delete(p); }
       //
       // Fix for warning - ''declaration' : no matching operator delete found;
       // memory will not be freed if initialization throws an exception'
       //
    #ifdef _MSC_VER
       void  operator delete(void *p, void *buf) {}
    #endif

     private:
       //
       // Find previos node
       //
       list_node prev_node(list_node node)
       {
        list_node  prev = first;

        while(prev->next != node)
             prev = prev->next;

        return prev;
       }
    };

// -----------------------------------------------------------------------------
template <class T>
RSTPNAME RSListT<T>::iterator RSListT<T>::insert(RSTPNAME RSListT<T>::iterator position, RSTPNAME RSListT<T>::const_reference value)
{
 list_node  node = position.node;

 if(node == first)
  {
   push_front(value);
   
   return first;
  }

 if(node == NULL)
  {
   push_back(value);
   
   return last;
  }

 length++;

 list_node  prev = prev_node(node);

 prev->next = new list_item(value, node);

 return prev->next;
}

// -----------------------------------------------------------------------------
template <class T>
void RSListT<T>::erase(iterator position)
{
 list_node  node = position.node;

 if(node == first)
  {
   pop_front();

   return;
  }

 if(node == last)
  {
   pop_back();
   
   return;
  }

 if(length)
  {
   length--;

   list_node  prev = prev_node(node);

   prev->next = node->next;

   delete node;
  }
}

// -----------------------------------------------------------------------------
template <class T>
void RSListT<T>::push_front(const_reference value)
{
 if(length)
  {
   length++;
   first = new list_item(value, first);
  }
 else
  {
   length = 1;
   first = last = new list_item(value, NULL);
  }
}

// -----------------------------------------------------------------------------
template <class T>
void RSListT<T>::push_back(const_reference value)
{
 if(length)
  {
   length++;
   last = last->next = new list_item(value, NULL);
  }
 else
  {
   length = 1;
   first = last = new list_item(value, NULL);
  }
}

// -----------------------------------------------------------------------------
template <class T>
void RSListT<T>::pop_front()
{
 if(length)
  {
   length--;

   list_node  node = first;

   if(length)
      first = first->next;
   else
      first = last = NULL;

   delete node;
  }
}

// -----------------------------------------------------------------------------
template <class T>
void RSListT<T>::pop_back()
{
 if(length)
  {
   length--;

   list_node  node = last;

   if(length)
    {
     last = prev_node(last);
     last->next = NULL;
    }
   else
     first = last = NULL;

   delete node;
  }
}

#endif //__RSLIST_H

/* <-- EOF --> */