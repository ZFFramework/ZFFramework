#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFString.h"
#include "ZFImpl/tools/UTFCodeUtil/UTFCodeUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFStringImpl_default, ZFString, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFFramework:zfstring")
public:
    virtual zfbool toUTF8(
            ZF_OUT zfstring &result
            , ZF_IN ZFStringEncoding srcEncoding
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            ) {
        switch(srcEncoding) {
            case v_ZFStringEncoding::e_UTF8:
                result.append(src, srcLen);
                return zftrue;
            case v_ZFStringEncoding::e_UTF16LE: {
                size_t srcLenTmp = (srcLen != zfindexMax() ? (size_t)srcLen : _len((const wchar_t *)src));
                size_t bufLen = UTFCodeUtil::UTF16ToUTF8(zfnull, (const wchar_t *)src, srcLenTmp);
                if(bufLen == (size_t)-1) {
                    return zffalse;
                }
                if(!result.capacity(result.length() + bufLen)) {
                    return zffalse;
                }
                UTFCodeUtil::UTF16ToUTF8(result.zfunsafe_buffer() + result.length(), (const wchar_t *)src, srcLenTmp);
                result.zfunsafe_length(result.length() + bufLen - 1);
                return zftrue;
            }
            case v_ZFStringEncoding::e_UTF16BE: {
                size_t srcLenTmp = (srcLen != zfindexMax() ? (size_t)srcLen : _len((const wchar_t *)src));
                zfstringW buf;
                if(!_utf16Swap(buf, (const wchar_t *)src, srcLenTmp)) {
                    return zffalse;
                }
                return this->toUTF8(result, v_ZFStringEncoding::e_UTF16LE, buf.buffer(), buf.length());
            }
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    virtual zfbool toUTF16(
            ZF_OUT zfstringW &result
            , ZF_IN ZFStringEncoding srcEncoding
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            ) {
        switch(srcEncoding) {
            case v_ZFStringEncoding::e_UTF8: {
                size_t srcLenTmp = (srcLen != zfindexMax() ? (size_t)srcLen : _len((const char *)src));
                size_t bufLen = UTFCodeUtil::UTF8ToUTF16(zfnull, (const char *)src, srcLenTmp);
                if(bufLen == (size_t)-1) {
                    return zffalse;
                }
                if(!result.capacity(result.length() + bufLen)) {
                    return zffalse;
                }
                UTFCodeUtil::UTF8ToUTF16(result.zfunsafe_buffer() + result.length(), (const char *)src, srcLenTmp);
                result.zfunsafe_length(result.length() + bufLen - 1);
                return zftrue;
            }
            case v_ZFStringEncoding::e_UTF16LE:
                result.append((const zfcharW *)src, srcLen);
                return zftrue;
            case v_ZFStringEncoding::e_UTF16BE: {
                return _utf16Swap(
                        result
                        , (const wchar_t *)src
                        , srcLen != zfindexMax() ? (size_t)srcLen : _len((const wchar_t *)src)
                        );
            }
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    virtual zfbool toUTF16BE(
            ZF_OUT zfstringW &result
            , ZF_IN ZFStringEncoding srcEncoding
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            ) {
        switch(srcEncoding) {
            case v_ZFStringEncoding::e_UTF8: {
                size_t srcLenTmp = (srcLen != zfindexMax() ? (size_t)srcLen : _len((const char *)src));
                size_t bufLen = UTFCodeUtil::UTF8ToUTF16(zfnull, (const char *)src, srcLenTmp);
                if(bufLen == (size_t)-1) {
                    return zffalse;
                }
                if(!result.capacity(result.length() + bufLen)) {
                    return zffalse;
                }
                UTFCodeUtil::UTF8ToUTF16(result.zfunsafe_buffer() + result.length(), (const char *)src, srcLenTmp);
                UTFCodeUtil::UTF16ToUTF16BE(result.zfunsafe_buffer() + result.length(), result.zfunsafe_buffer() + result.length(), bufLen - 1);
                result.zfunsafe_length(result.length() + bufLen - 1);
                return zftrue;
            }
            case v_ZFStringEncoding::e_UTF16LE: {
                return _utf16Swap(
                        result
                        , (const wchar_t *)src
                        , srcLen != zfindexMax() ? (size_t)srcLen : _len((const wchar_t *)src)
                        );
            }
            case v_ZFStringEncoding::e_UTF16BE:
                result.append((const zfcharW *)src, srcLen);
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
private:
    static size_t _len(ZF_IN const char *src) {
        size_t len = 0;
        while(*src++) {++len;}
        return len;
    }
    static size_t _len(ZF_IN const wchar_t *src) {
        size_t len = 0;
        while(*src++) {++len;}
        return len;
    }
    static zfbool _utf16Swap(ZF_OUT zfstringW ret, ZF_IN const wchar_t *src, ZF_IN size_t srcLen) {
        if(!ret.capacity(ret.length() + srcLen)) {
            return zffalse;
        }
        UTFCodeUtil::UTF16ToUTF16BE(ret.zfunsafe_buffer() + ret.length(), src, srcLen);
        ret.zfunsafe_length(ret.length() + srcLen);
        return zftrue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFStringImpl_default)

ZF_NAMESPACE_GLOBAL_END

