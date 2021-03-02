//===================================================================
//             ��⮬�⨧�஢����� ������᪠� ��⥬� RS-Bank
//                 Copyright (C) R-Style SoftLab 2003
//
//  ��� 䠩��        : AllTempl.hpp
//
//  ������⥪�       :
//
//  ���ᠭ��         : ��直� ������� 蠡����
//
//  �ணࠬ����      : �૮� �.�.
//
//  ������           : 30.05.2003
//
//===================================================================
#ifndef __ALLTEMPL_HPP__8C0C9AAA_A9BD_4A04_BD56_2FECD2908DC3
#define __ALLTEMPL_HPP__8C0C9AAA_A9BD_4A04_BD56_2FECD2908DC3

#include "alexcept.hpp"
#include "talloc.h"

#ifdef __BORLANDC__
  #pragma warn -inl
#endif

#pragma warning(push)
#pragma warning(disable: 4786) // 'identifier' : identifier was truncated to 'number' characters in the debug information


// abstract iteration position
//##ModelId=416FC2680311
struct __LPOSITION { };
//##ModelId=416FC268034B
typedef __LPOSITION* LPOSITION;


//////////////////////////////////////////////////////////////////////////
//                ��ࠬ��ਧ������ ����� ᯨ᪠                        //
//////////////////////////////////////////////////////////////////////////

//��ࠬ��ਧ������ ����� ᯨ᪠
//##ModelId=416FC269004E
template<class DataType, class ArgType = DataType&>
class TRsbList
{
   //��ࠬ��ਧ������ ����� ��쥪� ᯨ᪠
   //##ModelId=416FC26A00DA
   class CListObject
   {
   public:
        //##ModelId=416FC26A00EA
      CListObject(): m_pNext(NULL), m_pPrior(NULL){}
        //##ModelId=416FC26A00FA
      CListObject(ArgType info): m_pNext(NULL), m_pPrior(NULL), m_Info(info){}
      //���ଠ��
      //##ModelId=416FC26A011A
      DataType m_Info;
      //�����⥫� �� ᫥���騩 ��쥪� � ᯨ᪥
      //##ModelId=416FC26A012B
      CListObject* m_pNext;
      //�����⥫� �� �।��騩 ��쥪� � ᯨ᪥
      //##ModelId=416FC26A015A
      CListObject* m_pPrior;
   };
   
    //##ModelId=416FC26A0177
   typedef CListObject* TListElemPtr;
    //##ModelId=416FC26A01A5
   typedef CListObject  TListElement;
public:
   //�������� ���� ����� � ��砫� ᯨ᪠
   //##ModelId=416FC269008C
   LPOSITION AddHead(ArgType NewObj);

   //������ ���� ����� ᯨ᪠
   //##ModelId=416FC269009D
   void RemoveHead();

   //������ ��᫥����  ����� ᯨ᪠
   //##ModelId=416FC26900AB
   void RemoveTail();

   //������ �� ������ ᯨ᪠
   //##ModelId=416FC26900BB
   void RemoveAll();

   //�����頥� ������⢮ ����⮢ � ᯨ᪥
   //##ModelId=416FC26900CB
   unsigned long GetAmount();

   //�����頥� ��砫� ᯨ᪠
   //##ModelId=416FC26900CC
   DataType& GetHead();

   //�����頥� ����� ᯨ᪠
   //##ModelId=416FC26900DA
   DataType& GetTail();

   //��������� �� 㬮�砭�� ��� ᯨ��
   //##ModelId=416FC26900DB
   TRsbList();

   //�������� ��� ᯨ᪠
   //##ModelId=416FC2690109
   ~TRsbList();

   //�������� ���� ����� � ����� ᯨ᪠
   //##ModelId=416FC269010A
   LPOSITION AddTail(ArgType NewObj);

   //������ �� ᯨ᪠ �����, 㪠��⥫� �� ����� ���
   //������ ��� ��ࠬ���.
   //##ModelId=416FC2690128
   void RemoveAt(LPOSITION position);

   //�����頥� 㪠��⥫� �� �������� ᮢ�������, ���� NULL
   //##ModelId=416FC269012A
   LPOSITION Find(ArgType Obj, LPOSITION startAfter = NULL);

    //##ModelId=416FC2690148
   LPOSITION FindIndex(int nIndex);

    //##ModelId=416FC2690158
   int GetIndexByPos(const LPOSITION rPosition);

