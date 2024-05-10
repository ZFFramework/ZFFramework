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
            ZFClass::_ZFP_ZFClassUnregister(zfnull, it->first);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFClassDynamicRegisterAutoRemove)

// ============================================================
const ZFClass *ZFClassDynamicRegister(
        ZF_IN const zfchar *classNameFull
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
    if(zfstringIsEmpty(classNameFull)) {
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
    zfindex dotPos = zfstringFindReversely(classNameFull, zfindexMax(), ZFNamespaceSeparator());
    cls = ZFClass::_ZFP_ZFClassRegister(
        zfnull,
        dotPos == zfindexMax() ? zfnull : zfstring(classNameFull, dotPos).cString(),
        dotPos == zfindexMax() ? classNameFull : classNameFull + dotPos + ZFNamespaceSeparatorLen(),
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
    cls->_ZFP_ZFClass_autoRegister();
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterDataHolder)->m[cls] = zftrue;
    _ZFP_ZFNamespaceRegister(zfnull, ZFNamespaceSkipGlobal(cls->classNamespace()));
    return cls;
}
void ZFClassDynamicUnregister(ZF_IN const ZFClass *cls) {
    if(cls == zfnull) {
        return;
    }
    if(!cls->classIsDynamicRegister()) {
        zfCoreCriticalMessageTrim(
            "[ZFClassDynamicRegister] unregistering class %s that is not dyanmiac registered",
            cls);
    }
    _ZFP_ZFNamespaceUnregister(ZFNamespaceSkipGlobal(cls->classNamespace()));
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterDataHolder)->m.erase(cls);
    cls->classTagRemoveAll();
    ZFClass::_ZFP_ZFClassUnregister(zfnull, cls);
}

