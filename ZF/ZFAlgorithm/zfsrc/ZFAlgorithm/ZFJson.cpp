#include "ZFJson.h"
#include "protocol/ZFProtocolZFJson.h"

#include "ZFCore/ZFSTLWrapper/zfstldeque.h"
#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

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

zfbool ZFJsonOutputFlags::operator == (ZF_IN ZFJsonOutputFlags const &ref) const {
    return (zftrue
            && this->jsonToken == ref.jsonToken
            && this->jsonGlobalLineBeginToken == ref.jsonGlobalLineBeginToken
            && this->jsonObjectAddNewLineForContent == ref.jsonObjectAddNewLineForContent
            && this->jsonObjectTagInSameLineIfNoContent == ref.jsonObjectTagInSameLineIfNoContent
            && this->jsonArrayAddNewLineForContent == ref.jsonArrayAddNewLineForContent
            && this->jsonArrayContentTagInSameLine == ref.jsonArrayContentTagInSameLine
            && this->jsonArrayTagInSameLineIfNoContent == ref.jsonArrayTagInSameLineIfNoContent
        );
}
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFJsonOutputFlags, ZFJsonOutputFlags)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputFlags, ZFJsonOutputToken, jsonToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputFlags, zfstring, jsonGlobalLineBeginToken)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputFlags, zfbool, jsonObjectAddNewLineForContent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputFlags, zfbool, jsonObjectTagInSameLineIfNoContent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputFlags, zfbool, jsonArrayAddNewLineForContent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputFlags, zfbool, jsonArrayContentTagInSameLine)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFJsonOutputFlags, zfbool, jsonArrayTagInSameLineIfNoContent)

// ============================================================
ZFEXPORT_VAR_READONLY_DEFINE(ZFJsonOutputFlags, ZFJsonOutputFlagsDefault, ZFJsonOutputFlags())

static const ZFJsonOutputFlags &_ZFP_ZFJsonOutputFlagsTrimInit(void) {
    static ZFJsonOutputFlags d;
    d.jsonToken.jsonNewLineToken.removeAll();
    d.jsonToken.jsonIndentToken.removeAll();
    d.jsonToken.jsonValueSeparatorToken = ":";
    d.jsonToken.jsonSeparatorInSameLineToken = ",";
    d.jsonObjectAddNewLineForContent = zffalse;
    d.jsonArrayAddNewLineForContent = zffalse;
    return d;
}
ZFEXPORT_VAR_READONLY_DEFINE(ZFJsonOutputFlags, ZFJsonOutputFlagsTrim, _ZFP_ZFJsonOutputFlagsTrimInit())

// ============================================================
static void _ZFP_ZFJsonToOutput_outputIndent(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJsonOutputFlags &outputFlags
        , ZF_IN zfindex indentLevel
        ) {
    for(zfindex i = 0; i < indentLevel; ++i) {
        output << outputFlags.jsonToken.jsonIndentToken;
    }
}
static void _ZFP_ZFJsonToOutput_output(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &jsonItem
        , ZF_IN const ZFJsonOutputFlags &outputFlags
        , ZF_IN zfindex indentLevel
        );
