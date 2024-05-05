#ifndef _ZFI_ZFImpl_sys_Qt_ZF_impl_h_
#define _ZFI_ZFImpl_sys_Qt_ZF_impl_h_

#include "../ZFImpl_ZF_impl.h"
#include "ZFImpl_sys_Qt.h"


#if ZF_ENV_sys_Qt
#include <QObject>
#include <QDebug>
#include <QVariant>
#include "ZFCore/ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZFLIB_ZF_impl void ZFImpl_sys_Qt_QObjectTag(
        ZF_IN_OUT QObject *obj
        , ZF_IN const zfchar *name
        , ZF_IN QVariant const &tag
        );
extern ZFLIB_ZF_impl QVariant ZFImpl_sys_Qt_QObjectTag(
        ZF_IN_OUT QObject *obj
        , ZF_IN const zfchar *name
        );

extern ZFLIB_ZF_impl void ZFImpl_sys_Qt_QObjectZFObjectTag(
        ZF_IN_OUT QObject *obj
        , ZF_IN const zfchar *name
        , ZF_IN ZFObject *tag
        );
extern ZFLIB_ZF_impl zfany ZFImpl_sys_Qt_QObjectZFObjectTag(
        ZF_IN_OUT QObject *obj
        , ZF_IN const zfchar *name
        );

// ============================================================
/**
 * @brief convert QString to zfstring, appended to result, do nothing if error
 */
extern ZFLIB_ZF_impl void ZFImpl_sys_Qt_zfstringFromQString(
        ZF_IN_OUT zfstring &result
        , ZF_IN QString const &nativeString
        );
/**
 * @brief convert QString to zfstring, appended to result, do nothing if error
 */
inline zfstring ZFImpl_sys_Qt_zfstringFromQString(ZF_IN QString const &nativeString) {
    zfstring ret;
    ZFImpl_sys_Qt_zfstringFromQString(ret, nativeString);
    return ret;
}
/**
 * @brief convert zfstring to QString, return a autoreleased object
 */
extern ZFLIB_ZF_impl QString ZFImpl_sys_Qt_zfstringToQString(ZF_IN const zfchar *s);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt
#endif // #ifndef _ZFI_ZFImpl_sys_Qt_ZF_impl_h_

