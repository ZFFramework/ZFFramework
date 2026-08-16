#include "ZFObjectIO_zfsd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(zfsd, ZFLevelZFFrameworkPostNormal, {
        return fileExt == "zfsd";
    }, {
        return ZFObjectFromZFSDT(ret, input, errorHint);
    }, {
        return ZFObjectToZFSD(output, obj, errorHint);
    })

ZF_NAMESPACE_GLOBAL_END

