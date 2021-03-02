// Filename: SaxErrorHandler.cpp
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added registration of wide-char string element handler.
//                   and ReportError() helper function.

#include "XmlCommon.h"
#include "SaxErrorHandler.h"

#include "minmax.h"


// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
#define  MAX_ERROR_TEXT_SIZE    800


// -----------------------------------------------------------------------------
CSaxErrorHandler::CSaxErrorHandler()
{
 m_attachElementHandler  = NULL;
 m_attachWElementHandler = NULL;
}

// -----------------------------------------------------------------------------
CSaxErrorHandler::~CSaxErrorHandler()
{
 m_attachElementHandler  = NULL;
 m_attachWElementHandler = NULL;
}

// -----------------------------------------------------------------------------
long __stdcall CSaxErrorHandler::QueryInterface(const struct _GUID &riid, void ** ppvObject)
{
 // Not implemented as this class is not a COM object.
 return 0;
}

// -----------------------------------------------------------------------------
unsigned long __stdcall CSaxErrorHandler::AddRef()
{
 // Not implemented as this class is not a COM object.
 return 0;
}

// -----------------------------------------------------------------------------
unsigned long __stdcall CSaxErrorHandler::Release()
{
 // Not implemented as this class is not a COM object.
 return 0;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CSaxErrorHandler::error( 
    /* [in] */ ISAXLocator __RPC_FAR *pLocator,
    /* [in] */ const wchar_t         *pwchErrorMessage,
    /* [in] */ HRESULT                errCode)
{
 ReportError(pLocator, pwchErrorMessage, errCode);

 return S_OK;
}

// -----------------------------------------------------------------------------        
HRESULT STDMETHODCALLTYPE CSaxErrorHandler::fatalError( 
    /* [in] */ ISAXLocator __RPC_FAR *pLocator,
    /* [in] */ const wchar_t         *pwchErrorMessage,
    /* [in] */ HRESULT                errCode)
{
 ReportError(pLocator, pwchErrorMessage, errCode);

 return S_OK;
}

// -----------------------------------------------------------------------------        
HRESULT STDMETHODCALLTYPE CSaxErrorHandler::ignorableWarning( 
    /* [in] */ ISAXLocator __RPC_FAR *pLocator,
    /* [in] */ const wchar_t         *pwchErrorMessage,
    /* [in] */ HRESULT                errCode)
{
 return S_OK;
}

// -----------------------------------------------------------------------------
void CSaxErrorHandler::AttachElementHandler(IXmlElementHandler *pElementHandler)
{
 m_attachElementHandler  = pElementHandler;
 m_attachWElementHandler = NULL;
}

// -----------------------------------------------------------------------------
void CSaxErrorHandler::AttachElementHandler(IWXmlElementHandler *pWElementHandler)
{
 m_attachWElementHandler = pWElementHandler;
 m_attachElementHandler  = NULL;
}

// -----------------------------------------------------------------------------
void CSaxErrorHandler::DetachElementHandler()
{
 m_attachElementHandler  = NULL;
 m_attachWElementHandler = NULL;
}

// -----------------------------------------------------------------------------
void CSaxErrorHandler::ReportError(
    /* [in] */ ISAXLocator   *pLocator,
    /* [in] */ const wchar_t *pwchErrorMessage,
    /* [in] */ HRESULT        errCode)
{
 // Get the line and column where the error occurred.
 int  line   = 0;
 int  column = 0;


 if(pLocator != NULL)
  {
   pLocator->getLineNumber(&line);
   pLocator->getColumnNumber(&column);
  }

 // Get length of error message.
 int  len = (int)wcslen(pwchErrorMessage);


 len = min(len, MAX_ERROR_TEXT_SIZE - 1);

 if(m_attachElementHandler != NULL)
  {
   // SINGLE-CHAR case...

   // Get the error message text.
   char  szErrorMsg[MAX_ERROR_TEXT_SIZE];


   wcstombs(szErrorMsg, pwchErrorMessage, len);
   szErrorMsg[len] = 0;

   // Delegate error to element handler.
   m_attachElementHandler->OnXmlError(line, column, szErrorMsg, errCode);
  }
 else if(m_attachWElementHandler != NULL)
  {
   // WIDE-CHAR case...

   // Get the error message text.
   wchar_t  wszErrorMsg[MAX_ERROR_TEXT_SIZE];


   wcsncpy(wszErrorMsg, pwchErrorMessage, len);
   wszErrorMsg[len] = 0;

   // Delegate error to element handler.
   m_attachWElementHandler->OnXmlError(line, column, wszErrorMsg, errCode);
  }
}

/* <-- EOF --> */