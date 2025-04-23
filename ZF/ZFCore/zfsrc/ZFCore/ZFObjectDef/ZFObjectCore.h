/**
 * @file ZFObjectCore.h
 * @brief base class of all objects
 */

#ifndef _ZFI_ZFObjectCore_h_
#define _ZFI_ZFObjectCore_h_

#include "zfany.h"
#include "ZFClass.h"
#include "ZFMethod.h"
#include "ZFObjectDeclare.h"
#include "ZFObjectInterface.h"
#include "ZFProperty.h"
#include "ZFObjectMutex.h"
#include "ZFObjectObserver.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief left bracket when output ZFObject's info
 * @see ZFObject::objectInfo
 */
#define ZFTOKEN_ZFObjectInfoLeft "<"
/**
 * @brief right bracket when output ZFObject's info
 * @see ZFObject::objectInfo
 */
#define ZFTOKEN_ZFObjectInfoRight ">"

// ============================================================
/**
 * @brief object instance state for ZFObject
 */
typedef enum {
    ZFObjectInstanceStateOnInit, /**< @brief object is under #ZFObject::objectOnInit */
    ZFObjectInstanceStateOnInitFinish, /**< @brief object is under #ZFObject::objectOnInitFinish */
    ZFObjectInstanceStateIdle, /**< @brief object is constructed successfully */
    ZFObjectInstanceStateOnDeallocPrepare, /**< @brief object is under #ZFObject::objectOnDeallocPrepare */
    ZFObjectInstanceStateOnDealloc, /**< @brief object is under #ZFObject::objectOnDealloc */
} ZFObjectInstanceState;
/** @brief string tokens */
#define ZFTOKEN_ZFObjectInstanceStateOnInit "OnInit"
/** @brief string tokens */
#define ZFTOKEN_ZFObjectInstanceStateOnInitFinish "OnInitFinish"
/** @brief string tokens */
#define ZFTOKEN_ZFObjectInstanceStateIdle "Idle"
/** @brief string tokens */
#define ZFTOKEN_ZFObjectInstanceStateOnDeallocPrepare "OnDeallocPrepare"
/** @brief string tokens */
#define ZFTOKEN_ZFObjectInstanceStateOnDealloc "OnDealloc"

