#ifndef _ZFI_ZFImpl_sys_Qt_ZFUIKit_impl_h_
#define _ZFI_ZFImpl_sys_Qt_ZFUIKit_impl_h_

#include "../ZFImpl_ZFUIKit_impl.h"

#include "ZFImpl/sys_Qt/ZFImpl_sys_Qt_ZF_impl.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_Qt
#include <QColor>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QSizeF>

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFUIKit_impl void ZFImpl_sys_Qt_viewInfoT(
        ZF_OUT zfstring &ret
        , ZF_IN QObject *view
        );
inline zfstring ZFImpl_sys_Qt_viewInfo(ZF_IN QObject *view) {
    zfstring ret;
    ZFImpl_sys_Qt_viewInfoT(ret, view);
    return ret;
}

extern ZFLIB_ZFUIKit_impl void ZFImpl_sys_Qt_viewTreePrintT(
        ZF_OUT zfstring &ret
        , ZF_IN QObject *view
        );
inline zfstring ZFImpl_sys_Qt_viewTreePrint(ZF_IN QObject *view) {
    zfstring ret;
    ZFImpl_sys_Qt_viewTreePrintT(ret, view);
    return ret;
}

// convert utility
inline void ZFImpl_sys_Qt_ZFUIPointToQPointT(
        ZF_OUT QPoint &ret
        , ZF_IN const ZFUIPoint &point
        ) {
    ret = QPoint(point.x, point.y);
}
inline QPoint ZFImpl_sys_Qt_ZFUIPointToQPoint(ZF_IN const ZFUIPoint &point) {
    QPoint ret;
    ZFImpl_sys_Qt_ZFUIPointToQPointT(ret, point);
    return ret;
}
inline void ZFImpl_sys_Qt_ZFUIPointFromQPointT(
        ZF_OUT ZFUIPoint &ret
        , ZF_IN const QPoint &qPoint
        ) {
    ret.x = qPoint.x();
    ret.y = qPoint.y();
}
inline ZFUIPoint ZFImpl_sys_Qt_ZFUIPointFromQPoint(ZF_IN const QPoint &qPoint) {
    ZFUIPoint ret = ZFUIPointZero();
    ZFImpl_sys_Qt_ZFUIPointFromQPointT(ret, qPoint);
    return ret;
}

inline void ZFImpl_sys_Qt_ZFUIPointToQPointFT(
        ZF_OUT QPointF &ret
        , ZF_IN const ZFUIPoint &point
        ) {
    ret = QPointF(point.x, point.y);
}
inline QPointF ZFImpl_sys_Qt_ZFUIPointToQPointF(ZF_IN const ZFUIPoint &point) {
    QPointF ret;
    ZFImpl_sys_Qt_ZFUIPointToQPointFT(ret, point);
    return ret;
}
inline void ZFImpl_sys_Qt_ZFUIPointFromQPointFT(
        ZF_OUT ZFUIPoint &ret
        , ZF_IN const QPointF &qPoint
        ) {
    ret.x = (zffloat)qPoint.x();
    ret.y = (zffloat)qPoint.y();
}
inline ZFUIPoint ZFImpl_sys_Qt_ZFUIPointFromQPointF(ZF_IN const QPointF &qPoint) {
    ZFUIPoint ret = ZFUIPointZero();
    ZFImpl_sys_Qt_ZFUIPointFromQPointFT(ret, qPoint);
    return ret;
}

inline void ZFImpl_sys_Qt_ZFUISizeToQSizeT(
        ZF_OUT QSize &ret
        , ZF_IN const ZFUISize &size
        ) {
    ret = QSize(size.width, size.height);
}
inline QSize ZFImpl_sys_Qt_ZFUISizeToQSize(ZF_IN const ZFUISize &size) {
    QSize ret;
    ZFImpl_sys_Qt_ZFUISizeToQSizeT(ret, size);
    return ret;
}
inline void ZFImpl_sys_Qt_ZFUISizeFromQSizeT(
        ZF_OUT ZFUISize &ret
        , ZF_IN const QSize &qSize
        ) {
    ret.width = qSize.width();
    ret.height = qSize.height();
}
inline ZFUISize ZFImpl_sys_Qt_ZFUISizeFromQSize(ZF_IN const QSize &qSize) {
    ZFUISize ret = ZFUISizeZero();
    ZFImpl_sys_Qt_ZFUISizeFromQSizeT(ret, qSize);
    return ret;
}

inline void ZFImpl_sys_Qt_ZFUISizeToQSizeFT(
        ZF_OUT QSizeF &ret
        , ZF_IN const ZFUISize &size
        ) {
    ret = QSizeF(size.width, size.height);
}
inline QSizeF ZFImpl_sys_Qt_ZFUISizeToQSizeF(ZF_IN const ZFUISize &size) {
    QSizeF ret;
    ZFImpl_sys_Qt_ZFUISizeToQSizeFT(ret, size);
    return ret;
}
inline void ZFImpl_sys_Qt_ZFUISizeFromQSizeFT(
        ZF_OUT ZFUISize &ret
        , ZF_IN const QSizeF &qSize
        ) {
    ret.width = (zffloat)qSize.width();
    ret.height = (zffloat)qSize.height();
}
inline ZFUISize ZFImpl_sys_Qt_ZFUISizeFromQSizeF(ZF_IN const QSizeF &qSize) {
    ZFUISize ret = ZFUISizeZero();
    ZFImpl_sys_Qt_ZFUISizeFromQSizeFT(ret, qSize);
    return ret;
}

