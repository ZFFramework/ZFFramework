#include "ZFJson.h"
#include "protocol/ZFProtocolZFJson.h"

#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFJsonType)

// ============================================================
zfbool ZFJsonOutputToken::operator == (ZF_IN ZFJsonOutputToken const &ref) const
{
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
zfbool ZFJsonOutputFlags::operator == (ZF_IN ZFJsonOutputFlags const &ref) const
{
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

// ============================================================
ZFEXPORT_VAR_READONLY_DEFINE(ZFJsonOutputFlags, ZFJsonOutputFlagsDefault, ZFJsonOutputFlags())

static const ZFJsonOutputFlags &_ZFP_ZFJsonOutputFlagsTrimInit(void)
{
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
static void _ZFP_ZFJsonItemToOutput_outputIndent(ZF_IN_OUT const ZFOutput &output,
                                                 ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                 ZF_IN zfindex indentLevel)
{
    for(zfindex i = 0; i < indentLevel; ++i)
    {
        output << outputFlags.jsonToken.jsonIndentToken;
    }
}
static void _ZFP_ZFJsonItemToOutput_output(ZF_IN_OUT const ZFOutput &output,
                                           ZF_IN const ZFJsonItem &jsonItem,
                                           ZF_IN const ZFJsonOutputFlags &outputFlags,
                                           ZF_IN zfindex indentLevel);
static void _ZFP_ZFJsonItemToOutput_output_JsonValue(ZF_IN_OUT const ZFOutput &output,
                                                     ZF_IN const ZFJsonItem &jsonValue,
                                                     ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                     ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonValueTagLeft;
    ZFJsonEscapeCharEncode(output, jsonValue.jsonValue());
    output << outputFlags.jsonToken.jsonValueTagRight;
}
static void _ZFP_ZFJsonItemToOutput_output_JsonObject(ZF_IN_OUT const ZFOutput &output,
                                                      ZF_IN const ZFJsonItem &jsonObject,
                                                      ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                      ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonObjectTagLeft;
    zfbool first = zftrue;
    for(zfiterator it = jsonObject.attrIter(); jsonObject.attrIterValid(it); jsonObject.attrIterNext(it))
    {
        if(outputFlags.jsonObjectAddNewLineForContent)
        {
            if(first)
            {
                first = zffalse;
            }
            else
            {
                output << outputFlags.jsonToken.jsonSeparatorToken;
            }
            output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
            _ZFP_ZFJsonItemToOutput_outputIndent(output, outputFlags, indentLevel + 1);
        }
        else
        {
            if(first)
            {
                first = zffalse;
            }
            else
            {
                output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
            }
        }
        output
            << outputFlags.jsonToken.jsonKeyTagLeft
            << jsonObject.attrIterKey(it)
            << outputFlags.jsonToken.jsonKeyTagRight;
        output << outputFlags.jsonToken.jsonValueSeparatorToken;
        _ZFP_ZFJsonItemToOutput_output(output, jsonObject.attrIterValue(it), outputFlags, indentLevel + 1);
    }
    if(outputFlags.jsonObjectAddNewLineForContent
        && !(outputFlags.jsonObjectTagInSameLineIfNoContent && jsonObject.attrCount() == 0))
    {
        output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonItemToOutput_outputIndent(output, outputFlags, indentLevel);
    }
    output << outputFlags.jsonToken.jsonObjectTagRight;
}
static void _ZFP_ZFJsonItemToOutput_output_JsonArray(ZF_IN_OUT const ZFOutput &output,
                                                     ZF_IN const ZFJsonItem &jsonArray,
                                                     ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                     ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonArrayTagLeft;
    for(zfindex i = 0; i < jsonArray.childCount(); ++i)
    {
        if(outputFlags.jsonArrayAddNewLineForContent)
        {
            if(i != 0)
            {
                if(outputFlags.jsonArrayContentTagInSameLine)
                {
                    output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
                }
                else
                {
                    output << outputFlags.jsonToken.jsonSeparatorToken;
                }
            }

            if(!outputFlags.jsonArrayContentTagInSameLine)
            {
                output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
                _ZFP_ZFJsonItemToOutput_outputIndent(output, outputFlags, indentLevel);
            }
            _ZFP_ZFJsonItemToOutput_output(output, jsonArray.childAtIndex(i), outputFlags, indentLevel + 1);
        }
        else
        {
            if(i != 0)
            {
                output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
            }
            _ZFP_ZFJsonItemToOutput_output(output, jsonArray.childAtIndex(i), outputFlags, indentLevel + 1);
        }
    }
    if(outputFlags.jsonArrayAddNewLineForContent
        && !(outputFlags.jsonArrayTagInSameLineIfNoContent && jsonArray.childCount() == 0))
    {
        output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonItemToOutput_outputIndent(output, outputFlags, indentLevel);
    }
    output << outputFlags.jsonToken.jsonArrayTagRight;
}
static void _ZFP_ZFJsonItemToOutput_output(ZF_IN_OUT const ZFOutput &output,
                                           ZF_IN const ZFJsonItem &jsonItem,
                                           ZF_IN const ZFJsonOutputFlags &outputFlags,
                                           ZF_IN zfindex indentLevel)
{
    switch(jsonItem.jsonType())
    {
        case ZFJsonType::e_JsonValue:
            _ZFP_ZFJsonItemToOutput_output_JsonValue(output, jsonItem, outputFlags, indentLevel);
            break;
        case ZFJsonType::e_JsonObject:
            _ZFP_ZFJsonItemToOutput_output_JsonObject(output, jsonItem, outputFlags, indentLevel);
            break;
        case ZFJsonType::e_JsonArray:
            _ZFP_ZFJsonItemToOutput_output_JsonArray(output, jsonItem, outputFlags, indentLevel);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
zfclassLikePOD _ZFP_ZFJsonMemoryPoolString
{
private:
    void *_token;
    union {
        zfchar *_s;
        const zfchar *_sInPool;
    } _s;

private:
    _ZFP_ZFJsonMemoryPoolString(ZF_IN const _ZFP_ZFJsonMemoryPoolString &ref);
    _ZFP_ZFJsonMemoryPoolString &operator = (ZF_IN const _ZFP_ZFJsonMemoryPoolString &ref);
public:
    _ZFP_ZFJsonMemoryPoolString(void)
    : _token(zfnull)
    , _s()
    {
    }
    _ZFP_ZFJsonMemoryPoolString(ZF_IN const zfchar *s)
    : _token(zfnull)
    , _s()
    {
        _s._s = zfsCopy(s);
    }
    _ZFP_ZFJsonMemoryPoolString(ZF_IN const zfchar *s, ZF_IN void *token)
    : _token(token)
    , _s()
    {
        _s._sInPool = s;
    }
    ~_ZFP_ZFJsonMemoryPoolString(void)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFJson)->jsonMemoryPoolRelease(_token, _s._sInPool);
        }
        else
        {
            zffree(_s._s);
        }
    }

public:
    void value(ZF_IN const zfchar *value)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFJson)->jsonMemoryPoolRelease(_token, _s._sInPool);
            _token = zfnull;
            _s._s = zfnull;
        }
        zfsChange(_s._s, value);
    }
    void value(ZF_IN const zfchar *value, ZF_IN void *token)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFJson)->jsonMemoryPoolRelease(_token, _s._sInPool);
            _token = zfnull;
            _s._s = zfnull;
        }
        else
        {
            zffree(_s._s);
        }
        _token = token;
        _s._sInPool = value;
    }
    inline const zfchar *value(void) const
    {
        return _s._sInPool;
    }
};
zfclassNotPOD _ZFP_ZFJsonItemData
{
public:
    _ZFP_ZFJsonMemoryPoolString jsonKey;
    ZFJsonItem jsonItem;
public:
    _ZFP_ZFJsonItemData(ZF_IN const zfchar *jsonKey,
                        ZF_IN const ZFJsonItem &jsonItem)
    : jsonKey(jsonKey)
    , jsonItem(jsonItem)
    {
    }
    _ZFP_ZFJsonItemData(ZF_IN const zfchar *jsonKey,
                        ZF_IN void *jsonKeyToken,
                        ZF_IN const ZFJsonItem &jsonItem)
    : jsonKey(jsonKey, jsonKeyToken)
    , jsonItem(jsonItem)
    {
    }
};
zfclassNotPOD _ZFP_ZFJsonItemPrivate
{
public:
    zfuint refCount;
    ZFJsonTypeEnum jsonType;
    _ZFP_ZFJsonMemoryPoolString jsonValue;
    zfstldeque<_ZFP_ZFJsonItemData *> jsonItemMap;
    zfstldeque<ZFJsonItem> jsonArrayChildren;

public:
    explicit _ZFP_ZFJsonItemPrivate(ZF_IN ZFJsonTypeEnum jsonType)
    : refCount(1)
    , jsonType(jsonType)
    , jsonValue()
    , jsonItemMap()
    , jsonArrayChildren()
    {
    }
    ~_ZFP_ZFJsonItemPrivate(void)
    {
        this->jsonItemMapClear();
    }

public:
    void jsonItemMapClear(void)
    {
        for(zfstlsize i = 0; i < this->jsonItemMap.size(); ++i)
        {
            zfdelete(this->jsonItemMap[i]);
        }
        this->jsonItemMap.clear();
    }

    void jsonItemMapAdd(ZF_IN _ZFP_ZFJsonItemData *jsonItemData)
    {
        if(this->jsonItemMap.empty())
        {
            this->jsonItemMap.push_back(jsonItemData);
            return ;
        }
        zfstlsize l = 0;
        zfstlsize r = this->jsonItemMap.size() - 1;
        zfint cmp = 0;
        do
        {
            zfstlsize i = (l + r) / 2;
            _ZFP_ZFJsonItemData *t = this->jsonItemMap[i];
            cmp = zfscmp(jsonItemData->jsonKey.value(), t->jsonKey.value());
            if(cmp == 0)
            {
                l = i;
                break;
            }
            if(l == r)
            {
                break;
            }

            if(cmp < 0)
            {
                if(i == l)
                {
                    break;
                }
                r = i;
            }
            else
            {
                if(l == i || i + 1 == r)
                {
                    l = i + 1;
                }
                else
                {
                    l = i;
                }
            }
        } while(zftrue);

        if(cmp == 0)
        {
            zfdelete(this->jsonItemMap[l]);
            this->jsonItemMap[l] = jsonItemData;
        }
        else
        {
            if(cmp > 0)
            {
                ++l;
            }
            this->jsonItemMap.insert(this->jsonItemMap.begin() + l, jsonItemData);
        }
    }

    zfstlsize jsonItemMapIndex(ZF_IN const zfchar *key)
    {
        if(key == zfnull || this->jsonItemMap.empty())
        {
            return (zfstlsize)-1;
        }
        zfstlsize l = 0;
        zfstlsize r = this->jsonItemMap.size() - 1;
        zfint cmp = -1;
        do
        {
            zfstlsize i = (l + r) / 2;
            _ZFP_ZFJsonItemData *t = this->jsonItemMap[i];
            cmp = zfscmp(key, t->jsonKey.value());
            if(cmp == 0)
            {
                return i;
            }
            if(l == r)
            {
                break;
            }

            if(cmp < 0)
            {
                if(i == l)
                {
                    break;
                }
                r = i;
            }
            else
            {
                if(l == i || i + 1 == r)
                {
                    l = i + 1;
                }
                else
                {
                    l = i;
                }
            }
        } while(zftrue);
        return (zfstlsize)-1;
    }
};

