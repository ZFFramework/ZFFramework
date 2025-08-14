/**
 * @file ZFCallback.h
 * @brief header for ZFCallback
 */

#ifndef _ZFI_ZFCallback_h_
#define _ZFI_ZFCallback_h_

#include "ZFMethod.h"
#include "zfcast.h"
#include "ZFSerializableData.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFListener;

// ============================================================
/**
 * @brief callback type of ZFCallback
 */
typedef enum {
    ZFCallbackTypeDummy, /**< @brief dummy that must not be executed */
    ZFCallbackTypeMethod, /**< @brief class static member method described by #ZFMethod */
    ZFCallbackTypeMemberMethod, /**< @brief class memeber method that need a object instance to execute, described by #ZFMethod */
    ZFCallbackTypeRawFunction, /**< @brief static function without #ZFMethod */
    ZFCallbackTypeLambda, /**< @brief lambda function that can capture other vars */
} ZFCallbackType;
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeDummy "Dummy"
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeMethod "Method"
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeMemberMethod "MemeberMethod"
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeRawFunction "RawFunction"
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeLambda "Lambda"

// ============================================================
// callback invoker
extern ZFLIB_ZFCore void _ZFP_ZFCallback_executeNullCallback(void);
/**
 * @brief dummy type for function type pointer for ZFCallback
 */
#define _ZFP_ZFCALLBACK_INVOKER(N) \
    /** @brief see #ZFCallback, you must assign the exact return type and param types for safe */ \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TEMPLATE, ZFM_COMMA, ZFM_COMMA)> \
    T_ReturnType executeExact(ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_EMPTY, ZFM_COMMA)) const { \
        switch(this->callbackType()) { \
            case ZFCallbackTypeMethod: \
            case ZFCallbackTypeMemberMethod: \
                return this->callbackMethod()->executeExact<T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TYPE, ZFM_COMMA, ZFM_COMMA)>( \
                        this->callbackOwnerObject() \
                        ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA, ZFM_COMMA) \
                    ); \
            case ZFCallbackTypeRawFunction: \
                return ((T_ReturnType (*)(ZFM_REPEAT(N, ZFM_REPEAT_TYPE, ZFM_EMPTY, ZFM_COMMA)))(this->callbackRawFunction())) \
                    (ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_EMPTY, ZFM_COMMA)); \
            case ZFCallbackTypeLambda: { \
                ZFCallback holder_(*this); \
                return ((T_ReturnType (*)(void * ZFM_REPEAT(N, ZFM_REPEAT_TYPE, ZFM_COMMA, ZFM_COMMA)))(this->callbackLambdaInvoker()))( \
                        this->callbackLambdaImpl() \
                        ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA, ZFM_COMMA) \
                    ); \
            } \
            case ZFCallbackTypeDummy: \
            default: \
                _ZFP_ZFCallback_executeNullCallback(); \
                return ((T_ReturnType (*)(void))(zfnull)) (); \
        } \
    }

typedef void (*_ZFP_ZFCallbackLambdaDeleteCallback)(void *);

// ============================================================
// ZFCallback
zfclassFwd ZFSerializableData;
zfclassFwd _ZFP_ZFCallbackPrivate;
/**
 * @brief callback used by ZFFramework
 *
 * a callback may be a class member or static member declared as #ZFMethod,
 * or static raw function,
 * it's a ZFMethod pointer container in fact\n
 * use ZFCallbackForXXX to create a callback,
 * then you may pass it as param or store it for future use\n
 * to execute callback, use #executeExact similar to ZFMethod::executeExact,
 * while you have no need to take care of the owner object
 * @warning before execute, you must check whether the callback is valid
 *   (by #valid or `cb != zfnull` or `!cb`), otherwise, assert fail
 * @warning while execute, similar to ZFMethod,
 *   you must explicitly assign the ReturnType
 *   and each ParamType for the template param
 *
 * @note you may also declare a child class of ZFCallback,
 *   by ZFCALLBACK_DECLARE_XXX, see #ZFCALLBACK_DECLARE_BEGIN
 */
zfclassLikePOD ZFLIB_ZFCore ZFCallback {
public:
    /** @cond ZFPrivateDoc */
    ZFCallback(void);
    ZFCallback(ZF_IN const zfnullT &dummy);
    ZFCallback(ZF_IN const ZFCallback &ref);
    ZFCallback &operator = (ZF_IN const ZFCallback &ref);
    ZFCallback &operator = (ZF_IN const zfnullT &dummy);
    ~ZFCallback(void);
    zfbool operator == (ZF_IN const ZFCallback &ref) const {return (this->objectCompare(ref) == ZFCompareEqual);}
    zfbool operator != (ZF_IN const ZFCallback &ref) const {return (this->objectCompare(ref) != ZFCompareEqual);}
    zfbool operator == (ZF_IN const zfnullT &dummy) const {return (d == zfnull);}
    zfbool operator != (ZF_IN const zfnullT &dummy) const {return (d != zfnull);}
    operator bool (void) const {return this->valid();}
    inline void _ZFP_ZFCallback_d(ZF_IN _ZFP_ZFCallbackPrivate *d) {
        this->d = d;
    }
    inline _ZFP_ZFCallbackPrivate *_ZFP_ZFCallback_d(void) const {
        return d;
    }
    static ZFCallback _ZFP_ZFCallbackCreateMethod(ZF_IN const ZFMethod *callbackMethod);
    static ZFCallback _ZFP_ZFCallbackCreateMemberMethod(
            ZF_IN ZFObject *callbackOwnerObject
            , ZF_IN const ZFMethod *callbackMethod
            );
    static ZFCallback _ZFP_ZFCallbackCreateRawFunction(ZF_IN ZFFuncAddrType callbackRawFunction);
    static ZFCallback _ZFP_ZFCallbackCreateLambda(
            ZF_IN void *callbackLambdaImpl
            , ZF_IN _ZFP_ZFCallbackLambdaDeleteCallback callbackLambdaImplDestroy
            , ZF_IN ZFFuncAddrType callbackLambdaInvoker
            );
    /** @endcond */

    _ZFP_ZFCALLBACK_INVOKER(0)
    _ZFP_ZFCALLBACK_INVOKER(1)
    _ZFP_ZFCALLBACK_INVOKER(2)
    _ZFP_ZFCALLBACK_INVOKER(3)
    _ZFP_ZFCALLBACK_INVOKER(4)
    _ZFP_ZFCALLBACK_INVOKER(5)
    _ZFP_ZFCALLBACK_INVOKER(6)
    _ZFP_ZFCALLBACK_INVOKER(7)
    _ZFP_ZFCALLBACK_INVOKER(8)
    /* ZFMETHOD_MAX_PARAM */

public:
    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal inline zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

    /**
     * @brief compare with another callback
     *
     * two callback is considered the same if all of these are same:
     * -  #callbackType
     * -  #callbackOwnerObject
     * -  #callbackMethod
     * -  #callbackRawFunction
     */
    zffinal ZFCompareResult objectCompare(ZF_IN const ZFCallback &ref) const;
    /**
     * @brief compare callback by instance only (same callback contents not necessarily to be same instance)
     */
    zffinal ZFCompareResult objectCompareByInstance(ZF_IN const ZFCallback &ref) const {
        return ((d == ref.d) ? ZFCompareEqual : ZFCompareUncomparable);
    }

public:
    /**
     * @brief explicitly clear all reference of this callback
     *
     * useful for script with GC logic, to explicitly release all reference of callback
     */
    zffinal void callbackClear(void);

    /**
     * @brief see #callbackClear
     *
     * observer's sender would be the #v_ZFCallback,
     * you must not store it for further use
     */
    zffinal void callbackClearPrepareAdd(
            ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) const;
    /** @brief see #callbackClear */
    zffinal void callbackClearPrepareRemove(ZF_IN const ZFListener &observer) const;

    /**
     * @brief an unique id for the callback, used for cache logic
     *
     * two callback with same callback id is treated as same callback,
     * which is useful to achieve cache logic\n
     * you must ensure the callback id is unique and verbose enough to describe the callback,
     * otherwise, leave it empty
     */
    zffinal void callbackId(ZF_IN const zfstring &callbackId);
    /**
     * @brief see #callbackId
     */
    zffinal const zfstring &callbackId(void) const;

    /**
     * @brief hash of the callback
     */
    zffinal zfidentity callbackHash(void) const;

    /**
     * @brief retain and store a autoreleased object attached to this callback,
     *   or set null to remove
     *
     * recommended to have "ZFCallbackTagKeyword_" as prefix for key name\n
     * usually used to store the owner object,
     * so that it'll be released automatically when all the callback's
     * references were deleted,
     * you can also retain the owner by #callbackOwnerObjectRetain\n
     * you can also save state for the callback as the auto released data
     */
    zffinal void callbackTag(
            ZF_IN const zfstring &key
            , ZF_IN ZFObject *tag
            );
    /** @brief see #callbackTag */
    zffinal zfany callbackTag(ZF_IN const zfstring &key) const;
    /** @brief see #callbackTag */
    zffinal void callbackTagGetAllKeyValue(
            ZF_IN_OUT ZFCoreArray<zfstring> &allKey
            , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
            ) const;
    /**
     * @brief remove tag, same as set tag to null
     */
    inline void callbackTagRemove(ZF_IN const zfstring &key) {
        this->callbackTag(key, zfnull);
    }
    /**
     * @brief remove tag, return removed tag or null if not exist
     */
    zffinal zfauto callbackTagRemoveAndGet(ZF_IN const zfstring &key);
    /**
     * @brief see #callbackTag
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void callbackTagRemoveAll(void);

    /**
     * @brief return true if callback is valid
     */
    zffinal inline zfbool valid(void) const {
        return (this->callbackType() != ZFCallbackTypeDummy);
    }

    /**
     * @brief get the type of callback
     */
    zffinal ZFCallbackType callbackType(void) const;

    /**
     * @brief get the owner object,
     *   valid only if type is class member method
     */
    zffinal zfany callbackOwnerObject(void) const;

    /**
     * @brief get the method or null if not declared by ZFMethod
     */
    zffinal const ZFMethod *callbackMethod(void) const;

    /**
     * @brief get static function, valid only if type is #ZFCallbackTypeRawFunction
     */
    zffinal ZFFuncAddrType callbackRawFunction(void) const;

    /**
     * @brief impl for lambda type
     */
    zffinal void *callbackLambdaImpl(void) const;
    /**
     * @brief impl for lambda type
     */
    zffinal ZFFuncAddrType callbackLambdaInvoker(void) const;
    zffinal void _ZFP_ZFCallback_callbackLambdaInvoker(ZF_IN ZFFuncAddrType v) const;

    /**
     * @brief retain owner object and auto release it
     *   after callback's retain count reduced to 0
     *
     * by default, owner won't be retained,
     * you may use this method to retain owner,
     * or use #callbackTag to store owner
     */
    zffinal void callbackOwnerObjectRetain(void) const;
    /**
     * @brief manually release the owner, see #callbackOwnerObjectRetain
     */
    zffinal void callbackOwnerObjectRelease(void) const;

    // ============================================================
public:
    /**
     * @brief util method to copy callback info from another callback
     */
    zffinal void callbackInfoCopy(ZF_IN const ZFCallback &src) {
        this->callbackId(src.callbackId());
        if(src.callbackSerializeDisable()) {
            this->callbackSerializeDisable(zftrue);
        }
        else {
            this->callbackSerializeType(src.callbackSerializeType());
            this->callbackSerializeData(src.callbackSerializeData());
        }
        this->pathInfo(src.pathInfo());
    }

    // ============================================================
    // callback serialize logic
public:
    /**
     * @brief see #ZFTypeId_ZFCallback
     *
     * you may set to #ZFSerializeDisable to explicitly
     * disable callback serialization
     */
    zffinal void callbackSerializeType(ZF_IN const zfstring &customType);
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal const zfstring &callbackSerializeType(void) const;
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal void callbackSerializeData(ZF_IN const ZFSerializableData &customData);
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal ZFSerializableData callbackSerializeData(void) const;

    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal void callbackSerializeDisable(ZF_IN zfbool disable) {
        this->callbackSerializeType(disable ? ZFSerializeDisable : zfnull);
    }
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal zfbool callbackSerializeDisable(void) const {
        return zfstringIsEqual(this->callbackSerializeType(), ZFSerializeDisable);
    }

    // ============================================================
    // local path logic
public:
    /**
     * @brief used for impl to store path related info, see #ZFPathInfo
     */
    zffinal ZFPathInfo pathInfo(void) const;
    /** @brief see #pathInfo */
    zffinal void pathInfo(ZF_IN const ZFPathInfo &pathInfo);

private:
    _ZFP_ZFCallbackPrivate *d;
};
ZFCOMPARER_DEFAULT_DECLARE(ZFCallback, ZFCallback, {
    return v0 == v1 ? ZFCompareEqual : ZFCompareUncomparable;
})

