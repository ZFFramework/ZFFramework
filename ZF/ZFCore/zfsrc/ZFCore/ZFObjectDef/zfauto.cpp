#include "zfauto.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfauto::zfauto(ZF_IN zfauto const &obj) {
    zfCoreMutexLock();
    d = obj.d;
    if(d) {
        ++(d->refCount);
    }
    zfCoreMutexUnlock();
}
zfauto::~zfauto(void) {
    if(d) {
        zfCoreMutexLock();
        if(d->refCount == 1) {
            zfunsafe_zfRelease(d->obj);
            zfpoolDelete(d);
        }
        else {
            --(d->refCount);
        }
        zfCoreMutexUnlock();
    }
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
    zfunsafe_zfRetain(obj);
    if(d) {
        if(d->refCount == 1) {
            ZFObject *objTmp = d->obj;
            d->obj = obj;
            zfunsafe_zfRelease(objTmp);
        }
        else {
            --(d->refCount);
            if(obj) {
                d = zfpoolNew(_ZFP_zfautoPrivate, obj);
            }
            else {
                d = zfnull;
            }
        }
    }
    else if(obj) {
        d = zfpoolNew(_ZFP_zfautoPrivate, obj);
    }
}
void zfauto::zfunsafe_assign(ZF_IN zfauto const &obj) {
    if(d) {
        _ZFP_zfautoPrivate *dTmp = d;
        d = obj.d;
        if(d) {
            ++(d->refCount);
        }
        if(dTmp) {
            if(dTmp->refCount == 1) {
                zfunsafe_zfRelease(dTmp->obj);
                zfpoolDelete(dTmp);
            }
            else {
                --(dTmp->refCount);
            }
        }
    }
    else {
        d = obj.d;
        if(d) {
            ++(d->refCount);
        }
    }
}

const ZFClass *zfauto::ClassData(void) {
    return ZFObject::ClassData();
}

ZF_NAMESPACE_GLOBAL_END

