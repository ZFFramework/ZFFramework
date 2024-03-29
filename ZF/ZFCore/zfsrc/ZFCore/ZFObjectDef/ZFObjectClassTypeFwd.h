/**
 * @file ZFObjectClassTypeFwd.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFObjectClassTypeFwd_h_
#define _ZFI_ZFObjectClassTypeFwd_h_

#include "../ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief same as class, shows that this class is a #ZFObject type
 *
 * we use some dummy macro to show the basic type of class:
 * -  zfclassFwd: forward declare for a type
 * -  zfclass: shows that it's a subclass of ZFObject,
 *   note that a ZFObject is always not a POD type
 *   and always can't be declared in stack
 * -  zfabstract: shows that it's a abstract class of ZFObject type
 * -  zfclassPOD: shows that it's a POD type
 * -  zfclassLikePOD: not POD type, but you may use it like POD,
 *   e.g. declare in stack, copy value by operator = (if available),
 *   except one limitation: must not memset to 0
 * -  zfclassNotPOD: shows that it's neither ZFObject nor POD type
 *   (or not designed as POD type),
 *   and usually should not be declared in stack
 *   or copy it by copy constructor or operator =
 *
 * @warning zfclassLikePOD is not actually a POD,
 *   usually has it's own constructor or destructor,
 *   so it's highly deprecated to declare a zfclassLikePOD as static variable,
 *   use ZF_GLOBAL_INITIALIZER_INIT or ZF_STATIC_INITIALIZER_INIT instead
 *   to declare in global
 */
#define zfclass class
/**
 * @brief typename for class showing that its abstract
 */
#define zfabstract class

// ============================================================
/**
 * @brief ZFObject auto retain version of #ZFCORE_PARAM
 */
#define ZFCORE_PARAM_RETAIN(T_ParamType, paramName, ...) \
    _ZFP_ZFCORE_PARAM_RETAIN(T_ParamType, paramName, _ZFP_ZFCORE_PARAM_RETAIN_INIT(__VA_ARGS__))
#define _ZFP_ZFCORE_PARAM_RETAIN_INIT(...) __VA_ARGS__
#define _ZFP_ZFCORE_PARAM_RETAIN(T_ParamType, paramName, initValue) \
    public: \
        T_ParamType const &paramName(void) const { \
            return this->paramName##_PropV.value; \
        } \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD paramName##_ZFCoreParam { \
        public: \
            paramName##_ZFCoreParam(void) \
            : value(zfnull) \
            { \
                T_ParamType *t = zfnull; \
                this->value = zfRetain(*(t = zfnew(T_ParamType, initValue))); \
                zfdelete(t); \
            } \
            paramName##_ZFCoreParam(ZF_IN const paramName##_ZFCoreParam &ref) { \
                this->value = zfRetain(ref.value); \
            } \
            ~paramName##_ZFCoreParam(void) { \
                zfRelease(this->value); \
            } \
            paramName##_ZFCoreParam &operator = (ZF_IN const paramName##_ZFCoreParam &ref) { \
                zfCoreMutexLock(); \
                zfunsafe_zfRetain(ref.value); \
                zfunsafe_zfRelease(this->value); \
                zfCoreMutexUnlock(); \
                this->value = ref.value; \
                return *this; \
            } \
        public: \
            T_ParamType value; \
        }; \
        paramName##_ZFCoreParam paramName##_PropV; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        zfself &paramName(ZF_IN T_ParamType const &value) { \
            zfCoreMutexLock(); \
            zfunsafe_zfRetain(value); \
            zfunsafe_zfRelease(this->paramName##_PropV.value); \
            zfCoreMutexUnlock(); \
            this->paramName##_PropV.value = value; \
            return *this; \
        }

// ============================================================
#define _ZFP_ZFCORE_PARAM_RETAIN_DECLARE(T_ParamType, paramName) \
    public: \
        T_ParamType const &paramName(void) const; \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD paramName##_ZFCoreParam { \
        public: \
            paramName##_ZFCoreParam(void); \
            paramName##_ZFCoreParam(ZF_IN const paramName##_ZFCoreParam &ref); \
            ~paramName##_ZFCoreParam(void); \
            paramName##_ZFCoreParam &operator = (ZF_IN const paramName##_ZFCoreParam &ref); \
        public: \
            T_ParamType value; \
        }; \
        paramName##_ZFCoreParam paramName##_PropV; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        zfself &paramName(ZF_IN T_ParamType const &value);
