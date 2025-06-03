#include "ZFXml.h"
#include "protocol/ZFProtocolZFXml.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#include "ZFCore/ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFXmlType)

// ============================================================
zfbool ZFXmlOutputToken::operator == (ZF_IN ZFXmlOutputToken const &ref) const {
    return (zftrue
            && this->xmlNewLineToken == ref.xmlNewLineToken
            && this->xmlIndentToken == ref.xmlIndentToken
            && this->xmlDeclarationTagLeft == ref.xmlDeclarationTagLeft
            && this->xmlDeclarationTagRight == ref.xmlDeclarationTagRight
            && this->xmlDocTypeTagLeft == ref.xmlDocTypeTagLeft
            && this->xmlDocTypeTagRight == ref.xmlDocTypeTagRight
            && this->xmlPITagLeft == ref.xmlPITagLeft
            && this->xmlPITagRight == ref.xmlPITagRight
            && this->xmlElementBeginTagLeft == ref.xmlElementBeginTagLeft
            && this->xmlElementBeginTagRight == ref.xmlElementBeginTagRight
            && this->xmlElementEndTagLeft == ref.xmlElementEndTagLeft
            && this->xmlElementEndTagRight == ref.xmlElementEndTagRight
            && this->xmlElementSingleTagLeft == ref.xmlElementSingleTagLeft
            && this->xmlElementSingleTagRight == ref.xmlElementSingleTagRight
            && this->xmlAttrEqualTag == ref.xmlAttrEqualTag
            && this->xmlAttrQuoteTagLeft == ref.xmlAttrQuoteTagLeft
            && this->xmlAttrQuoteTagRight == ref.xmlAttrQuoteTagRight
            && this->xmlTextCDATATagLeft == ref.xmlTextCDATATagLeft
            && this->xmlTextCDATATagRight == ref.xmlTextCDATATagRight
            && this->xmlCommentTagLeft == ref.xmlCommentTagLeft
            && this->xmlCommentTagRight == ref.xmlCommentTagRight

            && this->xmlGlobalLineBeginToken == ref.xmlGlobalLineBeginToken
            && this->xmlElementAttrCountBeforeAddNewLine == ref.xmlElementAttrCountBeforeAddNewLine
            && this->xmlElementAddNewLineAtHeadIfNotSingleLine == ref.xmlElementAddNewLineAtHeadIfNotSingleLine
            && this->xmlElementTrimTagIfNoChildren == ref.xmlElementTrimTagIfNoChildren
            && this->xmlElementEndTagAtSameLineIfNoChildElement == ref.xmlElementEndTagAtSameLineIfNoChildElement
        );
}
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFXmlOutputToken, ZFXmlOutputToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlNewLineToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlIndentToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlDeclarationTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlDeclarationTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlDocTypeTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlDocTypeTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlPITagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlPITagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlElementBeginTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlElementBeginTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlElementEndTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlElementEndTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlElementSingleTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlElementSingleTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlAttrEqualTag)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlAttrQuoteTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlAttrQuoteTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlTextCDATATagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlTextCDATATagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlCommentTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlCommentTagRight)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfstring, xmlGlobalLineBeginToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfindex, xmlElementAttrCountBeforeAddNewLine)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfbool, xmlElementAddNewLineAtHeadIfNotSingleLine)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfbool, xmlElementTrimTagIfNoChildren)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFXmlOutputToken, zfbool, xmlElementEndTagAtSameLineIfNoChildElement)

// ============================================================
static const ZFXmlOutputToken &_ZFP_ZFXmlOutputTokenTrimInit(void) {
    static ZFXmlOutputToken d;
    d.xmlNewLineToken = zfnull;
    d.xmlIndentToken = zfnull;
    d.xmlElementAttrCountBeforeAddNewLine = zfindexMax();
    d.xmlElementAddNewLineAtHeadIfNotSingleLine = zffalse;
    d.xmlElementTrimTagIfNoChildren = zftrue;
    d.xmlElementEndTagAtSameLineIfNoChildElement = zftrue;
    return d;
}
ZFEXPORT_VAR_DEFINE(ZFXmlOutputToken, ZFXmlOutputTokenTrim, _ZFP_ZFXmlOutputTokenTrimInit())

