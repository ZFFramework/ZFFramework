#include "ZFUIViewType.h"
#include "ZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUISizeType)

// ============================================================
// ZFUISizeParam
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFUISizeParam, ZFUISizeParam, {
        ZFCoreArray<ZFIndexRange> pos;
        if(ZFCoreDataPairSplitString(pos, 2, src, srcLen)
                && ZFUISizeTypeFromStringT(v.width, src + pos[0].start, pos[0].count)
                && ZFUISizeTypeFromStringT(v.height, src + pos[1].start, pos[1].count)
                ) {
            return zftrue;
        }

        if(errorHint) {
            zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
        }
        return zffalse;
    }, {
        s += "(";
        ZFUISizeTypeToStringT(s, v.width);
        s += ", ";
        ZFUISizeTypeToStringT(s, v.height);
        s += ")";
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUISizeParam, ZFUISizeType, width)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUISizeParam, ZFUISizeType, height)

ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUISizeParam, ZFUISizeParamCreate
        , ZFMP_IN(ZFUISizeType const &, width)
        , ZFMP_IN(ZFUISizeType const &, height)
        )
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFUISizeParam, ZFUISizeParamCreate
        , ZFMP_IN(ZFUISizeType const &, v)
        )

ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamZero, ZFUISizeParamCreate(v_ZFUISizeType::e_Wrap, v_ZFUISizeType::e_Wrap))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamWrapWrap, ZFUISizeParamCreate(v_ZFUISizeType::e_Wrap, v_ZFUISizeType::e_Wrap))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamWrapFill, ZFUISizeParamCreate(v_ZFUISizeType::e_Wrap, v_ZFUISizeType::e_Fill))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamFillWrap, ZFUISizeParamCreate(v_ZFUISizeType::e_Fill, v_ZFUISizeType::e_Wrap))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamFillFill, ZFUISizeParamCreate(v_ZFUISizeType::e_Fill, v_ZFUISizeType::e_Fill))

// ============================================================
ZFOBJECT_REGISTER(ZFUILayoutParam)
ZFEVENT_REGISTER(ZFUILayoutParam, LayoutParamOnUpdate)

