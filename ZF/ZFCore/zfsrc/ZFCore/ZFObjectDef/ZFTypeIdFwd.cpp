#include "ZFTypeIdFwd.h"
#include "ZFObjectImpl.h"
#include "ZFListenerDeclare.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef zfstlhashmap<zfstring, ZFTypeInfo *> _ZFP_ZFTypeInfoMapType;
ZF_STATIC_INITIALIZER_INIT(ZFTypeInfoHolder) {
}
ZF_STATIC_INITIALIZER_DESTROY(ZFTypeInfoHolder) {
    ZFCoreAssert(m.empty());
}
_ZFP_ZFTypeInfoMapType m;
ZF_STATIC_INITIALIZER_END(ZFTypeInfoHolder)

void _ZFP_ZFTypeInfoRegister(
        ZF_IN const zfstring &typeId
        , ZF_IN ZFTypeInfo *typeIdData
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFTypeInfoMapType &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeInfoHolder)->m;
    ZFCoreAssert(m.find(typeId) == m.end());
    m[typeIdData->typeId()] = typeIdData;
}
ZFTypeInfo *_ZFP_ZFTypeInfoUnregister(ZF_IN const zfstring &typeId) {
    ZFCoreMutexLocker();
    _ZFP_ZFTypeInfoMapType &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeInfoHolder)->m;
    _ZFP_ZFTypeInfoMapType::iterator it = m.find(typeId);
    ZFCoreAssert(it != m.end());
    ZFTypeInfo *t = it->second;
    m.erase(it);
    return t;
}
const ZFTypeInfo *ZFTypeInfoForName(ZF_IN const zfstring &typeId) {
    ZFCoreMutexLocker();
    _ZFP_ZFTypeInfoMapType &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeInfoHolder)->m;
    _ZFP_ZFTypeInfoMapType::iterator it = m.find(typeId);
    if(it != m.end()) {
        return it->second;
    }
    else {
        return zfnull;
    }
}
void ZFTypeInfoGetAllT(ZF_IN_OUT ZFCoreArray<const ZFTypeInfo *> &ret) {
    ZFCoreMutexLocker();
    _ZFP_ZFTypeInfoMapType &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeInfoHolder)->m;
    for(_ZFP_ZFTypeInfoMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->second);
    }
}

// ============================================================
// special alias implicit convert
zfclass _ZFP_I_PropAliasHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_PropAliasHolder, ZFObject)
public:
    ZFObject *obj;
    void *v;
    _ZFP_PropAliasDetachCallback detachCallback;
    ZFListener ownerOnDealloc;
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->cleanup();
        zfsuper::objectOnDeallocPrepare();
    }
private:
    void cleanup(void) {
        if(this->ownerOnDealloc) {
            this->obj->observerRemove(ZFObject::E_ObjectBeforeDealloc(), this->ownerOnDealloc);
            this->ownerOnDealloc = zfnull;
        }
        if(this->detachCallback != zfnull) {
            _ZFP_PropAliasDetachCallback detachCallbackTmp = this->detachCallback;
            this->detachCallback = zfnull;
            detachCallbackTmp(this->obj, this->v);
        }
    }
};
ZFOBJECT_REGISTER(_ZFP_I_PropAliasHolder)

void _ZFP_PropAliasAttach(
        ZF_IN ZFObject *obj
        , ZF_IN void *v
        , ZF_IN const zfstring &tagKey
        , ZF_IN _ZFP_PropAliasDetachCallback detachCallback
        ) {
    _ZFP_I_PropAliasHolder *d = zfAlloc(_ZFP_I_PropAliasHolder);
    d->obj = obj;
    d->v = v;
    d->detachCallback = detachCallback;
    obj->objectTag(tagKey, d);
    zfRelease(d);

    // solve this issue:
    // 1. attach
    // 2. owner obj dealloc
    // 3. call detachCallback
    //     3.1. owner obj got retained due to alias type's zfvAccess
    //     3.2. crash due to obj got retained during dealloc
    //
    // it's designed safe to retain during E_ObjectBeforeDealloc, but not during objectOnDealloc
    ZFLISTENER_2(ownerOnDealloc
            , ZFObject *, obj
            , zfstring, tagKey
            ) {
        obj->objectTagRemove(tagKey);
    } ZFLISTENER_END()
    d->ownerOnDealloc = ownerOnDealloc;
    obj->observerAddForOnce(ZFObject::E_ObjectBeforeDealloc(), ownerOnDealloc);
}
void _ZFP_PropAliasDetach(
        ZF_IN ZFObject *obj
        , ZF_IN const zfstring &tagKey
        ) {
    obj->objectTagRemove(tagKey);
}

ZF_NAMESPACE_GLOBAL_END