static const ZFXmlOutputToken &_ZFP_ZFXmlOutputTokenDetailInit(void) {
    static ZFXmlOutputToken d;
    d.xmlElementAttrCountBeforeAddNewLine = 1;
    d.xmlElementAddNewLineAtHeadIfNotSingleLine = zftrue;
    d.xmlElementTrimTagIfNoChildren = zftrue;
    d.xmlElementEndTagAtSameLineIfNoChildElement = zftrue;
    return d;
}
ZFEXPORT_VAR_DEFINE(ZFXmlOutputToken, ZFXmlOutputTokenDetail, _ZFP_ZFXmlOutputTokenDetailInit())

ZFEXPORT_VAR_DEFINE(ZFXmlOutputToken, ZFXmlOutputTokenDefault, ZFXmlOutputTokenTrim())

// ============================================================
/*
 * Null
 * Element : attr, child, name
 * Text : value
 * Comment : value
 * Document : child
 * Declaration : attr
 * DocType : value
 * PI : name, value
 */
zfclassNotPOD _ZFP_ZFXmlPrivate {
public:
    typedef zfimplhashmap<zfstring, zfstring> AttrMap;
    typedef zfstldeque<ZFXml> ChildList;
public:
    zfuint refCount;
    ZFXmlType type;
    zfstring name;
    zfstring value;
    AttrMap attrMap;
    ChildList childList;
    zfbool CDATA;

public:
    explicit _ZFP_ZFXmlPrivate(ZF_IN ZFXmlType type)
    : refCount(1)
    , type(type)
    , name(zfnull)
    , value(zfnull)
    , attrMap()
    , childList()
    , CDATA(zffalse)
    {
    }
};

#define _ZFP_ZFXmlAssertCanHaveAttr(item) \
    ZFCoreAssert((item).type() == v_ZFXmlType::e_Element || (item).type() == v_ZFXmlType::e_Declaration)
#define _ZFP_ZFXmlAssertCanHaveChild(item) \
    ZFCoreAssert((item).type() == v_ZFXmlType::e_Element || (item).type() == v_ZFXmlType::e_Document)

// ============================================================
// ZFXml
ZFXml::ZFXml(ZF_IN _ZFP_ZFXmlPrivate *ref)
: d(ref)
{
    if(d) {
        ++(d->refCount);
    }
}

ZFXml::ZFXml(void)
: d(zfnew(_ZFP_ZFXmlPrivate, v_ZFXmlType::e_Element))
{
}
ZFXml::ZFXml(ZF_IN const zfnullT &dummy)
: d(zfnull)
{
}
ZFXml::ZFXml(ZF_IN ZFXmlType type) {
    if(type != v_ZFXmlType::e_Null) {
        d = zfnew(_ZFP_ZFXmlPrivate, type);
    }
    else {
        d = zfnull;
    }
}
ZFXml::ZFXml(ZF_IN const ZFXml &ref)
: d(ref.d)
{
    if(d) {
        ++(d->refCount);
    }
}
ZFXml::~ZFXml(void) {
    if(d) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
        }
    }
}

ZFXml &ZFXml::operator = (ZF_IN const ZFXml &ref) {
    _ZFP_ZFXmlPrivate *dTmp = d;
    d = ref.d;
    if(d) {
        ++(d->refCount);
    }
    if(dTmp) {
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfdelete(dTmp);
        }
    }
    return *this;
}
ZFXml &ZFXml::operator = (ZF_IN const zfnullT &dummy) {
    if(d) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
        }
        d = zfnull;
    }
    return *this;
}
zfbool ZFXml::operator == (ZF_IN const ZFXml &ref) const {
    return (d == ref.d);
}

