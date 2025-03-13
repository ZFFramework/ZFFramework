#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_tableInfo, ZFM_EXPAND({
    ZFImpl_ZFLua_execute(L,
            "function zfl_tableInfoT(p, t, indent)\n"
            "    indent = indent or 1\n"
            "    for k, v in pairs(t) do\n"
            "        local prefix = ''\n"
            "        for i = 1, indent do\n"
            "            prefix = prefix .. '    '\n"
            "        end\n"
            "        if type(v) == 'table' then\n"
            "            p(prefix .. k .. ':')\n"
            "            zfl_tableInfoT(p, v, indent + 1)\n"
            "        else\n"
            "            p(prefix .. k .. ': ' .. string.gsub(tostring(v), '\\n', '\\\\n'))\n"
            "        end\n"
            "    end\n"
            "end\n"
            "function zfl_tableInfo(t, indent)\n"
            "    local ret = ''\n"
            "    zfl_tableInfoT(function(s)\n"
            "        ret = ret .. s .. '\\n'\n"
            "    end, t, indent)\n"
            "    return ret\n"
            "end\n"
            "function zfl_tableInfoPrint(t, indent)\n"
            "    print(zfl_tableInfo(t, indent))\n"
            "end\n"
            );
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

