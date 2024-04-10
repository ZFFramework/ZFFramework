#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFJson.h"

#define RAPIDJSON_DEFAULT_ALLOCATOR ::RAPIDJSON_NAMESPACE::MemoryPoolAllocator< ::RAPIDJSON_NAMESPACE::CrtAllocator >
#include "../_repo/rapidjson/document.h"
#include "../_repo/rapidjson/reader.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFJsonImpl_default_MemoryPoolHolder {
public:
    /*
     * as for rapidjson, string values are directly stored in owner document's buffer,
     * store ref count, init as 0, each child node refering to the doc would inc the ref count
     */
    zfindex docRefCount;
    ZFBuffer buf;
    rapidjson::Document implJsonDoc;
public:
    _ZFP_ZFJsonImpl_default_MemoryPoolHolder(void)
    : docRefCount(0)
    , buf()
    , implJsonDoc()
    {
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFJsonImpl_default, ZFJson, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("rapidjson")
public:
    virtual ZFJson jsonParse(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex size = zfindexMax()
            ) {
        ZFBuffer buf;
        buf.bufferCopy(src, (size != zfindexMax() ? size : zfslen(src)) * sizeof(zfchar));
        return this->jsonParse(buf);
    }
    virtual ZFJson jsonParse(ZF_IN const ZFInput &inputCallback) {
        ZFBuffer buf;
        ZFInputRead(buf, inputCallback);
        return this->jsonParse(buf);
    }
    virtual void jsonMemoryPoolRelease(
            ZF_IN void *token
            , ZF_IN const zfchar *value
            ) {
        _ZFP_ZFJsonImpl_default_MemoryPoolHolder *docHolder = (_ZFP_ZFJsonImpl_default_MemoryPoolHolder *)token;
        --(docHolder->docRefCount);
        if(docHolder->docRefCount == 0) {
            zfdelete(docHolder);
        }
    }
private:
    ZFJson jsonParse(ZF_IN_OUT ZFBuffer &buf) {
        if(buf.buffer() == zfnull) {
            return zfnull;
        }
        _ZFP_ZFJsonImpl_default_MemoryPoolHolder *docHolder = zfnew(_ZFP_ZFJsonImpl_default_MemoryPoolHolder);
        docHolder->buf = buf;
        docHolder->implJsonDoc.ParseInsitu<rapidjson::kParseNumbersAsStringsFlag>(buf.text());
        if(docHolder->implJsonDoc.HasParseError()) {
            zfdelete(docHolder);
            return zfnull;
        }
        ZFJson ret = this->jsonConvert(docHolder->implJsonDoc, docHolder);
        if(docHolder->docRefCount == 0) {
            zfdelete(docHolder);
        }
        return ret;
    }
private:
    ZFJson jsonConvert(
            ZF_IN const rapidjson::Value &implJsonItem
            , ZF_IN _ZFP_ZFJsonImpl_default_MemoryPoolHolder *docHolder
            ) {
        switch(implJsonItem.GetType()) {
            case rapidjson::kNullType:
                return zfnull;
            case rapidjson::kFalseType:
            case rapidjson::kTrueType:
            case rapidjson::kStringType:
            case rapidjson::kNumberType: {
                ZFJson jsonValue(ZFJsonType::e_JsonValue);
                ++(docHolder->docRefCount);
                const char *implString = implJsonItem.GetString();
                if(implString[implJsonItem.GetStringLength()] == '\0') {
                    this->jsonMemoryPool_jsonValue(jsonValue, implString, docHolder);
                }
                else {
                    jsonValue.jsonValue(zfstring(implString, implJsonItem.GetStringLength()));
                }
                return jsonValue;
            }
            case rapidjson::kArrayType: {
                ZFJson jsonArray(ZFJsonType::e_JsonArray);
                for(rapidjson::Value::ConstValueIterator it = implJsonItem.Begin(); it != implJsonItem.End(); ++it) {
                    ZFJson jsonChild = this->jsonConvert(*it, docHolder);
                    if(!jsonChild) {
                        return zfnull;
                    }
                    jsonArray.childAdd(jsonChild);
                }
                return jsonArray;
            }
            case rapidjson::kObjectType: {
                ZFJson jsonObject(ZFJsonType::e_JsonObject);
                for(rapidjson::Value::ConstMemberIterator it = implJsonItem.MemberBegin(); it != implJsonItem.MemberEnd(); ++it) {
                    ZFJson jsonChild = this->jsonConvert(it->value, docHolder);
                    if(!jsonChild) {
                        return zfnull;
                    }
                    ++(docHolder->docRefCount);
                    this->jsonMemoryPool_jsonItem(jsonObject, it->name.GetString(), docHolder, jsonChild);
                }
                return jsonObject;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return zfnull;
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFJsonImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFJsonImpl_default)

ZF_NAMESPACE_GLOBAL_END

