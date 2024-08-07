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

// ============================================================
zfclassFwd _ZFP_ZFAniGroupPrivate;
/**
 * @brief animation group
 *
 * serializable data:
 * @code
 *   <ZFAniGroup>
 *       // all child animations
 *       <SomeAni category="child" ... />
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
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
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

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

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
            , ZFMP_IN(zfany, aniTarget)
            )

    /**
     * @brief get child animation's target, or null if not specified
     */
    ZFMETHOD_DECLARE_1(zftimet, childDurationAt
            , ZFMP_IN(zfindex , index)
            )
    /**
     * @brief set child animation's target
     */
    ZFMETHOD_DECLARE_2(void, childDurationAt
            , ZFMP_IN(zfindex , index)
            , ZFMP_IN(zftimet, aniDuration)
            )

    // ============================================================
    // property
public:
    /**
     * @brief whether update all children's aniTarget, true by default
     *
     * when true, child's aniTarget would be set to group's state,
     * unless #childTargetAt was set
     */
    ZFPROPERTY_ASSIGN(zfbool, autoUpdateTarget, zftrue)
    /**
     * @brief whether update all children's aniDuration, true by default
     *
     * when true, child's aniDuration would be set to group's state,
     * unless #childTargetAt was set
     */
    ZFPROPERTY_ASSIGN(zfbool, autoUpdateDuration, zftrue)

public:
    zfoverride
    virtual zftimet aniDurationFixed(void);

    // ============================================================
    // util for chained call
public:
    /** @brief util to add child animation */
    ZFMETHOD_DECLARE_3(void, child
            , ZFMP_IN(const zfstring &, name)
            , ZFMP_IN(ZFObject *, from)
            , ZFMP_IN(ZFObject *, to)
            )
    /** @brief util to add child animation */
    ZFMETHOD_DECLARE_1(void, childImpl
            , ZFMP_IN(const ZFListener &, aniImpl)
            )

public:
    /** @brief util to set last child's property */
    ZFMETHOD_DECLARE_1(void, childTarget
            , ZFMP_IN(ZFObject *, aniTarget)
            )
    /** @brief util to set last child's property */
    ZFMETHOD_DECLARE_1(void, childDelay
            , ZFMP_IN(zftimet, aniDelay)
            )
    /** @brief util to set last child's property */
    ZFMETHOD_DECLARE_1(void, childDuration
            , ZFMP_IN(zftimet, aniDuration)
            )
    /** @brief util to set last child's property */
    ZFMETHOD_DECLARE_1(void, childLoop
            , ZFMP_IN(zfindex, aniLoop)
            )
    /** @brief util to set last child's property */
    ZFMETHOD_DECLARE_1(void, childCurve
            , ZFMP_IN(ZFCurve *, aniCurve)
            )

public:
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, childOnDelayBegin
            , ZFMP_IN(const ZFListener &, cb)
            )
    /** @brief util to attach observer */
    ZFMETHOD_DECLARE_1(void, childOnDelayEnd
            , ZFMP_IN(const ZFListener &, cb)
            )
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
    // start stop
protected:
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);

protected:
    /** @brief see #EventChildAniOnStart */
    virtual inline void childAniOnStart(ZF_IN ZFAnimation *childAni) {
        this->observerNotify(ZFAniGroup::EventChildAniOnStart(), childAni);
    }
    /** @brief see #EventChildAniOnStop */
    virtual inline void childAniOnStop(ZF_IN ZFAnimation *childAni) {
        this->observerNotify(ZFAniGroup::EventChildAniOnStop(), childAni);
    }

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

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

public:
    /**
     * @brief util to add a dummy child animation to wait specified duration
     */
    ZFMETHOD_DECLARE_1(void, wait
            , ZFMP_IN(zftimet, duration)
            )

protected:
    zfoverride
    virtual zfbool _ZFP_ZFAniGroup_queueType(void) {
        return zftrue;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAniGroup_h_

