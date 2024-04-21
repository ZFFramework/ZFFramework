#include "ZFDynamicInvoker.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlhashmap.h" // for ZFDI_invoke method cache

#include "../ZFSTLWrapper/zfstlmap.h" // for ZFDI_invoke param convert backup

ZF_NAMESPACE_GLOBAL_BEGIN

typedef zfstlmap<zfindex, zfauto> _ZFP_ZFDI_ParamBackupMapType;

// ============================================================
ZFOBJECT_REGISTER(ZFDI_WrapperBase)
ZFOBJECT_REGISTER(ZFDI_Wrapper)
ZFOBJECT_REGISTER(ZFDI_WrapperRaw)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFDI_WrapperBase, void, zfv
        , ZFMP_IN(const zfchar *, zfv)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFDI_WrapperBase, const zfchar *, zfv)

// ============================================================
static zfbool _ZFP_ZFDI_cacheEnable = zffalse;

typedef zfstlhashmap<zfstring, const ZFClass *, zfstring_zfstlHasher, zfstring_zfstlHashComparer> _ZFP_ZFDI_ClassMapCache;
static _ZFP_ZFDI_ClassMapCache _ZFP_ZFDI_classMapCache;

typedef zfstlhashmap<zfstring, ZFCoreArray<const ZFMethod *>, zfstring_zfstlHasher, zfstring_zfstlHashComparer> _ZFP_ZFDI_MethodMapCache;
static _ZFP_ZFDI_MethodMapCache _ZFP_ZFDI_methodMapCache;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDI_MethodCache, ZFLevelZFFrameworkNormal) {
    zfCoreMutexLocker();
    this->classDataChangeListener = ZFCallbackForFunc(zfself::classDataChange);
    ZFClassDataChangeObserver().observerAdd(ZFGlobalEvent::EventClassDataChange(), this->classDataChangeListener);

    _ZFP_ZFDI_methodMapCache.clear();
    _ZFP_ZFDI_cacheEnable = zftrue;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDI_MethodCache) {
    zfCoreMutexLocker();
    _ZFP_ZFDI_cacheEnable = zffalse;
    _ZFP_ZFDI_methodMapCache.clear();
    ZFClassDataChangeObserver().observerRemove(ZFGlobalEvent::EventClassDataChange(), this->classDataChangeListener);
}
private:
    ZFListener classDataChangeListener;
    static void classDataChange(ZF_IN const ZFArgs &zfargs) {
        zfCoreMutexLocker();
        const ZFClassDataChangeData &changed = ZFCastZFObjectUnchecked(v_ZFClassDataChangeData *, zfargs.param0())->zfv;
        if(changed.changedClass != zfnull) {
            _ZFP_ZFDI_classMapCache.clear();
        }
        _ZFP_ZFDI_methodMapCache.clear();
    }
ZF_GLOBAL_INITIALIZER_END(ZFDI_MethodCache)

// ============================================================
ZFCoreArray<ZFOutput> &ZFDI_errorCallbacks(void) {
    static ZFCoreArray<ZFOutput> d;
    return d;
}
zfclassLikePOD _ZFP_ZFDI_ErrorHolder {
public:
    _ZFP_ZFDI_ErrorHolder(ZF_IN_OUT zfstring *&errorHint) {
        if(errorHint == zfnull && !ZFDI_errorCallbacks().isEmpty()) {
            errorHint = zfnew(zfstring);
            _errorHint = errorHint;
            _deleteFlag = zftrue;
        }
        else {
            _errorHint = errorHint;
            _deleteFlag = zffalse;
        }
    }
    _ZFP_ZFDI_ErrorHolder(void) {
        if(_deleteFlag) {
            zfdelete(_errorHint);
        }
    }
public:
    inline zfbool errorOccurred(void) {
        if(_errorHint) {
            *_errorHint += "\n";
            for(zfindex i = 0; i < ZFDI_errorCallbacks().count(); ++i) {
                ZFDI_errorCallbacks()[i].execute(_errorHint->cString(), _errorHint->length() * sizeof(zfchar));
            }
            _errorHint->remove(_errorHint->length() - 1);
        }
        return zffalse;
    }
private:
    zfstring *_errorHint;
    zfbool _deleteFlag;
};
#define _ZFP_ZFDI_errorPrepare() \
    _ZFP_ZFDI_ErrorHolder _ZFP_ZFDI_errorH(errorHint)
