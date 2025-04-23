/**
 * @file ZFNull.h
 * @brief ZFNull
 */

#ifndef _ZFI_ZFNull_h_
#define _ZFI_ZFNull_h_

#include "ZFObjectClassTypeFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFCore ZFObject *_ZFP_ZFNull;
/**
 * @brief a global null object
 *
 * useful when adding null to a container,
 * since containers doesn't allow null
 */
#define ZFNull() _ZFP_ZFNull

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFNull_h_

