//===================================================================
//             Автоматизированная банковская система RS-Bank
//                 Copyright (C) R-Style SoftLab 2003
//
//  Имя файла        : AllTempl.inl
//
//  Библиотека       :
//
//  Описание         : Всякие полезные шаблоны (inlines)
//
//  Программист      : Орлов В.Н.
//
//  Создан           : 08.07.2007
//
//===================================================================

#include "alapsto.h"


//////////////////////////////////////////////////////////////////////////
//                Параметризованный класс списка                        //
//////////////////////////////////////////////////////////////////////////

//##ModelId=416FC269008C
template<class DataType, class ArgType>
LPOSITION TRsbList<DataType, ArgType>::AddHead(ArgType NewObj)
{
   TListElemPtr p;
   p = new TListElement;
   p->m_Info = NewObj;
   if(m_pStart == NULL)// первый элемент
      m_pStart = m_pEnd = p;
   else // добавляем в начало
   {
      p->m_pNext = m_pStart;
      m_pStart->m_pPrior = p;
      m_pStart = p;
   }
   m_Amount++;

   return (LPOSITION)m_pStart;
}

//##ModelId=416FC269009D
template<class DataType, class ArgType>
void TRsbList<DataType, ArgType>::RemoveHead()
{
   if(m_Amount != 0)
      RemoveAt(GetHeadPosition());
}

//##ModelId=416FC26900AB
template<class DataType, class ArgType>
void TRsbList<DataType, ArgType>::RemoveTail()
{
   if(m_Amount != 0)
      RemoveAt(GetTailPosition());
}

//##ModelId=416FC26900BB
template<class DataType, class ArgType>
void TRsbList<DataType, ArgType>::RemoveAll()
{
   TListElemPtr p, p1;
   // Освобождаем все элементы списка
   p = m_pStart;
   while(p)
   {
      p1 = p->m_pNext;
      delete p;
      p = p1;
   }
   m_pStart = m_pEnd = NULL;
   m_Amount = 0;
}

//##ModelId=416FC26900CB
template<class DataType, class ArgType>
unsigned long TRsbList<DataType, ArgType>::GetAmount()
{
   return m_Amount;
}

//##ModelId=416FC26900CC
template<class DataType, class ArgType>
DataType& TRsbList<DataType, ArgType>::GetHead()
{
   return m_pStart->m_Info;
}

//##ModelId=416FC26900DA
template<class DataType, class ArgType>
DataType& TRsbList<DataType, ArgType>::GetTail()
{
   return m_pEnd->m_Info;
}

//##ModelId=416FC26900DB
template<class DataType, class ArgType>
TRsbList<DataType, ArgType>::TRsbList()
{
   m_pStart = m_pEnd = NULL;
   m_Amount = 0;
}

//##ModelId=416FC2690109
template<class DataType, class ArgType>
TRsbList<DataType, ArgType>::~TRsbList()
{
   RemoveAll();
}

//##ModelId=416FC269010A
template<class DataType, class ArgType>
LPOSITION TRsbList<DataType, ArgType>::AddTail(ArgType NewObj)
{
   TListElemPtr p;
   p = new TListElement;
   p->m_Info = NewObj;
   if(m_pStart == NULL)// первый элемент
      m_pStart = m_pEnd = p;
   else // добавляем в конец
   {
      p->m_pPrior = m_pEnd;
      m_pEnd->m_pNext = p;
      m_pEnd = p;
   }
   m_Amount++;
   return (LPOSITION)m_pEnd;
}


//##ModelId=416FC2690128
template<class DataType, class ArgType>
void TRsbList<DataType, ArgType>::RemoveAt(LPOSITION position)
{
   TListElemPtr pObj = (TListElemPtr)position;
   if(pObj == NULL)return;
   if(pObj->m_pPrior)// не первый элемент
   {
      pObj->m_pPrior->m_pNext = pObj->m_pNext;
      if(pObj->m_pNext)// не последний элемент
         pObj->m_pNext->m_pPrior = pObj->m_pPrior;
      else // в противном случае удаляется последний элемент
         m_pEnd=pObj->m_pPrior;// Обновление указателя на конец списка
   }
   else // удаляется первый элемент списка
   {
      if(pObj->m_pNext)// Список не пуст
      {
         pObj->m_pNext->m_pPrior = (NULL);
         m_pStart = pObj->m_pNext;
      }
      else // Теперь список пуст
         m_pStart = m_pEnd = NULL;
   }
   delete pObj;
   m_Amount--;
}


