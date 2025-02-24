/**
 * @file ZFUIWheelEvent.h
 * @brief wheel event for ui elements
 */

#ifndef _ZFI_ZFUIWheelEvent_h_
#define _ZFI_ZFUIWheelEvent_h_

#include "ZFUIEvent.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief wheel event for ui elements
 */
zffinal zfclass ZFLIB_ZFUIKit ZFUIWheelEvent : zfextend ZFUIEvent {
    ZFOBJECT_DECLARE(ZFUIWheelEvent, ZFUIEvent)

public:
    /**
     * @brief wheel rotate degree (0~360) in x direction
     *
     * positive value means wheel from left to right
     */
    zffloat wheelX;
    /**
     * @brief wheel rotate degree (0~360) in y direction
     *
     * positive value means wheel from top to bottom
     */
    zffloat wheelY;

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIWheelEvent_h_