// ============================================================
zfclassNotPOD ZFLIB_ZFCore _ZFP_Obj_Base {
public:
    static const ZFClass *ClassData(void) {
        return zfnull;
    }
    static void _ZFP_Obj_initImpl(ZFClass *cls) {}
    static ZFObject *_ZFP_zfAllocCacheIvk(void) {return zfnull;}
};
template<typename T_ZFObject, int valid>
zfclassNotPOD _ZFP_Obj_AllocCk;
zfclassFwd _ZFP_ZFObjectPrivate;
zfclassFwd _ZFP_I_zfweak;
/**
 * @brief base class of all objects
 *
 * -  ZFObject is a Object-C-like class, using objectRetainCount to manage memory:
 *   @code
 *     ZFObject *myObj = zfAlloc(ZFObject); // objectRetainCount = 1
 *     anotherObj = zfRetain(myObj);        // objectRetainCount = 2
 *     zfRelease(myObj);                    // objectRetainCount = 1
 *     zfRelease(anotherObject);            // objectRetainCount = 0, delete
 *   @endcode
 *   you should always use #zfAlloc to create a ZFObject or its subclass,
 *   and use #zfRetain and #zfRelease to manage memory
 *   @see zfRetain, zfRelease, ZFAutoReleasePool
 *
 *   ADVANCED:\n
 *   we are trying hard to simulate the autorelease in Object-C world:
 *   -  #zfautoRelease:
 *     the most powerful and most similar one to autorelease in Object-C,
 *     however it depends on ZFThread,
 *     so most of functions won't use this to return a autoreleased object,
 *     see #zfautoRelease for more info
 *   -  #ZFAutoReleasePool:
 *     similar to NSAutoReleasePool, release after pool release,
 *     zfautoRelease depends on it, and all ZFThread has a pool internally
 *   -  #zflineRelease:
 *     release after code line end, usually used for temporary object
 *   -  #zflineAlloc:
 *     same as zflineRelease(zfAlloc(...)) defined for short
 *   -  #zfblockedRelease:
 *     release after nearest code block
 *   -  #zfobj:
 *     declare a ZFObject which looks like allocated in stack,
 *     would be released after nearest code block
 *   -  #zfauto:
 *     declare a holder object which would release its content ZFObject when destroyed,
 *     much like the auto_ptr in C++ world
 * -  when inherit from ZFObject,
 *   you must add ZFOBJECT_DECLARE or ZFOBJECT_DECLARE_ABSTRACT to your subclass,
 *   and use zfextend:
 *   @code
 *     zfclass YourClassChild : zfextend YourClassBase {
 *         ZFOBJECT_DECLARE(YourClassChild, YourClassBase)
 *         // or ZFOBJECT_DECLARE_ABSTRACT if YourClassChild is abstract class
 *     };
 *   @endcode
 *   and, no multiple inheritance is allowed while using ZFObject\n
 *   additionally, you should add #ZFOBJECT_REGISTER if you want the ZFClass map function
 *   @see ZFOBJECT_DECLARE, zfextend
 * -  nested inner class or interface can be declared,
 *   however, (ZFTAG_LIMITATION) you must make sure outer class is declared
 *   @code
 *     zfclass YourOuterClass : zfextend ZFObject {
 *         ZFOBJECT_DECLARE(YourOuterClass, ZFObject)
 *
 *         zfclass YourInnerClass : zfextend ZFObject {
 *             ZFOBJECT_DECLARE(YourInnerClass, ZFObject, YourOuterClass) // <= note for the outer class
 *         };
 *         zfinterface YourInnerInterface : zfextend ZFInterface {
 *             ZFINTERFACE_DECLARE(YourInnerClass, ZFInterface, YourOuterClass) // <= note for the outer class
 *         };
 *     };
 *   @endcode
 * -  when inherit from ZFObject,
 *   member method is recommended declared as virtual method,
 *   unless you do want to prevent subclass to override,
 *   at this case, use #zffinal to mark the method is final and subclass should not try to override or shadow it
 * -  ZFObject use #objectOnInit and #objectOnDealloc to init and destory object,
 *   instead of constructors and destructors,
 *   for how to use it in subclass, see #objectOnInit
 * -  while override member method of ZFObject type,
 *   you should use zfsuper to call super's method:
 *   @code
 *     zfclass YourClass : zfextend Parent {
 *         ZFOBJECT_DECLARE(YourClass, Parent)
 *     public:
 *         zfoverride
 *         virtual void funcInParent(void) {
 *             // override and call super's method by zfsuper
 *             zfsuper::funcInParent();
 *         }
 *     };
 *   @endcode
 *   if overrided method is declared in super interface,
 *   you should use zfsuperI:
 *   @code
 *     zfclass Parent : zfextend ZFObject, zfimplement ParentInterface0 {
 *         ZFOBJECT_DECLARE(Parent, ZFObject)
 *         ZFIMPLEMENT_DECLARE(ParentInterface0)
 *     public:
 *         zfoverride
 *         virtual void funcInParentInterface0(void) {
 *             // would call ParentInterface0::funcInParentInterface0
 *             zfsuperI(ParentInterface0)::funcInParentInterface0();
 *         }
 *     };
 *     zfclass YourClass : zfextend Parent, zfimplement ParentInterface1 {
 *         ZFOBJECT_DECLARE(YourClass, Parent)
 *         ZFIMPLEMENT_DECLARE(ParentInterface1)
 *     public:
 *         zfoverride
 *         virtual void funcInParentInterface0(void) {
 *             // would call Parent::funcInParentInterface0
 *             zfsuperI(ParentInterface0)::funcInParentInterface0();
 *         }
 *         zfoverride
 *         virtual void funcInParentInterface1(void) {
 *             // would call ParentInterface1::funcInParentInterface1
 *             zfsuperI(ParentInterface1)::funcInParentInterface1();
 *         }
 *     };
 *   @endcode
 * -  (ZFTAG_LIMITATION) due to some limitation, you should always use ZFObject as a pointer
 *   but without const qualifier
 * -  you can access class info by:
 *   @code
 *     YourClass::ClassData();
 *     yourClassInstance->classData();
 *     ZFClass::classForName(name);
 *   @endcode
 * -  ZFObject is synchronizable object, you can make it mutable by:
 *   @code
 *     {
 *         zfsynchronize(obj)
 *         // mutable operation
 *     }
 *   @endcode
 *   for advanced mutable operation, use ZFMutex instead\n
 *   for global mutable operation, use #ZFCoreMutexLock instead
 *
 * \n
 * ADVANCED:\n
 * we use macros everywhere to achieve goals,
 * there are many types and functions and whatever-it-is,
 * which are generated by macros and are private,
 * you should never ever access it in your code\n
 * here's a list or them:
 * -  "_ZFP_*": shows it's private (maybe types or functions or anything)
 * -  "_ZFI_*": conditional code wrappers
 * -  "_ZFT_*": conditional type defines,
 *   must not be accessed by codes,
 *   but can be modified by adding precompile flags
 *   (usually have no need, and modify at your own risk)
 */
