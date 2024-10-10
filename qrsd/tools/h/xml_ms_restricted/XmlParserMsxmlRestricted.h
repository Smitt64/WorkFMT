#ifndef XMLPARSERMSXMLRESTRICTED_H
#define XMLPARSERMSXMLRESTRICTED_H

//#include "XmlParserBase.h"
//
//#include "XmlCommon.h"
//#include "SaxContentHandler.h"
//#include "SaxErrorHandler.h"

#include "xmlsup\XmlParserMsxml.h" 

#include <map>

// CXmlParserImpl //////////////////////////////////////////////////////

class CXmlParserRestrictedImpl : public CXmlParserImpl
{
 public:
    CXmlParserRestrictedImpl();
    virtual ~CXmlParserRestrictedImpl();


    // Set parser feature options.
    bool SetParserFeature(const std::wstring &featureName, bool value) override;
    bool GetParserFeature(const std::wstring &featureName, bool &value) const override;

    // Add/remove XSD schemas for validation.
    bool AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath) override;
    bool RemoveValidationSchema(const std::wstring &namespaceURI) override;


 protected:

  std::map<std::wstring, bool> mlast_feature_;
};

#endif