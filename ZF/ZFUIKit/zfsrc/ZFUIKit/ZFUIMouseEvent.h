/**
 * @file ZFUIMouseEvent.h
 * @brief mouse event for ui elements
 */

#ifndef _ZFI_ZFUIMouseEvent_h_
#define _ZFI_ZFUIMouseEvent_h_

#include "ZFUIEvent.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief mouse action for ZFUIMouseEvent
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIMouseAction)
    ZFENUM_VALUE(Down) /**< @brief mouse down */
    ZFENUM_VALUE(Move) /**< @brief mouse move */
    ZFENUM_VALUE(Up) /**< @brief mouse up */
    ZFENUM_VALUE(Cancel) /**< @brief mouse cancel */
    ZFENUM_VALUE(HoverEnter) /**< @brief mouse hover enter */
    ZFENUM_VALUE(Hover) /**< @brief mouse hover */
    ZFENUM_VALUE(HoverExit) /**< @brief mouse hover exit */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Down)
    ZFENUM_VALUE_REGISTER(Move)
    ZFENUM_VALUE_REGISTER(Up)
    ZFENUM_VALUE_REGISTER(Cancel)
    ZFENUM_VALUE_REGISTER(HoverEnter)
    ZFENUM_VALUE_REGISTER(Hover)
    ZFENUM_VALUE_REGISTER(HoverExit)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUIMouseAction)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFUIMouseAction)

/**
 * @brief mouse action for ZFUIMouseEvent
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIMouseButton)
    ZFENUM_VALUE(Left) /**< @brief left button */
    ZFENUM_VALUE(Right) /**< @brief right button */
    ZFENUM_VALUE(Center) /**< @brief center button */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Center)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUIMouseButton)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFUIMouseButton)

// ============================================================
/**
 * @brief mouse event for ui elements
 */
zffinal zfclass ZFLIB_ZFUIKit ZFUIMouseEvent : zfextend ZFUIEvent {
    ZFOBJECT_DECLARE(ZFUIMouseEvent, ZFUIEvent)

public:
    /**
     * @brief a unique value to differ mouse from each other
     *
     * mouseId keeps the same from mouse down/move/up/cancel,
     * another mouse down would renew the mouseId
     */
    zfidentity mouseId;
    /**
     * @brief mouse action
     */
    ZFUIMouseAction mouseAction;
    /**
     * @brief mouse point
     */
    ZFUIPoint mousePoint;
    /**
     * @brief mouse button
     *
     * note, this value is invalid for mouse hover event,
     * and would always be #v_ZFUIMouseButton::e_Left
     */
    ZFUIMouseButton mouseButton;

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

public:
    zfoverride
    virtual void eventOnApplyScale(ZF_IN zffloat scale);
    zfoverride
    virtual void eventOnApplyScaleReversely(ZF_IN zffloat scale);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIMouseEvent_h_