zfclass ZFLIB_ZFCore ZFObject {
    _ZFP_ZFOBJECT_DECLARE(ZFObject, _ZFP_Obj_Base)
    _ZFP_ZFOBJECT_DECLARE_OBJECT(ZFObject, _ZFP_Obj_Base, _ZFP_Obj_Base)

public:
    /** @cond ZFPrivateDoc */
    enum {_ZFP_ZFObjectCanAllocPublic = 1};
    static void _ZFP_Obj_initImpl(ZFClass *cls) {}
    static ZFObject *_ZFP_zfAllocCacheIvk(void) {return zfnull;}
    static inline void _ZFP_zfAllocCacheRelease(ZF_IN ZFObject *_obj) {}
    virtual inline void _ZFP_ObjI_onInitIvk(void) {}
    virtual inline void _ZFP_ObjI_onDeallocIvk(void) {}
    /** @endcond */

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when object allocated (before #objectOnInitFinish\n
     * this event is only designed for convenient and for debug use only,
     * usually you should use #ZFClass::instanceObserverAdd
     */
    ZFEVENT(ObjectBeforeAlloc)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when object allocated (after #objectOnInitFinish\n
     * this event is only designed for convenient and for debug use only,
     * usually you should use #ZFClass::instanceObserverAdd
     */
    ZFEVENT(ObjectAfterAlloc)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified before object dealloc\n
     * note: the object's retain count should be 1 when this event is notified,
     * it's safe to retain the object during this event,
     * but it's your responsibility to ensure logic valid,
     * after notified this event, all observer of this event would be removed,
     * so that it's safe to release the object again to finally destroy the object
     */
    ZFEVENT(ObjectBeforeDealloc)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is #v_ZFProperty that value changes\n
     * param1 is the old property value depends on these conditions:
     * -  holds #ZFNull when first time accessed (by setter or getter)
     * -  holds the value itself, for retain property
     * -  holds a #ZFTypeIdWrapper to the value, for assign property
     *
     * called when first time accessed, and each time setter is called
     */
    ZFEVENT(ObjectPropertyValueOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is #v_ZFProperty\n
     * called when #ZFPropertyValueReset (before value actually reset)\n
     * note, won't be called when object dealloc
     */
    ZFEVENT(ObjectPropertyValueOnReset)

public:
    /**
     * @brief return the object's retain count
     * @see zfRetain, zfRelease
     */
    zffinal inline zfindex objectRetainCount(void) {
        return _objectRetainCount;
    }

    zffinal _ZFP_I_zfweak *_ZFP_ZFObject_weakHolder(void);

    /** @brief see #objectInfoOfInstance */
    zffinal void objectInfoOfInstanceT(ZF_IN_OUT zfstring &ret) {
        ret += this->classData()->classNameFull();
        ret += "(";
        zfsFromPointerT(ret, this);
        ret += ")";
    }
    /**
     * @brief return a short string describe the object instance
     *
     * may looks like this:
     *   ClassName(0x123456)
     * @see objectInfo
     */
    zffinal inline zfstring objectInfoOfInstance(void) {
        zfstring ret;
        this->objectInfoOfInstanceT(ret);
        return ret;
    }

    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret);
    /**
     * @brief return object info
     *
     * subclass or impl may override or supply dynamic registered method
     * with `void objectInfoImpl(zfstring &ret)` to achieve custom logic,
     * or override #objectInfoImplAppend for chained impl
     */
    zffinal inline zfstring objectInfo(void) {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
    zffinal inline void _ZFP_ZFObject_objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        this->objectInfoImpl(ret);
    }

protected:
    /** @brief see #objectInfo */
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret += ZFTOKEN_ZFObjectInfoLeft;
        this->objectInfoImplAppend(ret);
        ret += ZFTOKEN_ZFObjectInfoRight;
    }
    /** @brief see #objectInfo */
    virtual inline void objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
        this->objectInfoOfInstanceT(ret);
    }

public:
    /**
     * @brief get hash of this object
     *
     * by default, return a hash value of the instance pointer
     *
     * @warning if you override this method, you must ensure
     *   two objects have same hash if they are regarded as same
     *   (i.e. #objectCompare return #ZFCompareEqual)
     * @note this method may or may not be called frequently,
     *   (for example, store as key of #ZFHashMap may cause frequently call),
     *   you should always try to make the implementation
     *   have good performance
     * @note for dynamic registered class (#ZFClassDynamicRegister),
     *   you may supply a reflectable method with `zfidentity objectHashImpl(void)`,
     *   to achieve custom logic
     */
    zffinal zfidentity objectHash(void);
    /**
     * @brief compare with anotherObj
     *
     * by default, compare by instance pointer\n
     * it's recommended to override this method only for objects that only contains value types,
     * for example, #v_zfstring, #v_zfindex, etc,
     * for other complex type,
     * it's recommended to override #objectCompareValue instead
     *
     * @warning if your override #objectCompare,
     *   you must also override #objectCompareValue,
     *   if two objects are #objectCompare as same,
     *   then #objectCompareValue must also result to same
     * @note for dynamic registered class (#ZFClassDynamicRegister),
     *   you may supply a reflectable method with `ZFCompareResult objectCompare(ZFObject *)`,
     *   to achieve custom logic
     */
    zffinal ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);
    /**
     * @brief explicitly compare object by logical value, see #objectCompare
     *
     * @note for dynamic registered class (#ZFClassDynamicRegister),
     *   you may supply a reflectable method with `ZFCompareResult objectCompareValue(ZFObject *)`,
     *   to achieve custom logic
     */
    zffinal ZFCompareResult objectCompareValue(ZF_IN ZFObject *anotherObj);

    zffinal zfidentity _ZFP_ZFObject_objectHashImpl(void) {
        return this->objectHashImpl();
    }
    zffinal ZFCompareResult _ZFP_ZFObject_objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        return this->objectCompareImpl(anotherObj);
    }
    zffinal ZFCompareResult _ZFP_ZFObject_objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
        return this->objectCompareValueImpl(anotherObj);
    }

