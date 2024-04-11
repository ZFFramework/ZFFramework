#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFJson.h"

#define RAPIDJSON_DEFAULT_ALLOCATOR ::RAPIDJSON_NAMESPACE::MemoryPoolAllocator< ::RAPIDJSON_NAMESPACE::CrtAllocator >
#include "../_repo/rapidjson/document.h"
#include "../_repo/rapidjson/reader.h"
#include "../_repo/rapidjson/error/en.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFJsonImpl_default, ZFJson, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("rapidjson")
public:
    virtual ZFJson jsonParse(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex size = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        ZFBuffer buf;
        buf.bufferCopy(src, (size != zfindexMax() ? size : zfslen(src)) * sizeof(zfchar));
        return this->jsonParse(buf);
    }
    virtual ZFJson jsonParse(
            ZF_IN const ZFInput &inputCallback
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        ZFBuffer buf;
        ZFInputRead(buf, inputCallback);
        return this->jsonParse(buf);
    }
private:
    ZFJson jsonParse(
            ZF_IN_OUT ZFBuffer &buf
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        if(buf.buffer() == zfnull) {
            return zfnull;
        }
        rapidjson::Document implJsonDoc;
        implJsonDoc.ParseInsitu<rapidjson::kParseNumbersAsStringsFlag>(buf.text());
        if(implJsonDoc.HasParseError()) {
            if(errorHint) {
                *errorHint += rapidjson::GetParseError_En(implJsonDoc.GetParseError());
            }
            return zfnull;
        }
        return this->jsonConvert(implJsonDoc);
    }
private:
    ZFJson jsonConvert(ZF_IN const rapidjson::Value &implJsonItem) {
        switch(implJsonItem.GetType()) {
            case rapidjson::kNullType:
                return zfnull;
            case rapidjson::kFalseType:
            case rapidjson::kTrueType:
            case rapidjson::kStringType:
            case rapidjson::kNumberType: {
                ZFJson jsonValue(ZFJsonType::e_JsonValue);
                const char *implString = implJsonItem.GetString();
                jsonValue.jsonValue(zfstring(implString, implJsonItem.GetStringLength()));
                return jsonValue;
            }
            case rapidjson::kArrayType: {
                ZFJson jsonArray(ZFJsonType::e_JsonArray);
                for(rapidjson::Value::ConstValueIterator it = implJsonItem.Begin(); it != implJsonItem.End(); ++it) {
                    ZFJson jsonChild = this->jsonConvert(*it);
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
                    ZFJson jsonChild = this->jsonConvert(it->value);
                    if(!jsonChild) {
                        return zfnull;
                    }
                    jsonObject.attr(it->name.GetString(), jsonChild);
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

