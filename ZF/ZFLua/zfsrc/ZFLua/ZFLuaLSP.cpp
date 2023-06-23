#include "ZFLuaLSP.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFLuaLSPGenFile_prefix(ZF_IN const ZFOutput &output)
{
    output <<
        "local _class = {}\n"
        "function _ZFP_ZFLuaLSP_Class(base)\n"
        "    local class_type = {}\n"
        "    class_type.__type = 'class'\n"
        "    class_type._ZFP_ZFLuaLSP_ctor = false\n"
        "    local vtbl = {}\n"
        "    _class[class_type] = vtbl\n"
        "    setmetatable(class_type,{__newindex = vtbl, __index = vtbl})\n"
        "    if base then\n"
        "        setmetatable(vtbl, {\n"
        "                __index = function(t,k)\n"
        "                    local ret = _class[base][k]\n"
        "                    vtbl[k] = ret\n"
        "                    return ret\n"
        "                end,\n"
        "            })\n"
        "    end\n"
        "    class_type.__base = base\n"
        "    class_type._ZFP_ZFLuaLSP_new = function(...)\n"
        "        local obj = {}\n"
        "        obj.__base = class_type\n"
        "        obj.__type = 'object'\n"
        "        do\n"
        "            local create\n"
        "            create = function(c, ...)\n"
        "                if c.__base then\n"
        "                    create(c.__base, ...)\n"
        "                end\n"
        "                if c._ZFP_ZFLuaLSP_ctor then\n"
        "                    c._ZFP_ZFLuaLSP_ctor(obj, ...)\n"
        "                end\n"
        "            end\n"
        "            create(class_type,...)\n"
        "        end\n"
        "        setmetatable(obj,{ __index = _class[class_type] })\n"
        "        return obj\n"
        "    end\n"
        "    class_type.super = function(self, f, ...)\n"
        "        assert(self and self.__type == 'object', string.format('self must be a object when call super(self, %s, ...)', tostring(f)))\n"
        "        local originBase = self.__base\n"
        "        local s = originBase\n"
        "        local base = s.__base\n"
        "        while base and s[f] == base[f] do\n"
        "            s = base\n"
        "            base = base.__base\n"
        "        end\n"
        "        assert(base and base[f], string.format('base class or function cannot be found when call .super(self, %s, ...)', tostring(f)))\n"
        "        while base.__base and base[f] == base.__base[f] do\n"
        "            base = base.__base\n"
        "        end\n"
        "        if base.__base then\n"
        "            self.__base = base\n"
        "        end\n"
        "        local result = base[f](self, ...)\n"
        "        if base.__base then\n"
        "            self.__base = originBase\n"
        "        end\n"
        "        return result\n"
        "    end\n"
        "    return class_type\n"
        "end\n"
        ;
}

static zfstring _ZFP_ZFLuaLSPGenFile_typeIdToSig(ZF_IN const ZFClass *cls)
{
    zfstring ret;
    ret += cls->classNameFull();
    zfstringReplace(ret, ".", "_");
    return ret;
}
static zfstring _ZFP_ZFLuaLSPGenFile_typeIdToSig(ZF_IN const zfchar *typeId)
{
    const ZFClass *cls = ZFClass::classForName(typeId);
    if(cls != zfnull)
    {
        return _ZFP_ZFLuaLSPGenFile_typeIdToSig(cls);
    }
    const ZFTypeInfo *typeInfo = ZFTypeInfoForName(typeId);
    if(typeInfo == zfnull || typeInfo->typeIdClass() == zfnull)
    {
        zfstring ret;
        ret += "_";
        ret += typeId;
        zfstringReplace(ret, ".", "_");
        return ret;
    }
    return _ZFP_ZFLuaLSPGenFile_typeIdToSig(typeInfo->typeIdClass());
}

static void _ZFP_ZFLuaLSPGenFile_param_ZFListener(ZF_IN const ZFOutput &output,
                                                  ZF_IN const zfchar *paramTypeId,
                                                  ZF_IN const zfchar *paramName)
{
    zfstring sig = _ZFP_ZFLuaLSPGenFile_typeIdToSig(paramTypeId);
    if(zfstringIsEqual(paramTypeId, ZFTypeId_ZFListener()))
    {
        // ---@param P0 v_ZFListener|fun(zfargs:v_ZFArgs)
        output << "---@param " << paramName << " " << sig
            << "|fun(zfargs:" << _ZFP_ZFLuaLSPGenFile_typeIdToSig(ZFTypeId_ZFArgs()) << ")"
            << "\n";
    }
    else
    {
        // ---@param P0 P0
        output << "---@param " << paramName << " " << sig << "\n";
    }
}

