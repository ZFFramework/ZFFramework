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
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, IsExist
        , ZFMP_IN(const zfchar *, resPath)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, IsDir
        , ZFMP_IN(const zfchar *, resPath)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFCore, zfbool, Copy
        , ZFMP_IN(const zfchar *, resPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        , ZFMP_IN_OPT(zfstring *, errPos, zfnull)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, FindFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(const zfchar *, resPath)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, FindNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, FindClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void *, Open
        , ZFMP_IN(const zfchar *, resPath)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, Close
        , ZFMP_IN(void *, token)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, Tell
        , ZFMP_IN(void *, token)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, Seek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfindex, Read
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, IsEof
        , ZFMP_IN(void *, token)
        )
/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, IsError
        , ZFMP_IN(void *, token)
        )

/** @brief raw res process which bypass #ZFResExtPathAdd logic */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfindex, Size
        , ZFMP_IN(void *, token)
        )

ZF_NAMESPACE_END(ZFResRaw)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_res_raw_h_

