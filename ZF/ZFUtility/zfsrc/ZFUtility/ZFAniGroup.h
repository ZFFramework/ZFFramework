/**
 * @file ZFAniGroup.h
 * @brief animation group
 */

#ifndef _ZFI_ZFAniGroup_h_
#define _ZFI_ZFAniGroup_h_

#include "ZFAnimation.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFAniGroup_child "child"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFAniGroup_childDuration "childDuration"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFAniGroup_childCurve "childCurve"

// ============================================================
zfclassFwd _ZFP_ZFAniGroupPrivate;
/**
 * @brief animation group
 *
 * serializable data:
 * @code
 *   <ZFAniGroup>
 *       // all child animations
 *       <SomeAni category="child"
 *           childDuration="123" // optional
 *           ...
 *           >
 *           <SomeCurve category="childCurve" /> // optional
 *       </SomeAni>
 *       ...
 *   </ZFAniGroup>
 * @endcode
 */
zfclass ZFLIB_ZFUtility ZFAniGroup : zfextend ZFAnimation {
    ZFOBJECT_DECLARE(ZFAniGroup, ZFAnimation)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the child animation
     */
    ZFEVENT(ChildAniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the child animation
     */
    ZFEVENT(ChildAniOnStop)

    // ============================================================
    // serialize
protected:
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

    // ============================================================
    // object
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);

    // ============================================================
    // child animation control
public:
    /**
     * @brief add child animation
     */
    ZFMETHOD_DECLARE_2(void, child
            , ZFMP_IN(ZFAnimation *, ani)
            , ZFMP_IN_OPT(zfindex, index, zfindexMax())
            )
    /**
     * @brief child animation count
     */
    ZFMETHOD_DECLARE_0(zfindex, childCount)
    /**
     * @brief get child animation
     */
    ZFMETHOD_DECLARE_1(zfanyT<ZFAnimation>, childAt
            , ZFMP_IN(zfindex , index)
            )
    /**
     * @brief remove child animation
     */
    ZFMETHOD_DECLARE_1(void, childRemoveAt
            , ZFMP_IN(zfindex, index)
            )
    /**
     * @brief remove all child animation
     */
    ZFMETHOD_DECLARE_0(void, childRemoveAll)

public:
    /**
     * @brief get child animation's target, or null if not specified
     */
    ZFMETHOD_DECLARE_1(zfany, childTargetAt
            , ZFMP_IN(zfindex , index)
            )
    /**
     * @brief set child animation's target
     */
    ZFMETHOD_DECLARE_2(void, childTargetAt
            , ZFMP_IN(zfindex , index)
            , ZFMP_IN(zfany, target)
            )

    /**
     * @brief get child animation's duration, or 0 if not specified
     */
    ZFMETHOD_DECLARE_1(zftimet, childDurationAt
            , ZFMP_IN(zfindex , index)
            )
    /**
     * @brief set child animation's duration
     */
    ZFMETHOD_DECLARE_2(void, childDurationAt
            , ZFMP_IN(zfindex , index)
            , ZFMP_IN(zftimet, duration)
            )

    /**
     * @brief get child animation's curve, or null if not specified
     */
    ZFMETHOD_DECLARE_1(zfautoT<ZFCurve>, childCurveAt
            , ZFMP_IN(zfindex , index)
            )
    /**
     * @brief set child animation's curve
     */
    ZFMETHOD_DECLARE_2(void, childCurveAt
            , ZFMP_IN(zfindex , index)
            , ZFMP_IN(ZFCurve *, curve)
            )

    // ============================================================
    // property
public:
    /**
     * @brief whether update all children's target, true by default
     *
     * when true, child's target would be set to group's state,
     * unless #childTargetAt was set
     */
    ZFPROPERTY_ASSIGN(zfbool, autoUpdateTarget, zftrue)
    /**
     * @brief whether update all children's duration, true by default
     *
     * when true, child's duration would be set to group's state,
     * unless #childDurationAt was set
     */
    ZFPROPERTY_ASSIGN(zfbool, autoUpdateDuration, zftrue)
    /**
     * @brief whether update all children's curve, true by default
     *
     * when true, child's curve would be set to group's state,
     * unless #childCurveAt was set
     */
    ZFPROPERTY_ASSIGN(zfbool, autoUpdateCurve, zftrue)

public:
    zfoverride
    virtual zftimet durationFixed(void);

    // ============================================================
    // util for chained call
public:
    /** @brief util to set last child's property */
    ZFMETHOD_DECLARE_1(void, childTarget
            , ZFMP_IN(ZFObject *, target)
            )
    /** @brief util to set last child's property */
    ZFMETHOD_DECLARE_1(void, childDuration
            , ZFMP_IN(zftimet, duration)
            )
    /** @brief util to set last child's property */
    ZFMETHOD_DECLARE_1(void, childCurve
            , ZFMP_IN(ZFCurve *, curve)
            )

public:
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, childOnStart
            , ZFMP_IN(const ZFListener &, cb)
            )
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, childOnLoop
            , ZFMP_IN(const ZFListener &, cb)
            )
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, childOnStop
            , ZFMP_IN(const ZFListener &, cb)
            )

    // ============================================================
    // util
public:
    /**
     * @brief util to add a dummy child animation to wait specified duration
     */
    ZFMETHOD_DECLARE_1(void, wait
            , ZFMP_IN(zftimet, duration)
            )
    /**
     * @brief util to attach callback to animation group
     *
     * -  when cancelImpl not specified:
     *   call runImpl and stop the step
     * -  when cancelImpl specified:
     *   -  runImpl would be called with #ZFTaskId as #ZFArgs::param0,
     *     runImpl must call #ZFTaskId::stop
     *     to indicates step finish
     *   -  cancelImpl would be called when owner animation group canceled
     */
    ZFMETHOD_DECLARE_2(void, step
            , ZFMP_IN(const ZFListener &, runImpl)
            , ZFMP_IN_OPT(const ZFListener &, cancelImpl, zfnull)
            )

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);

protected:
    /** @brief see #E_ChildAniOnStart */
    virtual inline void childAniOnStart(ZF_IN ZFAnimation *childAni) {
        this->observerNotify(ZFAniGroup::E_ChildAniOnStart(), childAni);
    }
    /** @brief see #E_ChildAniOnStop */
    virtual inline void childAniOnStop(ZF_IN ZFAnimation *childAni) {
        this->observerNotify(ZFAniGroup::E_ChildAniOnStop(), childAni);
    }

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

protected:
    virtual zfbool _ZFP_ZFAniGroup_queueType(void) {
        return zffalse;
    }

private:
    _ZFP_ZFAniGroupPrivate *d;
    friend zfclassFwd _ZFP_ZFAniGroupPrivate;
};

// ============================================================
/**
 * @brief animation queue
 *
 * serializable data:
 * @code
 *   <ZFAniQueue>
 *       // all child animations
 *       <SomeAni category="child" ... />
 *       ...
 *   </ZFAniQueue>
 * @endcode
 */
zfclass ZFLIB_ZFUtility ZFAniQueue : zfextend ZFAniGroup {
    ZFOBJECT_DECLARE(ZFAniQueue, ZFAniGroup)

protected:
    zfoverride
    virtual zfbool _ZFP_ZFAniGroup_queueType(void) {
        return zftrue;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAniGroup_h_

