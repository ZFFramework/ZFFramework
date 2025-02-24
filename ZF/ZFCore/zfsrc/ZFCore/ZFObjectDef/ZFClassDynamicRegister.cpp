#include "ZFClassDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic) {
}
zfstlmap<const ZFClass *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFClassDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassDynamicRegisterAutoRemove) {
    zfstlmap<const ZFClass *, zfbool> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterDataHolder)->m;
    if(!m.empty()) {
        zfstlmap<const ZFClass *, zfbool> t;
        t.swap(m);
        for(zfstlmap<const ZFClass *, zfbool>::iterator it = t.begin(); it != t.end(); ++it) {
            ZFClass::_ZFP_ZFClassUnregister(it->first);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFClassDynamicRegisterAutoRemove)

// ============================================================
const ZFClass *ZFClassDynamicRegister(
        ZF_IN const zfstring &classNameFull
        , ZF_IN_OPT const ZFClass *parent /* = zfnull */
        , ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(parent == zfnull) {
        parent = ZFObject::ClassData();
    }
    if(parent->classIsAbstract()) {
        zfstringAppend(errorHint,
            "parent must not be abstract: %s",
            parent);
        return zfnull;
    }
    if(!classNameFull) {
        zfstringAppend(errorHint, "null classNameFull");
        return zfnull;
    }
    const ZFClass *cls = ZFClass::classForName(classNameFull);
    if(cls != zfnull) {
        zfstringAppend(errorHint,
            "class %s already exist while registering to parent: %s",
            cls,
            parent);
        return zfnull;
    }
    zfindex dotPos = zfstringFindReversely(classNameFull, zfindexMax(), ".");
    cls = ZFClass::_ZFP_ZFClassRegister(
        dotPos == zfindexMax() ? zfnull : zfstring(classNameFull, dotPos).cString(),
        dotPos == zfindexMax() ? classNameFull : zfstring(classNameFull + dotPos + 1),
        parent,
        zfnull,
        zftrue,
        parent->_ZFP_objectAllocWithCacheCallback(),
        parent->_ZFP_objectConstructor(),
        parent->_ZFP_objectDestructor(),
        zfnull,
        zffalse,
        zftrue,
        classDynamicRegisterUserData);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterDataHolder)->m[cls] = zftrue;
    const zfchar *classNamespace = ZFNamespaceSkipGlobal(cls->classNamespace());
    if(classNamespace && ZFClass::classForName(classNamespace) == zfnull) {
        _ZFP_ZFNamespaceRegister(zfnull, classNamespace);
    }
    return cls;
}
void ZFClassDynamicUnregister(ZF_IN const ZFClass *cls) {
    if(cls == zfnull) {
        return;
    }
    if(!cls->classIsDynamicRegister()) {
        ZFCoreCriticalMessageTrim(
            "[ZFClassDynamicRegister] unregistering class %s that is not dyanmiac registered",
            cls);
    }
    _ZFP_ZFNamespaceUnregister(ZFNamespaceSkipGlobal(cls->classNamespace()));
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterDataHolder)->m.erase(cls);
    cls->classTagRemoveAll();
    ZFClass::_ZFP_ZFClassUnregister(cls);
}

// ============================================================
zfbool ZFImplementDynamicRegister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFClass *clsToImplement
        ) {
    return cls != zfnull && clsToImplement != zfnull && cls->_ZFP_ZFClass_ZFImplementDynamicRegister(clsToImplement);
}
void ZFImplementDynamicUnregister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFClass *clsToImplement
        ) {
    if(cls != zfnull && clsToImplement != zfnull) {
        cls->_ZFP_ZFClass_ZFImplementDynamicUnregister(clsToImplement);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(const ZFClass *, ZFClassDynamicRegister
        , ZFMP_IN(const zfstring &, classNameFull)
        , ZFMP_IN_OPT(const ZFClass *, parent, zfnull)
        , ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFClassDynamicUnregister
        , ZFMP_IN(const ZFClass *, cls)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFImplementDynamicRegister
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const ZFClass *, clsToImplement)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFImplementDynamicUnregister
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const ZFClass *, clsToImplement)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

