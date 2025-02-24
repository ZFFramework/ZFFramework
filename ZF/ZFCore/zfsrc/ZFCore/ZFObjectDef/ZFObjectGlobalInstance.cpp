#include "ZFObjectGlobalInstance.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFObjectGlobalInstanceDefine_(Name, Level) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, Level) { \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(Name) { \
        while(!this->instances.isEmpty()) { \
            ZFCoreArray<const ZFCorePointerBase *> tmp; \
            tmp.copyFrom(this->instances); \
            this->instances.removeAll(); \
            for(zfindex i = 0; i < tmp.count(); ++i) { \
                tmp[i]->refDelete(); \
            } \
        } \
    } \
    public: \
        ZFCoreArray<const ZFCorePointerBase *> instances; \
    ZF_GLOBAL_INITIALIZER_END(Name)
#define _ZFP_ZFObjectGlobalInstanceDefine(Name, Level) \
    _ZFP_ZFObjectGlobalInstanceDefine_(Name, Level)

_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_ZFFrameworkEssential, ZFLevelZFFrameworkEssential)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_ZFFrameworkHigh, ZFLevelZFFrameworkHigh)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_ZFFrameworkNormal, ZFLevelZFFrameworkNormal)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_ZFFrameworkLow, ZFLevelZFFrameworkLow)

_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_AppEssential, ZFLevelAppEssential)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_AppHigh, ZFLevelAppHigh)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_AppNormal, ZFLevelAppNormal)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_AppLow, ZFLevelAppLow)

_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_ZFFrameworkPostLow, ZFLevelZFFrameworkPostLow)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_ZFFrameworkPostNormal, ZFLevelZFFrameworkPostNormal)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_ZFFrameworkPostHigh, ZFLevelZFFrameworkPostHigh)
_ZFP_ZFObjectGlobalInstanceDefine(ObjGI_ZFFrameworkPostEssential, ZFLevelZFFrameworkPostEssential)

static ZFCoreArray<const ZFCorePointerBase *> &_ZFP_ZFObjectGlobalInstanceRef(ZF_IN ZFLevel level) {
    switch(level) {
        case ZFLevelZFFrameworkEssential:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_ZFFrameworkEssential)->instances;
        case ZFLevelZFFrameworkHigh:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_ZFFrameworkHigh)->instances;
        case ZFLevelZFFrameworkNormal:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_ZFFrameworkNormal)->instances;
        case ZFLevelZFFrameworkLow:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_ZFFrameworkLow)->instances;

        case ZFLevelAppEssential:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_AppEssential)->instances;
        case ZFLevelAppHigh:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_AppHigh)->instances;
        case ZFLevelAppNormal:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_AppNormal)->instances;
        case ZFLevelAppLow:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_AppLow)->instances;

        case ZFLevelZFFrameworkPostLow:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_ZFFrameworkPostLow)->instances;
        case ZFLevelZFFrameworkPostNormal:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_ZFFrameworkPostNormal)->instances;
        case ZFLevelZFFrameworkPostHigh:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_ZFFrameworkPostHigh)->instances;
        case ZFLevelZFFrameworkPostEssential:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_ZFFrameworkPostEssential)->instances;

        default:
            ZFCoreCriticalShouldNotGoHere();
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ObjGI_AppLow)->instances;
    }
}

const ZFCorePointerBase *ZFObjectGlobalInstanceAdd(
        ZF_IN const ZFCorePointerBase &sp
        , ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */
        ) {
    const ZFCorePointerBase *t = sp.refNew();
    _ZFP_ZFObjectGlobalInstanceRef(level).add(t);
    return t;
}

const ZFCorePointerBase *ZFObjectGlobalInstanceAdd(
        ZF_IN ZFObject *obj
        , ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */
        ) {
    if(obj) {
        return ZFObjectGlobalInstanceAdd(ZFCorePointerForZFObject<ZFObject *>(obj), level);
    }
    else {
        return zfnull;
    }
}

void ZFObjectGlobalInstanceRemove(
        ZF_IN const ZFCorePointerBase *sp
        , ZF_IN ZFLevel level
        ) {
    ZFCoreArray<const ZFCorePointerBase *> &instances = _ZFP_ZFObjectGlobalInstanceRef(level);
    zfindex index = instances.find(sp);
    if(index != zfindexMax()) {
        instances.remove(index);
        sp->refDelete();
    }
}

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFClassSingletonInstanceRefHolder) {
}
ZFCoreMap singletonInstanceMap; // _ZFP_ZFClassSingletonPointerHolder *
ZF_STATIC_INITIALIZER_END(ZFClassSingletonInstanceRefHolder)

_ZFP_ZFClassSingletonPointerHolder *_ZFP_ZFClassSingletonInstanceRefAccess(ZF_IN const zfstring &sig) {
    ZFCoreMutexLocker();
    ZFCoreMap &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFClassSingletonInstanceRefHolder)->singletonInstanceMap;
    const ZFCorePointerBase *v = m.get(sig);
    if(v == zfnull) {
        m.set(sig, ZFCorePointerForObject<_ZFP_ZFClassSingletonPointerHolder *>(zfnew(_ZFP_ZFClassSingletonPointerHolder)));
        v = m.get(sig);
    }
    return v->pointerValueT<_ZFP_ZFClassSingletonPointerHolder *>();
}

ZF_NAMESPACE_GLOBAL_END

