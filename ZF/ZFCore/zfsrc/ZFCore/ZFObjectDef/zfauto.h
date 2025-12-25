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
inline zfauto::zfauto(ZF_IN zfauto const &obj)
: _ZFP_obj(zfRetain(_ZFP_zfanyCast(obj)))
{
}
template<typename T_ZFObject>
zfauto::zfauto(ZF_IN T_ZFObject const &obj)
: _ZFP_obj(zfRetain(_ZFP_zfanyCast(obj)))
{
}
inline zfauto::~zfauto(void) {
    zfRelease(_ZFP_obj);
}

inline zfauto &zfauto::operator = (ZF_IN zfauto const &obj) {
    ZFCoreMutexLock();
    this->zfunsafe_assign(_ZFP_zfanyCast(obj));
    ZFCoreMutexUnlock();
    return *this;
}
template<typename T_ZFObject>
zfauto &zfauto::operator = (ZF_IN T_ZFObject const &obj) {
    ZFCoreMutexLock();
    this->zfunsafe_assign(_ZFP_zfanyCast(obj));
    ZFCoreMutexUnlock();
    return *this;
}

template<typename T_ZFObject>
zfbool zfauto::operator == (ZF_IN T_ZFObject const &obj) const {
    return this->toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject>
zfbool zfauto::operator != (ZF_IN T_ZFObject const &obj) const {
    return this->toObject() != _ZFP_zfanyCast(obj);
}

template<typename T_ZFObject>
inline zfauto::operator T_ZFObject * (void) const {
    return zfcast(T_ZFObject *, this->toObject());
}

inline const ZFClass *zfauto::ClassData(void) {
    return ZFObject::ClassData();
}

inline void zfauto::zfunsafe_assign(ZF_IN ZFObject *obj) {
    ZFObject *t = _ZFP_obj;
    _ZFP_obj = zfunsafe_zfRetain(obj);
    zfunsafe_zfRelease(t);
}

inline zfindex zfauto::objectRetainCount(void) const {
    return _ZFP_obj ? _ZFP_obj->objectRetainCount() : 0;
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

