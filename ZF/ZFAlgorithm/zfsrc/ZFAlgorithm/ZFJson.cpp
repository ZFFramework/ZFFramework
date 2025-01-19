#include "ZFJson.h"
#include "protocol/ZFProtocolZFJson.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#include "ZFCore/ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFJsonType)

// ============================================================
zfbool ZFJsonOutputToken::operator == (ZF_IN ZFJsonOutputToken const &ref) const {
    return (zftrue
            && this->jsonNewLineToken == ref.jsonNewLineToken
            && this->jsonIndentToken == ref.jsonIndentToken
            && this->jsonValueSeparatorToken == ref.jsonValueSeparatorToken
            && this->jsonSeparatorToken == ref.jsonSeparatorToken
            && this->jsonSeparatorInSameLineToken == ref.jsonSeparatorInSameLineToken
            && this->jsonKeyTagLeft == ref.jsonKeyTagLeft
            && this->jsonKeyTagRight == ref.jsonKeyTagRight
            && this->jsonValueTagLeft == ref.jsonValueTagLeft
            && this->jsonValueTagRight == ref.jsonValueTagRight
            && this->jsonObjectTagLeft == ref.jsonObjectTagLeft
            && this->jsonObjectTagRight == ref.jsonObjectTagRight
            && this->jsonArrayTagLeft == ref.jsonArrayTagLeft
            && this->jsonArrayTagRight == ref.jsonArrayTagRight

            && this->jsonGlobalLineBeginToken == ref.jsonGlobalLineBeginToken
            && this->jsonObjectAddNewLineForContent == ref.jsonObjectAddNewLineForContent
            && this->jsonObjectTagInSameLineIfNoContent == ref.jsonObjectTagInSameLineIfNoContent
            && this->jsonArrayAddNewLineForContent == ref.jsonArrayAddNewLineForContent
            && this->jsonArrayContentTagInSameLine == ref.jsonArrayContentTagInSameLine
            && this->jsonArrayTagInSameLineIfNoContent == ref.jsonArrayTagInSameLineIfNoContent
        );
}
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFJsonOutputToken, ZFJsonOutputToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonNewLineToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonIndentToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonValueSeparatorToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonSeparatorToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonSeparatorInSameLineToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonKeyTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonKeyTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonValueTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonValueTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonObjectTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonObjectTagRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonArrayTagLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonArrayTagRight)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfstring, jsonGlobalLineBeginToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfbool, jsonObjectAddNewLineForContent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfbool, jsonObjectTagInSameLineIfNoContent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfbool, jsonArrayAddNewLineForContent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfbool, jsonArrayContentTagInSameLine)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputToken, zfbool, jsonArrayTagInSameLineIfNoContent)

// ============================================================
ZFEXPORT_VAR_DEFINE(ZFJsonOutputToken, ZFJsonOutputTokenDefault, ZFJsonOutputToken())

static const ZFJsonOutputToken &_ZFP_ZFJsonOutputTokenTrimInit(void) {
    static ZFJsonOutputToken d;
    d.jsonNewLineToken.removeAll();
    d.jsonIndentToken.removeAll();
    d.jsonValueSeparatorToken = ":";
    d.jsonSeparatorInSameLineToken = ",";
    d.jsonObjectAddNewLineForContent = zffalse;
    d.jsonArrayAddNewLineForContent = zffalse;
    return d;
}
ZFEXPORT_VAR_DEFINE(ZFJsonOutputToken, ZFJsonOutputTokenTrim, _ZFP_ZFJsonOutputTokenTrimInit())
ZFEXPORT_VAR_DEFINE(ZFJsonOutputToken, ZFJsonOutputTokenDetail, ZFJsonOutputToken())

// ============================================================
static void _ZFP_ZFJsonToOutput_output(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &item
        , ZF_IN const ZFJsonOutputToken &token
        , ZF_IN zfindex indentLevel
        );
