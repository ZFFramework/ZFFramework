#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageView.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>
#include <QImage>
#include <QPainter>

class _ZFP_ZFUIImageViewImpl_sys_Qt_ImageView : public QGraphicsWidget {
    Q_OBJECT

public:
    ZFUIImage *_ZFP_image;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) {
        QGraphicsWidget::paint(painter, option, widget);
        if(_ZFP_image == zfnull || _ZFP_image->nativeImage() == zfnull) {
            return;
        }

        zffloat imageScaleFixed = _ZFP_image->imageScaleFixed();
        ZFUIMargin imageNinePatchScaled = ZFUIMarginApplyScale(_ZFP_image->imageNinePatch(), imageScaleFixed);

        QImage *image = (QImage *)_ZFP_image->nativeImage();
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        QRectF bounds(QPointF(), this->geometry().size());
        if(imageNinePatchScaled == ZFUIMarginZero()) {
            painter->drawImage(bounds, *image);
            return;
        }

        ZFUIImageImplNinePatchDrawData drawDatas[9];
        zfmemset(drawDatas, 0, sizeof(drawDatas));
        zfindex drawDatasCount = ZFUIImageImplNinePatchCalc(
                drawDatas,
                ZFUISizeApplyScaleReversely(ZFImpl_sys_Qt_ZFUISizeFromQSize(image->size()), imageScaleFixed),
                imageNinePatchScaled,
                ZFImpl_sys_Qt_ZFUISizeFromQSizeF(bounds.size()));

        for(zfindex i = 0; i < drawDatasCount; ++i) {
            const ZFUIImageImplNinePatchDrawData &drawData = drawDatas[i];
            painter->drawImage(
                ZFImpl_sys_Qt_ZFUIRectToQRect(drawData.dst),
                *image,
                ZFImpl_sys_Qt_ZFUIRectToQRect(ZFUIRectApplyScale(drawData.src, imageScaleFixed)));
        }
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageViewImpl_sys_Qt, ZFUIImageView, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Qt:QImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *nativeImageViewCreate(ZF_IN ZFUIImageView *imageView) {
        return new _ZFP_ZFUIImageViewImpl_sys_Qt_ImageView();
    }
    virtual void nativeImageViewDestroy(ZF_IN ZFUIImageView *imageView) {
        delete (_ZFP_ZFUIImageViewImpl_sys_Qt_ImageView *)imageView->nativeImplView();
    }

    virtual void image(
            ZF_IN ZFUIImageView *imageView
            , ZF_IN ZFUIImage *image
            ) {
        _ZFP_ZFUIImageViewImpl_sys_Qt_ImageView *nativeImplView = (_ZFP_ZFUIImageViewImpl_sys_Qt_ImageView *)imageView->nativeImplView();
        nativeImplView->_ZFP_image = image;
        nativeImplView->update();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIImageView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

