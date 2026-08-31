#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionMap.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionMapImpl_sys_Qt, ZFUIViewPositionMap, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewPositionMap(
            ZF_OUT ZFUIPoint &ret
            , ZF_IN ZFUIView *view
            , ZF_IN_OPT const ZFUIPoint &localPos = ZFUIPointZero()
            ) {
        QGraphicsWidget *nativeView = (QGraphicsWidget *)view->nativeView();
        QPointF nativePos = nativeView->mapToScene(QPointF(localPos.x, localPos.y));
        ret.x = nativePos.x();
        ret.y = nativePos.y();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionMapImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

