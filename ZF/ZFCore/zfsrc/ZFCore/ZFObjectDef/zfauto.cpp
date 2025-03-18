#include "zfauto.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfauto::zfauto(ZF_IN zfauto const &obj)
: _ZFP_obj(zfRetain(obj._ZFP_obj))
{
}
zfauto::~zfauto(void) {
    zfRelease(_ZFP_obj);
}

zfauto &zfauto::operator = (ZF_IN zfauto const &p) {
    ZFCoreMutexLock();
    this->zfunsafe_assign(_ZFP_zfanyCast(p));
    ZFCoreMutexUnlock();
    return *this;
}

void zfauto::zfunsafe_assign(ZF_IN ZFObject *obj) {
    ZFObject *t = _ZFP_obj;
    _ZFP_obj = zfunsafe_zfRetain(obj);
    zfunsafe_zfRelease(t);
}

zfindex zfauto::objectRetainCount(void) const {
    return _ZFP_obj ? _ZFP_obj->objectRetainCount() : 0;
}

const ZFClass *zfauto::ClassData(void) {
    return ZFObject::ClassData();
}

ZF_NAMESPACE_GLOBAL_END

