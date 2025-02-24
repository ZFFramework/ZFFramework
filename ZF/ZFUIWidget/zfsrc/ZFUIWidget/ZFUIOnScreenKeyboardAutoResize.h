/**
 * @file ZFUIOnScreenKeyboardAutoResize.h
 * @brief auto fit #ZFUIWindow according to #ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFUIOnScreenKeyboardAutoResize_h_
#define _ZFI_ZFUIOnScreenKeyboardAutoResize_h_

#include "ZFUIWidgetDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief auto resize #ZFUIWindow according to #ZFUIOnScreenKeyboardState
 *
 * resize logic are done by changing window's #ZFUILayoutParam::margin\n
 * \n
 * #ZFUIOnScreenKeyboardAutoResizeStop would be applied automatically when owner window destroyed
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIWidget, void, ZFUIOnScreenKeyboardAutoResizeStart
        , ZFMP_IN(ZFUIWindow *, window)
        )
/** @brief see #ZFUIOnScreenKeyboardAutoResizeStart */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIWidget, void, ZFUIOnScreenKeyboardAutoResizeStop
        , ZFMP_IN(ZFUIWindow *, window)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIOnScreenKeyboardAutoResize_h_

