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
zffinal zfclassLikePOD ZFLIB_ZFCore ZFArgs
{
    ZFCORE_PARAM_DECLARE_SELF(ZFArgs)
public:
    /**
     * @brief event id, may be #zfidentityInvalid
     */
    ZFCORE_PARAM_WITH_INIT(zfidentity, eventId, zfidentityInvalid())
    /**
     * @brief who notified the listener event, may be null
     * @note the sneder has no auto retain
     */
    ZFCORE_PARAM(ZFObject *, sender)
    /**
     * @brief params, may be null
     * @note the param has no auto retain
     */
    ZFCORE_PARAM(ZFObject *, param0)
    /**
     * @brief params, may be null
     * @note the param has no auto retain
     */
    ZFCORE_PARAM(ZFObject *, param1)

    /**
     * @brief result, must first enabled by #resultEnable
     * @note the result would be retained
     */
    ZFObject *const &result(void) const;
    /** @brief see #result */
    ZFArgs const &result(ZF_IN ZFObject * const &result) const;

    /** @brief see #result */
    ZFArgs const &resultEnable(ZF_IN zfbool enable);
    /** @brief see #result */
    zfbool resultEnabled(void) const;

    // ============================================================
    // util
public:
    /** @brief util to #sender */
    ZFAny senderT(void) const {return ZFAny(this->sender());}
    /** @brief util to #param0 */
    ZFAny param0T(void) const {return ZFAny(this->param0());}
    /** @brief util to #param1 */
    ZFAny param1T(void) const {return ZFAny(this->param1());}
    /** @brief util to #result */
    ZFAny resultT(void) const {return ZFAny(this->result());}

public:
    // ============================================================
    // event filter logic
    /**
     * @brief used to achieve event filter logic
     *
     * to achieve event filter,
     * you may attach an #ZFObserverHolder::observerAdd with higher #ZFLevel,
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
    inline ZFArgs &eventFilterEnable(ZF_IN zfbool enable) {this->_ZFP_eventFiltered = enable ? &(this->_ZFP_eventFilteredHolder) : zfnull; return *this;}
    /** @brief see #eventFiltered */
    inline zfbool eventFilterEnabled(void) const {return this->_ZFP_eventFiltered != zfnull;}

    // ============================================================
public:
    /**
     * @brief main constructor
     */
    ZFArgs(void)
    : _ZFP_resultHolder(zfnull)
    , _ZFP_result(zfnull)
    , _ZFP_eventFilteredHolder(zffalse)
    , _ZFP_eventFiltered(zfnull)
    {
    }
    /**
     * @brief construct with sender and params
     */
    ZFArgs(ZF_IN zfidentity eventId,
           ZF_IN ZFObject *sender,
           ZF_IN_OPT ZFObject *param0 = zfnull,
           ZF_IN_OPT ZFObject *param1 = zfnull)
    : _ZFP_resultHolder(zfnull)
    , _ZFP_result(zfnull)
    , _ZFP_eventFilteredHolder(zffalse)
    , _ZFP_eventFiltered(zfnull)
    {
        this->eventId(eventId);
        this->sender(sender);
        this->param0(param0);
        this->param1(param1);
    }
    /**
     * @brief construct with another data
     */
    ZFArgs(ZF_IN const ZFArgs &ref)
    {
        this->operator = (ref);
    }

public:
    /** @cond ZFPrivateDoc */
    zffinal ~ZFArgs(void);
    ZFArgs &operator = (ZF_IN const ZFArgs &ref)
    {
        this->eventId(ref.eventId());
        this->sender(ref.sender());
        this->param0(ref.param0());
        this->param1(ref.param1());
        this->resultEnable(ref.resultEnabled());
        this->result(ref.result());
        this->_ZFP_eventFilteredHolder = ref._ZFP_eventFilteredHolder;
        this->eventFilterEnable(ref.eventFilterEnabled());
        return *this;
    }
    zfbool operator == (ZF_IN const ZFArgs &ref) const
    {
        return (zftrue
                && this->eventId() == ref.eventId()
                && this->sender() == ref.sender()
                && this->param0() == ref.param0()
                && this->param1() == ref.param1()
                && this->_ZFP_resultHolder == ref._ZFP_resultHolder
                && this->resultEnabled() == ref.resultEnabled()
                && this->_ZFP_eventFilteredHolder == ref._ZFP_eventFilteredHolder
                && this->eventFilterEnabled() == ref.eventFilterEnabled()
            );
    }
    inline zfbool operator != (ZF_IN const ZFArgs &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief get a short info of this object */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

private:
    ZFObject *_ZFP_resultHolder;
    ZFObject **_ZFP_result;
    zfbool _ZFP_eventFilteredHolder;
    zfbool *_ZFP_eventFiltered;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFArgs_h_

