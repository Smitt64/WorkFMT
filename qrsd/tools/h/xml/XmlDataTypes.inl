// Filename: XmlDataTypes.cpp
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Parameterized data types in order to support
//                   single char or wide char strings.

#ifndef XML_DATA_TYPES_CPP_INCLUDED
#define XML_DATA_TYPES_CPP_INCLUDED

#include "minmax.h"

#define XDT_MAX_TEXT_SIZE            200
#define XDT_MAX_ATTRIBUTE_NAME_SIZE  200
#define XDT_MAX_ATTRIBUTE_VALUE_SIZE 400
#define XDT_MAX_BLANKS               800

// CBasicXmlAttribute ////////////////////////////////////////////////////////

template<class T>
void CBasicXmlAttribute<T>::Get(std::basic_string<T>& name, std::basic_string<T>& value) const
{
    name = m_name;
    value = m_value;
}

template<class T>
void CBasicXmlAttribute<T>::Set(const std::basic_string<T>& name, const std::basic_string<T>& value)
{
    m_name = name;
    m_value = value;
}

template<class T>
std::basic_string<T> CBasicXmlAttribute<T>::ToString() const
{
    T buf[3];
    buf[0] = '=';
    buf[1] = '\"';
    buf[2] = 0;
    std::basic_string<T> result = m_name + std::basic_string<T>(buf) + m_value;
    result += std::basic_string<T>(buf+1);
    
    return result;
}

template<class T>
void CBasicXmlAttribute<T>::SetAttribute(
    const wchar_t* pwchAttributeName, wchar_t nameLen,
    const wchar_t* pwchAttributeValue, wchar_t valueLen,
    CBasicXmlAttribute<char>& attribute)
{
    if ( pwchAttributeName == NULL || pwchAttributeValue == NULL ||
         nameLen <= 0 || valueLen <= 0 )
        return;

    char szAttributeName[XDT_MAX_ATTRIBUTE_NAME_SIZE];
    int len = min(nameLen, XDT_MAX_ATTRIBUTE_NAME_SIZE-1);
    wcstombs(szAttributeName, pwchAttributeName, len);
    szAttributeName[len] = 0;

    char szAttributeValue[XDT_MAX_ATTRIBUTE_VALUE_SIZE];
    len = min(valueLen, XDT_MAX_ATTRIBUTE_VALUE_SIZE-1);
    wcstombs(szAttributeValue, pwchAttributeValue, len);
    szAttributeValue[len] = 0;

    attribute.Set(szAttributeName, szAttributeValue);
}

template<class T>
void CBasicXmlAttribute<T>::SetAttribute(
    const wchar_t* pwchAttributeName, wchar_t nameLen,
    const wchar_t* pwchAttributeValue, wchar_t valueLen,
    CBasicXmlAttribute<wchar_t>& attribute)
{
    if ( pwchAttributeName == NULL || pwchAttributeValue == NULL ||
         nameLen <= 0 || valueLen <= 0 )
        return;

    wchar_t wszAttributeName[XDT_MAX_ATTRIBUTE_NAME_SIZE];
    int len = min(nameLen, XDT_MAX_ATTRIBUTE_NAME_SIZE-1);
    wcsncpy(wszAttributeName, pwchAttributeName, len);
    wszAttributeName[len] = 0;

    wchar_t wszAttributeValue[XDT_MAX_ATTRIBUTE_VALUE_SIZE];
    len = min(valueLen, XDT_MAX_ATTRIBUTE_VALUE_SIZE-1);
    wcsncpy(wszAttributeValue, pwchAttributeValue, len);
    wszAttributeValue[len] = 0;

    attribute.Set(wszAttributeName, wszAttributeValue);
}

// CBasicXmlElement //////////////////////////////////////////////////////////

