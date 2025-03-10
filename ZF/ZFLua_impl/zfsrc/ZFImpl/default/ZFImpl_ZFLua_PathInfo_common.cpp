#include "ZFImpl_ZFLua_PathInfo.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLocalPathInfo,
        "function (...)"
        "    local arg={...};"
        "    if #arg > 0 then return ZFPathInfoForLocal(zfl_l or zfnull, arg[1]) "
        "    else return zfl_l or zfnull end "
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFInputForLocal,
        "function (l, ...)"
        "    local arg={...};"
        "    return ZF.ZFInputForLocal(l, arg[1] or zfl_l or zfnull);"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFOutputForLocal,
        "function (l, ...)"
        "    local arg={...};"
        "    return ZF.ZFOutputForLocal(l, arg[1] or zfl_l or zfnull);"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(zfimport,
        "function (l, ...)"
        "    local arg={...};"
        "    return ZF.zfimport(l, arg[1] or zfl_l or zfnull);"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaExecute,
        "function (p, ...)"
        "    return ZF.ZFLuaExecuteDetail(p, ZFCoreArrayCreate(...), zfl_L());"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaExecuteDetail,
        "function (...)"
        "    local arg={...};"
        "    return ZF.ZFLuaExecuteDetail(arg[1] or ZFCallback(), arg[2] or ZFCoreArray(), arg[3] or zfl_L());"
        "end"
    )
ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLuaGC,
        "function (...)"
        "    local arg={...};"
        "    ZF.ZFLuaGC(arg[1] or zfl_L());"
        "end"
    )

ZF_NAMESPACE_GLOBAL_END

