#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTransform.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>
#include <QTransform>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewTransformImpl_sys_Qt, ZFUIViewTransform, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual ZFUITransformFlags transformAvailable(void) {
        return v_ZFUITransform::e_Transform2D;
    }

    virtual void viewTransform(ZF_IN ZFUIView *view) {
        QGraphicsWidget *nativeView = (QGraphicsWidget *)view->nativeView();
        QTransform t;
        t.translate(nativeView->geometry().width() / 2, nativeView->geometry().height() / 2);
        if(view->translateX() != 0 || view->translateY() != 0) {
            t.translate(
                    view->translateX() * view->UIScaleFixed(),
                    view->translateY() * view->UIScaleFixed()
                );
        }
        if(view->rotateZ() != 0) {
            t.rotate(view->rotateZ());
        }
        if(view->scaleX() != 1 || view->scaleY() != 1) {
            t.scale(view->scaleX(), view->scaleY());
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