//##ModelId=416FC269012A
template<class DataType, class ArgType>
LPOSITION TRsbList<DataType, ArgType>::Find(ArgType Obj, LPOSITION startAfter)
{
   if(m_Amount < 1)
      return NULL;

   TListElemPtr pListObj;
   pListObj=startAfter == NULL ? m_pStart : (TListElemPtr)startAfter;
   while(pListObj)
   {
      if(Obj == pListObj->m_Info)// Совпадение найдено
         return (LPOSITION)pListObj;
      pListObj = pListObj->m_pNext;
   }
   return NULL; // Совпадений не найдено
}

//##ModelId=416FC2690148
template<class DataType, class ArgType>
LPOSITION TRsbList<DataType, ArgType>::FindIndex(int nIndex)
{
   if (nIndex >= (int)m_Amount || nIndex < 0)
      return NULL;  // went too far

   TListElemPtr pListObj = m_pStart;
   while(nIndex)
   {
      pListObj = pListObj->m_pNext;
      nIndex--;
   }
   return (LPOSITION)pListObj;
}

//##ModelId=416FC2690158
template<class DataType, class ArgType>
int TRsbList<DataType, ArgType>::GetIndexByPos(const LPOSITION rPosition)
{
   int nIndex = -1;
   if(rPosition)
   {
      TListElemPtr pListObj = (TListElemPtr)rPosition;
      nIndex = 0;
      while(pListObj)
      {
         nIndex++;
         pListObj = pListObj->m_pPrior;
      }
   }
   return nIndex;
}

//##ModelId=416FC2690167
template<class DataType, class ArgType>
LPOSITION TRsbList<DataType, ArgType>::GetHeadPosition()
{
   return (LPOSITION)m_pStart;
}

//##ModelId=416FC2690168
template<class DataType, class ArgType>
LPOSITION TRsbList<DataType, ArgType>::GetTailPosition()
{
   return (LPOSITION)m_pEnd;
}

//##ModelId=416FC2690196
template<class DataType, class ArgType>
DataType& TRsbList<DataType, ArgType>::GetNext(LPOSITION& rPosition)
{
   TListElemPtr pListObj = (TListElemPtr)rPosition;

   rPosition = (LPOSITION)pListObj->m_pNext;
   return pListObj->m_Info;
}

//##ModelId=416FC26901A5
template<class DataType, class ArgType>
DataType& TRsbList<DataType, ArgType>::GetPrev(LPOSITION& rPosition)
{
   TListElemPtr pListObj = (TListElemPtr)rPosition;

   rPosition = (LPOSITION)pListObj->m_pPrior;
   return pListObj->m_Info;
}

//##ModelId=416FC26901B6
template<class DataType, class ArgType>
DataType& TRsbList<DataType, ArgType>::GetAt(LPOSITION rPosition)
{
   TListElemPtr pListObj = (TListElemPtr)rPosition;
   return pListObj->m_Info;
}

//##ModelId=416FC26901C6
template<class DataType, class ArgType>
void TRsbList<DataType, ArgType>::SetAt(LPOSITION pos, ArgType newElement)
{
   TListElemPtr pListObj = (TListElemPtr) pos;
   pListObj->m_Info = newElement;
}

//##ModelId=416FC26901E4
template<class DataType, class ArgType>
LPOSITION TRsbList<DataType, ArgType>::InsertBefore(LPOSITION position, ArgType newElement)
{
   if(position == NULL)
      return AddHead(newElement); // insert before nothing -> head of the list

   // Insert it before position
   TListElemPtr pOldNode = (TListElemPtr) position;
   TListElemPtr pNewNode = new TListElement(newElement);
   pNewNode->m_pPrior = pOldNode->m_pPrior;
   pNewNode->m_pNext = pOldNode;

   if (pOldNode->m_pPrior != NULL)
   {
      pOldNode->m_pPrior->m_pNext = pNewNode;
   }
   else
   {
      m_pStart = pNewNode;
   }
   pOldNode->m_pPrior = pNewNode;
   return (LPOSITION) pNewNode;
}

