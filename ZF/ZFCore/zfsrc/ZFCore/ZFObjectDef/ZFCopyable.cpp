#include "ZFCopyable.h"
#include "ZFObjectImpl.h"
#include "ZFSerializable.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFCopyable)

zfautoT<ZFCopyable> ZFCopyable::copy(void) {
    zfautoT<ZFCopyable> ret = this->classData()->newInstance();
    if(ret != zfnull) {
        zfcast(zfself *, ret)->copyableOnCopyFrom(this->toObject());
    }
    return ret;
}

void ZFCopyable::copyFrom(ZF_IN ZFObject *anotherObj) {
    if(anotherObj != zfnull && anotherObj != this->toObject()) {
        this->copyableOnCopyFrom(anotherObj);
    }
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFCopyable, zfautoT<ZFCopyable>, copy)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCopyable, void, copyFrom
        , ZFMP_IN(ZFObject *, anotherObj)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfauto, ZFObjectCopy
        , ZFMP_IN(ZFObject *, obj)
        )

ZF_NAMESPACE_GLOBAL_END