static void _ZFP_ZFJsonToOutput_output_JsonValue(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &jsonValue
        , ZF_IN const ZFJsonOutputFlags &outputFlags
        , ZF_IN zfindex indentLevel
        ) {
    output << outputFlags.jsonToken.jsonValueTagLeft;
    ZFJsonEscapeCharEncode(output, jsonValue.jsonValue());
    output << outputFlags.jsonToken.jsonValueTagRight;
}
static void _ZFP_ZFJsonToOutput_output_JsonObject(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &jsonObject
        , ZF_IN const ZFJsonOutputFlags &outputFlags
        , ZF_IN zfindex indentLevel
        ) {
    output << outputFlags.jsonToken.jsonObjectTagLeft;
    zfbool first = zftrue;
    for(zfiterator it = jsonObject.attrIter(); jsonObject.attrIterValid(it); jsonObject.attrIterNext(it)) {
        if(outputFlags.jsonObjectAddNewLineForContent) {
            if(first) {
                first = zffalse;
            }
            else {
                output << outputFlags.jsonToken.jsonSeparatorToken;
            }
            output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
            _ZFP_ZFJsonToOutput_outputIndent(output, outputFlags, indentLevel + 1);
        }
        else {
            if(first) {
                first = zffalse;
            }
            else {
                output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
            }
        }
        output
            << outputFlags.jsonToken.jsonKeyTagLeft
            << jsonObject.attrIterKey(it)
            << outputFlags.jsonToken.jsonKeyTagRight;
        output << outputFlags.jsonToken.jsonValueSeparatorToken;
        _ZFP_ZFJsonToOutput_output(output, jsonObject.attrIterValue(it), outputFlags, indentLevel + 1);
    }
    if(outputFlags.jsonObjectAddNewLineForContent
            && !(outputFlags.jsonObjectTagInSameLineIfNoContent && jsonObject.attrCount() == 0)
            ) {
        output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonToOutput_outputIndent(output, outputFlags, indentLevel);
    }
    output << outputFlags.jsonToken.jsonObjectTagRight;
}
static void _ZFP_ZFJsonToOutput_output_JsonArray(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &jsonArray
        , ZF_IN const ZFJsonOutputFlags &outputFlags
        , ZF_IN zfindex indentLevel
        ) {
    output << outputFlags.jsonToken.jsonArrayTagLeft;
    for(zfindex i = 0; i < jsonArray.childCount(); ++i) {
        if(outputFlags.jsonArrayAddNewLineForContent) {
            if(i != 0) {
                if(outputFlags.jsonArrayContentTagInSameLine) {
                    output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
                }
                else {
                    output << outputFlags.jsonToken.jsonSeparatorToken;
                }
            }

            if(!outputFlags.jsonArrayContentTagInSameLine) {
                output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
                _ZFP_ZFJsonToOutput_outputIndent(output, outputFlags, indentLevel);
            }
            _ZFP_ZFJsonToOutput_output(output, jsonArray.childAt(i), outputFlags, indentLevel + 1);
        }
        else {
            if(i != 0) {
                output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
            }
            _ZFP_ZFJsonToOutput_output(output, jsonArray.childAt(i), outputFlags, indentLevel + 1);
        }
    }
    if(outputFlags.jsonArrayAddNewLineForContent
            && !(outputFlags.jsonArrayTagInSameLineIfNoContent && jsonArray.childCount() == 0)
            ) {
        output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonToOutput_outputIndent(output, outputFlags, indentLevel);
    }
    output << outputFlags.jsonToken.jsonArrayTagRight;
}
static void _ZFP_ZFJsonToOutput_output(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFJson &jsonItem
        , ZF_IN const ZFJsonOutputFlags &outputFlags
        , ZF_IN zfindex indentLevel
        ) {
    switch(jsonItem.jsonType()) {
        case ZFJsonType::e_JsonValue:
            _ZFP_ZFJsonToOutput_output_JsonValue(output, jsonItem, outputFlags, indentLevel);
            break;
        case ZFJsonType::e_JsonObject:
            _ZFP_ZFJsonToOutput_output_JsonObject(output, jsonItem, outputFlags, indentLevel);
            break;
        case ZFJsonType::e_JsonArray:
            _ZFP_ZFJsonToOutput_output_JsonArray(output, jsonItem, outputFlags, indentLevel);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
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
    ZFJsonTypeEnum jsonType;
    union {
        zfchar *jsonValue; // for ZFJsonType::e_JsonValue
        AttrMap *attrMap; // for ZFJsonType::e_JsonObject
        ChildList *childList; // for ZFJsonType::e_JsonArray
    } d;

public:
    explicit _ZFP_ZFJsonPrivate(ZF_IN ZFJsonTypeEnum jsonType)
    : refCount(1)
    , jsonType(jsonType)
    {
        d.jsonValue = zfnull;
    }
    ~_ZFP_ZFJsonPrivate(void) {
        switch(this->jsonType) {
            case ZFJsonType::e_JsonNull:
                break;
            case ZFJsonType::e_JsonValue:
                if(d.jsonValue) {
                    zffree(d.jsonValue);
                }
                break;
            case ZFJsonType::e_JsonObject:
                if(d.attrMap) {
                    zfdelete(d.attrMap);
                }
                break;
            case ZFJsonType::e_JsonArray:
                if(d.childList) {
                    zfdelete(d.childList);
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
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
ZFJson::ZFJson(ZF_IN ZFJsonTypeEnum jsonType) {
    if(jsonType != ZFJsonType::e_JsonNull) {
        d = zfnew(_ZFP_ZFJsonPrivate, jsonType);
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
    ZFJsonToString(ret, *this, ZFJsonOutputFlagsTrim());
}

zfindex ZFJson::objectRetainCount(void) const {
    return (d ? d->refCount : 0);
}

// ============================================================
void ZFJson::_ZFP_ZFJson_jsonType(ZF_IN ZFJsonTypeEnum jsonType) {
    if(jsonType != ZFJsonType::e_JsonNull) {
        if(d) {
            d->jsonType = jsonType;
        }
        else {
            d = zfnew(_ZFP_ZFJsonPrivate, jsonType);
        }
    }
}
ZFJsonTypeEnum ZFJson::jsonType(void) const {
    return (d ? d->jsonType : ZFJsonType::e_JsonNull);
}

ZFJson ZFJson::copy(void) const {
    ZFJson ret(this->jsonType());
    switch(this->jsonType()) {
        case ZFJsonType::e_JsonNull:
            break;
        case ZFJsonType::e_JsonValue:
            ret.jsonValue(this->jsonValue());
            break;
        case ZFJsonType::e_JsonObject:
            for(zfiterator it = this->attrIter(); this->attrIterValid(it); this->attrIterNext(it)) {
                ret.attr(this->attrIterKey(it), this->attrIterValue(it).copy());
            }
            break;
        case ZFJsonType::e_JsonArray:
            for(zfindex i = 0; i < this->childCount(); ++i) {
                ret.childAdd(this->childAt(i).copy());
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    return ret;
}

// ============================================================
// for value type
ZFJson &ZFJson::jsonValue(ZF_IN const zfchar *value) {
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFJsonPrivate, ZFJsonType::e_JsonValue);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonValue);
    zfsChange(d->d.jsonValue, value);
    return *this;
}
const zfchar *ZFJson::jsonValue(void) const {
    return d && d->jsonType == ZFJsonType::e_JsonValue ? d->d.jsonValue : zfnull;
}

// ============================================================
// for object type
ZFJson &ZFJson::attr(
        ZF_IN const zfchar *key
        , ZF_IN const zfchar *value
        ) {
    if(value == zfnull) {
        this->attrRemove(key);
    }
    else {
        ZFJson jsonValue(ZFJsonType::e_JsonValue);
        jsonValue.jsonValue(value);
        this->attr(key, jsonValue);
    }
    return *this;
}
ZFJson &ZFJson::attr(
        ZF_IN const zfchar *key
        , ZF_IN const ZFJson &jsonItem
        ) {
    if(key == zfnull) {
        return *this;
    }
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFJsonPrivate, ZFJsonType::e_JsonObject);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    if(jsonItem) {
        if(d->d.attrMap == zfnull) {
            d->d.attrMap = zfnew(_ZFP_ZFJsonPrivate::AttrMap);
        }
        (*(d->d.attrMap))[key] = jsonItem;
    }
    else {
        this->attrRemove(key);
    }
    return *this;
}
ZFJson ZFJson::attr(ZF_IN const zfchar *key) const {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        _ZFP_ZFJsonPrivate::AttrMap::iterator it = d->d.attrMap->find(key);
        if(it != d->d.attrMap->end()) {
            return it->second;
        }
    }
    return zfnull;
}
const zfchar *ZFJson::attrValue(ZF_IN const zfchar *key) const {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        _ZFP_ZFJsonPrivate::AttrMap::iterator it = d->d.attrMap->find(key);
        if(it != d->d.attrMap->end() && it->second.jsonType() == ZFJsonType::e_JsonValue) {
            return it->second.jsonValue();
        }
    }
    return zfnull;
}

ZFJson &ZFJson::attrRemove(ZF_IN const zfchar *key) {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        d->d.attrMap->erase(key);
    }
    return *this;
}

ZFJson &ZFJson::attrRemoveAll(void) {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        d->d.attrMap->clear();
    }
    return *this;
}

zfindex ZFJson::attrCount(void) const {
    return d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap ? (zfindex)d->d.attrMap->size() : 0;
}

zfiterator ZFJson::attrIter(void) const {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        return d->d.attrMap->iter();
    }
    else {
        return zfiteratorInvalid();
    }
}