//##ModelId=416FC26901F5
template<class DataType, class ArgType>
LPOSITION TRsbList<DataType, ArgType>::InsertAfter(LPOSITION position, ArgType newElement)
{
   if (position == NULL)
      return AddTail(newElement); // insert after nothing -> tail of the list
      
   // Insert it before position
   TListElemPtr pOldNode = (TListElemPtr)position;
   TListElemPtr pNewNode = new TListElement(newElement);
   pNewNode->m_pPrior = pOldNode;
   pNewNode->m_pNext = pOldNode->m_pNext;

   if (pOldNode->m_pNext != NULL)
   {
      pOldNode->m_pNext->m_pPrior = pNewNode;
   }
   else
   {
      m_pEnd = pNewNode;
   }
   pOldNode->m_pNext = pNewNode;
   return (LPOSITION) pNewNode;
}

template<class DataType, class ArgType>
void TRsbList<DataType, ArgType>::Swap(LPOSITION leftPosition, LPOSITION rightPosition)
{
   if (leftPosition != rightPosition && leftPosition != 0 && rightPosition != 0)
   {
      TListElemPtr pLeftNode  = reinterpret_cast<TListElemPtr>(leftPosition);
      TListElemPtr pRightNode = reinterpret_cast<TListElemPtr>(rightPosition);

      TListElemPtr pLeftNodeNext  = pLeftNode->m_pNext;
      TListElemPtr pLeftNodePrior = pLeftNode->m_pPrior;

      TListElemPtr pRightNodeNext  = pRightNode->m_pNext;
      TListElemPtr pRightNodePrior = pRightNode->m_pPrior;

      if (pLeftNodeNext == pRightNode)
      {
          // lp - l - r - rn
          if (pLeftNodePrior != 0)
          {
              pLeftNodePrior->m_pNext  = pRightNode;
          }
          else
          {
              m_pStart = pRightNode;
          }

          if (pRightNodeNext != 0)
          {
              pRightNodeNext->m_pPrior = pLeftNode;
          }
          else
          {
              m_pEnd = pLeftNode;
          }

          pLeftNode->m_pNext  = pRightNodeNext;
          pLeftNode->m_pPrior = pRightNode;

          pRightNode->m_pNext  = pLeftNode;
          pRightNode->m_pPrior = pLeftNodePrior;
      }
      else if (pLeftNodePrior == pRightNode)
      {
          // rp - r - l - ln
          if (pRightNodePrior != 0)
          {
              pRightNodePrior->m_pNext = pLeftNode;
          }
          else
          {
              m_pStart = pLeftNode;
          }

          if (pLeftNodeNext != 0)
          {
              pLeftNodeNext->m_pPrior = pRightNode;
          }
          else
          {
              m_pEnd = pRightNode;
          }

          pRightNode->m_pNext  = pLeftNodeNext;
          pRightNode->m_pPrior = pLeftNode;

          pLeftNode->m_pNext  = pRightNode;
          pLeftNode->m_pPrior = pRightNodePrior;
      }
      else
      {
          // lp - l - ln - m - rp - r - rn
          if (pLeftNodeNext != 0)
          {
              pLeftNodeNext->m_pPrior = pRightNode;
          }
          else
          {
              m_pEnd = pRightNode;
          }

          if (pLeftNodePrior != 0)
          {
              pLeftNodePrior->m_pNext = pRightNode;
          }
          else
          {
              m_pStart = pRightNode;
          }

          if (pRightNodeNext != 0)
          {
              pRightNodeNext->m_pPrior = pLeftNode;
          }
          else
          {
              m_pEnd = pLeftNode;
          }

          if (pRightNodePrior != 0)
          {
              pRightNodePrior->m_pNext = pLeftNode;
          }
          else
          {
              m_pStart = pLeftNode;
          }

          pLeftNode->m_pNext  = pRightNodeNext;
          pLeftNode->m_pPrior = pRightNodePrior;

          pRightNode->m_pNext  = pLeftNodeNext;
          pRightNode->m_pPrior = pLeftNodePrior;
      }
   }
}

