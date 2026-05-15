#include "ZFImpl_ZFLua_ZFSourceCodeMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfbool _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfoDetect(
        ZF_IN lua_State *L
        , ZF_OUT ZFSourceCodeMap::Info &info
        ) {
    lua_Debug ar;
    if(!lua_getstack(L, 2, &ar)) {
        return zffalse;
    }

    lua_getinfo(L, "l", &ar);
    info.line = (zfindex)ar.currentline;

    for(int iLocal = 1; ; ++iLocal) {
        const char *varName = lua_getlocal(L, &ar, iLocal); // [var]
        if(varName == NULL) {
            return zffalse;
        }
        if(strcmp(varName, "ZFLocalPathInfo") == 0) {
            int error = lua_pcall(L, 0, 1, 0); // [pathInfoObj]
            if(error != 0) {
                lua_pop(L, 1);
                return zffalse;
            }
            zfauto pathInfoHolder;
            if(!ZFImpl_ZFLua_toObject(pathInfoHolder, L, -1)) {
                lua_pop(L, 1);
                return zffalse;
            }
            v_ZFPathInfo *tmp = pathInfoHolder;
            if(tmp != zfnull) {
                info.pathInfo = tmp->zfv;
            }
            lua_pop(L, 1);
            break;
        }
    }

    return zftrue;
}

static void _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfo_method(
        ZF_IN const zfstring &scope
        , ZF_IN const zfstring &methodName
        , ZF_IN const ZFMP &mp
        , ZF_IN const ZFSourceCodeMap::Info &info
        ) {
    const ZFClass *cls = ZFClass::classForName(scope);
    const ZFMethod *m;
    if(cls != zfnull) {
        m = cls->methodForName(methodName
                , mp.paramTypeIdAt(0)
                , mp.paramTypeIdAt(1)
                , mp.paramTypeIdAt(2)
                , mp.paramTypeIdAt(3)
                , mp.paramTypeIdAt(4)
                , mp.paramTypeIdAt(5)
                , mp.paramTypeIdAt(6)
                , mp.paramTypeIdAt(7)
                );
    }
    else {
        m = ZFMethodForName(scope, methodName
                , mp.paramTypeIdAt(0)
                , mp.paramTypeIdAt(1)
                , mp.paramTypeIdAt(2)
                , mp.paramTypeIdAt(3)
                , mp.paramTypeIdAt(4)
                , mp.paramTypeIdAt(5)
                , mp.paramTypeIdAt(6)
                , mp.paramTypeIdAt(7)
                );
    }
    if(m != zfnull) {
        ZFSourceCodeMap::attachMethod(m, info);
    }
}