    //##ModelId=416FC2690167
   LPOSITION GetHeadPosition();

    //##ModelId=416FC2690168
   LPOSITION GetTailPosition();

    //##ModelId=416FC2690196
   DataType& GetNext(LPOSITION& rPosition);

    //##ModelId=416FC26901A5
   DataType& GetPrev(LPOSITION& rPosition);

    //##ModelId=416FC26901B6
   DataType& GetAt(LPOSITION rPosition);

    //##ModelId=416FC26901C6
   void SetAt(LPOSITION pos, ArgType newElement);

    //##ModelId=416FC26901E4
   LPOSITION InsertBefore(LPOSITION position, ArgType newElement);

    //##ModelId=416FC26901F5
   LPOSITION InsertAfter(LPOSITION position, ArgType newElement);

   void Swap(LPOSITION leftPosition, LPOSITION rightPosition);

private:
   //������⢮ ����⮢ � ᯨ᪥
   //##ModelId=416FC2690251
   unsigned long m_Amount;

   //�����⥫� �� ��砫� ᯨ᪠
   //##ModelId=416FC26A008D
   TListElemPtr m_pStart;

   //�����⥫� �� ����� ᯨ᪠
   //##ModelId=416FC26A00BB
   TListElemPtr m_pEnd;

};

// ��������� ������ ������ ������ ��� ���������� ������������.
#define RSB_LIST_SHOW_READONLY(_BaseListImpl_)  \
   using _BaseListImpl_::GetAmount;             \
   using _BaseListImpl_::GetHead;               \
   using _BaseListImpl_::GetTail;               \
   using _BaseListImpl_::Find;                  \
   using _BaseListImpl_::FindIndex;             \
   using _BaseListImpl_::GetIndexByPos;         \
   using _BaseListImpl_::GetHeadPosition;       \
   using _BaseListImpl_::GetTailPosition;       \
   using _BaseListImpl_::GetNext;               \
   using _BaseListImpl_::GetPrev;               \
   using _BaseListImpl_::GetAt


//////////////////////////////////////////////////////////////////////////
//                       ���⮥ ����୮� ��ॢ�                        //
//////////////////////////////////////////////////////////////////////////
//##ModelId=416FC26A034B
template<class DataType, class ArgType = DataType&>
class TRsbBinTree
{
private:
   // ���� ��ॢ�
    //##ModelId=416FC26B01F4
   class CTreeNode
   {
   public:
        //##ModelId=416FC26B0203
      CTreeNode(): m_pLeft(NULL), m_pRight(NULL), m_pParent(NULL){}
        //##ModelId=416FC26B0204
      CTreeNode(ArgType info): m_pLeft(NULL), m_pRight(NULL), m_pParent(NULL), m_Info(info){}
        //##ModelId=416FC26B0223
      ~CTreeNode(){m_pLeft = m_pRight = m_pParent = NULL;}

      // ����� 㧫�
        //##ModelId=416FC26B0233
      DataType m_Info;
      // �����⥫� �� ���� 㧥�
        //##ModelId=416FC26B0254
      CTreeNode* m_pLeft;
      // �����⥫� �� �ࠢ� 㧥�
        //##ModelId=416FC26B0280
      CTreeNode* m_pRight;
      //�����⥫� �� �।��騩 ��쥪� � ᯨ᪥
        //##ModelId=416FC26B02B2
      CTreeNode* m_pParent;
   };

    //##ModelId=416FC26B02DE
   typedef CTreeNode* CTreeNodePtr;

    //##ModelId=416FC26A035E
   CTreeNodePtr m_pRootNode;

public:

    //##ModelId=416FC26A037A
   TRsbBinTree();
    //##ModelId=416FC26A038A
   ~TRsbBinTree();

   // �����頥� ����� ���⥢��� 㧫�.
   // � pos �����뢠���� ������ ����
    //##ModelId=416FC26A038B
   DataType& GetRoot(LPOSITION& pos);

   // �����頥� ������ ����
    //##ModelId=416FC26A0399
   LPOSITION GetRootPos() const;

   // �����頥� ����� ������ 㧫�,
   // �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
   // ��᫥ �믮������ � pos - ������ 㧫� ����� ���ண� ���㫨.
    //##ModelId=416FC26A03AA
   DataType& GetLeft(LPOSITION& pos);
   
   // �����頥� ������ ������ 㧫�, 
   // �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
    //##ModelId=416FC26A03B9
   LPOSITION GetLeftPos(const LPOSITION pos) const;