// ============================================================
// ZFJsonItem
ZFJsonItem::ZFJsonItem(ZF_IN _ZFP_ZFJsonItemPrivate *ref)
: d(ref)
{
    if(d)
    {
        ++(d->refCount);
    }
}
ZFJsonItem::ZFJsonItem(void)
: d(zfnull)
{
}
ZFJsonItem::ZFJsonItem(ZF_IN const zfnullT &dummy)
: d(zfnull)
{
}
ZFJsonItem::ZFJsonItem(ZF_IN ZFJsonTypeEnum jsonType)
{
    if(jsonType != ZFJsonType::e_JsonNull)
    {
        d = zfnew(_ZFP_ZFJsonItemPrivate, jsonType);
    }
    else
    {
        d = zfnull;
    }
}
ZFJsonItem::ZFJsonItem(ZF_IN const ZFJsonItem &ref)
: d(ref.d)
{
    if(d)
    {
        ++(d->refCount);
    }
}
ZFJsonItem::~ZFJsonItem(void)
{
    if(d)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
    }
}

ZFJsonItem &ZFJsonItem::operator = (ZF_IN const ZFJsonItem &ref)
{
    _ZFP_ZFJsonItemPrivate *dTmp = d;
    d = ref.d;
    if(d)
    {
        ++(d->refCount);
    }
    if(dTmp)
    {
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            zfdelete(dTmp);
        }
    }
    return *this;
}
ZFJsonItem &ZFJsonItem::operator = (ZF_IN const zfnullT &dummy)
{
    if(d)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
        d = zfnull;
    }
    return *this;
}
zfbool ZFJsonItem::operator == (ZF_IN const ZFJsonItem &ref) const
{
    return (d == ref.d);
}