//////////////////////////////////////////////////////////////////////////
//                       Простое Бинарное дерево                        //
//////////////////////////////////////////////////////////////////////////

//##ModelId=416FC26A037A
template<class DataType, class ArgType>
TRsbBinTree<DataType, ArgType>::TRsbBinTree()
: m_pRootNode(NULL)
{
}

//##ModelId=416FC26A038A
template<class DataType, class ArgType>
TRsbBinTree<DataType, ArgType>::~TRsbBinTree()
{
   RemoveAll();
}

//##ModelId=416FC26A038B
template<class DataType, class ArgType>
DataType& TRsbBinTree<DataType, ArgType>::GetRoot(LPOSITION& pos)
{
   CB_PRECONDITION(pos != NULL, "Tree is empty...");
   pos = (LPOSITION)m_pRootNode;
   return m_pRootNode->m_Info;
}

//##ModelId=416FC26A0399
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::GetRootPos() const
{
   return (LPOSITION)m_pRootNode;
}

//##ModelId=416FC26A03AA
template<class DataType, class ArgType>
DataType& TRsbBinTree<DataType, ArgType>::GetLeft(LPOSITION& pos)
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   CTreeNodePtr pNode = (CTreeNodePtr)pos;
   pos = (LPOSITION)pNode->m_pLeft;
   return pNode->m_Info;
}

//##ModelId=416FC26A03B9
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::GetLeftPos(const LPOSITION pos) const
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   return (LPOSITION)(((CTreeNodePtr)pos)->m_pLeft);
}

//##ModelId=416FC26A03D8
template<class DataType, class ArgType>
DataType& TRsbBinTree<DataType, ArgType>::GetRight(LPOSITION& pos)
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   CTreeNodePtr pNode = (CTreeNodePtr)pos;
   pos = (LPOSITION)pNode->m_pRight;
   return pNode->m_Info;
}

//##ModelId=416FC26B0000
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::GetRightPos(const LPOSITION pos) const
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   return (LPOSITION)(((CTreeNodePtr)pos)->m_pRight);
}

//##ModelId=416FC26B0010
template<class DataType, class ArgType>
DataType& TRsbBinTree<DataType, ArgType>::GetParent(LPOSITION& pos)
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   CTreeNodePtr pNode = (CTreeNodePtr)pos;
   pos = (LPOSITION)pNode->m_pParent;
   return pNode->m_Info;
}

//##ModelId=416FC26B0020
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::GetParentPos(const LPOSITION pos) const
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   return (LPOSITION)(((CTreeNodePtr)pos)->m_pParent);
}

//##ModelId=416FC26B004E
template<class DataType, class ArgType>
DataType& TRsbBinTree<DataType, ArgType>::GetAt(const LPOSITION pos)
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   CTreeNodePtr pNode = (CTreeNodePtr)pos;
   return pNode->m_Info;
}

//##ModelId=416FC26B005D
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::GetEndLeftPos(const LPOSITION RootPos)
{
   LPOSITION EndLeftPos = NULL;
   LPOSITION pos = RootPos ? RootPos : GetRootPos();
   while(pos)
   {
      EndLeftPos = pos;
      pos = GetLeftPos(pos);
   }
   return EndLeftPos;
}

//##ModelId=416FC26B006D
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::GetEndRightPos(const LPOSITION RootPos)
{
   LPOSITION EndRightPos = NULL;
   LPOSITION pos = RootPos ? RootPos : GetRootPos();
   while(pos)
   {
      EndRightPos = pos;
      pos = GetRightPos(pos);
   }
   return EndRightPos;
}

