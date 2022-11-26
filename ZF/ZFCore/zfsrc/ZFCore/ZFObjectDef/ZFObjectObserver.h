/**
 * @file ZFObjectObserver.h
 * @brief observer types for #ZFObject
 */

#ifndef _ZFI_ZFObjectObserver_h_
#define _ZFI_ZFObjectObserver_h_

#include "ZFCallback.h"
#include "ZFIdMap.h"
#include "ZFArgs.h"

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
    inline void execute(void) const
    {
        ZFArgs zfargs;
        ZFCallback::executeExact<void, const ZFArgs &>(zfargs);
    }
    /** @brief see #ZFListener */
    inline void execute(ZF_IN const ZFArgs &zfargs) const
    {
        ZFCallback::executeExact<void, const ZFArgs &>(zfargs);
    }
    /** @brief see #ZFListener */
    inline void operator () (void) const
    {
        ZFArgs zfargs;
        ZFCallback::executeExact<void, const ZFArgs &>(zfargs);
    }
    /** @brief see #ZFListener */
    inline void operator () (ZF_IN const ZFArgs &zfargs) const
    {
        ZFCallback::executeExact<void, const ZFArgs &>(zfargs);
    }
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFLIB_ZFCore, ZFListener, ZFCallback)

// ============================================================
// ZFObserverHolder
/** @brief see #ZFObject::observerNotify */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFObserverAddParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFObserverAddParam)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(zfidentity, eventId, zfidentityInvalid())

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM(ZFListener, observer)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(ZFObject *, owner, zfnull)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(zfbool, autoRemoveAfterActivate, zffalse)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(ZFLevel, observerLevel, ZFLevelAppNormal)

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN ZFObserverAddParam const &ref) const
    {
        return (this->eventId() == ref.eventId()
            && this->observer() == ref.observer()
            && this->owner() == ref.owner()
            && this->autoRemoveAfterActivate() == ref.autoRemoveAfterActivate()
            && this->observerLevel() == ref.observerLevel()
            );
    }
    inline zfbool operator != (ZF_IN ZFObserverAddParam const &ref) const {return !this->operator == (ref);}
    /** @endcond */
};

zfclassFwd _ZFP_ZFObserverHolderPrivate;
/**
 * @brief holder object for observer logic, see #ZFObject::observerNotify
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFObserverHolder
{
public:
    /** @cond ZFPrivateDoc */
    ZFObserverHolder(void);
    ZFObserverHolder(ZF_IN ZFObserverHolder const &ref);
    ~ZFObserverHolder(void);
    ZFObserverHolder &operator = (ZF_IN ZFObserverHolder const &ref);
    zfbool operator == (ZF_IN ZFObserverHolder const &ref) const;
    inline zfbool operator != (ZF_IN ZFObserverHolder const &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #ZFObject::observerNotify */
    zffinal zfidentity observerAdd(ZF_IN zfidentity eventId,
                                   ZF_IN const ZFListener &observer,
                                   ZF_IN_OPT ZFObject *owner = zfnull,
                                   ZF_IN_OPT zfbool autoRemoveAfterActivate = zffalse,
                                   ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal);
    /** @brief see #ZFObject::observerNotify */
    zffinal zfidentity observerAdd(ZF_IN const ZFObserverAddParam &param);
    /** @brief see #ZFObject::observerNotify */
    zffinal inline zfidentity observerAddForOnce(ZF_IN zfidentity eventId,
                                                 ZF_IN const ZFListener &observer,
                                                 ZF_IN_OPT ZFObject *owner = zfnull,
                                                 ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal)
    {
        return this->observerAdd(eventId, observer, owner, zftrue, observerLevel);
    }
    /** @brief see #ZFObject::observerMoveToFirst */
    zffinal void observerMoveToFirst(ZF_IN zfidentity taskId);
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemove(ZF_IN zfidentity eventId,
                                ZF_IN const ZFListener &callback);
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveByTaskId(ZF_IN zfidentity taskId);
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveByOwner(ZF_IN ZFObject *owner);
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveAll(ZF_IN zfidentity eventId);
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveAll(void);
    /** @brief see #ZFObject::observerNotify */
    zffinal zfbool observerHasAdd(void) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal zfbool observerHasAdd(ZF_IN zfidentity eventId) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal inline void observerNotify(ZF_IN zfidentity eventId,
                                       ZF_IN_OPT ZFObject *param0 = zfnull,
                                       ZF_IN_OPT ZFObject *param1 = zfnull) const
    {
        this->observerNotifyWithCustomSender(this->observerOwner(), eventId, param0, param1);
    }
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerNotifyWithCustomSender(ZF_IN ZFObject *customSender,
                                                ZF_IN zfidentity eventId,
                                                ZF_IN_OPT ZFObject *param0 = zfnull,
                                                ZF_IN_OPT ZFObject *param1 = zfnull) const;

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
     *   observer.observerHasAddStateAttach(EventXXX, &myFlag, Flag_MyEvent0);
     *   observer.observerHasAddStateAttach(EventXXX, &myFlag, Flag_MyEvent1);
     *
     *   if(ZFBitTest(myFlag, Flag_MyEvent0))
     *   {
     *       observer.observerNotify(...);
     *   }
     * @endcode
     */
    zffinal void observerHasAddStateAttach(ZF_IN zfidentity eventId,
                                           ZF_IN_OUT zfuint *flag,
                                           ZF_IN_OUT zfuint flagBit);
    /**
     * @brief see #observerHasAddStateAttach
     */
    zffinal void observerHasAddStateDetach(ZF_IN zfidentity eventId,
                                           ZF_IN_OUT zfuint *flag,
                                           ZF_IN_OUT zfuint flagBit);

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_OUT zfstring &ret) const;
    /**
     * @brief return a short string describe the object
     */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief owner object of this observer holder, or null if none */
    inline ZFObject *observerOwner(void) const
    {
        return this->_observerOwner;
    }
    zffinal void _ZFP_ZFObserverHolder_observerOwner(ZF_IN ZFObject *obj);

