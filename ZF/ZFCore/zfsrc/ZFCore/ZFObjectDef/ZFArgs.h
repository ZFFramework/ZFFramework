/**
 * @file ZFArgs.h
 * @brief params for #ZFListener
 */

#ifndef _ZFI_ZFArgs_h_
#define _ZFI_ZFArgs_h_

#include "zfany.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFArgs
zfclassFwd _ZFP_ZFArgsPrivate;
/**
 * @brief listener data used by #ZFListener
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFArgs {
public:
    /**
     * @brief event id, may be #zfidentityInvalid
     */
    ZFArgs &eventId(ZF_IN zfidentity const &v);
    /** @brief see #eventId */
    zfidentity const &eventId(void) const;
    /**
     * @brief who notified the listener event, may be null
     * @note the sneder has no auto retain
     */
    ZFArgs &sender(ZF_IN zfany const &v);
    /** @brief see #sender */
    zfany const &sender(void) const;
    /**
     * @brief params, may be null
     * @note the param has no auto retain
     */
    ZFArgs &param0(ZF_IN zfany const &v);
    /** @brief see #param0 */
    zfany const &param0(void) const;
    /**
     * @brief params, may be null
     * @note the param has no auto retain
     */
    ZFArgs &param1(ZF_IN zfany const &v);
    /** @brief see #param1 */
    zfany const &param1(void) const;

    /**
     * @brief the result
     * @note the result would be retained
     */
    zfany const &result(void) const;
    /** @brief see #result */
    ZFArgs const &result(ZF_IN zfany const &result) const;
    /** @brief see #result */
    ZFArgs &result(ZF_IN zfany const &result);

public:
    // ============================================================
    // event filter logic
    /**
     * @brief used to achieve event filter logic
     *
     * to achieve event filter,
     * you may attach an #ZFObserver::observerAdd with higher #ZFLevel,
     * and set #eventFiltered to true,
     * then the event would not be further dispatched
     */
    ZFArgs const &eventFiltered(ZF_IN zfbool eventFiltered) const;
    /** @brief see #eventFiltered */
    ZFArgs &eventFiltered(ZF_IN zfbool eventFiltered);
    /** @brief see #eventFiltered */
    zfbool eventFiltered(void) const;

    // ============================================================
public:
    /**
     * @brief main constructor
     */
    ZFArgs(void);
    /**
     * @brief construct with another data
     */
    ZFArgs(ZF_IN const ZFArgs &ref);

public:
    /** @cond ZFPrivateDoc */
    zffinal ~ZFArgs(void);
    ZFArgs &operator = (ZF_IN const ZFArgs &ref);
    zfbool operator == (ZF_IN const ZFArgs &ref) const;
    inline zfbool operator != (ZF_IN const ZFArgs &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief get a short info of this object */
    zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

private:
    _ZFP_ZFArgsPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFArgs_h_

