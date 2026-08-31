#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionMap.h"

#if ZF_ENV_sys_SDL
#include <cmath> // for sinf/cosf

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionMapImpl_sys_SDL, ZFUIViewPositionMap, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewPositionMap(
            ZF_OUT ZFUIPoint &ret
            , ZF_IN ZFUIView *view
            , ZF_IN_OPT const ZFUIPoint &localPos = ZFUIPointZero()
            ) {
        ZFImpl_sys_SDL_View *v = (ZFImpl_sys_SDL_View *)view->nativeView();
        ret.x = localPos.x;
        ret.y = localPos.y;
        static const zffloat pi = 3.14159265359f;
        while(v != NULL) {
            if(v->viewTransform) {
                zffloat sx = (ret.x - v->rect.w / 2) * v->viewTransform->scaleX;
                zffloat sy = (ret.y - v->rect.h / 2) * v->viewTransform->scaleY;
                zffloat r = v->viewTransform->rotateZ * pi / 180;
                zffloat cosR = cosf(r);
                zffloat sinR = sinf(r);
                ret.x = (sx * cosR + sy * sinR) + (v->rect.x + v->rect.w / 2);
                ret.y = (-sx * sinR + sy * cosR) + (v->rect.y + v->rect.h / 2);
            }
            else {
                ret.x += v->rect.x;
                ret.y += v->rect.y;
            }
            v = v->parent;
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionMapImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

