/**
 * @file ZFTypeIdCore.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeIdCore_h_
#define _ZFI_ZFTypeIdCore_h_

#include "ZFStyleable.h"
#include "ZFProgressable.h"
#include "ZFMethodUserRegister.h"
#include "ZFMethodFuncUserRegister.h"
#include "ZFMethodFuncDeclare.h"
#include "ZFObjectUtil.h"
#include "ZFSerializableUtil.h"
#include "ZFSerializableDataSerializableConverter.h"
#include "ZFObjectRetain.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief register a type
 *
 * usually for implementation use only,
 * ZFFramework would supply most of the types,
 * however you may use this to register your own types\n
 * \n
 * typical code to register a type:
 * @code
 *   // ============================================================
 *   // in h file
 *   // declare your type, name must be unique
 *   / **
 *    * add your Doxygen docs here
 *    * /
 *   ZFTYPEID_DECLARE(ZFLIB_APP, YourType, YourType)
 *
 *   // ============================================================
 *   // in cpp file
 *   // register your type
 *   ZFTYPEID_DEFINE(YourType, YourType, {
 *           // serializeFrom callback, proto type:
 *           //   zfbool YourTypeFromDataT(
 *           //           ZF_OUT YourType &v
 *           //           , ZF_IN const ZFSerializableData &serializableData
 *           //           , ZF_OUT_OPT zfstring *outErrorHint = zfnull
 *           //           , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
 *           //           );
 *           // you should:
 *           //   * check whether the type match the serializableData's type
 *           //   * serialize from serializableData
 *       }, {
 *           // serializeTo callback, proto type:
 *           //   zfbool YourTypeToDataT(
 *           //           ZF_OUT ZFSerializableData &serializableData
 *           //           , ZF_IN YourType const &v
 *           //           , ZF_OUT_OPT zfstring *outErrorHint = zfnull
 *           //           );
 *           // you should:
 *           //   * save type name to the serializableData
 *           //   * save type value to the serializableData
 *       }, {
 *           // convertFromString callback, proto type:
 *           //   zfbool YourTypeFromStringT(
 *           //           ZF_OUT YourType &v
 *           //           , ZF_IN const zfchar *src
 *           //           , ZF_IN_OPT zfindex srcLen = zfindexMax()
 *           //           , ZF_OUT_OPT zfstring *errorHint = zfnull
 *           //           );
 *       }, {
 *           // convertToString callbackk, proto type:
 *           //   zfbool YourTypeToStringT(
 *           //           ZF_IN_OUT zfstring &s
 *           //           , ZF_IN YourType const &v
 *           //           , ZF_OUT_OPT zfstring *errorHint = zfnull
 *           //           );
 *       })
 *
 *   // or, you may use #ZFTYPEID_DEFINE_BY_STRING_CONVERTER for short,
 *   // if your type support converter from/to string
 *   ZFTYPEID_DEFINE_BY_STRING_CONVERTER(YourType, YourType, YourTypeFromString, YourTypeToString)
 *
 *   // or, you may use #ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER for short,
 *   // if your type support converter from/to ZFSerializableData
 *   ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(YourType, YourType, YourTypeFromData, YourTypeToData)
 * @endcode
 *
 * once registered, your type can be used as #ZFPROPERTY_ASSIGN
 * which benefits from the powerful automatic serialization logic,
 * or, can be used as reflectable param or return type for #ZFMETHOD_INLINE_0\n
 * \n
 * \n
 * ADVANCED:\n
 * -  a type can be used as #ZFPROPERTY_ASSIGN only if declared by #ZFTYPEID_DECLARE
 * -  no inheritance support for #ZFTYPEID_DECLARE
 * -  for aliased type, you may use #ZFTYPEID_ALIAS_DECLARE
 */
#define ZFTYPEID_DECLARE(ZFLIB_, TypeName, Type) \
    ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(ZFLIB_, TypeName, Type) \
    _ZFP_ZFTYPEID_DECLARE(ZFLIB_, TypeName, Type)

/**
 * @brief declare a type id with custom type wrapper
 *
 * by default, all of your type should be registered by #ZFTYPEID_DECLARE series,
 * for some special case,
 * you may need to register your type manually,
 * to achieve this, you must:
 * -  specialize template #ZFTypeId
 * -  use this macro to register your type
 */