//##ModelId=416FC26B007E
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::InsertLeft(const LPOSITION pos, ArgType newData, bool ExistToLeft)
{
   CTreeNodePtr pNewNode = new CTreeNode(newData);
   if(m_pRootNode)
   {
      CTreeNodePtr pNode;
      CTreeNodePtr pExistNode;
      if(!pos)
      {
         pExistNode = m_pRootNode;
         m_pRootNode = pNewNode;
         pExistNode->m_pParent = m_pRootNode;
      }
      else
      {
         pNode = (CTreeNodePtr)pos;
         pExistNode = pNode->m_pLeft;
         pNode->m_pLeft = pNewNode;
         pNewNode->m_pParent = pNode;
      }

      if(pExistNode)
      {
         pExistNode->m_pParent = pNewNode;
         if(ExistToLeft)
            pNewNode->m_pLeft = pExistNode;
         else
            pNewNode->m_pRight = pExistNode;
      }
   }
   else
      m_pRootNode = pNewNode;

   return (LPOSITION)pNewNode;
}

//##ModelId=416FC26B009C
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::InsertRight(const LPOSITION pos, ArgType newData, bool ExistToLeft)
{
   CTreeNodePtr pNewNode = new CTreeNode(newData);
   if(m_pRootNode)
   {
      CTreeNodePtr pNode;
      CTreeNodePtr pExistNode;
      if(!pos)
      {
         pExistNode = m_pRootNode;
         m_pRootNode = pNewNode;
         pExistNode->m_pParent = m_pRootNode;
      }
      else
      {
         pNode = (CTreeNodePtr)pos;
         pExistNode = pNode->m_pRight;
         pNode->m_pRight = pNewNode;
         pNewNode->m_pParent = pNode;
      }

      if(pExistNode)
      {
         pExistNode->m_pParent = pNewNode;
         if(ExistToLeft)
            pNewNode->m_pLeft = pExistNode;
         else
            pNewNode->m_pRight = pExistNode;
      }
   }
   else
      m_pRootNode = pNewNode;

   return (LPOSITION)pNewNode;
}

//##ModelId=416FC26B00BC
template<class DataType, class ArgType>
void TRsbBinTree<DataType, ArgType>::RemoveLeft(const LPOSITION pos)
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   RemoveAt(GetLeftPos(pos));
}

//##ModelId=416FC26B00CB
template<class DataType, class ArgType>
void TRsbBinTree<DataType, ArgType>::RemoveRight(const LPOSITION pos)
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   RemoveAt(GetRightPos(pos));
}

//##ModelId=416FC26B00DB
template<class DataType, class ArgType>
void TRsbBinTree<DataType, ArgType>::RemoveAt(const LPOSITION pos)
{
   if(pos)
   {
      CTreeNodePtr pDelNode = (CTreeNodePtr)pos;

      // Удаляем левое поддерево
      RemoveAt(GetLeftPos(pos));
      // Удаляем правое поддерево
      RemoveAt(GetRightPos(pos));

      // Зануляем родительскую ссылку на узел
      LPOSITION ParentPos = GetParentPos(pos);
      if(ParentPos)
      {
         CTreeNodePtr pParentNode = (CTreeNodePtr)ParentPos;
         if(GetLeftPos(ParentPos) == pos)
            pParentNode->m_pLeft = NULL;
         else
            pParentNode->m_pRight = NULL;
      }
      else
         m_pRootNode = NULL;

      // Удаляем узел из памяти
      delete pDelNode;
   }
}

//##ModelId=416FC26B00EB
template<class DataType, class ArgType>
void TRsbBinTree<DataType, ArgType>::RemoveAll()
{
   RemoveAt(GetRootPos());
}

//##ModelId=416FC26B01C5
template<class DataType, class ArgType>
bool TRsbBinTree<DataType, ArgType>::IsChild(LPOSITION ParentPos, LPOSITION ChildPos)
{
   bool bRet = false;
   LPOSITION Pos = ChildPos;
   while(Pos = GetParentPos(Pos))
   {
      if(Pos == ParentPos)
      {
         bRet = true;
         break;
      }
   }
   return bRet;
}

