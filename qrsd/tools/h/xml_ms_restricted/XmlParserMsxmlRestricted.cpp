#include "XmlParserMsxmlRestricted.h"

// -----------------------------------------------------------------------------
CXmlParserRestrictedImpl::CXmlParserRestrictedImpl()
  : CXmlParserImpl(0)
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

}

// -----------------------------------------------------------------------------
CXmlParserRestrictedImpl::~CXmlParserRestrictedImpl()
{

}

// -----------------------------------------------------------------------------
bool CXmlParserRestrictedImpl::SetParserFeature(const std::wstring &featureName, bool value)
{
 if(featureName.empty())
   return false;

 mlast_feature_[featureName] = value;

 //if(!IsReady())
 //  return false;

 //VARIANT_BOOL  vfValue = (value ? VARIANT_TRUE : VARIANT_FALSE);
 ////_bstr_t bstrFeature = featureName.c_str();

 //HRESULT       hr     = m_reader->putFeature(featureName.c_str() /*bstrFeature*/, vfValue);

 //if(FAILED(hr))
 //  return false;

 return true;
}

// -----------------------------------------------------------------------------
bool CXmlParserRestrictedImpl::GetParserFeature(const std::wstring &featureName, bool &value) const
{
 if(featureName.empty())
   return false;

 auto ifeature = mlast_feature_.find(featureName);
 if(ifeature == mlast_feature_.end())
   return false;
 else
   return ifeature->second;

 //if(!IsReady())
 //  return false;

 //VARIANT_BOOL  vfValue = VARIANT_FALSE;
 ////_bstr_t bstrFeature = featureName.c_str();
 //HRESULT       hr      = m_reader->getFeature(featureName.c_str()/* bstrFeature*/, &vfValue);

 //if(FAILED(hr))
 //  return false;

 //value = (vfValue == VARIANT_TRUE ? true : false);

 return true;
}

// -----------------------------------------------------------------------------
bool CXmlParserRestrictedImpl::AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath)
{
 if(!IsReady())
   return false;

 if(m_schemaCache == NULL)
   return false;

 //// Check for existing schema associated with this namespace URI.
 //ISchema   *pExistingSchema = NULL;
 ////_bstr_t bstrNamespace = namespaceURI.c_str();
 //CComBSTR   bstrNamespace(namespaceURI.c_str());
 //HRESULT    hr = m_schemaCache->getSchema(bstrNamespace, &pExistingSchema);

 //if(SUCCEEDED(hr))
 // {
 //  // Remove the existing schema.
 //  hr = m_schemaCache->remove(bstrNamespace);

 //  if(FAILED(hr))
 //    return false;
 // }

 //// Add the new schema.
 //hr = m_schemaCache->add(bstrNamespace, CComVariant(xsdPath.c_str()) /*_variant_t(xsdPath.c_str())*/);

 //if(FAILED(hr))
 //  return false;

 return true;
}

// -----------------------------------------------------------------------------
bool CXmlParserRestrictedImpl::RemoveValidationSchema(const std::wstring &namespaceURI)
{
 if(!IsReady())
   return false;

 if(m_schemaCache == NULL)
   return false;

 //// Check for existing schema associated with this namespace URI.
 //ISchema   *pExistingSchema = NULL;
 ////_bstr_t bstrNamespace = namespaceURI.c_str();
 //CComBSTR   bstrNamespace(namespaceURI.c_str());
 //HRESULT    hr = m_schemaCache->getSchema(bstrNamespace, &pExistingSchema);

 //if(SUCCEEDED(hr))
 // {
 //  // Remove the existing schema.
 //  hr = m_schemaCache->remove(bstrNamespace);

 //  if(FAILED(hr))
 //    return false;
 // }
 //else
 // {
 //  // Return false if there is no schema to remove.
 //  return false;
 // }

 return true;
}