#define _ZFP_ZFCORE_PARAM_RETAIN_DEFINE(T_Owner, T_ParamType, paramName, initValue) \
    T_ParamType const &T_Owner::paramName(void) const { \
        return this->paramName##_PropV.value; \
    } \
    /** @cond ZFPrivateDoc */ \
    T_Owner::paramName##_ZFCoreParam::paramName##_ZFCoreParam(void) { \
        T_ParamType *t = zfnull; \
        this->value = zfRetain(*(t = zfnew(T_ParamType, initValue))); \
        zfdelete(t); \
    } \
    T_Owner::paramName##_ZFCoreParam::paramName##_ZFCoreParam(ZF_IN const T_Owner::paramName##_ZFCoreParam &ref) { \
        this->value = zfRetain(ref.value); \
    } \
    T_Owner::paramName##_ZFCoreParam::~paramName##_ZFCoreParam(void) { \
        zfRelease(this->value); \
    } \
    T_Owner::paramName##_ZFCoreParam &T_Owner::paramName##_ZFCoreParam::operator = (ZF_IN const T_Owner::paramName##_ZFCoreParam &ref) { \
        zfCoreMutexLock(); \
        zfunsafe_zfRetain(ref.value); \
        zfunsafe_zfRelease(this->value); \
        zfCoreMutexUnlock(); \
        this->value = ref.value; \
        return *this; \
    } \
    /** @endcond */ \
    T_Owner::zfself &T_Owner::paramName(ZF_IN T_ParamType const &value) { \
        zfCoreMutexLock(); \
        zfunsafe_zfRetain(value); \
        zfunsafe_zfRelease(this->paramName##_PropV.value); \
        zfCoreMutexUnlock(); \
        this->paramName##_PropV.value = value; \
        return *this; \
    }

// ============================================================
zfclassFwd ZFObject;
zfclassFwd ZFInterface;
zfclassFwd ZFClass;
zfclassFwd ZFProperty;
zfclassFwd ZFMethod;
zfclassFwd zfauto;

// ============================================================
/**
 * @brief function type to create object
 */
typedef zfauto (*ZFObjectCreator)(void);

// ============================================================
/**
 * @brief true if Type is #ZFObject type
 */
#define zftIsZFObject(Type) (zftTypeIsSame<Type, ZFObject>::TypeIsSame || ZFM_CLASS_HAS_MEMBER(_ZFP_zftIsZFObjectCheck, _ZFP_zftIsZFObject, Type))
ZFM_CLASS_HAS_MEMBER_DECLARE(_ZFP_zftIsZFObjectCheck, _ZFP_zftIsZFObject, void (*F)(void))
/* ZFTAG_TRICKS: zftTypeIsSame<...> is required to prevent incomplete type ZFObject when used in builtin types such as zfauto */

/**
 * @brief true if Type can cast to #ZFObject
 *
 * class with this method is treated can cast to #ZFObject:
 * ZFObject *toObject(void) const;
 */
#define zftIsZFObjectType(Type) ZFM_CLASS_HAS_MEMBER(_ZFP_zftIsZFObjectTypeCheck, toObject, Type)
ZFM_CLASS_HAS_MEMBER_DECLARE(_ZFP_zftIsZFObjectTypeCheck, toObject, ZFObject *(T::*F)(void) const)

// ============================================================
/** @brief type for #ZFGlobalEvent::EventClassDataChange */
typedef enum {
    ZFClassDataChangeTypeAttach, /**< @brief attach */
    ZFClassDataChangeTypeDetach, /**< @brief detach */
    ZFClassDataChangeTypeClassAliasAttach, /**< @brief #ZFClass::classAliasTo attach */
    ZFClassDataChangeTypeClassAliasDetach, /**< @brief #ZFClass::classAliasTo detach */
} ZFClassDataChangeType;
/** @brief string tokens */
#define ZFTOKEN_ZFClassDataChangeTypeAttach "Attach"
/** @brief string tokens */
#define ZFTOKEN_ZFClassDataChangeTypeDetach "Detach"
/** @brief string tokens */
#define ZFTOKEN_ZFClassDataChangeTypeClassAliasAttach "ClassAliasAttach"
/** @brief string tokens */
#define ZFTOKEN_ZFClassDataChangeTypeClassAliasDetach "ClassAliasDetach"

/** @brief data holder for #ZFGlobalEvent::EventClassDataChange */
zfclassPOD ZFLIB_ZFCore ZFClassDataChangeData {
public:
    ZFClassDataChangeType changeType; /**< @brief change type */
    const ZFClass *changedClass; /**< @brief changed class */
    const ZFProperty *changedProperty; /**< @brief changed property */
    const ZFMethod *changedMethod; /**< @brief changed method */
    /**
     * @brief extra data
     *
     * for ZFClassDataChangeTypeClassAliasAttach / ZFClassDataChangeTypeClassAliasDetach,
     * it's the aliased class name
     */
    const zfchar *name;

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
};
extern ZFLIB_ZFCore void _ZFP_ZFClassDataChangeNotify(
        ZF_IN ZFClassDataChangeType changeType
        , ZF_IN const ZFClass *changedClass
        , ZF_IN const ZFProperty *changedProperty
        , ZF_IN const ZFMethod *changedMethod
        , ZF_IN_OPT const zfchar *name = zfnull
        );

template<typename T_Type>
zfclassNotPOD _ZFP_zftToStringFallback<T_Type
, typename zftEnableIf<zftIsZFObject(typename zftTraits<T_Type>::TrNoRef)>::EnableIf
> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Type const &v
            ) {
        const_cast<T_Type &>(v).toObject()->objectInfoT(s);
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectClassTypeFwd_h_

