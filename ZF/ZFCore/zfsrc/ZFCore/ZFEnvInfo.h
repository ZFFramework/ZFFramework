/**
 * @file ZFEnvInfo.h
 * @brief info of native environment
 */

#ifndef _ZFI_ZFEnvInfo_h_
#define _ZFI_ZFEnvInfo_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFEnvInfo::envSummaryCallbackRegister */
typedef void (*ZFEnvSummaryCallback)(ZF_IN_OUT zfstring &ret);

ZF_NAMESPACE_BEGIN_REGISTER(ZFEnvInfo, ZF_NAMESPACE_GLOBAL)

// ============================================================
/**
 * @brief util method to get summary info
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, envSummaryT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
/**
 * @brief see #envSummaryT
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, envSummary)

/**
 * @brief register a callback to output custom env info, see #envSummaryT
 */
extern ZFLIB_ZFCore void envSummaryCallbackRegister(
        ZF_IN const zfstring &name
        , ZF_IN ZFEnvSummaryCallback callback
        );
/** @brief see #envSummaryCallbackRegister */
extern ZFLIB_ZFCore void envSummaryCallbackUnregister(ZF_IN const zfstring &name);

// ============================================================
/**
 * @brief name of native system, e.g. "iOS", empty if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, systemNameT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
/**
 * @brief see #systemNameT
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, systemName)
/**
 * @brief version of native system, e.g. "9.0.1", empty if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, systemVersionT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
/**
 * @brief see #systemVersionT
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, systemVersion)

// ============================================================
/**
 * @brief name of native framework, e.g. "Qt", empty if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, frameworkNameT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
/**
 * @brief see #frameworkNameT
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, frameworkName)
/**
 * @brief version of native framework, e.g. "9.0.1", empty if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, frameworkVersionT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
/**
 * @brief see #frameworkVersionT
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, zfstring, frameworkVersion)

ZF_NAMESPACE_END(ZFEnvInfo)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnvInfo_h_

