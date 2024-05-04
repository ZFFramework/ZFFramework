#include "ZFUIViewCapture.h"
#include "protocol/ZFProtocolZFUIViewCapture.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFUIImage>, ZFUIViewCapture
        , ZFMP_IN(ZFUIView *, view)
        ) {
    if(view == zfnull) {
        return zfnull;
    }
    zfautoT<ZFUIImage> ret = ZFUIImage::ClassData()->newInstance();
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewCapture) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewCapture);
    if(impl != zfnull && impl->viewCapture(view, ret)) {
        return ret;
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

