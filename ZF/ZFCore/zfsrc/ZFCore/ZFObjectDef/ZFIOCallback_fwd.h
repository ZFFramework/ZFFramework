/**
 * @file ZFIOCallback_fwd.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_fwd_h_
#define _ZFI_ZFIOCallback_fwd_h_

#include "ZFCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief for advance user only
 *
 * for #ZFOutput and #ZFInput to achieve some additional operation\n
 * \n
 * usage:\n
 * set an auto released data by #ZFCallback::callbackTag,
 * whose key is #ZFCallbackTagKeyword_ioOwner and value is a ZFObject
 * that optionally implement those method (as #ZFMethod):
 * -  ioFlush, similar to FILE's #ZFFileFlush, proto type:\n
 *   void ioFlush(void);
 * -  ioSeek, similar to FILE's #ZFFileSeek, proto type:\n
 *   zfbool ioSeek(
 *           ZF_IN zfindex byteSize
 *           , ZF_IN ZFSeekPos seekPos
 *           );\n
 *   return false if the callback doesn't support seek or error occurred
 * -  ioTell, similar to FILE's #ZFFileTell, proto type:\n
 *   zfindex ioTell(void);\n
 *   return current's index or zfindexMax() if the callback doesn't support seek
 * -  ioSize, get callback's size, proto type:\n
 *   zfindex ioSize(void);\n
 *   return size or zfindexMax() if the callback doesn't support
 *   @note for input callbacks, the size shows the total size of the input
 *   @note for ouput callbacks, the size shows the current size of the output,
 *     it may or may not be the actual size written to the output,
 *     usually it has no actual meaning
 */
#define ZFCallbackTagKeyword_ioOwner "ZFCallbackTagKeyword_ioOwner"
/**
 * @brief util method for ZFIOCallback::ioSeek's implementations to calculate seek index
 *
 * param:\n
 * -  (zfindex)offset callback's initial offset
 * -  (zfindex)length callback's length including offset (i.e. starting from original 0)
 * -  (zfindex)curPos callback's current position including offset (i.e. start from original 0)
 *
 * return an index start from 0 (offset is included), ensured range in [offset, length]
 */
extern ZFLIB_ZFCore zfindex ZFIOCallbackCalcSeek(
        ZF_IN zfindex offset
        , ZF_IN zfindex length
        , ZF_IN zfindex curPos
        , ZF_IN zfindex seekByteSize
        , ZF_IN ZFSeekPos seekPos
        );

// ============================================================
/**
 * @brief abstract base class, use #ZFOutput or #ZFInput
 */
ZFCALLBACK_DECLARE_BEGIN(ZFLIB_ZFCore, ZFIOCallback, ZFCallback)
public:
    /**
     * @brief similar to FILE's #ZFFileFlush
     */
    void ioFlush(void) const;
    /**
     * @brief similar to FILE's #ZFFileSeek
     *
     * return false if the callback doesn't support seek
     */
    zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) const;
    /**
     * @brief similar to FILE's #ZFFileTell, return current's index or zfindexMax() if the callback doesn't support seek
     */
    zfindex ioTell(void) const;
    /**
     * @brief calculate the callback's size or return zfindexMax() if not supported
     */
    zfindex ioSize(void) const;

    /**
     * @brief close the io callback
     */
    zfbool ioClose(void) const;

    /**
     * @brief util to set #ZFCallbackTagKeyword_ioOwner
     */
    void ioOwner(ZF_IN ZFObject *ioOwner) {
        this->callbackTag(ZFCallbackTagKeyword_ioOwner, ioOwner);
    }
    /**
     * @brief util to get #ZFCallbackTagKeyword_ioOwner
     */
    zfany ioOwner(void) const {
        return this->callbackTag(ZFCallbackTagKeyword_ioOwner);
    }
_ZFP_ZFCALLBACK_DECLARE_END(ZFLIB_ZFCore, ZFIOCallback, ZFCallback)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_fwd_h_

