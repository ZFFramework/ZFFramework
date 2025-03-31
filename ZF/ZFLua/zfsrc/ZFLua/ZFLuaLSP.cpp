#include "ZFLuaLSP.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFLuaLSP_supportMultiInherit 1
#define _ZFP_ZFLuaLSP_supportOptionalParam 1
#define _ZFP_ZFLuaLSP_supportTypeWithDot 1

template<typename T_str>
static zfstring _ZFP_ZFLuaLSPGen_luaKeywordsEscape(
        ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        , ZF_IN T_str const &s
        ) {
    if(luaKeywords.find(s) != luaKeywords.end()) {
        zfstring ret;
        ret += s;
        ret += "_";
        return ret;
    }
    else {
        return s;
    }
}

static zfstring _ZFP_ZFLuaLSPGen_typeIdToSig(ZF_IN const ZFClass *cls) {
#if _ZFP_ZFLuaLSP_supportTypeWithDot
    return cls->classNameFull();
#else
    return zfstringReplace(cls->classNameFull(), ".", "_");
#endif
}
static zfstring _ZFP_ZFLuaLSPGen_typeIdToSig(ZF_IN const zfstring &typeId) {
    const ZFTypeInfo *typeInfo = ZFTypeInfoForName(typeId);
    if(typeInfo == zfnull || typeInfo->typeIdClass() == zfnull) {
#if _ZFP_ZFLuaLSP_supportTypeWithDot
        return typeId;
#else
        return zfstringReplace(typeId, ".", "_");
#endif
    }
    else {
        return _ZFP_ZFLuaLSPGen_typeIdToSig(typeInfo->typeIdClass());
    }
}