zfiterator ZFJson::attrIterFind(ZF_IN const zfchar *key) const {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        return d->d.attrMap->iterFind(key);
    }
    else {
        return zfiteratorInvalid();
    }
}

zfbool ZFJson::attrIterValid(ZF_IN const zfiterator &it) const {
    return d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap && d->d.attrMap->iterValid(it);
}

void ZFJson::attrIterNext(ZF_IN_OUT zfiterator &it) const {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        d->d.attrMap->iterNext(it);
    }
}

const zfchar *ZFJson::attrIterKey(ZF_IN const zfiterator &it) const {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        return d->d.attrMap->iterKey(it);
    }
    else {
        return zfnull;
    }
}
ZFJson ZFJson::attrIterValue(ZF_IN const zfiterator &it) const {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        return d->d.attrMap->iterValue(it);
    }
    else {
        return zfnull;
    }
}

void ZFJson::attrIterValue(
        ZF_IN_OUT zfiterator &it
        , ZF_IN const ZFJson &jsonItem
        ) {
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFJsonPrivate, ZFJsonType::e_JsonObject);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    if(!jsonItem) {
        this->attrIterRemove(it);
    }
    else {
        if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
            d->d.attrMap->iterValue(it, jsonItem);
        }
    }
}

void ZFJson::attrIterRemove(ZF_IN_OUT zfiterator &it) {
    if(d && d->jsonType == ZFJsonType::e_JsonObject && d->d.attrMap) {
        d->d.attrMap->iterRemove(it);
    }
}

