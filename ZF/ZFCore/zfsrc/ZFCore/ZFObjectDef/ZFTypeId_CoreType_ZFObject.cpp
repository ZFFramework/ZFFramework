#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFClassDataChangeType, ZFClassDataChangeType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFClassDataChangeTypeAttach,
            ZFTOKEN_ZFClassDataChangeTypeDetach,
            ZFTOKEN_ZFClassDataChangeTypeClassAliasAttach,
            ZFTOKEN_ZFClassDataChangeTypeClassAliasDetach,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFClassDataChangeTypeAttach;
        switch(matched) {
            case 0:
                v = ZFClassDataChangeTypeAttach;
                return zftrue;
            case 1:
                v = ZFClassDataChangeTypeDetach;
                return zftrue;
            case 2:
                v = ZFClassDataChangeTypeClassAliasAttach;
                return zftrue;
            case 3:
                v = ZFClassDataChangeTypeClassAliasDetach;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFClassDataChangeTypeAttach:
                s += ZFTOKEN_ZFClassDataChangeTypeAttach;
                return zftrue;
            case ZFClassDataChangeTypeDetach:
                s += ZFTOKEN_ZFClassDataChangeTypeDetach;
                return zftrue;
            case ZFClassDataChangeTypeClassAliasAttach:
                s += ZFTOKEN_ZFClassDataChangeTypeClassAliasAttach;
                return zftrue;
            case ZFClassDataChangeTypeClassAliasDetach:
                s += ZFTOKEN_ZFClassDataChangeTypeClassAliasDetach;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_ENUM_DEFINE(ZFClassDataChangeType
        , ZFClassDataChangeTypeAttach
        , ZFClassDataChangeTypeDetach
        , ZFClassDataChangeTypeClassAliasAttach
        , ZFClassDataChangeTypeClassAliasDetach
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFClassDataChangeData, ZFClassDataChangeData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataChangeData, ZFClassDataChangeType, changeType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataChangeData, const ZFClass *, changedClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataChangeData, const ZFProperty *, changedProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataChangeData, const ZFMethod *, changedMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFClassDataChangeData, const zfchar *, name)

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
                zfCoreCriticalShouldNotGoHere();
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
                zfCoreCriticalShouldNotGoHere();
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
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_ENUM_DEFINE(ZFMethodType
        , ZFMethodTypeNormal
        , ZFMethodTypeStatic
        , ZFMethodTypeVirtual
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMethodParamDefaultValueCallback, ZFMethodParamDefaultValueCallback)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFArgs, ZFArgs)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFArgs, zfidentity, eventId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, sender
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, ZFAny, sender)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, param0
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, ZFAny, param0)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, param1
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, ZFAny, param1)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, userData
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, ZFAny, userData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, result
        , ZFMP_IN(ZFObject *, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, ZFAny, result)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, ZFArgs &, resultEnable
        , ZFMP_IN(zfbool, enable)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, zfbool, resultEnabled)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, void, eventFiltered
        , ZFMP_IN(zfbool, eventFiltered)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, zfbool, eventFiltered)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFArgs, ZFArgs &, eventFilterEnable
        , ZFMP_IN(zfbool, enable)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFArgs, zfbool, eventFilterEnabled)

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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserver, ZFAny, observerOwner)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFClass, ZFFilterForZFClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, void, copyFrom
        , ZFMP_IN(ZFFilterForZFClass const &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFClass, void, filterAdd
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, zfbool, filterCheckActive
        , ZFMP_IN(const ZFClass * const &, e)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFMethod, ZFFilterForZFMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, void, copyFrom
        , ZFMP_IN(ZFFilterForZFMethod const &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFMethod, void, filterAdd
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, zfbool, filterCheckActive
        , ZFMP_IN(const ZFMethod * const &, e)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFProperty, ZFFilterForZFProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, void, copyFrom
        , ZFMP_IN(ZFFilterForZFProperty const &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFProperty, void, filterAdd
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, zfbool, filterCheckActive
        , ZFMP_IN(const ZFProperty * const &, e)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFObject, ZFFilterForZFObject)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, void, copyFrom
        , ZFMP_IN(ZFFilterForZFObject const &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFObject, void, filterAdd
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, ZFAny, filterElementAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, ZFFilterType, filterTypeAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, zfbool, filterCheckActive
        , ZFMP_IN(ZFObject * const &, e)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMethodGenericInvoker, ZFMethodGenericInvoker)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodDynamicRegisterUserData
        , ZFMP_IN(ZFObject *, methodDynamicRegisterUserData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFAny, methodDynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodOwnerClass
        , ZFMP_IN(const ZFClass *, methodOwnerClass)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const ZFClass *, methodOwnerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodNamespace
        , ZFMP_IN(const zfchar *, methodNamespace)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodNamespace)
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
        , ZFMP_IN(const zfchar *, methodName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodReturnTypeId
        , ZFMP_IN(const zfchar *, methodReturnTypeId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodReturnTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodReturnTypeName
        , ZFMP_IN(const zfchar *, methodReturnTypeName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodReturnTypeName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFMethodDynamicRegisterParam, void, methodParamAdd
        , ZFMP_IN(const zfchar *, methodParamTypeId)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeName, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamName, zfnull)
        , ZFMP_IN_OPT(ZFMethodParamDefaultValueCallback, methodParamDefaultValueCallback, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFMethodDynamicRegisterParam, void, methodParamAddWithDefault
        , ZFMP_IN(const zfchar *, methodParamTypeId)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeName, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamName, zfnull)
        , ZFMP_IN_OPT(ZFObject *, methodParamDefaultValue, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, zfindex, methodParamCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfchar *, methodParamTypeIdAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfchar *, methodParamTypeNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfchar *, methodParamNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodParamDefaultValueCallback, methodParamDefaultValueCallbackAt
        , ZFMP_IN(zfindex, index)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMP, ZFMP)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFMP, ZFMP &, mp
        , ZFMP_IN(const zfchar *, methodParamTypeId)
        , ZFMP_IN_OPT(const zfchar *, methodParamName, zfnull)
        , ZFMP_IN_OPT(ZFObject *, methodParamDefaultValue, ZFMethodGenericInvokerDefaultParam())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMP, zfindex, methodParamCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, const zfchar *, methodParamTypeIdAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, const zfchar *, methodParamNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, ZFAny, methodParamDefaultValueAt
        , ZFMP_IN(zfindex, index)
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPropertyCallbackDynamicRegisterInitValueGetter, ZFPropertyCallbackDynamicRegisterInitValueGetter)
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyDynamicRegisterUserData
        , ZFMP_IN(ZFObject *, propertyDynamicRegisterUserData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFAny, propertyDynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyOwnerClass
        , ZFMP_IN(const ZFClass *, propertyOwnerClass)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const ZFClass *, propertyOwnerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyTypeId
        , ZFMP_IN(const zfchar *, propertyTypeId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyTypeName
        , ZFMP_IN(const zfchar *, propertyTypeName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyTypeName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyName
        , ZFMP_IN(const zfchar *, propertyName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyName)
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
                zfCoreCriticalShouldNotGoHere();
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
                zfCoreCriticalShouldNotGoHere();
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const ZFPathInfo *, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, pathInfo
        , ZFMP_IN(const ZFPathInfo *, pathInfo)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, pathInfo
        , ZFMP_IN(const zfchar *, pathType)
        , ZFMP_IN(const zfchar *, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const ZFPathInfo *, pathInfoCheck)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, serializableDataParent
        , ZFMP_OUT(ZFSerializableData &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, itemClass
        , ZFMP_IN(const zfchar *, classNameFull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, itemClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, propertyName
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, propertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, propertyValue
        , ZFMP_IN(const zfchar *, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, propertyValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, category
        , ZFMP_IN(const zfchar *, category)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, category)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, serializableDataTag
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN(ZFObject *, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, ZFAny, serializableDataTag
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, serializableDataTagGetAllKeyValue
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, allKey)
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, allValue)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, serializableDataTagRemove
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfauto, serializableDataTagRemoveAndGet
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, serializableDataTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, attr
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(const zfchar *, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attr
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfindex, attrCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrRemove
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, attrRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfiterator, attrIterFind
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfiterator, attrIter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, attrIterValid
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrIterNext
        , ZFMP_IN_OUT(zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attrIterKey
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attrIterValue
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, attrIterValue
        , ZFMP_IN_OUT(zfiterator &, it)
        , ZFMP_IN(const zfchar *, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrIterRemove
        , ZFMP_IN_OUT(zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, attrIterResolved
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrIterResolveMark
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attrIterResolveUnmark
        , ZFMP_IN(const zfiterator &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, childAdd
        , ZFMP_IN(const ZFSerializableData &, element)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, childAdd
        , ZFMP_IN(const ZFSerializableData &, element)
        , ZFMP_IN(zfindex, atIndex)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, childReplace
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(const ZFSerializableData &, element)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfindex, childForName
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfindex, childForCategory
        , ZFMP_IN(const zfchar *, name)
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, resolvedAttribute
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, resolveAttributeMark
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, resolveAttributeUnmark
        , ZFMP_IN(const zfchar *, name)
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveAttributeMarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveAttributeUnmarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, isEmpty)

ZFOUTPUT_TYPE_DEFINE(ZFSerializableData, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

