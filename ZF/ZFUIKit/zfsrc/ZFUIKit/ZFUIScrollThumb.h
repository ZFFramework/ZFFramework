/**
 * @file ZFUIScrollThumb.h
 * @brief abstract scroll thumb for #ZFUIScrollView
 */

#ifndef _ZFI_ZFUIScrollThumb_h_
#define _ZFI_ZFUIScrollThumb_h_

#include "ZFUIScrollView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIScrollThumb
/**
 * @brief abstract scroll thumb for #ZFUIScrollView
 */
zfinterface ZFLIB_ZFUIKit ZFUIScrollThumb : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFUIScrollThumb, ZFInterface)

public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when thumb updated
     */
    ZFEVENT(ScrollThumbOnUpdate)

public:
    /**
     * @brief util method to calculate scroll thumb's position according to view's position
     *
     * 0 result size means no need to show the scroll thumb
     */
    static void scrollThumbPosFromViewPos(
            ZF_OUT zffloat &scrollThumbResultPos
            , ZF_OUT zffloat &scrollThumbResultSize
            , ZF_IN zffloat scrollThumbRange
            , ZF_IN zffloat scrollViewSize
            , ZF_IN zffloat scrollContentOffset
            , ZF_IN zffloat scrollContentSize
            , ZF_IN zffloat scrollThumbMinSize
            , ZF_IN_OPT zffloat headMargin = 0
            , ZF_IN_OPT zffloat tailMargin = 0
            );
    /**
     * @brief util method to calculate view's position according to scroll thumb's position
     */
    static void scrollThumbPosToViewPos(
            ZF_OUT zffloat &scrollContentOffset
            , ZF_IN zffloat scrollViewSize
            , ZF_IN zffloat scrollContentSize
            , ZF_IN zffloat scrollThumbPos
            , ZF_IN zffloat scrollThumbSize
            , ZF_IN zffloat scrollThumbRange
            , ZF_IN_OPT zffloat headMargin = 0
            , ZF_IN_OPT zffloat tailMargin = 0
            );

public:
    /**
     * @brief owner scroll view
     */
    zffinal inline zfanyT<ZFUIScrollView> const &scrollView(void) {
        return this->_scrollView;
    }
    /**
     * @brief this thumb is horizontal or not
     */
    zffinal inline zfbool scrollThumbHorizontal(void) {
        return this->_horizontal;
    }

public:
    /**
     * @brief init the scroll thumb
     */
    virtual void scrollThumbInit(void) zfpurevirtual;
    /**
     * @brief destroy the scroll thumb
     */
    virtual void scrollThumbDealloc(void) zfpurevirtual;

    /**
     * @brief update the scroll thumb, this method may be called repeatly
     *
     * you may update thumb depending on #ZFUIScrollView::scrollViewState
     */
    virtual void scrollThumbUpdate(void) zfpurevirtual;

private:
    zfanyT<ZFUIScrollView> _scrollView;
    zfbool _horizontal;
    friend zfclassFwd ZFUIScrollView;
};

// ============================================================
/**
 * @brief change default scroll thumb class
 *
 * set to null to use default
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void, ZFUIScrollThumbHorizontalClass
        , ZFMP_IN(const ZFClass *, cls)
        )
/**
 * @brief get the class of default scroll thumb
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, const ZFClass *, ZFUIScrollThumbHorizontalClass)
/**
 * @brief change default scroll thumb class
 *
 * set to null to use default
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void, ZFUIScrollThumbVerticalClass
        , ZFMP_IN(const ZFClass *, cls)
        )
/**
 * @brief get the class of default scroll thumb
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUIKit, const ZFClass *, ZFUIScrollThumbVerticalClass)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollThumb_h_