// ============================================================
// util
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, sizeFill) {
    this->sizeParam(ZFUISizeParamFillFill());
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, sizeFill
        , ZFMP_IN(const ZFUISize &, size)
        ) {
    this->sizeParam(ZFUISizeParamFillFill());
    this->sizeHint(size);
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, sizeFill
        , ZFMP_IN(zffloat, size)
        ) {
    this->sizeParam(ZFUISizeParamFillFill());
    this->sizeHint(ZFUISizeCreate(size));
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, void, sizeFill
        , ZFMP_IN(zffloat, width)
        , ZFMP_IN(zffloat, height)
        ) {
    this->sizeParam(ZFUISizeParamFillFill());
    this->sizeHint(ZFUISizeCreate(width, height));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, sizeWrap) {
    this->sizeParam(ZFUISizeParamWrapWrap());
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, sizeWrap
        , ZFMP_IN(const ZFUISize &, size)
        ) {
    this->sizeParam(ZFUISizeParamWrapWrap());
    this->sizeHint(size);
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, sizeWrap
        , ZFMP_IN(zffloat, size)
        ) {
    this->sizeParam(ZFUISizeParamWrapWrap());
    this->sizeHint(ZFUISizeCreate(size));
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, void, sizeWrap
        , ZFMP_IN(zffloat, width)
        , ZFMP_IN(zffloat, height)
        ) {
    this->sizeParam(ZFUISizeParamWrapWrap());
    this->sizeHint(ZFUISizeCreate(width, height));
}

ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, widthFill) {
    this->sizeParam(ZFUISizeParamCreate(v_ZFUISizeType::e_Fill, this->sizeParam().height));
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, widthFill
        , ZFMP_IN(zffloat, width)
        ) {
    this->sizeParam(ZFUISizeParamCreate(v_ZFUISizeType::e_Fill, this->sizeParam().height));
    this->sizeHint(ZFUISizeCreate(width, this->sizeHint().height));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, widthWrap) {
    this->sizeParam(ZFUISizeParamCreate(v_ZFUISizeType::e_Wrap, this->sizeParam().height));
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, widthWrap
        , ZFMP_IN(zffloat, width)
        ) {
    this->sizeParam(ZFUISizeParamCreate(v_ZFUISizeType::e_Wrap, this->sizeParam().height));
    this->sizeHint(ZFUISizeCreate(width, this->sizeHint().height));
}

ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, heightFill) {
    this->sizeParam(ZFUISizeParamCreate(this->sizeParam().width, v_ZFUISizeType::e_Fill));
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, heightFill
        , ZFMP_IN(zffloat, height)
        ) {
    this->sizeParam(ZFUISizeParamCreate(this->sizeParam().width, v_ZFUISizeType::e_Fill));
    this->sizeHint(ZFUISizeCreate(this->sizeHint().width, height));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, heightWrap) {
    this->sizeParam(ZFUISizeParamCreate(this->sizeParam().width, v_ZFUISizeType::e_Wrap));
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, heightWrap
        , ZFMP_IN(zffloat, height)
        ) {
    this->sizeParam(ZFUISizeParamCreate(this->sizeParam().width, v_ZFUISizeType::e_Wrap));
    this->sizeHint(ZFUISizeCreate(this->sizeHint().width, height));
}

ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignCenter) {
    this->align(v_ZFUIAlign::e_Center);
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignLeft) {
    this->align(v_ZFUIAlign::e_Left);
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, alignLeft
        , ZFMP_IN(zffloat, marginLeft)
        ) {
    this->align(v_ZFUIAlign::e_Left);
    const ZFUIMargin &m = this->margin();
    this->margin(ZFUIMarginCreate(marginLeft, m.top, m.right, m.bottom));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignTop) {
    this->align(v_ZFUIAlign::e_Top);
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, alignTop
        , ZFMP_IN(zffloat, marginTop)
        ) {
    this->align(v_ZFUIAlign::e_Top);
    const ZFUIMargin &m = this->margin();
    this->margin(ZFUIMarginCreate(m.left, marginTop, m.right, m.bottom));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignRight) {
    this->align(v_ZFUIAlign::e_Right);
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, alignRight
        , ZFMP_IN(zffloat, marginRight)
        ) {
    this->align(v_ZFUIAlign::e_Right);
    const ZFUIMargin &m = this->margin();
    this->margin(ZFUIMarginCreate(m.left, m.top, marginRight, m.bottom));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignBottom) {
    this->align(v_ZFUIAlign::e_Bottom);
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, alignBottom
        , ZFMP_IN(zffloat, marginBottom)
        ) {
    this->align(v_ZFUIAlign::e_Bottom);
    const ZFUIMargin &m = this->margin();
    this->margin(ZFUIMarginCreate(m.left, m.top, m.right, marginBottom));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignLeftTop) {
    this->align(v_ZFUIAlign::e_Left | v_ZFUIAlign::e_Top);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, void, alignLeftTop
        , ZFMP_IN(zffloat, marginLeft)
        , ZFMP_IN(zffloat, marginTop)
        ) {
    this->align(v_ZFUIAlign::e_Left | v_ZFUIAlign::e_Top);
    const ZFUIMargin &m = this->margin();
    this->margin(ZFUIMarginCreate(marginLeft, marginTop, m.right, m.bottom));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignLeftBottom) {
    this->align(v_ZFUIAlign::e_Left | v_ZFUIAlign::e_Bottom);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, void, alignLeftBottom
        , ZFMP_IN(zffloat, marginLeft)
        , ZFMP_IN(zffloat, marginBottom)
        ) {
    this->align(v_ZFUIAlign::e_Left | v_ZFUIAlign::e_Bottom);
    const ZFUIMargin &m = this->margin();
    this->margin(ZFUIMarginCreate(marginLeft, m.top, m.right, marginBottom));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignRightTop) {
    this->align(v_ZFUIAlign::e_Right | v_ZFUIAlign::e_Top);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, void, alignRightTop
        , ZFMP_IN(zffloat, marginRight)
        , ZFMP_IN(zffloat, marginTop)
        ) {
    this->align(v_ZFUIAlign::e_Right | v_ZFUIAlign::e_Top);
    const ZFUIMargin &m = this->margin();
    this->margin(ZFUIMarginCreate(m.left, marginTop, marginRight, m.bottom));
}
ZFMETHOD_DEFINE_0(ZFUILayoutParam, void, alignRightBottom) {
    this->align(v_ZFUIAlign::e_Right | v_ZFUIAlign::e_Bottom);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, void, alignRightBottom
        , ZFMP_IN(zffloat, marginRight)
        , ZFMP_IN(zffloat, marginBottom)
        ) {
    this->align(v_ZFUIAlign::e_Right | v_ZFUIAlign::e_Bottom);
    const ZFUIMargin &m = this->margin();
    this->margin(ZFUIMarginCreate(m.left, m.top, marginRight, marginBottom));
}

ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, margin
        , ZFMP_IN(zffloat, margin)
        ) {
    this->margin(ZFUIMarginCreate(margin));
}
ZFMETHOD_DEFINE_4(ZFUILayoutParam, void, margin
        , ZFMP_IN(zffloat, left)
        , ZFMP_IN(zffloat, top)
        , ZFMP_IN(zffloat, right)
        , ZFMP_IN(zffloat, bottom)
        ) {
    this->margin(ZFUIMarginCreate(left, top, right, bottom));
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, marginLeft
        , ZFMP_IN(zffloat, left)
        ) {
    const ZFUIMargin &margin = this->margin();
    this->margin(ZFUIMarginCreate(left, margin.top, margin.right, margin.bottom));
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, marginTop
        , ZFMP_IN(zffloat, top)
        ) {
    const ZFUIMargin &margin = this->margin();
    this->margin(ZFUIMarginCreate(margin.left, top, margin.right, margin.bottom));
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, marginRight
        , ZFMP_IN(zffloat, right)
        ) {
    const ZFUIMargin &margin = this->margin();
    this->margin(ZFUIMarginCreate(margin.left, margin.top, right, margin.bottom));
}
ZFMETHOD_DEFINE_1(ZFUILayoutParam, void, marginBottom
        , ZFMP_IN(zffloat, bottom)
        ) {
    const ZFUIMargin &margin = this->margin();
    this->margin(ZFUIMarginCreate(margin.left, margin.top, margin.right, bottom));
}

