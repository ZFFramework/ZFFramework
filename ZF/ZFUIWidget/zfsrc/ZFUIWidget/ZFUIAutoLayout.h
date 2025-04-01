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
    ZFENUM_VALUE(Left)
    ZFENUM_VALUE(Top)
    ZFENUM_VALUE(Right)
    ZFENUM_VALUE(Bottom)
    ZFENUM_VALUE(Width)
    ZFENUM_VALUE(Height)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(None)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Bottom)
    ZFENUM_VALUE_REGISTER(Width)
    ZFENUM_VALUE_REGISTER(Height)
ZFENUM_END(ZFLIB_ZFUIWidget, ZFUIAutoLayoutPos)
ZFENUM_REG(ZFLIB_ZFUIWidget, ZFUIAutoLayoutPos)

/** @brief see #ZFUIAutoLayout */
zfclassLikePOD ZFLIB_ZFUIWidget ZFUIAutoLayoutRule {
    ZFCORE_PARAM_DECLARE_SELF(ZFUIAutoLayoutRule)

public:
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM_WEAK(zfanyT<ZFUIView>, target)
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM(ZFUIAutoLayoutPos, targetPos, v_ZFUIAutoLayoutPos::e_None)

public:
    /** @brief true if contains valid rule */
    inline zfbool valid(void) const {
        return this->target() || this->_ZFP_AL_targetId;
    }
    /** @brief remove all contents */
    void removeAll(void) {
        this->target(zfnull);
        this->targetPos(v_ZFUIAutoLayoutPos::e_None);
        this->_ZFP_AL_targetId.removeAll();
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
                && this->target() == ref.target()
                && this->targetPos() == ref.targetPos()
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
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFUIWidget, ZFUIAutoLayoutRule, ZFUIAutoLayoutRule)
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

    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIAlignFlags, align)

    // ============================================================
    // rule state
public:
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(const ZFUIAutoLayoutRule &, rule
            , ZFMP_IN(ZFUIAutoLayoutPos, pos)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, ruleRemove
            , ZFMP_IN(ZFUIAutoLayoutPos, pos)
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

    /** @brief util to setup #left + #top + #right + #bottom */
    ZFMETHOD_DECLARE_0(void, edges)

    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toWidth
            , ZFMP_IN(const zfstring &, viewId)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toHeight
            , ZFMP_IN(const zfstring &, viewId)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toLeft
            , ZFMP_IN(const zfstring &, viewId)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toTop
            , ZFMP_IN(const zfstring &, viewId)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toRight
            , ZFMP_IN(const zfstring &, viewId)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, toBottom
            , ZFMP_IN(const zfstring &, viewId)
            )

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
            , ZFMP_IN(const zfstring &, viewId)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_1(void, to
            , ZFMP_IN(ZFUIView *, target)
            )
    /** @brief see #ZFUIAutoLayout */
    ZFMETHOD_DECLARE_0(void, toParent)

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
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            );

    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable);
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);

public:
    zfclassLikePOD _ZFP_Data {
    public:
        ZFUIAutoLayoutRule ruleList[v_ZFUIAutoLayoutPos::ZFEnumCount];
        zfbool posAttached[v_ZFUIAutoLayoutPos::ZFEnumCount];
        zfbool posReset;
    public:
        _ZFP_Data(void) : ruleList(), posAttached(), posReset(zffalse) {}
    };
public:
    _ZFP_Data _ZFP_AL_d;
public:
    /** @cond ZFPrivateDoc */
    /* ZFTAG_TRICKS: util for chained call to build view tree */
    inline zfanyT<ZFUIAutoLayoutParam> child(
            ZF_IN const zfany &view
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            ) {
        return zfsuper::child(view, atIndex);
    }
    /** @endcond */
};

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_rule "rule"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent "@parent"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self "@self"

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
 *   <ZFUIAutoLayoutParam
 *       rule="Left:Left:@parent|Top:Bottom:SomeViewId|Width:Width:@2" // optional, rules for this lp
 *       >
 *       <zffloat prop="biasX" />   // optional, 0.5 by default
 *       <zffloat prop="biasY" />   // optional, 0.5 by default
 *   </ZFUIAutoLayoutParam>
 * @endcode
 *
 * rule format `<pos>:<targetPos>:<target>[|...]`:
 * -  `pos` / `targetPos` : #ZFUIAutoLayoutPos shows the rule affect which position
 * -  `target` : target of the rule, can be:
 *   -  `viewId` while `viewId` is #ZFUIView::viewId,
 *     ref to the sibling with the view id
 *   -  `@parent`, ref to parent
 *   -  `@self`, ref to self
 *   -  `@N` while `N` is a #zfindex value :
 *     ref to the sibling at the index
 * -  `biasX` : if both left and right rules specified and width param is wrap without fixed rule,
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
 *   parent->child(child)
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
 *   start #ZFUIAutoLayoutParam::rule with specified position
 * -  toLeft(target), top/right/bottom/width/height, etc :
 *   specify the #ZFUIAutoLayoutRule::target and #ZFUIAutoLayoutRule::targetPos
 * -  toParentLeft(), top/right/bottom/width/height, etc :
 *   util to toLeft(target) with parent
 * -  to(target) :
 *   util to toLeft(target) with same position of left() series,
 *   can be combined with multiple rules:
 *   @code
 *     parent->child(child)->c_left()->c_top()->c_to(target);
 *     parent->child(child)->c_left()->c_toLeft(target)->c_top()->c_toTop(target);
 *   @endcode
 * -  toParent() :
 *   util to to(target) with parent
 */
zfclass ZFLIB_ZFUIWidget ZFUIAutoLayout : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIAutoLayout, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIAutoLayout)

    // ============================================================
    // override ZFUIView
public:
    /** @cond ZFPrivateDoc */
    /* ZFTAG_TRICKS: util for chained call to build view tree */
    inline zfanyT<ZFUIAutoLayoutParam> child(
            ZF_IN const zfany &view
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            ) {
        return zfsuper::child(view, atIndex);
    }
    /** @endcond */
protected:
    zfoverride
    virtual void viewChildOnRemove(
            ZF_IN ZFUIView *child
            , ZF_IN ZFUIViewChildLayer layer
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
    virtual void styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable);
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
        );
extern ZFLIB_ZFUIWidget zfbool _ZFP_ZFUIAutoLayout_targetIdUpdate(
        ZF_OUT zfstring &targetId
        , ZF_IN ZFUIAutoLayoutRule &rule
        , ZF_IN ZFUIAutoLayout *parent
        , ZF_IN ZFUIView *child
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAutoLayout_h_