#define ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(ZFLIB_, TypeName, Type) \
    /** \n */ \
    inline const zfchar *ZFTypeId_##TypeName(void) { \
        return ZFM_TOSTRING_DIRECT(TypeName); \
    } \
    /** @brief see #ZFTYPEID_DECLARE */ \
    extern ZFLIB_ zfbool TypeName##FromDataT( \
            ZF_OUT Type &v \
            , ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull \
            ); \
    /** @brief see #ZFTYPEID_DECLARE */ \
    inline Type TypeName##FromData( \
            ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull \
            ) { \
        Type ret; \
        if(TypeName##FromDataT(ret, serializableData, outErrorHint, outErrorPos)) { \
            return ret; \
        } \
        else { \
            typedef Type _Type; \
            return _Type();\
        } \
        return ret; \
    } \
    /** @brief see #ZFTYPEID_DECLARE */ \
    extern ZFLIB_ zfbool TypeName##ToDataT( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_IN Type const &v \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            ); \
    /** @brief see #ZFTYPEID_DECLARE */ \
    inline ZFSerializableData TypeName##ToData( \
            ZF_IN Type const &v \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            ) { \
        ZFSerializableData ret; \
        if(TypeName##ToDataT(ret, v, outErrorHint)) { \
            return ret; \
        } \
        else { \
            return ZFSerializableData(); \
        } \
    } \
    ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFLIB_, TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE(TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction), ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_DEFINE(TypeName, Type, zffalse)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    zfbool TypeName##FromDataT( \
            ZF_OUT Type &v \
            , ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */ \
            ) { \
        ZFM_EXPAND(serializeFromAction) \
    } \
    zfbool TypeName##ToDataT( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_IN Type const &v \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            ) { \
        ZFM_EXPAND(serializeToAction) \
    } \
    ZFCORETYPE_STRING_CONVERTER_DEFINE(TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_DEFINE_METHOD_REGISTER(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)

#define _ZFP_ZFTYPEID_DEFINE_METHOD_REGISTER(TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(PropMtdReg_##TypeName) { \
        ZFMethodFuncUserRegister_4(method_FromDataT, { \
                return TypeName##FromDataT(v, serializableData, outErrorHint, outErrorPos); \
            }, ZF_NAMESPACE_GLOBAL_NAME, zfbool, ZFM_TOSTRING(TypeName##FromDataT) \
            , ZFMP_OUT(Type &, v) \
            , ZFMP_IN(const ZFSerializableData &, serializableData) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull) \
            ); \
        _method_FromDataT = method_FromDataT; \
        ZFMethodFuncUserRegister_3(method_FromData, { \
                return TypeName##FromData(serializableData, outErrorHint, outErrorPos); \
            }, ZF_NAMESPACE_GLOBAL_NAME, Type, ZFM_TOSTRING(TypeName##FromData) \
            , ZFMP_IN(const ZFSerializableData &, serializableData) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull) \
            ); \
        _method_FromData = method_FromData; \
        ZFMethodFuncUserRegister_3(method_ToDataT, { \
                return TypeName##ToDataT(serializableData, v, outErrorHint); \
            }, ZF_NAMESPACE_GLOBAL_NAME, zfbool, ZFM_TOSTRING(TypeName##ToDataT) \
            , ZFMP_OUT(ZFSerializableData &, serializableData) \
            , ZFMP_IN(Type const &, v) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            ); \
        _method_ToDataT = method_ToDataT; \
        ZFMethodFuncUserRegister_2(method_ToData, { \
                return TypeName##ToData(v, outErrorHint); \
            }, ZF_NAMESPACE_GLOBAL_NAME, ZFSerializableData, ZFM_TOSTRING(TypeName##ToData) \
            , ZFMP_IN(Type const &, v) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            ); \
        _method_ToData = method_ToData; \
        ZFMethodFuncUserRegister_4(method_FromStringT, { \
                return TypeName##FromStringT(v, src, srcLen, errorHint); \
            }, ZF_NAMESPACE_GLOBAL_NAME, zfbool, ZFM_TOSTRING(TypeName##FromStringT) \
            , ZFMP_OUT(Type &, v) \
            , ZFMP_IN(const zfchar *, src) \
            , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        _method_FromStringT = method_FromStringT; \
        ZFMethodFuncUserRegister_3(method_FromString, { \
                return TypeName##FromString(src, srcLen, errorHint); \
            }, ZF_NAMESPACE_GLOBAL_NAME, Type, ZFM_TOSTRING(TypeName##FromString) \
            , ZFMP_IN(const zfchar *, src) \
            , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        _method_FromString = method_FromString; \
        ZFMethodFuncUserRegister_3(method_ToStringT, { \
                return TypeName##ToStringT(s, v, errorHint); \
            }, ZF_NAMESPACE_GLOBAL_NAME, zfbool, ZFM_TOSTRING(TypeName##ToStringT) \
            , ZFMP_OUT(zfstring &, s) \
            , ZFMP_IN(Type const &, v) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        _method_ToStringT = method_ToStringT; \
        ZFMethodFuncUserRegister_2(method_ToString, { \
                return TypeName##ToString(v, errorHint); \
            }, ZF_NAMESPACE_GLOBAL_NAME, zfstring, ZFM_TOSTRING(TypeName##ToString) \
            , ZFMP_IN(Type const &, v) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        _method_ToString = method_ToString; \
    } \
    ZF_STATIC_REGISTER_DESTROY(PropMtdReg_##TypeName) { \
        ZFMethodFuncUserUnregister(_method_FromDataT); \
        ZFMethodFuncUserUnregister(_method_FromData); \
        ZFMethodFuncUserUnregister(_method_ToDataT); \
        ZFMethodFuncUserUnregister(_method_ToData); \
        ZFMethodFuncUserUnregister(_method_FromStringT); \
        ZFMethodFuncUserUnregister(_method_FromString); \
        ZFMethodFuncUserUnregister(_method_ToStringT); \
        ZFMethodFuncUserUnregister(_method_ToString); \
    } \
    public: \
        const ZFMethod *_method_FromDataT; \
        const ZFMethod *_method_FromData; \
        const ZFMethod *_method_ToDataT; \
        const ZFMethod *_method_ToData; \
        const ZFMethod *_method_FromStringT; \
        const ZFMethod *_method_FromString; \
        const ZFMethod *_method_ToStringT; \
        const ZFMethod *_method_ToString; \
    ZF_STATIC_REGISTER_END(PropMtdReg_##TypeName)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_DEFINE(TypeName, Type, zftrue)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, { \
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_##TypeName(), outErrorHint, outErrorPos) == zfnull) { \
            return zffalse; \
        } \
        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData); \
        if(valueString == zfnull) { \
            typedef Type _Type; \
            v = _Type(); \
            return zftrue; \
        } \
        if(!TypeName##FromStringT(v, valueString)) { \
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, \
                "invalid value: \"%s\"", valueString); \
            return zffalse; \
        } \
        serializableData.resolveMark(); \
        return zftrue; \
    }, { \
        serializableData.itemClass(ZFTypeId_##TypeName()); \
        zfstring s; \
        if(!TypeName##ToStringT(s, v)) { \
            ZFSerializableUtilErrorOccurred(outErrorHint, \
                "unable to convert value to string"); \
            return zffalse; \
        } \
        serializableData.propertyValue(s.isEmpty() ? zfnull : s.cString()); \
        return zftrue; \
    }, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction))

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(TypeName, Type, serializeFromAction, serializeToAction) \
    ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction)) \
    _ZFP_ZFTYPEID_DEFINE(TypeName, Type, zffalse)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, serializeFromAction, serializeToAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction), { \
        ZFSerializableData serializableData; \
        return (ZFSerializableDataFromZfsd(serializableData, src, srcLen) \
            && TypeName##FromDataT(v, serializableData)); \
    }, { \
        ZFSerializableData serializableData; \
        if(TypeName##ToDataT(serializableData, v)) { \
            return ZFSerializableDataToZfsd(s, serializableData, zfnull, zffalse); \
        } \
        else { \
            return zffalse; \
        } \
    })

// ============================================================
/**
 * @brief declare a type that reflectable and accessable, but not serializable
 *
 * by default, all return types and method types in ZFMethod
 * must be registered by #ZFTYPEID_DECLARE
 * for advanced reflection logic\n
 * if you simply want basic method declaration,
 * you may use this macro to disable your type explicitly\n
 * see #ZFTYPEID_DECLARE for more info
 */
#define ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_, TypeName, Type) \
    /** \n */ \
    inline const zfchar *ZFTypeId_##TypeName(void) { \
        return ZFM_TOSTRING_DIRECT(TypeName); \
    } \
    _ZFP_ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_, TypeName, Type)
/** @brief see #ZFTYPEID_ACCESS_ONLY_DECLARE */
#define ZFTYPEID_ACCESS_ONLY_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)
/** @brief see #ZFTYPEID_ACCESS_ONLY_DECLARE */
#define ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)

// ============================================================
/**
 * @brief alias an existing type to another type, see #ZFTYPEID_DECLARE
 *
 * the original type must be declared by #ZFTYPEID_DECLARE or #ZFTYPEID_ACCESS_ONLY_DECLARE\n
 * the newly aliased type would share same type id,
 * serializable converter and string converter,
 * with the original exist one\n
 * see #ZFTYPEID_DECLARE for more info
 * @note aliased type would:
 *   -  create temp holder object for conversion
 *   -  perform value copy during conversion
 *
 *   which would cause bad performance during reflection\n
 *   if performance matters,
 *   you should consider supply custom type id specializations (by #ZFTYPEID_ALIAS_DECLARE_CUSTOM),
 *   or, prevent to use aliased type for reflectable method
 */
#define ZFTYPEID_ALIAS_DECLARE(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
    /** @brief see @ref ZFTypeId_##AliasToTypeName */ \
    inline const zfchar *ZFTypeId_##TypeName(void) { \
        return ZFTypeId_##AliasToTypeName(); \
    } \
    _ZFP_ZFTYPEID_ALIAS_DECLARE(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, _ZFP_ZFTYPEID_ALIAS_EXPAND_DEFAULT)
