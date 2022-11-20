#include "ZFDynamicInvoker.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_string.h" // for ZFDI_invoke method cache
#include "../ZFSTLWrapper/zfstl_hashmap.h" // for ZFDI_invoke method cache

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFDI_WrapperBase)
ZFOBJECT_REGISTER(ZFDI_Wrapper)
ZFOBJECT_REGISTER(ZFDI_WrapperRaw)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFDI_WrapperBase, void, zfv, ZFMP_IN(const zfchar *, zfv))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFDI_WrapperBase, const zfchar *, zfv)

// ============================================================
static zfbool _ZFP_ZFDI_cacheEnable = zffalse;

typedef zfstlhashmap<zfstlstringZ, const ZFClass *> _ZFP_ZFDI_ClassMapCache;
static _ZFP_ZFDI_ClassMapCache _ZFP_ZFDI_classMapCache;

typedef zfstlhashmap<zfstlstringZ, ZFCoreArrayPOD<const ZFMethod *> > _ZFP_ZFDI_MethodMapCache;
static _ZFP_ZFDI_MethodMapCache _ZFP_ZFDI_methodMapCache;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDI_MethodCache, ZFLevelZFFrameworkNormal)
{
    zfCoreMutexLocker();
    this->classDataChangeListener = ZFCallbackForFunc(zfself::classDataChange);
    ZFClassDataChangeObserver.observerAdd(ZFGlobalEvent::EventClassDataChange(), this->classDataChangeListener);

    _ZFP_ZFDI_methodMapCache.clear();
    _ZFP_ZFDI_cacheEnable = zftrue;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDI_MethodCache)
{
    zfCoreMutexLocker();
    _ZFP_ZFDI_cacheEnable = zffalse;
    _ZFP_ZFDI_methodMapCache.clear();
    ZFClassDataChangeObserver.observerRemove(ZFGlobalEvent::EventClassDataChange(), this->classDataChangeListener);
}
private:
    ZFListener classDataChangeListener;
    static void classDataChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        zfCoreMutexLocker();
        const ZFClassDataChangeData &changed = ZFCastZFObjectUnchecked(v_ZFClassDataChangeData *, listenerData.param0())->zfv;
        if(changed.changedClass != zfnull)
        {
            _ZFP_ZFDI_classMapCache.clear();
        }
        _ZFP_ZFDI_methodMapCache.clear();
    }
ZF_GLOBAL_INITIALIZER_END(ZFDI_MethodCache)

// ============================================================
const zfchar *ZFDI_toString(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        return zfnull;
    }
    {
        v_zfstring *t = ZFCastZFObject(v_zfstring *, obj);
        if(t != zfnull)
        {
            return t->zfv;
        }
    }
    {
        ZFDI_WrapperBase *t = ZFCastZFObject(ZFDI_WrapperBase *, obj);
        if(t != zfnull)
        {
            return t->zfv();
        }
    }
    return zfnull;
}

