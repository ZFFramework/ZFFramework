#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFClassDataUpdateType, ZFClassDataUpdateType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFClassDataUpdateTypeAttach,
            ZFTOKEN_ZFClassDataUpdateTypeDetach,
            ZFTOKEN_ZFClassDataUpdateTypeClassAliasAttach,
            ZFTOKEN_ZFClassDataUpdateTypeClassAliasDetach,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFClassDataUpdateTypeAttach;
        switch(matched) {
            case 0:
                v = ZFClassDataUpdateTypeAttach;
                return zftrue;
            case 1:
                v = ZFClassDataUpdateTypeDetach;
                return zftrue;
            case 2:
                v = ZFClassDataUpdateTypeClassAliasAttach;
                return zftrue;
            case 3:
                v = ZFClassDataUpdateTypeClassAliasDetach;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFClassDataUpdateTypeAttach:
                s += ZFTOKEN_ZFClassDataUpdateTypeAttach;
                return zftrue;
            case ZFClassDataUpdateTypeDetach:
                s += ZFTOKEN_ZFClassDataUpdateTypeDetach;
                return zftrue;
            case ZFClassDataUpdateTypeClassAliasAttach:
                s += ZFTOKEN_ZFClassDataUpdateTypeClassAliasAttach;
                return zftrue;
            case ZFClassDataUpdateTypeClassAliasDetach:
                s += ZFTOKEN_ZFClassDataUpdateTypeClassAliasDetach;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_ENUM_DEFINE(ZFClassDataUpdateType
        , ZFClassDataUpdateTypeAttach
        , ZFClassDataUpdateTypeDetach
        , ZFClassDataUpdateTypeClassAliasAttach
        , ZFClassDataUpdateTypeClassAliasDetach
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFClassDataUpdateData, ZFClassDataUpdateData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataUpdateData, ZFClassDataUpdateType, changeType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataUpdateData, const ZFClass *, changedClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataUpdateData, const ZFProperty *, changedProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataUpdateData, const ZFMethod *, changedMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataUpdateData, zfstring, name)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFFilterForZFClassType, ZFFilterForZFClassType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFFilterForZFClassTypeInclude,
            ZFTOKEN_ZFFilterForZFClassTypeExclude,
            ZFTOKEN_ZFFilterForZFClassTypeIncludeChildOf,
            ZFTOKEN_ZFFilterForZFClassTypeIncludeParentOf,
            ZFTOKEN_ZFFilterForZFClassTypeExcludeChildOf,
            ZFTOKEN_ZFFilterForZFClassTypeExcludeParentOf,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFFilterForZFClassTypeInclude;
        switch(matched) {
            case 0:
                v = ZFFilterForZFClassTypeInclude;
                return zftrue;
            case 1:
                v = ZFFilterForZFClassTypeExclude;
                return zftrue;
            case 2:
                v = ZFFilterForZFClassTypeIncludeChildOf;
                return zftrue;
            case 3:
                v = ZFFilterForZFClassTypeIncludeParentOf;
                return zftrue;
            case 4:
                v = ZFFilterForZFClassTypeExcludeChildOf;
                return zftrue;
            case 5:
                v = ZFFilterForZFClassTypeExcludeParentOf;
                return zftrue;
            case 6:
                v = ZFFilterForZFClassTypeExclude;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFFilterForZFClassTypeInclude:
                s += ZFTOKEN_ZFFilterForZFClassTypeInclude;
                return zftrue;
            case ZFFilterForZFClassTypeExclude:
                s += ZFTOKEN_ZFFilterForZFClassTypeExclude;
                return zftrue;
            case ZFFilterForZFClassTypeIncludeChildOf:
                s += ZFTOKEN_ZFFilterForZFClassTypeIncludeChildOf;
                return zftrue;
            case ZFFilterForZFClassTypeIncludeParentOf:
                s += ZFTOKEN_ZFFilterForZFClassTypeIncludeParentOf;
                return zftrue;
            case ZFFilterForZFClassTypeExcludeChildOf:
                s += ZFTOKEN_ZFFilterForZFClassTypeExcludeChildOf;
                return zftrue;
            case ZFFilterForZFClassTypeExcludeParentOf:
                s += ZFTOKEN_ZFFilterForZFClassTypeExcludeParentOf;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_ENUM_DEFINE(ZFFilterForZFClassType
        , ZFFilterForZFClassTypeInclude
        , ZFFilterForZFClassTypeExclude
        , ZFFilterForZFClassTypeIncludeChildOf
        , ZFFilterForZFClassTypeIncludeParentOf
        , ZFFilterForZFClassTypeExcludeChildOf
        , ZFFilterForZFClassTypeExcludeParentOf
        )

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFMethodPrivilegeType, ZFMethodPrivilegeType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFMethodPrivilegeTypePublic,
            ZFTOKEN_ZFMethodPrivilegeTypeProtected,
            ZFTOKEN_ZFMethodPrivilegeTypePrivate,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFMethodPrivilegeTypePublic;
        switch(matched) {
            case 0:
                v = ZFMethodPrivilegeTypePublic;
                return zftrue;
            case 1:
                v = ZFMethodPrivilegeTypeProtected;
                return zftrue;
            case 2:
                v = ZFMethodPrivilegeTypePrivate;
                return zftrue;
            case 3:
                v = ZFMethodPrivilegeTypePublic;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFMethodPrivilegeTypePublic:
                s += ZFTOKEN_ZFMethodPrivilegeTypePublic;
                return zftrue;
            case ZFMethodPrivilegeTypeProtected:
                s += ZFTOKEN_ZFMethodPrivilegeTypeProtected;
                return zftrue;
            case ZFMethodPrivilegeTypePrivate:
                s += ZFTOKEN_ZFMethodPrivilegeTypePrivate;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_ENUM_DEFINE(ZFMethodPrivilegeType
        , ZFMethodPrivilegeTypePublic
        , ZFMethodPrivilegeTypeProtected
        , ZFMethodPrivilegeTypePrivate
        )

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFMethodType, ZFMethodType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFMethodTypeNormal,
            ZFTOKEN_ZFMethodTypeStatic,
            ZFTOKEN_ZFMethodTypeVirtual,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFMethodTypeNormal;
        switch(matched) {
            case 0:
                v = ZFMethodTypeNormal;
                return zftrue;
            case 1:
                v = ZFMethodTypeStatic;
                return zftrue;
            case 2:
                v = ZFMethodTypeVirtual;
                return zftrue;
            case 3:
                v = ZFMethodTypeVirtual;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFMethodTypeNormal:
                s += ZFTOKEN_ZFMethodTypeNormal;
                return zftrue;
            case ZFMethodTypeStatic:
                s += ZFTOKEN_ZFMethodTypeStatic;
                return zftrue;
            case ZFMethodTypeVirtual:
                s += ZFTOKEN_ZFMethodTypeVirtual;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_ENUM_DEFINE(ZFMethodType
        , ZFMethodTypeNormal
        , ZFMethodTypeStatic
        , ZFMethodTypeVirtual
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFArgs, ZFArgs)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFArgs, zfidentity, eventId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, sender
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, zfany, sender)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, param0
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, zfany, param0)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, param1
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, zfany, param1)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, result
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, zfany, result)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, eventFiltered
        , ZFMP_IN(zfbool, eventFiltered)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, zfbool, eventFiltered)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFObserver, ZFObserver)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserver, void, observerAdd
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserver, void, observerAddForOnce
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserver, void, on
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserver, void, once
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFObserver, void, observerRemove
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserver, void, observerRemoveAll
        , ZFMP_IN(zfidentity, eventId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserver, void, observerRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserver, zfbool, observerHasAdd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserver, zfbool, observerHasAdd
        , ZFMP_IN(zfidentity, eventId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserver, void, observerNotify
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFObserver, void, observerNotifyWithSender
        , ZFMP_IN(ZFObject *, customSender)
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserver, zfany, observerOwner)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFClass, ZFFilterForZFClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, void, copyFrom
        , ZFMP_IN(ZFFilterForZFClass const &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFClass, void, filter
        , ZFMP_IN(const ZFClass * const &, e)
        , ZFMP_IN_OPT(ZFFilterForZFClassType, filterType, ZFFilterForZFClassTypeExclude)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFClass, void, filterRemove
        , ZFMP_IN(const ZFClass * const &, e)
        , ZFMP_IN_OPT(ZFFilterForZFClassType, filterType, ZFFilterForZFClassTypeExclude)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, void, filterRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFClass, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFClass, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, const ZFClass *, filterElementAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, ZFFilterForZFClassType, filterTypeAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, zfbool, filterPassed
        , ZFMP_IN(const ZFClass * const &, e)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFMethod, ZFFilterForZFMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, void, copyFrom
        , ZFMP_IN(ZFFilterForZFMethod const &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFMethod, void, filter
        , ZFMP_IN(const ZFMethod * const &, e)
        , ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFMethod, void, filterRemove
        , ZFMP_IN(const ZFMethod * const &, e)
        , ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, void, filterRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFMethod, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFMethod, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, const ZFMethod *, filterElementAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, ZFFilterType, filterTypeAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, zfbool, filterPassed
        , ZFMP_IN(const ZFMethod * const &, e)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFProperty, ZFFilterForZFProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, void, copyFrom
        , ZFMP_IN(ZFFilterForZFProperty const &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFProperty, void, filter
        , ZFMP_IN(const ZFProperty * const &, e)
        , ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFProperty, void, filterRemove
        , ZFMP_IN(const ZFProperty * const &, e)
        , ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, void, filterRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFProperty, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFProperty, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, const ZFProperty *, filterElementAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, ZFFilterType, filterTypeAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, zfbool, filterPassed
        , ZFMP_IN(const ZFProperty * const &, e)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFObject, ZFFilterForZFObject)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, void, copyFrom
        , ZFMP_IN(ZFFilterForZFObject const &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFObject, void, filter
        , ZFMP_IN(ZFObject * const &, e)
        , ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFObject, void, filterRemove
        , ZFMP_IN(ZFObject * const &, e)
        , ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, void, filterRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFObject, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFObject, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, zfany, filterElementAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, ZFFilterType, filterTypeAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, zfbool, filterPassed
        , ZFMP_IN(ZFObject * const &, e)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMethodGenericInvoker, ZFMethodGenericInvoker)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, dynamicRegisterUserData
        , ZFMP_IN(ZFObject *, dynamicRegisterUserData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, zfany, dynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, ownerClass
        , ZFMP_IN(const ZFClass *, ownerClass)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const ZFClass *, ownerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodNamespace
        , ZFMP_IN(const zfstring &, methodNamespace)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfstring &, methodNamespace)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodGenericInvoker
        , ZFMP_IN(ZFMethodGenericInvoker, methodGenericInvoker)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodGenericInvoker, methodGenericInvoker)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodImpl
        , ZFMP_IN(const ZFListener &, methodImpl)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const ZFListener &, methodImpl)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodType
        , ZFMP_IN(ZFMethodType, methodType)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodType, methodType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodPrivilegeType
        , ZFMP_IN(ZFMethodPrivilegeType, methodPrivilegeType)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodPrivilegeType, methodPrivilegeType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodName
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfstring &, methodName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, returnTypeId
        , ZFMP_IN(const zfstring &, returnTypeId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfstring &, returnTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFMethodDynamicRegisterParam, void, methodParam
        , ZFMP_IN(const zfstring &, paramTypeId)
        , ZFMP_IN_OPT(const zfstring &, paramName, zfnull)
        , ZFMP_IN_OPT(const ZFListener &, paramDefaultValueCallback, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, zfindex, paramCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfstring &, paramTypeIdAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfstring &, paramNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const ZFListener &, paramDefaultValueCallbackAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam &, zfunsafe_disableChecker
        , ZFMP_IN(zfbool, disableChecker)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, zfbool, zfunsafe_disableChecker)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMP, ZFMP)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFMP, ZFMP &, mp
        , ZFMP_IN(const zfstring &, paramTypeId)
        , ZFMP_IN_OPT(const zfstring &, paramName, zfnull)
        , ZFMP_IN_OPT(const ZFListener &, paramDefaultValue, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMP, zfindex, paramCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, const zfstring &, paramTypeIdAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, const zfstring &, paramNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, const ZFListener &, paramDefaultValueCallbackAt
        , ZFMP_IN(zfindex, index)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPropertyCallbackDynamicRegisterInitValueGetter, ZFPropertyCallbackDynamicRegisterInitValueGetter)
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, dynamicRegisterUserData
        , ZFMP_IN(ZFObject *, dynamicRegisterUserData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, zfany, dynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, ownerClass
        , ZFMP_IN(const ZFClass *, ownerClass)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const ZFClass *, ownerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyTypeId
        , ZFMP_IN(const zfstring &, propertyTypeId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfstring &, propertyTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyTypeName
        , ZFMP_IN(const zfstring &, propertyTypeName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfstring &, propertyTypeName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyName
        , ZFMP_IN(const zfstring &, propertyName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfstring &, propertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyClassOfRetainProperty
        , ZFMP_IN(const ZFClass *, propertyClassOfRetainProperty)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const ZFClass *, propertyClassOfRetainProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyInitValueCallback
        , ZFMP_IN(ZFPropertyCallbackDynamicRegisterInitValueGetter, propertyInitValueCallback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFPropertyCallbackDynamicRegisterInitValueGetter, propertyInitValueCallback)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertySetterType
        , ZFMP_IN(ZFMethodPrivilegeType, propertySetterType)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFMethodPrivilegeType, propertySetterType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyGetterType
        , ZFMP_IN(ZFMethodPrivilegeType, propertyGetterType)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFMethodPrivilegeType, propertyGetterType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, zfunsafe_disableChecker
        , ZFMP_IN(zfbool, disableChecker)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, zfbool, zfunsafe_disableChecker)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFComparer_ZFObject, ZFComparer<ZFObject *>::Comparer)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFObjectInstanceState, ZFObjectInstanceState, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFObjectInstanceStateOnInit,
            ZFTOKEN_ZFObjectInstanceStateOnInitFinish,
            ZFTOKEN_ZFObjectInstanceStateIdle,
            ZFTOKEN_ZFObjectInstanceStateOnDeallocPrepare,
            ZFTOKEN_ZFObjectInstanceStateOnDealloc,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFObjectInstanceStateOnInit;
        switch(matched) {
            case 0:
                v = ZFObjectInstanceStateOnInit;
                return zftrue;
            case 1:
                v = ZFObjectInstanceStateOnInitFinish;
                return zftrue;
            case 2:
                v = ZFObjectInstanceStateIdle;
                return zftrue;
            case 3:
                v = ZFObjectInstanceStateOnDeallocPrepare;
                return zftrue;
            case 4:
                v = ZFObjectInstanceStateOnDealloc;
                return zftrue;
            case 5:
                v = ZFObjectInstanceStateIdle;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFObjectInstanceStateOnInit:
                s += ZFTOKEN_ZFObjectInstanceStateOnInit;
                return zftrue;
            case ZFObjectInstanceStateOnInitFinish:
                s += ZFTOKEN_ZFObjectInstanceStateOnInitFinish;
                return zftrue;
            case ZFObjectInstanceStateIdle:
                s += ZFTOKEN_ZFObjectInstanceStateIdle;
                return zftrue;
            case ZFObjectInstanceStateOnDeallocPrepare:
                s += ZFTOKEN_ZFObjectInstanceStateOnDeallocPrepare;
                return zftrue;
            case ZFObjectInstanceStateOnDealloc:
                s += ZFTOKEN_ZFObjectInstanceStateOnDealloc;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_ENUM_DEFINE(ZFObjectInstanceState
        , ZFObjectInstanceStateOnInit
        , ZFObjectInstanceStateOnInitFinish
        , ZFObjectInstanceStateIdle
        , ZFObjectInstanceStateOnDeallocPrepare
        , ZFObjectInstanceStateOnDealloc
        )

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFCallbackType, ZFCallbackType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFCallbackTypeDummy,
            ZFTOKEN_ZFCallbackTypeMethod,
            ZFTOKEN_ZFCallbackTypeMemberMethod,
            ZFTOKEN_ZFCallbackTypeRawFunction,
            ZFTOKEN_ZFCallbackTypeLambda,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFCallbackTypeDummy;
        switch(matched) {
            case 0:
                v = ZFCallbackTypeDummy;
                return zftrue;
            case 1:
                v = ZFCallbackTypeMethod;
                return zftrue;
            case 2:
                v = ZFCallbackTypeMemberMethod;
                return zftrue;
            case 3:
                v = ZFCallbackTypeRawFunction;
                return zftrue;
            case 4:
                v = ZFCallbackTypeDummy;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFCallbackTypeDummy:
                s += ZFTOKEN_ZFCallbackTypeDummy;
                return zftrue;
            case ZFCallbackTypeMethod:
                s += ZFTOKEN_ZFCallbackTypeMethod;
                return zftrue;
            case ZFCallbackTypeMemberMethod:
                s += ZFTOKEN_ZFCallbackTypeMemberMethod;
                return zftrue;
            case ZFCallbackTypeRawFunction:
                s += ZFTOKEN_ZFCallbackTypeRawFunction;
                return zftrue;
            case ZFCallbackTypeLambda:
                s += ZFTOKEN_ZFCallbackTypeLambda;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFSerializablePropertyType, ZFSerializablePropertyType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFSerializablePropertyTypeUnspecified,
            ZFTOKEN_ZFSerializablePropertyTypeNotSerializable,
            ZFTOKEN_ZFSerializablePropertyTypeSerializable,
            ZFTOKEN_ZFSerializablePropertyTypeEmbeded,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFSerializablePropertyTypeUnspecified;
        switch(matched) {
            case 0:
                v = ZFSerializablePropertyTypeUnspecified;
                return zftrue;
            case 1:
                v = ZFSerializablePropertyTypeNotSerializable;
                return zftrue;
            case 2:
                v = ZFSerializablePropertyTypeSerializable;
                return zftrue;
            case 3:
                v = ZFSerializablePropertyTypeEmbeded;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFSerializablePropertyTypeUnspecified:
                s += ZFTOKEN_ZFSerializablePropertyTypeUnspecified;
                return zftrue;
            case ZFSerializablePropertyTypeNotSerializable:
                s += ZFTOKEN_ZFSerializablePropertyTypeNotSerializable;
                return zftrue;
            case ZFSerializablePropertyTypeSerializable:
                s += ZFTOKEN_ZFSerializablePropertyTypeSerializable;
                return zftrue;
            case ZFSerializablePropertyTypeEmbeded:
                s += ZFTOKEN_ZFSerializablePropertyTypeEmbeded;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE(ZFSerializableData, ZFSerializableData, {
        v = serializableData;
        return zftrue;
    }, {
        serializableData = v;
        return zftrue;
    }, {
        return ZFSerializableDataFromZfsd(v, src, srcLen);
    }, {
        return ZFSerializableDataToZfsd(s, v, zfnull, zffalse);
    })

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, copyFrom
        , ZFMP_IN(const ZFSerializableData &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, ZFSerializableData, copy)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, ZFPathInfo, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, pathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, ZFPathInfo, pathInfoCheck)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, serializableDataParent
        , ZFMP_OUT(ZFSerializableData &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, itemClass
        , ZFMP_IN(const zfstring &, classNameFull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfstring &, itemClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, propertyName
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfstring, propertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, propertyValue
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfstring, propertyValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, category
        , ZFMP_IN(const zfstring &, category)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfstring, category)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, serializableDataTag
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFObject *, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfany, serializableDataTag
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, serializableDataTagGetAllKeyValue
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, allKey)
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, allValue)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, serializableDataTagRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfauto, serializableDataTagRemoveAndGet
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, serializableDataTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, attr
        , ZFMP_IN(const zfstring &, name)
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfstring, attr
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfindex, attrCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrRemove
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, attrRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfiter, attrIterFind
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfiter, attrIter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfstring, attrIterKey
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfstring, attrIterValue
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, attrIterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrIterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, attrIterResolved
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrIterResolveMark
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrIterResolveUnmark
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, child
        , ZFMP_IN(const ZFSerializableData &, element)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, child
        , ZFMP_IN(const ZFSerializableData &, element)
        , ZFMP_IN(zfindex, atIndex)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, childReplace
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(const ZFSerializableData &, element)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfindex, childForName
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfindex, childForCategory
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfindex, childCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const ZFSerializableData &, childAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, childRemoveAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, childRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolved)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, resolvedAttr
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, resolveAttrMark
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, resolveAttrUnmark
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedPropertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyNameMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyNameUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedPropertyValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyValueMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyValueUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedCategory)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveCategoryMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveCategoryUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveMarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveUnmarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveAttrMarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveAttrUnmarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, valid)

ZFOUTPUT_TYPE_DEFINE(ZFSerializableData, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