static zfstring _ZFP_ZFLuaLSPGen_paramSig(ZF_IN const zfstring &typeId) {
    if(typeId == ZFTypeId_ZFCallback()) {
        return zfstr("%s|fun(zfargs:ZFArgs):any", typeId);
    }
    else {
        const ZFClass *cls = ZFClass::classForName(typeId);
        if(cls != zfnull && (zffalse
                    || cls->classIsTypeOf(ZFTypeIdWrapper::ClassData())
                    || cls == ZFObject::ClassData()
                    )) {
            return zfstr("%s|string|number", _ZFP_ZFLuaLSPGen_typeIdToSig(cls));
        }
        else {
            return _ZFP_ZFLuaLSPGen_typeIdToSig(typeId);
        }
    }
}
static zfbool _ZFP_ZFLuaLSPGen_isChainedRet(ZF_IN const ZFMethod *m) {
    return (m->returnTypeId() == ZFTypeId_void()
                && m->ownerClass()
                && m->methodType() != ZFMethodTypeStatic
                );
}
static zfstring _ZFP_ZFLuaLSPGen_retSig(ZF_IN const ZFMethod *m) {
    /* method return type
     *   fun(p0:P0, p1:P1):Ret
     *                     ^ ^
     */
    if(m->methodName() == "objectOnInit") {
        return _ZFP_ZFLuaLSPGen_typeIdToSig(m->ownerClass());
    }
    else {
        if(_ZFP_ZFLuaLSPGen_isChainedRet(m)) {
            // for class member method with void return,
            // change it to ownerClass for chained call
            return _ZFP_ZFLuaLSPGen_typeIdToSig(m->ownerClass());
        }
        else {
            return _ZFP_ZFLuaLSPGen_typeIdToSig(m->returnTypeId());
        }
    }
}
static void _ZFP_ZFLuaLSPGen_method_overloadAnnotation(
        ZF_IN const ZFOutput &output
        , ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        , ZF_IN const ZFMethod *m
        , ZF_IN const zfstring &returnTypeId
        ) {
    /*
        ---@overload fun(p0:P0):Ret
        ---@overload fun(p0:P0, p1:P1):Ret
     */
    zfstring retSig = returnTypeId ? returnTypeId : _ZFP_ZFLuaLSPGen_retSig(m);
    zfindex paramCount = m->paramCountMin();
    do {
        output << "---@overload fun(";
        for(zfindex i = 0; i < paramCount; ++i) {
            if(i != 0) {
                output << ", ";
            }
            output
                << _ZFP_ZFLuaLSPGen_luaKeywordsEscape(luaKeywords, m->paramNameAt(i))
                << ":"
                << _ZFP_ZFLuaLSPGen_paramSig(m->paramTypeIdAt(i))
                ;
        }
        output << ")";
        if(retSig != ZFTypeId_void()) {
            output << ":" << retSig;
        }
        output << "\n";

        if(paramCount == m->paramCount()) {
            break;
        }
        else {
            ++paramCount;
            continue;
        }
    } while(zftrue);
}
static void _ZFP_ZFLuaLSPGen_method(
        ZF_IN const ZFOutput &output
        , ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        , ZF_IN const ZFMethod *m
        , ZF_IN_OPT const ZFClass *ctorCls = zfnull
        ) {
    /* method
        ---@param p0 P0
        ---@return Ret
        function NS.Cls:func(p0) end

        ---@generic T:NS.Cls
        ---@param self T
        ---@param p0 P0
        ---@param p1 P1
        ---@return T
        function NS.Cls:func(p0, p1) end
     */
    zfstring retSig = _ZFP_ZFLuaLSPGen_retSig(m);
    zfbool isChainedRet = _ZFP_ZFLuaLSPGen_isChainedRet(m);
#if _ZFP_ZFLuaLSP_supportOptionalParam
    zfindex paramCount = m->paramCount();
#else
    zfindex paramCount = m->paramCountMin();
#endif
    do {
        if(isChainedRet) {
            output
                << "---@generic T:" << m->ownerClass()->classNameFull() << "\n"
                << "---@param self T\n"
                ;
        }

        for(zfindex i = 0; i < paramCount; ++i) {
            output
                << "---@param " << _ZFP_ZFLuaLSPGen_luaKeywordsEscape(luaKeywords, m->paramNameAt(i))
#if _ZFP_ZFLuaLSP_supportOptionalParam
                << (i >= m->paramCountMin() ? "?" : "")
#endif
                << " "
                << _ZFP_ZFLuaLSPGen_paramSig(m->paramTypeIdAt(i))
                << "\n"
                ;
        }

        if(isChainedRet) {
            output << "---@return T\n";
        }
        else {
            if(retSig != ZFTypeId_void()) {
                output << "---@return " << retSig << "\n";
            }
        }

        output << "function ";
        if(ctorCls) {
            output << retSig;
        }
        else {
            if(m->ownerClass()) {
                output << m->ownerClass()->classNameFull();
                if(m->methodType() == ZFMethodTypeStatic) {
                    output << ".";
                }
                else {
                    output << ":";
                }
            }
            else if(m->methodNamespace()) {
                output << m->methodNamespace() << ".";
            }
            output << m->methodName();
        }
        output << "(";
        for(zfindex i = 0; i < paramCount; ++i) {
            if(i != 0) {
                output << ", ";
            }
            output << _ZFP_ZFLuaLSPGen_luaKeywordsEscape(luaKeywords, m->paramNameAt(i));
        }
        output << ") end\n";

#if _ZFP_ZFLuaLSP_supportOptionalParam
        break;
#else
        if(paramCount == m->paramCount()) {
            break;
        }
        else {
            ++paramCount;
            continue;
        }
#endif
    } while(zftrue);
}

// ============================================================
static void _ZFP_ZFLuaLSPGen_NS(
        ZF_IN const ZFOutput &output
        , ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        , ZF_IN_OUT zfstlmap<zfstring, zfbool> &NSMap
        , ZF_IN const zfchar *NS
        ) {
    if(NSMap.find(NS) != NSMap.end()
            || luaKeywords.find(NS) != luaKeywords.end()
            ) {
        return;
    }
    ZFCoreArray<ZFIndexRange> pos;
    ZFNamespaceSplit(pos, NS);
    for(zfindex i = 0; i < pos.count() - 1; ++i) {
        _ZFP_ZFLuaLSPGen_NS(output, luaKeywords, NSMap, zfstring(NS, 0, pos[i].start + pos[i].count));
    }

    /* NS
        ---@class NS0.NS1
        NS0.NS1 = {}
     */
    output << "---@class " << NS << "\n";
    output << NS << " = {}\n";
    NSMap[NS] = zftrue;
}
static void _ZFP_ZFLuaLSPGen_allNS(
        ZF_IN const ZFOutput &output
        , ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        ) {
    ZFCoreArray<zfstring> allNS = ZFNamespaceGetAll();
    zfstlmap<zfstring, zfbool> NSMap;
    for(zfindex i = 0; i < allNS.count(); ++i) {
        _ZFP_ZFLuaLSPGen_NS(output, luaKeywords, NSMap, allNS[i]);
    }
}

