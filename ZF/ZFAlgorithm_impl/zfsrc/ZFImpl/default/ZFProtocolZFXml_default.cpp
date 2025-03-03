#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFXml.h"

#include "../_repo/pugixml/pugixml.hpp"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFXmlImpl_default, ZFXml, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("pugixml")
public:
    virtual ZFXml xmlParse(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex size = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        zfstring buf;
        buf.assign(src, (size != zfindexMax() ? size : zfslen(src)));
        return this->xmlParse(buf, errorHint);
    }
    virtual ZFXml xmlParse(
            ZF_IN const ZFInput &inputCallback
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        zfstring buf;
        ZFInputRead(buf, inputCallback);
        return this->xmlParse(buf, errorHint);
    }
private:
    ZFXml xmlParse(
            ZF_IN_OUT zfstring &buf
            , ZF_OUT zfstring *errorHint
            ) {
        if(buf.buffer() == zfnull) {
            return zfnull;
        }
        pugi::xml_document implXmlDoc;
        pugi::xml_parse_result implResult = implXmlDoc.load_buffer_inplace(buf.zfunsafe_buffer(), buf.length(), pugi::parse_full);
        if(implResult.status != pugi::status_ok) {
            if(errorHint) {
                *errorHint += implResult.description();
                zfindex offset = (zfindex)implResult.offset;
                if(offset >= 0 && offset < buf.length()) {
                    *errorHint += ", at: ";
                    zfindex len = buf.length() - offset;
                    zfstring tmp;
                    if(len >= 128) {
                        tmp.append(buf + offset, 32);
                        tmp += "...";
                    }
                    else {
                        tmp += buf + offset;
                    }
                    *errorHint += zfstringReplace(tmp, "\n", "\\n");
                }
            }
            return zfnull;
        }
        ZFXml doc(v_ZFXmlType::e_Document);
        this->translateChildren(implXmlDoc, doc);
        return doc;
    }
    void translateAttr(
            ZF_IN const pugi::xml_node &implXmlItem
            , ZF_OUT ZFXml &zfXmlItem
            ) {
        pugi::xml_attribute implAttr = implXmlItem.first_attribute();
        while(implAttr) {
            zfXmlItem.attr(implAttr.name(), implAttr.value());
            implAttr = implAttr.next_attribute();
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
                    ZFCoreCriticalShouldNotGoHere();
                    return;
                case pugi::node_element: {
                    ZFXml zfXmlChild(v_ZFXmlType::e_Element);
                    this->translateAttr(implXmlChild, zfXmlChild);
                    this->translateChildren(implXmlChild, zfXmlChild);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlItem.child(zfXmlChild);
                    break;
                }
                case pugi::node_pcdata: {
                    ZFXml zfXmlChild(v_ZFXmlType::e_Text);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.child(zfXmlChild);
                    break;
                }
                case pugi::node_cdata: {
                    ZFXml zfXmlChild(v_ZFXmlType::e_Text);
                    zfXmlChild.CDATA(zftrue);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.child(zfXmlChild);
                    break;
                }
                case pugi::node_comment: {
                    ZFXml zfXmlChild(v_ZFXmlType::e_Comment);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.child(zfXmlChild);
                    break;
                }
                case pugi::node_declaration: {
                    ZFXml zfXmlChild(v_ZFXmlType::e_Declaration);
                    this->translateAttr(implXmlChild, zfXmlChild);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.child(zfXmlChild);
                    break;
                }
                case pugi::node_doctype: {
                    ZFXml zfXmlChild(v_ZFXmlType::e_DocType);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.child(zfXmlChild);
                    break;
                }
                case pugi::node_pi: {
                    ZFXml zfXmlChild(v_ZFXmlType::e_PI);

                    zfXmlChild.name(implXmlChild.name());
                    zfXmlChild.value(implXmlChild.value());
                    zfXmlItem.child(zfXmlChild);
                    break;
                }
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return;
            }

            implXmlChild = implXmlChild.next_sibling();
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFXmlImpl_default)

ZF_NAMESPACE_GLOBAL_END