// ============================================================
void ZFJsonItem::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ZFJsonItemToString(ret, *this, ZFJsonOutputFlagsTrim());
}

zfindex ZFJsonItem::objectRetainCount(void) const
{
    return (d ? d->refCount : 0);
}

// ============================================================
void ZFJsonItem::_ZFP_ZFJsonItem_jsonType(ZF_IN ZFJsonTypeEnum jsonType)
{
    if(jsonType != ZFJsonType::e_JsonNull)
    {
        if(d)
        {
            d->jsonType = jsonType;
        }
        else
        {
            d = zfnew(_ZFP_ZFJsonItemPrivate, jsonType);
        }
    }
}
ZFJsonTypeEnum ZFJsonItem::jsonType(void) const
{
    return (d ? d->jsonType : ZFJsonType::e_JsonNull);
}

ZFJsonItem ZFJsonItem::jsonCloneTree(void) const
{
    ZFJsonItem ret(this->jsonType());
    switch(this->jsonType())
    {
        case ZFJsonType::e_JsonNull:
            break;
        case ZFJsonType::e_JsonValue:
            ret.jsonValue(this->jsonValue());
            break;
        case ZFJsonType::e_JsonObject:
            for(zfiterator it = this->attrIter(); this->attrIterValid(it); this->attrIterNext(it))
            {
                ret.attr(this->attrIterKey(it), this->attrIterValue(it).jsonCloneTree());
            }
            break;
        case ZFJsonType::e_JsonArray:
            for(zfindex i = 0; i < this->childCount(); ++i)
            {
                ret.childAdd(this->childAtIndex(i).jsonCloneTree());
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    return ret;
}

void ZFJsonItem::_ZFP_ZFJson_jsonMemoryPool_jsonValue(ZF_IN const zfchar *value, ZF_IN void *token)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFJsonItemPrivate, ZFJsonType::e_JsonValue);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonValue);
    d->jsonValue.value(value, token);
}
void ZFJsonItem::_ZFP_ZFJson_jsonMemoryPool_jsonItem(ZF_IN const zfchar *key, ZF_IN void *token, ZF_IN const ZFJsonItem &jsonItem)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFJsonItemPrivate, ZFJsonType::e_JsonObject);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    zfCoreAssert(jsonItem);
    if(key != zfnull)
    {
        d->jsonItemMapAdd(zfnew(_ZFP_ZFJsonItemData, key, token, jsonItem));
    }
}