void ZFImpl_ZFLua_ZFSourceCodeMapUpdate(
        ZF_IN lua_State *L
        , ZF_IN const ZFArgs &zfargs
        , ZF_IN const zfstring &name
        ) {
    if(zfcast(v_ZFDynamic *, zfargs.sender()) == zfnull) {
        return;
    }
    ZFDynamic &d = zfcast(v_ZFDynamic *, zfargs.sender())->zfv;
    ZFSourceCodeMap::Info info;
    if(zffalse) {
    }
    else if(zffalse
            || name == "NSBegin"
            ) {
        if(!_ZFP_ZFImpl_ZFLua_ZFSourceCodeInfoDetect(L, info)) {return;}
        zfstring scope = d.currentScope();
        if(!scope) {
            return;
        }
        ZFSourceCodeMap::attachNamespace(scope, info);
    }
    else if(zffalse
            || name == "classBegin"
            || name == "enumBegin"
            || name == "enumBeginFlags"
            ) {
        if(!_ZFP_ZFImpl_ZFLua_ZFSourceCodeInfoDetect(L, info)) {return;}
        const ZFClass *cls = ZFClass::classForName(d.currentScope());
        if(cls == zfnull) {
            return;
        }
        ZFSourceCodeMap::attachClass(cls, info);
    }
    else if(zffalse
            || name == "event"
            ) {
        if(!_ZFP_ZFImpl_ZFLua_ZFSourceCodeInfoDetect(L, info)) {return;}
        zfstring t = ZFDI_toString(zfargs.param0());
        if(!t) {
            return;
        }
        zfstring scope = d.currentScope();
        const ZFClass *cls = ZFClass::classForName(scope);
        if(cls) {
            const ZFMethod *m = cls->methodForName(zfstr("E_%s", t));
            if(m == zfnull) {
                m = cls->methodForName(t);
            }
            if(m == zfnull) {
                return;
            }
            ZFSourceCodeMap::attachMethod(m, info);
        }
        else {
            if(scope == zfnull) {
                scope = zftext("ZFGlobalEvent");
            }
            const ZFMethod *m = ZFMethodForName(scope, zfstr("E_%s", t));
            if(m == zfnull) {
                m = ZFMethodForName(scope, t);
            }
            if(m == zfnull) {
                return;
            }
            ZFSourceCodeMap::attachMethod(m, info);
        }
    }
    else if(zffalse
            || name == "property"
            || name == "propertyWithInit"
            || name == "staticProperty"
            || name == "staticPropertyWithInit"
            ) {
        if(!_ZFP_ZFImpl_ZFLua_ZFSourceCodeInfoDetect(L, info)) {return;}
        v_ZFPropertyDynamicRegisterParam *t = zfargs.param0();
        if(t != zfnull) {
            _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfo_method(
                    d.currentScope()
                    , t->zfv.propertyName()
                    , ZFMP()
                    , info
                    );
            _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfo_method(
                    d.currentScope()
                    , t->zfv.propertyName()
                    , ZFMP().mp(t->zfv.propertyTypeId())
                    , info
                    );
        }
        else {
            zfstring propertyName = ZFDI_toString(zfargs.param1());
            if(propertyName == zfnull) {
                return;
            }
            zfstring propertyTypeId;
            {
                zfstring t = ZFDI_toString(zfargs.param0());
                if(t != zfnull) {
                    propertyTypeId = t;
                }
                else {
                    v_ZFClass *tCls = zfargs.param1();
                    if(tCls != zfnull) {
                        propertyTypeId = tCls->zfv->classNameFull();
                    }
                    else {
                        return;
                    }
                }
            }
            _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfo_method(
                    d.currentScope()
                    , propertyName
                    , ZFMP()
                    , info
                    );
            _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfo_method(
                    d.currentScope()
                    , propertyName
                    , ZFMP().mp(propertyTypeId)
                    , info
                    );
        }
    }
    else if(zffalse
            || name == "method"
            || name == "staticMethod"
            ) {
        if(!_ZFP_ZFImpl_ZFLua_ZFSourceCodeInfoDetect(L, info)) {return;}
        v_ZFMethodDynamicRegisterParam *t = zfargs.param0();
        if(t != zfnull) {
            _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfo_method(
                    t->zfv.methodNamespace() ? t->zfv.methodNamespace() : d.currentScope()
                    , t->zfv.methodName()
                    , t->zfv.getMP()
                    , info
                    );
        }
        else {
            zfstring methodName = ZFDI_toString(zfargs.param1());
            if(methodName == zfnull) {
                return;
            }
            v_ZFMP *mp = zfargs.param2();
            if(mp == zfnull) {
                return;
            }
            _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfo_method(
                    d.currentScope()
                    , methodName
                    , mp->zfv
                    , info
                    );
        }
    }
    else if(zffalse
            || name == "singleton"
            ) {
        if(!_ZFP_ZFImpl_ZFLua_ZFSourceCodeInfoDetect(L, info)) {return;}
        zfstring t = ZFDI_toString(zfargs.param0());
        if(t == zfnull) {
            return;
        }
        _ZFP_ZFImpl_ZFLua_ZFSourceCodeInfo_method(
                d.currentScope()
                , t
                , ZFMP()
                , info
                );
    }
}

ZF_NAMESPACE_GLOBAL_END