inline void ZFImpl_sys_Qt_ZFUIRectToQRectT(
        ZF_OUT QRect &ret
        , ZF_IN const ZFUIRect &rect
        ) {
    ret.setRect(
            ZFUIRectRoundX(rect)
            , ZFUIRectRoundY(rect)
            , ZFUIRectRoundWidth(rect)
            , ZFUIRectRoundHeight(rect)
            );
}
inline QRect ZFImpl_sys_Qt_ZFUIRectToQRect(ZF_IN const ZFUIRect &rect) {
    QRect ret;
    ZFImpl_sys_Qt_ZFUIRectToQRectT(ret, rect);
    return ret;
}
inline void ZFImpl_sys_Qt_ZFUIRectFromQRectT(
        ZF_OUT ZFUIRect &ret
        , ZF_IN const QRect &qRect
        ) {
    ret.x = qRect.x();
    ret.y = qRect.y();
    ret.width = qRect.width();
    ret.height = qRect.height();
}
inline ZFUIRect ZFImpl_sys_Qt_ZFUIRectFromQRect(ZF_IN const QRect &qRect) {
    ZFUIRect ret = ZFUIRectZero();
    ZFImpl_sys_Qt_ZFUIRectFromQRectT(ret, qRect);
    return ret;
}

inline void ZFImpl_sys_Qt_ZFUIRectToQRectFT(
        ZF_OUT QRectF &ret
        , ZF_IN const ZFUIRect &rect
        ) {
    ret = QRectF(rect.x, rect.y, rect.width, rect.height);
}
inline QRectF ZFImpl_sys_Qt_ZFUIRectToQRectF(ZF_IN const ZFUIRect &rect) {
    QRectF ret;
    ZFImpl_sys_Qt_ZFUIRectToQRectFT(ret, rect);
    return ret;
}
inline void ZFImpl_sys_Qt_ZFUIRectFromQRectFT(
        ZF_OUT ZFUIRect &ret
        , ZF_IN const QRectF &qRect
        ) {
    ret.x = (zffloat)qRect.x();
    ret.y = (zffloat)qRect.y();
    ret.width = (zffloat)qRect.width();
    ret.height = (zffloat)qRect.height();
}
inline ZFUIRect ZFImpl_sys_Qt_ZFUIRectFromQRectF(ZF_IN const QRectF &qRect) {
    ZFUIRect ret = ZFUIRectZero();
    ZFImpl_sys_Qt_ZFUIRectFromQRectFT(ret, qRect);
    return ret;
}

inline void ZFImpl_sys_Qt_ZFUIColorToQColorT(
        ZF_IN QColor &ret
        , ZF_IN const ZFUIColor &color
        ) {
    ret.setRgbF(
            (qreal)ZFUIColorGetR(color),
            (qreal)ZFUIColorGetG(color),
            (qreal)ZFUIColorGetB(color),
            (qreal)ZFUIColorGetA(color)
        );
}
inline QColor ZFImpl_sys_Qt_ZFUIColorToQColor(ZF_IN const ZFUIColor &color) {
    QColor ret;
    ZFImpl_sys_Qt_ZFUIColorToQColorT(ret, color);
    return ret;
}
inline void ZFImpl_sys_Qt_ZFUIColorFromQColorT(
        ZF_OUT ZFUIColor &ret
        , ZF_IN const QColor qColor
        ) {
    ret = ZFUIColorCreate(
            (zffloat)qColor.redF(),
            (zffloat)qColor.greenF(),
            (zffloat)qColor.blueF(),
            (zffloat)qColor.alphaF()
        );
}
inline ZFUIColor ZFImpl_sys_Qt_ZFUIColorFromQColor(ZF_IN const QColor qColor) {
    ZFUIColor ret = ZFUIColorZero();
    ZFImpl_sys_Qt_ZFUIColorFromQColorT(ret, qColor);
    return ret;
}

extern ZFLIB_ZFUIKit_impl void ZFImpl_sys_Qt_ZFUIAlignFlagsToQAlignmentT(
        ZF_OUT Qt::Alignment &ret
        , ZF_IN const ZFUIAlignFlags &align
        );
inline Qt::Alignment ZFImpl_sys_Qt_ZFUIAlignFlagsToQAlignment(ZF_IN const ZFUIAlignFlags &align) {
    Qt::Alignment ret = (Qt::Alignment)0;
    ZFImpl_sys_Qt_ZFUIAlignFlagsToQAlignmentT(ret, align);
    return ret;
}
extern ZFLIB_ZFUIKit_impl void ZFImpl_sys_Qt_ZFUIAlignFlagsFromQAlignmentT(
        ZF_OUT ZFUIAlignFlags &ret
        , ZF_IN const Qt::Alignment qAlign
        );
inline ZFUIAlignFlags ZFImpl_sys_Qt_ZFUIAlignFlagsFromQAlignment(ZF_IN const Qt::Alignment qAlign) {
    ZFUIAlignFlags ret = 0;
    ZFImpl_sys_Qt_ZFUIAlignFlagsFromQAlignmentT(ret, qAlign);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt
#endif // #ifndef _ZFI_ZFImpl_sys_Qt_ZFUIKit_impl_h_

