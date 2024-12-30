#include "zfstringW.h"
#include "protocol/ZFProtocolZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFStringEncoding)

// ============================================================
// conversion
zfbool zfstringToUTF8(
        ZF_OUT zfstring &result
        , ZF_IN const void *s
        , ZF_IN ZFStringEncoding srcEncoding
        ) {
    if(s == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF8(result, s, srcEncoding);
}

zfbool zfstringToUTF16(
        ZF_OUT zfstringW &result
        , ZF_IN const void *s
        , ZF_IN ZFStringEncoding srcEncoding
        ) {
    if(s == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF16(result, s, srcEncoding);
}
zfbool zfstringToUTF16BE(
        ZF_OUT zfstringW &result
        , ZF_IN const void *s
        , ZF_IN ZFStringEncoding srcEncoding
        ) {
    if(s == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF16BE(result, s, srcEncoding);
}

ZF_NAMESPACE_GLOBAL_END