static void _ZFP_ZFLuaLSPGenFile_NS(ZF_IN const ZFOutput &output,
                                    ZF_IN_OUT zfstlmap<zfstring, zfbool> &NSMap,
                                    ZF_IN const zfchar *NS)
{
    if(NSMap.find(NS) != NSMap.end())
    {
        return;
    }
    ZFCoreArrayPOD<ZFIndexRange> pos;
    ZFNamespaceSplit(pos, NS);
    for(zfindex i = 0; i < pos.count() - 1; ++i)
    {
        _ZFP_ZFLuaLSPGenFile_NS(output, NSMap, zfstring(NS, 0, pos[i].start + pos[i].count));
    }
    /* NS
        NS0.NS1 = {}
     */
    output << NS << " = {}\n";
    NSMap[NS] = zftrue;
}
static void _ZFP_ZFLuaLSPGenFile_allNS(ZF_IN const ZFOutput &output)
{
    ZFCoreArrayPOD<const zfchar *> allNS = ZFNamespaceGetAll();
    zfstlmap<zfstring, zfbool> NSMap;
    for(zfindex i = 0; i < allNS.count(); ++i)
    {
        _ZFP_ZFLuaLSPGenFile_NS(output, NSMap, allNS[i]);
    }
}

static void _ZFP_ZFLuaLSPGenFile_class(ZF_IN const ZFOutput &output,
                                       ZF_IN_OUT zfstlmap<const ZFClass *, zfbool> &clsMap,
                                       ZF_IN const ZFClass *cls)
{
    if(clsMap.find(cls) == clsMap.end())
    {
        return;
    }
    clsMap.erase(cls);
    if(cls != zfnull)
    {
        _ZFP_ZFLuaLSPGenFile_class(output, clsMap, cls->classParent());
    }

    zfstring classSig = _ZFP_ZFLuaLSPGenFile_typeIdToSig(cls);
    zfstring classNameFull;
    if(zfsncmp(cls->className(), ZFTypeIdWrapperPrefixName, ZFTypeIdWrapperPrefixNameLen) == 0)
    {
        if(cls->classNamespace() != zfnull)
        {
            classNameFull += cls->classNamespace();
            classNameFull += ".";
        }
        classNameFull += cls->className() + ZFTypeIdWrapperPrefixNameLen;
    }
    else
    {
        classNameFull += cls->classNameFull();
    }

    /* class
        NS.v_Cls = {}
        ---@class NS_Cls : Base
        NS_Cls = _ZFP_ZFLuaLSP_Class(Base)
        ---@return NS_Cls
        function NS.v_Cls() end
     */
    zfstring classParentSig;
    if(cls->classParent() != zfnull)
    {
        classParentSig = _ZFP_ZFLuaLSPGenFile_typeIdToSig(cls->classParent());
    }
    output <<
        classNameFull << " = {}\n"
        << "---@class " << classSig << (!classParentSig.isEmpty() ? ": " : "") << classParentSig << "\n"
        << classSig << " = _ZFP_ZFLuaLSP_Class(" << classParentSig << ")\n"
        << "---@return " << classSig << "\n"
        << "function " << classNameFull << "() end\n"
        ;

    /* constructor
        ---@param P0 P0
        ---@return NS_Cls
        function NS.v_Cls(P0) end
     */
    ZFCoreArrayPOD<const ZFMethod *> ctorMethods = cls->methodForNameGetAll("objectOnInit");
    for(zfindex iMethod = 0; iMethod < ctorMethods.count(); ++iMethod)
    {
        const ZFMethod *m = ctorMethods[iMethod];
        if(m->methodParamCount() == 0
            || (m->methodOwnerClass() != cls && !m->methodOwnerClass()->classIsInterface())
        ) {
            continue;
        }
        zfstring paramList;
        for(zfindex i = 0; i < m->methodParamCount(); ++i)
        {
            _ZFP_ZFLuaLSPGenFile_param_ZFListener(output, m->methodParamTypeIdAt(i), m->methodParamNameAt(i));
            if(!paramList.isEmpty())
            {
                paramList += ", ";
            }
            paramList += m->methodParamNameAt(i);
        }
        output
            << "---@return " << classSig << "\n"
            << "function " << classNameFull << "(" << paramList << ") end\n"
            ;
    }

    ZFCoreArrayPOD<const ZFMethod *> allMethod = cls->methodGetAll();
    for(zfindex iMethod = 0; iMethod < allMethod.count(); ++iMethod)
    {
        const ZFMethod *m = allMethod[iMethod];
        if(!m->methodIsPublic() || m->methodIsInternal()
            || (m->methodOwnerClass() != cls && !m->methodOwnerClass()->classIsInterface())
            || zfstringIsEqual(m->methodName(), "objectOnInit")
        ) {
            continue;
        }
        /* member methods
            ---@param P0 P0
            ---@return Ret
            function NS.v_Cls.Func(P0)
            function NS_Cls:Func(P0)
            end
         */
        zfstring paramList;
        for(zfindex i = 0; i < m->methodParamCount(); ++i)
        {
            _ZFP_ZFLuaLSPGenFile_param_ZFListener(output, m->methodParamTypeIdAt(i), m->methodParamNameAt(i));
            if(!paramList.isEmpty())
            {
                paramList += ", ";
            }
            paramList += m->methodParamNameAt(i);
        }
        if(!zfstringIsEqual(m->methodReturnTypeId(), ZFTypeId_void()))
        {
            zfstring retSig = _ZFP_ZFLuaLSPGenFile_typeIdToSig(m->methodReturnTypeId());
            output << "---@return " << retSig << "\n";
        }
        else
        {
            if(m->methodType() != ZFMethodTypeStatic)
            {
                zfstring retSig = _ZFP_ZFLuaLSPGenFile_typeIdToSig(m->methodOwnerClass());
                output << "---@return " << retSig << "\n";
            }
        }
        zfstring funcPrefix;
        if(m->methodType() == ZFMethodTypeStatic)
        {
            funcPrefix += classNameFull;
            funcPrefix += ".";
        }
        else
        {
            funcPrefix += classSig;
            funcPrefix += ":";
        }
        output
            << "function " << funcPrefix << m->methodName() << "(" << paramList << ") end\n"
            ;
    }
}
static void _ZFP_ZFLuaLSPGenFile_allClass(ZF_IN const ZFOutput &output)
{
    ZFCoreArrayPOD<const ZFClass *> allClass = ZFClassGetAll();
    zfstlmap<const ZFClass *, zfbool> clsMap;
    for(zfindex i = 0; i < allClass.count(); ++i)
    {
        const ZFClass *cls = allClass[i];
        if(cls->classIsInternal())
        {
            continue;
        }
        clsMap[cls] = zftrue;
    }
    while(!clsMap.empty())
    {
        _ZFP_ZFLuaLSPGenFile_class(output, clsMap, clsMap.begin()->first);
    }
}