// ============================================================
zfindex ZFJson::childCount(void) const {
    if(d && d->jsonType == ZFJsonType::e_JsonArray && d->d.childList) {
        return (zfindex)d->d.childList->size();
    }
    else {
        return 0;
    }
}
ZFJson ZFJson::childAt(ZF_IN zfindex index) const {
    if(d && d->jsonType == ZFJsonType::e_JsonArray && d->d.childList && index < (zfindex)d->d.childList->size()) {
        return d->d.childList->at((zfstlsize)index);
    }
    else {
        return zfnull;
    }
}

ZFJson &ZFJson::childAdd(
        ZF_IN const zfchar *jsonValue
        , ZF_IN_OPT zfindex atIndex /* = zfindexMax() */
        ) {
    return this->childAdd(ZFJson(ZFJsonType::e_JsonValue).jsonValue(jsonValue), atIndex);
}
ZFJson &ZFJson::childAdd(
        ZF_IN const ZFJson &jsonObject
        , ZF_IN_OPT zfindex atIndex /* = zfindexMax() */
        ) {
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFJsonPrivate, ZFJsonType::e_JsonArray);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonArray);
    if(d->d.childList == zfnull) {
        d->d.childList = zfnew(_ZFP_ZFJsonPrivate::ChildList);
    }
    if(atIndex == zfindexMax()) {
        atIndex = (zfindex)d->d.childList->size();
    }
    else if(atIndex > (zfindex)d->d.childList->size()) {
        zfCoreCriticalIndexOutOfRange(atIndex, (zfindex)d->d.childList->size() + 1);
        return *this;
    }
    zfCoreAssertWithMessage(jsonObject, "add null object");
    d->d.childList->insert(d->d.childList->begin() + atIndex, jsonObject);
    return *this;
}

