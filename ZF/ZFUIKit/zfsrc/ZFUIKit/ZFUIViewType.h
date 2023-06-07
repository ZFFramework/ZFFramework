/**
 * @file ZFUIViewType.h
 * @brief additional class for ZFUIView
 */

#ifndef _ZFI_ZFUIViewType_h_
#define _ZFI_ZFUIViewType_h_

#include "ZFUIGlobalStyle.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFUIView;

// ============================================================
// ZFUISizeType
/**
 * @brief size params for parent to define how to measure children,
 *   see #ZFUILayoutParam for more info
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUISizeType)
    /**
     * @brief wrap child to minimal
     */
    ZFENUM_VALUE(Wrap)
    /**
     * @brief fill child to parent
     */
    ZFENUM_VALUE(Fill)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Wrap)
    ZFENUM_VALUE_REGISTER(Fill)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUISizeType)

// ============================================================
// ZFUISizeParam
/**
 * @brief 2D size
 */
zfclassPOD ZFLIB_ZFUIKit ZFUISizeParam
{
public:
    ZFUISizeTypeEnum width;   /**< @brief width */
    ZFUISizeTypeEnum height;  /**< @brief height */
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUISizeParam
 *       value="(Wrap, Wrap)" // optional, #ZFUISizeType::e_Wrap by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFUIKit, ZFUISizeParam, ZFUISizeParam)

ZFOUTPUT_TYPE(ZFUISizeParam, {output << ZFUISizeParamToString(v);})

ZFCORE_POD_COMPARER_DECLARE(ZFUISizeParam)

/**
 * @brief make a ZFUISizeParam
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUISizeParam, ZFUISizeParamMake,
                               ZFMP_IN(ZFUISizeTypeEnum const &, width),
                               ZFMP_IN(ZFUISizeTypeEnum const &, height))
{
    ZFUISizeParam ret = {width, height};
    return ret;
}
/**
 * @brief make a ZFUISizeParam
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUISizeParam, ZFUISizeParamMake,
                               ZFMP_IN(ZFUISizeTypeEnum const &, v))
{
    ZFUISizeParam ret = {v, v};
    return ret;
}

/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Wrap, #ZFUISizeType::e_Wrap)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUISizeParam, ZFUISizeParamZero)
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Wrap, #ZFUISizeType::e_Wrap)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUISizeParam, ZFUISizeParamWrapWrap)
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Wrap, #ZFUISizeType::e_Fill)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUISizeParam, ZFUISizeParamWrapFill)
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Fill, #ZFUISizeType::e_Wrap)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUISizeParam, ZFUISizeParamFillWrap)
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Fill, #ZFUISizeType::e_Fill)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUISizeParam, ZFUISizeParamFillFill)

// ============================================================
// ZFUILayoutParam
/**
 * @brief base class of all layout param in ZFUIView
 *
 * ZFUIView supply basic but useful dynamic layout mechanism,
 * which use align, margin and measure logic to supply dynamic layout\n
 * while layout a child, ZFUIView would do those things:
 * -  measure child to see it's size depends on #sizeParam
 * -  layout child depends on #ZFUIAlignApply
 *
 * you should always use #ZFUIView::layoutParamCreate to create a layout param\n
 * subclass may override or define new params to supply more complex layout mechanism\n
 * \n
 * dynamic layout logic depends on #ZFUILayoutParam::sizeHint and #ZFUILayoutParam::sizeParam
 *
 * @note all layout param must be declared as #ZFProperty,
 *   and must be serializable
 * @note by default, a layout param would be compared by comparing all property
 */
zfclass ZFLIB_ZFUIKit ZFUILayoutParam : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUILayoutParam, ZFStyleableObject)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when layout param changed,
     * this event would be automatically fired if any of property value changed
     * (#ZFObject::EventObjectPropertyValueOnUpdate),
     * you may also notify it manually to show the layout param changed
     */
    ZFOBSERVER_EVENT(LayoutParamOnChange)


public:
    // ============================================================
    /**
     * @brief see #ZFUILayoutParam, default is #ZFUISizeInvalid, which means no size limit
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, sizeHint,
                                ZFUISizeInvalid())
    /**
     * @brief see #ZFUILayoutParam, default is #ZFUISizeParamWrapWrap
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISizeParam, sizeParam,
                                ZFUISizeParamWrapWrap())
    /**
     * @brief see #ZFUILayoutParam, default is (ZFUIAlign::e_Left | ZFUIAlign::e_Top)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, layoutAlign,
                                ZFUIAlign::e_Left | ZFUIAlign::e_Top)
    /**
     * @brief see #ZFUILayoutParam, default is (0, 0, 0, 0)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, layoutMargin,
                                ZFUIMarginZero())

public:
    // ============================================================
    // util
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_0(void, sizeFill)
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_1(void, sizeFill,
                       ZFMP_IN(zffloat, size))
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_1(void, sizeFill,
                       ZFMP_IN(const ZFUISize &, size))
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_2(void, sizeFill,
                       ZFMP_IN(zffloat, width),
                       ZFMP_IN(zffloat, height))
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_0(void, sizeWrap)
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_1(void, sizeWrap,
                       ZFMP_IN(zffloat, size))
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_1(void, sizeWrap,
                       ZFMP_IN(const ZFUISize &, size))
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_2(void, sizeWrap,
                       ZFMP_IN(zffloat, width),
                       ZFMP_IN(zffloat, height))

    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_0(void, widthFill)
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_1(void, widthFill,
                       ZFMP_IN(zffloat, width))
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_0(void, widthWrap)
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_1(void, widthWrap,
                       ZFMP_IN(zffloat, width))

    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_0(void, heightFill)
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_1(void, heightFill,
                       ZFMP_IN(zffloat, height))
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_0(void, heightWrap)
    /** @brief util to set #sizeParam and #sizeHint */
    ZFMETHOD_DECLARE_1(void, heightWrap,
                       ZFMP_IN(zffloat, height))

    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignCenter)
    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignLeft)
    /** @brief util to set #layoutAlign and #layoutMargin */
    ZFMETHOD_DECLARE_1(void, alignLeft,
                       ZFMP_IN(zffloat, marginLeft))
    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignTop)
    /** @brief util to set #layoutAlign and #layoutMargin */
    ZFMETHOD_DECLARE_1(void, alignTop,
                       ZFMP_IN(zffloat, marginTop))
    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignRight)
    /** @brief util to set #layoutAlign and #layoutMargin */
    ZFMETHOD_DECLARE_1(void, alignRight,
                       ZFMP_IN(zffloat, marginRight))
    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignBottom)
    /** @brief util to set #layoutAlign and #layoutMargin */
    ZFMETHOD_DECLARE_1(void, alignBottom,
                       ZFMP_IN(zffloat, marginBottom))
    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignLeftTop)
    /** @brief util to set #layoutAlign and #layoutMargin */
    ZFMETHOD_DECLARE_2(void, alignLeftTop,
                       ZFMP_IN(zffloat, marginLeft),
                       ZFMP_IN(zffloat, marginTop))
    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignLeftBottom)
    /** @brief util to set #layoutAlign and #layoutMargin */
    ZFMETHOD_DECLARE_2(void, alignLeftBottom,
                       ZFMP_IN(zffloat, marginLeft),
                       ZFMP_IN(zffloat, marginBottom))
    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignRightTop)
    /** @brief util to set #layoutAlign and #layoutMargin */
    ZFMETHOD_DECLARE_2(void, alignRightTop,
                       ZFMP_IN(zffloat, marginRight),
                       ZFMP_IN(zffloat, marginTop))
    /** @brief util to set #layoutAlign */
    ZFMETHOD_DECLARE_0(void, alignRightBottom)
    /** @brief util to set #layoutAlign and #layoutMargin */
    ZFMETHOD_DECLARE_2(void, alignRightBottom,
                       ZFMP_IN(zffloat, marginRight),
                       ZFMP_IN(zffloat, marginBottom))

    /** @brief util to set #layoutMargin */
    ZFMETHOD_DECLARE_1(void, margin,
                       ZFMP_IN(zffloat, margin))
    /** @brief util to set #layoutMargin */
    ZFMETHOD_DECLARE_4(void, margin,
                       ZFMP_IN(zffloat, left),
                       ZFMP_IN(zffloat, top),
                       ZFMP_IN(zffloat, right),
                       ZFMP_IN(zffloat, bottom))

