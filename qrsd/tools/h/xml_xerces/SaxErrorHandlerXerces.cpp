// Filename: SaxErrorHandler.cpp
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added registration of wide-char string element handler.
//                   and ReportError() helper function.

#include "XmlCommon.h"
#include "SaxErrorHandlerXerces.h"

#include "minmax.h"


// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
#define  MAX_ERROR_TEXT_SIZE    800


// -----------------------------------------------------------------------------
CSaxErrorHandlerXerces::CSaxErrorHandlerXerces()
{
 m_attachElementHandler  = NULL;
 m_attachWElementHandler = NULL;
}

// -----------------------------------------------------------------------------
CSaxErrorHandlerXerces::~CSaxErrorHandlerXerces()
{
 m_attachElementHandler  = NULL;
 m_attachWElementHandler = NULL;
}

// -----------------------------------------------------------------------------

void CSaxErrorHandlerXerces::error(const SAXParseException& exc)
{
 ReportError(exc);

}

// -----------------------------------------------------------------------------        
void CSaxErrorHandlerXerces::fatalError(const SAXParseException& exc)
{
 ReportError(exc);

}

// -----------------------------------------------------------------------------        
 void CSaxErrorHandlerXerces::warning(const SAXParseException& exc)
{

}

// -----------------------------------------------------------------------------
void CSaxErrorHandlerXerces::AttachElementHandler(IXmlElementHandler *pElementHandler)
{
 m_attachElementHandler  = pElementHandler;
 m_attachWElementHandler = nullptr;
}

// -----------------------------------------------------------------------------
void CSaxErrorHandlerXerces::AttachElementHandler(IWXmlElementHandler *pWElementHandler)
{
 m_attachWElementHandler = pWElementHandler;
 m_attachElementHandler  = nullptr;
}

// -----------------------------------------------------------------------------
void CSaxErrorHandlerXerces::DetachElementHandler()
{
 m_attachElementHandler  = nullptr;
 m_attachWElementHandler = nullptr;
}

// -----------------------------------------------------------------------------
void CSaxErrorHandlerXerces::ReportError(
    const SAXParseException& exc)
{
 // Get the line and column where the error occurred.
 int  line   = exc.getLineNumber();
 int  column = exc.getColumnNumber();
 unsigned long errCode = -1;

 const wchar_t* pwchErrorMessage = exc.getMessage();

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