protected:
    /** @brief see #objectHash */
    virtual inline zfidentity objectHashImpl(void) {
        return zfidentityCalcPointer(this);
    }
    /** @brief see #objectCompare */
    virtual inline ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        return ((this == anotherObj) ? ZFCompareEqual : ZFCompareUncomparable);
    }
    /** @brief see #objectCompareValue */
    virtual inline ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
        return this->objectCompare(anotherObj);
    }

public:
    /* ZFMETHOD_MAX_PARAM */
    /**
     * @brief util method to perform #ZFDI_invoke,
     *   do nothing if fail
     */
    virtual zfauto invoke(
            ZF_IN const zfstring &methodName
            );
    /**
     * @brief util method to perform #ZFDI_invoke,
     *   do nothing if fail
     */
    virtual zfauto invoke(
            ZF_IN const zfstring &methodName
            , ZF_IN ZFObject *param0
            , ZF_IN_OPT ZFObject *param1 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param2 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param3 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param4 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param5 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param6 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param7 = ZFMP_DEF()
            );
    /**
     * @brief util method to perform #ZFDI_invoke,
     *   do nothing if fail
     */
    virtual zfbool invokeT(
            ZF_OUT zfauto &ret
            , ZF_OUT_OPT zfstring *errorHint
            , ZF_IN const zfstring &methodName
            , ZF_IN_OPT ZFObject *param0 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param1 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param2 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param3 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param4 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param5 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param6 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param7 = ZFMP_DEF()
            );
    /**
     * @brief util method to perform #ZFDI_invoke,
     *   do nothing if fail
     */
    virtual zfauto invokeDetail(
            ZF_IN const zfstring &methodName
            , ZF_IN const ZFCoreArray<zfauto> &params
            , ZF_OUT_OPT zfbool *success = zfnull
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );

