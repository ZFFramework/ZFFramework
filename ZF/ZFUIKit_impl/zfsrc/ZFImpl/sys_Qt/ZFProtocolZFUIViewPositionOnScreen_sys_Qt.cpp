#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionOnScreen.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionOnScreenImpl_sys_Qt, ZFUIViewPositionOnScreen, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewPositionOnScreen(
            ZF_IN ZFUIView *view
            , ZF_OUT ZFUIRect &rect
            ) {
        QGraphicsWidget *nativeView = (QGraphicsWidget *)view->nativeView();

        QPointF nativePos = nativeView->mapToScene(QPointF(0, 0));
        rect.x = nativePos.x();
        rect.y = nativePos.y();
        rect.width = view->viewFrame().width;
        rect.height = view->viewFrame().height;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionOnScreenImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

