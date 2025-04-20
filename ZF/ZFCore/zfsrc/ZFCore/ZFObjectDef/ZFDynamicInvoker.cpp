#include "ZFDynamicInvoker.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlmap.h" // for ZFDI_invoke param convert backup

// #define _ZFP_ZFDI_DEBUG 1

#if _ZFP_ZFDI_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFDI_invokeTimeLogger(fmt, ...) \
        zfimplInvokeTimeLoggerWithCost(2, "[ZFDI] " fmt \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_ZFDI_invokeTimeLogger(fmt, ...)
#endif

#define _ZFP_ZFDI_CACHE_ENABLE 1

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
ZFCoreArray<ZFOutput> &ZFDI_errorCallbacks(void) {
    static ZFCoreArray<ZFOutput> d;
    return d;
}
static void _ZFP_ZFDI_errorOccurred(ZF_IN const ZFArgs &zfargs) {
    if(!zfargs.ignoreErrorEvent()
            && !zfargs.ignoreError()
            && zfargs.errorHint()
            && !ZFDI_errorCallbacks().isEmpty()
            ) {
        for(zfindex i = 0; i < ZFDI_errorCallbacks().count(); ++i) {
            const ZFOutput &o = ZFDI_errorCallbacks()[i];
            o.execute(zfargs.errorHint(), zfargs.errorHint().length());
            o.execute("\n");
        }
    }
}

// ============================================================
const zfchar *ZFDI_toString(ZF_IN ZFObject *obj) {
    if(obj == zfnull) {
        return zfnull;
    }
    {
        v_zfstring *t = zfcast(v_zfstring *, obj);
        if(t != zfnull) {
            return t->zfv;
        }
    }
    {
        ZFDI_WrapperBase *t = zfcast(ZFDI_WrapperBase *, obj);
        if(t != zfnull) {
            return t->zfv();
        }
    }
    return zfnull;
}

typedef zfbool (*_ZFP_ZFDI_toNumberConv)(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDI_toNumber_DataHolder, ZFLevelZFFrameworkEssential) {
    m[v_zfbool::ClassData()] = _conv_zfbool;
    m[v_zfindex::ClassData()] = _conv_zfindex;
    m[v_zfint::ClassData()] = _conv_zfint;
    m[v_zfuint::ClassData()] = _conv_zfuint;
    m[v_zflong::ClassData()] = _conv_zflong;
    m[v_zfulong::ClassData()] = _conv_zfulong;
    m[v_zffloat::ClassData()] = _conv_zffloat;
    m[v_zfdouble::ClassData()] = _conv_zfdouble;
    m[v_zfbyte::ClassData()] = _conv_zfbyte;
    m[v_zftimet::ClassData()] = _conv_zftimet;
    m[v_zfflags::ClassData()] = _conv_zfflags;
    m[v_zfidentity::ClassData()] = _conv_zfidentity;
}
public:
    _ZFP_ZFDI_toNumberConv check(ZF_IN const ZFClass *cls) {
        zfstlmap<const ZFClass *, _ZFP_ZFDI_toNumberConv>::iterator it = m.find(cls);
        if(it != m.end()) {
            return it->second;
        }
        else {
            return zfnull;
        }
    }
private:
    zfstlmap<const ZFClass *, _ZFP_ZFDI_toNumberConv> m;
private:
    static zfbool _conv_zfbool(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        ret = zfcast(v_zfbool *, obj)->zfv ? (zft_zfdouble)1 : (zft_zfdouble)0;
        return zftrue;
    }
    static zfbool _conv_zfindex(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        v_zfindex *t = zfcast(v_zfindex *, obj);
        ret = t->zfv == zfindexMax() ? (zft_zfdouble)-1 : (zft_zfdouble)t->zfv;
        return zftrue;
    }
    static zfbool _conv_zfint(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        ret = (zft_zfdouble)(zfcast(v_zfint *, obj)->zfv);
        return zftrue;
    }
    static zfbool _conv_zfuint(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        v_zfuint *t = zfcast(v_zfuint *, obj);
        ret = t->zfv == (zfuint)-1 ? (zft_zfdouble)-1 : (zft_zfdouble)t->zfv;
        return zftrue;
    }
    static zfbool _conv_zflong(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        ret = (zft_zfdouble)(zfcast(v_zflong *, obj)->zfv);
        return zftrue;
    }
    static zfbool _conv_zfulong(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        v_zfulong *t = zfcast(v_zfulong *, obj);
        ret = t->zfv == (zfulong)-1 ? (zft_zfdouble)-1 : (zft_zfdouble)t->zfv;
        return zftrue;
    }
    static zfbool _conv_zffloat(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        ret = (zft_zfdouble)(zfcast(v_zffloat *, obj)->zfv);
        return zftrue;
    }
    static zfbool _conv_zfdouble(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        ret = zfcast(v_zfdouble *, obj)->zfv;
        return zftrue;
    }
    static zfbool _conv_zfbyte(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        ret = (zft_zfdouble)(zfcast(v_zfbyte *, obj)->zfv);
        return zftrue;
    }
    static zfbool _conv_zftimet(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        ret = (zft_zfdouble)(zfcast(v_zftimet *, obj)->zfv);
        return zftrue;
    }
    static zfbool _conv_zfflags(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        ret = (zft_zfdouble)(zfcast(v_zfflags *, obj)->zfv);
        return zftrue;
    }
    static zfbool _conv_zfidentity(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
        v_zfidentity *t = zfcast(v_zfidentity *, obj);
        ret = t->zfv == zfidentityInvalid() ? (zft_zfdouble)-1 : (zft_zfdouble)t->zfv;
        return zftrue;
    }
ZF_GLOBAL_INITIALIZER_END(ZFDI_toNumber_DataHolder)

zfbool ZFDI_toNumberT(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj) {
    const ZFClass *cls = obj->classData();
    _ZFP_ZFDI_toNumberConv conv = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDI_toNumber_DataHolder)->check(cls);
    if(conv) {
        return conv(ret, obj);
    }
    else if(cls->classIsTypeOf(ZFEnum::ClassData())) {
        ZFEnum *t = zfcast(ZFEnum *, obj);
        ret = t->enumValue() == ZFEnumInvalid() ? (zft_zfdouble)-1 : (zft_zfdouble)t->enumValue();
        return zftrue;
    }
    const zfchar *s = ZFDI_toString(obj);
    if(s == zfnull) {
        return zffalse;
    }
    return zfdoubleFromStringT(ret, s);
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
void ZFDI_paramInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const ZFArgs &zfargs
        ) {
    for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i) {
        if(zfargs.paramAt(i) == ZFMP_DEF()) {
            return;
        }
        if(i != 0) {
            ret += ", ";
        }
        _ZFP_ZFDI_paramInfo(ret, zfargs.paramAt(i));
    }
}

