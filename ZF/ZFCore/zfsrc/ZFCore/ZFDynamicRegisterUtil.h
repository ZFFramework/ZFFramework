/**
 * @file ZFDynamicRegisterUtil.h
 * @brief user registered ZFProperty
 */

#ifndef _ZFI_ZFDynamicRegisterUtil_h_
#define _ZFI_ZFDynamicRegisterUtil_h_

#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

// ============================================================
zfclassFwd _ZFP_ZFDynamicPrivate;
/**
 * @brief util class to dynamic register class/method/property
 *
 * usage:
 * @code
 *   ZFDynamic()
 *       .classBegin(className [, parent, classDynamicRegisterUserData])
 *           .event(eventName)
 *           .method(returnTypeId, methodName, ZFMP()
 *               .mp(paramTypeId0 [, paramName0, paramDefault0])
 *               , methodImpl
 *           )
 *           .property(typeIdOrRetainClass, propertyName [, propertyInitValue])
 *           .onEvent(ZFObject::E_ObjectAfterAlloc(), callback)
 *           .onInit(callback)
 *           .onDealloc(callback)
 *           .singleton()
 *       .classEnd()
 *       .NSBegin(methodNamespace)
 *           .event(eventName)
 *           .method(returnTypeId, methodName, ZFMP()
 *               .mp(paramTypeId0 [, paramName0, paramDefault0])
 *               , methodImpl
 *           )
 *       .NSEnd()
 *       .enumBegin(enumClassName) // or enumBeginFlags()
 *           .enumValue(enumName [, enumValue])
 *           .enumValue(enumName [, enumValue])
 *       .enumEnd([enumDefault])
 *   ;
 * @endcode
 *
 * when any steps failed, #errorCallbacks would be notified,
 * and all further call would be ignored\n
 * \n
 * you may store the returned ZFDynamic object,
 * and use #removeAll to remove all registered items at once\n
 * to make it more convenient for script language,
 * you may also use #regTag to make the registration looks like singleton registration
 */
