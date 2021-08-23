// Filename: XmlParser.cpp
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added CWXmlParser wrapper class for wide-char string support.

#include "XmlCommon.h"
#include "XmlParser.h"
#include "SaxContentHandler.h"
#include "SaxErrorHandler.h"

#include <msxml2.h>

// -----------------------------------------------------------------------------
using namespace std;

// -----------------------------------------------------------------------------
#define  MAX_XML_PATH       1024
#define  MAX_XSD_PATH       1000
#define  MAX_FEATURE_NAME    200
#define  MAX_NAMESPACE_URI  1000

// CXmlParserImpl //////////////////////////////////////////////////////

class CXmlParserImpl
{
 public:
    CXmlParserImpl();
   ~CXmlParserImpl();

    // Is the parser available (e.g. was the COM class created properly?).
    bool IsReady() const;

    // Attach XML events handler.
    void AttachElementHandler(IXmlElementHandler *pElementHandler);
    void AttachElementHandler(IWXmlElementHandler *pWElementHandler);
    void DetachElementHandler();

    // Set parser feature options.
    bool SetParserFeature(const std::wstring &featureName, bool value);
    bool GetParserFeature(const std::wstring &featureName, bool &value) const;

    // Add/remove XSD schemas for validation.
    bool AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath);
    bool RemoveValidationSchema(const std::wstring &namespaceURI);

    // Parse a local file path, or a HTTP URL path.
    bool Parse(const std::wstring &xmlPath);
    bool ParseString(const std::wstring &xml);

 private:
    void CreateSaxReader();
    void CreateSchemaCache();

    // SAX reader.
    ISAXXMLReader             *m_reader;

    // Content and error handlers to be used with the SAX reader.
    CSaxContentHandler        *m_contentHandler;
    CSaxErrorHandler          *m_errorHandler;

    // Schema cache for XSD validation.
    IXMLDOMSchemaCollection2  *m_schemaCache;

    // Our own XML events handler.
    IXmlElementHandler        *m_attachElementHandler;
    IWXmlElementHandler       *m_attachWElementHandler;
};

// -----------------------------------------------------------------------------
CXmlParserImpl::CXmlParserImpl()
{
 m_reader = NULL;

 m_contentHandler = NULL;
 m_errorHandler   = NULL;

 m_schemaCache = NULL;

 m_attachElementHandler  = NULL;
 m_attachWElementHandler = NULL;

 // Initialize COM.
 CoInitialize(NULL);

 // Create the SAX XML reader (COM class).
 CreateSaxReader();

 // Create the schema cache (COM class) for XSD validation.
 CreateSchemaCache();
}

// -----------------------------------------------------------------------------
CXmlParserImpl::~CXmlParserImpl()
{
 DetachElementHandler();

 // Release the SAX XML reader object.
 if(m_reader != NULL)
  {
   m_reader->Release();
   m_reader = NULL;
  }

 // Release the schema cache object.
 if(m_schemaCache != NULL)
  {
   m_schemaCache->Release();
   m_schemaCache = NULL;
  }

 delete m_errorHandler;
 m_errorHandler = NULL;

 delete m_contentHandler;
 m_contentHandler = NULL;

 // Shutdown COM.
 CoUninitialize();
}

// -----------------------------------------------------------------------------
bool CXmlParserImpl::IsReady() const
{
 return (m_reader != NULL);
}

// -----------------------------------------------------------------------------
void CXmlParserImpl::AttachElementHandler(IXmlElementHandler *pElementHandler)
{
 m_attachElementHandler = pElementHandler;

 if(m_contentHandler != NULL)
   m_contentHandler->AttachElementHandler(pElementHandler);

 if(m_errorHandler != NULL)
   m_errorHandler->AttachElementHandler(pElementHandler);
}

// -----------------------------------------------------------------------------
void CXmlParserImpl::AttachElementHandler(IWXmlElementHandler *pWElementHandler)
{
 m_attachWElementHandler = pWElementHandler;

 if(m_contentHandler != NULL)
   m_contentHandler->AttachElementHandler(pWElementHandler);

 if(m_errorHandler != NULL)
   m_errorHandler->AttachElementHandler(pWElementHandler);
}

// -----------------------------------------------------------------------------
void CXmlParserImpl::DetachElementHandler()
{
 if(m_errorHandler != NULL)
   m_errorHandler->DetachElementHandler();

 if(m_contentHandler != NULL)
   m_contentHandler->DetachElementHandler();

 m_attachElementHandler  = NULL;
 m_attachWElementHandler = NULL;
}