// ============================================================
void ZFXml::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ZFXmlToStringT(ret, *this, ZFXmlOutputTokenDefault());
}

zfindex ZFXml::objectRetainCount(void) const {
    return d ? d->refCount : 0;
}

// ============================================================
void ZFXml::_ZFP_ZFXml_xmlType(ZF_IN ZFXmlType type) {
    if(type != v_ZFXmlType::e_Null) {
        if(d) {
            d->type = type;
        }
        else {
            d = zfnew(_ZFP_ZFXmlPrivate, type);
        }
    }
}
ZFXmlType ZFXml::type(void) const {
    return d ? d->type : v_ZFXmlType::e_Null;
}

ZFXml &ZFXml::name(ZF_IN const zfstring &name) {
    switch(this->type()) {
        case v_ZFXmlType::e_PI:
        case v_ZFXmlType::e_Element:
            d->name = name;
            break;
        default:
            break;
    }
    return *this;
}
const zfstring &ZFXml::name(void) const {
    return d ? d->name : zfstring::Empty();
}

ZFXml &ZFXml::value(ZF_IN const zfstring &value) {
    switch(this->type()) {
        case v_ZFXmlType::e_DocType:
        case v_ZFXmlType::e_PI:
        case v_ZFXmlType::e_Text:
        case v_ZFXmlType::e_Comment:
            d->value = value;
            break;
        default:
            break;
    }
    return *this;
}
const zfstring &ZFXml::value(void) const {
    return d ? d->value : zfstring::Empty();
}

// ============================================================
ZFXml ZFXml::copy(void) const {
    ZFXml ret(this->type());
    switch(this->type()) {
        case v_ZFXmlType::e_Null:
            break;
        case v_ZFXmlType::e_Element:
            ret.name(this->name());
            for(zfiter it = this->attrIter(); it; ++it) {
                ret.attr(this->attrIterKey(it), this->attrIterValue(it));
            }
            for(zfindex i = 0; i < this->childCount(); ++i) {
                ret.child(this->childAt(i).copy());
            }
            break;
        case v_ZFXmlType::e_Text:
            ret.value(this->value());
            ret.CDATA(this->CDATA());
            break;
        case v_ZFXmlType::e_Comment:
            ret.value(this->value());
            break;
        case v_ZFXmlType::e_Document:
            for(zfindex i = 0; i < this->childCount(); ++i) {
                ret.child(this->childAt(i).copy());
            }
            break;
        case v_ZFXmlType::e_Declaration:
            for(zfiter it = this->attrIter(); it; ++it) {
                ret.attr(this->attrIterKey(it), this->attrIterValue(it));
            }
            break;
        case v_ZFXmlType::e_DocType:
            ret.value(this->value());
            break;
        case v_ZFXmlType::e_PI:
            ret.name(this->name());
            ret.value(this->value());
            break;
        default:
            break;
    }
    return ret;
}

// ============================================================
zfindex ZFXml::attrCount(void) const {
    return (zfindex)(d ? d->attrMap.size() : 0);
}
ZFXml &ZFXml::attr(
        ZF_IN const zfstring &key
        , ZF_IN const zfstring &value
        ) {
    if(key) {
        _ZFP_ZFXmlAssertCanHaveAttr(*this);
        if(value == zfnull) {
            d->attrMap.erase(key);
        }
        else {
            d->attrMap[key] = value;
        }
    }
    return *this;
}
zfstring ZFXml::attr(ZF_IN const zfstring &key) const {
    if(d && key) {
        _ZFP_ZFXmlPrivate::AttrMap::iterator it = d->attrMap.find(key);
        if(it != d->attrMap.end()) {
            return it->second;
        }
    }
    return zfnull;
}
zfbool ZFXml::attrExist(ZF_IN const zfstring &key) const {
    return d && key && d->attrMap.find(key) != d->attrMap.end();
}
ZFXml &ZFXml::attrRemove(ZF_IN const zfstring &key /* = zfnull */) {
    if(d && key) {
        d->attrMap.erase(key);
    }
    return *this;
}
ZFXml &ZFXml::attrRemoveAll(void) {
    if(d) {
        d->attrMap.clear();
    }
    return *this;
}