const ZFClass *ZFDI_classForName(ZF_IN const zfchar *className,
                                 ZF_IN_OPT const zfchar *NS /* = zfnull */)
{
    if(className == zfnull)
    {
        return zfnull;
    }
    zfstlstringZ key;
    key += className;
    if(NS != zfnull)
    {
        key += ":";
        key += NS;
    }
    _ZFP_ZFDI_ClassMapCache::iterator it = _ZFP_ZFDI_classMapCache.find(key);
    if(it != _ZFP_ZFDI_classMapCache.end())
    {
        return it->second;
    }

    const ZFClass *cls = ZFClass::classForName(className, NS);
    if(cls == zfnull)
    {
        zfindex dotPos = zfstringFindReversely(className, zfindexMax(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen());
        if(dotPos == zfindexMax())
        {
            zfstring classNameTmp;
            classNameTmp += ZFTypeIdWrapperPrefixName;
            classNameTmp += className;
            cls = ZFClass::classForName(classNameTmp, NS);
        }
        else
        {
            zfstring classNameTmp;
            classNameTmp.append(className, dotPos + ZFNamespaceSeparatorLen());
            classNameTmp += ZFTypeIdWrapperPrefixName;
            classNameTmp += className + dotPos + ZFNamespaceSeparatorLen();
            cls = ZFClass::classForName(classNameTmp, NS);
        }
    }

    _ZFP_ZFDI_classMapCache[key] = cls;
    return cls;
}

static void _ZFP_ZFDI_paramInfo(ZF_IN_OUT zfstring &ret,
                                ZF_IN ZFObject *param)
{
    if(param == zfnull)
    {
        ret += ZFTOKEN_zfnull;
    }
    else
    {
        ret += "(";
        ret += param->classData()->className();
        ret += ")";
        param->objectInfoT(ret);
    }
}
void ZFDI_paramInfo(ZF_IN_OUT zfstring &ret
                    , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                    )
{
    if(param0 == zfnull || param0 == ZFMethodGenericInvokerDefaultParam()) {return ;}
    ret += '[';
    do
    {
        _ZFP_ZFDI_paramInfo(ret, param0);

        #define _ZFP_ZFDI_paramInfo_loop(N) \
            if(param##N == zfnull || param##N == ZFMethodGenericInvokerDefaultParam()) \
            { \
                break; \
            } \
            else \
            { \
                ret += ", "; \
                _ZFP_ZFDI_paramInfo(ret, param##N); \
            }
        _ZFP_ZFDI_paramInfo_loop(1)
        _ZFP_ZFDI_paramInfo_loop(2)
        _ZFP_ZFDI_paramInfo_loop(3)
        _ZFP_ZFDI_paramInfo_loop(4)
        _ZFP_ZFDI_paramInfo_loop(5)
        _ZFP_ZFDI_paramInfo_loop(6)
        _ZFP_ZFDI_paramInfo_loop(7)
    } while(zffalse);
    ret += ']';
}

// ============================================================
static inline void _ZFP_ZFDI_paramCount(ZF_IN_OUT zfindex &paramCount,
                                        ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM])
{
    paramCount = 0;
    while(paramCount < ZFMETHOD_MAX_PARAM && paramList[paramCount] != ZFMethodGenericInvokerDefaultParam())
    {
        ++paramCount;
    }
}
static zfbool _ZFP_ZFDI_invoke(ZF_OUT zfautoObject &ret
                               , ZF_OUT_OPT zfstring *errorHint
                               , ZF_IN_OPT ZFObject *obj
                               , ZF_IN const zfchar *name
                               , ZF_IN const ZFCoreArray<const ZFMethod *> &methodList
                               , ZF_IN_OPT zfindex paramCount
                               , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                               )
{
    if(methodList.isEmpty())
    {
        if(errorHint != zfnull)
        {
            *errorHint += "no matching method to call ";
            *errorHint += name;
            *errorHint += "(";
            ZFDI_paramInfo(*errorHint
                    , paramList[0]
                    , paramList[1]
                    , paramList[2]
                    , paramList[3]
                    , paramList[4]
                    , paramList[5]
                    , paramList[6]
                    , paramList[7]
                );
            *errorHint += ")";
        }
        return zffalse;
    }
    else
    {
        if(_ZFP_ZFDI_cacheEnable)
        {
            zfCoreMutexLocker();
            zfstlstringZ key;
            if(obj != zfnull)
            {
                key += obj->classData()->classNameFull();
            }
            key += ':';
            key += name;
            _ZFP_ZFDI_methodMapCache[key].addFrom(methodList);
        }
        return ZFDI_invoke(ret, errorHint, obj, methodList, paramCount, paramList);
    }
}
zfbool ZFDI_invoke(ZF_OUT zfautoObject &ret
                   , ZF_OUT_OPT zfstring *errorHint
                   , ZF_IN_OPT ZFObject *obj
                   , ZF_IN const zfchar *name
                   , ZF_IN_OPT zfindex paramCount
                   , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                   )
{
    if(_ZFP_ZFDI_cacheEnable)
    {
        zfCoreMutexLock();
        zfstlstringZ key;
        if(obj != zfnull)
        {
            key += obj->classData()->classNameFull();
        }
        key += ':';
        key += name;
        _ZFP_ZFDI_MethodMapCache::iterator it = _ZFP_ZFDI_methodMapCache.find(key);
        zfCoreMutexUnlock();
        if(it != _ZFP_ZFDI_methodMapCache.end())
        {
            return ZFDI_invoke(ret, errorHint, obj, it->second, paramCount, paramList);
        }
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;

    // obj->methodName()
    if(obj != zfnull)
    {
        obj->classData()->methodForNameGetAllT(methodList, name);
        return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList);
    }

    zfindex dotPos = zfstringFindReversely(name, zfslen(name), ZFNamespaceSeparator());
    if(dotPos == zfindexMax())
    {
        // methodName()
        ZFMethodFuncForNameGetAllT(methodList, zfnull, name);
        if(!methodList.isEmpty())
        {
            return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList);
        }

        // ClassName()
        // v_ClassName()
        const ZFClass *cls = ZFDI_classForName(name, zfnull);
        if(cls != zfnull)
        {
            return ZFDI_alloc(ret, errorHint, cls, paramCount, paramList);
        }

        // fail
        return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList);
    }
    else
    {
        zfstring scopeTmp(name, dotPos);
        const zfchar *nameTmp = name + dotPos + 1;

        // NS.ClassName.methodName()
        // NS.v_ClassName.methodName()
        {
            const ZFClass *cls = ZFDI_classForName(scopeTmp, zfnull);
            if(cls != zfnull)
            {
                cls->methodForNameGetAllT(methodList, nameTmp);
                return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList);
            }
        }

        // NS.ClassName()
        // NS.v_ClassName()
        {
            const ZFClass *cls = ZFDI_classForName(name, zfnull);
            if(cls != zfnull)
            {
                return ZFDI_alloc(ret, errorHint, cls, paramCount, paramList);
            }
        }

        // NS.methodName()
        ZFMethodFuncForNameGetAllT(methodList, scopeTmp, nameTmp);
        return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList);
    }
}
zfbool ZFDI_invoke(ZF_OUT zfautoObject &ret
                   , ZF_OUT_OPT zfstring *errorHint
                   , ZF_IN_OPT ZFObject *obj
                   , ZF_IN const ZFCoreArray<const ZFMethod *> &methodList
                   , ZF_IN_OPT zfindex paramCount
                   , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                   )
{
    if(methodList.isEmpty())
    {
        if(errorHint != zfnull)
        {
            *errorHint += "no matching method to call";
        }
        return zffalse;
    }

    zfstring _errorHintTmp;
    zfstring *errorHintTmp = errorHint ? &_errorHintTmp : zfnull;
    if(methodList.count() == 1)
    {
        do {
            const ZFMethod *method = methodList[0];
            if(!method->methodIsPublic())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "can not invoke %s method: %s",
                        ZFMethodPrivilegeTypeToString(method->methodPrivilegeType()).cString(),
                        method->objectInfo().cString());
                }
                break;
            }
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %zi",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount()).cString()
                            : zfstringWithFormat("%zi~%zi", method->methodParamCountMin(), method->methodParamCount()).cString()),
                        paramCount);
                }
                break;
            }
            zfbool paramConvertSuccess = zftrue;
            zfautoObject paramConvertCache;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam)
            {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramList[iParam]);
                if(wrapper != zfnull)
                {
                    paramConvertCache.zfunsafe_assign(paramList[iParam]);
                    if(!ZFDI_objectFromString(
                        paramList[iParam], method->methodParamTypeIdAt(iParam), wrapper->zfv(), errorHintTmp))
                    {
                        paramList[iParam].zfunsafe_assign(paramConvertCache);
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
            if(!paramConvertSuccess)
            {
                break;
            }
            if(method->methodGenericInvoker()(method, obj, errorHintTmp, ret, paramList))
            {
                if(zfscmpTheSame(method->methodReturnTypeId(), ZFTypeId_void()))
                {
                    ret = obj;
                }
                return zftrue;
            }
        } while(zffalse);
    }
    else
    {
        zfautoObject paramListTmp[ZFMETHOD_MAX_PARAM];
        for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod)
        {
            const ZFMethod *method = methodList[iMethod];
            if(errorHintTmp != zfnull && !_errorHintTmp.isEmpty())
            {
                _errorHintTmp += "\n    ";
            }
            if(!method->methodIsPublic())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "can not invoke %s method: %s",
                        ZFMethodPrivilegeTypeToString(method->methodPrivilegeType()).cString(),
                        method->objectInfo().cString());
                }
                break;
            }
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %zi",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount()).cString()
                            : zfstringWithFormat("%zi~%zi", method->methodParamCountMin(), method->methodParamCount()).cString()),
                        paramCount);
                }
                continue;
            }

            for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i)
            {
                paramListTmp[i].zfunsafe_assign(paramList[i]);
            }

            zfbool paramConvertSuccess = zftrue;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam)
            {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramListTmp[iParam]);
                if(wrapper != zfnull)
                {
                    if(!ZFDI_objectFromString(
                        paramListTmp[iParam], method->methodParamTypeIdAt(iParam), wrapper->zfv(), errorHintTmp))
                    {
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
            if(!paramConvertSuccess)
            {
                continue;
            }
            if(method->methodGenericInvoker()(method, obj, errorHintTmp, ret, paramListTmp))
            {
                if(zfscmpTheSame(method->methodReturnTypeId(), ZFTypeId_void()))
                {
                    ret = obj;
                }
                return zftrue;
            }
        }
    }
    if(errorHint != zfnull)
    {
        *errorHint += "no matching method to call";
        *errorHint += ", error hint:\n    ";
        *errorHint += _errorHintTmp;
        *errorHint += "\n  candidate methods:";
        for(zfindex i = 0; i < methodList.count(); ++i)
        {
            *errorHint += "\n    ";
            methodList[i]->objectInfoT(*errorHint);
        }

        if(paramCount > 0)
        {
            *errorHint += "\n  with params: ";
            ZFDI_paramInfo(*errorHint
                    , paramList[0]
                    , paramList[1]
                    , paramList[2]
                    , paramList[3]
                    , paramList[4]
                    , paramList[5]
                    , paramList[6]
                    , paramList[7]
                );
        }
    }
    return zffalse;
}

zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                  , ZF_OUT_OPT zfstring *errorHint
                  , ZF_IN const ZFClass *cls
                  , ZF_IN zfindex paramCount
                  , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                  )
{
    if(cls == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "null class");
        }
        return zffalse;
    }
    if(cls->classIsAbstract())
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "class is abstract: \"%s\"", cls->classNameFull());
        }
        return zffalse;
    }
    if(!cls->classCanAllocPublic())
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "class can only create by reflection: \"%s\"", cls->classNameFull());
        }
        return zffalse;
    }

    if(paramCount == zfindexMax())
    {
        _ZFP_ZFDI_paramCount(paramCount, paramList);
    }
    if(paramCount == 0)
    {
        ret = cls->newInstance();
        if(ret == zfnull)
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "unable to alloc class \"%s\"", cls->classNameFull());
            }
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }
    ZFCoreArrayPOD<const ZFMethod *> methodList;
    cls->methodForNameGetAllT(methodList, "objectOnInit");
    if(methodList.isEmpty())
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "class \"%s\" has no reflectable objectOnInit", cls->classNameFull());
        }
        return zffalse;
    }

    void *token = cls->newInstanceGenericBegin();
    if(token == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "unable to alloc class \"%s\"", cls->classNameFull());
        }
        return zffalse;
    }

    zfstring _errorHintTmp;
    zfstring *errorHintTmp = errorHint ? &_errorHintTmp : zfnull;
    if(methodList.count() == 1)
    {
        do {
            const ZFMethod *method = methodList[0];
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %zi",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount()).cString()
                            : zfstringWithFormat("%zi~%zi", method->methodParamCountMin(), method->methodParamCount()).cString()),
                        paramCount);
                }
                continue;
            }
            zfbool paramConvertSuccess = zftrue;
            zfautoObject paramConvertCache;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam)
            {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramList[iParam]);
                if(wrapper != zfnull)
                {
                    paramConvertCache.zfunsafe_assign(paramList[iParam]);
                    if(!ZFDI_objectFromString(
                        paramList[iParam], method->methodParamTypeIdAt(iParam), wrapper->zfv(), errorHintTmp))
                    {
                        paramList[iParam].zfunsafe_assign(paramConvertCache);
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
            if(!paramConvertSuccess)
            {
                break;
            }
            if(cls->newInstanceGenericCheck(token, method, paramList, errorHintTmp))
            {
                ret = cls->newInstanceGenericEnd(token, zftrue);
                return zftrue;
            }
        } while(zffalse);
    }
    else
    {
        zfautoObject paramListTmp[ZFMETHOD_MAX_PARAM];
        for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod)
        {
            const ZFMethod *method = methodList[iMethod];
            if(!_errorHintTmp.isEmpty())
            {
                _errorHintTmp += "\n    ";
            }
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %zi",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount()).cString()
                            : zfstringWithFormat("%zi~%zi", method->methodParamCountMin(), method->methodParamCount()).cString()),
                        paramCount);
                }
                continue;
            }

            for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i)
            {
                paramListTmp[i].zfunsafe_assign(paramList[i]);
            }

            zfbool paramConvertSuccess = zftrue;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam)
            {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramListTmp[iParam]);
                if(wrapper != zfnull)
                {
                    if(!ZFDI_objectFromString(
                        paramListTmp[iParam], method->methodParamTypeIdAt(iParam), wrapper->zfv(), errorHintTmp))
                    {
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
            if(!paramConvertSuccess)
            {
                continue;
            }
            if(cls->newInstanceGenericCheck(token, method, paramListTmp, errorHintTmp))
            {
                ret = cls->newInstanceGenericEnd(token, zftrue);
                return zftrue;
            }
        }
    }
    cls->newInstanceGenericEnd(token, zffalse);
    if(errorHint != zfnull)
    {
        *errorHint += "no matching objectOnInit to call for class \"";
        *errorHint += cls->className();
        *errorHint += "\"";
        *errorHint += ", error hint:\n    ";
        *errorHint += _errorHintTmp;
        *errorHint += "\n  candidate methods:";
        for(zfindex i = 0; i < methodList.count(); ++i)
        {
            *errorHint += "\n    ";
            methodList[i]->objectInfoT(*errorHint);
        }

        if(paramCount > 0)
        {
            *errorHint += "\n  with params: ";
            ZFDI_paramInfo(*errorHint
                    , paramList[0]
                    , paramList[1]
                    , paramList[2]
                    , paramList[3]
                    , paramList[4]
                    , paramList[5]
                    , paramList[6]
                    , paramList[7]
                );
        }
    }
    return zffalse;
}

