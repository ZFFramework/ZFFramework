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
    ZFMETHOD_DECLARE_2(void, ani
            , ZFMP_IN(ZFAnimation *, ani)
            , ZFMP_IN_OPT(zfindex, index, zfindexMax())
            )
    /**
     * @brief child animation count
     */
    ZFMETHOD_DECLARE_0(zfindex, aniCount)
    /**
     * @brief get child animation
     */
    ZFMETHOD_DECLARE_1(ZFAnimation *, aniAt
            , ZFMP_IN(zfindex , index)
            )
    /**
     * @brief remove child animation
     */
    ZFMETHOD_DECLARE_1(void, aniRemoveAt
            , ZFMP_IN(zfindex, index)
            )
    /**
     * @brief remove all child animation
     */
    ZFMETHOD_DECLARE_0(void, aniRemoveAll)

    // ============================================================
    // property
public:
    /**
     * @brief whether update all children's aniTarget, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, autoUpdateTarget, zftrue)
    /**
     * @brief whether update all children's aniDuration, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, autoUpdateDuration, zftrue)

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