static void _ZFP_ZFLuaLSPGen_class(
        ZF_IN const ZFOutput &output
        , ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        , ZF_IN_OUT zfstlmap<const ZFClass *, zfbool> &clsMap
        , ZF_IN const ZFClass *cls
        ) {
    if(clsMap.find(cls) == clsMap.end()
            || luaKeywords.find(cls->className()) != luaKeywords.end()
            ) {
        return;
    }
    clsMap.erase(cls);
    if(cls->classParent() != zfnull) {
        _ZFP_ZFLuaLSPGen_class(output, luaKeywords, clsMap, cls->classParent());
    }

    zfstring classSig = _ZFP_ZFLuaLSPGen_typeIdToSig(cls);
    ZFCoreArray<zfstring> aliasNames;
    aliasNames.add(classSig);
    for(zfindex i = 0; i < cls->classAliasTo().count(); ++i) {
        aliasNames.add(cls->classAliasTo()[i]);
    }

    /* class and constructor
        ---@class NS.Cls:Base1,Base2
        ---@overload fun():NS.Cls
        ---@overload fun(p0:P0, p1:P1):NS.Cls
        NS.Cls = {}
     */
    for(zfindex i = 0; i < aliasNames.count(); ++i) {
        const zfstring &aliasName = aliasNames[i];

        zfstring classParentSig;
        if(cls->classParent() != zfnull) {
            classParentSig = _ZFP_ZFLuaLSPGen_typeIdToSig(cls->classParent());
        }
#if _ZFP_ZFLuaLSP_supportMultiInherit
        for(zfindex i = 0; i < cls->dynamicInterfaceCount(); ++i) {
            if(classParentSig) {
                classParentSig += ",";
            }
            classParentSig += _ZFP_ZFLuaLSPGen_typeIdToSig(cls->dynamicInterfaceAt(i));
        }
        for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i) {
            if(classParentSig) {
                classParentSig += ",";
            }
            classParentSig += _ZFP_ZFLuaLSPGen_typeIdToSig(cls->implementedInterfaceAt(i));
        }
#endif
        output
            << "---@class " << aliasName << (!classParentSig.isEmpty() ? ":" : "") << classParentSig << "\n"
            << "---@overload fun():" << classSig << "\n"
            ;
        ZFCoreArray<const ZFMethod *> ctorMethods = cls->methodForNameGetAll("objectOnInit");
        for(zfindex iMethod = 0; iMethod < ctorMethods.count(); ++iMethod) {
            const ZFMethod *m = ctorMethods[iMethod];
            if(m->paramCount() == 0) {
                continue;
            }
            _ZFP_ZFLuaLSPGen_method_overloadAnnotation(output, luaKeywords, m, classSig);
        }
        output
            << aliasName << " = {}\n"
            ;
    }

    zfstlmap<const zfchar *, ZFCoreArray<const ZFMethod *>, zfcharConst_zfstlLess> methodMap;
    ZFCoreArray<const ZFMethod *> allMethod = cls->methodGetAll();
    for(zfindex iMethod = 0; iMethod < allMethod.count(); ++iMethod) {
        const ZFMethod *m = allMethod[iMethod];
        if(!m->isPublic() || m->isInternalPrivate()
                || zfstringIsEqual(m->methodName(), "objectOnInit")
                || luaKeywords.find(m->methodName()) != luaKeywords.end()
#if _ZFP_ZFLuaLSP_supportMultiInherit
                || m->ownerClass() != cls
#else
                || (m->ownerClass() != cls
                    && !m->ownerClass()->classIsInterface()
                    && !cls->dynamicImplementOf(m->ownerClass())
                    )
#endif
                ) {
            continue;
        }
        zfstlmap<const zfchar *, ZFCoreArray<const ZFMethod *>, zfcharConst_zfstlLess>::iterator itMethod = methodMap.find(m->methodName());
        if(itMethod != methodMap.end()) {
            zfbool exist = zffalse;
            for(zfindex i = itMethod->second.count() - 1; i != zfindexMax(); --i) {
                const ZFMethod *methodExist = itMethod->second[i];
                if(m->paramTypeIdIsMatch(methodExist)
                        && m->returnTypeId() == methodExist->returnTypeId()
                        ) {
                    exist = zftrue;
                    break;
                }
            }
            if(exist) {
                continue;
            }
        }
        // member methods
        _ZFP_ZFLuaLSPGen_method(output, luaKeywords, m);
        methodMap[m->methodName()].add(m);
    }
}
static void _ZFP_ZFLuaLSPGen_allClass(
        ZF_IN const ZFOutput &output
        , ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        ) {
    ZFCoreArray<const ZFClass *> allClass = ZFClassGetAll();
    zfstlmap<const ZFClass *, zfbool> clsMap;
    for(zfindex i = 0; i < allClass.count(); ++i) {
        const ZFClass *cls = allClass[i];
        if(cls->classIsInternal()) {
            continue;
        }
        clsMap[cls] = zftrue;
    }
    while(!clsMap.empty()) {
        _ZFP_ZFLuaLSPGen_class(output, luaKeywords, clsMap, clsMap.begin()->first);
    }
}