private:
    _ZFP_ZFObserverHolderPrivate *d;
    ZFObject *_observerOwner;
};

// ============================================================
/**
 * @brief all event notified by #ZFObject::observerNotify would also be notified to the observers added by this method,
 *   see #ZFObject::observerNotify
 *
 * use only if necessary, which may cause performance issue
 */
extern ZFLIB_ZFCore ZFObserverHolder &ZFGlobalObserver(void);

// ============================================================
// observer
/**
 * @brief see #ZFObject::observerNotify
 *
 * usage:
 * @code
 *   zfclass YourClass ...
 *   {
 *       ZFOBSERVER_EVENT(YourEvent)
 *   };
 *
 *   ZFOBSERVER_EVENT_REGISTER(YourClass, YourEvent)
 * @endcode
 * declare a event for ZFObject's observer logic,
 * declared event name can be accessed by:
 * @code
 *   zfidentity eventId = YourClass::EventYourEvent();
 *   const zfchar *eventName = ZFIdMapNameForId(eventId);
 * @endcode
 * note that subclass may declare a event same as parent,
 * while the final event name is different:\n
 *   ParentClass::EventYourEvent() => "ParentClassEventYourEvent"\n
 *   ChildClass::EventYourEvent() => "ChildClassEventYourEvent"\n
 * \n
 * @note we declare the event id as int types for performance,
 *   it's ensured each event has different event id,
 *   but it's only ensured while app is running,
 *   after relaunching the app,
 *   the event id is not ensured the same,
 *   you should use the name of the event to store or pass between apps,
 *   and you can use #ZFIdMapIdForName or #ZFIdMapNameForId
 *   to convert them easily
 */
#define ZFOBSERVER_EVENT(YourEvent) \
    ZFIDMAP_DETAIL(Event, YourEvent)

/** @brief see #ZFOBSERVER_EVENT */
#define ZFOBSERVER_EVENT_REGISTER(YourClass, YourEvent) \
    ZFIDMAP_REGISTER_DETAIL(YourClass, Event, YourEvent)

// ============================================================
/**
 * @brief declare a observer event in global scope, see #ZFOBSERVER_EVENT
 *
 * usage:
 * @code
 *   // in header files
 *   ZF_NAMESPACE_BEGIN(YourNamespace)
 *   / ** @brief you can add doxygen docs here * /
 *   ZFOBSERVER_EVENT_GLOBAL(ZFLIB_APP, YourEvent)
 *   ZF_NAMESPACE_END(YourNamespace)
 *
 *   ZFOBSERVER_EVENT_GLOBAL_REGISTER(YourNamespace, YourEvent)
 *
 *   // use the event
 *   zfidentity eventId = YourNamespace::EventYourEvent();
 * @endcode
 * unlike #ZFOBSERVER_EVENT, this macro would declare event outside of class scope,
 * typically you should use #ZFOBSERVER_EVENT_GLOBAL which have "ZFGlobalEvent" as namespace
 */
#define ZFOBSERVER_EVENT_GLOBAL(ZFLIB_, YourEvent) \
    ZFIDMAP_GLOBAL_DETAIL(ZFLIB_, Event, YourEvent)

/** @brief see #ZFOBSERVER_EVENT */
#define ZFOBSERVER_EVENT_GLOBAL_REGISTER(YourEvent) \
    ZFIDMAP_GLOBAL_REGISTER_DETAIL(Event, YourEvent)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectObserver_h_

