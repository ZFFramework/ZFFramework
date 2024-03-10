#include "zfauto.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfauto::zfauto(ZF_IN zfauto const &ref) {
    zfCoreMutexLock();
    d = ref.d;
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

zfauto &zfauto::operator = (ZF_IN zfauto const &ref) {
    zfCoreMutexLock();
    this->zfunsafe_assign(ref);
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
void zfauto::zfunsafe_assign(ZF_IN zfauto const &ref) {
    if(d) {
        _ZFP_zfautoPrivate *dTmp = d;
        d = ref.d;
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
        d = ref.d;
        if(d) {
            ++(d->refCount);
        }
    }
}

// ============================================================
void _ZFP_zfautoTError(void) {
    zfCoreCriticalMessageTrim("[zfautoT] cast from incompatible type");
}

ZF_NAMESPACE_GLOBAL_END

