#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFXml.h"

#include "../_repo/pugixml/pugixml.hpp"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFXmlImpl_default_MemoryPoolHolder
{
public:
    /*
     * as for pugixml, string values are directly stored in owner document's buffer,
     * store ref count, init as 0, each child node refering to the doc would inc the ref count
     */
    zfindex docRefCount;
    ZFBuffer buf;
    pugi::xml_document implXmlDoc;
public:
    _ZFP_ZFXmlImpl_default_MemoryPoolHolder(void)
    : docRefCount(0)
    , buf()
    , implXmlDoc()
    {
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFXmlImpl_default, ZFXml, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("pugixml")
public:
    virtual ZFXml xmlParse(ZF_IN const zfchar *src,
                           ZF_IN_OPT zfindex size = zfindexMax())
    {
        ZFBuffer buf;
        buf.bufferCopy(src, (size != zfindexMax() ? size : zfslen(src)) * sizeof(zfchar));
        return this->xmlParse(buf);
    }
    virtual ZFXml xmlParse(ZF_IN const ZFInput &inputCallback)
    {
        ZFBuffer buf;
        ZFInputRead(buf, inputCallback);
        return this->xmlParse(buf);
    }
    virtual void xmlMemoryPoolRelease(ZF_IN void *token, ZF_IN const zfchar *value)
    {
        _ZFP_ZFXmlImpl_default_MemoryPoolHolder *docHolder = (_ZFP_ZFXmlImpl_default_MemoryPoolHolder *)token;
        --(docHolder->docRefCount);
        if(docHolder->docRefCount == 0)
        {
            zfdelete(docHolder);
        }
    }
private:
    ZFXml xmlParse(ZF_IN_OUT ZFBuffer &buf)
    {
        if(buf.buffer() == zfnull)
        {
            return ZFXml();
        }
        _ZFP_ZFXmlImpl_default_MemoryPoolHolder *docHolder = zfnew(_ZFP_ZFXmlImpl_default_MemoryPoolHolder);
        docHolder->buf = buf;
        pugi::xml_parse_result implResult = docHolder->implXmlDoc.load_buffer_inplace(buf.buffer(), buf.bufferSize(), pugi::parse_full);
        if(implResult.status != pugi::status_ok)
        {
            zfdelete(docHolder);
            return ZFXml();
        }
        ZFXml doc(ZFXmlType::e_XmlDocument);
        this->translateChildren(docHolder->implXmlDoc, doc, docHolder);
        if(docHolder->docRefCount == 0)
        {
            zfdelete(docHolder);
        }
        return doc;
    }
    void translateAttribute(ZF_IN const pugi::xml_node &implXmlItem,
                            ZF_OUT ZFXml &zfXmlItem,
                            ZF_IN _ZFP_ZFXmlImpl_default_MemoryPoolHolder *docHolder)
    {
        pugi::xml_attribute implXmlAttribute = implXmlItem.first_attribute();
        while(implXmlAttribute)
        {
            ZFXml zfXmlAttribute(ZFXmlType::e_XmlAttribute);
            ++(docHolder->docRefCount);
            this->xmlMemoryPool_xmlName(zfXmlAttribute, implXmlAttribute.name(), docHolder);
            ++(docHolder->docRefCount);
            this->xmlMemoryPool_xmlValue(zfXmlAttribute, implXmlAttribute.value(), docHolder);
            zfXmlItem.attrAdd(zfXmlAttribute);

            implXmlAttribute = implXmlAttribute.next_attribute();
        }
    }
    void translateChildren(ZF_IN const pugi::xml_node &implXmlItem,
                           ZF_OUT ZFXml &zfXmlItem,
                           ZF_IN _ZFP_ZFXmlImpl_default_MemoryPoolHolder *docHolder)
    {
        pugi::xml_node implXmlChild = implXmlItem.first_child();
        while(implXmlChild != zfnull)
        {
            switch(implXmlChild.type())
            {
                case pugi::node_document:
                    zfCoreCriticalShouldNotGoHere();
                    return;
                case pugi::node_element:
                {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlElement);
                    this->translateAttribute(implXmlChild, zfXmlChild, docHolder);
                    this->translateChildren(implXmlChild, zfXmlChild, docHolder);

                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlName(zfXmlChild, implXmlChild.name(), docHolder);
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pcdata:
                {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlText);

                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlName(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlValue(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_cdata:
                {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlText);
                    zfXmlChild.xmlTextCDATA(zftrue);

                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlName(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlValue(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_comment:
                {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlComment);

                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlName(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlValue(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_declaration:
                {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlDeclaration);
                    this->translateAttribute(implXmlChild, zfXmlChild, docHolder);

                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlName(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlValue(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_doctype:
                {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlDocType);

                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlName(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlValue(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.childAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pi:
                {
                    ZFXml zfXmlChild(ZFXmlType::e_XmlPI);

                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlName(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->docRefCount);
                    this->xmlMemoryPool_xmlValue(zfXmlChild, implXmlChild.value(), docHolder);
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

