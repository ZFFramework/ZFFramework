#include "ZFSourceCodeMap.h"
#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFSourceCodeMap)

ZFTYPEID_ACCESS_ONLY_DEFINE(Info, Info)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_Info, ZFPathInfo, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_Info, zfindex, line)

ZFEXPORT_VAR_DEFINE(zfbool, Enable, zffalse)

// ============================================================
static zfimplhashmap<zfstring, ZFCoreArray<Info> > _ZFP_SCM_Namespace;
ZFMETHOD_FUNC_DEFINE_2(void, attachNamespace
        , ZFMP_IN(const zfstring &, v)
        , ZFMP_IN(const Info &, info)
        ) {
    if(v == zfnull) {
        return;
    }
    ZFCoreArray<Info> &list = _ZFP_SCM_Namespace[v];
    if(!list.isContain(info)) {
        list.add(info);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, detachNamespace
        , ZFMP_IN(const zfstring &, v)
        , ZFMP_IN(const Info &, info)
        ) {
    zfimplhashmap<zfstring, ZFCoreArray<Info> >::iterator it = _ZFP_SCM_Namespace.find(v);
    if(it == _ZFP_SCM_Namespace.end()) {
        return;
    }
    it->second.removeElement(info);
    if(it->second.isEmpty()) {
        _ZFP_SCM_Namespace.erase(it);
    }
}
ZFMETHOD_FUNC_DEFINE_1(ZFCoreArray<Info>, checkNamespace
        , ZFMP_IN(const zfstring &, v)
        ) {
    zfimplhashmap<zfstring, ZFCoreArray<Info> >::iterator it = _ZFP_SCM_Namespace.find(v);
    if(it != _ZFP_SCM_Namespace.end()) {
        return it->second;
    }
    else {
        return ZFCoreArray<Info>();
    }
}

// ============================================================
static zfimplhashmap<const ZFClass *, ZFCoreArray<Info> > _ZFP_SCM_Class;
ZFMETHOD_FUNC_DEFINE_2(void, attachClass
        , ZFMP_IN(const ZFClass *, v)
        , ZFMP_IN(const Info &, info)
        ) {
    if(v == zfnull) {
        return;
    }
    ZFCoreArray<Info> &list = _ZFP_SCM_Class[v];
    if(!list.isContain(info)) {
        list.add(info);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, detachClass
        , ZFMP_IN(const ZFClass *, v)
        , ZFMP_IN(const Info &, info)
        ) {
    zfimplhashmap<const ZFClass *, ZFCoreArray<Info> >::iterator it = _ZFP_SCM_Class.find(v);
    if(it == _ZFP_SCM_Class.end()) {
        return;
    }
    it->second.removeElement(info);
    if(it->second.isEmpty()) {
        _ZFP_SCM_Class.erase(it);
    }
}
ZFMETHOD_FUNC_DEFINE_1(ZFCoreArray<Info>, checkClass
        , ZFMP_IN(const ZFClass *, v)
        ) {
    zfimplhashmap<const ZFClass *, ZFCoreArray<Info> >::iterator it = _ZFP_SCM_Class.find(v);
    if(it != _ZFP_SCM_Class.end()) {
        return it->second;
    }
    else {
        return ZFCoreArray<Info>();
    }
}

// ============================================================
static zfimplhashmap<const ZFMethod *, ZFCoreArray<Info> > _ZFP_SCM_Method;
ZFMETHOD_FUNC_DEFINE_2(void, attachMethod
        , ZFMP_IN(const ZFMethod *, v)
        , ZFMP_IN(const Info &, info)
        ) {
    if(v == zfnull) {
        return;
    }
    ZFCoreArray<Info> &list = _ZFP_SCM_Method[v];
    if(!list.isContain(info)) {
        list.add(info);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, detachMethod
        , ZFMP_IN(const ZFMethod *, v)
        , ZFMP_IN(const Info &, info)
        ) {
    zfimplhashmap<const ZFMethod *, ZFCoreArray<Info> >::iterator it = _ZFP_SCM_Method.find(v);
    if(it == _ZFP_SCM_Method.end()) {
        return;
    }
    it->second.removeElement(info);
    if(it->second.isEmpty()) {
        _ZFP_SCM_Method.erase(it);
    }
}
ZFMETHOD_FUNC_DEFINE_1(ZFCoreArray<Info>, checkMethod
        , ZFMP_IN(const ZFMethod *, v)
        ) {
    zfimplhashmap<const ZFMethod *, ZFCoreArray<Info> >::iterator it = _ZFP_SCM_Method.find(v);
    if(it != _ZFP_SCM_Method.end()) {
        return it->second;
    }
    else {
        return ZFCoreArray<Info>();
    }
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSourceCodeMapAutoClean, ZFLevelZFFrameworkPostNormal) {
    ZFClassDataUpdateObserver().observerAdd(ZFGlobalEvent::E_ClassDataUpdate(), ZFCallbackForFunc(zfself::action));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFSourceCodeMapAutoClean) {
    ZFClassDataUpdateObserver().observerRemove(ZFGlobalEvent::E_ClassDataUpdate(), ZFCallbackForFunc(zfself::action));
}
private:
    static void action(ZF_IN const ZFArgs &zfargs) {
        ZFClassDataUpdateData const &data = zfargs.param0().to<v_ZFClassDataUpdateData *>()->zfv;
        if(data.changeType == ZFClassDataUpdateTypeDetach) {
            if(data.changedClass) {
                _ZFP_SCM_Class.erase(data.changedClass);
            }
            if(data.changedMethod) {
                _ZFP_SCM_Method.erase(data.changedMethod);
            }
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFSourceCodeMapAutoClean)

ZF_NAMESPACE_END(ZFSourceCodeMap)
ZF_NAMESPACE_GLOBAL_END