template<class T>
void CBasicXmlElement<T>::MapToString(const wchar_t* pwchText, int textLen, std::string& text)
{
    if ( pwchText == NULL || textLen <= 0 )
        return;

    char szText[XDT_MAX_TEXT_SIZE];
    int len = min(textLen, XDT_MAX_TEXT_SIZE-1);
    wcstombs(szText, pwchText, len);
    szText[len] = 0;

    text = std::string(szText);
}

template<class T>
void CBasicXmlElement<T>::MapToString(const wchar_t* pwchText, int textLen, std::wstring& text)
{
    if ( pwchText == NULL || textLen <= 0 )
        return;

    wchar_t wszText[XDT_MAX_TEXT_SIZE];
    int len = min(textLen, XDT_MAX_TEXT_SIZE-1);
    wcsncpy(wszText, pwchText, len);
    wszText[len] = 0;

    text = std::wstring(wszText);
}

template<class T>
void CBasicXmlElement<T>::AddAttribute(const CBasicXmlAttribute<T>& attribute)
{
    // Don't allow empty attribute name.
    if ( attribute.GetName().empty() )
        return;

    // For efficiency, we don't check for duplicates.
    m_attributes.push_back(attribute);
}

template<class T>
void CBasicXmlElement<T>::AddAttribute(const std::basic_string<T>& attributeName, const std::basic_string<T>& attributeValue)
{
    AddAttribute(CBasicXmlAttribute<T>(attributeName, attributeValue));
}

template<class T>
std::basic_string<T> CBasicXmlElement<T>::GetAttributeValue(const std::basic_string<T>& attributeName) const
{
    std::basic_string<T> emptyString;

    if ( attributeName.empty() )
        return emptyString;

    for(int i = 0; i < (int)m_attributes.size(); ++i)
    {
        const CBasicXmlAttribute<T>& attr = m_attributes[i];
        if ( attr.GetName() == attributeName )
            return attr.GetValue();
    }

    return emptyString;
}

template<class T>
void CBasicXmlElement<T>::SetAttributeValue(const std::basic_string<T>& attributeName, const std::basic_string<T>& attributeValue)
{
    if ( attributeName.empty() )
        return;

    for(int i = 0; i < (int)m_attributes.size(); ++i)
    {
        CBasicXmlAttribute<T>& attr = m_attributes[i];
        if ( attr.GetName() == attributeName )
        {
            // We only set the first occurrence.
            attr.SetValue(attributeValue);
            break;
        }
    }
}

template<class T>
std::basic_string<T> CBasicXmlElement<T>::ToString() const
{
    // Determine amount of leading whitespace. For each
    // depth level, we indent 4 blankspaces.
    T szBlanks[XDT_MAX_BLANKS];
    int numIndents = m_depth;
    if ( IsStartElement() )
        numIndents -= 1;
    int numBlanks = min(XDT_MAX_BLANKS-1, 4 * numIndents);
    numBlanks = max(0, numBlanks);
    for(int i = 0; i < numBlanks; ++i)
        szBlanks[i] = ' ';
    szBlanks[numBlanks] = 0;
    std::basic_string<T> result = std::basic_string<T>(szBlanks);

    // Construct the left bracket.
    T szLeft[3];
    szLeft[0] = '<';
    if ( IsStartElement() )
    {
        szLeft[1] = 0;
    }
    else
    {
        szLeft[1] = '/';
        szLeft[2] = 0;
    }
    result += std::basic_string<T>(szLeft);

    // Append element name.
    result += m_elementName;

    // Append attributes.
    T szSpace[2];
    szSpace[0] = ' ';
    szSpace[1] = 0;
    for(int j = 0; j < (int)m_attributes.size(); ++j)
    {
        result += std::basic_string<T>(szSpace) + m_attributes[j].ToString();
    }

    // Append right bracket.
    T szRight[2];
    szRight[0] = '>';
    szRight[1] = 0;
    result += std::basic_string<T>(szRight);

    return result;
}

#endif // XML_DATA_TYPES_CPP_INCLUDED

// END