static void _ZFP_ZFJsonToOutput_output_Value(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &value
        , ZF_IN const ZFJsonOutputToken &token
        , ZF_IN zfindex indentLevel
        ) {
    output << token.jsonValueTagLeft;
    ZFJsonEscapeCharEncode(output, value.value());
    output << token.jsonValueTagRight;
}
static void _ZFP_ZFJsonToOutput_output_Object(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &item
        , ZF_IN const ZFJsonOutputToken &token
        , ZF_IN zfindex indentLevel
        ) {
    output << token.jsonObjectTagLeft;
    zfbool first = zftrue;
    for(zfiter it = item.attrIter(); it; ++it) {
        if(token.jsonObjectAddNewLineForContent) {
            if(first) {
                first = zffalse;
            }
            else {
                output << token.jsonSeparatorToken;
            }
            output << token.jsonNewLineToken << token.jsonGlobalLineBeginToken;
            ZFOutputRepeat(output, token.jsonIndentToken, indentLevel + 1);
        }
        else {
            if(first) {
                first = zffalse;
            }
            else {
                output << token.jsonSeparatorInSameLineToken;
            }
        }
        output
            << token.jsonKeyTagLeft
            << item.attrIterKey(it)
            << token.jsonKeyTagRight;
        output << token.jsonValueSeparatorToken;
        _ZFP_ZFJsonToOutput_output(output, item.attrIterValue(it), token, indentLevel + 1);
    }
    if(token.jsonObjectAddNewLineForContent
            && !(token.jsonObjectTagInSameLineIfNoContent && item.attrCount() == 0)
            ) {
        output << token.jsonNewLineToken << token.jsonGlobalLineBeginToken;
        ZFOutputRepeat(output, token.jsonIndentToken, indentLevel);
    }
    output << token.jsonObjectTagRight;
}
static void _ZFP_ZFJsonToOutput_output_Array(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &jsonArray
        , ZF_IN const ZFJsonOutputToken &token
        , ZF_IN zfindex indentLevel
        ) {
    output << token.jsonArrayTagLeft;
    for(zfindex i = 0; i < jsonArray.childCount(); ++i) {
        if(token.jsonArrayAddNewLineForContent) {
            if(i != 0) {
                if(token.jsonArrayContentTagInSameLine) {
                    output << token.jsonSeparatorInSameLineToken;
                }
                else {
                    output << token.jsonSeparatorToken;
                }
            }

            if(!token.jsonArrayContentTagInSameLine) {
                output << token.jsonNewLineToken << token.jsonGlobalLineBeginToken;
                ZFOutputRepeat(output, token.jsonIndentToken, indentLevel);
            }
            _ZFP_ZFJsonToOutput_output(output, jsonArray.childAt(i), token, indentLevel + 1);
        }
        else {
            if(i != 0) {
                output << token.jsonSeparatorInSameLineToken;
            }
            _ZFP_ZFJsonToOutput_output(output, jsonArray.childAt(i), token, indentLevel + 1);
        }
    }
    if(token.jsonArrayAddNewLineForContent
            && !(token.jsonArrayTagInSameLineIfNoContent && jsonArray.childCount() == 0)
            ) {
        output << token.jsonNewLineToken << token.jsonGlobalLineBeginToken;
        ZFOutputRepeat(output, token.jsonIndentToken, indentLevel);
    }
    output << token.jsonArrayTagRight;
}
static void _ZFP_ZFJsonToOutput_output(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &item
        , ZF_IN const ZFJsonOutputToken &token
        , ZF_IN zfindex indentLevel
        ) {
    switch(item.type()) {
        case ZFJsonType::e_Value:
            _ZFP_ZFJsonToOutput_output_Value(output, item, token, indentLevel);
            break;
        case ZFJsonType::e_Object:
            _ZFP_ZFJsonToOutput_output_Object(output, item, token, indentLevel);
            break;
        case ZFJsonType::e_Array:
            _ZFP_ZFJsonToOutput_output_Array(output, item, token, indentLevel);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
zfclassNotPOD _ZFP_ZFJsonPrivate {
public:
    typedef zfimplmap<zfstring, ZFJson> AttrMap;
    typedef zfstldeque<ZFJson> ChildList;
public:
    zfuint refCount;
    ZFJsonType type;
    union {
        zfstring *value; // for ZFJsonType::e_Value
        AttrMap *attrMap; // for ZFJsonType::e_Object
        ChildList *childList; // for ZFJsonType::e_Array
    } d;

public:
    explicit _ZFP_ZFJsonPrivate(ZF_IN ZFJsonType type)
    : refCount(1)
    , type(type)
    {
        d.value = zfnull;
    }
    ~_ZFP_ZFJsonPrivate(void) {
        switch(this->type) {
            case ZFJsonType::e_Null:
                break;
            case ZFJsonType::e_Value:
                if(d.value) {
                    zfdelete(d.value);
                }
                break;
            case ZFJsonType::e_Object:
                if(d.attrMap) {
                    zfdelete(d.attrMap);
                }
                break;
            case ZFJsonType::e_Array:
                if(d.childList) {
                    zfdelete(d.childList);
                }
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                break;
        }
    }
};

// ============================================================
// ZFJson
ZFJson::ZFJson(ZF_IN _ZFP_ZFJsonPrivate *ref)
: d(ref)
{
    if(d) {
        ++(d->refCount);
    }
}
ZFJson::ZFJson(void)
: d(zfnull)
{
}
ZFJson::ZFJson(ZF_IN const zfnullT &dummy)
: d(zfnull)
{
}
ZFJson::ZFJson(ZF_IN ZFJsonType type) {
    if(type != ZFJsonType::e_Null) {
        d = zfnew(_ZFP_ZFJsonPrivate, type);
    }
    else {
        d = zfnull;
    }
}
ZFJson::ZFJson(ZF_IN const ZFJson &ref)
: d(ref.d)
{
    if(d) {
        ++(d->refCount);
    }
}
ZFJson::~ZFJson(void) {
    if(d) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
        }
    }
}

