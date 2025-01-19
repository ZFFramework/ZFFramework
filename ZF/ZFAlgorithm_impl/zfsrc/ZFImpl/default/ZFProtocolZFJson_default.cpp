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
        zfstring buf;
        buf.assign(src, (size != zfindexMax() ? size : zfslen(src)));
        return this->jsonParse(buf, errorHint);
    }
    virtual ZFJson jsonParse(
            ZF_IN const ZFInput &inputCallback
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        zfstring buf;
        ZFInputRead(buf, inputCallback);
        return this->jsonParse(buf, errorHint);
    }
private:
    ZFJson jsonParse(
            ZF_IN_OUT zfstring &buf
            , ZF_OUT zfstring *errorHint
            ) {
        if(buf.buffer() == zfnull) {
            return zfnull;
        }
        rapidjson::Document implJsonDoc;
        implJsonDoc.ParseInsitu<rapidjson::kParseNumbersAsStringsFlag>(buf.zfunsafe_buffer());
        if(implJsonDoc.HasParseError()) {
            if(errorHint) {
                *errorHint += rapidjson::GetParseError_En(implJsonDoc.GetParseError());
                zfindex offset = (zfindex)implJsonDoc.GetErrorOffset();
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
                ZFJson value(ZFJsonType::e_Value);
                const char *implString = implJsonItem.GetString();
                value.value(zfstring(implString, implJsonItem.GetStringLength()));
                return value;
            }
            case rapidjson::kArrayType: {
                ZFJson jsonArray(ZFJsonType::e_Array);
                for(rapidjson::Value::ConstValueIterator it = implJsonItem.Begin(); it != implJsonItem.End(); ++it) {
                    ZFJson jsonChild = this->jsonConvert(*it);
                    if(!jsonChild) {
                        return zfnull;
                    }
                    jsonArray.child(jsonChild);
                }
                return jsonArray;
            }
            case rapidjson::kObjectType: {
                ZFJson jsonObject(ZFJsonType::e_Object);
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
                ZFCoreCriticalShouldNotGoHere();
                return zfnull;
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFJsonImpl_default)

ZF_NAMESPACE_GLOBAL_END