// -----------------------------------------------------------------------------
bool CXmlParserImpl::SetParserFeature(const std::wstring &featureName, bool value)
{
 if(featureName.empty())
   return false;

 if(!IsReady())
   return false;

 VARIANT_BOOL  vfValue = (value ? VARIANT_TRUE : VARIANT_FALSE);
 //_bstr_t bstrFeature = featureName.c_str();

 HRESULT       hr     = m_reader->putFeature(featureName.c_str() /*bstrFeature*/, vfValue);

 if(FAILED(hr))
   return false;

 return true;
}

// -----------------------------------------------------------------------------
bool CXmlParserImpl::GetParserFeature(const std::wstring &featureName, bool &value) const
{
 if(featureName.empty())
   return false;

 if(!IsReady())
   return false;

 VARIANT_BOOL  vfValue = VARIANT_FALSE;
 //_bstr_t bstrFeature = featureName.c_str();
 HRESULT       hr      = m_reader->getFeature(featureName.c_str()/* bstrFeature*/, &vfValue);

 if(FAILED(hr))
   return false;

 value = (vfValue == VARIANT_TRUE ? true : false);

 return true;
}

// -----------------------------------------------------------------------------
bool CXmlParserImpl::AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath)
{
 if(!IsReady())
   return false;

 if(m_schemaCache == NULL)
   return false;

 // Check for existing schema associated with this namespace URI.
 ISchema   *pExistingSchema = NULL;
 //_bstr_t bstrNamespace = namespaceURI.c_str();
 CComBSTR   bstrNamespace(namespaceURI.c_str());
 HRESULT    hr = m_schemaCache->getSchema(bstrNamespace, &pExistingSchema);

 if(SUCCEEDED(hr))
  {
   // Remove the existing schema.
   hr = m_schemaCache->remove(bstrNamespace);

   if(FAILED(hr))
     return false;
  }

 // Add the new schema.
 hr = m_schemaCache->add(bstrNamespace, CComVariant(xsdPath.c_str()) /*_variant_t(xsdPath.c_str())*/);

 if(FAILED(hr))
   return false;

 return true;
}

// -----------------------------------------------------------------------------
bool CXmlParserImpl::RemoveValidationSchema(const std::wstring &namespaceURI)
{
 if(!IsReady())
   return false;

 if(m_schemaCache == NULL)
   return false;

 // Check for existing schema associated with this namespace URI.
 ISchema   *pExistingSchema = NULL;
 //_bstr_t bstrNamespace = namespaceURI.c_str();
 CComBSTR   bstrNamespace(namespaceURI.c_str());
 HRESULT    hr = m_schemaCache->getSchema(bstrNamespace, &pExistingSchema);

 if(SUCCEEDED(hr))
  {
   // Remove the existing schema.
   hr = m_schemaCache->remove(bstrNamespace);

   if(FAILED(hr))
     return false;
  }
 else
  {
   // Return false if there is no schema to remove.
   return false;
  }

 return true;
}

// -----------------------------------------------------------------------------
bool CXmlParserImpl::ParseString(const std::wstring &xml)
{
 if(!IsReady())
   return false;

 // Do the parse.
// _bstr_t bstrXmlPath = xmlPath.c_str();
 HRESULT  hr = m_reader->parse(CComVariant(xml.c_str()) /*bstrXmlPath*/);

 if(FAILED(hr))
   return false;

 return true;
}

// -----------------------------------------------------------------------------
bool CXmlParserImpl::Parse(const std::wstring &xmlPath)
{
 if(!IsReady())
   return false;

 // Do the parse.
// _bstr_t bstrXmlPath = xmlPath.c_str();
 HRESULT  hr = m_reader->parseURL(xmlPath.c_str() /*bstrXmlPath*/);

 if(FAILED(hr))
   return false;

 return true;
}

// -----------------------------------------------------------------------------
void CXmlParserImpl::CreateSaxReader()
{
 // А на MSVS 2017 наоборот, не поддерживается ниже SAXXMLReader60
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
 HRESULT  hr = -1;
#else
 HRESULT  hr = CoCreateInstance(__uuidof(SAXXMLReader40), NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (void **)&m_reader);
#endif


 // #2912
 // Некрасиво, но MSVS 2005 не поддерживает предопределённые идентификаторы выше SAXXMLReader40
 if(FAILED(hr))
  {
   CLSID  cls60;

#ifdef SAXXMLReader60
   cls60 = __uuidof(SAXXMLReader60);
#else
   hr = CLSIDFromString(L"Msxml2.SAXXMLReader.6.0", &cls60);

   if(SUCCEEDED(hr))
#endif
     hr = CoCreateInstance(cls60, NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (void **)&m_reader);
  }

 if(SUCCEEDED(hr))
  {
   // Set the content handler.
   m_contentHandler = new CSaxContentHandler;

   hr = m_reader->putContentHandler(m_contentHandler);

   if(FAILED(hr))
    {
     delete m_contentHandler;
     m_contentHandler = NULL;
    }

   // Set the error handler.
   m_errorHandler = new CSaxErrorHandler;

   hr = m_reader->putErrorHandler(m_errorHandler);

   if(FAILED(hr))
    {
     delete m_errorHandler;
     m_errorHandler = NULL;
    }
  }
 else
  {
   delete m_errorHandler;
   m_errorHandler = NULL;

   delete m_contentHandler;
   m_contentHandler = NULL;

   m_reader = NULL;
  }
}

