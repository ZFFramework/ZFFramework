/**
 * @file ZFUIKeyEvent.h
 * @brief key event for ui elements
 */

#ifndef _ZFI_ZFUIKeyEvent_h_
#define _ZFI_ZFUIKeyEvent_h_

#include "ZFUIEvent.h"
#include "ZFUIKeyCode.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief key event for ui elements
 */
zffinal zfclass ZFLIB_ZFUIKit ZFUIKeyEvent : zfextend ZFUIEvent {
    ZFOBJECT_DECLARE(ZFUIKeyEvent, ZFUIEvent)

public:
    /**
     * @brief a unique value to differ key event from each other
     */
    zfidentity keyId;
    /**
     * @brief action
     */
    ZFUIKeyAction keyAction;
    /**
     * @brief key code
     */
    ZFUIKeyCode keyCode;
    /**
     * @brief raw key code depends on implementation, try not to abuse it
     */
    zfflags keyCodeRaw;

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIKeyEvent_h_

