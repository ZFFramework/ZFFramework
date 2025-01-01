/**
 * @file ZFObjectObserver.h
 * @brief observer types for #ZFObject
 */

#ifndef _ZFI_ZFObjectObserver_h_
#define _ZFI_ZFObjectObserver_h_

#include "ZFCallback.h"
#include "ZFIdMap.h"
#include "ZFArgs.h"
#include "ZFListenerDeclare.h"

#if ZF_ENV_LAMBDA
#include <functional>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief listener as ZFCallback, mostly used by #ZFObject::observerNotify
 *
 * listeners usually achieved by observer logic in ZFObject,
 * see #ZFObject::observerNotify for more info\n
 * \n
 * what params means, is determined by invoker of the listener,
 * see each listener's comments before use it
 */
ZFCALLBACK_DECLARE_BEGIN(ZFLIB_ZFCore, ZFListener, ZFCallback)
public:
    /** @brief see #ZFListener */
    inline zfbool execute(void) const {
        if(this->valid()) {
            ZFArgs zfargs;
            ZFCallback::executeExact<void, const ZFArgs &>(zfargs);
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
    /** @brief see #ZFListener */
    inline zfbool execute(ZF_IN const ZFArgs &zfargs) const {
        if(this->valid()) {
            ZFCallback::executeExact<void, const ZFArgs &>(zfargs);
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
    /** @brief see #ZFListener */
    inline zfbool operator () (void) const {
        if(this->valid()) {
            ZFArgs zfargs;
            ZFCallback::executeExact<void, const ZFArgs &>(zfargs);
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
    /** @brief see #ZFListener */
    inline zfbool operator () (ZF_IN const ZFArgs &zfargs) const {
        if(this->valid()) {
            ZFCallback::executeExact<void, const ZFArgs &>(zfargs);
            return zftrue;
        }
        else {
            return zffalse;
        }
    }

    /** @cond ZFPrivateDoc */
public:
    ZFListener(
            ZF_IN ZFObject *owner
            , ZF_IN const ZFMethod *f) {
        ZFCallback::operator = (ZFCallbackForMemberMethod(owner, f));
    }
    ZFListener(ZF_IN const ZFMethod *f) {
        ZFCallback::operator = (ZFCallbackForMethod(f));
    }
    ZFListener &operator = (ZF_IN const ZFMethod *f) {
        ZFCallback::operator = (ZFCallbackForMethod(f));
        return *this;
    }
    /** @endcond */

    /** @cond ZFPrivateDoc */
public:
    typedef void (*FUNC_TYPE)(ZF_IN const ZFArgs &zfargs);
    ZFListener(ZF_IN FUNC_TYPE f) {
        FUNC_TYPE fTmp = f;
        ZFLISTENER_1(wrapper
                , FUNC_TYPE, fTmp
                ) {
            fTmp(zfargs);
        } ZFLISTENER_END()
        ZFCallback::operator = ((const ZFCallback &)wrapper);
    }
    ZFListener &operator = (ZF_IN FUNC_TYPE f) {
        FUNC_TYPE fTmp = f;
        ZFLISTENER_1(wrapper
                , FUNC_TYPE, fTmp
                ) {
            fTmp(zfargs);
        } ZFLISTENER_END()
        ZFCallback::operator = ((const ZFCallback &)wrapper);
        return *this;
    }
    /** @endcond */

    /** @cond ZFPrivateDoc */
#if ZF_ENV_LAMBDA
public:
    template<typename T_Func>
    ZFListener(ZF_IN T_Func f) {
        ZFLISTENER_1(wrapper
                , std::function<void(const ZFArgs &)>, f
                ) {
            f(zfargs);
        } ZFLISTENER_END()
        ZFCallback::operator = ((const ZFCallback &)wrapper);
    }
    template<typename T_Func>
    ZFListener &operator = (ZF_IN T_Func f) {
        ZFLISTENER_1(wrapper
                , std::function<void(const ZFArgs &)>, f
                ) {
            f(zfargs);
        } ZFLISTENER_END()
        ZFCallback::operator = ((const ZFCallback &)wrapper);
        return *this;
    }
#endif
    /** @endcond */
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFLIB_ZFCore, ZFListener, ZFCallback)

// ============================================================
// ZFObserver
zfclassFwd _ZFP_ZFObserverPrivate;
/**
 * @brief holder object for observer logic, see #ZFObject::observerNotify
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFObserver {
public:
    /** @cond ZFPrivateDoc */
    ZFObserver(void);
    ZFObserver(ZF_IN ZFObserver const &ref);
    ~ZFObserver(void);
    ZFObserver &operator = (ZF_IN ZFObserver const &ref);
    zfbool operator == (ZF_IN ZFObserver const &ref) const;
    inline zfbool operator != (ZF_IN ZFObserver const &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerAdd(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            );
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerAddForOnce(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            );
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemove(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &callback
            );
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveAll(ZF_IN zfidentity eventId);
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveAll(void);
    /** @brief see #ZFObject::observerNotify */
    zffinal zfbool observerHasAdd(void) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal zfbool observerHasAdd(ZF_IN zfidentity eventId) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal inline void observerNotify(
            ZF_IN zfidentity eventId
            , ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            ) const {
        this->observerNotifyWithSender(this->observerOwner(), eventId, param0, param1);
    }
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerNotifyWithSender(
            ZF_IN ZFObject *customSender
            , ZF_IN zfidentity eventId
            , ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            ) const;

public:
    /** @brief util to #observerAdd */
    zffinal inline void on(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) {
        this->observerAdd(eventId, observer, observerLevel);
    }

public:
    /**
     * @brief attach a state that indicate whether the observer has add,
     *   for performance usage
     *
     * notifying an observer requires much CPU to check and execute,
     * even if no observer has ever added\n
     * to optimize this, you may attach a flag to indicate whether
     * any observer has ever added\n
     * usage:
     * @code
     *   enum {
     *       Flag_MyEvent0,
     *       Flag_MyEvent1,
     *   };
     *   zfuint myFlag = 0;
     *   observer.observerHasAddStateAttach(E_XXX, &myFlag, Flag_MyEvent0);
     *   observer.observerHasAddStateAttach(E_XXX, &myFlag, Flag_MyEvent1);
     *
     *   if(ZFBitTest(myFlag, Flag_MyEvent0)) {
     *       observer.observerNotify(...);
     *   }
     * @endcode
     */
    zffinal void observerHasAddStateAttach(
            ZF_IN zfidentity eventId
            , ZF_IN_OUT zfuint *flag
            , ZF_IN_OUT zfuint flagBit
            );
    /**
     * @brief see #observerHasAddStateAttach
     */
    zffinal void observerHasAddStateDetach(
            ZF_IN zfidentity eventId
            , ZF_IN_OUT zfuint *flag
            , ZF_IN_OUT zfuint flagBit
            );

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_OUT zfstring &ret) const;
    /**
     * @brief return a short string describe the object
     */
    zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief owner object of this observer holder, or null if none */
    zffinal zfany observerOwner(void) const;
    zffinal void _ZFP_ZFObserver_observerOwner(ZF_IN ZFObject *obj);

private:
    _ZFP_ZFObserverPrivate *d;
};

// ============================================================
/**
 * @brief all event notified by #ZFObject::observerNotify would also be notified to the observers added by this method,
 *   see #ZFObject::observerNotify
 *
 * use only if necessary, which may cause performance issue
 */
extern ZFLIB_ZFCore ZFObserver &ZFGlobalObserver(void);

// ============================================================
// observer
/**
 * @brief see #ZFObject::observerNotify
 *
 * usage:
 * @code
 *   zfclass YourClass ... {
 *       ZFEVENT(YourEvent)
 *   };
 *
 *   ZFEVENT_REGISTER(YourClass, YourEvent)
 * @endcode
 * declare a event for ZFObject's observer logic,
 * declared event name can be accessed by:
 * @code
 *   zfidentity eventId = YourClass::E_YourEvent();
 *   zfstring eventName = ZFEventNameForId(eventId);
 * @endcode
 * note that subclass may declare a event same as parent,
 * while the final event name is different:\n
 *   ParentClass::E_YourEvent() => "ParentClass.E_YourEvent"\n
 *   ChildClass::E_YourEvent() => "ChildClass.E_YourEvent"\n
 * \n
 * @note we declare the event id as int types for performance,
 *   it's ensured each event has different event id,
 *   but it's only ensured while app is running,
 *   after relaunching the app,
 *   the event id is not ensured the same,
 *   you should use the name of the event to store or pass between apps,
 *   and you can use #ZFEventIdForName or #ZFEventNameForId
 *   to convert them easily
 */
#define ZFEVENT(YourEvent) \
    ZFIDMAP_DETAIL(E_, YourEvent)

/** @brief see #ZFEVENT */
#define ZFEVENT_INLINE(YourEvent) \
    ZFIDMAP_DETAIL_INLINE(E_, YourEvent)

/** @brief see #ZFEVENT */
#define ZFEVENT_REGISTER(YourClass, YourEvent) \
    ZFIDMAP_REGISTER_DETAIL(YourClass, E_, YourEvent)

// ============================================================
/**
 * @brief declare a observer event in global scope, see #ZFEVENT
 *
 * usage:
 * @code
 *   // in header files
 *   ZF_NAMESPACE_BEGIN(YourNamespace)
 *   / ** @brief you can add doxygen docs here * /
 *   ZFEVENT_GLOBAL(ZFLIB_APP, YourEvent)
 *   ZF_NAMESPACE_END(YourNamespace)
 *
 *   ZFEVENT_GLOBAL_REGISTER(YourNamespace, YourEvent)
 *
 *   // use the event
 *   zfidentity eventId = YourNamespace::E_YourEvent();
 * @endcode
 * unlike #ZFEVENT, this macro would declare event outside of class scope,
 * typically you should use #ZFEVENT_GLOBAL which have "ZFGlobalEvent" as namespace
 */
#define ZFEVENT_GLOBAL(ZFLIB_, YourEvent) \
    ZFIDMAP_GLOBAL_DETAIL(ZFLIB_, E_, YourEvent)

/** @brief see #ZFEVENT */
#define ZFEVENT_GLOBAL_REGISTER(YourEvent) \
    ZFIDMAP_GLOBAL_REGISTER_DETAIL(E_, YourEvent)

// ============================================================
/**
 * @brief get id name from id value, or null if no such id, see #ZFEventIdForName
 */
inline zfstring ZFEventNameForId(ZF_IN zfidentity idValue) {
    return ZFIdMapNameForId(idValue);
}
/**
 * @brief get id value from id name, or #zfidentityInvalid if no such id name
 *
 * the id name should looks like `YourClass.E_YourIdName` or `YourNamespace.E_YourIdName`
 *
 * note: can be found only if:
 * -  declared with #ZFEVENT_INLINE
 * -  registered by #ZFEVENT_REGISTER series
 * -  ever accessed
 */
inline zfidentity ZFEventIdForName(ZF_IN const zfstring &idName) {
    return ZFIdMapIdForName(idName);
}

/**
 * @brief dynamically register your own id
 *
 * assert fail if already registered
 */
inline zfidentity ZFEventDynamicRegister(ZF_IN const zfstring &idName) {
    return ZFIdMapDynamicRegister(idName);
}
/**
 * @brief unregister id that was registered by #ZFEventDynamicRegister
 *
 * do nothing if no such id,
 * assert fail if the id is not dynamically registered
 */
inline void ZFEventDynamicUnregister(ZF_IN zfidentity idValue) {
    ZFIdMapDynamicUnregister(idValue);
}

ZF_NAMESPACE_BEGIN_REGISTER(ZFGlobalEvent, ZF_NAMESPACE_GLOBAL)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectObserver_h_

