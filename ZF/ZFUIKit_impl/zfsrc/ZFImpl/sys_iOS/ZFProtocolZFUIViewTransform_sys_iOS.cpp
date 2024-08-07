#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTransform.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_iOS

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewTransformImpl_sys_iOS, ZFUIViewTransform, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual ZFUITransformFlags viewTransformAvailable(void) {
        return ZFUITransform::e_Transform2D;
    }

    virtual void viewTransform(ZF_IN ZFUIView *view) {
        // transform won't work when changed immediately after UIView created,
        // delay for some time
        _delayTaskMap[view] = zftrue;
        zfself *thiz = this;
        zfautoT<ZFObjectHolder> holder = view->objectHolder();
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            ZFUIView *viewTmp = holder->objectHolded();
            if(viewTmp == zfnull || thiz->_delayTaskMap.find(viewTmp) == thiz->_delayTaskMap.end()) {
                return;
            }
            UIView *nativeView = (__bridge UIView *)viewTmp->nativeView();
            CATransform3D t = CATransform3DIdentity;
            if(viewTmp->viewTranslateX() != 0 || viewTmp->viewTranslateY() != 0) {
                t = CATransform3DTranslate(t
                        , viewTmp->viewTranslateX() * viewTmp->UIScaleFixed()
                        , viewTmp->viewTranslateY() * viewTmp->UIScaleFixed()
                        , 0
                    );
            }
            if(viewTmp->viewRotateZ() != 0) {
                t = CATransform3DRotate(t, ((int)viewTmp->viewRotateZ()) * M_PI / 180, 0, 0, 1);
            }
            if(viewTmp->viewScaleX() != 1 || viewTmp->viewScaleY() != 1) {
                t = CATransform3DScale(t, viewTmp->viewScaleX(), viewTmp->viewScaleY(), 1);
            }
            nativeView.layer.transform = t;
        });
    }
    virtual void viewTransformReset(ZF_IN ZFUIView *view) {
        UIView *nativeView = (__bridge UIView *)view->nativeView();
        nativeView.layer.transform = CATransform3DIdentity;
        _delayTaskMap.erase(view);
    }
public:
    zfstlmap<ZFUIView *, zfbool> _delayTaskMap;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewTransformImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

