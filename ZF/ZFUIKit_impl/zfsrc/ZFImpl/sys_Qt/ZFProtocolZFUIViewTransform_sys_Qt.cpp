#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTransform.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>
#include <QTransform>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewTransformImpl_sys_Qt, ZFUIViewTransform, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual ZFUITransformFlags viewTransformAvailable(void) {
        return ZFUITransform::e_Transform2D;
    }

    virtual void viewTransform(ZF_IN ZFUIView *view) {
        QGraphicsWidget *nativeView = (QGraphicsWidget *)view->nativeView();
        QTransform t;
        t.translate(nativeView->geometry().width() / 2, nativeView->geometry().height() / 2);
        if(view->viewTranslateX() != 0 || view->viewTranslateY() != 0) {
            t.translate(
                    view->viewTranslateX() * view->UIScaleFixed(),
                    view->viewTranslateY() * view->UIScaleFixed()
                );
        }
        if(view->viewRotateZ() != 0) {
            t.rotate(view->viewRotateZ());
        }
        if(view->viewScaleX() != 1 || view->viewScaleY() != 1) {
            t.scale(view->viewScaleX(), view->viewScaleY());
        }
        t.translate(-nativeView->geometry().width() / 2, -nativeView->geometry().height() / 2);
        nativeView->setTransform(t);
    }
    virtual void viewTransformReset(ZF_IN ZFUIView *view) {
        QGraphicsWidget *nativeView = (QGraphicsWidget *)view->nativeView();
        nativeView->setTransform(QTransform());
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewTransformImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