// -----------------------------------------------------------------------------
void CXmlParserImpl::CreateSchemaCache()
{
 if(m_reader == NULL)
   return;


 // А на MSVS 2017 наоборот, не поддерживается ниже SAXXMLReader60
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
 HRESULT  hr = -1;
#else
 HRESULT  hr = CoCreateInstance(__uuidof(XMLSchemaCache40), NULL, CLSCTX_ALL, __uuidof(IXMLDOMSchemaCollection2), (void **)&m_schemaCache);
#endif


 // #2912
 // Некрасиво, но MSVS 2005 не поддерживает предопределённые идентификаторы выше SAXXMLReader40
 if(FAILED(hr))
  {
   CLSID  cls60;

#ifdef XMLSchemaCache60
   cls60 = __uuidof(XMLSchemaCache60);
#else
   hr = CLSIDFromString(L"Msxml2.XMLSchemaCache.6.0", &cls60);

   if(SUCCEEDED(hr))
#endif
     hr = CoCreateInstance(cls60, NULL, CLSCTX_ALL, __uuidof(IXMLDOMSchemaCollection2), (void **)&m_schemaCache);
  }

 if(SUCCEEDED(hr))
  {
   // Set the "schemas" property in the reader in order
   // to associate the schema cache with the reader.
   hr = m_reader->putProperty(L"schemas", CComVariant(m_schemaCache));

   if(FAILED(hr))
    {
     OutputDebugString("CXmlParserImpl::CreateSchemaCache(): putProperty(L\"schemas\",...) failed\n");
    }
  }
}

// CXmlParser //////////////////////////////////////////////////////////

CXmlParser::CXmlParser()
{
 m_impl = new CXmlParserImpl;
}

// -----------------------------------------------------------------------------
CXmlParser::~CXmlParser()
{
 delete m_impl;
}

// -----------------------------------------------------------------------------
bool CXmlParser::IsReady() const
{
 return m_impl->IsReady();
}

// -----------------------------------------------------------------------------
void CXmlParser::AttachElementHandler(IXmlElementHandler *pElementHandler)
{
 m_impl->AttachElementHandler(pElementHandler);
}

// -----------------------------------------------------------------------------
void CXmlParser::DetachElementHandler()
{
 m_impl->DetachElementHandler();
}

// -----------------------------------------------------------------------------
bool CXmlParser::SetParserFeature(const std::string &featureName, bool value)
{
 if(featureName.empty())
   return false;

 wchar_t  wszFeatureName[MAX_FEATURE_NAME];


 wszFeatureName[0] = 0;
 mbstowcs(wszFeatureName, featureName.c_str(), MAX_FEATURE_NAME - 1);
 wszFeatureName[MAX_FEATURE_NAME - 1] = 0;

 bool  result = m_impl->SetParserFeature(wszFeatureName, value);

 return result;
}

// -----------------------------------------------------------------------------
bool CXmlParser::GetParserFeature(const std::string &featureName, bool &value) const
{
 if(featureName.empty())
   return false;

 wchar_t  wszFeatureName[MAX_FEATURE_NAME];

 wszFeatureName[0] = 0;
 mbstowcs(wszFeatureName, featureName.c_str(), MAX_FEATURE_NAME - 1);
 wszFeatureName[MAX_FEATURE_NAME - 1] = 0;

 bool  result = m_impl->GetParserFeature(wszFeatureName, value);

 return result;
}

// -----------------------------------------------------------------------------
bool CXmlParser::AddValidationSchema(const std::string &namespaceURI, const std::string &xsdPath)
{
 wchar_t  wszNamespaceURI[MAX_NAMESPACE_URI];


 wszNamespaceURI[0] = 0;
 mbstowcs(wszNamespaceURI, namespaceURI.c_str(), MAX_NAMESPACE_URI - 1);
 wszNamespaceURI[MAX_NAMESPACE_URI - 1] = 0;

 wchar_t  wszXsdPath[MAX_XSD_PATH];


 wszXsdPath[0] = 0;
 mbstowcs(wszXsdPath, xsdPath.c_str(), MAX_XSD_PATH - 1);
 wszXsdPath[MAX_XSD_PATH - 1] = 0;

 bool  result = m_impl->AddValidationSchema(wszNamespaceURI, wszXsdPath);

 return result;
}

