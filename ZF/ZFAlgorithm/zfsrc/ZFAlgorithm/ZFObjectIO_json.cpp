#include "ZFObjectIO_json.h"
#include "ZFJsonSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(json, ZFLevelZFFrameworkPostNormal, {
        return fileExt == "json";
    }, {
        return ZFObjectFromJsonT(ret, input, outErrorHint);
    }, {
        return ZFObjectToJson(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END

