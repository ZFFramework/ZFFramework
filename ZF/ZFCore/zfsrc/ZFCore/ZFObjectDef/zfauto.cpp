#include "zfauto.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfauto::zfauto(ZF_IN zfauto const &obj) {
    zfRetainChange(_ZFP_obj, obj.toObject());
}
zfauto::~zfauto(void) {
    zfRelease(_ZFP_obj);
}

zfauto &zfauto::operator = (ZF_IN zfauto const &obj) {
    zfCoreMutexLock();
    this->zfunsafe_assign(obj);
    zfCoreMutexUnlock();
    return *this;
}

zfauto &zfauto::operator = (ZF_IN zfnullT const &) {
    zfCoreMutexLock();
    this->zfunsafe_assign(zfnull);
    zfCoreMutexUnlock();
    return *this;
}

void zfauto::zfunsafe_assign(ZF_IN ZFObject *obj) {
    zfunsafe_zfRetainChange(_ZFP_obj, obj);
}
void zfauto::zfunsafe_assign(ZF_IN zfauto const &obj) {
    zfunsafe_zfRetainChange(_ZFP_obj, obj.toObject());
}

zfindex zfauto::objectRetainCount(void) const {
    return _ZFP_obj ? _ZFP_obj->objectRetainCount() : 0;
}

const ZFClass *zfauto::ClassData(void) {
    return ZFObject::ClassData();
}

ZF_NAMESPACE_GLOBAL_END

