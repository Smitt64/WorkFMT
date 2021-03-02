/*-----------------------------------------------------------------------*-

 File Name   : rsforms\creator.hpp
 Programmer  : Leonid S. Pavlov

 Sybsystem   : RSForms library
 Description : 

 Source      : 

 Library(s)  : RSFSrv.d32

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Mar 28, 2002  Leonid S. Pavlov (LSP) - Create file
 Jun 21, 2002  Nechaev Pavel D. (NPD) - Modify file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSCREATOR_H__
#define __RSFORMSCREATOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/


template <class Creator> 
class RsfmGeneralCreator : public Creator
   {
protected:
   RsfmGeneralCreator (TAppObj* app) : Creator (app) 
      {
      }

   void initialize (IParmInfo* pInfo, IRsObj* outerObj) throw (XRSFormsError)
      {
      TRsStat stat = constructObject (outerObj, 0, 0, pInfo);
      if (stat != RSL_STAT_OK)
         throw XRSFormsError (getApp(), stat);
      }
public:

   _NoAddRefReleaseOnTRsPtr<typename Creator::UserClassType>* operator->()
      {
      if (!isInited ())
         initialize (NULL, NULL);
      return Creator::operator->();
      }
   };


template <class Creator> 
class RsfmControlCreator : public RsfmGeneralCreator<Creator>
   {
public:
   RsfmControlCreator (TAppObj* app) : RsfmGeneralCreator<Creator> (app) 
      {
      }

   /* Не инициализированный контрол */
   void init()                
      {
      initialize ((IParmInfo*)NULL, NULL); 
      }

   /* Контрол для создания по типу */                                                              
   void init (const char* szType, const char* szName)                
      { 
      init (szType, szName, FT_USER_DEF); 
      }

   /* Контрол для создания по типу */                                                              
   void init (const char* szType, const char* szName, evFilterType et) 
      { 
      TParmArray prm(getApp(), 7);
      prm[0] = szType;
      prm[1] = szName;
      prm[2] = et;
      initialize (prm, NULL);
      }

   //void init(const char* szLib, const char* szRes, const char* szName)
   //void init(const char* szLib, const char* szRes, const char* szName, evFilterType et)

   /* Дочерний элемент формы (связывание по имени) */
   void init (IContainer* owner, const char* szName)                   
      { 
      TParmArray prm(getApp(), 7);
      prm[1] = szName;
      prm[3] = owner;
      initialize (prm, owner); 
      }

   /* Клонированный контрол */
   void init (IControl* source, ConstructType ct)
      {
      TParmArray prm(getApp(), 7);
      prm[4] = source;
      prm[5] = ct;
      initialize (prm, NULL);
      }

   /* используется при инициализации InfoView */
   void init (IFrameWindow* owner)
      {
      TParmArray prm(getApp(), 7);
      prm[6] = owner;
      initialize (prm, NULL); 
      }

   operator IFrameView* ()
      {
      return TRsPtrX<IFrameView, IID_FRAME_VIEW>((*this)->rsObj());
      }
   };



template <class T> 
class control : public RsfmControlCreator< TRsHeapOwnerCreator<T> >
   {
public:
   control (TAppObj* app) : RsfmControlCreator< TRsHeapOwnerCreator<T> > (app)
      {
      }

   // Для создания поименованной формы верхнего уровня
   control (TAppObj* app, const char* name) : RsfmControlCreator< TRsHeapOwnerCreator<T> > (app)
      {
      init (RSFORM_CTRL, name, FT_USER_DEF);
      }

   };

template <class T> 
class auto_control : public RsfmControlCreator< TRsAutoOwnerCreator<T> >
   {
public:
   auto_control (TAppObj* app) : RsfmControlCreator< TRsAutoOwnerCreator<T> > (app)
      {
      }
   };

template <class T> 
class member_control : public RsfmControlCreator< TRsHeapMemberOwnerCreator<T> >
   {
public:
   member_control (TAppObj* app) : RsfmControlCreator< TRsHeapMemberOwnerCreator<T> > (app)
      {
      }
   };




