//-*--------------------------------------------------------------------------*-
//
// File Name   : rsfCtlModule.h
//
// Copyright (c) 2002 - 2008 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// May 23,2002  Alexander N. Kostarev - Create file
//-*--------------------------------------------------------------------------*-

//
// Functions, macros' and interfaces needed by modules with RS-controls
//

#ifndef __RSFCTLMODULE_H_
#define __RSFCTLMODULE_H_

#ifdef RSF_EXPORT
    #define  RSF_API  __declspec(dllexport)
#else
    #define  RSF_API  __declspec(dllimport)
#endif


////////////////////////
// Forward declarations
interface IRSImageList;
interface IRSMenu;
interface RSMenuItems;


namespace rsfm
{
 /*+--------------------------------------------------------------------------------------------+*/
 /*| IRSObject: внутренний интерфейс дл€ взаимосв€зи с RS-контейнерами                          |*/
 /*+--------------------------------------------------------------------------------------------+*/

 MIDL_INTERFACE("F64CFD94-39CF-11d7-9B05-00047585070C")
 IRSObject : public IUnknown
 {
  virtual HRESULT STDMETHODCALLTYPE CreatePropertyPage(REFCLSID rclsid, LPPROPERTYPAGE *pp) = 0;
  virtual void STDMETHODCALLTYPE OnContainerLoaded() = 0;
  virtual void STDMETHODCALLTYPE OnImageListChanged() = 0;

  virtual HRESULT STDMETHODCALLTYPE SaveSessionData(LPSTREAM lpStm) = 0;
  virtual void STDMETHODCALLTYPE LoadSessionData(LPSTREAM lpStm) = 0;
 };

 typedef IRSObject __RPC_FAR  *LPRSOBJECT;

 /*+--------------------------------------------------------------------------------------------+*/
 /*| IRSObject2: внутренний интерфейс дл€ взаимосв€зи с RS-контейнерами                         |*/
 /*+--------------------------------------------------------------------------------------------+*/

 MIDL_INTERFACE("AEBA3A30-8D7A-4007-AF89-EA02A11172A0")
 IRSObject2 : public IRSObject
 {
  virtual bool STDMETHODCALLTYPE GetContextMenu(IRSMenu **pp) = 0;
  virtual bool STDMETHODCALLTYPE OnCommand(IRSMenu *p, DWORD cmd) = 0;
  virtual bool STDMETHODCALLTYPE OnUpdateUIState(IRSMenu *p, DWORD cmd) = 0;
 };

 typedef IRSObject2 __RPC_FAR  *LPRSOBJECT2;


 /*+--------------------------------------------------------------------------------------------+*/
 /*| INameList: special collback interface for building available imagelist names               |*/
 /*+--------------------------------------------------------------------------------------------+*/

 #ifndef __AXCITF_H_
 enum enFurtherProcessing   // варианты последующего перечислени€ элементов
    {
     fpContinue     = 0,
     fpStop         = 1,
     fpSkipChildren = 2
    };

 interface INameList
 {                       // sz should not be copied but must be released
  virtual enFurtherProcessing AddName(BSTR sz, LPUNKNOWN lp) = 0;
 };

 typedef INameList __RPC_FAR  *LPNAMELIST;
 #endif  // __AXCITF_H_


 /*+--------------------------------------------------------------------------------------------+*/
 /*| IRSObjectSite: интерфейс поддержки управл€ющих элементов на стороне контейнера             |*/
 /*+--------------------------------------------------------------------------------------------+*/

 MIDL_INTERFACE("43850BE6-13FF-11d7-9ADC-00047585070C")
 IRSObjectSite : public IUnknown
 {
  virtual HRESULT STDMETHODCALLTYPE ShowMenu(int x, int y, RSMenuItems *lp = NULL) = 0;  // (x, y) положение в координатах экрана

  virtual bool STDMETHODCALLTYPE SearchImageList(LPOLESTR szFullName, IRSImageList **pp) = 0;
  virtual void STDMETHODCALLTYPE BuildImageListNames(LPNAMELIST lp) = 0;

  virtual bool STDMETHODCALLTYPE SearchContextMenu(LPOLESTR szFullName, IRSMenu **pp) = 0;
  virtual bool STDMETHODCALLTYPE FindFrameMenu(LPOLESTR szName, IRSMenu **pp) = 0;
  virtual void STDMETHODCALLTYPE BuildContextMenuNames(LPNAMELIST lp) = 0;
 };