/** @brief see #ZFTYPEID_ALIAS_DECLARE */
#define ZFTYPEID_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type) \
    _ZFP_ZFTYPEID_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type)

/**
 * @brief see #ZFTYPEID_ALIAS_DECLARE
 *
 * the TypeIdValueConversion must supply as macro expansion with these proto type:
 * @code
 *   #define MyExpand(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
 *       template<typename T_Access = _ZFP_PropTypeW_##TypeName \
 *           , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
 *               && zftIsSame< \
 *                       typename zftTraits<T_Access>::TrNoRef, \
 *                       _ZFP_PropTypeW_##TypeName \
 *                   >::Value != 1) \
 *               ? 1 : 0) \
 *           , typename T_Fix = void \
 *           > \
 *       zfclassNotPOD Value { \
 *       public: \
 *           static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
 *               return yourChecker(xxx); \
 *           } \
 *           static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
 *               return yourAccess(xxx); \
 *           } \
 *           static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
 *               yourCleanup(xxx); \
 *           } \
 *       }; \
 *       template<typename T_Access> \
 *       zfclassNotPOD Value<T_Access, 1> { \
 *       public: \
 *           static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
 *               return yourChecker(xxx); \
 *           } \
 *           static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
 *               return yourAccess(xxx); \
 *           } \
 *           static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
 *               yourCleanup(xxx); \
 *           } \
 *       };
 * @endcode
 */