// ============================================================
static zfbool _ZFP_ZFDI_paramConvert(
        ZF_IN const ZFMethod *method
        , ZF_IN_OUT _ZFP_ZFDI_ParamBackupMapType &paramBackup
        , ZF_IN_OUT const ZFArgs &zfargs
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN_OPT zfbool convStr /* = zffalse */
        ) {
    for(zfindex iParam = 0; iParam < ZFMETHOD_MAX_PARAM; ++iParam) {
        ZFObject *param = zfargs.paramAt(iParam);
        if(param == ZFMP_DEF()) {
            if(iParam < method->paramCountMin()) {
                if(errorHint != zfnull) {
                    zfstringAppend(errorHint, "expect %s param, got %s",
                            ((method->paramCountMin() == method->paramCount())
                             ? zfindexToString(method->paramCount())
                             : zfstr("%s~%s", method->paramCountMin(), method->paramCount())),
                            iParam);
                }
                return zffalse;
            }
            return zftrue;
        }
        else if(iParam >= method->paramCount()) {
            if(errorHint != zfnull) {
                zfstringAppend(errorHint, "expect %s param, got %s",
                        ((method->paramCountMin() == method->paramCount())
                         ? zfindexToString(method->paramCount())
                         : zfstr("%s~%s", method->paramCountMin(), method->paramCount())),
                        ZFDI_paramCount(zfargs));
            }
            return zffalse;
        }

        const zfchar *s = zfnull;
        ZFDI_WrapperBase *wrapper = zfargs.paramAt(iParam);
        if(wrapper != zfnull) {
            s = wrapper->zfv();
            if(s == zfnull) {
                s = "";
            }
        }
        else if(convStr) {
            v_zfstring *holder = zfargs.paramAt(iParam);
            if(holder) {
                const ZFClass *cls = ZFClass::classForName(method->paramTypeIdAt(iParam));
                if(cls != ZFObject::ClassData()
                        && !v_zfstring::ClassData()->classIsTypeOf(cls)
                        ) {
                    s = holder->zfv;
                    if(s == zfnull) {
                        s = "";
                    }
                }
            }
        }
        if(s != zfnull) {
            zfauto paramTmp;
            if(!ZFDI_objectFromString(
                        paramTmp, method->paramTypeIdAt(iParam), s, zfindexMax(), errorHint)
                        ) {
                return zffalse;
            }
            paramBackup[iParam] = zfargs.paramAt(iParam);
            zfargs.param(iParam, paramTmp);
            continue;
        }
    }
    return zftrue;
}