// ============================================================
// ZFCallback create
/**
 * @brief create a callback from ZFMethod
 *
 * assert fail if method not valid\n
 * method can be local method or class static member method
 */
#define ZFCallbackForMethod(zfmethod) \
    ZFCallback::_ZFP_ZFCallbackCreateMethod(zfmethod)

/**
 * @brief create a callback from ZFMethod of an object
 *
 * assert fail if method not valid or obj has no such method\n
 * obj won't be retained unless #ZFCallback::callbackOwnerObjectRetain is called,
 * so you must make sure the obj is alive during callback's invocation
 */
#define ZFCallbackForMemberMethod(obj, zfmethod) \
    ZFCallback::_ZFP_ZFCallbackCreateMemberMethod(obj, zfmethod)

/**
 * @brief create a callback from static function
 *
 * assert fail if function address not valid
 */
#define ZFCallbackForFunc(callbackRawFunction) \
    ZFCallback::_ZFP_ZFCallbackCreateRawFunction( \
        (ZFFuncAddrType)(void (*)(const ZFArgs &))(callbackRawFunction) \
        )

// ============================================================
// child callback declare
#define _ZFP_ZFCALLBACK_DECLARE_BEGIN(ZFLIB_, CallbackTypeName, ParentType) \
    zfclassLikePOD ZFLIB_ CallbackTypeName : zfextend ParentType { \
        _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(CallbackTypeName, ParentType) \
    public:
#define _ZFP_ZFCALLBACK_DECLARE_END(ZFLIB_, CallbackTypeName, ParentType) \
    }; \
    ZFOUTPUT_TYPE(CallbackTypeName, {v.objectInfoT(s);})
