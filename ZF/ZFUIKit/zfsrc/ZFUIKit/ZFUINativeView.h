/**
 * @file ZFUINativeView.h
 * @brief native view wrapper
 */

#ifndef _ZFI_ZFUINativeView_h_
#define _ZFI_ZFUINativeView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUINativeView
/**
 * @brief view container to hold a native view
 *
 * used to hold a native view in ZFUIKit environment,
 * what you should do is create a #ZFUINativeView
 * and store native view by #nativeImplView\n
 * the embeded native view would be layouted to fill #ZFUINativeView's frame,
 * to change it's layout logic, you should use it as a #ZFUIView\n
 * the #ZFUINativeView itself is serializable,
 * however the native view itself can not be serialized,
 * it should be initialized by app manually
 */
zffinal zfclass ZFLIB_ZFUIKit ZFUINativeView : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUINativeView, ZFUIView)

protected:
    /**
     * @brief init with native view, see #ZFUINativeView
     */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(void *, nativeImplView))

    zfoverride
    virtual void objectOnInit(void) {zfsuper::objectOnInit();}

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

public:
    zfoverride
    virtual void *nativeImplView(void) {
        return zfsuper::nativeImplView();
    }

    /**
     * @brief see #nativeImplView
     */
    zfoverride
    virtual void nativeImplView(
            ZF_IN void *nativeImplView
            , ZF_IN ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback = zfnull
            ) {
        zfsuper::nativeImplView(nativeImplView, nativeImplViewDeleteCallback);
    }

    /**
     * @brief measure the native view
     */
    ZFMETHOD_DECLARE_2(void, measureNativeView
            , ZFMP_OUT(ZFUISize &, ret)
            , ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeInvalid())
            )

protected:
    zfoverride
    virtual void layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        );
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUINativeView_h_