#if _ZFP_ZFDI_CACHE_ENABLE
zfclassPOD _ZFP_ZFDI_CacheData {
public:
    // cls && !m : alloc with no param
    // cls && m : alloc with custom ctor
    // !cls && m : method call
    const ZFClass *cls;
    const ZFMethod *m;
};
static zfstlmap<zfstring, _ZFP_ZFDI_CacheData> _ZFP_ZFDI_cacheMap;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDI_cache, ZFLevelZFFrameworkEssential) {
    _ZFP_ZFDI_cacheMap.clear();
    ZFClassDataUpdateObserver().observerAdd(ZFGlobalEvent::E_ClassDataUpdate(), ZFCallbackForFunc(zfself::classOnUpdate));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDI_cache) {
    ZFClassDataUpdateObserver().observerRemove(ZFGlobalEvent::E_ClassDataUpdate(), ZFCallbackForFunc(zfself::classOnUpdate));
}
private:
    static void classOnUpdate(ZF_IN const ZFArgs &zfargs) {
        v_ZFClassDataUpdateData *info = zfargs.param0();
        if(info->zfv.changeType == ZFClassDataUpdateTypeDetach) {
            const ZFMethod *m = info->zfv.changedMethod;
            const ZFClass *cls = info->zfv.changedClass;
            if(m) {
                if(cls) {
                    for(zfstlmap<zfstring, _ZFP_ZFDI_CacheData>::iterator it = _ZFP_ZFDI_cacheMap.begin(); it != _ZFP_ZFDI_cacheMap.end();) {
                        if(it->second.m == m || it->second.cls == cls) {_ZFP_ZFDI_cacheMap.erase(it++);} else {++it;}
                    }
                }
                else {
                    for(zfstlmap<zfstring, _ZFP_ZFDI_CacheData>::iterator it = _ZFP_ZFDI_cacheMap.begin(); it != _ZFP_ZFDI_cacheMap.end();) {
                        if(it->second.m == m) {_ZFP_ZFDI_cacheMap.erase(it++);} else {++it;}
                    }
                }
            }
            else {
                if(cls) {
                    for(zfstlmap<zfstring, _ZFP_ZFDI_CacheData>::iterator it = _ZFP_ZFDI_cacheMap.begin(); it != _ZFP_ZFDI_cacheMap.end();) {
                        if(it->second.cls == cls) {_ZFP_ZFDI_cacheMap.erase(it++);} else {++it;}
                    }
                }
            }
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFDI_cache)

static zfstring _ZFP_ZFDI_cacheKey(
        ZF_IN ZFObject *obj
        , ZF_IN const ZFArgs &zfargs
        , ZF_IN const zfstring &methodName
        , ZF_IN zfbool convStr
        ) {
    zfstring ret;
    if(obj) {
        zfidentityToStringT(ret, obj->classData()->classId());
    }
    ret += '.';
    ret += methodName;
    for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i) {
        ZFObject *param = zfargs.paramAt(i);
        if(param == ZFMP_DEF()) {
            break;
        }
        ret += ':';
        if(param) {
            zfidentityToStringT(ret, param->classData()->classId());
        }
    }
    if(!convStr) {
        ret += '*';
    }
    return ret;
}
#endif