// ============================================================
// for value type
ZFJsonItem &ZFJsonItem::jsonValue(ZF_IN const zfchar *value)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFJsonItemPrivate, ZFJsonType::e_JsonValue);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonValue);
    d->jsonValue.value(value);
    return *this;
}
const zfchar *ZFJsonItem::jsonValue(void) const
{
    return d ? d->jsonValue.value() : zfnull;
}

// ============================================================
// for object type
ZFJsonItem &ZFJsonItem::attr(ZF_IN const zfchar *key,
                             ZF_IN const zfchar *value)
{
    if(value == zfnull)
    {
        this->attrRemove(key);
    }
    else
    {
        ZFJsonItem jsonValue(ZFJsonType::e_JsonValue);
        jsonValue.jsonValue(value);
        this->attr(key, jsonValue);
    }
    return *this;
}
ZFJsonItem &ZFJsonItem::attr(ZF_IN const zfchar *key,
                             ZF_IN const ZFJsonItem &jsonItem)
{
    if(key == zfnull)
    {
        return *this;
    }
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFJsonItemPrivate, ZFJsonType::e_JsonObject);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    if(jsonItem)
    {
        d->jsonItemMapAdd(zfnew(_ZFP_ZFJsonItemData, key, jsonItem));
    }
    else
    {
        this->attrRemove(key);
    }
    return *this;
}
ZFJsonItem ZFJsonItem::attr(ZF_IN const zfchar *key) const
{
    if(d)
    {
        zfstlsize index = d->jsonItemMapIndex(key);
        if(index != (zfstlsize)-1)
        {
            return d->jsonItemMap[index]->jsonItem;
        }
    }
    return ZFJsonItem();
}
const zfchar *ZFJsonItem::attrValue(ZF_IN const zfchar *key) const
{
    if(d)
    {
        zfstlsize index = d->jsonItemMapIndex(key);
        if(index != (zfstlsize)-1)
        {
            return d->jsonItemMap[index]->jsonItem.jsonValue();
        }
    }
    return zfnull;
}

