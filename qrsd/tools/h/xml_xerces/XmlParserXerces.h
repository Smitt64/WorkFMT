#ifndef XMLPARSERXERCES_H
#define XMLPARSERXERCES_H

#include "XmlParserBase.h"

#include "SaxContentHandlerXerces.h"
#include "SaxErrorHandlerXerces.h"

#include <xercesc/sax2/SAX2XMLReader.hpp>

//#include <boost/shared_ptr.hpp>
//#include <boost/weak_ptr.hpp>

class CXmlParserXerces : public CXmlParserBase
{
 public:
    CXmlParserXerces();
   ~CXmlParserXerces();

    // Is the parser available (e.g. was the COM class created properly?).
    bool IsReady() const override;

    // Attach XML events handler.
    void AttachElementHandler(IXmlElementHandler *pElementHandler) override;
    void AttachElementHandler(IWXmlElementHandler *pWElementHandler) override;
    void DetachElementHandler() override;

    // Set parser feature options.
    bool SetParserFeature(const std::wstring &featureName, bool value) override;
    bool GetParserFeature(const std::wstring &featureName, bool &value) const override;

    // Add/remove XSD schemas for validation.
    bool AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath) override;
    bool RemoveValidationSchema(const std::wstring &namespaceURI) override;

    // Parse a local file path, or a HTTP URL path.
    bool Parse(const std::wstring &xmlPath) override;
    bool ParseString(const std::wstring &xml) override;

 private:
    void CreateSaxReader();
    void CreateSchemaCache();

    // SAX reader.
    xercesc::SAX2XMLReader* parser_;

    // Content and error handlers to be used with the SAX reader.
    CSaxContentHandlerXerces *m_contentHandler; //CSaxContentHandler        *m_contentHandler;
    CSaxErrorHandlerXerces *m_errorHandler; //CSaxErrorHandler          *m_errorHandler;

    // Schema cache for XSD validation.
    //IXMLDOMSchemaCollection2  *m_schemaCache;

    // Our own XML events handler.
    IXmlElementHandler        *m_attachElementHandler;
    IWXmlElementHandler       *m_attachWElementHandler;

    std::wstring xsdPath_;
    bool use_schema_location_;

    std::shared_ptr<int>  XMLPlatformUtils_auto_destroy_;
    static std::weak_ptr<int>  XMLPlatformUtils_glob_auto_destroy_;
};

#endif