static zfbool _ZFP_ZFDI_invokeAction(
        ZF_IN ZFObject *obj
        , ZF_IN const ZFArgs &zfargs
        , ZF_IN const ZFMethod *method
        , ZF_IN_OUT _ZFP_ZFDI_ParamBackupMapType &paramBackup
        , ZF_IN_OUT zfstring *errorHint
        , ZF_IN zfbool convStr
        ) {
    if(!method->methodInvokeCheck(zfargs.sender(), errorHint)) {
        return zffalse;
    }
    if(_ZFP_ZFDI_paramConvert(
                method
                , paramBackup
                , zfargs
                , !zfargs.ignoreError() ? errorHint : zfnull
                , convStr
                )) {
        zfargs.success(zftrue);
        zfargs.errorHint(zfnull);
        zfargs._ZFP_ZFArgs_removeConst().ownerMethod(method);
        method->methodGenericInvoker()(zfargs);
        if(zfargs.success()) {
            if(zfstringIsEqual(method->returnTypeId(), ZFTypeId_void())) {
                zfargs.result(obj);
            }
            return zftrue;
        }
        else {
            if(errorHint) {
                *errorHint += zfargs.errorHint();
            }
        }
    }

    for(_ZFP_ZFDI_ParamBackupMapType::iterator it = paramBackup.begin(); it != paramBackup.end(); ++it) {
        zfargs.param(it->first, it->second);
    }
    paramBackup.clear();
    return zffalse;
}
static void _ZFP_ZFDI_invoke(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN const zfstring &name
        , ZF_IN const ZFCoreArray<const ZFMethod *> &methodList
        , ZF_IN_OPT zfbool convStr
#if _ZFP_ZFDI_CACHE_ENABLE
        , ZF_IN const zfstring &cacheKey
#endif
        ) {
    if(methodList.isEmpty()) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfstring errorHint;
            errorHint += "no matching method to call: ";
            errorHint += name;
            errorHint += "(";
            ZFDI_paramInfoT(errorHint, zfargs);
            errorHint += ")";
            if(zfargs.sender()) {
                errorHint += ", for object: ";
                zfargs.sender()->objectInfoT(errorHint);
            }
            zfargs.errorHint(errorHint);
        }
        return _ZFP_ZFDI_errorOccurred(zfargs);
    }

    ZFObject *obj = zfargs.sender();
    if(obj != zfnull) {
        obj = obj->_ZFP_ZFObject_ZFImplementDynamicOwnerOrSelf();
    }
    if(methodList.isEmpty()) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfargs.errorHint("no method to call");
        }
        return _ZFP_ZFDI_errorOccurred(zfargs);
    }

    zfstring errorHint;
    {
        _ZFP_ZFDI_ParamBackupMapType paramBackup;
        for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod) {
            const ZFMethod *method = methodList[iMethod];
            if(!zfargs.ignoreError() && !errorHint.isEmpty()) {
                errorHint += "\n    ";
            }

            if(_ZFP_ZFDI_invokeAction(obj, zfargs, method, paramBackup, &errorHint, convStr)) {
#if _ZFP_ZFDI_CACHE_ENABLE
                for(_ZFP_ZFDI_ParamBackupMapType::iterator it = paramBackup.begin(); it != paramBackup.end(); ++it) {
                    zfargs.param(it->first, it->second);
                }
                paramBackup.clear();
                _ZFP_ZFDI_CacheData &cache = _ZFP_ZFDI_cacheMap[cacheKey];
                cache.cls = zfnull;
                cache.m = method;
#endif
                return;
            }
        }
    }
    zfargs.success(zffalse);
    if(!zfargs.ignoreError()) {
        zfstring errorHintTmp;
        errorHintTmp += "no matching method to call";
        errorHintTmp += ", error hint:\n    ";
        errorHintTmp += errorHint;
        errorHintTmp += "\n  candidate methods:";
        for(zfindex i = 0; i < methodList.count(); ++i) {
            errorHintTmp += "\n    ";
            methodList[i]->objectInfoT(errorHintTmp);
        }
        if(ZFDI_paramCount(zfargs) > 0) {
            errorHintTmp += "\n  with params: (";
            ZFDI_paramInfoT(errorHintTmp, zfargs);
            errorHintTmp += ")";
        }
        zfargs.errorHint(errorHintTmp);
    }
    return _ZFP_ZFDI_errorOccurred(zfargs);
}