ZFJson &ZFJson::operator = (ZF_IN const ZFJson &ref) {
    _ZFP_ZFJsonPrivate *dTmp = d;
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
ZFJson &ZFJson::operator = (ZF_IN const zfnullT &dummy) {
    if(d) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
        }
        d = zfnull;
    }
    return *this;
}
zfbool ZFJson::operator == (ZF_IN const ZFJson &ref) const {
    return (d == ref.d);
}

// ============================================================
void ZFJson::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ZFJsonToStringT(ret, *this, ZFJsonOutputTokenTrim());
}

zfindex ZFJson::objectRetainCount(void) const {
    return (d ? d->refCount : 0);
}

// ============================================================
void ZFJson::_ZFP_ZFJson_jsonType(ZF_IN ZFJsonType type) {
    if(type != ZFJsonType::e_Null) {
        if(d) {
            d->type = type;
        }
        else {
            d = zfnew(_ZFP_ZFJsonPrivate, type);
        }
    }
}
ZFJsonType ZFJson::type(void) const {
    return (d ? d->type : ZFJsonType::e_Null);
}

ZFJson ZFJson::copy(void) const {
    ZFJson ret(this->type());
    switch(this->type()) {
        case ZFJsonType::e_Null:
            break;
        case ZFJsonType::e_Value:
            ret.value(this->value());
            break;
        case ZFJsonType::e_Object:
            for(zfiter it = this->attrIter(); it; ++it) {
                ret.attr(this->attrIterKey(it), this->attrIterValue(it).copy());
            }
            break;
        case ZFJsonType::e_Array:
            for(zfindex i = 0; i < this->childCount(); ++i) {
                ret.child(this->childAt(i).copy());
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
    return ret;
}

ZFJson &ZFJson::merge(ZF_IN const ZFJson &other) {
    if(d == other.d) {
        return *this;
    }
    switch(this->type()) {
        case ZFJsonType::e_Null:
            if(other.type() == ZFJsonType::e_Object || other.type() == ZFJsonType::e_Array) {
                this->operator=(other.copy());
            }
            break;
        case ZFJsonType::e_Value:
            break;
        case ZFJsonType::e_Object:
            if(other.type() == ZFJsonType::e_Object) {
                for(zfiter it = other.attrIter(); it; ++it) {
                    zfstring key = other.attrIterKey(it);
                    ZFJson exist = this->attr(key);
                    ZFJson attach = other.attrIterValue(it);
                    if(exist.type() == ZFJsonType::e_Object && attach.type() == ZFJsonType::e_Object) {
                        exist.merge(attach);
                    }
                    else {
                        this->attr(key, attach);
                    }
                }
            }
            break;
        case ZFJsonType::e_Array:
            if(other.type() == ZFJsonType::e_Array) {
                this->childRemoveAll();
                for(zfindex i = 0; i < other.childCount(); ++i) {
                    this->child(other.childAt(i));
                }
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
    return *this;
}

// ============================================================
// for value type
ZFJson &ZFJson::value(ZF_IN const zfstring &value) {
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFJsonPrivate, ZFJsonType::e_Value);
    }
    ZFCoreAssert(this->type() == ZFJsonType::e_Value);
    if(d->d.value == zfnull) {
        d->d.value = zfnew(zfstring, value);
    }
    else {
        *(d->d.value) = value;
    }
    return *this;
}
const zfstring &ZFJson::value(void) const {
    return d && d->type == ZFJsonType::e_Value && d->d.value ? *(d->d.value) : zfstring::Empty();
}

// ============================================================
// for object type
zfindex ZFJson::attrCount(void) const {
    return d && d->type == ZFJsonType::e_Object && d->d.attrMap ? (zfindex)d->d.attrMap->size() : 0;
}

ZFJson &ZFJson::attr(
        ZF_IN const zfstring &key
        , ZF_IN const zfstring &value
        ) {
    if(value == zfnull) {
        this->attrRemove(key);
    }
    else {
        ZFJson tmp(ZFJsonType::e_Value);
        tmp.value(value);
        this->attr(key, tmp);
    }
    return *this;
}
ZFJson &ZFJson::attr(
        ZF_IN const zfstring &key
        , ZF_IN const ZFJson &item
        ) {
    if(key == zfnull) {
        return *this;
    }
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFJsonPrivate, ZFJsonType::e_Object);
    }
    ZFCoreAssert(this->type() == ZFJsonType::e_Object);
    if(item) {
        if(d->d.attrMap == zfnull) {
            d->d.attrMap = zfnew(_ZFP_ZFJsonPrivate::AttrMap);
        }
        (*(d->d.attrMap))[key] = item;
    }
    else {
        this->attrRemove(key);
    }
    return *this;
}
ZFJson ZFJson::attr(ZF_IN const zfstring &key) const {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        _ZFP_ZFJsonPrivate::AttrMap::iterator it = d->d.attrMap->find(key);
        if(it != d->d.attrMap->end()) {
            return it->second;
        }
    }
    return zfnull;
}
zfstring ZFJson::attrValue(ZF_IN const zfstring &key) const {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        _ZFP_ZFJsonPrivate::AttrMap::iterator it = d->d.attrMap->find(key);
        if(it != d->d.attrMap->end() && it->second.type() == ZFJsonType::e_Value) {
            return it->second.value();
        }
    }
    return zfnull;
}