 typedef IRSObjectSite __RPC_FAR  *LPRSOBJECTSITE;


 /*+--------------------------------------------------------------------------------------------+*/
 /*| ISiteCustomise:                                                                            |*/
 /*+--------------------------------------------------------------------------------------------+*/

 MIDL_INTERFACE("9EDBF0AE-1E95-4bd2-8246-380FB9516BDE")
 ISiteCustomise : public IUnknown
 {
  virtual void STDMETHODCALLTYPE RefreshClsid() = 0;
 };


 /*+--------------------------------------------------------------------------------------------+*/
 /*| IBorderable: исполльзуетс€ при выравнивании элементов с наложением границ                  |*/
 /*+--------------------------------------------------------------------------------------------+*/

 MIDL_INTERFACE("D5975BF7-0601-4c6c-BF2A-67868B756A87")
 IBorderable : public IUnknown
 {
  virtual unsigned short STDMETHODCALLTYPE GetBorderWidth() = 0;
  virtual unsigned short STDMETHODCALLTYPE GetBorderHeight() = 0;
 };


 /*+--------------------------------------------------------------------------------------------+*/
 /*| enCollType - задает виды коллекций объектов, с которыми работает IRSBindContainer          |*/
 /*+--------------------------------------------------------------------------------------------+*/

 typedef enum enCollType
       {
        ctNotify = 1,
        ctBind   = 2
       } enCollType;

 /*+--------------------------------------------------------------------------------------------+*/
 /*| IRSBindContainer - поддержка эффективного механизма св€зывани€ [binding] элементов         |*/
 /*+--------------------------------------------------------------------------------------------+*/

 MIDL_INTERFACE("92E12A44-3849-11d7-9B04-00047585070C")
 IRSBindContainer : public IUnknown
 {
  virtual HRESULT STDMETHODCALLTYPE EnumObjects(enCollType ct, LPENUMUNKNOWN *pp) = 0;
  virtual HRESULT STDMETHODCALLTYPE AddObject(enCollType ct, LPUNKNOWN lpUnk) = 0;
  virtual HRESULT STDMETHODCALLTYPE RemoveObject(enCollType ct, LPUNKNOWN lpUnk) = 0;
 };

 typedef IRSBindContainer __RPC_FAR  *LPRSBINDCONTAINER;


 /*+--------------------------------------------------------------------------------------------+*/
 /*| IRSContainer - внутренний интерфейс дл€ взаимодействи€ с RS-Controls                       |*/
 /*+--------------------------------------------------------------------------------------------+*/

 MIDL_INTERFACE("A1F42F64-39BB-11d7-9B05-00047585070C")
 IRSContainer : public IUnknown
 {
  virtual HRESULT STDMETHODCALLTYPE GetBindContainer(LPRSBINDCONTAINER *pp) = 0;

  template<class Q> HRESULT GetRootContainer(Q **pp)
  {
   LPRSBINDCONTAINER  lp = NULL;
   HRESULT  hr = GetBindContainer(&lp); // get strong reference

   ATLASSERT(SUCCEEDED(hr) && lp || FAILED(hr) && !lp);

   if(SUCCEEDED(hr))
    {
     hr = lp->QueryInterface(pp);
     lp->Release();
    }

   return hr;
  }
 };

 typedef IRSContainer __RPC_FAR  *LPRSCONTAINER;


 /*+--------------------------------------------------------------------------------------------+*/
 /*| IRSTooltipProvider - взаимодействие с tooltip provider'ом                                  |*/
 /*+--------------------------------------------------------------------------------------------+*/

 MIDL_INTERFACE("11504151-D044-447a-8D19-860F6BA2F317")
 IRSTooltipProvider : public IUnknown
 {
  virtual HWND STDMETHODCALLTYPE GetTooltipCtrl() = 0;
 };

 #define  SID_SRSTooltipProvider  __uuidof(rsfm::IRSTooltipProvider)
}   // rsfm

#ifdef RSF_USING_RSFM
    using namespace rsfm;
#endif