// ============================================================
ZFMETHOD_DEFINE_4(ZFUILayoutParam, void, layoutParamApplyT
        , ZFMP_OUT(ZFUIRect &, ret)
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(ZFUIView *, child)
        , ZFMP_IN(ZFUILayoutParam *, lp)
        ) {
    ZFUILayoutParam::layoutParamApplyT(ret, rect, child
            , lp->sizeHint()
            , lp->sizeParam()
            , lp->align()
            , lp->margin()
            );
}
ZFMETHOD_DEFINE_3(ZFUILayoutParam, ZFUIRect, layoutParamApply
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(ZFUIView *, child)
        , ZFMP_IN(ZFUILayoutParam *, lp)
        ) {
    ZFUIRect ret = ZFUIRectZero();
    ZFUILayoutParam::layoutParamApplyT(ret, rect, child
            , lp->sizeHint()
            , lp->sizeParam()
            , lp->align()
            , lp->margin()
            );
    return ret;
}
ZFMETHOD_DEFINE_7(ZFUILayoutParam, void, layoutParamApplyT
        , ZFMP_OUT(ZFUIRect &, ret)
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(ZFUIView *, child)
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(const ZFUISizeParam &, sizeParam)
        , ZFMP_IN(const ZFUIAlignFlags &, align)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ZFUISize refSizeTmp = ZFUIRectGetSize(ZFUIRectApplyMargin(rect, margin));
    if(refSizeTmp.width < 0) {
        refSizeTmp.width = 0;
    }
    if(refSizeTmp.height < 0) {
        refSizeTmp.height = 0;
    }
    if(sizeHint.width >= 0) {
        refSizeTmp.width = zfmMin(sizeHint.width, refSizeTmp.width);
    }
    if(sizeHint.height >= 0) {
        refSizeTmp.height = zfmMin(sizeHint.height, refSizeTmp.height);
    }

    child->layoutMeasure(refSizeTmp, sizeParam);
    ZFUIAlignApplyT(
            ret
            , align
            , ZFUIRectApplyMargin(rect, margin)
            , child->layoutMeasuredSize()
            );
}
ZFMETHOD_DEFINE_6(ZFUILayoutParam, ZFUIRect, layoutParamApply
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(ZFUIView *, child)
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(const ZFUISizeParam &, sizeParam)
        , ZFMP_IN(const ZFUIAlignFlags &, align)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ZFUIRect ret = ZFUIRectZero();
    ZFUILayoutParam::layoutParamApplyT(ret, rect, child
            , sizeHint
            , sizeParam
            , align
            , margin
            );
    return ret;
}