static zfbool _ZFP_ZFDI_alloc(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN const ZFClass *cls
        , ZF_IN_OPT zfbool convStr /* = zffalse */
        , ZF_OUT_OPT const ZFMethod **ctorMethod = zfnull
        );

void ZFDI_invoke(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN const zfstring &name
        , ZF_IN_OPT zfbool convStr /* = zffalse */
        ) {
    ZFObject *obj = zfargs.sender();
    if(obj != zfnull) {
        obj = obj->_ZFP_ZFObject_ZFImplementDynamicOwnerOrSelf();
    }
    _ZFP_ZFDI_invokeTimeLogger("ivk: %s::%s"
            , obj ? obj->classData()->classNameFull().cString() : ""
            , name.cString()
            );
#if _ZFP_ZFDI_CACHE_ENABLE
    zfstring cacheKey = _ZFP_ZFDI_cacheKey(obj, zfargs, name, convStr);
    zfstlmap<zfstring, _ZFP_ZFDI_CacheData>::iterator itCache = _ZFP_ZFDI_cacheMap.find(cacheKey);
    if(itCache != _ZFP_ZFDI_cacheMap.end()) {
        const _ZFP_ZFDI_CacheData &cache = itCache->second;
        if(cache.cls) {
            if(cache.m) {
                ZFCoreMutexLocker();
                _ZFP_ZFDI_ParamBackupMapType paramBackup;
                void *token = cache.cls->newInstanceGenericBegin();
                if(_ZFP_ZFDI_paramConvert(cache.m, paramBackup, zfargs, zfnull, convStr)
                        && cache.cls->newInstanceGenericCheck(token, cache.m, zfargs)
                        ) {
                    zfargs.result(cache.cls->newInstanceGenericEnd(token, zftrue));
                    return;
                }
                cache.cls->newInstanceGenericEnd(token, zffalse);
                for(_ZFP_ZFDI_ParamBackupMapType::iterator it = paramBackup.begin(); it != paramBackup.end(); ++it) {
                    zfargs.param(it->first, it->second);
                }
                paramBackup.clear();
            }
            else {
                zfauto ret = cache.cls->newInstance();
                if(ret) {
                    zfargs.result(ret);
                    return;
                }
            }
        }
        else {
            _ZFP_ZFDI_ParamBackupMapType paramBackup;
            if(_ZFP_ZFDI_invokeAction(obj, zfargs, cache.m, paramBackup, zfnull, convStr)) {
                return;
            }
        }
#if _ZFP_ZFDI_DEBUG
        // not an error, typical case:
        //     // declare
        //     void test(int) {}
        //     void test(float) {}
        //     // call
        //     test('123');
        //     test('123.456');
        ZFCoreLogTrim("[ZFDI] cache mismatch: %s, obj: %s, args: [%s], cache cls: %s, cache method: %s"
                , name
                , ZFObjectInfoOfInstance(obj)
                , ZFDI_paramInfo(zfargs)
                , cache.cls
                , cache.m
                );
#endif
    }
#if _ZFP_ZFDI_DEBUG
    // cache miss debug
    ZFCoreLogTrim("[ZFDI] cache miss: %s, obj: %s, args: [%s]"
            , name
            , ZFObjectInfoOfInstance(obj)
            , ZFDI_paramInfo(zfargs)
            );
#endif
#endif

    ZFCoreArray<const ZFMethod *> methodList;

    // obj->methodName()
    if(obj != zfnull) {
        obj->classData()->methodForNameGetAllT(methodList, name);
        return _ZFP_ZFDI_invoke(zfargs, name, methodList, convStr
#if _ZFP_ZFDI_CACHE_ENABLE
                , cacheKey
#endif
                );
    }

    zfindex dotPos = zfstringFindReversely(name, zfslen(name), ".");
    if(dotPos == zfindexMax()) {
        // ClassName()
        // v_ClassName()
        const ZFClass *cls = ZFClass::classForName(name, zfnull);
        if(cls != zfnull) {
            const ZFMethod *ctorMethod = zfnull;
            if(_ZFP_ZFDI_alloc(zfargs, cls, convStr, &ctorMethod)) {
#if _ZFP_ZFDI_CACHE_ENABLE
                _ZFP_ZFDI_CacheData &cache = _ZFP_ZFDI_cacheMap[cacheKey];
                cache.cls = cls;
                cache.m = ctorMethod;
#endif
            }
            return;
        }

        // methodName()
        ZFMethodFuncForNameGetAllT(methodList, zfnull, name);
        if(!methodList.isEmpty()) {
            return _ZFP_ZFDI_invoke(zfargs, name, methodList, convStr
#if _ZFP_ZFDI_CACHE_ENABLE
                    , cacheKey
#endif
                    );
        }

        // fail
        return _ZFP_ZFDI_invoke(zfargs, name, methodList, convStr
#if _ZFP_ZFDI_CACHE_ENABLE
                , cacheKey
#endif
                );
    }
    else {
        // NS.ClassName()
        // NS.v_ClassName()
        // NS.ClassName.InnerClassName()
        {
            const ZFClass *cls = ZFClass::classForName(name, zfnull);
            if(cls != zfnull) {
                const ZFMethod *ctorMethod = zfnull;
                if(_ZFP_ZFDI_alloc(zfargs, cls, convStr, &ctorMethod)) {
#if _ZFP_ZFDI_CACHE_ENABLE
                    _ZFP_ZFDI_CacheData &cache = _ZFP_ZFDI_cacheMap[cacheKey];
                    cache.cls = cls;
                    cache.m = ctorMethod;
#endif
                }
                return;
            }
        }

        zfstring scopeTmp(name, dotPos);
        const zfchar *nameTmp = name + dotPos + 1;

        // NS.ClassName.methodName()
        // NS.v_ClassName.methodName()
        {
            const ZFClass *cls = ZFClass::classForName(scopeTmp, zfnull);
            if(cls != zfnull) {
                cls->methodForNameGetAllT(methodList, nameTmp);
                return _ZFP_ZFDI_invoke(zfargs, name, methodList, convStr
#if _ZFP_ZFDI_CACHE_ENABLE
                        , cacheKey
#endif
                        );
            }
        }

        // NS.methodName()
        {
            ZFMethodFuncForNameGetAllT(methodList, scopeTmp, nameTmp);
            if(!methodList.isEmpty()) {
                return _ZFP_ZFDI_invoke(zfargs, name, methodList, convStr
#if _ZFP_ZFDI_CACHE_ENABLE
                        , cacheKey
#endif
                        );
            }
        }

        // fail
        return _ZFP_ZFDI_invoke(zfargs, name, methodList, convStr
#if _ZFP_ZFDI_CACHE_ENABLE
                , cacheKey
#endif
                );
    }
}