public:
    /**
     * @brief see #objectTag, true if this object has tag,
     *   and tag can be checked by #objectTagGetAllKeyValue
     */
    zffinal zfbool objectTagExist(void);
    /**
     * @brief used to hold a object for app's use, auto retained
     *
     * replace if existing, remove if tag is null
     */
    zffinal void objectTag(
            ZF_IN const zfstring &key
            , ZF_IN ZFObject *tag
            );
    /**
     * @brief see #objectTag
     */
    zffinal zfany objectTag(ZF_IN const zfstring &key);
    /**
     * @brief get all key value
     */
    zffinal void objectTagGetAllKeyValue(
            ZF_IN_OUT ZFCoreArray<zfstring> &allKey
            , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
            );
    /**
     * @brief remove tag, same as set tag to null
     */
    zffinal inline void objectTagRemove(ZF_IN const zfstring &key) {
        this->objectTag(key, zfnull);
    }
    /**
     * @brief remove tag, return removed tag or null if not exist
     */
    zffinal zfauto objectTagRemoveAndGet(ZF_IN const zfstring &key);
    /**
     * @brief remove all tag
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     * @note this method would be called during #E_ObjectBeforeDealloc and #objectOnDeallocPrepare
     */
    zffinal void objectTagRemoveAll(void);