ZFMETHOD_DEFINE_4(ZFUILayoutParam, void, sizeHintApplyT
        , ZFMP_OUT(zffloat &, ret)
        , ZFMP_IN(zffloat, size)
        , ZFMP_IN(zffloat, sizeHint)
        , ZFMP_IN(ZFUISizeType, sizeParam)
        ) {
    ret = size;
    switch(sizeParam) {
        case v_ZFUISizeType::e_Wrap:
            if(sizeHint >= 0 && size > sizeHint) {
                ret = sizeHint;
            }
            break;
        case v_ZFUISizeType::e_Fill:
            if(sizeHint >= 0) {
                ret = sizeHint;
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
}
ZFMETHOD_DEFINE_3(ZFUILayoutParam, zffloat, sizeHintApply
        , ZFMP_IN(zffloat, size)
        , ZFMP_IN(zffloat, sizeHint)
        , ZFMP_IN(ZFUISizeType, sizeParam)
        ) {
    zffloat ret = size;
    ZFUILayoutParam::sizeHintApplyT(ret, size, sizeHint, sizeParam);
    return ret;
}

ZFMETHOD_DEFINE_4(ZFUILayoutParam, void, sizeHintApplyT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(const ZFUISizeParam &, sizeParam)
        ) {
    ZFUILayoutParam::sizeHintApplyT(ret.width, size.width, sizeHint.width, sizeParam.width);
    ZFUILayoutParam::sizeHintApplyT(ret.height, size.height, sizeHint.height, sizeParam.height);
}
ZFMETHOD_DEFINE_3(ZFUILayoutParam, ZFUISize, sizeHintApply
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(const ZFUISizeParam &, sizeParam)
        ) {
    ZFUISize ret = ZFUISizeZero();
    ZFUILayoutParam::sizeHintApplyT(ret.width, size.width, sizeHint.width, sizeParam.width);
    ZFUILayoutParam::sizeHintApplyT(ret.height, size.height, sizeHint.height, sizeParam.height);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintMergeT
        , ZFMP_OUT(zffloat &, ret)
        , ZFMP_IN(zffloat, sizeHint0)
        , ZFMP_IN(zffloat, sizeHint1)
        ) {
    if(sizeHint0 < 0 && sizeHint1 < 0) {
        ret = -1;
    }
    else if(sizeHint0 >= 0 && sizeHint1 >= 0) {
        ret = zfmMin(sizeHint0, sizeHint1);
    }
    else {
        ret = zfmMax(sizeHint0, sizeHint1);
    }
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, zffloat, sizeHintMerge
        , ZFMP_IN(zffloat, sizeHint0)
        , ZFMP_IN(zffloat, sizeHint1)
        ) {
    zffloat ret = 0;
    ZFUILayoutParam::sizeHintMergeT(ret, sizeHint0, sizeHint1);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintMergeT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, sizeHint0)
        , ZFMP_IN(const ZFUISize &, sizeHint1)
        ) {
    ZFUILayoutParam::sizeHintMergeT(ret.width, sizeHint0.width, sizeHint1.width);
    ZFUILayoutParam::sizeHintMergeT(ret.height, sizeHint0.height, sizeHint1.height);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, ZFUISize, sizeHintMerge
        , ZFMP_IN(const ZFUISize &, sizeHint0)
        , ZFMP_IN(const ZFUISize &, sizeHint1)
        ) {
    ZFUISize ret = ZFUISizeZero();
    ZFUILayoutParam::sizeHintMergeT(ret, sizeHint0, sizeHint1);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintOffsetT
        , ZFMP_OUT(zffloat &, ret)
        , ZFMP_IN(zffloat, sizeHint)
        , ZFMP_IN(zffloat, offset)
        ) {
    if(offset >= 0) {
        ret = ((sizeHint >= 0) ? sizeHint + offset : -1);
    }
    else {
        ret = ((sizeHint >= 0) ? zfmMax<zffloat>(0, sizeHint + offset) : (zffloat)-1);
    }
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, zffloat, sizeHintOffset
        , ZFMP_IN(zffloat, sizeHint)
        , ZFMP_IN(zffloat, offset)
        ) {
    zffloat ret = 0;
    ZFUILayoutParam::sizeHintOffsetT(ret, sizeHint, offset);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintOffsetT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(const ZFUISize &, offset)
        ) {
    ZFUILayoutParam::sizeHintOffsetT(ret.width, sizeHint.width, offset.width);
    ZFUILayoutParam::sizeHintOffsetT(ret.height, sizeHint.height, offset.height);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, ZFUISize, sizeHintOffset
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(const ZFUISize &, offset)
        ) {
    ZFUISize ret = ZFUISizeZero();
    ZFUILayoutParam::sizeHintOffsetT(ret, sizeHint, offset);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintOffsetT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(zffloat, offset)
        ) {
    ZFUILayoutParam::sizeHintOffsetT(ret.width, sizeHint.width, offset);
    ZFUILayoutParam::sizeHintOffsetT(ret.height, sizeHint.height, offset);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, ZFUISize, sizeHintOffset
        , ZFMP_IN(const ZFUISize &, sizeHint)
        , ZFMP_IN(zffloat, offset)
        ) {
    ZFUISize ret = ZFUISizeZero();
    ZFUILayoutParam::sizeHintOffsetT(ret, sizeHint, offset);
    return ret;
}

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUILayoutParam, ZFUIView *, ownerView)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUILayoutParam, ZFUIView *, ownerLayout)

