/**
 * @file ZFIODef_util.h
 * @brief abstract IO
 */

#ifndef _ZFI_ZFIODef_util_h_
#define _ZFI_ZFIODef_util_h_

#include "ZFIODef_fwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief format path
 *
 * format rules:
 * -  ensured no '/' at tail (except for `xxx://`)
 * -  all extra '/' and '\\' would be replaced with single '/' (except for `xxx://`)
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
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFileNameOfT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFFileNameOfT */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFFileNameOf
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get file name without extension from path or src if error,
 *   e.g. "file.ext0" from "/path/file.ext0.ext1"
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFileNameOfWithoutExtT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFFileNameOfWithoutExtT */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFFileNameOfWithoutExt
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get file extension from path or empty if error,
 *   e.g. "ext1" from "/path/file.ext0.ext1",
 *   or empty if no extension
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFFileExtOfT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFFileExtOfT */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFFileExtOf
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get file path without ext,
 *   e.g. "/path/file.ext0" from "/path/file.ext0.ext1",
 *   or original string if no extension
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathOfWithoutExtT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFPathOfWithoutExtT */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFPathOfWithoutExt
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get file path without all ext,
 *   e.g. "/path/file" from "/path/file.ext0.ext1",
 *   or original string if no extension
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathOfWithoutAllExtT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFPathOfWithoutAllExtT */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFPathOfWithoutAllExt
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get parent path
 *
 * return empty string if no parent found
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathParentOfT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFPathParentOfT */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfstring, ZFPathParentOf
        , ZFMP_IN(const zfchar *, src)
        )
/**
 * @brief get each file components from path or empty if error,
 *   e.g. {"path", "file.ext"}(as array) from "/path/file.ext"
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathComponentsOfT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief see #ZFPathComponentsOfT */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFCoreArray<zfstring>, ZFPathComponentsOf
        , ZFMP_IN(const zfchar *, src)
        )

// ============================================================
/**
 * @brief list all files, usually for debug use
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, void, ZFIOTreePrint
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
 *       const ZFIOFindData &fd = zfargs.param1().to<v_ZFIOFindData *>()->zfv;
 *       // do your things with the file
 *   } ZFLISTENER_END()
 *   ZFIOForEach(pathInfo, fileCallback);
 * @endcode
 *
 * fileCallback's param0 is #v_ZFPathInfo that describe the child file or dir,
 * and param1 is #v_ZFIOFindData,
 * set #ZFArgs::eventFiltered to indicates process done
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFIOForEach
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )
/** @brief util method to loop each file, see #ZFIOForEach */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFIOForEachFile
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )
/** @brief util method to loop each dir, see #ZFIOForEach */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFIOForEachDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )

// ============================================================
/**
 * @brief copy a file or directory from srcPath to dstPath
 *
 * if both src and dst exist, but one is file and another is dir,
 * return zffalse\n
 * merge directory if dst is an existing dir
 * (if isForce not set,
 * would return false if dst has a child file with the same path in src)\n
 * override file if dst is an existing file and isForce is zftrue
 * @note path must be well formed, use #ZFPathFormat if necessary
 * @note always use read/write to copy files
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFIOCopy
        , ZFMP_IN(const ZFPathInfo &, srcPath)
        , ZFMP_IN(const ZFPathInfo &, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIODef_util_h_