ZFJsonItem &ZFJsonItem::attrRemove(ZF_IN const zfchar *key)
{
    if(d)
    {
        zfstlsize index = d->jsonItemMapIndex(key);
        if(index != (zfstlsize)-1)
        {
            zfdelete(d->jsonItemMap[index]);
            d->jsonItemMap.erase(d->jsonItemMap.begin() + index);
        }
    }
    return *this;
}

ZFJsonItem &ZFJsonItem::attrRemoveAll(void)
{
    if(d)
    {
        d->jsonItemMapClear();
    }
    return *this;
}

zfindex ZFJsonItem::attrCount(void) const
{
    return d ? (zfindex)d->jsonItemMap.size() : 0;
}

static void _ZFP_ZFJsonItem_iterDeleteCallback(void *data)
{
    zfdelete(ZFCastStatic(zfstlsize *, data));
}
static void *_ZFP_ZFJsonItem_iterCopyCallback(void *data)
{
    return zfnew(zfstlsize,
        *ZFCastStatic(zfstlsize *, data));
}
zfiterator ZFJsonItem::attrIter(void) const
{
    return zfiterator(
        zfnew(zfstlsize, 0),
        _ZFP_ZFJsonItem_iterDeleteCallback,
        _ZFP_ZFJsonItem_iterCopyCallback);
}

zfiterator ZFJsonItem::attrIterFind(ZF_IN const zfchar *key) const
{
    return zfiterator(
        zfnew(zfstlsize, d ? d->jsonItemMapIndex(key) : -1),
        _ZFP_ZFJsonItem_iterDeleteCallback,
        _ZFP_ZFJsonItem_iterCopyCallback);
}

zfbool ZFJsonItem::attrIterValid(ZF_IN const zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    return (data != zfnull && d && *data < d->jsonItemMap.size());
}

