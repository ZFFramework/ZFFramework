#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_range, ZFM_EXPAND({
        ZFImpl_ZFLua_execute(L,
                "function zfl_range(n, ...)\n"
                "    local args = {...}\n"
                "    local i\n"
                "    local iEnd\n"
                "    local offset\n"
                "    if #args >= 1 then\n"
                "        i = n\n"
                "        iEnd = args[1]\n"
                "        if zfl_eq(iEnd, zfindexMax()) then\n"
                "            iEnd = -1\n"
                "        end\n"
                "        if #args >= 2 then\n"
                "            offset = args[2]\n"
                "        else\n"
                "            offset = ((iEnd >= i) and 1 or -1)\n"
                "        end\n"
                "    else\n"
                "        i = n - n\n"
                "        iEnd = n\n"
                "        offset = ((iEnd >= i) and 1 or -1)\n"
                "    end\n"
                "    return function()\n"
                "        if (offset >= 0 and i < iEnd) or (offset < 0 and i > iEnd) then\n"
                "            local ret = i\n"
                "            i = i + offset\n"
                "            return ret\n"
                "        end\n"
                "    end\n"
                "end\n"
                );
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

