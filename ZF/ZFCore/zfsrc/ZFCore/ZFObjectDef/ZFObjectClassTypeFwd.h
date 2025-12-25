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
 * @brief ZFObject weak version of #ZFCORE_PARAM
 */
#define ZFCORE_PARAM_WEAK(T_ParamType, paramName, ...) \
    _ZFP_ZFCORE_PARAM_WEAK(T_ParamType, paramName, _ZFP_ZFCORE_PARAM_WEAK_INIT(__VA_ARGS__))
#define _ZFP_ZFCORE_PARAM_WEAK_INIT(...) __VA_ARGS__
#define _ZFP_ZFCORE_PARAM_WEAK(T_ParamType, paramName, initValue) \
    public: \
        T_ParamType const &paramName(void) const { \
            return (*(this->paramName##_PropV.value) = this->paramName##_PropV.holder); \
        } \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD paramName##_PropT { \
        private: \
            typedef T_ParamType ImplType; \
        public: \
            paramName##_PropT(void) \
            : holder(initValue) \
            { \
                this->value = zfpoolNew(ImplType, this->holder); \
            } \
            paramName##_PropT(ZF_IN const paramName##_PropT &ref) \
            : holder(ref.holder) \
            { \
                this->value = zfpoolNew(ImplType, this->holder); \
            } \
            ~paramName##_PropT(void) { \
                zfpoolDelete(this->value); \
            } \
            paramName##_PropT &operator = (ZF_IN const paramName##_PropT &ref) { \
                this->holder = ref.holder; \
                *(this->value) = this->holder; \
                return *this; \
            } \
        public: \
            ImplType *value; \
            zfweak holder; \
        }; \
        paramName##_PropT paramName##_PropV; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        zfself &paramName(ZF_IN T_ParamType const &value) { \
            this->paramName##_PropV.holder = value; \
            *(this->paramName##_PropV.value) = this->paramName##_PropV.holder; \
            return *this; \
        }

// ============================================================
zfclassFwd ZFObject;
zfclassFwd ZFInterface;
zfclassFwd ZFClass;
zfclassFwd ZFProperty;
zfclassFwd ZFMethod;
zfclassFwd zfauto;
zfclassFwd zfwrap;

// ============================================================
/**
 * @brief true if Type is #ZFObject type or #ZFInterface type
 */
#define zftIsZFObject(Type) (zftIsSame<Type, ZFObject>::Value || ZFM_CLASS_HAS_MEMBER(ObjCk, _ZFP_zftIsZFObject, Type))
ZFM_CLASS_HAS_MEMBER_DECLARE(ObjCk, _ZFP_zftIsZFObject, void (*F)(void))
/* ZFTAG_TRICKS: zftIsSame<...> is required to prevent incomplete type ZFObject when used in builtin types such as zfauto */

/**
 * @brief true if Type is #ZFObject type or #ZFInterface type
 */
#define zftIsZFInterface(Type) (ZFM_CLASS_HAS_MEMBER(ObjCk, _ZFP_zftIsZFInterface, Type))
ZFM_CLASS_HAS_MEMBER_DECLARE(ObjCk, _ZFP_zftIsZFInterface, void (*F)(void))

/**
 * @brief true if Type can cast to #ZFObject
 *
 * class with this method is treated can cast to #ZFObject:
 * ZFObject *toObject(void) const;
 */
#define zftIsZFObjectType(Type) ZFM_CLASS_HAS_MEMBER(ObjTCk, toObject, Type)
ZFM_CLASS_HAS_MEMBER_DECLARE(ObjTCk, toObject, ZFObject *(T::*F)(void) const)

// ============================================================
/** @brief type for #ZFGlobalEvent::E_ClassDataUpdate */
typedef enum {
    ZFClassDataUpdateTypeAttach, /**< @brief attach */
    ZFClassDataUpdateTypeDetach, /**< @brief detach */
    ZFClassDataUpdateTypeClassAliasAttach, /**< @brief #ZFClass::classAliasTo attach */
    ZFClassDataUpdateTypeClassAliasDetach, /**< @brief #ZFClass::classAliasTo detach */
} ZFClassDataUpdateType;
/** @brief string tokens */
#define ZFTOKEN_ZFClassDataUpdateTypeAttach "Attach"
/** @brief string tokens */
#define ZFTOKEN_ZFClassDataUpdateTypeDetach "Detach"
/** @brief string tokens */
#define ZFTOKEN_ZFClassDataUpdateTypeClassAliasAttach "ClassAliasAttach"
/** @brief string tokens */
#define ZFTOKEN_ZFClassDataUpdateTypeClassAliasDetach "ClassAliasDetach"

/** @brief data holder for #ZFGlobalEvent::E_ClassDataUpdate */
zfclassLikePOD ZFLIB_ZFCore ZFClassDataUpdateData {
public:
    ZFClassDataUpdateType changeType; /**< @brief change type */
    const ZFClass *changedClass; /**< @brief changed class */
    const ZFProperty *changedProperty; /**< @brief changed property */
    const ZFMethod *changedMethod; /**< @brief changed method */
    /**
     * @brief extra data
     *
     * for ZFClassDataUpdateTypeClassAliasAttach / ZFClassDataUpdateTypeClassAliasDetach,
     * it's the aliased class name
     */
    zfstring name;

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
extern ZFLIB_ZFCore void _ZFP_ZFClassDataUpdateNotify(
        ZF_IN ZFClassDataUpdateType changeType
        , ZF_IN const ZFClass *changedClass
        , ZF_IN const ZFProperty *changedProperty
        , ZF_IN const ZFMethod *changedMethod
        , ZF_IN_OPT const zfstring &name = zfnull
        );

template<typename T_Type>
zfclassNotPOD _ZFP_zftToStringFallback<T_Type
, typename zftEnableIf<zftIsZFObject(typename zftTraits<T_Type>::TrNoRef)>::Value
> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Type const &v
            ) {
        const_cast<T_Type &>(v).toObject()->objectInfoT(s);
    }
};

/**
 * @brief special string to explicit disable serialization for impl
 *
 * serializing would cause additional data storage and processing,
 * explicitly disable serialization may improve performance for some case
 */
#define ZFSerializeDisable "-"

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectClassTypeFwd_h_