void ZFJsonItem::attrIterNext(ZF_IN_OUT zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && d && *data < d->jsonItemMap.size())
    {
        ++(*data);
    }
}

const zfchar *ZFJsonItem::attrIterKey(ZF_IN const zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && d && *data < d->jsonItemMap.size())
    {
        return d->jsonItemMap[*data]->jsonKey.value();
    }
    else
    {
        return zfnull;
    }
}
ZFJsonItem ZFJsonItem::attrIterValue(ZF_IN const zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && d && *data < d->jsonItemMap.size())
    {
        return d->jsonItemMap[*data]->jsonItem;
    }
    else
    {
        return ZFJsonItem();
    }
}

void ZFJsonItem::attrIterValue(ZF_IN_OUT zfiterator &it, ZF_IN const ZFJsonItem &jsonItem)
{
    if(!jsonItem)
    {
        this->attrIterRemove(it);
        return ;
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && d && *data < d->jsonItemMap.size())
    {
        _ZFP_ZFJsonItemData *jsonItemData = d->jsonItemMap[*data];
        jsonItemData->jsonItem = jsonItem;
    }
}

void ZFJsonItem::attrIterRemove(ZF_IN_OUT zfiterator &it)
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && d && *data < d->jsonItemMap.size())
    {
        zfdelete(d->jsonItemMap[*data]);
        d->jsonItemMap.erase(d->jsonItemMap.begin() + (*data));
    }
}

// ============================================================
zfindex ZFJsonItem::childCount(void) const
{
    return d ? (zfindex)d->jsonArrayChildren.size() : 0;
}
ZFJsonItem ZFJsonItem::childAtIndex(ZF_IN zfindex index) const
{
    if(index >= this->childCount())
    {
        zfCoreCriticalIndexOutOfRange(index, this->childCount());
    }
    return d->jsonArrayChildren[index];
}

ZFJsonItem &ZFJsonItem::childAdd(ZF_IN const zfchar *jsonValue,
                                 ZF_IN_OPT zfindex atIndex /* = zfindexMax() */)
{
    return this->childAdd(ZFJsonItem(ZFJsonType::e_JsonValue).jsonValue(jsonValue), atIndex);
}
ZFJsonItem &ZFJsonItem::childAdd(ZF_IN const ZFJsonItem &jsonObject,
                                 ZF_IN_OPT zfindex atIndex /* = zfindexMax() */)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFJsonItemPrivate, ZFJsonType::e_JsonArray);
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonArray);
    if(atIndex == zfindexMax())
    {
        atIndex = (zfindex)d->jsonArrayChildren.size();
    }
    else if(atIndex > (zfindex)d->jsonArrayChildren.size())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, (zfindex)(d->jsonArrayChildren.size() + 1));
        return *this;
    }
    zfCoreAssertWithMessage(jsonObject, "add null object");
    d->jsonArrayChildren.insert(d->jsonArrayChildren.begin() + atIndex, jsonObject);
    return *this;
}

ZFJsonItem &ZFJsonItem::childRemoveAtIndex(ZF_IN zfindex index)
{
    if(index >= this->childCount())
    {
        zfCoreCriticalIndexOutOfRange(index, this->childCount());
    }
    d->jsonArrayChildren.erase(d->jsonArrayChildren.begin() + index);
    return *this;
}
ZFJsonItem &ZFJsonItem::childRemoveAll(void)
{
    if(d)
    {
        d->jsonArrayChildren.clear();
    }
    return *this;
}
zfindex ZFJsonItem::childFind(ZF_IN const ZFJsonItem &jsonObject) const
{
    if(d)
    {
        for(zfstlsize i = 0; i < d->jsonArrayChildren.size(); ++i)
        {
            if(jsonObject == d->jsonArrayChildren[i])
            {
                return (zfindex)i;
            }
        }
    }
    return zfindexMax();
}

