#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFXml.h"

#include "../_repo/pugixml/pugixml.hpp"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFXmlImpl_default, ZFXml, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("pugixml")
public:
    virtual ZFXml xmlParse(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex size = zfindexMax()
            ) {
        ZFBuffer buf;
        buf.bufferCopy(src, (size != zfindexMax() ? size : zfslen(src)) * sizeof(zfchar));
        return this->xmlParse(buf);
    }
    virtual ZFXml xmlParse(ZF_IN const ZFInput &inputCallback) {
        ZFBuffer buf;
        ZFInputRead(buf, inputCallback);
        return this->xmlParse(buf);
    }
private:
    ZFXml xmlParse(ZF_IN_OUT ZFBuffer &buf) {
        if(buf.buffer() == zfnull) {
            return zfnull;
        }
        pugi::xml_document implXmlDoc;
        pugi::xml_parse_result implResult = implXmlDoc.load_buffer_inplace(buf.buffer(), buf.bufferSize(), pugi::parse_full);
        if(implResult.status != pugi::status_ok) {
            return zfnull;
        }
        ZFXml doc(ZFXmlType::e_XmlDocument);
        this->translateChildren(implXmlDoc, doc);
        return doc;
    }
    void translateAttribute(
            ZF_IN const pugi::xml_node &implXmlItem
            , ZF_OUT ZFXml &zfXmlItem
            ) {
        pugi::xml_attribute implXmlAttribute = implXmlItem.first_attribute();
        while(implXmlAttribute) {
            ZFXml zfXmlAttribute(ZFXmlType::e_XmlAttribute);
            zfXmlAttribute.xmlName(implXmlAttribute.name());
            zfXmlAttribute.xmlValue(implXmlAttribute.value());
            zfXmlItem.attrAdd(zfXmlAttribute);

            implXmlAttribute = implXmlAttribute.next_attribute();
        }
    }
    void translateChildren(
            ZF_IN const pugi::xml_node &implXmlItem
            , ZF_OUT ZFXml &zfXmlItem
            ) {
        pugi::xml_node implXmlChild = implXmlItem.first_child();
        while(implXmlChild != zfnull) {
            switch(implXmlChild.type()) {
                case pugi::node_document:
                    zfCoreCriticalShouldNotGoHere();
                    return;
                case pugi::node_element: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlElement);
                    this->translateAttribute(implXmlChild, zfXmlChild);
                    this->translateChildren(implXmlChild, zfXmlChild);

                    zfXmlChild.xmlName(implXmlChild.name());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pcdata: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlText);

                    zfXmlChild.xmlName(implXmlChild.name());
                    zfXmlChild.xmlValue(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_cdata: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlText);
                    zfXmlChild.xmlTextCDATA(zftrue);

                    zfXmlChild.xmlName(implXmlChild.name());
                    zfXmlChild.xmlValue(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_comment: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlComment);

                    zfXmlChild.xmlName(implXmlChild.name());
                    zfXmlChild.xmlValue(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_declaration: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlDeclaration);
                    this->translateAttribute(implXmlChild, zfXmlChild);

                    zfXmlChild.xmlName(implXmlChild.name());
                    zfXmlChild.xmlValue(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_doctype: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlDocType);

                    zfXmlChild.xmlName(implXmlChild.name());
                    zfXmlChild.xmlValue(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pi: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlPI);

                    zfXmlChild.xmlName(implXmlChild.name());
                    zfXmlChild.xmlValue(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return;
            }

            implXmlChild = implXmlChild.next_sibling();
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFXmlImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFXmlImpl_default)

ZF_NAMESPACE_GLOBAL_END