ZFJson &ZFJson::attrRemove(ZF_IN const zfstring &key) {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        d->d.attrMap->erase(key);
    }
    return *this;
}

ZFJson &ZFJson::attrRemoveAll(void) {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        d->d.attrMap->clear();
    }
    return *this;
}

zfiter ZFJson::attrIter(void) const {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        return d->d.attrMap->iter();
    }
    else {
        return zfnull;
    }
}

zfiter ZFJson::attrIterFind(ZF_IN const zfstring &key) const {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        return d->d.attrMap->iterFind(key);
    }
    else {
        return zfnull;
    }
}

zfstring ZFJson::attrIterKey(ZF_IN const zfiter &it) const {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        return d->d.attrMap->iterKey(it);
    }
    else {
        return zfnull;
    }
}
ZFJson ZFJson::attrIterValue(ZF_IN const zfiter &it) const {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        return d->d.attrMap->iterValue(it);
    }
    else {
        return zfnull;
    }
}

void ZFJson::attrIterValue(
        ZF_IN_OUT zfiter &it
        , ZF_IN const ZFJson &item
        ) {
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFJsonPrivate, ZFJsonType::e_Object);
    }
    ZFCoreAssert(this->type() == ZFJsonType::e_Object);
    if(!item) {
        this->attrIterRemove(it);
    }
    else {
        if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
            d->d.attrMap->iterValue(it, item);
        }
    }
}

void ZFJson::attrIterRemove(ZF_IN_OUT zfiter &it) {
    if(d && d->type == ZFJsonType::e_Object && d->d.attrMap) {
        d->d.attrMap->iterRemove(it);
    }
}

// ============================================================
zfindex ZFJson::childCount(void) const {
    if(d && d->type == ZFJsonType::e_Array && d->d.childList) {
        return (zfindex)d->d.childList->size();
    }
    else {
        return 0;
    }
}
ZFJson ZFJson::childAt(ZF_IN zfindex index) const {
    if(d && d->type == ZFJsonType::e_Array && d->d.childList && index < (zfindex)d->d.childList->size()) {
        return d->d.childList->at((zfstlsize)index);
    }
    else {
        return zfnull;
    }
}

ZFJson &ZFJson::child(
        ZF_IN const zfstring &value
        , ZF_IN_OPT zfindex index /* = zfindexMax() */
        ) {
    return this->child(ZFJson(ZFJsonType::e_Value).value(value), index);
}
ZFJson &ZFJson::child(
        ZF_IN const ZFJson &item
        , ZF_IN_OPT zfindex index /* = zfindexMax() */
        ) {
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFJsonPrivate, ZFJsonType::e_Array);
    }
    ZFCoreAssert(this->type() == ZFJsonType::e_Array);
    if(d->d.childList == zfnull) {
        d->d.childList = zfnew(_ZFP_ZFJsonPrivate::ChildList);
    }
    if(index == zfindexMax()) {
        index = this->childCount();
    }
    else {
        ZFCoreAssertIndexRange(index, this->childCount() + 1);
    }
    ZFCoreAssertWithMessage(item, "add null object");
    d->d.childList->insert(d->d.childList->begin() + index, item);
    return *this;
}

