/**
 * @file ZFFile_util.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_util_h_
#define _ZFI_ZFFile_util_h_

#include "ZFFile_file.h"
#include "ZFFile_res.h"
#include "ZFFile_pathInfo.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief format path
 *
 * format rules:
 * -  ensured no '/' at tail
 * -  all extra '/' and '\\' would be replaced with single '/'
 * -  strip spaces and tabs at head and tail
 * -  replace these escape chars:
 *   -  '\"' to double quote
 *   -  '\ ' to space
 * -  remove "./" at head
 * -  remove "/." at tail
 * -  remove "/./" at anywhere
 * -  null src is treated as error, while empty string src is not an error
 * -  single "/" src would result empty string
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathFormatT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
/** @brief see #ZFPathFormatT */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFCore, zfstring, ZFPathFormat
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        ) {
    zfstring ret;
    ZFPathFormatT(ret, src, srcLen);
    return ret;
}

/**
 * @brief util method to resolve ".." in path
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathFormatRelativeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
/** @brief see #ZFPathFormatRelativeT */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFCore, zfstring, ZFPathFormatRelative
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        ) {
    zfstring ret;
    ZFPathFormatRelativeT(ret, src, srcLen);
    return ret;
}

/**
 * @brief get file name from path or src if error,
 *   e.g. "file.ext" from "/path/file.ext"
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFileNameOf
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFFileNameOf */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFFileNameOf
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get file name without extension from path or src if error,
 *   e.g. "file.ext0" from "/path/file.ext0.ext1"
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFileNameOfWithoutExt
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFFileNameOfWithoutExt */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFFileNameOfWithoutExt
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get file extension from path or empty if error,
 *   e.g. "ext1" from "/path/file.ext0.ext1",
 *   or empty if no extension
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFileExtOf
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFFileExtOf */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFFileExtOf
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get file path without ext,
 *   e.g. "/path/file.ext0" from "/path/file.ext0.ext1",
 *   or original string if no extension
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathOfWithoutExt
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFFileExtOf */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFPathOfWithoutExt
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get parent path
 *
 * return empty string if no parent found
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathParentOf
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFPathParentOf */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFPathParentOf
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get each file components from path or empty if error,
 *   e.g. {"path", "file.ext"}(as array) from "/path/file.ext"
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathComponentsOf
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFPathComponentsOf */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFCoreArray<zfstring>, ZFPathComponentsOf
        , ZFMP_IN(const zfchar *, src)
        )

// ============================================================
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFFileCloseHolder {
public:
    _ZFP_ZFFileCloseHolder(ZF_IN void *token) : token(token) {}
    ~_ZFP_ZFFileCloseHolder(void) {
        if(this->token != zfnull) {
            ZFFileClose(this->token);
        }
    }
private:
    void *token;
};
/**
 * @brief util method to call #ZFFileClose after code block
 */
#define ZFFileCloseHolder(token) _ZFP_ZFFileCloseHolder ZFUniqueName(ZFFileCloseHolder)(token)

// ============================================================
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFResCloseHolder {
public:
    _ZFP_ZFResCloseHolder(ZF_IN void *token) : token(token) {}
    ~_ZFP_ZFResCloseHolder(void) {
        if(this->token != zfnull) {
            ZFResClose(this->token);
        }
    }
private:
    void *token;
};
/**
 * @brief util method to call #ZFResClose after code block
 */
#define ZFResCloseHolder(token) _ZFP_ZFResCloseHolder ZFUniqueName(ZFResCloseHolder)(token)

// ============================================================
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFPathInfoCloseHolder {
public:
    _ZFP_ZFPathInfoCloseHolder(
            ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN void *token
            )
    : pathInfo(pathInfo)
    , token(token)
    {
    }
    ~_ZFP_ZFPathInfoCloseHolder(void) {
        if(this->token != zfnull) {
            ZFPathInfoClose(this->pathInfo, this->token);
        }
    }
private:
    const ZFPathInfo &pathInfo;
    void *token;
};
/**
 * @brief util method to call #ZFPathInfoClose after code block
 */
#define ZFPathInfoCloseHolder(pathInfo, token) _ZFP_ZFPathInfoCloseHolder ZFUniqueName(ZFPathInfoCloseHolder)(pathInfo, token)

// ============================================================
/**
 * @brief list all files, usually for debug use
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, void, ZFPathInfoTreePrint
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        , ZFMP_IN_OPT(const zfchar *, headToken, zfnull)
        , ZFMP_IN_OPT(const zfchar *, indentToken, "  ")
        )

// ============================================================
/**
 * @brief util method to loop each child file or dir in specified pathInfo
 *
 * usage:
 * @code
 *   ZFLISTENER(fileCallback) {
 *       const ZFPathInfo &pathInfo = zfargs.param0().to<v_ZFPathInfo *>()->zfv;
 *       const ZFFileFindData &fd = zfargs.param1().to<v_ZFFileFindData *>()->zfv;
 *       // do your things with the file
 *   } ZFLISTENER_END()
 *   ZFPathInfoForEach(pathInfo, fileCallback);
 * @endcode
 *
 * fileCallback's param0 is #v_ZFPathInfo that describe the child file or dir,
 * and param1 is #v_ZFFileFindData,
 * set #ZFArgs::eventFiltered to indicates process done
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForEach
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )
/** @brief util method to loop each file, see #ZFPathInfoForEach */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForEachFile
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )
/** @brief util method to loop each dir, see #ZFPathInfoForEach */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForEachDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_util_h_