// ============================================================
void ZFImplementDynamicRegister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFClass *clsToImplement
        , ZF_OUT_OPT ZFCoreArray<const ZFMethod *> *methodRegistered /* = zfnull */
        , ZF_OUT_OPT ZFCoreArray<const ZFProperty *> *propertyRegistered /* = zfnull */
        ) {
    if(cls == zfnull || clsToImplement == zfnull || cls == clsToImplement) {
        return;
    }
    zfCoreMutexLocker();

    ZFCoreArray<const ZFMethod *> allPropertyMethod;
    ZFCoreArray<const ZFProperty *> allProperty;
    clsToImplement->propertyGetAllT(allProperty);
    for(zfindex iProperty = 0; iProperty < allProperty.count(); ++iProperty) {
        const ZFProperty *propertyToImplement = allProperty[iProperty];
        if(!propertyToImplement->propertyIsDynamicRegister()) {
            continue;
        }
        const ZFProperty *property = ZFPropertyDynamicRegister(ZFPropertyDynamicRegisterParam()
                .propertyDynamicRegisterUserData(propertyToImplement->propertyDynamicRegisterUserData())
                .propertyInitValueCallback(propertyToImplement->callbackDynamicRegisterInitValueGetter)
                .propertyOwnerClass(cls)
                .propertyTypeId(propertyToImplement->propertyTypeId())
                .propertyTypeName(propertyToImplement->propertyTypeName())
                .propertyName(propertyToImplement->propertyName())
                .propertyClassOfRetainProperty(propertyToImplement->propertyClassOfRetainProperty())
                .propertySetterType(propertyToImplement->setterMethod()->methodPrivilegeType())
                .propertyGetterType(propertyToImplement->getterMethod()->methodPrivilegeType())
                );
        if(property != zfnull) {
            allPropertyMethod.add(propertyToImplement->setterMethod());
            allPropertyMethod.add(propertyToImplement->getterMethod());
        }
        if(property != zfnull && propertyRegistered != zfnull) {
            propertyRegistered->add(property);
        }
    }

    ZFCoreArray<const ZFMethod *> allMethod;
    clsToImplement->methodGetAllT(allMethod);
    for(zfindex iMethod = 0; iMethod < allMethod.count(); ++iMethod) {
        const ZFMethod *methodToImplement = allMethod[iMethod];
        if(!methodToImplement->methodIsDynamicRegister()
                || allPropertyMethod.find(methodToImplement) != zfindexMax()
                ) {
            continue;
        }
        ZFMethodDynamicRegisterParam param;
        param
                .methodDynamicRegisterUserData(methodToImplement->methodDynamicRegisterUserData())
                .methodOwnerClass(cls)
                .methodGenericInvoker(methodToImplement->methodGenericInvokerOrig())
                .methodType(methodToImplement->methodType())
                .methodPrivilegeType(methodToImplement->methodPrivilegeType())
                .methodName(methodToImplement->methodName())
                .methodReturnTypeId(methodToImplement->methodReturnTypeId())
                .methodReturnTypeName(methodToImplement->methodReturnTypeName())
                ;
        for(zfindex i = 0; i < methodToImplement->methodParamCount(); ++i) {
            if(methodToImplement->_ZFP_ZFMethod_paramDefaultValueList[i] != zfnull) {
                param.methodParamAddWithDefault(
                        methodToImplement->methodParamTypeIdAt(i)
                        , methodToImplement->methodParamTypeNameAt(i)
                        , methodToImplement->methodParamNameAt(i)
                        , methodToImplement->_ZFP_ZFMethod_paramDefaultValueList[i]
                        );
            }
            else {
                param.methodParamAdd(
                        methodToImplement->methodParamTypeIdAt(i)
                        , methodToImplement->methodParamTypeNameAt(i)
                        , methodToImplement->methodParamNameAt(i)
                        , methodToImplement->methodParamDefaultValueCallbackAt(i)
                        );
            }
        }
        const ZFMethod *method = ZFMethodDynamicRegister(param);
        if(method != zfnull && methodRegistered != zfnull) {
            methodRegistered->add(method);
        }
    }
}
void ZFImplementDynamicUnregister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFClass *clsToImplement
        ) {
    if(cls == zfnull || clsToImplement == zfnull || cls == clsToImplement) {
        return;
    }
    zfCoreMutexLocker();

    ZFCoreArray<const ZFProperty *> allProperty;
    clsToImplement->propertyGetAllT(allProperty);
    for(zfindex iProperty = 0; iProperty < allProperty.count(); ++iProperty) {
        const ZFProperty *propertyToImplement = allProperty[iProperty];
        if(!propertyToImplement->propertyIsDynamicRegister()) {
            continue;
        }
        const ZFProperty *property = cls->propertyForName(propertyToImplement->propertyName());
        if(property == zfnull
                || !property->propertyIsDynamicRegister()
                || !zfstringIsEqual(property->propertyTypeId(), propertyToImplement->propertyTypeId())
                ) {
            continue;
        }
        ZFPropertyDynamicUnregister(property);
    }

    ZFCoreArray<const ZFMethod *> allMethod;
    clsToImplement->methodGetAllT(allMethod);
    for(zfindex iMethod = 0; iMethod < allMethod.count(); ++iMethod) {
        const ZFMethod *methodToImplement = allMethod[iMethod];
        if(!methodToImplement->methodIsDynamicRegister()) {
            continue;
        }
        const ZFMethod *method = cls->methodForName(methodToImplement->methodName()
                , methodToImplement->methodParamTypeIdAt(0)
                , methodToImplement->methodParamTypeIdAt(1)
                , methodToImplement->methodParamTypeIdAt(2)
                , methodToImplement->methodParamTypeIdAt(3)
                , methodToImplement->methodParamTypeIdAt(4)
                , methodToImplement->methodParamTypeIdAt(5)
                , methodToImplement->methodParamTypeIdAt(6)
                , methodToImplement->methodParamTypeIdAt(7)
                ); // ZFMETHOD_MAX_PARAM
        if(method == zfnull
                || !method->methodIsDynamicRegister()
                || method->methodType() != methodToImplement->methodType()
                || !zfstringIsEqual(method->methodReturnTypeId(), methodToImplement->methodReturnTypeId())
                ) {
            continue;
        }
        ZFMethodDynamicUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(const ZFClass *, ZFClassDynamicRegister
        , ZFMP_IN(const zfchar *, classNameFull)
        , ZFMP_IN_OPT(const ZFClass *, parent, zfnull)
        , ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFClassDynamicUnregister
        , ZFMP_IN(const ZFClass *, cls)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFImplementDynamicRegister
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const ZFClass *, clsToImplement)
        , ZFMP_OUT_OPT(ZFCoreArray<const ZFMethod *> *, methodRegistered, zfnull)
        , ZFMP_OUT_OPT(ZFCoreArray<const ZFProperty *> *, propertyRegistered, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFImplementDynamicUnregister
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const ZFClass *, clsToImplement)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