#define ZFTYPEID_ALIAS_DECLARE_CUSTOM(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion) \
    /** @brief see @ref ZFTypeId_##AliasToTypeName */ \
    inline const zfchar *ZFTypeId_##TypeName(void) { \
        return ZFTypeId_##AliasToTypeName(); \
    } \
    _ZFP_ZFTYPEID_ALIAS_DECLARE(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion)

// ============================================================
/**
 * @brief explicitly mark the type can not be reflected when used in #ZFMETHOD_INLINE_0 or #ZFPROPERTY_ASSIGN
 *
 * by default, all return types and method types in ZFMethod
 * must be registered by #ZFTYPEID_DECLARE
 * for advanced reflection logic\n
 * if you simply want basic method declaration,
 * you may use this macro to disable your type explicitly\n
 * see #ZFTYPEID_DECLARE for more info
 */
#define ZFTYPEID_DISABLE(Type) \
    _ZFP_ZFTYPEID_DISABLE(Type)

// ============================================================
/**
 * @brief dummy base for all wrapper types,
 *   see #ZFTypeId::Value
 *
 * serializable data:
 * @code
 *   <WrapperTypeClass
 *       value="string content" // optional, empty by default
 *   />
 * @endcode
 */
zfabstract ZFLIB_ZFCore ZFTypeIdWrapper : zfextend ZFStyleableObject, zfimplement ZFProgressable {
    ZFOBJECT_DECLARE_ABSTRACT_WITH_CUSTOM_CTOR(ZFTypeIdWrapper, ZFStyleableObject)
    ZFIMPLEMENT_DECLARE(ZFProgressable)
    ZFALLOC_CACHE_RELEASE_ABSTRACT({
        cache->wrappedValueIsConst = zffalse;
        cache->wrappedValueReset();
    })

public:
    /**
     * @brief whether the holded value should not be changed
     */
    zfbool wrappedValueIsConst;
    /**
     * @brief util to mark #wrappedValueIsConst
     */
    static void markConst(ZF_IN_OUT_OPT ZFObject *obj) {
        zfself *t = ZFCastZFObject(zfself *, obj);
        if(t != zfnull) {
            t->wrappedValueIsConst = zftrue;
        }
    }

/** @cond ZFPrivateDoc */
protected:
    ZFTypeIdWrapper(void) : wrappedValueIsConst(zffalse) {}
/** @endcond */

public:
    /**
     * @brief copy internal value, assert fail if #wrappedValueIsConst
     */
    zffinal ZFTypeIdWrapper *wrappedValueAssign(ZF_IN ZFTypeIdWrapper *ref) {
        zfCoreAssert(!this->wrappedValueIsConst);
        this->wrappedValueOnAssign(ref);
        return this;
    }
protected:
    /**
     * @brief see #wrappedValueAssign
     */
    virtual void wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref) zfpurevirtual;

