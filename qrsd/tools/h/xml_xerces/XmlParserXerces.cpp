#include "XmlParserXerces.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>

#include <xercesc/sax2/XMLReaderFactory.hpp>
//#include <xercesc/sax2/SAXParseException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/framework/URLInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>



#include <iostream>


XERCES_CPP_NAMESPACE_USE

std::weak_ptr<int>  CXmlParserXerces::XMLPlatformUtils_glob_auto_destroy_;

CXmlParserXerces::CXmlParserXerces()
  : parser_(nullptr)
  , m_contentHandler(nullptr)
  , m_errorHandler(nullptr)
  , m_attachElementHandler(nullptr)
  , m_attachWElementHandler(nullptr)
{
  if( XMLPlatformUtils_glob_auto_destroy_.expired() ) {
    XMLPlatformUtils::Initialize();
    XMLPlatformUtils_glob_auto_destroy_ = XMLPlatformUtils_auto_destroy_ = std::shared_ptr<int>(nullptr, [](int * val){ 
      XMLPlatformUtils::Terminate();
    });
  } else {
    XMLPlatformUtils_auto_destroy_ = XMLPlatformUtils_glob_auto_destroy_.lock();
  }

  parser_ = XMLReaderFactory::createXMLReader();

  m_contentHandler = new CSaxContentHandlerXerces;

  parser_->setContentHandler(m_contentHandler);

  m_errorHandler = new CSaxErrorHandlerXerces;

  parser_->setErrorHandler(m_errorHandler);

  parser_->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);

}

CXmlParserXerces::~CXmlParserXerces(){
  DetachElementHandler();

  if(parser_ != nullptr) {
    delete parser_;
    parser_ = nullptr;
  }

  delete m_errorHandler;
  m_errorHandler = nullptr;

  delete m_contentHandler;
  m_contentHandler = nullptr;

}

bool CXmlParserXerces::IsReady() const {
  return parser_ != nullptr;
}

void CXmlParserXerces::AttachElementHandler(IXmlElementHandler *pElementHandler) {
  m_attachElementHandler = pElementHandler;
  if(m_contentHandler != nullptr)
    m_contentHandler->AttachElementHandler(pElementHandler);

  if(m_errorHandler != nullptr)
    m_errorHandler->AttachElementHandler(pElementHandler);
}

void CXmlParserXerces::AttachElementHandler(IWXmlElementHandler *pWElementHandler) {
  m_attachWElementHandler = pWElementHandler;
  if(m_contentHandler != nullptr)
    m_contentHandler->AttachElementHandler(pWElementHandler);

  if(m_errorHandler != nullptr)
    m_errorHandler->AttachElementHandler(pWElementHandler);
}

void CXmlParserXerces::DetachElementHandler() {
  if(m_errorHandler != nullptr)
    m_errorHandler->DetachElementHandler();

  if(m_contentHandler != nullptr)
    m_contentHandler->DetachElementHandler();

  m_attachElementHandler  = nullptr;
  m_attachWElementHandler = nullptr;
}

// Set parser feature options.
bool CXmlParserXerces::SetParserFeature(const std::wstring &featureName, bool value) {
  if(featureName.empty())
  return false;

  if(!IsReady())
    return false;

  //VARIANT_BOOL  vfValue = (value ? VARIANT_TRUE : VARIANT_FALSE);
  if( featureName.compare(L"schema-validation") == 0){
    parser_->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, value);
    parser_->setFeature(XMLUni::fgSAX2CoreValidation, value);

  } else if( featureName.compare(L"use-schema-location") == 0){
    use_schema_location_ = value;
  } else
    return false;

  return true;
}

bool CXmlParserXerces::GetParserFeature(const std::wstring &featureName, bool &value) const {
  if( featureName.compare(L"schema-validation") == 0){
    return parser_->getFeature(XMLUni::fgSAX2CoreValidation);
  } else if( featureName.compare(L"use-schema-location") == 0){
    return use_schema_location_;
  } else
    return false;
}

// Add/remove XSD schemas for validation.
bool CXmlParserXerces::AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath) {

  xsdPath_ = namespaceURI + L" " + xsdPath;
  //parser_->setProperty(xercesc::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, (void*)xsdPath.c_str()); не подходит без пространства имен.
  parser_->setProperty(xercesc::XMLUni::fgXercesSchemaExternalSchemaLocation\
    , (void*)xsdPath_.c_str() );

  return true;
}

bool CXmlParserXerces::RemoveValidationSchema(const std::wstring &namespaceURI) {
  parser_->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, false);
  parser_->setFeature(XMLUni::fgSAX2CoreValidation, false);
  return true;
}

// Parse a local file path, or a HTTP URL path.
bool CXmlParserXerces::Parse(const std::wstring &xmlPath) {
  if(!IsReady())
  return false;

  bool ret = true;
  //char* my_xmlPath = XMLString::transcode(xmlPath.c_str());
  try {
    parser_->parse( xmlPath.c_str() );
  } catch( SAXException & e ){
    ret = false;
  }
  //XMLString::release(&my_xmlPath);

  //if(FAILED(hr))
  //  return false;

  return ret;
}

bool CXmlParserXerces::ParseString(const std::wstring &xml) {
   if(!IsReady())
   return false;
 
 std::unique_ptr<MemBufInputSource> myxml_buf(new MemBufInputSource((XMLByte*)xml.c_str(), xml.size() * sizeof(std::wstring::value_type), L"XML"));
 myxml_buf->setEncoding( XMLUni::fgUTF16EncodingString );
 parser_->parse( *myxml_buf );

 //if(FAILED(hr))
 //  return false;

 return true;
}
