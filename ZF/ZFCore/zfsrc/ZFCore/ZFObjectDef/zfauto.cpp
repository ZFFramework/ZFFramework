#include "zfauto.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfauto::~zfauto(void) {
    zfRelease(_ZFP_obj);
}

void zfauto::zfunsafe_assign(ZF_IN ZFObject *obj) {
    zfunsafe_zfRetainChange(_ZFP_obj, obj);
}

zfindex zfauto::objectRetainCount(void) const {
    return _ZFP_obj ? _ZFP_obj->objectRetainCount() : 0;
}

const ZFClass *zfauto::ClassData(void) {
    return ZFObject::ClassData();
}

ZF_NAMESPACE_GLOBAL_END

