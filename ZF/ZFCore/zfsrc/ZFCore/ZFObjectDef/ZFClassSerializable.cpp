#include "ZFClassSerializable.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFClassInstanceObserverAddParam, ZFClassInstanceObserverAddParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, ZFListener, observer)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, zfany, owner)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, ZFLevel, observerLevel)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, zfbool , observeAllChildType)

ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFClass, const ZFClass *, {
        v = ZFClass::classForName((srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString());
        if(v == zfnull) {
            if(errorHint) {
                zfstringAppend(errorHint, "no class for name: \"%s\"", zfstring(src, srcLen));
            }
            return zffalse;
        }
        return zftrue;
    }, {
        if(v) {
            s += v->classNameFull();
        }
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_1(ZFClass, v_ZFClass, const ZFClass *, classForName
        , ZFMP_IN(const zfstring &, className)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_2(ZFClass, v_ZFClass, const ZFClass *, classForName
        , ZFMP_IN(const zfstring &, className)
        , ZFMP_IN(const zfstring &, classNamespace)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFClass, void, instanceObserverAdd
    , ZFMP_IN(const ZFListener &, observer)
    , ZFMP_IN_OPT(ZFObject *, owner, zfnull)
    , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
    , ZFMP_IN_OPT(zfbool, observeAllChildType, zftrue)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, instanceObserverAdd
        , ZFMP_IN(const ZFClassInstanceObserverAddParam &, param)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, instanceObserverRemove
        , ZFMP_IN(const ZFListener &, observer)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, instanceObserverRemoveByOwner
        , ZFMP_IN(ZFObject *, owner)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, void, instanceObserverRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, classDataChangeAutoRemoveTagAdd
        , ZFMP_IN(const zfstring &, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, classDataChangeAutoRemoveTagRemove
        , ZFMP_IN(const zfstring &, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, objectInfoOfInheritTreeT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfstring, objectInfoOfInheritTree)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, zfbool, classIsTypeOf
        , ZFMP_IN(const ZFClass *, cls)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, const zfstring &, classNamespace)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, const zfstring &, className)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, const zfstring &, classNameFull)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, const ZFClass *, classParent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classIsAbstract)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classIsInterface)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classIsInternal)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classIsInternalPrivate)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classCanAllocPublic)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfauto, newInstance)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFClass, zfauto, newInstance
    , ZFMP_IN(ZFObject *, param0)
    , ZFMP_IN_OPT(ZFObject *, param1, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param2, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param3, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param4, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param5, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param6, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param7, ZFMethodGenericInvokerDefaultParam())
    // ZFMETHOD_MAX_PARAM
    // , ZFMP_OUT_OPT(zfbool *, success, zfnull)
    // , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFClass, zfauto, newInstanceDetail
    , ZFMP_IN(const ZFCoreArray<zfauto> &, params)
    , ZFMP_OUT_OPT(zfbool *, success, zfnull)
    , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfindex, implementedInterfaceCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFClass *, implementedInterfaceAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfindex, dynamicInterfaceCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFClass *, dynamicInterfaceAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfindex, methodCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFMethod *, methodAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, methodGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, ZFCoreArray<const ZFMethod *>, methodGetAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFClass, const ZFMethod *, methodForNameIgnoreParent
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId0, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull)
        // , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull)
        ) /* ZFMETHOD_MAX_PARAM */
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFClass, const ZFMethod *, methodForName
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId0, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull)
        // , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull)
        ) /* ZFMETHOD_MAX_PARAM */
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFClass, void, methodForNameGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret)
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, ZFCoreArray<const ZFMethod *>, methodForNameGetAll
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfindex, propertyCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFProperty *, propertyAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, propertyGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, ZFCoreArray<const ZFProperty *>, propertyGetAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFProperty *, propertyForNameIgnoreParent
        , ZFMP_IN(const zfstring &, propertyName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFProperty *, propertyForName
        , ZFMP_IN(const zfstring &, propertyName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, propertyHasOverrideInitStep)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, zfbool, propertyHasOverrideInitStep
        , ZFMP_IN(const ZFProperty *, property)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFClass, void, classTag
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFObject *, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, zfany, classTag
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFClass, void, classTagGetAllKeyValue
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, allKey)
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, allValue)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, classTagRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, zfauto, classTagRemoveAndGet
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, void, classTagRemoveAll)

ZF_NAMESPACE_GLOBAL_END

