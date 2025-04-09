/**
 * @file ZFObserverGroup.h
 * @brief helper for observer logic
 */

#ifndef _ZFI_ZFObserverGroup_h_
#define _ZFI_ZFObserverGroup_h_

#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFObserverGroupPrivate;
/**
 * @brief util to add multiple observer
 *
 * usage:
 * @code
 *   ZFObject *owner = xxx;
 *   ZFObject *target = xxx;
 *   ZFObserverGroup(owner, target)
 *       .observerAdd(xxx) // add observer to target
 *       .observerAdd(xxx)
 *       ;
 *
 *   // when done, remove all observer which attached to owner
 *   // note the owner is compared by instance
 *   ZFObserverGroupRemove(owner);
 *
 *   // or, remove by the group holder
 *   ZFObserverGroup holder(owner, target);
 *   holder.observerAdd(xxx);
 *   holder.observerRemove(xxx);
 *   holder.observerRemoveAll();
 * @endcode
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFObserverGroup {
public:
    /** @brief see #ZFObserverGroup */
    zffinal const ZFObserverGroup &observerAdd(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) const;
    /** @brief see #ZFObserverGroup */
    zffinal const ZFObserverGroup &observerAddForOnce(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) const;
    /** @brief see #ZFObserverGroup */
    zffinal void observerRemove(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &callback
            ) const;
    /** @brief see #ZFObserverGroup */
    zffinal void observerRemoveAll(ZF_IN zfidentity eventId) const;
    /** @brief see #ZFObserverGroup */
    zffinal void observerRemoveAll(void) const;
    /** @brief see #ZFObserverGroup */
    zffinal zfbool observerHasAdd(void) const;
    /** @brief see #ZFObserverGroup */
    zffinal zfbool observerHasAdd(ZF_IN zfidentity eventId) const;
    /** @brief see #ZFObserverGroup */
    zffinal void observerNotify(
            ZF_IN zfidentity eventId
            , ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            ) const;
    /** @brief see #ZFObserverGroup */
    zffinal void observerNotifyWithSender(
            ZF_IN ZFObject *customSender
            , ZF_IN zfidentity eventId
            , ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            ) const;

public:
    /** @brief see #ZFObserverGroup */
    zffinal inline const ZFObserverGroup &on(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) const {
        return this->observerAdd(eventId, observer, observerLevel);
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
    /** @brief see #ZFObserverGroup */
    ZFObserverGroup(
            ZF_IN ZFObject *owner
            , ZF_IN const ZFObserver &target
            );
    /** @brief see #ZFObserverGroup */
    ZFObserverGroup(
            ZF_IN ZFObject *owner
            , ZF_IN ZFObject *target
            );

public:
    /** @cond ZFPrivateDoc */
    ZFObserverGroup(void);
    ZFObserverGroup(ZF_IN const ZFObserverGroup &ref);
    ~ZFObserverGroup(void);

    zfbool operator == (ZF_IN const ZFObserverGroup &ref) const {return d == ref.d;}
    zfbool operator != (ZF_IN const ZFObserverGroup &ref) const {return d != ref.d;}
    ZFObserverGroup &operator = (ZF_IN const ZFObserverGroup &ref);
    /** @endcond */

private:
    _ZFP_ZFObserverGroupPrivate *d;;
public:
    void _ZFP_update(
            ZF_IN ZFObject *owner
            , ZF_IN const ZFObserver &target
            );
    void _ZFP_update(
            ZF_IN ZFObject *owner
            , ZF_IN ZFObject *target
            );
};

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFObserverGroup, ZFObserverGroup)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFObserverGroup, ZFObserverGroup)
ZFOUTPUT_TYPE(ZFObserverGroup, {v.objectInfoT(s);})

/** @brief see #ZFObserverGroup */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFObserverGroupRemove
        , ZFMP_IN(ZFObject *, owner)
        )

// ============================================================
/**
 * @brief util to create a global #ZFObserverGroup,
 *   which all of observers would be removed during #ZFFrameworkCleanup as level #ZFLevelAppNormal
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFObserverGroup, ZFGlobalObserverGroup
        , ZFMP_IN(const ZFObserver &, target)
        )
/**
 * @brief util to create a global #ZFObserverGroup,
 *   which all of observers would be removed during #ZFFrameworkCleanup
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFObserverGroup, ZFGlobalObserverGroup
        , ZFMP_IN(ZFObject *, target)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObserverGroup_h_
