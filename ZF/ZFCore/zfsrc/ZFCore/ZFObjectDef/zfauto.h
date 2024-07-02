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
template<typename T_ZFObject>
zfauto::zfauto(ZF_IN T_ZFObject *p)
: _ZFP_obj(zfRetain(p ? p->toObject() : zfnull))
{
}
template<typename T_ZFObject>
zfauto::zfauto(ZF_IN T_ZFObject const &p)
: _ZFP_obj(zfRetain(_ZFP_zfanyCast(p)))
{
}

template<typename T_ZFObject>
zfauto &zfauto::operator = (ZF_IN T_ZFObject *p) {
    zfCoreMutexLock();
    this->zfunsafe_assign(p ? p->toObject() : zfnull);
    zfCoreMutexUnlock();
    return *this;
}
template<typename T_ZFObject>
zfauto &zfauto::operator = (ZF_IN T_ZFObject const &p) {
    zfCoreMutexLock();
    this->zfunsafe_assign(_ZFP_zfanyCast(p));
    zfCoreMutexUnlock();
    return *this;
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

/** @cond ZFPrivateDoc */
// ============================================================
template<typename T_ZFObject>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfauto const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfauto const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}

// ============================================================
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfautoT<T_ZFObjectBase> const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfautoT<T_ZFObjectBase> const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfauto_h_