zfbool ZFDI_objectFromString(ZF_OUT zfautoObject &ret,
                             ZF_IN const ZFClass *cls,
                             ZF_IN const zfchar *s,
                             ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(cls == zfnull)
    {
        zfstringAppend(errorHint, "null class");
        return zffalse;
    }
    else if(cls->classIsAbstract())
    {
        zfstringAppend(errorHint, "class \"%s\" is abstract", cls->classNameFull());
        return zffalse;
    }
    else if(cls->classIsTypeOf(ZFTypeIdWrapper::ClassData()))
    {
        ret = cls->newInstance();
        ZFTypeIdWrapper *wrapper = ret;
        if(wrapper != zfnull && wrapper->wrappedValueFromString(s))
        {
            return zftrue;
        }
        else
        {
            zfstringAppend(errorHint, "%s can not be converted from string \"%s\"", cls->classNameFull(), s);
            return zffalse;
        }
    }
    else
    {
        if(ZFSerializeFromString(ret, cls, s))
        {
            return zftrue;
        }
        else
        {
            zfstringAppend(errorHint, "%s can not be converted from string \"%s\"", cls->classNameFull(), s);
            return zffalse;
        }
    }
}
zfbool ZFDI_objectFromString(ZF_OUT zfautoObject &ret,
                             ZF_IN const zfchar *typeId,
                             ZF_IN const zfchar *s,
                             ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    const ZFClass *cls = ZFDI_classForName(typeId);
    if(cls == zfnull)
    {
        zfstringAppend(errorHint, "no such type \"%s\"",
            typeId);
        return zffalse;
    }
    else
    {
        return ZFDI_objectFromString(ret, cls, s, errorHint);
    }
}

