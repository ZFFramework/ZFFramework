/**
 * @file ZFUIAutoLayout.h
 * @brief auto layout container with constraint
 */

#ifndef _ZFI_ZFUIAutoLayout_h_
#define _ZFI_ZFUIAutoLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFUIAutoLayout */
ZFENUM_BEGIN(ZFLIB_ZFUIWidget, ZFUIAutoLayoutPos)
    ZFENUM_VALUE(None)
    ZFENUM_VALUE(Width)
    ZFENUM_VALUE(Height)
    ZFENUM_VALUE(Left)
    ZFENUM_VALUE(Top)
    ZFENUM_VALUE(Right)
    ZFENUM_VALUE(Bottom)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(None)
    ZFENUM_VALUE_REGISTER(Width)
    ZFENUM_VALUE_REGISTER(Height)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Bottom)
ZFENUM_END(ZFLIB_ZFUIWidget, ZFUIAutoLayoutPos)

/** @brief see #ZFUIAutoLayout */
zfclassLikePOD ZFLIB_ZFUIWidget ZFUIAutoLayoutRule {
    ZFCORE_PARAM_DECLARE_SELF(ZFUIAutoLayoutRule)

public:
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM(ZFUIAutoLayoutPosEnum, pos, ZFUIAutoLayoutPos::e_None)

private:
    zfautoT<ZFObjectHolder> target_PropV;
public:
    /** @brief see #ZFUIAutoLayout */
    inline ZFUIView *target(void) const {
        return this->target_PropV ? zfcast(ZFUIView *, this->target_PropV->objectHolded()) : zfnull;
    }
    /** @brief see @ref target */
    inline ZFUIAutoLayoutRule &target(ZF_IN ZFUIView * const &value) {
        if(value != zfnull) {
            this->target_PropV = value->objectHolder();
        }
        else {
            this->target_PropV = zfnull;
        }
        return *this;
    }
    ZFUIView *_ZFP_targetForLayout(ZF_IN ZFUIView *parent) const {
        ZFUIView *target = this->target();
        if(target == zfnull || target == parent || target->viewParent() != parent) {
            return parent;
        }
        else {
            return target;
        }
    }

    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM(ZFUIAutoLayoutPosEnum, targetPos, ZFUIAutoLayoutPos::e_None)
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM(zffloat, weight, 0)
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM(zffloat, offset)

public:
    /** @brief remove all contents */
    void removeAll(void) {
        this->pos(ZFUIAutoLayoutPos::e_None);
        this->target(zfnull);
        this->targetPos(ZFUIAutoLayoutPos::e_None);
        this->weight(0);
        this->offset(0);
    }

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFUIAutoLayoutRule &ref) const {
        return (zftrue
                && this->pos() == ref.pos()
                && this->target() == ref.target()
                && this->targetPos() == ref.targetPos()
                && this->weight() == ref.weight()
                && this->offset() == ref.offset()
            );
    }
    inline zfbool operator != (ZF_IN const ZFUIAutoLayoutRule &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */

public:
    zfstring _ZFP_AL_targetId;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFUIWidget, ZFUIAutoLayoutRule, ZFUIAutoLayoutRule)
ZFOUTPUT_TYPE(ZFUIAutoLayoutRule, {v.objectInfoT(s);})

// ============================================================
// ZFUIAutoLayoutParam
zfclassFwd ZFUIAutoLayout;
/** @brief see #ZFUIAutoLayout */
zfclass ZFLIB_ZFUIWidget ZFUIAutoLayoutParam : zfextend ZFUILayoutParam {
    ZFOBJECT_DECLARE(ZFUIAutoLayoutParam, ZFUILayoutParam)

    /** @brief see #ZFUIAutoLayout */
    ZFPROPERTY_ASSIGN(zffloat, biasX, 0.5f)
    /** @brief see #ZFUIAutoLayout */
    ZFPROPERTY_ASSIGN(zffloat, biasY, 0.5f)

    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIAlignFlags, layoutAlign)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIMargin, layoutMargin)

    /** @brief the owner parent view this param attached to */
    ZFMETHOD_DECLARE_0(ZFUIAutoLayout *, ownerParent)

    /** @brief the owner child view this param attached to */
    ZFMETHOD_DECLARE_0(ZFUIView *, ownerChild)

    // ============================================================
    // rule state
