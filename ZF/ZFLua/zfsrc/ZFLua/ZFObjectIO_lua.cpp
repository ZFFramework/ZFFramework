#include "ZFObjectIO_lua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(lua, {
        zfstring fileExt;
        if(!ZFPathInfoToFileName(fileExt, pathInfo)
                || !ZFFileExtOfT(fileExt, fileExt)
                || !fileExt
                ) {
            return zffalse;
        }
        zfstringToLowerT(fileExt);
        return fileExt == "lua";
    }, {
        ret = ZFLuaExecute(input);
        return input;
    }, {
        zfstringAppend(outErrorHint, "not supported");
        return zffalse;
    })

ZF_NAMESPACE_GLOBAL_END