#define _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(CallbackTypeName, ParentType) \
    public: \
        /** @cond ZFPrivateDoc */ \
        CallbackTypeName(void) \
        : ParentType() \
        { \
        } \
        CallbackTypeName(ZF_IN const zfnullT &dummy) \
        : ParentType(dummy) \
        { \
        } \
        CallbackTypeName(ZF_IN const ZFCallback &ref) \
        : ParentType(ref) \
        { \
        } \
        CallbackTypeName(ZF_IN const CallbackTypeName &ref) \
        : ParentType((const ZFCallback &)ref) \
        { \
        } \
        CallbackTypeName &operator = (ZF_IN const zfnullT &dummy) { \
            ParentType::operator = (dummy); \
            return *this; \
        } \
        CallbackTypeName &operator = (ZF_IN const ZFCallback &ref) { \
            ParentType::operator = (ref); \
            return *this; \
        } \
        CallbackTypeName &operator = (ZF_IN const CallbackTypeName &ref) { \
            ParentType::operator = ((const ZFCallback &)ref); \
            return *this; \
        } \
        /** @endcond */
/**
 * @brief see #ZFCALLBACK_DECLARE_BEGIN
 */
#define ZFCALLBACK_DECLARE_END(ZFLIB_, CallbackTypeName, ParentType) \
    _ZFP_ZFCALLBACK_DECLARE_END(ZFLIB_, CallbackTypeName, ParentType) \
    ZFTYPEID_ALIAS_DECLARE(ZFLIB_, ZFCallback, ZFCallback, CallbackTypeName, CallbackTypeName) \
    ZFTYPEID_ALIAS_REG(ZFLIB_, ZFCallback, ZFCallback, CallbackTypeName, CallbackTypeName)
#define _ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFLIB_, CallbackTypeName, ParentType) \
    _ZFP_ZFCALLBACK_DECLARE_END(ZFLIB_, CallbackTypeName, ParentType)

/**
 * @brief util macro to declare a child type of ZFCallback
 *
 * declaration:
 * @code
 *   // in header file
 *   // declare by default:
 *   ZFCALLBACK_DECLARE(ZFLIB_APP, CallbackTypeName, ParentType)
 *
 *   // if you need extra functions
 *   ZFCALLBACK_DECLARE_BEGIN(ZFLIB_APP, CallbackTypeName, ParentType)
 *       // your extra functions here
 *       // it's not allowed to add member variables
 *   ZFCALLBACK_DECLARE_END(ZFLIB_APP, CallbackTypeName, ParentType)
 *
 *   // in source file
 *   ZFCALLBACK_DEFINE(CallbackTypeName, ParentType)
 * @endcode
 */
#define ZFCALLBACK_DECLARE_BEGIN(ZFLIB_, CallbackTypeName, ParentType) \
    _ZFP_ZFCALLBACK_DECLARE_BEGIN(ZFLIB_, CallbackTypeName, ParentType)
/**
 * @brief see #ZFCALLBACK_DECLARE_BEGIN
 */
#define ZFCALLBACK_DECLARE(ZFLIB_, CallbackTypeName, ParentType) \
    ZFCALLBACK_DECLARE_BEGIN(ZFLIB_, CallbackTypeName, ParentType) \
    ZFCALLBACK_DECLARE_END(ZFLIB_, CallbackTypeName, ParentType)

/**
 * @brief see #ZFCALLBACK_DECLARE_BEGIN
 */
#define ZFCALLBACK_DEFINE(CallbackTypeName, ParentType) \
    ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, CallbackTypeName, CallbackTypeName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallback_h_