template <class Creator> 
class RsfmCmdSetCreator : public RsfmGeneralCreator<Creator>
   {
public:
   RsfmCmdSetCreator (TAppObj* app) : RsfmGeneralCreator<Creator> (app) 
      {
      }

    /* Элемент для загрузки из стрима */                                                            
   void init (const char* szName, const char* szLib, const char* szRes)                    
      {
      TParmArray prm(getApp(), 4);
      prm[0] = szName;
      prm[1] = NULL;
      prm[2] = szLib;
      prm[3] = szRes;
      initialize (prm, NULL); 
      }
                                                                                                    
    /* Дочерний элемент для связывания по имени */                                                  
   void init (ICmdSetContainer* pOwner, const char* szName)            
      {
      TParmArray prm (getApp(), 4);
      prm[0] = szName;
      prm[1] = pOwner;
      prm[2] = NULL;
      prm[3] = NULL;
      initialize (prm, pOwner); 
      }
   };


template <class T> 
class cmdset : public RsfmCmdSetCreator< TRsHeapOwnerCreator<T> >
   {
public:
   cmdset (TAppObj* app) : RsfmCmdSetCreator< TRsHeapOwnerCreator<T> > (app)
      {
      }
   };

template <class T> 
class auto_cmdset : public RsfmCmdSetCreator< TRsAutoOwnerCreator<T> >
   {
public:
   auto_cmdset (TAppObj* app) : RsfmCmdSetCreator< TRsAutoOwnerCreator<T> > (app)
      {
      }
   };

template <class T> 
class member_cmdset : public RsfmCmdSetCreator< TRsHeapMemberOwnerCreator<T> >
   {
public:
   member_cmdset (TAppObj* app) : RsfmCmdSetCreator< TRsHeapMemberOwnerCreator<T> > (app)
      {
      }
   };


template <class Creator> 
class RsfmReportTableCreator : public RsfmGeneralCreator<Creator>
   {
public:
   RsfmReportTableCreator (TAppObj* app) : RsfmGeneralCreator<Creator> (app) 
      {
      }
      
   void init()                
      {
      initialize ((IParmInfo*)NULL, NULL); 
      }

   /* Контрол для создания по типу */                                                              
   void init (const char* szName)                
      { 
      init (szName, FT_USER_DEF); 
      }

   /* Контрол для создания по типу */                                                              
   void init (const char* szName, evFilterType et) 
      { 
      TParmArray prm(getApp(), 7);
      prm[0] = RSREPTABLE_CTRL;
      prm[1] = szName;
      prm[2] = et;
      initialize (prm, NULL); 
      }

   /* Дочерний элемент формы (связывание по имени) */
   void init (IContainer* owner, const char* szName)                   
      { 
      TParmArray prm(getApp(), 7);
      prm[0] = RSREPTABLE_CTRL;
      prm[1] = szName;
      prm[3] = owner;
      initialize (prm, owner); 
      }

   /* Клонированный контрол */
   void init (IControl* source, ConstructType ct)
      {
      TParmArray prm(getApp(), 7);
      prm[4] = source;
      prm[5] = ct;
      initialize (prm, NULL);
      }
   };
   
   
template <class T>
class report_table : public RsfmReportTableCreator< TRsHeapOwnerCreator<T> >
   {
public:
   report_table (TAppObj* app) : RsfmReportTableCreator< TRsHeapOwnerCreator<T> > (app)
      {
      }
   };

template <class T> 
class auto_report_table : public RsfmReportTableCreator< TRsAutoOwnerCreator<T> >
   {
public:
   auto_report_table (TAppObj* app) : RsfmReportTableCreator< TRsAutoOwnerCreator<T> > (app)
      {
      }
   };

template <class T> 
class member_report_table : public RsfmReportTableCreator< TRsHeapMemberOwnerCreator<T> >
   {
public:
   member_report_table (TAppObj* app) : RsfmReportTableCreator< TRsHeapMemberOwnerCreator<T> > (app)
      {
      }
   };