public:
    /**
     * @brief see #observerNotify
     */
    zffinal void observerAdd(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            );
    /**
     * @brief see #observerNotify
     */
    zffinal void observerAddForOnce(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            );
    /**
     * @brief see #observerNotify
     */
    zffinal void observerRemove(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &callback
            );
    /**
     * @brief see #observerNotify
     *
     * @note it's unsafe to remove all observer manually,
     *   which may break unrelated modules' observer logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void observerRemoveAll(ZF_IN zfidentity eventId);
    /**
     * @brief see #observerNotify
     *
     * @note it's unsafe to remove all observer manually,
     *   which may break unrelated modules' observer logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void observerRemoveAll(void);
    /**
     * @brief true if any observer has been added
     */
    zffinal zfbool observerHasAdd(void);
    /**
     * @brief true if any observer with eventId has been added
     */
    zffinal zfbool observerHasAdd(ZF_IN zfidentity eventId);
    /**
     * @brief notify the observer with eventId
     *
     * the #ZFListener would be executed\n
     * it's ensured the first added observer would be executed first\n
     * use #ZFGlobalObserver for global observer\n
     * for instance observer:
     * @code
     *   zfclass YourClass {
     *       // declare your event here,
     *       // which would generate a method:
     *       //   static zfidentity E_YourEvent(void)
     *       ZFEVENT(YourEvent)
     *   };
     *   // notify using declared event
     *   yourClass->observerNotify(YourClass::E_YourEvent(), params...);
     * @endcode
     * @note observers would be compared
     *   by #ZFCallback::objectCompareByInstance when add or remove
     *
     * \n
     * to pass extra params to the callback,
     * see #ZFLISTENER for more info
     */
    zffinal void observerNotify(
            ZF_IN zfidentity eventId
            , ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            );
    /** @brief see #observerNotify */
    zffinal void observerNotifyWithSender(
            ZF_IN ZFObject *customSender
            , ZF_IN zfidentity eventId
            , ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            );
    /** @brief see #observerNotify */
    zffinal void observerNotifyReversely(
            ZF_IN zfidentity eventId
            , ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            );
    /** @brief see #observerNotify */
    zffinal void observerNotifyReverselyWithSender(
            ZF_IN ZFObject *customSender
            , ZF_IN zfidentity eventId
            , ZF_IN_OPT ZFObject *param0 = zfnull
            , ZF_IN_OPT ZFObject *param1 = zfnull
            );

    /**
     * @brief access the internal observer holder
     *
     * usually for internal use only\n
     * calling owner object's #observerAdd series is ensured
     * same with calling the observerHolder's proper method
     */
    zffinal ZFObserver &observerHolder(void);

public:
    /**
     * @brief util to #observerAdd
     *
     * note, this method would recursively search event name by #ZFIdMapIdForName
     * from entire inherit tree,
     * used for convenient only (in script for example),
     * take care of performance\n
     * example:
     * @code
     *   ZFUIButtonBasic *obj = xxx;
     *   obj->on("ButtonOnClick", xxx);
     *   // these event would be searched, until proper event found:
     *   // 1. ZFUIButtonBasic.E_ButtonOnClick : not found
     *   // 2. ZFUIButton.E_ButtonOnClick : found
     *
     *   // or, use exact event name
     *   obj->on("ZFUIButton.E_ButtonOnClick", xxx);
     * @endcode
     */
    zffinal void on(
            ZF_IN const zfstring &eventName
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            );
    /** @brief util to #observerAdd */
    zffinal inline void on(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) {
        this->observerAdd(eventId, observer, observerLevel);
    }
    /**
     * @brief util for script to achieve chained call
     */
    zffinal inline void onInit(ZF_IN const ZFListener &impl) {
        impl.execute(ZFArgs().sender(this));
    }
    /**
     * @brief util for script to achieve chained call
     */
    zffinal inline void onDealloc(ZF_IN const ZFListener &impl) {
        this->observerAddForOnce(zfself::E_ObjectBeforeDealloc(), impl);
    }

protected:
    /**
     * @brief called when add first observer
     */
    virtual void observerOnAdd(ZF_IN zfidentity eventId);
    /**
     * @brief called when remove last observer
     */
    virtual void observerOnRemove(ZF_IN zfidentity eventId);
    /**
     * @brief notified when #observerNotify
     *
     * ensured called before any other registered observer
     */
    virtual inline void observerOnEvent(ZF_IN const ZFArgs &zfargs) {
    }

public:
    void *_ZFP_ZFObjectMutexImpl(void);
    void _ZFP_ZFObjectLock(void);
    void _ZFP_ZFObjectUnlock(void);
    zfbool _ZFP_ZFObjectTryLock(void);

    void _ZFP_ZFObject_objectOnInit(void);
    void _ZFP_ZFObject_objectOnInitFinish(void);
    void _ZFP_ZFObject_objectOnInitFromCache(void);
    void _ZFP_ZFObjectCheckRelease(void);