zfclassLikePOD ZFLIB_ZFCore ZFDynamic {
public:
    /** @brief main constructor */
    ZFDynamic(void);
    /** @brief construct with #regTag */
    ZFDynamic(ZF_IN const zfstring &regTag);

    /** @cond ZFPrivateDoc */
    ZFDynamic(ZF_IN const ZFDynamic &ref);
    ~ZFDynamic(void);
    ZFDynamic &operator = (ZF_IN const ZFDynamic &ref);
    zfbool operator == (ZF_IN const ZFDynamic &ref) const;
    zfbool operator != (ZF_IN const ZFDynamic &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /**
     * @brief util method to export all symbols to a tag file
     *
     * the tag file contains all of these in plain text format, one line for each:
     * -  class name
     * -  method namespace
     * -  method name (including property)
     * -  type id name
     *
     * output ensured unique, while order are not ensured\n
     * anything that starts with "_ZFP_" would be ignored\n
     * \n
     * this is useful to use ZFFramework in some script language
     * which has no semantic completion,
     * so that you can use keyword completion by using the tag file
     */
    static void exportTag(
            ZF_IN_OUT const ZFOutput &output
            , ZF_IN_OPT zfbool exportScope = zffalse
            , ZF_IN_OPT zfbool exportInternal = zftrue
            );

public:
    /**
     * @brief util to make the registration able to be called more than once
     *
     * by default, dynamic register would fail if contents already exists,
     * that's not very convenient for script languages\n
     * to solve this, you may use this method to mark the registration,
     * which would automatically unregister old ones if exists,
     * identified by that regTag
     */
    ZFDynamic &regTag(ZF_IN const zfstring &regTag);
    /** @brief see #regTag */
    const zfstring &regTag(void) const;

public:
    /** @brief see #ZFDynamic */
    void removeAll(void);
    /** @brief see #ZFDynamic */
    const ZFCoreArray<const ZFClass *> &allClass(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArray<const ZFClass *> &allEnum(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArray<const ZFMethod *> &allMethod(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArray<const ZFProperty *> &allProperty(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArray<zfidentity> &allEvent(void) const;

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(
            ZF_IN const zfstring &className
            , ZF_IN const zfstring &parentClassName
            , ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull
            );
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(
            ZF_IN const zfstring &className
            , ZF_IN_OPT const ZFClass *classParent = ZFObject::ClassData()
            , ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull
            );
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(ZF_IN const ZFClass *cls);
    /** @brief see #ZFDynamic */
    ZFDynamic &classEnd(void);

    /** @brief see #ZFImplementDynamicRegister */
    ZFDynamic &classImplement(ZF_IN const ZFClass *clsToImplement);

    /** @brief see #ZFClass::classCanAllocPublic */
    ZFDynamic &classCanAllocPublic(ZF_IN zfbool value);

    /**
     * @brief see #ZFDynamic
     *
     * attach observer to instance of current #classBegin, by #ZFClass::instanceObserverAdd
     * @note only affect classes that are allocated after this method called
     */
    ZFDynamic &onEvent(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &callback
            , ZF_IN_OPT ZFLevel level = ZFLevelAppNormal
            );
    /** @brief see #ZFDynamic and #onEvent */
    ZFDynamic &onInit(ZF_IN const ZFListener &callback) {
        return this->onEvent(ZFObject::E_ObjectBeforeAlloc(), callback, ZFLevelZFFrameworkNormal);
    }
    /** @brief see #ZFDynamic and #onEvent */
    ZFDynamic &onInitFinish(ZF_IN const ZFListener &callback) {
        return this->onEvent(ZFObject::E_ObjectAfterAlloc(), callback, ZFLevelZFFrameworkNormal);
    }
    /** @brief see #ZFDynamic and #onEvent */
    ZFDynamic &onDeallocPrepare(ZF_IN const ZFListener &callback) {
        return this->onEvent(ZFObject::E_ObjectBeforeDealloc(), callback, ZFLevelZFFrameworkPostNormal);
    }
    /** @brief see #ZFDynamic and #onEvent */
    ZFDynamic &onDealloc(ZF_IN const ZFListener &callback) {
        return this->onEvent(ZFObject::E_ObjectBeforeDealloc(), callback, ZFLevelZFFrameworkPostHigh);
    }

    /**
     * @brief util to register custom constructor
     *
     * when impl valid, the impl would be used to init the object,
     * otherwise, each init param would be treated as property to init the object
     * @code
     *   ZFDynamic()
     *       :classBegin('MyClass')
     *       :property('zfstring', 'myProp')
     *       :onInit(ZFMP()
     *           :mp('zfstring', 'myProp')
     *       , function(zfargs)  -- when no impl supplied, this is the default behavior
     *           zfargs:sender():myProp(zfargs:param0())
     *       end)
     *       :classEnd()
     * @endcode
     */
    ZFDynamic &onInit(
            ZF_IN const ZFMP &mp
            , ZF_IN_OPT const ZFListener &impl = zfnull
            );

    /** @brief util for impl to implement default behavior of #onInit */
    static void onInitImpl(ZF_IN const ZFArgs &zfargs);

    /** @brief util to implement #ZFObject::objectInfoImpl by #ZFObjectVerboseInfo */
    ZFDynamic &objectInfoImplByProp(void);
    /** @brief util to implement #ZFObject::objectHashImpl by hash all properties */
    ZFDynamic &objectHashImplByProp(void);
    /** @brief util to implement #ZFObject::objectCompareImpl by #ZFPropertyAllEqual */
    ZFDynamic &objectCompareImplByProp(void);
    /** @brief util to implement #ZFObject::objectCompareValueImpl by #ZFPropertyAllEqual */
    ZFDynamic &objectCompareValueImplByProp(void);

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &NSBegin(ZF_IN const zfstring &methodNamespace);
    /** @brief see #ZFDynamic */
    ZFDynamic &NSEnd(void);

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &enumBegin(ZF_IN const zfstring &enumClassName);
    /** @brief see #ZFDynamic */
    ZFDynamic &enumBeginFlags(ZF_IN const zfstring &enumClassName);
    /** @brief see #ZFDynamic */
    ZFDynamic &enumValue(
            ZF_IN const zfstring &enumName
            , ZF_IN_OPT zfuint enumValue = ZFEnumInvalid()
            );
    /** @brief see #ZFDynamic */
    ZFDynamic &enumEnd(ZF_IN_OPT zfuint enumDefault = ZFEnumInvalid());

public:
    /**
     * @brief register a event, see also #ZFEVENT #ZFEVENT_GLOBAL
     *
     * if within class scope (#classBegin),
     * `YourClassName::E_YourEvent` would be registered,
     * otherwise, `YourNamespace::E_YourEvent` would be registered\n
     * registered event would include:
     * -  an event registered by #ZFEventDynamicRegister
     * -  a #ZFMethod to access the event
     */
    ZFDynamic &event(ZF_IN const zfstring &eventName);

public:
    /**
     * @brief see #ZFDynamic
     *
     * util method to register method (global method or class member method)
     *
     * usage:
     * @code
     *   ZFDynamic()
     *       .method("void", "myMethod", ZFMP()
     *           .mp("zfint", "p0")
     *           .mp("zfint", "p1", zfobj<v_zfint>(123))
     *           , methodImpl);
     * @endcode
     */
    ZFDynamic &method(
            ZF_IN const zfstring &returnTypeId
            , ZF_IN const zfstring &methodName
            , ZF_IN const ZFMP &methodParam
            , ZF_IN const ZFListener &methodImpl
            , ZF_IN_OPT ZFMethodType methodType = ZFMethodTypeVirtual
            , ZF_IN_OPT ZFMethodAccessType methodAccessType = ZFMethodAccessTypePublic
            );

    /** @brief see #ZFDynamic */
    ZFDynamic &method(ZF_IN const ZFMethodDynamicRegisterParam &param);

    /** @brief see #ZFDynamic */
    ZFDynamic &staticMethod(
            ZF_IN const zfstring &returnTypeId
            , ZF_IN const zfstring &methodName
            , ZF_IN const ZFMP &methodParam
            , ZF_IN const ZFListener &methodImpl
            );

    /**
     * @brief util to register a static method that holds singleton instance
     */
    ZFDynamic &singleton(ZF_IN_OPT const zfstring &methodName = zftext("instance"));

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &property(
            ZF_IN const zfstring &propertyTypeId
            , ZF_IN const zfstring &propertyName
            , ZF_IN_OPT ZFObject *propertyInitValue = zfnull
            , ZF_IN_OPT ZFMethodAccessType setterAccessType = ZFMethodAccessTypePublic
            , ZF_IN_OPT ZFMethodAccessType getterAccessType = ZFMethodAccessTypePublic
            );
    /** @brief see #ZFDynamic */
    ZFDynamic &property(
            ZF_IN const ZFClass *propertyClassOfRetainProperty
            , ZF_IN const zfstring &propertyName
            , ZF_IN_OPT ZFObject *propertyInitValue = zfnull
            , ZF_IN_OPT ZFMethodAccessType setterAccessType = ZFMethodAccessTypePublic
            , ZF_IN_OPT ZFMethodAccessType getterAccessType = ZFMethodAccessTypePublic
            );
    /**
     * @brief see #property
     *
     * the propertyInitValue callback's sender would be the #v_ZFProperty
     */
    ZFDynamic &propertyWithInit(
            ZF_IN const zfstring &propertyTypeId
            , ZF_IN const zfstring &propertyName
            , ZF_IN const ZFListener &propertyInitValue
            , ZF_IN_OPT ZFMethodAccessType setterAccessType = ZFMethodAccessTypePublic
            , ZF_IN_OPT ZFMethodAccessType getterAccessType = ZFMethodAccessTypePublic
            );
    /** @brief see #ZFDynamic */
    ZFDynamic &propertyWithInit(
            ZF_IN const ZFClass *propertyClassOfRetainProperty
            , ZF_IN const zfstring &propertyName
            , ZF_IN const ZFListener &propertyInitValue
            , ZF_IN_OPT ZFMethodAccessType setterAccessType = ZFMethodAccessTypePublic
            , ZF_IN_OPT ZFMethodAccessType getterAccessType = ZFMethodAccessTypePublic
            );
    /** @brief see #ZFDynamic */
    ZFDynamic &property(ZF_IN const ZFPropertyDynamicRegisterParam &param);

    /** @brief util to #ZFPropertyDynamicRegisterLifeCycle */
    ZFDynamic &propertyOnInit(
            ZF_IN const zfstring &propertyName
            , ZF_IN const ZFListener &callback
            );
    /** @brief util to #ZFPropertyDynamicRegisterLifeCycle */
    ZFDynamic &propertyOnUpdate(
            ZF_IN const zfstring &propertyName
            , ZF_IN const ZFListener &callback
            );
    /** @brief util to #ZFPropertyDynamicRegisterLifeCycle */
    ZFDynamic &propertyOnAttach(
            ZF_IN const zfstring &propertyName
            , ZF_IN const ZFListener &callback
            );
    /** @brief util to #ZFPropertyDynamicRegisterLifeCycle */
    ZFDynamic &propertyOnDetach(
            ZF_IN const zfstring &propertyName
            , ZF_IN const ZFListener &callback
            );

    /** @brief util to #ZFPropertyDynamicRegisterLifeCycle */
    ZFDynamic &propertyLifeCycle(
            ZF_IN const zfstring &propertyName
            , ZF_IN ZFPropertyLifeCycle lifeCycle
            , ZF_IN const ZFListener &callback
            );

public:
    /**
     * @brief util to register setter and getter static method to simulate static property
     */
    ZFDynamic &staticProperty(
            ZF_IN const zfstring &propertyTypeId
            , ZF_IN const zfstring &propertyName
            , ZF_IN_OPT ZFObject *propertyInitValue = zfnull
            , ZF_IN_OPT ZFMethodAccessType setterAccessType = ZFMethodAccessTypePublic
            , ZF_IN_OPT ZFMethodAccessType getterAccessType = ZFMethodAccessTypePublic
            );
    /** @brief see #staticProperty */
    ZFDynamic &staticProperty(
            ZF_IN const ZFClass *propertyClassOfRetainProperty
            , ZF_IN const zfstring &propertyName
            , ZF_IN_OPT ZFObject *propertyInitValue = zfnull
            , ZF_IN_OPT ZFMethodAccessType setterAccessType = ZFMethodAccessTypePublic
            , ZF_IN_OPT ZFMethodAccessType getterAccessType = ZFMethodAccessTypePublic
            );
    /**
     * @brief util to register setter and getter static method to simulate static property
     *
     * the propertyInitValue callback's sender would be the #v_ZFMethod of owner getter or setter method
     */
    ZFDynamic &staticPropertyWithInit(
            ZF_IN const zfstring &propertyTypeId
            , ZF_IN const zfstring &propertyName
            , ZF_IN const ZFListener &propertyInitValue
            , ZF_IN_OPT ZFMethodAccessType setterAccessType = ZFMethodAccessTypePublic
            , ZF_IN_OPT ZFMethodAccessType getterAccessType = ZFMethodAccessTypePublic
            );
    /** @brief see #staticPropertyWithInit */
    ZFDynamic &staticPropertyWithInit(
            ZF_IN const ZFClass *propertyClassOfRetainProperty
            , ZF_IN const zfstring &propertyName
            , ZF_IN const ZFListener &propertyInitValue
            , ZF_IN_OPT ZFMethodAccessType setterAccessType = ZFMethodAccessTypePublic
            , ZF_IN_OPT ZFMethodAccessType getterAccessType = ZFMethodAccessTypePublic
            );

public:
    /**
     * @brief callbacks which would be called when error occurred
     *
     * by default, #ZFOutputDefault would be attached during #ZFFrameworkInit
     * with #ZFLevelZFFrameworkNormal
     */
    static ZFCoreArray<ZFOutput> &errorCallbacks(void);

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

private:
    _ZFP_ZFDynamicPrivate *d;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFDynamic, ZFDynamic)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFDynamic, ZFDynamic)
ZFOUTPUT_TYPE(ZFDynamic, {v.objectInfoT(s);})

// ============================================================
/**
 * @brief util method to remove all contents registered by #ZFDynamic
 *
 * ensured called during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkNormal
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, void, ZFDynamicRemoveAll)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFDynamicRegisterUtil_h_

