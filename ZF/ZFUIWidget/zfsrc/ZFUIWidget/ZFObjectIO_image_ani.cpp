#include "ZFObjectIO_image_ani.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(image_ani, ZFLevelZFFrameworkPostLow, ZFM_EXPAND({
        if(!ZFObjectIO_image_imageExtCheck(fileExt)) {
            return zffalse;
        }
        ZFUISize frameSize = ZFUISizeZero();
        zfindex frameCount = 0;
        zftimet frameDuration = 0;
        return ZFUIImageAniLoadCheck(frameSize, frameCount, frameDuration, fileName);
    }), {
        ret = ZFUIImageAniLoad(input);
        if(ret == zfnull) {
            zfstringAppend(outErrorHint
                , "unable to load image ani from %s"
                , input.pathInfo()
                );
            return zffalse;
        }
        else {
            return zftrue;
        }
    }, {
        zfstringAppend(outErrorHint
            , "image_ani does not support write to file"
            );
        return zffalse;
    })

ZF_NAMESPACE_GLOBAL_END