   // �����頥� ����� �ࠢ��� 㧫�,
   // �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
   // ��᫥ �믮������ pos - ������ 㧫� ����� ���ண� ���㫨.
    //##ModelId=416FC26A03D8
   DataType& GetRight(LPOSITION& pos);

   // �����頥� ������ �ࠢ��� 㧫�, 
   // �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
    //##ModelId=416FC26B0000
   LPOSITION GetRightPos(const LPOSITION pos) const;
   
   // �����頥� ����� த�⥫�᪮�� 㧫�,
   // �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
   // ��᫥ �믮������ � pos - ������ 㧫� ����� ���ண� ���㫨.
    //##ModelId=416FC26B0010
   DataType& GetParent(LPOSITION& pos);

   // �����頥� ������ த�⥫�᪮�� 㧫�, 
   // �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
    //##ModelId=416FC26B0020
   LPOSITION GetParentPos(const LPOSITION pos) const;

   // �����頥� ����� 㧫�,
   // ������ ���ண� ��।��� � pos.
    //##ModelId=416FC26B004E
   DataType& GetAt(const LPOSITION pos);

   // �����頥� ������ ����筮�� ������ 㧫�
   // ��� ��ॢ� � ��୥� pos.
    //##ModelId=416FC26B005D
   LPOSITION GetEndLeftPos(const LPOSITION RootPos = NULL);

   // �����頥� ������ ����筮�� �ࠢ��� 㧫�
   // ��� ��ॢ� � ��୥� pos.
    //##ModelId=416FC26B006D
   LPOSITION GetEndRightPos(const LPOSITION RootPos = NULL);

   // ��⠢��� ���� 㧥�, �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
   // newData - ����� ��⠢�塞��� 㧫�.
   // ExistToLeft - �������騩 㧥� ᤥ���� ���� 㧫�� ��⠢�塞���.
   // �����頥� ������ ����� ��⠢������� 㧫�.
    //##ModelId=416FC26B007E
   LPOSITION InsertLeft(const LPOSITION pos, ArgType newData, bool ExistToLeft = true);

   // ��⠢��� �ࠢ� 㧥� �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
   // newData - ����� ��⠢�塞��� 㧫�.
   // ExistToLeft - �������騩 㧥� ᤥ���� ���� 㧫�� ��⠢�塞���.
   // �����頥� ������ ����� ��⠢������� 㧫�.
    //##ModelId=416FC26B009C
   LPOSITION InsertRight(const LPOSITION pos, ArgType newData, bool ExistToLeft = true);

   // ������ ����� �����ॢ�
   // �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
    //##ModelId=416FC26B00BC
   void RemoveLeft(const LPOSITION pos);

   // ������ �ࠢ�� �����ॢ�
   // �⭮ᨫ��쭮 㧫�, ������ ���ண� ��।��� � pos.
    //##ModelId=416FC26B00CB
   void RemoveRight(const LPOSITION pos);

   // ������ �����ॢ� ������ ���ண� ��।��� � pos.
    //##ModelId=416FC26B00DB
   void RemoveAt(const LPOSITION pos);

   // ������ �� ������ ��ॢ�.
    //##ModelId=416FC26B00EB
   void RemoveAll();

   // ��६�頥� �����ॢ� � ��୥� � ����樨 FromPos,
   // � 㧥� � ����樥� ToPos.
   // ��᫥ �믮������ ������ FromPos �⠭�� ����������.
    //##ModelId=416FC26B00FA
   void Move(LPOSITION ToPos, LPOSITION& FromPos);

   // ��६�頥� �����ॢ� � ��୥� � ����樨 FromPos,
   // � ����� ���� 㧫� � ����樥� ToPos.
   // �᫨ ����� ��⪠ 㧫� ToPos 㦥 �������, �
   // ��᫥ �믮������ ������ FromPos �⠭�� ����������.
    //##ModelId=416FC26B0119
   void MoveToLeft(LPOSITION ToPos, LPOSITION& FromPos);
   
   // ��६�頥� �����ॢ� � ��୥� � ����樨 FromPos,
   // � �ࠢ�� ���� 㧫� � ����樥� ToPos.
   // �᫨ �ࠢ�� ��⪠ 㧫� ToPos 㦥 �������, �
   // ��᫥ �믮������ ������ FromPos �⠭�� ����������.
    //##ModelId=416FC26B0138
   void MoveToRight(LPOSITION ToPos, LPOSITION& FromPos);

