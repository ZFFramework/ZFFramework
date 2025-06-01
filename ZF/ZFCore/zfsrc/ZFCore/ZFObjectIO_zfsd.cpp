#include "ZFObjectIO_zfsd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(zfsd, ZFLevelZFFrameworkPostNormal, {
        return fileExt == "zfsd";
    }, {
        return ZFObjectFromZFSD(ret, input, outErrorHint);
    }, {
        return ZFObjectToZFSD(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END