protected:
    /**
     * @brief override this to init your object
     *
     * subclass must call superclass's objectOnInit before any other code if override\n
     * you may also declare objectOnInit with params like this:
     * @code
     *   zfclass Parent : ... {
     *       ...
     *   protected:
     *       virtual void objectOnInit(void) {...};
     *   };
     *   zfclass Child : Parent {
     *   protected:
     *       // override all parent's objectOnInit is also required,
     *       // because of function hiding of C++
     *       zfoverride
     *       virtual void objectOnInit(void) {
     *           zfsuper::objectOnInit();
     *       }
     *       // custom init entry
     *       virtual void objectOnInit(Params...) {
     *           // your extra init steps
     *           ...
     *       }
     *   };
     * @endcode
     * @warning objectOnInit and objectOnDealloc is called as a virtual function,
     *   take good care of other virtual function call,
     *   use zfself::func() instead of this->func() if necessary,
     *   or use #objectOnInitFinish/#objectOnDeallocPrepare,
     *   or declare your own constructor with #ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR
     * @note (ZFTAG_LIMITATION) due to limitations of C++,
     *   if your parent type declared objectOnInit with different params,
     *   and your child type want to override part of those objectOnInit,
     *   then your child type must also override all objectOnInit that parent declared,
     *   otherwise, some may be hidden
     * @note for objects that designed not to be allocated by user,
     *   you should use #ZFOBJECT_PRIVATE_ALLOC,
     *   typically usage:
     *   @code
     *     zfclass MyObject : zfextend ZFObject {
     *         ZFOBJECT_DECLARE(MyObject, ZFObject)
     *         ZFOBJECT_PRIVATE_ALLOC("should be created by MyObject::instanceForXxx only")
     *
     *     public:
     *         static zfauto instanceForXxx(xxx) {
     *             // can only be allocated by reflection
     *             return zfself::ClassData()->newInstance();
     *         }
     *     };
     *     static void func(void) {
     *         // MyObject *obj = zfAlloc(MyObject); // would compile error
     *         zfauto obj = MyObject::instanceForXxx(xxx); // OK
     *     }
     *   @endcode
     */
    virtual void objectOnInit(void);
    /**
     * @brief called after #objectOnInit, safe to call virtual functions here
     *
     * usually used to initialize independent resources other than self's internal resources
     */
    virtual inline void objectOnInitFinish(void) {
    }
    /**
     * @brief called before #objectOnDealloc, safe to call virtual functions here
     *
     * usually used to cleanup resources attached to this object other than self's internal resources
     */
    virtual inline void objectOnDeallocPrepare(void) {
        this->objectTagRemoveAll();
        this->observerRemoveAll();
    }
    /**
     * @brief override this to destroy your object
     *
     * subclass must call superclass's objectOnDealloc after any other code if override\n
     * see #objectOnInit for more info
     */
    virtual void objectOnDealloc(void);

    /**
     * @brief called to retain object
     *
     * subclass must call superclass's objectOnRetain before any other code if override\n
     * usually you should not override this method
     */
    virtual inline void objectOnRetain(void) {
        ZFCoreAssertWithMessageTrim(_objectRetainCount > 0,
            "[ZFObject] retain an object while deallocating: %s", this->objectInfoOfInstance());
        ++_objectRetainCount;
    }
    /**
     * @brief called to release object
     *
     * subclass must call superclass's onRelase after any other code if override\n
     * usually you should not override this method
     */
    virtual inline void objectOnRelease(void) {
        --_objectRetainCount;
    }

