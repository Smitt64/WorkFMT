// Filename: XmlParser.cpp
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added CWXmlParser wrapper class for wide-char string support.

#include "xmlsup/XmlCommon.h"
#include "xml/XmlParser.h"

#include "rsfname.h"

#include "xmlsup/XmlParserMsxml.h"
#include "xml_ms_restricted/XmlParserMsxmlRestricted.h"
#include "xml_xerces/XmlParserXerces.h"

// -----------------------------------------------------------------------------
using namespace std;

// -----------------------------------------------------------------------------
#define  MAX_XML_PATH       1024
#define  MAX_XSD_PATH       1000
#define  MAX_FEATURE_NAME    200
#define  MAX_NAMESPACE_URI  1000

// dbg
//void ReadConfig( char* fname, int(fn)(char *, char *, void *), void * data){
//  fn("XML_PARSER_TYPE", "MSXML_RESTRICTED_FOR_WINE", data);
//}

enum EXmlParserType {
  e_empty = 0,
  e_msxml,
  e_msxml_restricted,
  e_xerces
};

static int readRsreq(char *key, char *name, void *data)
{
  size_t *parser_id = (size_t*)data;
  if(strcmp(key, "XML_PARSER_TYPE") == 0)
  {
    if(strcmp(name, "MSXML") == 0)
      *parser_id = (size_t)e_msxml;
    else if(strcmp(name, "MSXML_RESTRICTED_FOR_WINE") == 0)
      *parser_id = (size_t)e_msxml_restricted;
    else if(strcmp(name, "XERCES") == 0)
      *parser_id = (size_t)e_xerces;
  }
 return 0;
}



CXmlParserFactory::CXmlParserFactory() 
  : parser_id_(e_empty)
  , next_parser_(nullptr)
{
   ReadConfig("rsreq.ini", readRsreq, &parser_id_);
}

CXmlParserFactory::~CXmlParserFactory() {
  if(next_parser_ != nullptr)
    delete next_parser_;
}

CXmlParserBase* CXmlParserFactory::makeParser(){
  if(next_parser_ == nullptr)
    next_parser_ = makeParserImpl();
  
  CXmlParserBase* ret = next_parser_;
  next_parser_ = makeParserImpl();
  return ret;
}

CXmlParserBase* CXmlParserFactory::makeParserImpl(){
  CXmlParserBase* ret;
  switch(parser_id_){
  case e_msxml: 
    ret = new CXmlParserImpl;
    break;
  case e_msxml_restricted:
    ret = new CXmlParserRestrictedImpl;
    break;
  case e_xerces:
    ret = new CXmlParserXerces;
    break;
  default:
    ret = new CXmlParserImpl;
  }
  return ret;
}



// CXmlParser //////////////////////////////////////////////////////////

CXmlParser::CXmlParser()
{
  m_impl = CXmlParserFactory::inst().makeParser();
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
  m_impl = CXmlParserFactory::inst().makeParser();
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