#include "XmlParserMsxml.h"

CXmlParserImpl::CXmlParserImpl(int empty)
{

}

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