// ============================================================
zfautoObject ZFInvoke(ZF_IN const zfchar *name
                      , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_OUT_OPT zfbool *success /* = zfnull */
                      , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
                      )
{
    zfCoreMutexLock();
    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = ZFMETHOD_MAX_PARAM;
    do {
        if(param0 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 0; break;} else {paramList[0].zfunsafe_assign(param0);}
        if(param1 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 1; break;} else {paramList[1].zfunsafe_assign(param1);}
        if(param2 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 2; break;} else {paramList[2].zfunsafe_assign(param2);}
        if(param3 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 3; break;} else {paramList[3].zfunsafe_assign(param3);}
        if(param4 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 4; break;} else {paramList[4].zfunsafe_assign(param4);}
        if(param5 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 5; break;} else {paramList[5].zfunsafe_assign(param5);}
        if(param6 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 6; break;} else {paramList[6].zfunsafe_assign(param6);}
        if(param7 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 7; break;} else {paramList[7].zfunsafe_assign(param7);}
    } while(zffalse);
    zfCoreMutexUnlock();
    zfautoObject ret;
    if(ZFDI_invoke(ret, errorHint, zfnull, name, paramCount, paramList))
    {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else
    {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}
zfautoObject ZFInvoke(ZF_IN const zfchar *name
                      , ZF_IN const zfchar *param0
                      , ZF_IN_OPT const zfchar *param1 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param2 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param3 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param4 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param5 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param6 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param7 /* = zfnull */
                      , ZF_OUT_OPT zfbool *success /* = zfnull */
                      , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
                      )
{
    zfCoreMutexLock();
    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = ZFMETHOD_MAX_PARAM;
    do {
        if(param0 == zfnull) {paramCount = 0; break;} else {paramList[0] = zfunsafe_zflineAlloc(ZFDI_Wrapper, param0);}
        if(param1 == zfnull) {paramCount = 1; break;} else {paramList[1] = zfunsafe_zflineAlloc(ZFDI_Wrapper, param1);}
        if(param2 == zfnull) {paramCount = 2; break;} else {paramList[2] = zfunsafe_zflineAlloc(ZFDI_Wrapper, param2);}
        if(param3 == zfnull) {paramCount = 3; break;} else {paramList[3] = zfunsafe_zflineAlloc(ZFDI_Wrapper, param3);}
        if(param4 == zfnull) {paramCount = 4; break;} else {paramList[4] = zfunsafe_zflineAlloc(ZFDI_Wrapper, param4);}
        if(param5 == zfnull) {paramCount = 5; break;} else {paramList[5] = zfunsafe_zflineAlloc(ZFDI_Wrapper, param5);}
        if(param6 == zfnull) {paramCount = 6; break;} else {paramList[6] = zfunsafe_zflineAlloc(ZFDI_Wrapper, param6);}
        if(param7 == zfnull) {paramCount = 7; break;} else {paramList[7] = zfunsafe_zflineAlloc(ZFDI_Wrapper, param7);}
    } while(zffalse);
    zfCoreMutexUnlock();
    zfautoObject ret;
    if(ZFDI_invoke(ret, errorHint, zfnull, name, paramCount, paramList))
    {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else
    {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