public:
    // ============================================================
    /** @brief see #layoutParamApply */
    ZFMETHOD_DECLARE_STATIC_4(void, layoutParamApply,
                              ZFMP_OUT(ZFUIRect &, ret),
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(ZFUIView *, child),
                              ZFMP_IN(ZFUILayoutParam *, lp))
    /**
     * @brief calculate single child's frame using logic of #ZFUILayoutParam
     *
     * this is not necessary for subclass to declare its own one,
     * it's declared for convenient for subclass to layout child
     * using parent's layout logic
     */
    ZFMETHOD_DECLARE_STATIC_3(ZFUIRect, layoutParamApply,
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(ZFUIView *, child),
                              ZFMP_IN(ZFUILayoutParam *, lp))

    /**
     * @brief util method to apply sizeHint according sizeParam
     */
    ZFMETHOD_DECLARE_STATIC_4(void, sizeHintApply,
                              ZFMP_OUT(zffloat &, ret),
                              ZFMP_IN(zffloat, size),
                              ZFMP_IN(zffloat, sizeHint),
                              ZFMP_IN(ZFUISizeTypeEnum, sizeParam))
    /** @brief see #sizeHintApply */
    ZFMETHOD_DECLARE_STATIC_3(zffloat, sizeHintApply,
                              ZFMP_IN(zffloat, size),
                              ZFMP_IN(zffloat, sizeHint),
                              ZFMP_IN(ZFUISizeTypeEnum, sizeParam))

    /**
     * @brief util method to apply sizeHint according sizeParam
     */
    ZFMETHOD_DECLARE_STATIC_4(void, sizeHintApply,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISizeParam &, sizeParam))
    /** @brief see #sizeHintApply */
    ZFMETHOD_DECLARE_STATIC_3(ZFUISize, sizeHintApply,
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISizeParam &, sizeParam))

    /**
     * @brief merge two size hint
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintMerge,
                              ZFMP_OUT(zffloat &, ret),
                              ZFMP_IN(zffloat, sizeHint0),
                              ZFMP_IN(zffloat, sizeHint1))
    /** @brief see #sizeHintMerge */
    ZFMETHOD_DECLARE_STATIC_2(zffloat, sizeHintMerge,
                              ZFMP_IN(zffloat, sizeHint0),
                              ZFMP_IN(zffloat, sizeHint1))

    /**
     * @brief merge two size hint
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintMerge,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint0),
                              ZFMP_IN(const ZFUISize &, sizeHint1))
    /** @brief see #sizeHintMerge */
    ZFMETHOD_DECLARE_STATIC_2(ZFUISize, sizeHintMerge,
                              ZFMP_IN(const ZFUISize &, sizeHint0),
                              ZFMP_IN(const ZFUISize &, sizeHint1))

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintOffset,
                              ZFMP_OUT(zffloat &, ret),
                              ZFMP_IN(zffloat, sizeHint),
                              ZFMP_IN(zffloat, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_DECLARE_STATIC_2(zffloat, sizeHintOffset,
                              ZFMP_IN(zffloat, sizeHint),
                              ZFMP_IN(zffloat, offset))

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintOffset,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISize &, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_DECLARE_STATIC_2(ZFUISize, sizeHintOffset,
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISize &, offset))

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintOffset,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(zffloat, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_DECLARE_STATIC_2(ZFUISize, sizeHintOffset,
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(zffloat, offset))

public:
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityCalcString(this->classData()->classNameFull());
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        if(anotherObj != zfnull && anotherObj->classData()->classIsTypeOf(zfself::ClassData())
            && ZFClassUtil::allPropertyIsEqual(this, anotherObj))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

protected:
    zfoverride
    virtual void objectPropertyValueOnUpdate(ZF_IN const ZFProperty *property, ZF_IN const void *oldValue)
    {
        zfsuper::objectPropertyValueOnUpdate(property, oldValue);
        if(oldValue != zfnull)
        {
            this->layoutParamOnChange();
        }
    }
protected:
    /** @brief see #EventLayoutParamOnChange */
    virtual inline void layoutParamOnChange(void)
    {
        this->observerNotify(ZFUILayoutParam::EventLayoutParamOnChange());
    }
};