   // �����頥� ������ ��ࢮ�� ��������� 㧫�, ���� NULL.
    //##ModelId=416FC26B0148
   LPOSITION Find(ArgType Data, const LPOSITION startAfter = NULL);

   // �����頥� ������ ᫥���饣� ��᫥ ��������� 㧫�, ���� NULL.
    //##ModelId=416FC26B0167
   LPOSITION FindNext(ArgType Data, const LPOSITION PrevFindedPos = NULL, bool FindInThisBranch = true);

   // �����頥� true, �᫨ 㧥� � ����樥� pos ���� �ࠢ� �� �⭮襭�� � �।��
    //##ModelId=416FC26B0179
   bool IsRightNodePos(const LPOSITION pos);

   // �����頥� true, �᫨ 㧥� � ����樥� pos ���� ���� �� �⭮襭�� � �।��
    //##ModelId=416FC26B0196
   bool IsLeftNodePos(const LPOSITION pos);

   // �����頥� true, �᫨ 㧥� � ����樥� pos ���� ��୥��
    //##ModelId=416FC26B01A5
   bool IsRootNodePos(const LPOSITION pos);

   // ���⮥ �� ��ॢ�
    //##ModelId=416FC26B01B5
   bool IsEmpty();

   // ��।���� ���� �� 㧥� � �����樥� ChildPos 
   // ���୨� �� �⭮襭�� � ParentPos.
    //##ModelId=416FC26B01C5
   bool IsChild(LPOSITION ParentPos, LPOSITION ChildPos);

};


//////////////////////////////////////////////////////////////////////////
//                       ������஢����� ��ॢ�                          //
//////////////////////////////////////////////////////////////////////////
//template<class DataType, class ArgType = DataType&>
//class TRsbTree: protected TRsbBinTree<DataType, ArgType>
//{
//};
//
//

//////////////////////////////////////////////////////////////////////////
//               ������ᠫ�� "㬭�" 㪠��⥫�                        //
//////////////////////////////////////////////////////////////////////////

// �����⥫� ��� ��ꥪ� ����祭���� �����஬ new
//##ModelId=416FC26B038A
template<class TypePtr>
class TRsbAutoPtr
{
public:

    //##ModelId=416FC26B039A
    TRsbAutoPtr(TypePtr* p = NULL) throw();

    //##ModelId=416FC26C000F
    TRsbAutoPtr(TRsbAutoPtr<TypePtr>& a) throw();

    //##ModelId=416FC26C0011
    void operator =(TRsbAutoPtr<TypePtr>& rhs);

    //##ModelId=416FC26C0020
    virtual ~TRsbAutoPtr();

    //##ModelId=416FC26C003E
    TRsbAutoPtr<TypePtr>& operator =(TypePtr* rhs);

    //##ModelId=416FC26C004E
    TypePtr& operator *()const throw();

    //##ModelId=416FC26C0050
    TypePtr* operator ->()const throw();

    //##ModelId=416FC26C005D
    operator TypePtr*()const throw();


    //##ModelId=416FC26C006D
    TypePtr* get()const throw();

    //##ModelId=416FC26C006F
    TypePtr* release() throw();

protected:

    //##ModelId=416FC26C007D
    TypePtr* reset(TypePtr* p = NULL) throw();
    //##ModelId=416FC26C008C
    TypePtr* the_p;
};

//////////////////////////////////////////////////////////////////////////

// �����⥫� ��� ����� ����祭���� �����஬ new (ᯥ樠������ ��� void)
template<> class TRsbAutoPtr<void>
{
public:

    explicit TRsbAutoPtr(void* p = NULL) throw() : the_p(p){}

    TRsbAutoPtr(TRsbAutoPtr<void>& a) throw() : the_p(a.release()){}

    void operator =(TRsbAutoPtr<void>& rhs)
    {
       reset(rhs.release());
    }

    virtual ~TRsbAutoPtr()
    {
       if(the_p)
         delete the_p;
    }

    TRsbAutoPtr<void>& operator =(void*& rhs)
    {
       the_p = rhs;
       return *this;
    }

    operator void*()const throw()
    {
       return the_p;
    }


    void* get()const throw()
    {
       return the_p;
    }

    void* release() throw()
    {
       return reset();
    }

protected:

    void* reset(void* p = NULL) throw()
    {
       void* tmp = the_p;
       the_p = p;
       return tmp;
    }

    void* the_p;
};

