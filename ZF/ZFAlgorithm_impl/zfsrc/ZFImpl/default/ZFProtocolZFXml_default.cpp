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
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        ZFBuffer buf;
        buf.bufferCopy(src, (size != zfindexMax() ? size : zfslen(src)) * sizeof(zfchar));
        return this->xmlParse(buf, errorHint);
    }
    virtual ZFXml xmlParse(
            ZF_IN const ZFInput &inputCallback
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        ZFBuffer buf;
        ZFInputRead(buf, inputCallback);
        return this->xmlParse(buf, errorHint);
    }
private:
    ZFXml xmlParse(
            ZF_IN_OUT ZFBuffer &buf
            , ZF_OUT zfstring *errorHint
            ) {
        if(buf.buffer() == zfnull) {
            return zfnull;
        }
        pugi::xml_document implXmlDoc;
        pugi::xml_parse_result implResult = implXmlDoc.load_buffer_inplace(buf.buffer(), buf.bufferSize(), pugi::parse_full);
        if(implResult.status != pugi::status_ok) {
            if(errorHint) {
                *errorHint += implResult.description();
                zfindex offset = (zfindex)implResult.offset;
                if(offset >= 0 && offset < buf.bufferSize()) {
                    *errorHint += ", at: ";
                    zfindex len = buf.bufferSize() - offset;
                    zfstring tmp;
                    if(len >= 128) {
                        tmp.append(buf.bufferT<zfchar *>() + offset, 32);
                        tmp += "...";
                    }
                    else {
                        tmp += buf.bufferT<zfchar *>() + offset;
                    }
                    zfstringReplace(tmp, "\n", "\\n");
                    *errorHint += tmp;
                }
            }
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
            zfXmlItem.attr(implXmlAttribute.name(), implXmlAttribute.value());
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

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pcdata: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlText);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_cdata: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlText);
                    zfXmlChild.CDATA(zftrue);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_comment: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlComment);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_declaration: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlDeclaration);
                    this->translateAttribute(implXmlChild, zfXmlChild);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_doctype: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlDocType);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pi: {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlPI);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
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

ZF_NAMESPACE_GLOBAL_END

