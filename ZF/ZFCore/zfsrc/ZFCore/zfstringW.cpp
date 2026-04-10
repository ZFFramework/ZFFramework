#include "zfstringW.h"
#include "protocol/ZFProtocolZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFStringEncoding)

// ============================================================
// conversion
zfbool zfstringToUTF8(
        ZF_IN_OUT zfstring &result
        , ZF_IN ZFStringEncoding srcEncoding
        , ZF_IN const void *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    if(src == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF8(result, srcEncoding, src, srcLen);
}

zfbool zfstringToUTF16(
        ZF_IN_OUT zfstringW &result
        , ZF_IN ZFStringEncoding srcEncoding
        , ZF_IN const void *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    if(src == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF16(result, srcEncoding, src, srcLen);
}
zfbool zfstringToUTF16BE(
        ZF_IN_OUT zfstringW &result
        , ZF_IN ZFStringEncoding srcEncoding
        , ZF_IN const void *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    if(src == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF16BE(result, srcEncoding, src, srcLen);
}

ZF_NAMESPACE_GLOBAL_END