public:
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(const ZFUIAutoLayoutRule &, rule
            , ZFMP_IN(ZFUIAutoLayoutPosEnum, pos)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, ruleRemove
            , ZFMP_IN(ZFUIAutoLayoutPosEnum, pos)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, ruleRemoveAll)

    // ============================================================
    // util methods to setup rule
public:
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, width)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, height)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, left)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, top)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, right)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, bottom)

    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toWidth
            , ZFMP_IN(ZFUIView *, target)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toHeight
            , ZFMP_IN(ZFUIView *, target)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toLeft
            , ZFMP_IN(ZFUIView *, target)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toTop
            , ZFMP_IN(ZFUIView *, target)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toRight
            , ZFMP_IN(ZFUIView *, target)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toBottom
            , ZFMP_IN(ZFUIView *, target)
            )

    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, toParentWidth)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, toParentHeight)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, toParentLeft)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, toParentTop)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, toParentRight)
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, toParentBottom)

    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, to
            , ZFMP_IN(ZFUIView *, target)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, toParent)

    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, weight
            , ZFMP_IN(zffloat, weight)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, offset
            , ZFMP_IN(zffloat, offset)
            )

public:
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

protected:
    zfoverride
    virtual ZFSerializablePropertyType serializableOnCheckPropertyType(ZF_IN const ZFProperty *property);
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            );

public:
    zfclassLikePOD _ZFP_Data {
    public:
        ZFUIAutoLayout *ownerParent;
        zfautoT<ZFObjectHolder> ownerChild;
        ZFUIAutoLayoutRule ruleList[ZFUIAutoLayoutPos::ZFEnumCount];
        zfbool posAttached[ZFUIAutoLayoutPos::ZFEnumCount];
        zfbool posReset;
    public:
        _ZFP_Data(void) : ownerParent(zfnull), ruleList(), posAttached(), posReset(zffalse) {}
    };
public:
    _ZFP_Data _ZFP_AL_d;
};

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_rule "rule"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_pos "pos"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target "target"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token ':'
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent "parent"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self "self"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_prev "prev"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_weight "weight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_offset "offset"