//##ModelId=416FC26B00FA
template<class DataType, class ArgType>
void TRsbBinTree<DataType, ArgType>::Move(LPOSITION ToPos, LPOSITION& FromPos)
{
   // Можем преместить только если узлы различны и FromPos не родитель ToPos
   if(FromPos != ToPos && !IsChild(FromPos, ToPos))
   {
      CTreeNodePtr pFromNode = (CTreeNodePtr)FromPos;
      CTreeNodePtr pToNode = (CTreeNodePtr)ToPos;
      CTreeNodePtr pFromParentNode = pFromNode->m_pParent;

      if(pFromParentNode->m_pLeft == pFromNode)
         pFromParentNode->m_pLeft = NULL;
      else
         pFromParentNode->m_pRight = NULL;

      // Удаляем левое поддерево
      RemoveAt(GetLeftPos(ToPos));
      // Удаляем правое поддерево
      RemoveAt(GetRightPos(ToPos));

      pToNode->m_Info = pFromNode->m_Info;
      pToNode->m_pLeft = pFromNode->m_pLeft;
      if(pToNode->m_pLeft)
         pToNode->m_pLeft->m_pParent = pToNode;
      pToNode->m_pRight = pFromNode->m_pRight;
      if(pToNode->m_pRight)
         pToNode->m_pRight->m_pParent = pToNode;
      FromPos = NULL;
      delete pFromNode;
   }
}

//##ModelId=416FC26B0119
template<class DataType, class ArgType>
void TRsbBinTree<DataType, ArgType>::MoveToLeft(LPOSITION ToPos, LPOSITION& FromPos)
{
   LPOSITION ToLeftPos = GetLeftPos(ToPos);
   if(ToLeftPos == NULL)
   {
      if(FromPos != ToPos && !IsChild(FromPos, ToPos))
      {
         CTreeNodePtr pFromNode = (CTreeNodePtr)FromPos;
         CTreeNodePtr pToNode = (CTreeNodePtr)ToPos;
         CTreeNodePtr pFromParentNode = pFromNode->m_pParent;
         
         if(pFromParentNode->m_pLeft == pFromNode)
            pFromParentNode->m_pLeft = NULL;
         else
            pFromParentNode->m_pRight = NULL;

         pFromNode->m_pParent = pToNode;
         pToNode->m_pLeft = pFromNode;
      }
   }
   else
   {
      Move(ToLeftPos, FromPos);
   }
}

//##ModelId=416FC26B0138
template<class DataType, class ArgType>
void TRsbBinTree<DataType, ArgType>::MoveToRight(LPOSITION ToPos, LPOSITION& FromPos)
{
   LPOSITION ToRightPos = GetRightPos(ToPos);
   if(ToRightPos == NULL)
   {
      if(FromPos != ToPos && !IsChild(FromPos, ToPos))
      {
         CTreeNodePtr pFromNode = (CTreeNodePtr)FromPos;
         CTreeNodePtr pToNode = (CTreeNodePtr)ToPos;
         CTreeNodePtr pFromParentNode = pFromNode->m_pParent;
         
         if(pFromParentNode->m_pLeft == pFromNode)
            pFromParentNode->m_pLeft = NULL;
         else
            pFromParentNode->m_pRight = NULL;
         
         pFromNode->m_pParent = pToNode;
         pToNode->m_pRight = pFromNode;
      }
   }
   else
   {
      Move(ToRightPos, FromPos);
   }
}

//##ModelId=416FC26B0148
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::Find(ArgType Data, const LPOSITION startAfter)
{
   if(IsEmpty())
      return NULL;

   LPOSITION StartPos = startAfter ? startAfter : GetRootPos();
   LPOSITION FindedPos = NULL;
   if(Data == GetAt(StartPos))
      FindedPos = StartPos;
   else
   {
      LPOSITION LeftPos = GetLeftPos(StartPos);
      LPOSITION RightPos = GetRightPos(StartPos);
      if(RightPos)
         FindedPos = Find(Data, RightPos);
      if(!FindedPos && LeftPos)
         FindedPos = Find(Data, LeftPos);
   }
   return FindedPos;
}

//##ModelId=416FC26B0167
template<class DataType, class ArgType>
LPOSITION TRsbBinTree<DataType, ArgType>::FindNext(ArgType Data, const LPOSITION PrevFindedPos, bool FindInThisBranch)
{
   LPOSITION FindedPos = NULL;
   LPOSITION LeftPos = GetLeftPos(PrevFindedPos);
   LPOSITION RightPos = GetRightPos(PrevFindedPos);
   if(RightPos)
      FindedPos = Find(Data, RightPos);
   if(!FindedPos && LeftPos)
      FindedPos = Find(Data, LeftPos);
   if(!FindInThisBranch)
   {
      LPOSITION ParentPos;
      LPOSITION CurPos = PrevFindedPos;
      while(!FindedPos && (ParentPos = GetParentPos(CurPos)))
      {
         LeftPos = GetLeftPos(ParentPos);
         if(LeftPos && IsRightNodePos(CurPos))
            FindedPos = Find(Data, LeftPos);
         CurPos = ParentPos;
      }
   }
   return FindedPos;
}