ZFJson &ZFJson::childRemoveAt(ZF_IN zfindex index) {
    if(index >= this->childCount()) {
        zfCoreCriticalIndexOutOfRange(index, this->childCount());
    }
    if(d && d->jsonType == ZFJsonType::e_JsonArray && d->d.childList) {
        d->d.childList->erase(d->d.childList->begin() + index);
    }
    return *this;
}
ZFJson &ZFJson::childRemoveAll(void) {
    if(d && d->jsonType == ZFJsonType::e_JsonArray && d->d.childList) {
        d->d.childList->clear();
    }
    return *this;
}
zfindex ZFJson::childFind(ZF_IN const ZFJson &jsonObject) const {
    if(d && d->jsonType == ZFJsonType::e_JsonArray && d->d.childList) {
        for(zfstlsize i = 0; i < d->d.childList->size(); ++i) {
            if(jsonObject == d->d.childList->at(i)) {
                return (zfindex)i;
            }
        }
    }
    return zfindexMax();
}

ZFJson::operator zfstring (void) const {
    return ZFJsonToString(*this);
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFJson, ZFJson, {
        v = ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(src, srcLen);
        return v;
    }, {
        return ZFJsonToOutput(ZFOutputForString(s), v);
    })

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        invokerObject->to<v_ZFJson *>()->zfv._ZFP_ZFJson_jsonType(jsonType);
    }, v_ZFJson
    , ZFMP_IN(ZFJsonTypeEnum, jsonType)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, void, objectInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, ZFJsonTypeEnum, jsonType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfbool, jsonTypeValid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, ZFJson, copy)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson &, jsonValue
        , ZFMP_IN(const zfchar *, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, const zfchar *, jsonValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, ZFJson &, attr
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN(const zfchar *, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, ZFJson &, attr
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN(const ZFJson &, jsonItem)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson, attr
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, const zfchar *, attrValue
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson &, attrRemove
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, ZFJson &, attrRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfindex, attrCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfiterator, attrIter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, zfiterator, attrIterFind
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, zfbool, attrIterValid
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, void, attrIterNext
        , ZFMP_IN_OUT(zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, const zfchar *, attrIterKey
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson, attrIterValue
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, void, attrIterValue
        , ZFMP_IN_OUT(zfiterator &, it)
        , ZFMP_IN(const ZFJson &, jsonItem)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, void, attrIterRemove
        , ZFMP_IN_OUT(zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, zfindex, childCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson, childAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, ZFJson &, childAdd
        , ZFMP_IN(const zfchar *, jsonValue)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJson, ZFJson &, childAdd
        , ZFMP_IN(const ZFJson &, jsonObject)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, ZFJson &, childRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJson, ZFJson &, childRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJson, zfindex, childFind
        , ZFMP_IN(const ZFJson &, jsonObject)
        )

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(ZFJson, ZFJsonObject) {return ZFJson(ZFJsonType::e_JsonObject);}
ZFMETHOD_FUNC_DEFINE_0(ZFJson, ZFJsonArray) {return ZFJson(ZFJsonType::e_JsonArray);}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFJson, ZFJsonFromInput
        , ZFMP_IN(const ZFInput &, input)
        ) {
    return ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(input);
}
ZFMETHOD_FUNC_DEFINE_2(ZFJson, ZFJsonFromString
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, length, zfindexMax())
        ) {
    return ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(src, length);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFJsonToOutput
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFJson &, jsonItem)
        , ZFMP_IN_OPT(const ZFJsonOutputFlags &, outputFlags, ZFJsonOutputFlagsDefault())
        ) {
    if(output && jsonItem) {
        output << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonToOutput_output(output, jsonItem, outputFlags, 0);
        return zftrue;
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFJsonToString
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFJson &, jsonItem)
        , ZFMP_IN(const ZFJsonOutputFlags &, outputFlags)
        ) {
    return ZFJsonToOutput(ZFOutputForString(ret), jsonItem, outputFlags);
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFJsonToString
        , ZFMP_IN(const ZFJson &, jsonItem)
        , ZFMP_IN(const ZFJsonOutputFlags &, outputFlags)
        ) {
    zfstring ret;
    ZFJsonToString(ret, jsonItem, outputFlags);
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