/*+--------------------------------------------------------------------------------------------+*/
/*| RSFCONTROLINFO - предоставление библиотекой информации о поддерживаемых ею контролах       |*/
/*+--------------------------------------------------------------------------------------------+*/

typedef struct tagRSFControlInfo
      {
       const CLSID *pclsid;// CLSID контрола
       UINT labelid;       // им€ контрола
       UINT bitmapid;      // картинка
       UINT groupid;       // им€ группы
      } RSFCONTROLINFO;

/*+--------------------------------------------------------------------------------------------+*/
/*| RSFCONTROLINFO2 - предоставление библиотекой информации о поддерживаемых ею контролах      |*/
/*+--------------------------------------------------------------------------------------------+*/

typedef struct tagRSFControlInfo2
      {
       unsigned       version;       // верси€ структуры. ”становить в 0.
       const CLSID   *pclsid;        // CLSID контрола
       const WCHAR   *label;         // им€ контрола
       const WCHAR   *group;         // им€ группы
       const WCHAR   *description;   // краткое описание контрола
       const WCHAR   *helpFile;      // им€ chm файла справки
       const WCHAR   *helpTopic;     // им€ топика справки
       HBITMAP        bitmap;        // картинка
       UINT           bitmapid;      // id картинки. »спользуетс€ если bitmap == NULL
      } RSFCONTROLINFO2;


/*+--------------------------------------------------------------------------------------------+*/
/*| Helper macros for support control info                                                     |*/
/*+--------------------------------------------------------------------------------------------+*/

#define  RSF_BEGIN_CONTROLS_MAP(pMap)  static RSFCONTROLINFO pMap[] = {

#define  RSF_BEGIN_CONTROLS_MAP2(pMap)  RSFCONTROLINFO2 pMap[] = {

#define  RSF_CONTROL_ENTRY(clsid, labelid, bitmapid, groupid)  {&clsid, labelid, bitmapid, groupid},

#define  RSF_CONTROL_ENTRY2(clsid, label, bitmapid, group)  {0, &clsid, label, group, NULL, NULL, NULL, NULL, bitmapid},

#define  RSF_END_CONTROLS_MAP()  };

#define  RSF_IMPLEMENT_CONTROLS_INFO(pMap) \
extern "C" __declspec(dllexport) HRESULT RSGetControlInfo(UINT iIndex, /*[out]*/ RSFCONTROLINFO* pInfo) \
{ \
    if(iIndex >= sizeof(pMap)/sizeof(RSFCONTROLINFO)) \
        return E_UNEXPECTED; \
    if(NULL == pInfo) \
        return  E_POINTER; \
    *pInfo = pMap[iIndex]; \
    return S_OK; \
} \
extern "C" __declspec(dllexport) LONG RSGetControlInfoCount() \
{ \
    return sizeof(pMap)/sizeof(RSFCONTROLINFO); \
}

#define  RSF_IMPLEMENT_CONTROLS_INFO2(pMap) \
extern "C" __declspec(dllexport) HRESULT RSGetControlInfo2 (UINT iIndex, /*[out]*/ RSFCONTROLINFO2 const** pInfo) \
{ \
    if(iIndex >= sizeof(pMap)/sizeof(RSFCONTROLINFO2)) \
        return E_UNEXPECTED; \
    if(NULL == pInfo) \
        return  E_POINTER; \
    *pInfo = pMap + iIndex; \
    return S_OK; \
} \
extern "C" __declspec(dllexport) LONG RSGetControlInfoCount() \
{ \
    return sizeof(pMap)/sizeof(RSFCONTROLINFO2); \
}


/*+--------------------------------------------------------------------------------------------+*/
/*| Access to RSCreateInstance through LoadLibrary                                             |*/
/*+--------------------------------------------------------------------------------------------+*/

typedef HRESULT (*RSFCREATORFUNC)(REFCLSID rclsid, LPUNKNOWN pUnkOuter, REFIID riid, LPVOID *ppv);

inline HRESULT RSCreateInstance(HMODULE hModule, REFCLSID rclsid, LPUNKNOWN lpUnkOuter, REFIID riid, LPVOID *pp)
{
 if(RSFCREATORFUNC pfn = reinterpret_cast<RSFCREATORFUNC>(::GetProcAddress(hModule, "RSCreateInstance")))
  {
   return pfn(rclsid, lpUnkOuter, riid, pp);
  }

 return E_UNEXPECTED;
}

