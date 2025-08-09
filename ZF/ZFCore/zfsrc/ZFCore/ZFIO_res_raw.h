/**
 * @file ZFIO_res_raw.h
 * @brief abstract IO
 */

#ifndef _ZFI_ZFIO_res_raw_h_
#define _ZFI_ZFIO_res_raw_h_

#include "ZFIODef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is the raw res path (see #ZFResRaw)
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFIO_DECLARE(ZFLIB_ZFCore, resraw)

// ============================================================
// ZFInputForResRaw
/**
 * @brief util to create a resource file input callback,
 *   see #ZFResOpen for what resource file is
 *
 * param:
 * -  (const zfstring &)resFilePath: resource file path to use
 *
 * auto open and auto close files, may return a null callback if open file error
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFCore, ZFInput, ZFInputForResRaw
        , ZFMP_IN(const zfstring &, resFilePath)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(ZFPathType_resraw(), resFilePath));
    return ret;
}

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZF_NAMESPACE_BEGIN_REGISTER(ZFResRaw, ZF_NAMESPACE_GLOBAL)

// ============================================================
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsExist
        , ZFMP_IN(const zfstring &, resPath)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsDir
        , ZFMP_IN(const zfstring &, resPath)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFResFindFirst
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, resPath)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResFindNext
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFResFindClose
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void *, ZFResOpen
        , ZFMP_IN(const zfstring &, resPath)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResClose
        , ZFMP_IN(void *, token)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, ZFResTell
        , ZFMP_IN(void *, token)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFResSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, seekPos, ZFSeekPosBegin)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfindex, ZFResRead
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, ZFResSize
        , ZFMP_IN(void *, token)
        )

ZF_NAMESPACE_END(ZFResRaw)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIO_res_raw_h_

