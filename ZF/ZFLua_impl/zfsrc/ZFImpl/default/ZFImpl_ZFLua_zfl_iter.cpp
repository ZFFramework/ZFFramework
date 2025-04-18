#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_iter, ZFM_EXPAND({
        ZFImpl_ZFLua_execute(L,
                "function zfl_iter(a)\n"
                "    if a == nil or type(a) ~= 'userdata' then\n"
                "        return a\n"
                "    elseif a:classData():classIsTypeOf(ZFKeyValueContainer.ClassData()) then\n"
                "        local i = -1\n"
                "        local it = a:iter()\n"
                "        return function(a)\n"
                "            i = i + 1\n"
                "            if it:valid() then\n"
                "                local k = a:iterKey(it)\n"
                "                local v = a:iterValue(it)\n"
                "                it:next()\n"
                "                return i, k, v\n"
                "            else\n"
                "                return nil\n"
                "            end\n"
                "        end, a\n"
                "    elseif a:classData():classIsTypeOf(ZFContainer.ClassData()) or a:classData():classIsTypeOf(ZFCoreArray.ClassData()) then\n"
                "        local i = -1\n"
                "        return function(a)\n"
                "            i = i + 1\n"
                "            if i < a:count() then\n"
                "                return i, a:get(i)\n"
                "            else\n"
                "                return nil\n"
                "            end\n"
                "        end, a\n"
                "    end\n"
                "end\n"
                );
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