//##ModelId=416FC26B0179
template<class DataType, class ArgType>
bool TRsbBinTree<DataType, ArgType>::IsRightNodePos(const LPOSITION pos)
{
   LPOSITION ParentPos = GetParentPos(pos);
   if(ParentPos && (pos == GetRightPos(ParentPos)))
      return true;

   return false;
}

//##ModelId=416FC26B0196
template<class DataType, class ArgType>
bool TRsbBinTree<DataType, ArgType>::IsLeftNodePos(const LPOSITION pos)
{
   LPOSITION ParentPos = GetParentPos(pos);
   if(ParentPos && (pos == GetLeftPos(ParentPos)))
      return true;
   
   return false;
}

//##ModelId=416FC26B01A5
template<class DataType, class ArgType>
bool TRsbBinTree<DataType, ArgType>::IsRootNodePos(const LPOSITION pos)
{
   CB_PRECONDITION(pos != NULL, "Position of tree node is not valid...");
   return pos == GetRootPos();
}

//##ModelId=416FC26B01B5
template<class DataType, class ArgType>
bool TRsbBinTree<DataType, ArgType>::IsEmpty()
{
   return GetRootPos() == NULL;
}

//////////////////////////////////////////////////////////////////////////
//               Универсальные "умные" указатели                        //
//////////////////////////////////////////////////////////////////////////

// Указатель для объекта полученного оператором new

//##ModelId=416FC26B039A
template<class TypePtr>
TRsbAutoPtr<TypePtr>::TRsbAutoPtr(TypePtr* p) throw()
: the_p(p)
{
}

//##ModelId=416FC26C000F
template<class TypePtr>
TRsbAutoPtr<TypePtr>::TRsbAutoPtr(TRsbAutoPtr<TypePtr>& a) throw()
: the_p(a.release())
{
}

//##ModelId=416FC26C0011
template<class TypePtr>
void TRsbAutoPtr<TypePtr>::operator =(TRsbAutoPtr<TypePtr>& rhs)
{
   reset(rhs.release());
}

//##ModelId=416FC26C0020
template<class TypePtr>
TRsbAutoPtr<TypePtr>::~TRsbAutoPtr()
{
   if(the_p)
      delete the_p;
}

//##ModelId=416FC26C003E
template<class TypePtr>
TRsbAutoPtr<TypePtr>& TRsbAutoPtr<TypePtr>::operator =(TypePtr* rhs)
{
   if(the_p)
      delete the_p;

   the_p = rhs;
   return *this;
}

//##ModelId=416FC26C004E
template<class TypePtr>
TypePtr& TRsbAutoPtr<TypePtr>::operator *()const throw()
{
   return *the_p;
}

//##ModelId=416FC26C0050
template<class TypePtr>
TypePtr* TRsbAutoPtr<TypePtr>::operator ->()const throw()
{
   return the_p;
}

//##ModelId=416FC26C005D
template<class TypePtr>
TRsbAutoPtr<TypePtr>::operator TypePtr*()const throw()
{
   return the_p;
}


//##ModelId=416FC26C006D
template<class TypePtr>
TypePtr* TRsbAutoPtr<TypePtr>::get()const throw()
{
   return the_p;
}

//##ModelId=416FC26C006F
template<class TypePtr>
TypePtr* TRsbAutoPtr<TypePtr>::release() throw()
{
   return reset();
}

//##ModelId=416FC26C007D
template<class TypePtr>
TypePtr* TRsbAutoPtr<TypePtr>::reset(TypePtr* p) throw()
{
   TypePtr* tmp = the_p;
   the_p = p;
   return tmp;
}


//////////////////////////////////////////////////////////////////////////