ZFJson &ZFJson::childRemoveAt(ZF_IN zfindex index) {
    if(index < this->childCount()) {
        d->d.childList->erase(d->d.childList->begin() + index);
    }
    return *this;
}
ZFJson &ZFJson::childRemoveAll(void) {
    if(d && d->type == ZFJsonType::e_Array && d->d.childList) {
        d->d.childList->clear();
    }
    return *this;
}
zfindex ZFJson::childFind(ZF_IN const ZFJson &item) const {
    if(d && d->type == ZFJsonType::e_Array && d->d.childList) {
        for(zfstlsize i = 0; i < d->d.childList->size(); ++i) {
            if(item == d->d.childList->at(i)) {
                return (zfindex)i;
            }
        }
    }
    return zfindexMax();
}

ZFJson::operator zfstring (void) const {
    if(this->valid()) {
        return ZFJsonToString(*this);
    }
    else {
        return zfnull;
    }
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFJson, ZFJson, {
        v = ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(src, srcLen, errorHint);
        return v;
    }, {
        return ZFJsonToOutput(ZFOutputForString(s), v);
    })

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        v_ZFJson *v = invokerObject;
        v->zfv._ZFP_ZFJson_jsonType(type);
    }, v_ZFJson
    , ZFMP_IN(ZFJsonType, type)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, void, objectInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, ZFJsonType, type)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, ZFJson, copy)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson &, merge
        , ZFMP_IN(const ZFJson &, other)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson &, value
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, const zfstring &, value)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, ZFJson &, attr
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, ZFJson &, attr
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const ZFJson &, item)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson, attr
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, zfstring, attrValue
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson &, attrRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, ZFJson &, attrRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfindex, attrCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfiter, attrIter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, zfiter, attrIterFind
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, zfstring, attrIterKey
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson, attrIterValue
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, void, attrIterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(const ZFJson &, item)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, void, attrIterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfindex, childCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson, childAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, ZFJson &, child
        , ZFMP_IN(const zfstring &, value)
        , ZFMP_IN_OPT(zfindex, index, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, ZFJson &, child
        , ZFMP_IN(const ZFJson &, item)
        , ZFMP_IN_OPT(zfindex, index, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson &, childRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, ZFJson &, childRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, zfindex, childFind
        , ZFMP_IN(const ZFJson &, item)
        )

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(ZFJson, ZFJsonObject) {return ZFJson(ZFJsonType::e_Object);}
ZFMETHOD_FUNC_DEFINE_0(ZFJson, ZFJsonArray) {return ZFJson(ZFJsonType::e_Array);}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFJson, ZFJsonFromInput
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        ) {
    return ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(input, errorHint);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFJsonToOutput
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFJson &, item)
        , ZFMP_IN_OPT(const ZFJsonOutputToken &, token, ZFJsonOutputTokenDefault())
        ) {
    if(output && item) {
        output << token.jsonGlobalLineBeginToken;
        if(item.type() == ZFJsonType::e_Value) {
            output << item.value();
        }
        else {
            _ZFP_ZFJsonToOutput_output(output, item, token, 0);
        }
        return zftrue;
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFJsonToStringT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFJson &, item)
        , ZFMP_IN(const ZFJsonOutputToken &, token)
        ) {
    return ZFJsonToOutput(ZFOutputForString(ret), item, token);
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFJsonToString
        , ZFMP_IN(const ZFJson &, item)
        , ZFMP_IN(const ZFJsonOutputToken &, token)
        ) {
    zfstring ret;
    ZFJsonToStringT(ret, item, token);
    return ret;
}

// ============================================================
// escape chars
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharEncode
        , ZFMP_OUT(zfstring &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFJsonEscapeCharEncode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharEncode
        , ZFMP_OUT(const ZFOutput &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFPROTOCOL_ACCESS(ZFJson)->jsonEscapeCharEncode(dst, src, count);
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharDecode
        , ZFMP_OUT(zfstring &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFJsonEscapeCharDecode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharDecode
        , ZFMP_OUT(const ZFOutput &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFPROTOCOL_ACCESS(ZFJson)->jsonEscapeCharDecode(dst, src, count);
}

ZF_NAMESPACE_GLOBAL_END

