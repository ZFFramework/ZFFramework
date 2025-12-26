/**
 * @file ZFIOCallback_output.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_output_h_
#define _ZFI_ZFIOCallback_output_h_

#include "ZFIOCallback_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFOutput
/**
 * @brief general output callback
 *
 * proto type:
 * @code
 *   zfindex output(
 *           ZF_IN const void *src
 *           , ZF_IN_OPT zfindex count = zfindexMax()
 *           );
 * @endcode
 *
 * params:
 * -  (const void *) output buffer
 * -  (zfindex) byte size to write, or zfindexMax() to calculate by src
 *  (at this case, src must be NULL-terminated zfchar string)
 *
 * return:
 * -  byte size written to output,
 *   return a value less than count indicates error state
 *
 * ADVANCED:\n
 * for implementations, see #ZFCallbackTagKeyword_ioOwner if need support seek
 */
ZFCALLBACK_DECLARE_BEGIN(ZFLIB_ZFCore, ZFOutput, ZFIOCallback)
public:
    /** @brief see #ZFOutput */
    inline zfindex execute(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) const {
        if(this->valid()) {
            return ZFCallback::executeExact<zfindex, const void *, zfindex>(src, count);
        }
        else {
            return 0;
        }
    }
    /** @brief see #ZFOutput */
    inline zfindex operator () (
            ZF_IN const void *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) const {
        if(this->valid()) {
            return ZFCallback::executeExact<zfindex, const void *, zfindex>(src, count);
        }
        else {
            return 0;
        }
    }
    /** @brief see #ZFOutput */
    inline const ZFOutput &output(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            , ZF_OUT_OPT zfindex *result = zfnull
            ) const {
        if(result != zfnull) {
            *result = this->execute(src, count);
        }
        else {
            this->execute(src, count);
        }
        return *this;
    }
_ZFP_ZFCALLBACK_DECLARE_END(ZFLIB_ZFCore, ZFOutput, ZFIOCallback)

/** @cond ZFPrivateDoc */
template<typename T>
inline const ZFOutput &operator << (ZF_IN_OUT const ZFOutput &o, ZF_IN T const &v) {
    zfstring s;
    zftToStringT(s, v);
    o.output(s.cString(), s.length());
    return o;
}
inline const ZFOutput &operator << (ZF_IN_OUT const ZFOutput &o, ZF_IN zfstring const &s) {
    o.output(s.cString(), s.length());
    return o;
}
inline const ZFOutput &operator << (ZF_IN_OUT const ZFOutput &o, ZF_IN const zfchar * const &s) {
    if(s == zfnull) {
        o.output(ZFTOKEN_zfnull);
    }
    else {
        o.output(s);
    }
    return o;
}
inline const ZFOutput &operator << (ZF_IN_OUT const ZFOutput &o, ZF_IN zfchar * const &s) {
    if(s == zfnull) {
        o.output(ZFTOKEN_zfnull);
    }
    else {
        o.output(s);
    }
    return o;
}
/** @endcond */

// ============================================================
// common output callbacks
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback callbackType="ZFOutputDummy">
 *       // dummy input has no contents
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFOutputDummy "ZFOutputDummy"
/**
 * @brief a dummy output that always output nothing with success state
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputDummy(void);

/**
 * @brief create a output callback to output to a zfstring
 *
 * @note you must ensure the string to output is alive while the callback is still under use
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputForString(ZF_IN_OUT zfstring &s);
/**
 * @brief create a output callback to output to a buffer
 *
 * params:
 * -  (void *) buffer to write to
 * -  (zfindex) max buffer size or zfindexMax() for no limit (you must make sure buffer is enough),
 *   if autoAppendNullToken, maxCount should contain the extra '\0' size
 * -  (zfbool) whether auto append '\0' to tail each time write
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputForBufferUnsafe(
        ZF_IN void *buf
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT zfbool autoAppendNullToken = zftrue
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_output_h_