static void _ZFP_ZFLuaLSPGen_allMethod(
        ZF_IN const ZFOutput &output
        , ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        ) {
    ZFCoreArray<const ZFMethod *> allMethod = ZFMethodFuncGetAll();
    for(zfindex iMethod = 0; iMethod < allMethod.count(); ++iMethod) {
        const ZFMethod *m = allMethod[iMethod];
        if(m->isInternalPrivate()
                || luaKeywords.find(m->methodName()) != luaKeywords.end()
                ) {
            continue;
        }
        // global methods
        _ZFP_ZFLuaLSPGen_method(output, luaKeywords, m);
    }
}

static void _ZFP_ZFLuaLSPGen_spec(
        ZF_IN const ZFOutput &output
        , ZF_IN const zfstlmap<zfstring, zfbool> &luaKeywords
        ) {
    output
        << "---@meta ZF"
        << "\n"

        << "zfnull = nil\n"
        << "zftrue = true\n"
        << "zffalse = false\n"

        << "---@return ZFCoreArray\n"
        << "function ZFCoreArrayCreate(...) end\n"

        << "---@param ret zfstring\n"
        << "---@param fmt zfstring|string\n"
        << "function zfstringAppend(ret, fmt, ...) end\n"
        << "---@param fmt zfstring|string\n"
        << "---@return zfstring\n"
        << "function zfstr(fmt, ...) end\n"

        << "---@return zfptr\n"
        << "function zfl_L(...) end\n"
        << "---@return ZFObject\n"
        << "function zfl_call(...) end\n"
        << "function zfl_value(e) end\n"
        << "---@return number\n"
        << "function zfl_cmp(v0, v1) end\n"
        << "---@return boolean\n"
        << "function zfl_eq(v0, v1) end\n"

        << "---@param fmt zfstring|string\n"
        << "---@return ZFCallback\n"
        << "function ZFLog(fmt, ...) end\n"
        << "---@param fmt zfstring|string\n"
        << "---@return ZFCallback\n"
        << "function ZFLogTrim(fmt, ...) end\n"

        << "---@param a ZFCoreArray|ZFContainer|ZFKeyValueContainer\n"
        << "---@return fun(a):any\n"
        << "function zfl_iter(a) end\n"
        << "function zfl_range(n, ...) end\n"

        << "---@param tbl table\n"
        << "---@return zfstring\n"
        << "function zfl_tableInfo(tbl) end\n"
        << "---@param tbl table\n"
        << "function zfl_tableInfoPrint(tbl) end\n"
        ;
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaLSPGen
        , ZFMP_IN(const ZFOutput &, output)
        ) {
    zfstlmap<zfstring, zfbool> luaKeywords;
    luaKeywords["and"] = zftrue;
    luaKeywords["break"] = zftrue;
    luaKeywords["do"] = zftrue;
    luaKeywords["else"] = zftrue;
    luaKeywords["elseif"] = zftrue;
    luaKeywords["end"] = zftrue;
    luaKeywords["false"] = zftrue;
    luaKeywords["for"] = zftrue;
    luaKeywords["function"] = zftrue;
    luaKeywords["goto"] = zftrue;
    luaKeywords["if"] = zftrue;
    luaKeywords["in"] = zftrue;
    luaKeywords["local"] = zftrue;
    luaKeywords["nil"] = zftrue;
    luaKeywords["not"] = zftrue;
    luaKeywords["or"] = zftrue;
    luaKeywords["repeat"] = zftrue;
    luaKeywords["return"] = zftrue;
    luaKeywords["then"] = zftrue;
    luaKeywords["true"] = zftrue;
    luaKeywords["until"] = zftrue;
    luaKeywords["while"] = zftrue;

    _ZFP_ZFLuaLSPGen_spec(output, luaKeywords);
    _ZFP_ZFLuaLSPGen_allNS(output, luaKeywords);
    _ZFP_ZFLuaLSPGen_allClass(output, luaKeywords);
    _ZFP_ZFLuaLSPGen_allMethod(output, luaKeywords);
}

ZF_NAMESPACE_GLOBAL_END

