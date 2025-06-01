#include "ZFObjectIO_image.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef zfstlhashmap<zfstring, zfbool> _ZFP_ZFObjectIO_image_imageExtMapType;
static _ZFP_ZFObjectIO_image_imageExtMapType &_ZFP_ZFObjectIO_image_imageExtMap(void) {
    static _ZFP_ZFObjectIO_image_imageExtMapType m;
    return m;
}

void ZFObjectIO_image_imageExtAdd(ZF_IN const zfstring &imageExt) {
    ZFCoreAssert(imageExt);
    _ZFP_ZFObjectIO_image_imageExtMap()[imageExt] = zftrue;
}
void ZFObjectIO_image_imageExtRemove(ZF_IN const zfstring &imageExt) {
    _ZFP_ZFObjectIO_image_imageExtMap().erase(imageExt);
}
void ZFObjectIO_image_imageExtGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) {
    _ZFP_ZFObjectIO_image_imageExtMapType &m = _ZFP_ZFObjectIO_image_imageExtMap();
    for(_ZFP_ZFObjectIO_image_imageExtMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectIO_image_imageExtDefault, ZFLevelZFFrameworkStatic) {
    ZFObjectIO_image_imageExtAdd("png");
    ZFObjectIO_image_imageExtAdd("jpg");
    ZFObjectIO_image_imageExtAdd("jpeg");
}
ZF_GLOBAL_INITIALIZER_END(ZFObjectIO_image_imageExtDefault)

// ============================================================
ZFOBJECTIO_DEFINE(image, ZFLevelZFFrameworkPostNormal, ZFM_EXPAND({
        _ZFP_ZFObjectIO_image_imageExtMapType &m = _ZFP_ZFObjectIO_image_imageExtMap();
        return m.find(fileExt) != m.end();
    }), {
        ret = ZFUIImageFromInput(input);
        if(ret == zfnull) {
            zfstringAppend(outErrorHint,
                "unable to load image from %s",
                input.pathInfo());
            return zffalse;
        }
        else {
            return zftrue;
        }
    }, {
        ZFUIImage *image = obj;
        if(image == zfnull) {
            zfstringAppend(outErrorHint,
                "object %s is not type of %s",
                ZFObjectInfoOfInstance(obj),
                ZFUIImage::ClassData()->classNameFull());
            return zffalse;
        }
        if(!ZFUIImageToOutput(output, image)) {
            zfstringAppend(outErrorHint,
                "unable to convert image %s to image file",
                image);
            return zffalse;
        }
        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