public:
    /**
     * @brief type id for this wrapper type
     */
    virtual const zfchar *wrappedValueTypeId(void) zfpurevirtual;
    /**
     * @brief access the value's address
     */
    virtual void *wrappedValue(void) zfpurevirtual;
    /**
     * @brief set the value, no type safe check
     */
    virtual void wrappedValue(ZF_IN const void *v) zfpurevirtual;
    /**
     * @brief get the value, no type safe check
     */
    virtual void wrappedValueCopy(ZF_IN void *v) zfpurevirtual;

    /**
     * @brief reset the value to it's init value
     */
    virtual void wrappedValueReset(void) zfpurevirtual;
    /**
     * @brief true if the value is in init value state
     */
    virtual zfbool wrappedValueIsInit(void) zfpurevirtual;

public:
    /**
     * @brief whether prefer string converter during serialization,
     *   which would result much shorter result
     *
     * @code
     *   // default complex serializable data:
     *   <Owner>
     *       <TypeId prop="PropertyName" value="xxx"/>
     *   </Owner>
     *
     *   // if prefer string converter
     *   <Owner PropertyName="xxx" />
     * @endcode
     */
    virtual zfbool wrappedValuePreferStringConverter(void) {return zffalse;}
    /**
     * @brief convert from serializable data
     */
    virtual zfbool wrappedValueFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) zfpurevirtual;
    /**
     * @brief convert to serializable data
     */
    virtual zfbool wrappedValueToData(
            ZF_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) zfpurevirtual;
    /**
     * @brief convert from string
     */
    virtual zfbool wrappedValueFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;
    /**
     * @brief convert to string
     */
    virtual zfbool wrappedValueToString(
            ZF_IN_OUT zfstring &s
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;

public:
    zfoverride
    virtual zfbool progressUpdate(
            ZF_IN ZFProgressable *from
            , ZF_IN ZFProgressable *to
            , ZF_IN zffloat progress
            ) {
        return zffalse;
    }

public:
    zfoverride
    virtual zfbool objectIsInternal(void) {
        return zftrue;
    }

protected:
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable) {
        zfsuper::styleableOnCopyFrom(anotherStyleable);
        this->wrappedValue(anotherStyleable->to<ZFTypeIdWrapper *>()->wrappedValue());
    }

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) {
        if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
        if(valueString == zfnull) {
            this->wrappedValueReset();
        }
        else if(!this->wrappedValueFromString(valueString)) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "failed to convert from \"%s\"", valueString);
            return zffalse;
        }
        return zftrue;
    }
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) {
        if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
        zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

        if((ref == zfnull && !this->wrappedValueIsInit())
                || (ref != zfnull && this->objectCompare(ref) != ZFCompareTheSame)
                ) {
            zfstring valueString;
            if(!this->wrappedValueToString(valueString, outErrorHint)) {
                return zffalse;
            }
            serializableData.propertyValue(valueString);
        }

        return zftrue;
    }