//////////////////////////////////////////////////////////////////////////

// �����⥫� ��� ��ꥪ� ����祭���� �����஬ calloc � �.�.
//##ModelId=416FC26C00CB
template<class TypePtr>
class TRsbFreePtr : public TRsbAutoPtr<TypePtr>
{
public:
    //##ModelId=416FC26D0205
    TRsbFreePtr(TypePtr* p = NULL) throw() : TRsbAutoPtr<TypePtr>(p){}
    //##ModelId=416FC26D0214
    TRsbFreePtr(void* p) throw() : TRsbAutoPtr<TypePtr>((TypePtr*)p){}

    //##ModelId=416FC26D0223
    TRsbFreePtr(TRsbAutoPtr<TypePtr>& a) throw() : TRsbAutoPtr<TypePtr>(a){}

    //##ModelId=416FC26D0233
    ~TRsbFreePtr()
    {
       FreeMem((void**)&the_p);
       release();
    }
};

//////////////////////////////////////////////////////////////////////////
// ���࠭�� ���祭�� ������쭮� ��६�����
//##ModelId=416FC26D0261
template <class Type>
class TRsbValSaver
{
    //##ModelId=416FC26D0272
  Type m_SavedVal;
    //##ModelId=416FC26D0290
  Type* m_ptrVal;

public:
    //##ModelId=416FC26D029F
  TRsbValSaver(Type& Value) : m_SavedVal(Value), m_ptrVal(&Value)
  {
  }

    //##ModelId=416FC26D02AF
  ~TRsbValSaver()
  {
     *m_ptrVal = m_SavedVal;
  }
};

//////////////////////////////////////////////////////////////////////////
// �࠭����� ��� ��⮤� �����
//##ModelId=416FC26D02FD
template<class ClsType>
class TRsbObjectTrn
{
    //##ModelId=416FC26E001F
   typedef int (ClsType::*TrnHndlType)(void*);

   // �����⥫� �� ��ꥪ� ��⮤ ���ண� �㤥� ��뢠����
    //##ModelId=416FC26D032C
   ClsType* m_pObj;

   // �����⥫쭠 ��⮤ ����� �㤥� ��뢠����
    //##ModelId=416FC26D034C
   TrnHndlType m_pTrnHndl;

    //##ModelId=416FC26D035B
   char* m_sWarn;

    //##ModelId=416FC26D037A
   void* m_pParm;

    //##ModelId=416FC26D038A
   static int TrnProcSt(void* pParm);

public:
    //##ModelId=416FC26D03A9
   TRsbObjectTrn(ClsType* pObj, TrnHndlType pHndl, char* sWarn)
      :m_pObj(pObj),
       m_pTrnHndl(pHndl),
       m_sWarn(sWarn),
       m_pParm(NULL)
   {
   }

    //##ModelId=416FC26D03BA
   TRsbObjectTrn(ClsType& pObj, TrnHndlType pHndl, char* sWarn)
      :m_pObj(&pObj),
       m_pTrnHndl(pHndl),
       m_sWarn(sWarn),
       m_pParm(NULL)
   {
   }

   // ����� �࠭���樨
    //##ModelId=416FC26D03D9
   int Run(void* pParm = NULL)
   {
      m_pParm = pParm;
      return TrueTrn(TrnProcSt, this, m_sWarn);
   }

    //##ModelId=416FC26E0001
   friend static int TRsbObjectTrn<ClsType>::TrnProcSt(void* pParm);

};

//##ModelId=416FC26D038A
template<class ClsType>
int TRsbObjectTrn<ClsType>::TrnProcSt(void* pParm)
{
      TRsbObjectTrn<ClsType>* pThis = (TRsbObjectTrn<ClsType>*)pParm;
      return (pThis->m_pObj->*(pThis->m_pTrnHndl))(pThis->m_pParm);
}


// ����� ��� �맮�� ��⮤� ����� � �࠭���樨
#define RSB_RUN_OBJTRN(Class, ClsObj, ClsMetod, pTrnParm, sTrnWarn) \
   TRsbObjectTrn<Class>(ClsObj, &Class::ClsMetod, sTrnWarn).Run(pTrnParm)

//////////////////////////////////////////////////////////////////////////

// Inlines
#include "AllTempl.inl"

#pragma warning(pop)

#endif // __ALLTEMPL_HPP__8C0C9AAA_A9BD_4A04_BD56_2FECD2908DC3
