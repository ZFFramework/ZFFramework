#include "ZFObjectIO_json.h"
#include "ZFJsonSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(json, {
        zfstring fileExt;
        if(!ZFPathInfoToFileName(fileExt, pathInfo)
                || !ZFFileExtOfT(fileExt, fileExt)
                || !fileExt
                ) {
            return zffalse;
        }
        zfstringToLowerT(fileExt);
        return fileExt == "json";
    }, {
        return ZFObjectFromJson(ret, input, outErrorHint);
    }, {
        return ZFObjectToJson(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END