public:
    /**
     * @brief object instance's state
     */
    zffinal zfbool objectInstanceStateCheck(ZF_IN ZFObjectInstanceState state);
    /** @brief util method to check whether #ZFObjectInstanceStateOnInit or #ZFObjectInstanceStateOnInitFinish running */
    zffinal zfbool objectInitRunning(void);
    /** @brief util method to check whether #ZFObjectInstanceStateOnDeallocPrepare or #ZFObjectInstanceStateOnDealloc running */
    zffinal zfbool objectDeallocRunning(void);

public:
    /**
     * @brief whether this object is internal object or its class is #ZFClass::classIsInternal
     *
     * if #objectIsInternalPrivate, then it is also #objectIsInternal
     */
    virtual zfbool objectIsInternal(void);
    /**
     * @brief whether this object is internal private object or its class is #ZFClass::classIsInternalPrivate
     *
     * if #objectIsInternalPrivate, then it is also #objectIsInternal
     */
    virtual zfbool objectIsInternalPrivate(void);

public:
    zffinal void _ZFP_ZFObject_objectPropertyValueAttach(ZF_IN const ZFProperty *property);
    zffinal void _ZFP_ZFObject_objectPropertyValueDetach(ZF_IN const ZFProperty *property);
    zffinal inline void _ZFP_ZFObject_objectPropertyValueOnUpdate(
            ZF_IN const ZFProperty *property
            , ZF_IN const void *oldValue
            ) {
        this->objectPropertyValueOnUpdate(property, oldValue);
    }
    zffinal inline void _ZFP_ZFObject_objectPropertyValueOnReset(
            ZF_IN const ZFProperty *property
            ) {
        this->objectPropertyValueOnReset(property);
    }
protected:
    /**
     * @brief see #E_ObjectPropertyValueOnUpdate
     *
     * oldValue should be:
     * -  zfnull, if property first time accessed (by getter or setter)
     * -  addr of zfauto, for retain property
     * -  addr of raw value, for assign property
     */
    virtual void objectPropertyValueOnUpdate(
            ZF_IN const ZFProperty *property
            , ZF_IN const void *oldValue
            );
    /** @brief see #E_ObjectPropertyValueOnReset */
    virtual void objectPropertyValueOnReset(
            ZF_IN const ZFProperty *property
            );

    // ============================================================
public:
    zffinal void _ZFP_ZFObject_zfAllocCacheRelease(ZF_IN _ZFP_zfAllocCacheReleaseCallback callback);
    zffinal _ZFP_zfAllocCacheReleaseCallback _ZFP_ZFObject_zfAllocCacheRelease(void);
    zffinal ZFObject *_ZFP_ZFObject_ZFImplementDynamicOwnerOrSelf(void);
    zffinal ZFObject *_ZFP_ZFObject_ZFImplementDynamicHolder(ZF_IN const ZFClass *clsToImplement);

private:
    friend zfclassFwd ZFClass;
    friend zfclassFwd ZFObserver;
    friend zfclassFwd _ZFP_ZFObserverPrivate;
    template<typename T_ZFObject, int valid>
    friend zfclassFwd _ZFP_Obj_AllocCk;
    friend void _ZFP_zfRetainAction(ZF_IN ZFObject *obj);
    friend void _ZFP_zfReleaseAction(ZF_IN ZFObject *obj);
public:
    const ZFClass *_ZFP_ZFObject_classDynamic;
private:
    _ZFP_ZFObjectPrivate *d;
    zfuint _objectRetainCount;
    zfuint _stateFlags;
protected:
    /** @cond ZFPrivateDoc */
    ZFObject(void)
    : _ZFP_ZFObject_classDynamic(zfnull)
    , d(zfnull)
    , _objectRetainCount(1)
    , _stateFlags(0)
    {
    }
    virtual ~ZFObject(void) {
    }
    /** @endcond */
};

/**
 * @brief generic version of object cast
 */
extern ZFLIB_ZFCore zfany ZFObjectCast(ZF_IN const ZFClass *cls, ZF_IN const zfany &obj);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectCore_h_

