/**
 * @file ZFUIViewTreePrint.h
 * @brief print view tree for ZFUIView
 */

#ifndef _ZFI_ZFUIViewTreePrint_h_
#define _ZFI_ZFUIViewTreePrint_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief debug use only, to output human readable view tree info
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, void, ZFUIViewTreePrint
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        )

/**
 * @brief delay to #ZFUIViewTreePrint using #ZFTimerOnce
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIViewTreePrintAfterDelay
        , ZFMP_IN(zftimet, delay)
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        )

/**
 * @brief register a custom info getter for output info of a view type using #ZFUIViewTreePrint
 *
 * viewInfoGetter's sender is the view being checking,
 * param0 is a #ZFOutput to output the view's info\n
 * output should contain only infomations without extra new line or tokens,
 * newly set would override old one, set null to remove\n
 * you may add your own at runtime, using #ZF_GLOBAL_INITIALIZER_INIT is recommended\n
 * if a view class not registered, only #ZFObject::objectInfoImplAppend would be outputed
 * \n
 * if both base class and child class registered,
 * child class's one would have higher priority,
 * without parent's one being called
 */
extern ZFLIB_ZFUIKit void ZFUIViewTreePrintInfoGetterForClass(
        ZF_IN const ZFClass *viewClass
        , ZF_IN const ZFListener &viewInfoGetter
        );
/**
 * @brief see #ZFUIViewTreePrintInfoGetterForClass
 */
extern ZFLIB_ZFUIKit ZFListener ZFUIViewTreePrintInfoGetterForClass(ZF_IN const ZFClass *viewClass);

// ============================================================
/**
 * @brief similar to #ZFUIViewTreePrint,
 *   but print native view's view tree
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, void, ZFUIViewTreeNativePrint
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        )

/**
 * @brief delay to #ZFUIViewTreeNativePrint using #ZFTimerOnce
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIViewTreeNativePrintAfterDelay
        , ZFMP_IN(zftimet, delay)
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewTreePrint_h_