zfiter ZFXml::attrIter(void) const {
    return d ? d->attrMap.iter() : zfnull;
}
zfiter ZFXml::attrIterFind(ZF_IN const zfstring &key) const {
    return d ? d->attrMap.iterFind(key) : zfnull;
}
zfstring ZFXml::attrIterKey(ZF_IN const zfiter &it) const {
    return d ? d->attrMap.iterKey(it) : zfnull;
}
zfstring ZFXml::attrIterValue(ZF_IN const zfiter &it) const {
    return d ? d->attrMap.iterValue(it) : zfnull;
}

void ZFXml::attrIterValue(
        ZF_IN_OUT zfiter &it
        , ZF_IN const zfstring &value
        ) {
    if(d) {
        d->attrMap.iterValue(it, value);
    }
}
void ZFXml::attrIterRemove(ZF_IN_OUT zfiter &it) {
    if(d) {
        d->attrMap.iterRemove(it);
    }
}

// ============================================================
zfindex ZFXml::childCount(void) const {
    return (zfindex)(d ? d->childList.size() : 0);
}
ZFXml ZFXml::childAt(ZF_IN zfindex index) const {
    if(d && index < d->childList.size()) {
        return d->childList[index];
    }
    else {
        return zfnull;
    }
}
ZFXml &ZFXml::child(
        ZF_IN const ZFXml &item
        , ZF_IN_OPT zfindex index /* = zfindexMax() */
        ) {
    _ZFP_ZFXmlAssertCanHaveChild(*this);
    switch(item.type()) {
        case v_ZFXmlType::e_Null:
        case v_ZFXmlType::e_Document:
            ZFCoreCriticalMessage("%s can not be added to %s"
                    , item.type()
                    , this->type()
                );
            break;
        case v_ZFXmlType::e_Declaration:
        case v_ZFXmlType::e_DocType:
        case v_ZFXmlType::e_PI:
            if(this->type() != v_ZFXmlType::e_Document) {
                ZFCoreCriticalMessage("%s can not be added to %s"
                        , item.type()
                        , this->type()
                    );
            }
            break;
        default:
            break;
    }
    if(index == zfindexMax()) {
        index = this->childCount();
    }
    else {
        ZFCoreAssertIndexRange(index, (zfindex)(d->childList.size() + 1));
    }
    ZFCoreAssertWithMessage(item, "add null object");
    d->childList.insert(d->childList.begin() + index, item);
    return *this;
}
ZFXml &ZFXml::childRemoveAt(ZF_IN zfindex index) {
    if(index < this->childCount()) {
        d->childList.erase(d->childList.begin() + index);
    }
    return *this;
}
ZFXml &ZFXml::childRemoveAll(void) {
    if(d) {
        d->childList.clear();
    }
    return *this;
}
zfindex ZFXml::childFind(ZF_IN const ZFXml &item) const {
    if(d) {
        for(zfstlsize i = 0; i < d->childList.size(); ++i) {
            if(d->childList[i] == item) {
                return (zfindex)i;
            }
        }
    }
    return zfindexMax();
}

ZFXml ZFXml::childElement(void) const {
    for(zfindex i = 0; i < this->childCount(); ++i) {
        ZFXml child = this->childAt(i);
        if(child.type() == v_ZFXmlType::e_Element) {
            return child;
        }
    }
    return zfnull;
}

ZFXml ZFXml::childElementAt(ZF_IN zfindex index) const {
    ZFXml child = this->childAt(index);
    if(child.type() == v_ZFXmlType::e_Element) {
        return child;
    }
    else {
        return zfnull;
    }
}

// ============================================================
void ZFXml::CDATA(ZF_IN zfbool CDATA) {
    ZFCoreAssert(this->type() == v_ZFXmlType::e_Text);
    d->CDATA = CDATA;
}
zfbool ZFXml::CDATA(void) const {
    return d ? d->CDATA : zffalse;
}

// ============================================================
zfstring ZFXml::toString(ZF_IN_OPT const ZFXmlOutputToken &token /* = ZFXmlOutputTokenDefault() */) const {
    if(this->valid()) {
        return ZFXmlToString(*this, token);
    }
    else {
        return zfnull;
    }
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFXml, ZFXml, {
        v = ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(src, srcLen, errorHint);
        return v;
    }, {
        return ZFXmlToStringT(s, v, ZFXmlOutputTokenDefault());
    })

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        v_ZFXml *v = invokerObject;
        v->zfv._ZFP_ZFXml_xmlType(type);
    }, v_ZFXml
    , ZFMP_IN(ZFXmlType, type)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, void, objectInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, ZFXmlType, type)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, ZFXml &, name
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, const zfstring &, name)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, ZFXml &, value
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, const zfstring &, value)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, ZFXml, copy)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, zfindex, attrCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXml, ZFXml &, attr
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, zfstring, attr
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, zfbool, attrExist
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, ZFXml &, attrRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, ZFXml &, attrRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, zfiter, attrIter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, zfiter, attrIterFind
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, zfstring, attrIterKey
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, zfstring, attrIterValue
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXml, void, attrIterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, void, attrIterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, zfindex, childCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, ZFXml, childAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXml, ZFXml &, child
        , ZFMP_IN(const ZFXml &, item)
        , ZFMP_IN_OPT(zfindex, index, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, ZFXml &, childRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, ZFXml &, childRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, zfindex, childFind
        , ZFMP_IN(const ZFXml &, item)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, ZFXml, childElement)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, ZFXml, childElementAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, void, CDATA
        , ZFMP_IN(zfbool, CDATA)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXml, zfbool, CDATA)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXml, zfstring, toString
        , ZFMP_IN_OPT(const ZFXmlOutputToken &, token, ZFXmlOutputTokenDefault())
        )

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFXml, ZFXmlElement
        , ZFMP_IN(const zfstring &, name)
        ) {
    return ZFXml(v_ZFXmlType::e_Element).name(name);
}
ZFMETHOD_FUNC_DEFINE_1(ZFXml, ZFXmlText
        , ZFMP_IN(const zfstring &, value)
        ) {
    return ZFXml(v_ZFXmlType::e_Text).value(value);
}
ZFMETHOD_FUNC_DEFINE_1(ZFXml, ZFXmlComment
        , ZFMP_IN(const zfstring &, value)
        ) {
    return ZFXml(v_ZFXmlType::e_Comment).value(value);
}
ZFMETHOD_FUNC_DEFINE_0(ZFXml, ZFXmlDocument) {
    return ZFXml(v_ZFXmlType::e_Document);
}
ZFMETHOD_FUNC_DEFINE_0(ZFXml, ZFXmlDeclaration) {
    return ZFXml(v_ZFXmlType::e_Declaration);
}
ZFMETHOD_FUNC_DEFINE_1(ZFXml, ZFXmlDocType
        , ZFMP_IN(const zfstring &, value)
        ) {
    return ZFXml(v_ZFXmlType::e_DocType).value(value);
}
ZFMETHOD_FUNC_DEFINE_2(ZFXml, ZFXmlPI
        , ZFMP_IN(const zfstring &, name)
        , ZFMP_IN(const zfstring &, value)
        ) {
    return ZFXml(v_ZFXmlType::e_PI).name(name).value(value);
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFXml, ZFXmlFromInput
        , ZFMP_IN(const ZFInput &, callback)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        ) {
    return ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(callback, errorHint);
}

