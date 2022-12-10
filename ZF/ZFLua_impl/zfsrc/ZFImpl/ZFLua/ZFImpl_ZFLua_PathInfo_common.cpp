#include "ZFImpl_ZFLua_PathInfo.h"

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLocalPathInfo,
        "function ()"
        "    return zfl_l or zfnull;"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(zfimport,
        "function (l, ...)"
        "    local arg={...};"
        "    return ZF.zfimport(l, arg[1] or zfl_l or zfnull);"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaExecute,
        "function (...)"
        "    local arg={...};"
        "    return ZF.ZFLuaExecute(arg[1] or ZFCallback(), arg[2] or ZFCoreArray(), arg[3] or zfl_L());"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaGC,
        "function (...)"
        "    local arg={...};"
        "    ZF.ZFLuaGC(arg[1] or zfl_L());"
        "end"
    )

ZF_NAMESPACE_GLOBAL_END

