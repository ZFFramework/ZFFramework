/**
 * @file ZFCoreArg.h
 * @brief arg utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreArg_h_
#define _ZFI_ZFCoreArg_h_

#include "ZFCoreArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief split cmd line to separate item
 *
 * result would be appended to result instead of clear and set
 */
extern ZFLIB_ZFCore void zfCoreArgSplit(
        ZF_IN_OUT ZFCoreArray<zfstring> &result
        , ZF_IN const zfchar *cmdLine
        );
/**
 * @brief split cmd line to separate item
 */
extern ZFLIB_ZFCore ZFCoreArray<zfstring> zfCoreArgSplit(ZF_IN const zfchar *cmdLine);
/**
 * @brief see #zfCoreArgSplit
 *
 * result is appended to result instead of clear and set
 */
extern ZFLIB_ZFCore void zfCoreArgMerge(
        ZF_OUT zfstring &result
        , ZF_IN const ZFCoreArray<zfstring> &argList
        );
/**
 * @brief see #zfCoreArgMerge
 */
extern ZFLIB_ZFCore zfstring zfCoreArgMerge(ZF_IN const ZFCoreArray<zfstring> &argList);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreArg_h_

