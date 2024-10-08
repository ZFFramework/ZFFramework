#ifndef _ZFI_ZFImpl_sys_iOS_ZFUIKit_impl_h_
#define _ZFI_ZFImpl_sys_iOS_ZFUIKit_impl_h_

#include "../ZFImpl_ZFUIKit_impl.h"

#include "ZFImpl/sys_iOS/ZFImpl_sys_iOS_ZF_impl.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_iOS

#include <UIKit/UIKit.h>
ZF_NAMESPACE_GLOBAL_BEGIN

// convert utility
inline void ZFImpl_sys_iOS_ZFUIPointToCGPointT(
        ZF_OUT CGPoint &ret
        , ZF_IN const ZFUIPoint &point
        ) {
    ret.x = point.x;
    ret.y = point.y;
}
inline CGPoint ZFImpl_sys_iOS_ZFUIPointToCGPoint(ZF_IN const ZFUIPoint &point) {
    CGPoint ret = CGPointZero;
    ZFImpl_sys_iOS_ZFUIPointToCGPointT(ret, point);
    return ret;
}
inline void ZFImpl_sys_iOS_ZFUIPointFromCGPointT(
        ZF_OUT ZFUIPoint &ret
        , ZF_IN const CGPoint &cgPoint
        ) {
    ret.x = cgPoint.x;
    ret.y = cgPoint.y;
}
inline ZFUIPoint ZFImpl_sys_iOS_ZFUIPointFromCGPoint(ZF_IN const CGPoint &cgPoint) {
    ZFUIPoint ret = ZFUIPointZero();
    ZFImpl_sys_iOS_ZFUIPointFromCGPointT(ret, cgPoint);
    return ret;
}

inline void ZFImpl_sys_iOS_ZFUISizeToCGSizeT(
        ZF_OUT CGSize &ret
        , ZF_IN const ZFUISize &size
        ) {
    ret.width = size.width;
    ret.height = size.height;
}
inline CGSize ZFImpl_sys_iOS_ZFUISizeToCGSize(ZF_IN const ZFUISize &size) {
    CGSize ret = CGSizeZero;
    ZFImpl_sys_iOS_ZFUISizeToCGSizeT(ret, size);
    return ret;
}
inline void ZFImpl_sys_iOS_ZFUISizeFromCGSizeT(
        ZF_OUT ZFUISize &ret
        , ZF_IN const CGSize &cgSize
        ) {
    ret.width = cgSize.width;
    ret.height = cgSize.height;
}
inline ZFUISize ZFImpl_sys_iOS_ZFUISizeFromCGSize(ZF_IN const CGSize &cgSize) {
    ZFUISize ret = ZFUISizeZero();
    ZFImpl_sys_iOS_ZFUISizeFromCGSizeT(ret, cgSize);
    return ret;
}

inline void ZFImpl_sys_iOS_ZFUIRectToCGRectT(
        ZF_OUT CGRect &ret
        , ZF_IN const ZFUIRect &rect
        ) {
    ZFUIRectRound(
            ret.origin.x
            , ret.origin.y
            , ret.size.width
            , ret.size.height
            , rect
            );
}
inline CGRect ZFImpl_sys_iOS_ZFUIRectToCGRect(ZF_IN const ZFUIRect &rect) {
    CGRect ret = CGRectZero;
    ZFImpl_sys_iOS_ZFUIRectToCGRectT(ret, rect);
    return ret;
}
inline void ZFImpl_sys_iOS_ZFUIRectFromCGRectT(
        ZF_OUT ZFUIRect &ret
        , ZF_IN const CGRect &cgRect
        ) {
    ret.x = cgRect.origin.x;
    ret.y = cgRect.origin.y;
    ret.width = cgRect.size.width;
    ret.height = cgRect.size.height;
}
inline ZFUIRect ZFImpl_sys_iOS_ZFUIRectFromCGRect(ZF_IN const CGRect &cgRect) {
    ZFUIRect ret = ZFUIRectZero();
    ZFImpl_sys_iOS_ZFUIRectFromCGRectT(ret, cgRect);
    return ret;
}

inline void ZFImpl_sys_iOS_ZFUIMarginToUIEdgeInsetsT(
        ZF_OUT UIEdgeInsets &ret
        , ZF_IN const ZFUIMargin &margin
        ) {
    ret.left = margin.left;
    ret.top = margin.top;
    ret.right = margin.right;
    ret.bottom = margin.bottom;
}
inline UIEdgeInsets ZFImpl_sys_iOS_ZFUIMarginToUIEdgeInsets(ZF_IN const ZFUIMargin &margin) {
    UIEdgeInsets ret = UIEdgeInsetsZero;
    ZFImpl_sys_iOS_ZFUIMarginToUIEdgeInsetsT(ret, margin);
    return ret;
}
inline void ZFImpl_sys_iOS_ZFUIMarginFromUIEdgeInsetsT(
        ZF_OUT ZFUIMargin &ret
        , ZF_IN const UIEdgeInsets &edgeInsets
        ) {
    ret.left = edgeInsets.left;
    ret.top = edgeInsets.top;
    ret.right = edgeInsets.right;
    ret.bottom = edgeInsets.bottom;
}
inline ZFUIMargin ZFImpl_sys_iOS_ZFUIMarginFromUIEdgeInsets(ZF_IN const UIEdgeInsets &edgeInsets) {
    ZFUIMargin ret = ZFUIMarginZero();
    ZFImpl_sys_iOS_ZFUIMarginFromUIEdgeInsetsT(ret, edgeInsets);
    return ret;
}

extern ZFLIB_ZFUIKit_impl zfbool ZFImpl_sys_iOS_UIColorToARGB(
        ZF_IN_OUT zffloat *pARGB
        , ZF_IN UIColor *color
        );
inline UIColor *ZFImpl_sys_iOS_ZFUIColorToUIColor(ZF_IN const ZFUIColor &color) {
    return [UIColor colorWithRed:ZFUIColorGetR(color) green:ZFUIColorGetG(color) blue:ZFUIColorGetB(color) alpha:ZFUIColorGetA(color)];
}
inline void ZFImpl_sys_iOS_ZFUIColorFromUIColorT(
        ZF_OUT ZFUIColor &ret
        , ZF_IN UIColor *uiColor
        ) {
    zffloat argb[4] = {0};
    ZFImpl_sys_iOS_UIColorToARGB(argb, uiColor);
    ret = ZFUIColorCreate(argb[1], argb[2], argb[3], argb[0]);
}
inline ZFUIColor ZFImpl_sys_iOS_ZFUIColorFromUIColor(ZF_IN UIColor *uiColor) {
    ZFUIColor ret = ZFUIColorZero();
    ZFImpl_sys_iOS_ZFUIColorFromUIColorT(ret, uiColor);
    return ret;
}

// ============================================================
// util method
extern ZFLIB_ZFUIKit_impl void ZFImpl_sys_iOS_viewTreePrintT(
        ZF_OUT zfstring &ret
        , ZF_IN UIView *view
        );
inline zfstring ZFImpl_sys_iOS_viewTreePrint(ZF_IN UIView *view) {
    zfstring ret;
    ZFImpl_sys_iOS_viewTreePrintT(ret, view);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS
#endif // #ifndef _ZFI_ZFImpl_sys_iOS_ZFUIKit_impl_h_

