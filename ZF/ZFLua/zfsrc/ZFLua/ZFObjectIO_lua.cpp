#include "ZFObjectIO_lua.h"
#include "ZFPathType_lua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(lua, {
        return (zfstringIsEqual(pathInfo.pathType.cString(), ZFPathType_lua())
            || ZFObjectIOImplCheck(pathInfo, "lua"));
    }, {
        ret = ZFLuaExecute(input);
        return input;
    }, {
        zfstringAppend(outErrorHint, "not supported");
        return zffalse;
    })

ZF_NAMESPACE_GLOBAL_END