// ============================================================
/**
 * @brief three layer for ZFUIView
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIViewChildLayer)
    ZFENUM_VALUE(Normal) /**< @brief added by #ZFUIView::childAdd */
    ZFENUM_VALUE(InternalImpl) /**< @brief added by #ZFUIView::internalImplViewAdd */
    ZFENUM_VALUE(InternalBg) /**< @brief added by #ZFUIView::internalBgViewAdd */
    ZFENUM_VALUE(InternalFg) /**< @brief added by #ZFUIView::internalFgViewAdd */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(InternalImpl)
    ZFENUM_VALUE_REGISTER(InternalBg)
    ZFENUM_VALUE_REGISTER(InternalFg)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUIViewChildLayer)

// ============================================================
// ZFUIViewMeasureResult
/**
 * @brief data used by #ZFUIView::EventViewLayoutOnMeasureFinish,
 *   you may modify the #ZFUIViewMeasureResult::measuredSize
 *   to override the measured result
 */
zfclass ZFLIB_ZFUIKit ZFUIViewMeasureResult : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIViewMeasureResult, ZFObject)

public:
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISize sizeHint;
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISizeParam sizeParam;
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISize measuredSize;

    ZFALLOC_CACHE_RELEASE({
        cache->sizeHint = ZFUISizeZero();
        cache->sizeParam = ZFUISizeParamZero();
        cache->measuredSize = ZFUISizeZero();
    })

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