static zfbool _ZFP_ZFDI_alloc(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN const ZFClass *cls
        , ZF_IN_OPT zfbool convStr
        , ZF_OUT_OPT const ZFMethod **ctorMethod /* = zfnull */
        ) {
    if(cls->classIsAbstract()) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfargs.errorHint(zfstr("class is abstract: \"%s\""
                        , cls->classNameFull()
                        ));
        }
        _ZFP_ZFDI_errorOccurred(zfargs);
        return zffalse;
    }
    if(!cls->classCanAllocPublic()) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfargs.errorHint(zfstr("class can only create by reflection: \"%s\""
                        , cls->classNameFull()
                        ));
        }
        _ZFP_ZFDI_errorOccurred(zfargs);
        return zffalse;
    }

    if(zfargs.param0() == ZFMP_DEF()) {
        zfauto ret = cls->newInstance();
        if(ret == zfnull) {
            zfargs.success(zffalse);
            if(!zfargs.ignoreError()) {
                zfargs.errorHint(zfstr("unable to alloc class \"%s\""
                            , cls->classNameFull()
                            ));
            }
            _ZFP_ZFDI_errorOccurred(zfargs);
            return zffalse;
        }
        else {
            zfargs.result(ret);
            if(ctorMethod) {
                *ctorMethod = zfnull;
            }
            return zftrue;
        }
    }
    ZFCoreArray<const ZFMethod *> methodList;
    cls->methodForNameGetAllT(methodList, "objectOnInit");
    if(methodList.isEmpty()) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfargs.errorHint(zfstr("class \"%s\" has no reflectable objectOnInit"
                        , cls->classNameFull()
                        ));
        }
        _ZFP_ZFDI_errorOccurred(zfargs);
        return zffalse;
    }

    ZFCoreMutexLocker();
    void *token = cls->newInstanceGenericBegin();
    if(token == zfnull) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfargs.errorHint(zfstr("unable to alloc class \"%s\""
                        , cls->classNameFull()
                        ));
        }
        _ZFP_ZFDI_errorOccurred(zfargs);
        return zffalse;
    }

    zfstring errorHint;
    {
        _ZFP_ZFDI_ParamBackupMapType paramBackup;
        for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod) {
            const ZFMethod *method = methodList[iMethod];
            if(!zfargs.ignoreError() && !errorHint.isEmpty()) {
                errorHint += "\n    ";
            }

            zfargs.errorHint(zfnull);
            if(_ZFP_ZFDI_paramConvert(
                        method
                        , paramBackup
                        , zfargs
                        , !zfargs.ignoreError() ? &errorHint : zfnull
                        , convStr
                        )) {
                if(cls->newInstanceGenericCheck(token, method, zfargs)) {
                    zfargs.result(cls->newInstanceGenericEnd(token, zftrue));
                    if(ctorMethod) {
                        *ctorMethod = method;
                    }
                    return zftrue;
                }
                else {
                    if(!zfargs.ignoreError()) {
                        errorHint += zfargs.errorHint();
                    }
                }
            }

            for(_ZFP_ZFDI_ParamBackupMapType::iterator it = paramBackup.begin(); it != paramBackup.end(); ++it) {
                zfargs.param(it->first, it->second);
            }
            paramBackup.clear();
        }
    }
    cls->newInstanceGenericEnd(token, zffalse);
    zfargs.success(zffalse);
    if(!zfargs.ignoreError()) {
        zfstring errorHintTmp;
        errorHintTmp += "no matching objectOnInit to call for class \"";
        errorHintTmp += cls->className();
        errorHintTmp += "\"";
        errorHintTmp += ", error hint:\n    ";
        errorHintTmp += errorHint;
        errorHintTmp += "\n  candidate methods:";
        for(zfindex i = 0; i < methodList.count(); ++i) {
            errorHintTmp += "\n    ";
            methodList[i]->objectInfoT(errorHintTmp);
        }
        if(ZFDI_paramCount(zfargs) > 0) {
            errorHintTmp += "\n  with params: (";
            ZFDI_paramInfoT(errorHintTmp, zfargs);
            errorHintTmp += ")";
        }
        zfargs.errorHint(errorHintTmp);
    }
    _ZFP_ZFDI_errorOccurred(zfargs);
    return zffalse;
}
void ZFDI_alloc(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN const ZFClass *cls
        , ZF_IN_OPT zfbool convStr /* = zffalse */
        ) {
    if(cls == zfnull) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfargs.errorHint("null class");
        }
        _ZFP_ZFDI_errorOccurred(zfargs);
        return;
    }
    _ZFP_ZFDI_alloc(zfargs, cls, convStr);
}