ZFUIView *ZFUILayoutParam::ownerLayout(void) {
    if(_ZFP_LP_owner) {
        return _ZFP_LP_owner->parent();
    }
    else {
        return zfnull;
    }
}

zfanyT<ZFUILayoutParam> ZFUILayoutParam::child(
            ZF_IN const zfany &view
            , ZF_IN_OPT zfindex atIndex /* = zfindexMax() */
            ) {
    ZFCoreAssertWithMessageTrim(_ZFP_LP_owner
            , "layout param not attached to view: %s"
            , this
            );
    ZFUIView *tmp = view->classData()->classIsTypeOf(ZFUILayoutParam::ClassData())
        ? zfcast(ZFUILayoutParam *, view)->ownerLayout()
        : zfcast(ZFUIView *, view);
    ZFCoreAssertWithMessageTrim(tmp
            , "invalid view: %s, must be type of ZFUIView or ZFUILayoutParam"
            , view
            );
    return _ZFP_LP_owner->parent()->child(tmp, atIndex);
}
/* ZFTAG_TRICKS: util for chained call to build view tree */
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUILayoutParam, zfanyT<ZFUILayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUILayoutParam, zfanyT<ZFUILayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )

// ============================================================
ZFENUM_DEFINE(ZFUIViewChildLayer)

ZFOBJECT_REGISTER(ZFUIViewMeasureResult)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIViewMeasureResult, ZFUISize, sizeHint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIViewMeasureResult, ZFUISizeParam, sizeParam)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIViewMeasureResult, ZFUISize, measuredSize)

void ZFUIViewMeasureResult::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    zfstringAppend(ret, "%sMeasure(%s, %s)=>%s%s"
            , ZFTOKEN_ZFObjectInfoLeft
            , this->sizeParam
            , this->sizeHint
            , this->measuredSize
            , ZFTOKEN_ZFObjectInfoRight
            );
}

// ============================================================
ZFENUM_DEFINE_FLAGS(ZFUITransform, ZFUITransformFlags)

ZF_NAMESPACE_GLOBAL_END

