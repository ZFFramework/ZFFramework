#include "ZFInputForImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFInputForImplOwner : zfextend ZFObject {
public:
    ZFOBJECT_DECLARE(_ZFP_I_ZFInputForImplOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->_bufCache->zfv.removeAll();
        cache->_countCache->zfv = 0;
        cache->_resultCache->zfv = 0;
        cache->impl = zfnull;
    })

public:
    ZFListener impl;

private:
    v_zfstring *_bufCache;
    v_zfindex *_countCache;
    v_zfindex *_resultCache;

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->_bufCache = zfunsafe_zfAlloc(v_zfstring);
        this->_countCache = zfunsafe_zfAlloc(v_zfindex);
        this->_resultCache = zfunsafe_zfAlloc(v_zfindex);
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        zfunsafe_zfRelease(this->_bufCache);
        zfunsafe_zfRelease(this->_countCache);
        zfunsafe_zfRelease(this->_resultCache);
        zfsuper::objectOnDealloc();
    }

protected:
    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            )
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForImplOwner, zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            ) {
    if(this->impl) {
        if(buf == zfnull || count == zfindexMax()) {
            this->_countCache->zfv = zfindexMax();
        }
        else {
            this->_countCache->zfv = count;
        }
        this->_bufCache->zfv.removeAll();
        this->_resultCache->zfv = 0;
        this->impl.execute(ZFArgs()
                .param0(this->_bufCache)
                .param1(this->_countCache)
                .result(this->_resultCache)
            );
        ZFCoreAssert(this->_resultCache->zfv <= this->_bufCache->zfv.length());
        zfmemcpy(buf, this->_bufCache->zfv.cString(), this->_resultCache->zfv);
        return this->_resultCache->zfv;
    }
    else {
        return 0;
    }
}

ZFMETHOD_FUNC_DEFINE_1(ZFInput, ZFInputForImpl
            , ZFMP_IN(const ZFListener &, impl)
            ) {
    if(!impl) {
        return zfnull;
    }
    zfobj<_ZFP_I_ZFInputForImplOwner> owner;
    owner->impl = impl;
    ZFInput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFInputForImplOwner, onInput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

