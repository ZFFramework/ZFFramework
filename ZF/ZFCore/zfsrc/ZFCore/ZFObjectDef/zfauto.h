/**
 * @file zfauto.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfauto_h_
#define _ZFI_zfauto_h_

#include "ZFObjectRetain.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfauto
/** @cond ZFPrivateDoc */
inline zfauto::zfauto(ZF_IN ZFObject *p)
: _ZFP_obj(zfRetain(p))
{
}

template<typename T_ZFObject>
zfauto::zfauto(ZF_IN T_ZFObject const &p)
: _ZFP_obj(zfRetain(_ZFP_zfanyCast(p)))
{
}

template<typename T_ZFObject>
zfauto &zfauto::operator = (ZF_IN T_ZFObject const &p) {
    ZFCoreMutexLock();
    this->zfunsafe_assign(_ZFP_zfanyCast(p));
    ZFCoreMutexUnlock();
    return *this;
}
template<typename T_ZFObject>
inline zfauto::operator T_ZFObject * (void) const {
    return zfcast(T_ZFObject *, this->toObject());
}
template<typename T_ZFObject>
inline T_ZFObject zfauto::to(void) const {
    return zfcast(T_ZFObject, this->toObject());
}
/** @endcond */

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_ZFObjectBase>
T_ZFObjectBase *zfautoT<T_ZFObjectBase>::operator -> (void) const {
    return zfcast(T_ZFObjectBase *, this->toObject());
}

template<typename T_ZFObjectBase>
const ZFClass *zfautoT<T_ZFObjectBase>::ClassData(void) {
    return T_ZFObjectBase::ClassData();
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfauto_h_