// ============================================================
/**
 * @brief transform support info
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUITransform)
    ZFENUM_VALUE_WITH_INIT(Unavailable, 0) /**< @brief no transform support */

    ZFENUM_VALUE_WITH_INIT(TranslateX, 1) /**< @brief support translate */
    ZFENUM_VALUE_WITH_INIT(TranslateY, 2) /**< @brief support translate */
    ZFENUM_VALUE_WITH_INIT(TranslateZ, 4) /**< @brief support translate */
    ZFENUM_VALUE_WITH_INIT(ScaleX, 8) /**< @brief support scale */
    ZFENUM_VALUE_WITH_INIT(ScaleY, 16) /**< @brief support scale */
    ZFENUM_VALUE_WITH_INIT(ScaleZ, 32) /**< @brief support scale */
    ZFENUM_VALUE_WITH_INIT(RotateX, 64) /**< @brief support rotate */
    ZFENUM_VALUE_WITH_INIT(RotateY, 128) /**< @brief support rotate */
    ZFENUM_VALUE_WITH_INIT(RotateZ, 256) /**< @brief support rotate */

    /**
     * @brief support basic 2D transform
     */
    ZFENUM_VALUE_WITH_INIT(Transform2D, 0
            | e_TranslateX
            | e_TranslateY
            | e_ScaleX
            | e_ScaleY
            | e_RotateZ
        )
    /**
     * @brief support 3D transform
     */
    ZFENUM_VALUE_WITH_INIT(Transform3D, 0
            | e_TranslateX
            | e_TranslateY
            | e_TranslateZ
            | e_ScaleX
            | e_ScaleY
            | e_ScaleZ
            | e_RotateX
            | e_RotateY
            | e_RotateZ
        )
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Unavailable)
    ZFENUM_VALUE_REGISTER(TranslateX)
    ZFENUM_VALUE_REGISTER(TranslateY)
    ZFENUM_VALUE_REGISTER(TranslateZ)
    ZFENUM_VALUE_REGISTER(ScaleX)
    ZFENUM_VALUE_REGISTER(ScaleY)
    ZFENUM_VALUE_REGISTER(ScaleZ)
    ZFENUM_VALUE_REGISTER(RotateX)
    ZFENUM_VALUE_REGISTER(RotateY)
    ZFENUM_VALUE_REGISTER(RotateZ)
ZFENUM_END_FLAGS(ZFLIB_ZFUIKit, ZFUITransform, ZFUITransformFlags)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewType_h_

