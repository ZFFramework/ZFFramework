#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTransform.h"

#if ZF_ENV_sys_iOS

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewTransformImpl_sys_iOS, ZFUIViewTransform, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual ZFUITransformFlags transformAvailable(void) {
        return v_ZFUITransform::e_Transform2D;
    }

    virtual void viewTransform(ZF_IN ZFUIView *view) {
        UIView *nativeView = (__bridge UIView *)view->nativeView();
        CATransform3D t = CATransform3DIdentity;
        if(view->translateX() != 0 || view->translateY() != 0) {
            t = CATransform3DTranslate(t
                    , view->translateX() * view->UIScaleFixed()
                    , view->translateY() * view->UIScaleFixed()
                    , 0
                );
        }
        if(view->rotateZ() != 0) {
            t = CATransform3DRotate(t, ((int)view->rotateZ()) * M_PI / 180, 0, 0, 1);
        }
        if(view->scaleX() != 1 || view->scaleY() != 1) {
            t = CATransform3DScale(t, view->scaleX(), view->scaleY(), 1);
        }
        nativeView.layer.transform = t;
    }
    virtual void viewTransformReset(ZF_IN ZFUIView *view) {
        UIView *nativeView = (__bridge UIView *)view->nativeView();
        nativeView.layer.transform = CATransform3DIdentity;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewTransformImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

