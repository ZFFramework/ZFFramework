#include "ZFObjectIO_lua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(lua, {
        return ZFObjectIOImplCheck(pathInfo, "lua");
    }, {
        ret = ZFLuaExecute(input);
        return input;
    }, {
        zfstringAppend(outErrorHint, "not supported");
        return zffalse;
    })

ZF_NAMESPACE_GLOBAL_END

