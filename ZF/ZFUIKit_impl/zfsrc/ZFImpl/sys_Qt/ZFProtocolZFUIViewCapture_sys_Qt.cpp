#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewCapture.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QImage>
#include <QPainter>
#include <QPixmap>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewCaptureImpl_sys_Qt, ZFUIViewCapture, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Qt:QImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual zfbool viewCapture(
            ZF_IN ZFUIView *view
            , ZF_IN_OUT ZFUIImage *image
            ) {
        QGraphicsWidget *nativeView = (QGraphicsWidget *)view->nativeView();

        QImage nativeImage(nativeView->geometry().width(), nativeView->geometry().height(), QImage::Format_ARGB32);
        nativeImage.fill(QColor(0, 0, 0, 0));
        QPainter painter(&nativeImage);
        nativeView->scene()->render(&painter);
        image->nativeImage(&nativeImage);

        return zftrue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewCaptureImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

