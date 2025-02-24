/**
 * @file ZFCoreTypeDef_StringType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_StringType_h_
#define _ZFI_ZFCoreTypeDef_StringType_h_

#include "zfstring.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #zft_zfstring */
#ifndef _ZFT_zfstring
    typedef zft_zfstring<zfchar> zfstring;
#else
    typedef _ZFT_zfstring zfstring;
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_StringType_h_