protected:
    zfoverride
    virtual inline zfbool serializableOnSerializeFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        return this->wrappedValueFromString(src, srcLen, errorHint);
    }
    zfoverride
    virtual inline zfbool serializableOnSerializeToString(
            ZF_IN_OUT zfstring &ret
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        return this->wrappedValueToString(ret, errorHint);
    }
};

// ============================================================
/**
 * @brief register Type's progress update logic
 *
 * a progress update means,
 * the type can be changed by timeline by supply a "from/to" value range\n
 * for example, an float type with range "[0.5, 3.0]" and progress "0.3",
 * would result "0.5 + (3.0 - 0.5) * 0.3", which is 1.25\n
 * this is useful to make a property change with animation\n
 * \n
 * to use this, register your type by this macro, with this proto type:
 * @code
 *   zfbool update(
 *           ZF_OUT Type &ret
 *           , ZF_IN Type const &from
 *           , ZF_IN Type const &to
 *           , ZF_IN zffloat progress
 *           );
 * @endcode
 * then use them by #ZFProgressable::progressUpdate\n
 * \n
 * to register:
 * @code
 *   ZFTYPEID_PROGRESS_DEFINE(YourType, YourType, {
 *           yourProgress(ret, from, to, progress);
 *       })
 * @endcode
 * or use #ZFTYPEID_PROGRESS_DEFINE_BY_VALUE for short
 */
#define ZFTYPEID_PROGRESS_DEFINE(TypeName, Type, progressUpdateAction) \
    ZF_STATIC_REGISTER_INIT(_ZFP_ZFTypeIdProgressReg_##TypeName) { \
        v_##TypeName::_ZFP_ZFTypeId_progressUpdate = zfself::I; \
    } \
    static zfbool I( \
            ZF_IN_OUT ZFProgressable *_ret \
            , ZF_IN ZFProgressable *_from \
            , ZF_IN ZFProgressable *_to \
            , ZF_IN zffloat progress \
            ) { \
        v_##TypeName *_retH = ZFCastZFObject(v_##TypeName *, _ret); \
        v_##TypeName *_fromH = ZFCastZFObject(v_##TypeName *, _from); \
        v_##TypeName *_toH = ZFCastZFObject(v_##TypeName *, _to); \
        if(_retH == zfnull || _fromH == zfnull || _toH == zfnull) { \
            return zffalse; \
        } \
        Type &ret = _retH->zfv; \
        Type const &from = _fromH->zfv; \
        Type const &to = _toH->zfv; \
        progressUpdateAction \
        return zftrue; \
    } \
    ZF_STATIC_REGISTER_END(_ZFP_ZFTypeIdProgressReg_##TypeName)
/**
 * @brief util macro to declare #ZFTYPEID_PROGRESS_DEFINE by raw value calculating
 */
#define ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(TypeName, Type) \
    ZFTYPEID_PROGRESS_DEFINE(TypeName, Type, { \
            ret = (Type)(from + (Type)((to - from) * progress)); \
        })

// ============================================================
template<typename T_Type>
ZFV::operator T_Type &(void) const {
    ZFTypeIdWrapper *w = ZFCastZFObject(ZFTypeIdWrapper *, _ZFP_obj);
    typedef typename zftTraits<T_Type &>::TrNoRef T_Type_;
    zfCoreAssertWithMessageTrim(w != zfnull && w->classData()->classIsTypeOf(ZFTypeId<T_Type_>::TypeIdClass())
            , "unable to access %s from object: %s"
            , ZFTypeId<T_Type_>::TypeId()
            , _ZFP_obj
            );
    return *(T_Type_ *)w->wrappedValue();
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdCore_h_

