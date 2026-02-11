#include "ZFOutputForImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFOutputForImplOwner : zfextend ZFObject {
public:
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputForImplOwner, ZFObject)

    ZFOBJECT_CACHE_RELEASE({
        cache->_srcCache->zfv.removeAll();
        cache->_countCache->zfv = 0;
        cache->_resultCache->zfv = 0;
        cache->impl = zfnull;
    })

public:
    ZFListener impl;

private:
    v_zfstring *_srcCache;
    v_zfindex *_countCache;
    v_zfindex *_resultCache;

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->_srcCache = zfunsafe_zfobjAlloc(v_zfstring);
        this->_countCache = zfunsafe_zfobjAlloc(v_zfindex);
        this->_resultCache = zfunsafe_zfobjAlloc(v_zfindex);
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        zfunsafe_zfobjRelease(this->_srcCache);
        zfunsafe_zfobjRelease(this->_countCache);
        zfunsafe_zfobjRelease(this->_resultCache);
        zfsuper::objectOnDealloc();
    }

protected:
    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_IN(const void *, src)
            , ZFMP_IN_OPT(zfindex, count, zfindexMax())
            )
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForImplOwner, zfindex, onOutput
        , ZFMP_IN(const void *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    if(this->impl) {
        if(count == zfindexMax()) {
            count = zfslen((const zfchar *)src);
        }
        this->_srcCache->zfv.assign((const zfchar *)src, count);
        this->_countCache->zfv = count;
        this->_resultCache->zfv = count;
        this->impl.execute(ZFArgs()
                .param0(this->_srcCache)
                .param1(this->_countCache)
                .result(this->_resultCache)
            );
        return this->_resultCache->zfv;
    }
    else {
        return 0;
    }
}

ZFMETHOD_FUNC_DEFINE_1(ZFOutput, ZFOutputForImpl
        , ZFMP_IN(const ZFListener &, impl)
        ) {
    if(!impl) {
        return zfnull;
    }
    zfobj<_ZFP_I_ZFOutputForImplOwner> owner;
    owner->impl = impl;
    ZFOutput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFOutputForImplOwner, onOutput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