template <class T> 
struct window : public RsfmGeneralCreator< TRsHeapOwnerCreator<T> >
   {
   // конструктор без фазы создания окна
   window (TAppObj* app) 
      : RsfmGeneralCreator< TRsHeapOwnerCreator<T> > (app) 
      {
      initialize ((IParmInfo*)NULL, NULL);
      }

   // создание фрейма для дальнейшей настройки и использования
   window (TAppObj* app, const char* szName, unsigned long dwStyle, IWindow* owner = NULL) throw (XRSFormsError) 
      : RsfmGeneralCreator< TRsHeapOwnerCreator<T> > (app) 
      { 
      initialize ((IParmInfo*)NULL, NULL);
      (*this)->create (szName, dwStyle, owner); 
      }

   // создание RSCOM-обёртки для внешнего окна
   window (TAppObj* app, HWND handle) throw (XRSFormsError) 
      : RsfmGeneralCreator< TRsHeapOwnerCreator<T> > (app) 
      { 
      TParmArray prm(app, 1);
      prm[0] = (long)handle;
      initialize (prm, NULL);
      }
   };

template <class T> 
struct application : public RsfmGeneralCreator< TRsAutoOwnerCreator<T> >
   {
   application (TAppObj* app) 
         : RsfmGeneralCreator< TRsAutoOwnerCreator<T> > (app) 
      {
      initialize ((IParmInfo*)NULL, NULL);
      }

   application (TAppObj* app, char* appName, const char* hlpName = NULL) throw (XRSFormsError)
         : RsfmGeneralCreator< TRsAutoOwnerCreator<T> > (app) 
      {
      TParmArray prm (app, 2);
      prm[0] = appName;
      prm[1] = hlpName;
      initialize (prm, NULL);
      }
   };




//template <class T>
//class application : public TRsObjAuto<T>
//   {
//public:
//   application(TAppObj* app): TRsObjAuto<T> (app, NULL, NULL, 0) 
//      {
//      }
//
//   application(TAppObj* app, const char* appName,const char *hlpName = NULL) throw(XRSFormsError) :
//         TRsObjAuto<T> (app, NULL, NULL, 0) 
//      { 
//      (*this)->init (appName, hlpName); 
//      }
//   };

/*-----------------------------------------------------------------------*/

template <class T> 
class auto_reportsgroup : public TRsObjAuto<T>
   {
public:
   /* Неинициализированный объект */
   auto_reportsgroup (TAppObj* app) : TRsObjAuto<T> (app, NULL, NULL, 0) 
      {
      }
   };

template <class T> 
class reportsgroup : public TRsObjDyn<T>
   {
public:
   /* Неинициализированный объект */
   reportsgroup (TAppObj* app) : TRsObjDyn<T> (app, NULL, NULL, 0) 
      {
      }
   };

/*---------------------------------------------------------------------------*/

template <class T> 
struct report : public RsfmGeneralCreator< TRsHeapOwnerCreator<T> >
   {
   // конструктор без фазы создания
   report (TAppObj* app) 
      : RsfmGeneralCreator< TRsHeapOwnerCreator<T> > (app) 
      {
      TParmArray prm (getApp (), 3);
      prm[0] = "";
      prm[1] = RSREPORTOBJ_CTRL;
      prm[2] = FT_USER_DEF;
      initialize (prm, NULL); 
      }

   // создание отчета без и с использованием группировки
   bool create(IReportsGroup* p_group = NULL)
      {
      return p->init (p_group, p);             
      }                                       

   // создание отчета по шаблону без и с использованием группировки
   bool create(const char* szLib, const char* szRes, IReportsGroup* p_group = NULL)      
      {                                                                                   
      p->setTemplate (szLib, szRes);                                                       
      return p->init (p_group, p);                                                         
      }                                                                                   
   };

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSCREATOR_H__
