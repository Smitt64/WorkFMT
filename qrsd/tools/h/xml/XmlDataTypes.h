// Filename: XmlDataTypes.h
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Parameterized data types in order to support
//                   single char or wide char strings.

#ifndef XML_DATA_TYPES_INCLUDED
#define XML_DATA_TYPES_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

// CBasicXmlAttribute
// Represents an XML attribute (a name-value pair), zero or more
// of which may appear in an XML start element. This is a 
// parameterized class, the type T should be char or wchar_t.
template<class T>
class CBasicXmlAttribute
{
public:
    // Constructors / destructor.
    CBasicXmlAttribute() {}
    CBasicXmlAttribute(const std::basic_string<T>& name, const std::basic_string<T>& value)
        : m_name(name), m_value(value) {}
    ~CBasicXmlAttribute() {}
    
    // Get/set the attribute name.
    const std::basic_string<T>& GetName() const       { return m_name; }
    void  SetName(const std::basic_string<T>& name)   { m_name = name; }
    
    // Get/set the attribute value.
    const std::basic_string<T>& GetValue() const      { return m_value; }
    void  SetValue(const std::basic_string<T>& value) { m_value = value; }
    
    // Get/set both the attribute name and value.
    void  Get(std::basic_string<T>& name, std::basic_string<T>& value) const;
    void  Set(const std::basic_string<T>& name, const std::basic_string<T>& value);

    // Return a displayable string representing this attribute.
    std::basic_string<T> ToString() const;
    
    // Static helper methods for conversion from SAX2 strings.
    static void SetAttribute(const wchar_t* pwchAttributeName, wchar_t nameLen,
                             const wchar_t* pwchAttributeValue, wchar_t valueLen,
                             CBasicXmlAttribute<char>& attribute);
    static void SetAttribute(const wchar_t* pwchAttributeName, wchar_t nameLen,
                             const wchar_t* pwchAttributeValue, wchar_t valueLen,
                             CBasicXmlAttribute<wchar_t>& attribute);

private:
    std::basic_string<T> m_name;
    std::basic_string<T> m_value;
};

// Typedefs for template specializations.
// Client code uses these typedefs instead of
// using CBasicXmlAttribute directly.
typedef CBasicXmlAttribute<char>    CXmlAttribute;
typedef CBasicXmlAttribute<wchar_t> CWXmlAttribute;

// CBasicXmlElement
// Represents an XML start or end element. Contains all
// of its attributes as well (if any). This is a 
// parameterized class, the type T should be char or wchar_t.
template<class T>
class CBasicXmlElement
{
public:
    // Constructor / destructor.
    CBasicXmlElement()
        : m_isStartElement(true), m_depth(0) {}
    ~CBasicXmlElement() {}

    // Is this a start element?
    bool IsStartElement() const               { return m_isStartElement; }
    void SetStartElement(bool isStartElement) { m_isStartElement = isStartElement; }

    // Get/set the element name.
    const std::basic_string<T>& GetElementName() const           { return m_elementName; }
    void SetElementName(const std::basic_string<T>& elementName) { m_elementName = elementName; }

    // Static helper methods for mapping a SAX2 text string to STL string.
    // Useful for mapping text strings such as the XML element name.
    static void MapToString(const wchar_t* pwchText, int textLen, std::string& text);
    static void MapToString(const wchar_t* pwchText, int textLen, std::wstring& text);

    // Add a new attribute to this element.
    void AddAttribute(const CBasicXmlAttribute<T>& attribute);
    void AddAttribute(const std::basic_string<T>& attributeName, const std::basic_string<T>& attributeValue);

    // Get/set the value of an existing attribute.
    std::basic_string<T> GetAttributeValue(const std::basic_string<T>& attributeName) const;
    void SetAttributeValue(const std::basic_string<T>& attributeName, const std::basic_string<T>& attributeValue);

    // Get/set the element's nesting level/depth.
    int  GetDepth() const    { return m_depth; }
    void SetDepth(int depth) { m_depth = depth; }

    // Read-only access to attributes.
    const std::vector<CBasicXmlAttribute<T> >& GetAttributes() const { return m_attributes; }

    // Return a displayable string representing this element.
    std::basic_string<T> ToString() const;

private:
    bool                 m_isStartElement;
    std::basic_string<T> m_elementName;
    int                  m_depth;

    std::vector<CBasicXmlAttribute<T> > m_attributes;
};

// Typedefs for template specializations.
// Client code uses these typedefs instead of
// using CBasicXmlElement directly.
typedef CBasicXmlElement<char>    CXmlElement;
typedef CBasicXmlElement<wchar_t> CWXmlElement;

// Include the CPP source.
#include "XmlDataTypes.inl"

#endif // XML_DATA_TYPES_INCLUDED

// END
