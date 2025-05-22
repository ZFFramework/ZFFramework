/**
 * @file ZFFile_res_raw.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_res_raw_h_
#define _ZFI_ZFFile_res_raw_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZF_NAMESPACE_BEGIN_REGISTER(ZFResRaw, ZF_NAMESPACE_GLOBAL)

// ============================================================
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsExist
        , ZFMP_IN(const zfchar *, resPath)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsDir
        , ZFMP_IN(const zfchar *, resPath)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFResCopy
        , ZFMP_IN(const zfchar *, resPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFResFindFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(const zfchar *, resPath)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResFindNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFResFindClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void *, ZFResOpen
        , ZFMP_IN(const zfchar *, resPath)
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
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfindex, ZFResRead
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsEof
        , ZFMP_IN(void *, token)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFResIsError
        , ZFMP_IN(void *, token)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, ZFResSize
        , ZFMP_IN(void *, token)
        )

ZF_NAMESPACE_END(ZFResRaw)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_res_raw_h_