// ============================================================
// ZFUIAutoLayout
zfclassFwd _ZFP_ZFUIAutoLayoutPrivate;
/**
 * @brief auto layout with constraint rule
 *
 * for #ZFUIAutoLayout,
 * base class #ZFUILayoutParam's params would take no effect\n
 * \n
 * serializable data:
 * @code
 *   <ZFUIAutoLayoutParam>
 *       <zffloat prop="biasX" />   // optional, 0.5 by default
 *       <zffloat prop="biasY" />   // optional, 0.5 by default
 *       <rule
 *           pos=""                 // required, #ZFUIAutoLayoutPos
 *           target=""              // required, rule target, see below
 *           weight=""              // optional, 0 by default
 *           offset=""              // optional, 0 by default
 *           />
 *       ... // other rules
 *   </ZFUIAutoLayoutParam>
 * @endcode
 *
 * rules:
 * -  `pos` : #ZFUIAutoLayoutPos shows the rule affect which position
 * -  `target` : target of the rule, it splits by two part, `Pos:Target`,
 *   while `Pos` is the value of #ZFUIAutoLayoutPos,
 *   and `Target` can be:
 *   -  `@viewId` while `viewId` is #ZFUIView::viewId,
 *     ref to the sibling with the view id
 *   -  `parent`, ref to parent
 *   -  `self`, ref to self
 *   -  `N` while `N` is a #zfindex value :
 *     ref to the sibling at the index
 * -  `weight` : #zffloat value, `0` by default
 *   -  for #ZFUIAutoLayoutPos::e_Width rule,
 *     `0` or negative value to show the width should be flexible,
 *     otherwise it's the weight value ref to the `target`
 *     (resultWidth = targetWidth * weight)
 *   -  for #ZFUIAutoLayoutPos::e_Left rule,
 *     `0` or negative value to show the rule should be flexible,
 *     otherwise shows the rule are fixed value\n
 *     note, if left or right rule not set,
 *     it's implicitly set to align to parent with flexible rule\n
 *     note, if both left/right and width have flexible rule,
 *     the left/right rule would have higher priority
 * -  `offset` : #zffloat shows offset to `target`
 * -  `biasX` : if both left and right has flexible rule,
 *   align to which side, `0` means totally align to left,
 *   while `1` means align to right
 *
 *
 * during layout, a child has a list of positions (see #ZFUIAutoLayoutPos)
 * to apply rules to finally confirm the child's frame,
 * each position can have 0 or 1 rule\n
 * \n
 * for convenient, #ZFUIAutoLayoutParam has many util methods to build the rule,
 * typical case:
 * @code
 *   parent->childAdd(child)
 *       ->c_widthFill(100)
 *       ->c_left()->c_toParentLeft()
 *       ->c_top()->c_right()->c_to(sibling)
 *       ;
 * @endcode
 *
 * a full list of util methods:
 * -  widthFill(zffloat) / heightFill(zffloat) / sizeFill(zffloat, zffloat) :
 *   explicitly specify the size rule,
 *   same as setting #ZFUIView::layoutParam with sizeHint and fill sizeParam,
 *   would be overrided by other rules if conflict,
 *   set to negative value would reset the rule
 * -  left(), top/right/bottom/width/height, etc :
 *   specify the #ZFUIAutoLayoutRule::pos
 * -  toLeft(target), top/right/bottom/width/height, etc :
 *   specify the #ZFUIAutoLayoutRule::target and #ZFUIAutoLayoutRule::targetPos
 * -  toParentLeft(), top/right/bottom/width/height, etc :
 *   util to toLeft(target) with parent
 * -  to(target) :
 *   util to toLeft(target) with same position of left() series,
 *   can be combined with multiple rules:
 *   @code
 *     parent->childAdd(child)->c_left()->c_top()->c_to(target);
 *     parent->childAdd(child)->c_left()->c_toLeft(target)->c_top()->c_toTop(target);
 *   @endcode
 * -  toParent() :
 *   util to to(target) with parent
 * -  weight(weight) :
 *   specify the #ZFUIAutoLayoutRule::weight
 * -  offset(offset) :
 *   specify the #ZFUIAutoLayoutRule::offset
 */
zfclass ZFLIB_ZFUIWidget ZFUIAutoLayout : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIAutoLayout, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIAutoLayout)

    // ============================================================
    // override ZFUIView
public:
    /** @brief util method for #childAddWithParam */
    ZFMETHOD_DECLARE_2(ZFAnyT<ZFUIAutoLayoutParam>, childAdd
            , ZFMP_IN(ZFUIView *, view)
            , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
            )
protected:
    zfoverride
    virtual void viewChildOnAdd(
            ZF_IN ZFUIView *child
            , ZF_IN ZFUIViewChildLayerEnum layer
            );
    zfoverride
    virtual void viewChildOnRemove(
            ZF_IN ZFUIView *child
            , ZF_IN ZFUIViewChildLayerEnum layer
            );

    zfoverride
    virtual const ZFClass *layoutParamClass(void) {
        return ZFUIAutoLayoutParam::ClassData();
    }

    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFUIAutoLayoutPrivate *d;
};

extern ZFLIB_ZFUIWidget zfbool _ZFP_ZFUIAutoLayout_targetUpdate(
        ZF_IN_OUT ZFUIAutoLayoutRule &rule
        , ZF_IN ZFUIAutoLayout *parent
        , ZF_IN ZFUIView *child
        , ZF_IN const zfchar *targetId
        );
extern ZFLIB_ZFUIWidget zfbool _ZFP_ZFUIAutoLayout_targetIdUpdate(
        ZF_OUT zfstring &targetId
        , ZF_IN const ZFUIAutoLayoutRule &rule
        , ZF_IN ZFUIAutoLayout *parent
        , ZF_IN ZFUIView *child
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAutoLayout_h_