#define _ZFP_ZFDI_errorOccurred() \
    _ZFP_ZFDI_errorH.errorOccurred()

// ============================================================
const zfchar *ZFDI_toString(ZF_IN ZFObject *obj) {
    if(obj == zfnull) {
        return zfnull;
    }
    {
        v_zfstring *t = ZFCastZFObject(v_zfstring *, obj);
        if(t != zfnull) {
            return t->zfv;
        }
    }
    {
        ZFDI_WrapperBase *t = ZFCastZFObject(ZFDI_WrapperBase *, obj);
        if(t != zfnull) {
            return t->zfv();
        }
    }
    return zfnull;
}

const ZFClass *ZFDI_classForName(
        ZF_IN const zfchar *className
        , ZF_IN_OPT const zfchar *NS /* = zfnull */
        ) {
    if(className == zfnull) {
        return zfnull;
    }
    zfstring key;
    key += className;
    if(NS != zfnull) {
        key += ":";
        key += NS;
    }
    _ZFP_ZFDI_ClassMapCache::iterator it = _ZFP_ZFDI_classMapCache.find(key);
    if(it != _ZFP_ZFDI_classMapCache.end()) {
        return it->second;
    }

    const ZFClass *cls = ZFClass::classForName(className, NS);
    if(cls == zfnull) {
        zfindex dotPos = zfstringFindReversely(className, zfindexMax(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen());
        if(dotPos == zfindexMax()) {
            zfstring classNameTmp;
            classNameTmp += ZFTypeIdWrapperPrefixName;
            classNameTmp += className;
            cls = ZFClass::classForName(classNameTmp, NS);
        }
        else {
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

static void _ZFP_ZFDI_paramInfo(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *param
        ) {
    if(param == zfnull) {
        ret += ZFTOKEN_zfnull;
    }
    else {
        ret += "(";
        ret += param->classData()->className();
        ret += ")";
        param->objectInfoT(ret);
    }
}
void ZFDI_paramInfo(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
        ) {
    if(param0 == ZFMethodGenericInvokerDefaultParam()) {
        return;
    }
    do {
        _ZFP_ZFDI_paramInfo(ret, param0);

        #define _ZFP_ZFDI_paramInfo_loop(N) \
            if(param##N == ZFMethodGenericInvokerDefaultParam()) { \
                break; \
            } \
            else { \
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
}

// ============================================================
static zfbool _ZFP_ZFDI_invoke(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN_OPT ZFObject *obj
        , ZF_IN const zfchar *name
        , ZF_IN const ZFCoreArray<const ZFMethod *> &methodList
        , ZF_IN_OPT zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        ) {
    if(methodList.isEmpty()) {
        if(errorHint != zfnull) {
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
    else {
        if(_ZFP_ZFDI_cacheEnable) {
            zfCoreMutexLocker();
            zfstring key;
            if(obj != zfnull) {
                key += obj->classData()->classNameFull();
                key += ':';
            }
            key += name;
            _ZFP_ZFDI_methodMapCache[key].addFrom(methodList);
        }
        return ZFDI_invoke(ret, errorHint, obj, methodList, paramCount, paramList);
    }
}
zfbool ZFDI_invoke(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN_OPT ZFObject *obj
        , ZF_IN const zfchar *name
        , ZF_IN_OPT zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        ) {
    _ZFP_ZFDI_errorPrepare();
    if(_ZFP_ZFDI_cacheEnable) {
        zfCoreMutexLock();
        zfstring key;
        if(obj != zfnull) {
            key += obj->classData()->classNameFull();
            key += ':';
        }
        key += name;
        _ZFP_ZFDI_MethodMapCache::iterator it = _ZFP_ZFDI_methodMapCache.find(key);
        if(it != _ZFP_ZFDI_methodMapCache.end()) {
            ZFCoreArray<const ZFMethod *> methodList = it->second;
            zfCoreMutexUnlock();
            return ZFDI_invoke(ret, errorHint, obj, methodList, paramCount, paramList)
                || _ZFP_ZFDI_errorOccurred();
        }
        else {
            zfCoreMutexUnlock();
        }
    }

    ZFCoreArray<const ZFMethod *> methodList;

    // obj->methodName()
    if(obj != zfnull) {
        obj->classData()->methodForNameGetAllT(methodList, name);
        return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList)
            || _ZFP_ZFDI_errorOccurred();
    }

    zfindex dotPos = zfstringFindReversely(name, zfslen(name), ZFNamespaceSeparator());
    if(dotPos == zfindexMax()) {
        // methodName()
        ZFMethodFuncForNameGetAllT(methodList, zfnull, name);
        if(!methodList.isEmpty()) {
            return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList)
                || _ZFP_ZFDI_errorOccurred();
        }

        // ClassName()
        // v_ClassName()
        const ZFClass *cls = ZFDI_classForName(name, zfnull);
        if(cls != zfnull) {
            return ZFDI_alloc(ret, errorHint, cls, paramCount, paramList)
                || _ZFP_ZFDI_errorOccurred();
        }

        // fail
        return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList)
            || _ZFP_ZFDI_errorOccurred();
    }
    else {
        zfstring scopeTmp(name, dotPos);
        const zfchar *nameTmp = name + dotPos + 1;

        // NS.ClassName.methodName()
        // NS.v_ClassName.methodName()
        {
            const ZFClass *cls = ZFDI_classForName(scopeTmp, zfnull);
            if(cls != zfnull) {
                cls->methodForNameGetAllT(methodList, nameTmp);
                return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList)
                    || _ZFP_ZFDI_errorOccurred();
            }
        }

        // NS.ClassName()
        // NS.v_ClassName()
        {
            const ZFClass *cls = ZFDI_classForName(name, zfnull);
            if(cls != zfnull) {
                return ZFDI_alloc(ret, errorHint, cls, paramCount, paramList)
                    || _ZFP_ZFDI_errorOccurred();
            }
        }

        // NS.methodName()
        ZFMethodFuncForNameGetAllT(methodList, scopeTmp, nameTmp);
        return _ZFP_ZFDI_invoke(ret, errorHint, obj, name, methodList, paramCount, paramList)
            || _ZFP_ZFDI_errorOccurred();
    }
}
zfbool ZFDI_invoke(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN_OPT ZFObject *obj
        , ZF_IN const ZFCoreArray<const ZFMethod *> &methodList
        , ZF_IN_OPT zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        ) {
    _ZFP_ZFDI_errorPrepare();
    if(methodList.isEmpty()) {
        if(errorHint != zfnull) {
            *errorHint += "no matching method to call";
        }
        return _ZFP_ZFDI_errorOccurred();
    }
    if(paramCount == zfindexMax()) {
        paramCount = ZFDI_paramCount(paramList);
    }

    zfstring _errorHintTmp;
    {
        zfstring *errorHintTmp = errorHint ? &_errorHintTmp : zfnull;
        _ZFP_ZFDI_ParamBackupMapType paramBackup;
        for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod) {
            for(_ZFP_ZFDI_ParamBackupMapType::iterator it = paramBackup.begin(); it != paramBackup.end(); ++it) {
                paramList[it->first] = it->second;
            }
            paramBackup.clear();

            const ZFMethod *method = methodList[iMethod];
            if(errorHintTmp != zfnull && !_errorHintTmp.isEmpty()) {
                _errorHintTmp += "\n    ";
            }
            if(!method->methodIsPublic()) {
                if(errorHintTmp != zfnull) {
                    zfstringAppend(_errorHintTmp, "can not invoke %s method: %s",
                        method->methodPrivilegeType(),
                        method);
                }
                break;
            }
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount()) {
                if(errorHintTmp != zfnull) {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %s",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount())
                            : zfstr("%s~%s", method->methodParamCountMin(), method->methodParamCount())),
                        paramCount);
                }
                continue;
            }

            zfbool paramConvertSuccess = zftrue;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam) {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramList[iParam]);
                if(wrapper != zfnull) {
                    zfauto paramTmp;
                    if(!ZFDI_objectFromString(
                                paramTmp, method->methodParamTypeIdAt(iParam), wrapper->zfv(), zfindexMax(), errorHintTmp)
                                ) {
                        paramConvertSuccess = zffalse;
                        break;
                    }
                    paramBackup[iParam] = paramList[iParam];
                    paramList[iParam] = paramTmp;
                }
            }
            if(!paramConvertSuccess) {
                continue;
            }
            if(method->methodGenericInvoker()(ret, errorHintTmp, obj, method, paramCount, paramList)) {
                if(zfstringIsEqual(method->methodReturnTypeId(), ZFTypeId_void())) {
                    ret = obj;
                }
                return zftrue;
            }
        }
    }
    if(errorHint != zfnull) {
        *errorHint += "no matching method to call";
        *errorHint += ", error hint:\n    ";
        *errorHint += _errorHintTmp;
        *errorHint += "\n  candidate methods:";
        for(zfindex i = 0; i < methodList.count(); ++i) {
            *errorHint += "\n    ";
            methodList[i]->objectInfoT(*errorHint);
        }

        if(paramCount > 0) {
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
    return _ZFP_ZFDI_errorOccurred();
}

zfbool ZFDI_alloc(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const ZFClass *cls
        , ZF_IN zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        ) {
    _ZFP_ZFDI_errorPrepare();
    if(cls == zfnull) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint, "null class");
        }
        return _ZFP_ZFDI_errorOccurred();
    }
    if(cls->classIsAbstract()) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint, "class is abstract: \"%s\"", cls->classNameFull());
        }
        return _ZFP_ZFDI_errorOccurred();
    }
    if(!cls->classCanAllocPublic()) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint, "class can only create by reflection: \"%s\"", cls->classNameFull());
        }
        return _ZFP_ZFDI_errorOccurred();
    }

    if(paramCount == zfindexMax()) {
        paramCount = ZFDI_paramCount(paramList);
    }
    if(paramCount == 0) {
        ret = cls->newInstance();
        if(ret == zfnull) {
            if(errorHint != zfnull) {
                zfstringAppend(errorHint, "unable to alloc class \"%s\"", cls->classNameFull());
            }
            return _ZFP_ZFDI_errorOccurred();
        }
        else {
            return zftrue;
        }
    }
    ZFCoreArray<const ZFMethod *> methodList;
    cls->methodForNameGetAllT(methodList, "objectOnInit");
    if(methodList.isEmpty()) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint, "class \"%s\" has no reflectable objectOnInit", cls->classNameFull());
        }
        return _ZFP_ZFDI_errorOccurred();
    }

    void *token = cls->newInstanceGenericBegin();
    if(token == zfnull) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint, "unable to alloc class \"%s\"", cls->classNameFull());
        }
        return _ZFP_ZFDI_errorOccurred();
    }

    zfstring _errorHintTmp;
    {
        zfstring *errorHintTmp = errorHint ? &_errorHintTmp : zfnull;
        _ZFP_ZFDI_ParamBackupMapType paramBackup;
        for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod) {
            for(_ZFP_ZFDI_ParamBackupMapType::iterator it = paramBackup.begin(); it != paramBackup.end(); ++it) {
                paramList[it->first] = it->second;
            }
            paramBackup.clear();

            const ZFMethod *method = methodList[iMethod];
            if(!_errorHintTmp.isEmpty()) {
                _errorHintTmp += "\n    ";
            }
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount()) {
                if(errorHintTmp != zfnull) {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %s",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount())
                            : zfstr("%s~%s", method->methodParamCountMin(), method->methodParamCount())),
                        paramCount);
                }
                continue;
            }

            zfbool paramConvertSuccess = zftrue;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam) {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramList[iParam]);
                if(wrapper != zfnull) {
                    zfauto paramTmp;
                    if(!ZFDI_objectFromString(
                                paramTmp, method->methodParamTypeIdAt(iParam), wrapper->zfv(), zfindexMax(), errorHintTmp)
                                ) {
                        paramConvertSuccess = zffalse;
                        break;
                    }
                    paramBackup[iParam] = paramList[iParam];
                    paramList[iParam] = paramTmp;
                }
            }
            if(!paramConvertSuccess) {
                continue;
            }
            if(cls->newInstanceGenericCheck(token, method, paramCount, paramList, errorHintTmp)) {
                ret = cls->newInstanceGenericEnd(token, zftrue);
                return zftrue;
            }
        }
    }
    cls->newInstanceGenericEnd(token, zffalse);
    if(errorHint != zfnull) {
        *errorHint += "no matching objectOnInit to call for class \"";
        *errorHint += cls->className();
        *errorHint += "\"";
        *errorHint += ", error hint:\n    ";
        *errorHint += _errorHintTmp;
        *errorHint += "\n  candidate methods:";
        for(zfindex i = 0; i < methodList.count(); ++i) {
            *errorHint += "\n    ";
            methodList[i]->objectInfoT(*errorHint);
        }

        if(paramCount > 0) {
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
    return _ZFP_ZFDI_errorOccurred();
}

zfbool ZFDI_objectFromString(
        ZF_OUT zfauto &ret
        , ZF_IN const ZFClass *cls
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(cls == zfnull) {
        zfstringAppend(errorHint, "null class");
        return zffalse;
    }
    else if(cls->classIsAbstract()) {
        zfstringAppend(errorHint, "class \"%s\" is abstract", cls->classNameFull());
        return zffalse;
    }
    else if(cls->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
        ret = cls->newInstance();
        ZFTypeIdWrapper *wrapper = ret;
        if(wrapper != zfnull && wrapper->wrappedValueFromString(src, srcLen)) {
            return zftrue;
        }
        else {
            zfstringAppend(errorHint, "%s can not be converted from string \"%s\"", cls->classNameFull(), zfstring(src, srcLen));
            return zffalse;
        }
    }
    else {
        if(ZFSerializeFromString(ret, cls, src, srcLen, errorHint)) {
            return zftrue;
        }
        else {
            zfstringAppend(errorHint, "%s can not be converted from string \"%s\"", cls->classNameFull(), zfstring(src, srcLen));
            return zffalse;
        }
    }
}
zfbool ZFDI_objectFromString(
        ZF_OUT zfauto &ret
        , ZF_IN const zfchar *typeId
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    const ZFClass *cls = ZFDI_classForName(typeId);
    if(cls == zfnull) {
        zfstringAppend(errorHint, "no such type \"%s\"",
            typeId);
        return zffalse;
    }
    else if(cls == ZFObject::ClassData()) {
        zfblockedAlloc(v_zfstring, tmp);
        tmp->zfv.assign(src, srcLen);
        ret = tmp;
        return zftrue;
    }
    else {
        return ZFDI_objectFromString(ret, cls, src, srcLen, errorHint);
    }
}

// ============================================================
zfauto ZFInvoke(
        ZF_IN const zfchar *name
        ) {
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfauto ret;
    if(ZFDI_invoke(ret, zfnull, zfnull, name, 0, paramList)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
zfauto ZFInvoke(
        ZF_IN const zfchar *name
        , ZF_IN ZFObject *param0
        , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
        ) {
    zfCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
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
    zfauto ret;
    if(ZFDI_invoke(ret, zfnull, zfnull, name, paramCount, paramList)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
zfauto ZFInvokeDetail(
        ZF_IN const zfchar *name
        , ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    zfCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = zfmMin((zfindex)ZFMETHOD_MAX_PARAM, params.count());
    for(zfindex i = 0; i < paramCount; ++i) {
        paramList[i].zfunsafe_assign(params[i]);
    }
    for(zfindex i = paramCount; i < ZFMETHOD_MAX_PARAM; ++i) {
        paramList[i].zfunsafe_assign(ZFMethodGenericInvokerDefaultParam());
    }
    zfCoreMutexUnlock();
    zfauto ret;
    if(ZFDI_invoke(ret, errorHint, zfnull, name, paramCount, paramList)) {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}

zfauto ZFInvokeGeneric(
        ZF_IN const zfchar *name
        , ZF_IN const zfchar *param0
        , ZF_IN_OPT const zfchar *param1 /* = zfnull */
        , ZF_IN_OPT const zfchar *param2 /* = zfnull */
        , ZF_IN_OPT const zfchar *param3 /* = zfnull */
        , ZF_IN_OPT const zfchar *param4 /* = zfnull */
        , ZF_IN_OPT const zfchar *param5 /* = zfnull */
        , ZF_IN_OPT const zfchar *param6 /* = zfnull */
        , ZF_IN_OPT const zfchar *param7 /* = zfnull */
        ) {
    zfCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = ZFMETHOD_MAX_PARAM;
    do {
        if(param0 == zfnull) {paramCount = 0; break;} else {paramList[0] = zfunsafe_zflineAlloc(ZFDI_WrapperRaw, param0);}
        if(param1 == zfnull) {paramCount = 1; break;} else {paramList[1] = zfunsafe_zflineAlloc(ZFDI_WrapperRaw, param1);}
        if(param2 == zfnull) {paramCount = 2; break;} else {paramList[2] = zfunsafe_zflineAlloc(ZFDI_WrapperRaw, param2);}
        if(param3 == zfnull) {paramCount = 3; break;} else {paramList[3] = zfunsafe_zflineAlloc(ZFDI_WrapperRaw, param3);}
        if(param4 == zfnull) {paramCount = 4; break;} else {paramList[4] = zfunsafe_zflineAlloc(ZFDI_WrapperRaw, param4);}
        if(param5 == zfnull) {paramCount = 5; break;} else {paramList[5] = zfunsafe_zflineAlloc(ZFDI_WrapperRaw, param5);}
        if(param6 == zfnull) {paramCount = 6; break;} else {paramList[6] = zfunsafe_zflineAlloc(ZFDI_WrapperRaw, param6);}
        if(param7 == zfnull) {paramCount = 7; break;} else {paramList[7] = zfunsafe_zflineAlloc(ZFDI_WrapperRaw, param7);}
    } while(zffalse);
    zfCoreMutexUnlock();
    zfauto ret;
    if(ZFDI_invoke(ret, zfnull, zfnull, name, paramCount, paramList)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
zfauto ZFInvokeGenericDetail(
        ZF_IN const zfchar *name
        , ZF_IN const ZFCoreArray<zfstring> &params
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    zfCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = zfmMin((zfindex)ZFMETHOD_MAX_PARAM, params.count());
    for(zfindex i = 0; i < paramCount; ++i) {
        paramList[i].zfunsafe_assign(zfunsafe_zflineAlloc(ZFDI_WrapperRaw, params[i]));
    }
    for(zfindex i = paramCount; i < ZFMETHOD_MAX_PARAM; ++i) {
        paramList[i].zfunsafe_assign(ZFMethodGenericInvokerDefaultParam());
    }
    zfCoreMutexUnlock();
    zfauto ret;
    if(ZFDI_invoke(ret, errorHint, zfnull, name, paramCount, paramList)) {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCoreArray<ZFOutput> &, ZFDI_errorCallbacks)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfauto, ZFInvoke
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfauto, ZFInvoke
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(ZFObject *, param0)
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMethodGenericInvokerDefaultParam())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMethodGenericInvokerDefaultParam())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfauto, ZFInvokeDetail
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(const ZFCoreArray<zfauto> &, params)
        , ZFMP_OUT_OPT(zfbool *, success, zfnull)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfauto, ZFInvokeGeneric
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(const zfchar *, param0)
        , ZFMP_IN_OPT(const zfchar *, param1, zfnull)
        , ZFMP_IN_OPT(const zfchar *, param2, zfnull)
        , ZFMP_IN_OPT(const zfchar *, param3, zfnull)
        , ZFMP_IN_OPT(const zfchar *, param4, zfnull)
        , ZFMP_IN_OPT(const zfchar *, param5, zfnull)
        , ZFMP_IN_OPT(const zfchar *, param6, zfnull)
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(const zfchar *, param7, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfauto, ZFInvokeGenericDetail
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(const ZFCoreArray<zfstring> &, params)
        , ZFMP_OUT_OPT(zfbool *, success, zfnull)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