/*+--------------------------------------------------------------------------------------------+*/
/*| Access to RSCreateInstanceEx through LoadLibrary                                           |*/
/*+--------------------------------------------------------------------------------------------+*/

typedef HRESULT (*RSFCREATORFUNCEX)(LPCOLESTR szAssemblyName, LPCOLESTR szClassName, LPUNKNOWN lpUnkOuter, REFIID riid, LPVOID * ppv);

inline HRESULT RSCreateInstanceEx (HMODULE hModule, LPCOLESTR szAssemblyName, LPCOLESTR szClassName, LPUNKNOWN lpUnkOuter, REFIID riid, LPVOID* pp)
{
 if(RSFCREATORFUNCEX pfn = reinterpret_cast<RSFCREATORFUNCEX>(::GetProcAddress(hModule, "RSCreateInstanceEx")))
  {
   return pfn(szAssemblyName, szClassName, lpUnkOuter, riid, pp);
  }

 return E_UNEXPECTED;
}


/*+--------------------------------------------------------------------------------------------+*/
/*| Helper macros for support RSCreateInstance                                                 |*/
/*+--------------------------------------------------------------------------------------------+*/

#define  RSF_BEGIN_CONTROLS_CREATEINSTANCE() \
           extern "C" __declspec(dllexport) HRESULT RSCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* ppv) \
           {

#define  RSF_BEGIN_CONTROLS_CREATEINSTANCE_LIB(module) \
           extern "C" HRESULT module##_RSCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* ppv) \
           {

#define  RSF_CONTROL_CREATEINSTANCE(clsid, pcf) \
            if(InlineIsEqualGUID(rclsid, clsid)) return pcf(pUnkOuter, riid, ppv); \


#define  RSF_CONTROL_CREATEINSTANCE_ATL(clsid, class) \
            if(InlineIsEqualGUID(rclsid, clsid)) \
              return class::_CreatorClass::CreateInstance(pUnkOuter, riid, ppv); \


#define  RSF_END_CONTROLS_CREATEINSTANCE() \
            return ::CoCreateInstance(rclsid, pUnkOuter, CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER, riid, ppv); \
            }

#define  RSF_END_CONTROLS_CREATEINSTANCE_EX(module) \
    return ::CoCreateInstance(rclsid, pUnkOuter, CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER, riid, ppv); \
} \
extern "C" __declspec(dllexport) HRESULT module##_RSCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* ppv) \
{ \
    return RSCreateInstance(rclsid, pUnkOuter, riid, ppv); \
}

#define  RSF_END_CONTROLS_CREATEINSTANCE_()  return CLASS_E_CLASSNOTAVAILABLE; }


#define RSF_DEFINE_CREATEINSTANCE_TEMPLATE \
           template<class Q> inline HRESULT RSCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, Q **ppv) \
           { \
            return RSCreateInstance(rclsid, pUnkOuter, __uuidof(Q), (void **)ppv); \
           }

#define RSF_DEFINE_CREATEINSTANCE_TEMPLATE_EX(module) \
           template<class Q> inline HRESULT module##_RSCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, Q **ppv) \
           { \
               return module##_RSCreateInstance(rclsid, pUnkOuter, __uuidof(Q), (void **)ppv); \
           }


/*+--------------------------------------------------------------------------------------------+*/
/*| Ёкспортируемое API [дл€ статического импорта не предназначено]                             |*/
/*+--------------------------------------------------------------------------------------------+*/

#ifdef RSF_EXPORT
   // Provide information about dll objects
   extern "C" RSF_API HRESULT RSGetControlInfo(UINT iIndex, /*[out]*/ RSFCONTROLINFO *pInfo);
   extern "C" RSF_API HRESULT RSGetControlInfo2(UINT iIndex, /*[out]*/ RSFCONTROLINFO2 const **pInfo);
   extern "C" RSF_API LONG RSGetControlInfoCount();

   // InProc creator of dll objects
   extern "C" RSF_API HRESULT RSCreateInstance(REFCLSID rclsid, LPUNKNOWN lpUnkOuter, REFIID riid, LPVOID *ppv);
#endif // RSF_EXPORT

#endif // __RSFCTLMODULE_H_

/* <-- EOF --> */