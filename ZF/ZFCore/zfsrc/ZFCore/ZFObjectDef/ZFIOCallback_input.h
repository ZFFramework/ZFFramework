/**
 * @file ZFIOCallback_input.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_input_h_
#define _ZFI_ZFIOCallback_input_h_

#include "ZFIOCallback_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFInput
/**
 * @brief general input callback
 *
 * proto type:
 * @code
 *   zfindex input(
 *           ZF_OUT void *buf
 *           , ZF_IN zfindex count
 *           );
 * @endcode
 *
 * params:
 * -  (void *) buffer to write to
 * -  (zfindex) max byte size to read (excluding '\0')
 *
 * return:
 * -  (zfindex) if buffer is null,
 *   return max byte size in the input or zfindexMax if not available\n
 *   if buffer is not null, return byte size actually read even if error (excluding '\0')
 *
 * ADVANCED:\n
 * for implementations, see #ZFCallbackTagKeyword_ioOwner if need support seek
 */
ZFCALLBACK_DECLARE_BEGIN(ZFLIB_ZFCore, ZFInput, ZFIOCallback)
public:
    /** @brief see #ZFInput */
    inline zfindex execute(
            ZF_OUT void *buf
            , ZF_IN zfindex count
            ) const {
        if(this->valid()) {
            return ZFCallback::executeExact<zfindex, void *, zfindex>(buf, count);
        }
        else {
            return 0;
        }
    }
    /** @brief see #ZFInput */
    inline zfindex operator () (
            ZF_OUT void *buf
            , ZF_IN zfindex count
            ) const {
        if(this->valid()) {
            return ZFCallback::executeExact<zfindex, void *, zfindex>(buf, count);
        }
        else {
            return 0;
        }
    }
    /** @brief see #ZFInput */
    const ZFInput &input(
            ZF_OUT void *buf
            , ZF_IN zfindex count
            , ZF_OUT_OPT zfindex *result = zfnull
            ) const {
        if(result != zfnull) {
            *result = this->execute(buf, count);
        }
        else {
            this->execute(buf, count);
        }
        return *this;
    }
    /** @brief see #ZFInput */
    const ZFInput &input(
            ZF_OUT zfstring &buf
            , ZF_IN zfindex count
            , ZF_OUT_OPT zfindex *result = zfnull
            ) const {
        if(count == zfindexMax()) {
            count = this->execute(zfnull, zfindexMax());
            if(count == zfindexMax()) {
                if(result != zfnull) {
                    *result = zfindexMax();
                }
                return *this;
            }
        }
        void *bufTmp = zfmalloc(count);
        zfindex read = this->execute(bufTmp, count);
        if(read > count) {
            zffree(bufTmp);
            if(result != zfnull) {
                *result = zfindexMax();
            }
            return *this;
        }
        buf.append((const zfchar *)bufTmp, read);
        zffree(bufTmp);
        if(result != zfnull) {
            *result = read;
        }
        return *this;
    }
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFLIB_ZFCore, ZFInput, ZFIOCallback)

// ============================================================
// ZFInputDummy
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback callbackType="ZFInputDummy">
 *       // dummy input has no contents
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFInputDummy "ZFInputDummy"
/**
 * @brief a dummy input that always read zero byte with success state
 */
extern ZFLIB_ZFCore ZFInput ZFInputDummy(void);

// ============================================================
// ZFInputForInputInRange
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFInputForInputInRange"
 *       start="zfindex" // optional, 0 by default
 *       count="zfindex" // optional, zfindexMax() by default
 *       autoRestorePos="zfbool" // optional, zftrue by default
 *   >
 *       <something category="input" ... /> // required, the source input callback
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFInputForInputInRange "ZFInputForInputInRange"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForInputInRange_input "input"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForInputInRange_start "start"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForInputInRange_count "count"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForInputInRange_autoRestorePos "autoRestorePos"
/**
 * @brief create a intput callback from another input callback, specified in range
 *
 * params:
 * -  (const ZFInput &) input callback to use
 * -  (zfindex) src's start index
 * -  (zfindex) src's count or zfindexMax() to use whole
 * -  (zfbool) whether to restore src input callback's position after result callback deleted
 *
 * seeking the result input callback would ensure in range [start, start + count]\n
 * src must support seek, otherwise a null callback would be returned
 */
extern ZFLIB_ZFCore ZFInput ZFInputForInputInRange(
        ZF_IN const ZFInput &inputCallback
        , ZF_IN_OPT zfindex start = 0
        , ZF_IN_OPT zfindex count = zfindexMax()
        , ZF_IN_OPT zfbool autoRestorePos = zftrue
        );

// ============================================================
// ZFInputForBuffer
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFInputForBuffer"
 *       buf="xxx" // optional, the buffer contents encoded by ZFCoreDataEncode
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFInputForBuffer "ZFInputForBuffer"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForBuffer_buf "buf"

/**
 * @brief create a intput callback input from a const void *,
 *   you must ensure the buffer is alive during the callback's life time
 *
 * params:
 * -  (const void *) src to use
 * -  (zfindex) src's count or zfindexMax() to calculate automatically (treated as const zfchar *),
 *   zfindexMax() by default
 */
extern ZFLIB_ZFCore ZFInput ZFInputForBufferUnsafe(
        ZF_IN const void *src
        , ZF_IN_OPT zfindex count = zfindexMax()
        , ZF_IN_OPT zfbool serializable = zffalse
        );
/**
 * @brief see #ZFInputForBufferUnsafe,
 *   copy the contents and auto free it
 */
extern ZFLIB_ZFCore ZFInput ZFInputForBuffer(
        ZF_IN const void *src
        , ZF_IN_OPT zfindex count = zfindexMax()
        , ZF_IN_OPT zfbool serializable = zffalse
        );

/**
 * @brief create a input callback from string
 *
 * the source string would be retained until the result ZFInput destroyed
 */
extern ZFLIB_ZFCore ZFInput ZFInputForString(
        ZF_IN const zfstring &src
        , ZF_IN_OPT zfbool serializable = zffalse
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_input_h_

