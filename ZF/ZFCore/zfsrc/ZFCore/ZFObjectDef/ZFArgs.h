/**
 * @file ZFArgs.h
 * @brief params for #ZFListener
 */

#ifndef _ZFI_ZFArgs_h_
#define _ZFI_ZFArgs_h_

#include "ZFAny.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFArgs
/**
 * @brief listener data used by #ZFListener
 *
 * @note this object only hold the necessary datas as pointer,
 *   without auto retain or release logic
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFArgs {
public:
    /**
     * @brief event id, may be #zfidentityInvalid
     */
    ZFArgs &eventId(ZF_IN zfidentity const &v) {d.eventId = v; return *this;}
    /** @brief see #eventId */
    zfidentity const &eventId(void) const {return d.eventId;}
    /**
     * @brief who notified the listener event, may be null
     * @note the sneder has no auto retain
     */
    ZFArgs &sender(ZF_IN ZFAny const &v) {d.sender = v; return *this;}
    /** @brief see #sender */
    ZFAny const &sender(void) const {return d.sender;}
    /**
     * @brief params, may be null
     * @note the param has no auto retain
     */
    ZFArgs &param0(ZF_IN ZFAny const &v) {d.param0 = v; return *this;}
    /** @brief see #param0 */
    ZFAny const &param0(void) const {return d.param0;}
    /**
     * @brief params, may be null
     * @note the param has no auto retain
     */
    ZFArgs &param1(ZF_IN ZFAny const &v) {d.param1 = v; return *this;}
    /** @brief see #param1 */
    ZFAny const &param1(void) const {return d.param1;}
    /**
     * @brief extra user data passed from #ZFCallback::userData
     * @note the userData stored in #ZFArgs has no auto retain
     */
    ZFArgs &userData(ZF_IN ZFAny const &v) {d.userData = v; return *this;}
    /** @brief see #userData */
    ZFAny const &userData(void) const {return d.userData;}

    /**
     * @brief result, must first enabled by #resultEnable
     * @note the result would be retained
     */
    ZFAny const &result(void) const;
    /** @brief see #result */
    ZFArgs const &result(ZF_IN ZFAny const &result) const;
    /** @brief see #result */
    ZFArgs &result(ZF_IN ZFAny const &result);

    /** @brief see #result */
    ZFArgs &resultEnable(ZF_IN zfbool enable);
    /** @brief see #result */
    zfbool resultEnabled(void) const;

public:
    // ============================================================
    // event filter logic
    /**
     * @brief used to achieve event filter logic
     *
     * to achieve event filter,
     * you may attach an #ZFObserver::observerAdd with higher #ZFLevel,
     * and set #eventFiltered to true,
     * then the event would not be further dispatched\n
     * the #eventFilterEnable must be called to enable filter logic
     */
    inline ZFArgs const &eventFiltered(ZF_IN zfbool eventFiltered) const {if(this->_ZFP_eventFiltered) {*(this->_ZFP_eventFiltered) = eventFiltered;} return *this;}
    /** @brief see #eventFiltered */
    inline ZFArgs &eventFiltered(ZF_IN zfbool eventFiltered) {if(this->_ZFP_eventFiltered) {*(this->_ZFP_eventFiltered) = eventFiltered;} return *this;}
    /** @brief see #eventFiltered */
    inline zfbool eventFiltered(void) const {if(this->_ZFP_eventFiltered) {return *(this->_ZFP_eventFiltered);} else {return zffalse;}}

    /** @brief see #eventFiltered */
    inline ZFArgs &eventFilterEnable(ZF_IN zfbool enable) {this->_ZFP_eventFiltered = enable ? &(d.eventFiltered) : zfnull; return *this;}
    /** @brief see #eventFiltered */
    inline zfbool eventFilterEnabled(void) const {return this->_ZFP_eventFiltered != zfnull;}

    // ============================================================
public:
    /**
     * @brief main constructor
     */
    ZFArgs(void)
    : _ZFP_result(zfnull)
    , _ZFP_eventFiltered(zfnull)
    {
        zfmemset(&d, 0, sizeof(_ZFP_D));
        d.eventId = zfidentityInvalid();
    }
    /**
     * @brief construct with another data
     */
    ZFArgs(ZF_IN const ZFArgs &ref)
    : _ZFP_result(zfnull)
    , _ZFP_eventFiltered(zfnull)
    {
        this->operator = (ref);
    }

public:
    /** @cond ZFPrivateDoc */
    zffinal ~ZFArgs(void);
    ZFArgs &operator = (ZF_IN const ZFArgs &ref) {
        zfmemcpy(&d, &ref.d, sizeof(_ZFP_D));
        this->resultEnable(ref.resultEnabled());
        this->result(ref.result());
        this->eventFilterEnable(ref.eventFilterEnabled());
        return *this;
    }
    zfbool operator == (ZF_IN const ZFArgs &ref) const {
        return (zftrue
                && zfmemcmp(&d, &ref.d, sizeof(_ZFP_D)) == 0
                && this->resultEnabled() == ref.resultEnabled()
                && this->eventFilterEnabled() == ref.eventFilterEnabled()
            );
    }
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
    zfclassPOD _ZFP_D {
    public:
        zfidentity eventId;
        ZFAny sender;
        ZFAny param0;
        ZFAny param1;
        ZFAny userData;
        ZFAny result;
        zfbool eventFiltered;
    };
    _ZFP_D d;
    ZFAny *_ZFP_result;
    zfbool *_ZFP_eventFiltered;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFArgs_h_