ZFJsonItem::operator zfstring (void) const
{
    return ZFJsonItemToString(*this);
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFJsonItem, ZFJsonItem, {
        v = ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(src, srcLen);
        return v;
    }, {
        return ZFJsonItemToOutput(ZFOutputForString(s), v);
    })

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        invokerObject->to<v_ZFJsonItem *>()->zfv._ZFP_ZFJsonItem_jsonType(jsonType);
    }, v_ZFJsonItem
    , ZFMP_IN(ZFJsonTypeEnum, jsonType)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, objectInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfindex, objectRetainCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, ZFJsonTypeEnum, jsonType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfbool, jsonTypeValid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, ZFJsonItem, jsonCloneTree)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem &, jsonValue, ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, const zfchar *, jsonValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, ZFJsonItem &, attr, ZFMP_IN(const zfchar *, key), ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, ZFJsonItem &, attr, ZFMP_IN(const zfchar *, key), ZFMP_IN(const ZFJsonItem &, jsonItem))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem, attr, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, const zfchar *, attrValue, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem &, attrRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, ZFJsonItem &, attrRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfindex, attrCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfiterator, attrIter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, zfiterator, attrIterFind, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, zfbool, attrIterValid, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, attrIterNext, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, const zfchar *, attrIterKey, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem, attrIterValue, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, void, attrIterValue, ZFMP_IN_OUT(zfiterator &, it), ZFMP_IN(const ZFJsonItem &, jsonItem))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, attrIterRemove, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfindex, childCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem, childAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, ZFJsonItem &, childAdd, ZFMP_IN(const zfchar *, jsonValue), ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, ZFJsonItem &, childAdd, ZFMP_IN(const ZFJsonItem &, jsonObject), ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem &, childRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, ZFJsonItem &, childRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, zfindex, childFind, ZFMP_IN(const ZFJsonItem &, jsonObject))

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(ZFJsonItem, ZFJsonObject) {return ZFJsonItem(ZFJsonType::e_JsonObject);}
ZFMETHOD_FUNC_DEFINE_0(ZFJsonItem, ZFJsonArray) {return ZFJsonItem(ZFJsonType::e_JsonArray);}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFJsonItem, ZFJsonItemFromInput, ZFMP_IN(const ZFInput &, input))
{
    return ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(input);
}
ZFMETHOD_FUNC_DEFINE_2(ZFJsonItem, ZFJsonItemFromString,
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, length, zfindexMax()))
{
    return ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(src, length);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFJsonItemToOutput,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFJsonItem &, jsonItem),
                       ZFMP_IN_OPT(const ZFJsonOutputFlags &, outputFlags, ZFJsonOutputFlagsDefault()))
{
    if(output && jsonItem)
    {
        output << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonItemToOutput_output(output, jsonItem, outputFlags, 0);
        return zftrue;
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFJsonItemToString,
                       ZFMP_IN_OUT(zfstring &, ret),
                       ZFMP_IN(const ZFJsonItem &, jsonItem),
                       ZFMP_IN(const ZFJsonOutputFlags &, outputFlags))
{
    return ZFJsonItemToOutput(ZFOutputForString(ret), jsonItem, outputFlags);
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFJsonItemToString,
                       ZFMP_IN(const ZFJsonItem &, jsonItem),
                       ZFMP_IN(const ZFJsonOutputFlags &, outputFlags))
{
    zfstring ret;
    ZFJsonItemToString(ret, jsonItem, outputFlags);
    return ret;
}

// ============================================================
// escape chars
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharEncode,
                       ZFMP_OUT(zfstring &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFJsonEscapeCharEncode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharEncode,
                       ZFMP_OUT(const ZFOutput &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFPROTOCOL_ACCESS(ZFJson)->jsonEscapeCharEncode(dst, src, count);
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharDecode,
                       ZFMP_OUT(zfstring &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFJsonEscapeCharDecode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharDecode,
                       ZFMP_OUT(const ZFOutput &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFPROTOCOL_ACCESS(ZFJson)->jsonEscapeCharDecode(dst, src, count);
}

ZF_NAMESPACE_GLOBAL_END

