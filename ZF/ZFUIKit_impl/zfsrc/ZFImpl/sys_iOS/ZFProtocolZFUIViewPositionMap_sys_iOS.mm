#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionMap.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionMapImpl_sys_iOS, ZFUIViewPositionMap, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewPositionMap(
            ZF_OUT ZFUIPoint &ret
            , ZF_IN ZFUIView *view
            , ZF_IN_OPT const ZFUIPoint &localPos = ZFUIPointZero()
            ) {
        UIView *nativeView = (__bridge UIView *)view->nativeView();

        CGPoint nativePos = CGPointZero;
        if(nativeView.window == nil) {
            nativePos.x = nativeView.frame.origin.x + localPos.x;
            nativePos.y = nativeView.frame.origin.y + localPos.y;
        }
        else {
            nativePos = [nativeView convertPoint:ZFImpl_sys_iOS_ZFUIPointToCGPoint(localPos) toView:nil];
        }
        ZFImpl_sys_iOS_ZFUIPointFromCGPointT(ret, nativePos);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionMapImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

