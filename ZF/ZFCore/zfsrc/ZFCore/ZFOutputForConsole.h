/**
 * @file ZFOutputForConsole.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFOutputForConsole_h_
#define _ZFI_ZFOutputForConsole_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback callbackType="ZFOutputForConsole">
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFOutputForConsole "ZFOutputForConsole"

/**
 * @brief default output callback to output to console, source must be zfchar *string
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFCore, ZFOutput, ZFOutputForConsole)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForConsole_h_

