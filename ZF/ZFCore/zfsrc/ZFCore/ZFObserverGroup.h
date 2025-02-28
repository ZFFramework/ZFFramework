/**
 * @file ZFObserverGroup.h
 * @brief helper for observer logic
 */

#ifndef _ZFI_ZFObserverGroup_h_
#define _ZFI_ZFObserverGroup_h_

#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFObserverGroupHolderPrivate;
/** @brief see #ZFObserverGroup */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFObserverGroupHolder {
public:
    /** @brief see #ZFObserverGroup */
    zffinal const ZFObserverGroupHolder &observerAdd(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) const;
    /** @brief see #ZFObserverGroup */
    zffinal const ZFObserverGroupHolder &observerAddForOnce(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) const;
    /** @brief see #ZFObserverGroup */
    zffinal inline const ZFObserverGroupHolder &on(
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
    /** @cond ZFPrivateDoc */
    ZFObserverGroupHolder(void);
    ZFObserverGroupHolder(ZF_IN const ZFObserverGroupHolder &ref);
    ~ZFObserverGroupHolder(void);

    zfbool operator == (ZF_IN const ZFObserverGroupHolder &ref) const {return d == ref.d;}
    zfbool operator != (ZF_IN const ZFObserverGroupHolder &ref) const {return d != ref.d;}
    ZFObserverGroupHolder &operator = (ZF_IN const ZFObserverGroupHolder &ref);
    /** @endcond */

private:
    _ZFP_ZFObserverGroupHolderPrivate *d;;
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

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFObserverGroupHolder, ZFObserverGroupHolder)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFObserverGroupHolder, ZFObserverGroupHolder)
ZFOUTPUT_TYPE(ZFObserverGroupHolder, {v.objectInfoT(s);})

/**
 * @brief util to add multiple observer
 *
 * usage:
 * @code
 *   ZFObject *owner = xxx;
 *   ZFObject *target = xxx;
 *   ZFObserverGroup(owner, target)
 *       ->observerAdd(xxx) // add observer to target
 *       ->observerAdd(xxx)
 *       ;
 *
 *   // when done, remove all observer which attached to owner
 *   // note the owner is compared by instance
 *   ZFObserverGroupRemove(owner);
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFObserverGroupHolder, ZFObserverGroup
        , ZFMP_IN(ZFObject *, owner)
        , ZFMP_IN(const ZFObserver &, target)
        )
/** @brief see #ZFObserverGroup */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFObserverGroupHolder, ZFObserverGroup
        , ZFMP_IN(ZFObject *, owner)
        , ZFMP_IN(ZFObject *, target)
        )

/** @brief see #ZFObserverGroup */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFObserverGroupRemove
        , ZFMP_IN(ZFObject *, owner)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObserverGroup_h_