static void _ZFP_ZFXmlToOutput(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        );
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFXmlToOutput
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFXml &, xmlItem)
        , ZFMP_IN_OPT(const ZFXmlOutputToken &, token, ZFXmlOutputTokenDefault())
        ) {
    if(!output || !xmlItem) {
        return zffalse;
    }
    _ZFP_ZFXmlToOutput(output, zfnull, xmlItem, token, 0, 0);
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFXmlToStringT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFXml &, xmlItem)
        , ZFMP_IN(const ZFXmlOutputToken &, token)
        ) {
    return ZFXmlToOutput(ZFOutputForString(ret), xmlItem, token);
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFXmlToString
        , ZFMP_IN(const ZFXml &, xmlItem)
        , ZFMP_IN(const ZFXmlOutputToken &, token)
        ) {
    zfstring ret;
    ZFXmlToStringT(ret, xmlItem, token);
    return ret;
}

// ============================================================
// escape chars
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharEncode
        , ZFMP_OUT(zfstring &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFXmlEscapeCharEncode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharEncode
        , ZFMP_OUT(const ZFOutput &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFPROTOCOL_ACCESS(ZFXml)->xmlEscapeCharEncode(dst, src, count);
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharDecode
        , ZFMP_OUT(zfstring &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFXmlEscapeCharDecode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharDecode
        , ZFMP_OUT(const ZFOutput &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFPROTOCOL_ACCESS(ZFXml)->xmlEscapeCharDecode(dst, src, count);
}

// ============================================================
static void _ZFP_ZFXmlOutput(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN_OPT const zfchar *s
        ) {
    if(s && *s) {
        output.execute(s);
    }
}
static void _ZFP_ZFXmlOutputEncoded(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN_OPT const zfchar *s
        ) {
    if(s && *s) {
        ZFXmlEscapeCharEncode(output, s);
    }
}
static zfbool _ZFP_ZFXmlOutputElementAttrNeedNewLine(
        ZF_IN const ZFXml &element
        , ZF_IN const ZFXmlOutputToken &token
        ) {
    if(!element) {
        return zffalse;
    }
    if(token.xmlElementAttrCountBeforeAddNewLine == zfindexMax()) {
        return zffalse;
    }
    return element.attrCount() > token.xmlElementAttrCountBeforeAddNewLine;
}
static zfbool _ZFP_ZFXmlOutputElementChildNeedNewLine(
        ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        ) {
    if(!token.xmlElementEndTagAtSameLineIfNoChildElement) {
        return zftrue;
    }
    for(zfindex i = 0; i < xmlItem.childCount(); ++i) {
        if(xmlItem.childAt(i).type() != v_ZFXmlType::e_Text) {
            return zftrue;
        }
    }
    return zffalse;
}
static zfbool _ZFP_ZFXmlOutputElementUseSingleTag(
        ZF_IN const ZFXml &element
        , ZF_IN const ZFXmlOutputToken &token
        ) {
    return (token.xmlElementTrimTagIfNoChildren && element.childCount() == 0);
}

static void _ZFP_ZFXmlToOutput_Attr(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        ) {
    zfindex attrIndex = 0;
    for(zfiter it = xmlItem.attrIter(); it; ++it, ++attrIndex) {
        if(token.xmlElementAttrCountBeforeAddNewLine == 0
                || (token.xmlElementAttrCountBeforeAddNewLine != zfindexMax()
                    && attrIndex > 0
                    && (attrIndex % token.xmlElementAttrCountBeforeAddNewLine) == 0)
                    ) {
            _ZFP_ZFXmlOutput(output, token.xmlNewLineToken);
            _ZFP_ZFXmlOutput(output, token.xmlGlobalLineBeginToken);
            ZFOutputRepeat(output, token.xmlIndentToken, depth);
        }
        else {
            _ZFP_ZFXmlOutput(output, " ");
        }

        _ZFP_ZFXmlOutput(output, xmlItem.attrIterKey(it));
        _ZFP_ZFXmlOutput(output, token.xmlAttrEqualTag);

        _ZFP_ZFXmlOutput(output, token.xmlAttrQuoteTagLeft);
        _ZFP_ZFXmlOutputEncoded(output, xmlItem.attrIterValue(it));
        _ZFP_ZFXmlOutput(output, token.xmlAttrQuoteTagRight);
    }
}
static void _ZFP_ZFXmlToOutput_Element(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        ) {
    // element tag begin
    if(depth > 0 || siblingIndex > 0) {
        _ZFP_ZFXmlOutput(output, token.xmlNewLineToken);
    }
    _ZFP_ZFXmlOutput(output, token.xmlGlobalLineBeginToken);
    if((depth > 0 && siblingIndex > 0) && token.xmlElementAddNewLineAtHeadIfNotSingleLine
            && (_ZFP_ZFXmlOutputElementAttrNeedNewLine(xmlItem, token)
                || _ZFP_ZFXmlOutputElementChildNeedNewLine(xmlItem, token))
                ) {
        _ZFP_ZFXmlOutput(output, token.xmlNewLineToken);
        _ZFP_ZFXmlOutput(output, token.xmlGlobalLineBeginToken);
    }
    ZFOutputRepeat(output, token.xmlIndentToken, depth);
    if(_ZFP_ZFXmlOutputElementUseSingleTag(xmlItem, token)) {
        _ZFP_ZFXmlOutput(output, token.xmlElementSingleTagLeft);
    }
    else {
        _ZFP_ZFXmlOutput(output, token.xmlElementBeginTagLeft);
    }
    _ZFP_ZFXmlOutput(output, xmlItem.name());

    // attribute
    _ZFP_ZFXmlToOutput_Attr(output, xmlParent, xmlItem, token, depth + 1, siblingIndex);

    // element tag end
    if(_ZFP_ZFXmlOutputElementUseSingleTag(xmlItem, token)) {
        _ZFP_ZFXmlOutput(output, token.xmlElementSingleTagRight);
    }
    else {
        _ZFP_ZFXmlOutput(output, token.xmlElementBeginTagRight);
    }

    // child
    for(zfindex i = 0; i < xmlItem.childCount(); ++i) {
        _ZFP_ZFXmlToOutput(output, xmlItem, xmlItem.childAt(i), token, depth + 1, i);
    }

    // element end
    if(!_ZFP_ZFXmlOutputElementUseSingleTag(xmlItem, token)) {
        if(_ZFP_ZFXmlOutputElementChildNeedNewLine(xmlItem, token)
                || _ZFP_ZFXmlOutputElementAttrNeedNewLine(xmlItem, token)
                ) {
            _ZFP_ZFXmlOutput(output, token.xmlNewLineToken);
            _ZFP_ZFXmlOutput(output, token.xmlGlobalLineBeginToken);
            ZFOutputRepeat(output, token.xmlIndentToken, depth);
        }
        _ZFP_ZFXmlOutput(output, token.xmlElementEndTagLeft);
        _ZFP_ZFXmlOutput(output, xmlItem.name());
        _ZFP_ZFXmlOutput(output, token.xmlElementEndTagRight);
    }
}
static void _ZFP_ZFXmlToOutput_Text(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        ) {
    if(xmlItem.CDATA()) {
        _ZFP_ZFXmlOutput(output, token.xmlTextCDATATagLeft);
        _ZFP_ZFXmlOutput(output, xmlItem.value());
        _ZFP_ZFXmlOutput(output, token.xmlTextCDATATagRight);
    }
    else {
        _ZFP_ZFXmlOutputEncoded(output, xmlItem.value());
    }
}
static void _ZFP_ZFXmlToOutput_Comment(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        ) {
    if(depth > 0 || siblingIndex > 0) {
        _ZFP_ZFXmlOutput(output, token.xmlNewLineToken);
    }
    _ZFP_ZFXmlOutput(output, token.xmlGlobalLineBeginToken);
    ZFOutputRepeat(output, token.xmlIndentToken, depth);

    _ZFP_ZFXmlOutput(output, token.xmlCommentTagLeft);
    _ZFP_ZFXmlOutput(output, xmlItem.value());
    _ZFP_ZFXmlOutput(output, token.xmlCommentTagRight);
}
static void _ZFP_ZFXmlToOutput_Declaration(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        ) {
    if(depth > 0 || siblingIndex > 0) {
        _ZFP_ZFXmlOutput(output, token.xmlNewLineToken);
    }
    _ZFP_ZFXmlOutput(output, token.xmlGlobalLineBeginToken);
    ZFOutputRepeat(output, token.xmlIndentToken, depth);
    _ZFP_ZFXmlOutput(output, token.xmlDeclarationTagLeft);
    _ZFP_ZFXmlToOutput_Attr(output, xmlParent, xmlItem, token, depth + 1, siblingIndex);
    _ZFP_ZFXmlOutput(output, token.xmlDeclarationTagRight);
}
static void _ZFP_ZFXmlToOutput_DocType(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        ) {
    if(depth > 0 || siblingIndex > 0) {
        _ZFP_ZFXmlOutput(output, token.xmlNewLineToken);
    }
    _ZFP_ZFXmlOutput(output, token.xmlGlobalLineBeginToken);
    ZFOutputRepeat(output, token.xmlIndentToken, depth);
    _ZFP_ZFXmlOutput(output, token.xmlDocTypeTagLeft);
    _ZFP_ZFXmlOutput(output, " ");
    _ZFP_ZFXmlOutput(output, xmlItem.value());
    _ZFP_ZFXmlOutput(output, token.xmlDocTypeTagRight);
}
static void _ZFP_ZFXmlToOutput_PI(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        ) {
    if(depth > 0 || siblingIndex > 0) {
        _ZFP_ZFXmlOutput(output, token.xmlNewLineToken);
    }
    _ZFP_ZFXmlOutput(output, token.xmlGlobalLineBeginToken);
    ZFOutputRepeat(output, token.xmlIndentToken, depth);
    _ZFP_ZFXmlOutput(output, token.xmlPITagLeft);
    _ZFP_ZFXmlOutput(output, xmlItem.name());
    _ZFP_ZFXmlOutput(output, " ");
    _ZFP_ZFXmlOutput(output, xmlItem.value());
    _ZFP_ZFXmlOutput(output, token.xmlPITagRight);
}
static void _ZFP_ZFXmlToOutput(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFXml &xmlParent
        , ZF_IN const ZFXml &xmlItem
        , ZF_IN const ZFXmlOutputToken &token
        , ZF_IN zfindex depth
        , ZF_IN zfindex siblingIndex
        ) {
    switch(xmlItem.type()) {
        case v_ZFXmlType::e_Null:
            ZFCoreCriticalShouldNotGoHere();
            break;
        case v_ZFXmlType::e_Element:
            _ZFP_ZFXmlToOutput_Element(output, xmlParent, xmlItem, token, depth, siblingIndex);
            break;
        case v_ZFXmlType::e_Text:
            _ZFP_ZFXmlToOutput_Text(output, xmlParent, xmlItem, token, depth, siblingIndex);
            break;
        case v_ZFXmlType::e_Comment:
            _ZFP_ZFXmlToOutput_Comment(output, xmlParent, xmlItem, token, depth, siblingIndex);
            break;
        case v_ZFXmlType::e_Document:
            for(zfindex i = 0; i < xmlItem.childCount(); ++i) {
                _ZFP_ZFXmlToOutput(output, xmlItem, xmlItem.childAt(i), token, depth, i);
            }
            break;
        case v_ZFXmlType::e_Declaration:
            _ZFP_ZFXmlToOutput_Declaration(output, xmlParent, xmlItem, token, depth, siblingIndex);
            break;
        case v_ZFXmlType::e_DocType:
            _ZFP_ZFXmlToOutput_DocType(output, xmlParent, xmlItem, token, depth, siblingIndex);
            break;
        case v_ZFXmlType::e_PI:
            _ZFP_ZFXmlToOutput_PI(output, xmlParent, xmlItem, token, depth, siblingIndex);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
}

ZF_NAMESPACE_GLOBAL_END

