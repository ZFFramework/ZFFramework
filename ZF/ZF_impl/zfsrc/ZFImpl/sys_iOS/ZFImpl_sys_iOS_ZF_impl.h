#ifndef _ZFI_ZFImpl_sys_iOS_ZF_impl_h_
#define _ZFI_ZFImpl_sys_iOS_ZF_impl_h_

#include "../ZFImpl_ZF_impl.h"
#include "ZFImpl_sys_iOS.h"


#if ZF_ENV_sys_iOS

#include <Foundation/Foundation.h>
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief dummy macro to show a property override for iOS
 */
#define ZFImpl_sys_iOS_overrideProperty

// ============================================================
/**
 * @brief convert NSString to zfstring, appended to result, do nothing if error
 */
extern ZFLIB_ZF_impl void ZFImpl_sys_iOS_zfstringFromNSString(
        ZF_IN_OUT zfstring &result
        , ZF_IN NSString *nativeString
        );
/**
 * @brief convert NSString to zfstring, appended to result, do nothing if error
 */
inline zfstring ZFImpl_sys_iOS_zfstringFromNSString(ZF_IN NSString *nativeString) {
    zfstring ret;
    ZFImpl_sys_iOS_zfstringFromNSString(ret, nativeString);
    return ret;
}
/**
 * @brief convert zfstring to NSString, return a autoreleased object
 */
extern ZFLIB_ZF_impl NSString *ZFImpl_sys_iOS_zfstringToNSString(ZF_IN const zfchar *s);

// ============================================================
/**
 * @brief return a string describe the object, for debug use only
 */
extern ZFLIB_ZF_impl NSString *ZFImpl_sys_iOS_objectInfoNative(
        ZF_IN id nativeObject
        , ZF_IN_OPT NSArray *ignorePropertyNames = nil
        );
/**
 * @brief return a string describe the object, for debug use only
 */
extern ZFLIB_ZF_impl zfstring ZFImpl_sys_iOS_objectInfo(
        ZF_IN id nativeObject
        , ZF_IN_OPT ZFCoreArray<zfstring> *ignorePropertyNames = zfnull
        );

// ============================================================
/** @brief util to supply iterator access for NSDictionary */
extern ZFLIB_ZF_impl zfiter ZFImpl_sys_iOS_NSDictionaryIter(ZF_IN NSDictionary *dict);
/** @brief see #ZFImpl_sys_iOS_NSDictionaryIter */
extern ZFLIB_ZF_impl zfiter ZFImpl_sys_iOS_NSDictionaryIterFind(
        ZF_IN NSDictionary *dict
        , ZF_IN id key
        );
/** @brief see #ZFImpl_sys_iOS_NSDictionaryIter */
extern ZFLIB_ZF_impl id ZFImpl_sys_iOS_NSDictionaryIterKey(
        ZF_IN const zfiter &it
        );
/** @brief see #ZFImpl_sys_iOS_NSDictionaryIter */
extern ZFLIB_ZF_impl id ZFImpl_sys_iOS_NSDictionaryIterValue(ZF_IN const zfiter &it);
/** @brief see #ZFImpl_sys_iOS_NSDictionaryIter */
extern ZFLIB_ZF_impl void ZFImpl_sys_iOS_NSDictionaryIterValue(
        ZF_IN_OUT zfiter &it
        , ZF_IN id value
        );
/** @brief see #ZFImpl_sys_iOS_NSDictionaryIter */
extern ZFLIB_ZF_impl void ZFImpl_sys_iOS_NSDictionaryIterRemove(ZF_IN_OUT zfiter &it);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS
#endif // #ifndef _ZFI_ZFImpl_sys_iOS_ZF_impl_h_