zfbool ZFDI_objectFromString(
        ZF_OUT zfauto &ret
        , ZF_IN const zfstring &typeId
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    const ZFClass *cls = ZFClass::classForName(typeId);
    if(cls == zfnull) {
        zfstringAppend(errorHint, "no such type \"%s\"", typeId);
        return zffalse;
    }
    else if(cls == ZFObject::ClassData()) {
        zfobj<v_zfstring> tmp;
        tmp->zfv.assign(src, srcLen);
        ret = tmp;
        return zftrue;
    }
    else {
        return ZFObjectFromStringT(ret, cls, src, srcLen, errorHint);
    }
}

zfbool ZFDI_implicitConvertT(
        ZF_OUT zfauto &ret
        , ZF_IN const zfstring &desiredTypeId
        , ZF_IN ZFObject *value
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    const ZFClass *cls = ZFClass::classForName(desiredTypeId);
    if(cls == zfnull) {
        return zffalse;
    }
    if(value == zfnull) {
        if(cls->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
            ret = cls->newInstance();
            return (ret != zfnull);
        }
        else {
            ret = zfnull;
            return zftrue;
        }
    }
    else if(value->classData()->classIsTypeOf(cls)) {
        ret = value;
        return zftrue;
    }
    else {
        // try to convert by construct new value
        ZFArgs zfargs;
        zfargs
            .paramInit()
            .param0(value)
            .ignoreErrorEvent(errorHint != zfnull)
            ;
        ZFDI_alloc(zfargs, cls, zftrue);
        if(zfargs.success()) {
            ret = zfargs.result();
        }
        else {
            if(errorHint != zfnull) {
                *errorHint += zfargs.errorHint();
            }
        }
        return zfargs.success();
    }
}