// -----------------------------------------------------------------------------
bool CXmlParser::RemoveValidationSchema(const std::string &namespaceURI)
{
 wchar_t  wszNamespaceURI[MAX_NAMESPACE_URI];


 wszNamespaceURI[0] = 0;
 mbstowcs(wszNamespaceURI, namespaceURI.c_str(), MAX_NAMESPACE_URI - 1);
 wszNamespaceURI[MAX_NAMESPACE_URI - 1] = 0;

 bool  result = m_impl->RemoveValidationSchema(wszNamespaceURI);

 return result;
}

// -----------------------------------------------------------------------------
bool CXmlParser::Parse(const std::string &xmlPath)
{
 wchar_t  wszXmlPath[MAX_XML_PATH];


 wszXmlPath[0] = 0;
 mbstowcs(wszXmlPath, xmlPath.c_str(), MAX_XML_PATH - 1);
 wszXmlPath[MAX_XML_PATH - 1] = 0;

 bool  result = m_impl->Parse(wszXmlPath);

 return result;
}

// -----------------------------------------------------------------------------
bool CXmlParser::ParseString(const std::string& xmlPath)
{
//AV 11.03.2011 - MAX_XML_PATH (1024) мало для текста в CLOB-полях журналов RS-Bank,
//  да и теоретически размер текста может быть любым, поэтому переписал на динамическое выделение памяти
/*
    wchar_t wszXmlPath[MAX_XML_PATH];
    wszXmlPath[0] = 0;
    mbstowcs(wszXmlPath, xmlPath.c_str(), MAX_XML_PATH-1);
    wszXmlPath[MAX_XML_PATH-1] = 0;

    bool result = m_impl->ParseString(wszXmlPath);

    return result;
*/
 int   len_path = (int)xmlPath.length();
 bool  result = true;


 if(len_path > 0)
  {
   int       wlen_path  = (len_path + 1)*sizeof(wchar_t);
   wchar_t  *wszXmlPath = (wchar_t *)operator new(wlen_path);

   if(wszXmlPath)
    {
     //AV 07.04.2011 в качестве размера нужно передавать количество символов len_path
     int  res_len = (int)mbstowcs(wszXmlPath, xmlPath.c_str(), len_path);


     if(res_len >= 0)
      {
       wszXmlPath[res_len] = 0; //AV 07.04.2011 \0 ставим именно в res_len, потому что столько символов обработала mbstowcs
       result = m_impl->ParseString(wszXmlPath);
      }

     operator delete(wszXmlPath);
    }
  }

 return result;
}

// CWXmlParser //////////////////////////////////////////////////////////

CWXmlParser::CWXmlParser()
{
 m_impl = new CXmlParserImpl;
}

// -----------------------------------------------------------------------------
CWXmlParser::~CWXmlParser()
{
 delete m_impl;
}

// -----------------------------------------------------------------------------
bool CWXmlParser::IsReady() const
{
 return m_impl->IsReady();
}

// -----------------------------------------------------------------------------
void CWXmlParser::AttachElementHandler(IWXmlElementHandler *pWElementHandler)
{
 m_impl->AttachElementHandler(pWElementHandler);
}

// -----------------------------------------------------------------------------
void CWXmlParser::DetachElementHandler()
{
 m_impl->DetachElementHandler();
}

// -----------------------------------------------------------------------------
bool CWXmlParser::SetParserFeature(const std::wstring &featureName, bool value)
{
 return m_impl->SetParserFeature(featureName, value);
}

// -----------------------------------------------------------------------------
bool CWXmlParser::GetParserFeature(const std::wstring &featureName, bool &value) const
{
 return m_impl->GetParserFeature(featureName, value);
}

// -----------------------------------------------------------------------------
bool CWXmlParser::AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath)
{
 return m_impl->AddValidationSchema(namespaceURI, xsdPath);
}

// -----------------------------------------------------------------------------
bool CWXmlParser::RemoveValidationSchema(const std::wstring &namespaceURI)
{
 return m_impl->RemoveValidationSchema(namespaceURI);
}

// -----------------------------------------------------------------------------
bool CWXmlParser::Parse(const std::wstring &xmlPath)
{
 return m_impl->Parse(xmlPath);
}

// -----------------------------------------------------------------------------
bool CWXmlParser::ParseString(const std::wstring &xmlPath)
{
 return m_impl->ParseString(xmlPath);
}

/* <-- EOF --> */