static void _ZFP_ZFLuaLSPGenFile_allMethod(ZF_IN const ZFOutput &output)
{
    ZFCoreArrayPOD<const ZFMethod *> allMethod = ZFMethodFuncGetAll();
    for(zfindex iMethod = 0; iMethod < allMethod.count(); ++iMethod)
    {
        const ZFMethod *m = allMethod[iMethod];
        if(m->methodIsInternal())
        {
            continue;
        }
        /* global methods
            ---@param P0 P0
            ---@return Ret
            function NS.Func(P0) end
         */
        zfstring paramList;
        for(zfindex i = 0; i < m->methodParamCount(); ++i)
        {
            _ZFP_ZFLuaLSPGenFile_param_ZFListener(output, m->methodParamTypeIdAt(i), m->methodParamNameAt(i));
            if(!paramList.isEmpty())
            {
                paramList += ", ";
            }
            paramList += m->methodParamNameAt(i);
        }
        if(!zfstringIsEqual(m->methodReturnTypeId(), ZFTypeId_void()))
        {
            zfstring retSig = _ZFP_ZFLuaLSPGenFile_typeIdToSig(m->methodReturnTypeId());
            output << "---@return " << retSig << "\n";
        }
        zfstring funcPrefix;
        if(m->methodNamespace() != zfnull)
        {
            funcPrefix += m->methodNamespace();
            funcPrefix += ".";
        }
        output
            << "function " << funcPrefix << m->methodName() << "(" << paramList << ") end\n"
            ;
    }
}

static void _ZFP_ZFLuaLSPGenFile_spec(ZF_IN const ZFOutput &output)
{
    output
        << "---@return v_ZFCoreArray\n"
        << "function ZFCoreArrayCreate(...) end\n"

        << "---@param v_zfstring v_zfstring\n"
        << "---@param v_zfstring v_zfstring\n"
        << "function zfstringAppend(ret, fmt, ...) end\n"
        << "---@param v_zfstring v_zfstring\n"
        << "---@param v_zfstring v_zfstring\n"
        << "---@return v_zfstring\n"
        << "function zfstringWithFormat(fmt, ...) end\n"

        << "---@return v_ZFPtr\n"
        << "function zfl_L(...) end\n"

        << "---@return v_ZFPathInfo\n"
        << "function ZFLocalPathInfo() end\n"

        << "---@param v_zfstring\n"
        << "function zfLog(fmt, ...) end\n"
        << "---@param v_zfstring\n"
        << "function zfLogTrim(fmt, ...) end\n"

        << "---@return v_ZFOutput\n"
        << "function zfLog() end\n"
        << "---@return v_ZFOutput\n"
        << "function zfLogTrim() end\n"

        << "---@return v_zfstring\n"
        << "function zfl_tableInfo(tbl) end\n"
        << "function zfl_tableInfoPrint(tbl) end\n"
        ;
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaLSPGenFile,
                       ZFMP_IN(const ZFOutput &, output))
{
    _ZFP_ZFLuaLSPGenFile_prefix(output);
    _ZFP_ZFLuaLSPGenFile_allNS(output);
    _ZFP_ZFLuaLSPGenFile_allClass(output);
    _ZFP_ZFLuaLSPGenFile_allMethod(output);
    _ZFP_ZFLuaLSPGenFile_spec(output);
}

ZF_NAMESPACE_GLOBAL_END