// ============================================================
zfauto ZFInvoke(
        ZF_IN const zfstring &name
        ) {
    ZFArgs zfargs;
    zfargs
        .paramInit()
        ;
    ZFDI_invoke(zfargs, name, zftrue);
    return zfargs.result();
}
zfauto ZFInvoke(
        ZF_IN const zfstring &name
        , ZF_IN ZFObject *param0
        , ZF_IN_OPT ZFObject *param1 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMP_DEF() */
        ) {
    ZFArgs zfargs;
    zfargs
        .paramInit(
                param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
                )
        ;
    ZFDI_invoke(zfargs, name, zftrue);
    return zfargs.result();
}
zfbool ZFInvokeT(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const zfstring &name
        , ZF_IN_OPT ZFObject *param0 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param1 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMP_DEF() */
        ) {
    ZFArgs zfargs;
    zfargs
        .paramInit(
                param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
                )
        .ignoreErrorEvent(errorHint != zfnull)
        ;
    ZFDI_invoke(zfargs, name, zftrue);
    ret = zfargs.result();
    if(errorHint != zfnull) {
        *errorHint += zfargs.errorHint();
    }
    return zfargs.success();
}
zfauto ZFInvokeDetail(
        ZF_IN const zfstring &name
        , ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFArgs zfargs;
    zfargs
        .paramInit(params)
        .ignoreErrorEvent(errorHint != zfnull)
        ;
    ZFDI_invoke(zfargs, name, zftrue);
    if(success != zfnull) {
        *success = zfargs.success();
    }
    if(zfargs.success()) {
        return zfargs.result();
    }
    else {
        if(errorHint != zfnull) {
            *errorHint += zfargs.errorHint();
        }
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCoreArray<ZFOutput> &, ZFDI_errorCallbacks)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfauto, ZFInvoke
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfauto, ZFInvoke
        , ZFMP_IN(const zfstring &, name)
        , ZFMP_IN(ZFObject *, param0)
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfbool, ZFInvokeT
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_OUT(zfstring *, errorHint)
        , ZFMP_IN(const zfstring &, name)
        , ZFMP_IN_OPT(ZFObject *, param0, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        // , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfauto, ZFInvokeDetail
        , ZFMP_IN(const zfstring &, name)
        , ZFMP_IN(const ZFCoreArray<zfauto> &, params)
        , ZFMP_OUT_OPT(zfbool *, success, zfnull)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

