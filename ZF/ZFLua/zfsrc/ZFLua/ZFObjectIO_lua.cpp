#include "ZFObjectIO_lua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(lua, ZFLevelZFFrameworkPostNormal, {
        return fileExt == "lua";
    }, {
        ret = ZFLuaExecute(input);
        return input;
    }, {
        zfstringAppend(errorHint, "not supported");
        return zffalse;
    })

ZF_NAMESPACE_GLOBAL_END

