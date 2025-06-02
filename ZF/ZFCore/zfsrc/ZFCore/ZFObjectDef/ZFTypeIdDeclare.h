/**
 * @file ZFTypeIdDeclare.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeIdDeclare_h_
#define _ZFI_ZFTypeIdDeclare_h_

#include "ZFTypeIdFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief register a type for reflection
 *
 * usually for implementation use only,
 * ZFFramework would supply most of the types,
 * however you may use this to register your own types\n
 * \n
 * typical code to register a type:
 * @code
 *   // ============================================================
 *   // in h file
 *   // declare your type
 *   // * must be declared inside global namespace (#ZF_NAMESPACE_GLOBAL_BEGIN)
 *   // * type name must be unique, you may use custom prefix to prevent name conflict
 *   / **
 *    * add your Doxygen docs here
 *    * /
 *   ZFTYPEID_DECLARE(ZFLIB_APP, YourType, YourType)
 *   ZFTYPEID_REG(ZFLIB_APP, YourType, YourType)
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
 * -  for types that can not be serialized, you may use #ZFTYPEID_ACCESS_ONLY_DECLARE
 * -  for aliased type, you may use #ZFTYPEID_ALIAS_DECLARE
 *
 *
 * ADVANCED:\n
 * ZFTypeId can also be declared in different namespace,
 * or even declared as inner class, with some limitations\n
 * \n
 * declare in different namespace:
 * @code
 *   // ============================================================
 *   // in h file
 *   ZF_NAMESPACE_BEGIN(YourNamespace)
 *   // declare as usual, inside namespace
 *   ZFTYPEID_DECLARE(ZFLIB_APP, YourType, YourType)
 *   ZF_NAMESPACE_END(YourNamespace)
 *
 *   // ZFTYPEID_REG must be declared in global scope, with owner namespace declared
 *   ZFTYPEID_REG(ZFLIB_APP, YourType, YourType, YourNamespace)
 *
 *   // ============================================================
 *   // in cpp file
 *   ZFTYPEID_DEFINE(YourType, YourType, ...)
 * @endcode
 * \n
 * declare as inner class:
 * @code
 *   // ============================================================
 *   // in h file
 *   zfclass ZFLIB_APP YourOuterClass : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(YourOuterClass, ZFObject)
 *
 *       zfclassNotPOD YourType {};
 *
 *       // declare as inner class
 *       ZFTYPEID_INNER_DECLARE(YourOuterClass, YourType, YourType)
 *   };
 *
 *   // ZFTYPEID_INNER_REG must be declared in global scope
 *   ZFTYPEID_INNER_REG(YourOuterClass, YourType, YourType)
 *
 *   // ============================================================
 *   // in cpp file
 *   ZFTYPEID_INNER_DEFINE(YourOuterClass, YourType, YourType, ...)
 * @endcode
 */
#define ZFTYPEID_DECLARE(ZFLIB_, TypeName, Type) \
    ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(ZFLIB_, TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DECLARE(ZFLIB_, TypeName, Type)
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(ZFLIB_, TypeName, Type) \
    /** \n */ \
    inline const zfstring &ZFTypeId_##TypeName(void) { \
        static ZFSigName d(ZF_NAMESPACE_CURRENT() != zfnull ? zfstr("%s.%s", ZF_NAMESPACE_CURRENT(), #TypeName) : zftext(#TypeName)); \
        return d; \
    } \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    _ZFP_ZFTYPEID_CONVERTER_DECLARE(ZFLIB_, TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_REG(ZFLIB_, TypeName, Type, ...) \
    _ZFP_ZFTYPEID_REG(ZFLIB_, TypeName, Type, ##__VA_ARGS__)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE(TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction), ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_DEFINE(TypeName, Type, zffalse)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    _ZFP_ZFTYPEID_CONVERTER_DEFINE(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction), ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_METHOD_REGISTER(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_DEFINE(TypeName, Type, zftrue)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type \
            , _ZFP_ZFTYPEID_DEF_SERIALIZABLE_CONVERTER_FROM(TypeName, Type) \
            , _ZFP_ZFTYPEID_DEF_SERIALIZABLE_CONVERTER_TO(TypeName, Type) \
            , ZFM_EXPAND(convertFromStringAction) \
            , ZFM_EXPAND(convertToStringAction) \
            )

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(TypeName, Type, serializeFromAction, serializeToAction) \
    ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction)) \
    _ZFP_ZFTYPEID_DEFINE(TypeName, Type, zffalse)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(TypeName, Type, serializeFromAction, serializeToAction) \
    ZFTYPEID_DEFINE_WITH_CUSTOM_WRAPPER(TypeName, Type \
            , ZFM_EXPAND(serializeFromAction) \
            , ZFM_EXPAND(serializeToAction) \
            , _ZFP_ZFTYPEID_DEF_STRING_CONVERTER_FROM(TypeName, Type) \
            , _ZFP_ZFTYPEID_DEF_STRING_CONVERTER_TO(TypeName, Type) \
            )

// ============================================================
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_, TypeName, Type) \
    /** \n */ \
    inline const zfstring &ZFTypeId_##TypeName(void) { \
        static ZFSigName d(ZF_NAMESPACE_CURRENT() != zfnull ? zfstr("%s.%s", ZF_NAMESPACE_CURRENT(), #TypeName) : zftext(#TypeName)); \
        return d; \
    } \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    _ZFP_ZFTYPEID_WRAPPER_DECLARE(ZFLIB_, TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_, TypeName, Type, ...) \
    _ZFP_ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_, TypeName, Type, ##__VA_ARGS__)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_ACCESS_ONLY_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_ID_DATA_REGISTER(TypeName, Type)

// ============================================================
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_ALIAS_DECLARE(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
    /** @brief see @ref ZFTypeId_##AliasToTypeName */ \
    inline const zfstring &ZFTypeId_##TypeName(void) { \
        return ZFTypeId_##AliasToTypeName(); \
    } \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    _ZFP_ZFTYPEID_ALIAS_DECLARE(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_ALIAS_REG(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, ...) \
    _ZFP_ZFTYPEID_ALIAS_REG(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, _ZFP_ZFTYPEID_ALIAS_VALUE_ACCESS_DEFAULT, __VA_ARGS__ ::)
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_ALIAS_REG_CUSTOM(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion, ...) \
    _ZFP_ZFTYPEID_ALIAS_REG(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion, __VA_ARGS__ ::)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type) \
    _ZFP_ZFTYPEID_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_WRAPPER_DECLARE(ZFLIB_, TypeName, Type) \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    zfclass ZFLIB_ v_##TypeName : zfextend ZFTypeIdWrapper { \
        ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(v_##TypeName, ZFTypeIdWrapper) \
        ZFALLOC_CACHE_RELEASE({ \
            cache->zfvReset(); \
        }) \
    public: \
        typedef _ZFP_PropTypeW_##TypeName _ZFP_PropType; \
        /** @brief the value, see #ZFTypeId::Value */ \
        _ZFP_PropType zfv; \
    protected: \
        v_##TypeName(void) : zfv() {} \
    protected: \
        /** @brief init with value */ \
        virtual void objectOnInit(ZF_IN _ZFP_PropType const &value) { \
            this->zfv = value; \
        } \
        zfoverride \
        virtual void objectOnInit(void) {zfsuper::objectOnInit();} \
    public: \
        zfoverride \
        virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret); \
        zfoverride \
        virtual zfidentity objectHashImpl(void); \
        zfoverride \
        virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj); \
    public: \
        zfoverride \
        virtual void wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref); \
        zfoverride \
        virtual const zfstring &zfvTypeId(void); \
        zfoverride \
        virtual void *wrappedValue(void) {return &(this->zfv);} \
        zfoverride \
        virtual void wrappedValue(ZF_IN const void *v) {this->zfv = *(const _ZFP_PropType *)v;} \
        zfoverride \
        virtual void wrappedValueCopy(ZF_IN void *v) {*(_ZFP_PropType *)v = this->zfv;} \
    public: \
        zfoverride \
        virtual void zfvReset(void) { \
            this->zfv = zftValue<_ZFP_PropType>().zfv; \
        } \
        zfoverride \
        virtual zfbool zfvIsInit(void); \
    public: \
        zfoverride \
        virtual zfbool wrappedValuePreferStringConverter(void); \
        zfoverride \
        virtual zfbool zfvFromData( \
                ZF_IN const ZFSerializableData &serializableData \
                , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
                , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull \
                ); \
        zfoverride \
        virtual zfbool zfvToData( \
                ZF_OUT ZFSerializableData &serializableData \
                , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
                ); \
        zfoverride \
        virtual zfbool zfvFromString( \
                ZF_IN const zfchar *src \
                , ZF_IN_OPT zfindex srcLen = zfindexMax() \
                , ZF_OUT_OPT zfstring *errorHint = zfnull \
                ); \
        zfoverride \
        virtual zfbool zfvToString( \
                ZF_IN_OUT zfstring &s \
                , ZF_OUT_OPT zfstring *errorHint = zfnull \
                ); \
    public: \
        static _ZFP_ZFTypeIdProgressUpdate &_ZFP_ZFTypeId_progressUpdate(void); \
        zfoverride \
        virtual zfbool progressOnUpdate( \
                ZF_IN ZFProgressable *from \
                , ZF_IN ZFProgressable *to \
                , ZF_IN zffloat progress \
                ) { \
            if(zfself::_ZFP_ZFTypeId_progressUpdate()) { \
                return zfself::_ZFP_ZFTypeId_progressUpdate()(this, from, to, progress); \
            } \
            else { \
                return zfsuper::progressOnUpdate(from, to, progress); \
            } \
        } \
    };

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    ZFOBJECT_REGISTER(v_##TypeName) \
    _ZFP_ZFTypeIdProgressUpdate &v_##TypeName::_ZFP_ZFTypeId_progressUpdate(void) { \
        static _ZFP_ZFTypeIdProgressUpdate d = zfnull; \
        return d; \
    } \
    void v_##TypeName::objectInfoImpl(ZF_IN_OUT zfstring &ret) { \
        zftToStringT(ret, this->zfv); \
    } \
    void v_##TypeName::wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref) { \
        zfself *refTmp = zfcast(zfself *, ref); \
        if(refTmp != zfnull) { \
            this->zfv = refTmp->zfv; \
        } \
    } \
    const zfstring &v_##TypeName::zfvTypeId(void) { \
        return ZFTypeId<_ZFP_PropType>::TypeId(); \
    } \
    ZF_STATIC_REGISTER_INIT(TypeIdReg_##TypeName) { \
        ZFMethodUserRegister_1(setterMethod, { \
                invokerObject->to<v_##TypeName *>()->zfv = value; \
            }, v_##TypeName::ClassData(), void, zftext("zfv") \
            , ZFMP_IN(v_##TypeName::_ZFP_PropType const &, value) \
            ); \
        ZFMethodUserRegister_0(getterMethod, { \
                return invokerObject->to<v_##TypeName *>()->zfv; \
            }, v_##TypeName::ClassData(), v_##TypeName::_ZFP_PropType const &, zftext("zfv") \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(TypeIdReg_##TypeName) { \
        ZFMethodUserUnregister(v_##TypeName::ClassData()->methodForNameIgnoreParent(zftext("zfv"), ZFTypeId<v_##TypeName::_ZFP_PropType>::TypeId())); \
        ZFMethodUserUnregister(v_##TypeName::ClassData()->methodForNameIgnoreParent(zftext("zfv"), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(TypeIdReg_##TypeName)

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_SERIALIZABLE(TypeName, Type, preferStringConverter) \
    zfbool v_##TypeName::wrappedValuePreferStringConverter(void) { \
        return preferStringConverter; \
    } \
    zfbool v_##TypeName::zfvFromData( \
            ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */ \
            ) { \
        return TypeName##FromDataT(this->zfv, serializableData, outErrorHint, outErrorPos); \
    } \
    zfbool v_##TypeName::zfvToData( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            ) { \
        return TypeName##ToDataT(serializableData, this->zfv, outErrorHint); \
    } \
    zfbool v_##TypeName::zfvFromString( \
            ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */ \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        return TypeName##FromStringT(this->zfv, src, srcLen, errorHint); \
    } \
    zfbool v_##TypeName::zfvToString( \
            ZF_IN_OUT zfstring &s \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        return TypeName##ToStringT(s, this->zfv, errorHint); \
    }

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_NOT_SERIALIZABLE(TypeName, Type) \
    zfbool v_##TypeName::wrappedValuePreferStringConverter(void) { \
        return zffalse; \
    } \
    zfbool v_##TypeName::zfvFromData( \
            ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */ \
            ) { \
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, \
            "registered type %s is not serializable", #TypeName); \
        return zffalse; \
    } \
    zfbool v_##TypeName::zfvToData( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            ) { \
        ZFSerializableUtilErrorOccurred(outErrorHint, \
            "registered type %s is not serializable", #TypeName); \
        return zffalse; \
    } \
    zfbool v_##TypeName::zfvFromString( \
            ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */ \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        return zffalse; \
    } \
    zfbool v_##TypeName::zfvToString( \
            ZF_IN_OUT zfstring &s \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        return zffalse; \
    }

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMPARABLE(TypeName, Type) \
    zfidentity v_##TypeName::objectHashImpl(void) { \
        return zfhash(this->zfv); \
    } \
    ZFCompareResult v_##TypeName::objectCompareImpl(ZF_IN ZFObject *anotherObj) { \
        ZFTypeIdWrapper *t = zfcast(ZFTypeIdWrapper *, anotherObj); \
        if(t == zfnull || !zfstringIsEqual(this->zfvTypeId(), t->zfvTypeId())) { \
            return ZFCompareUncomparable; \
        } \
        else { \
            return ZFComparerDefault(this->zfv, *(_ZFP_PropType *)t->wrappedValue()); \
        } \
    } \
    zfbool v_##TypeName::zfvIsInit(void) { \
        return (ZFComparerDefault(this->zfv, zftValue<_ZFP_PropType>().zfv) == ZFCompareEqual); \
    }

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_UNCOMPARABLE(TypeName, Type) \
    zfidentity v_##TypeName::objectHashImpl(void) { \
        return zfidentityCalc(this); \
    } \
    ZFCompareResult v_##TypeName::objectCompareImpl(ZF_IN ZFObject *anotherObj) { \
        return this == anotherObj ? ZFCompareEqual : ZFCompareUncomparable; \
    } \
    zfbool v_##TypeName::zfvIsInit(void) { \
        return zffalse; \
    }

// ============================================================
#define _ZFP_ZFTYPEID_CONVERTER_DECLARE(ZFLIB_, TypeName, Type) \
    /** @brief see #ZFTYPEID_DECLARE */ \
    extern ZFLIB_ zfbool TypeName##FromDataT( \
            ZF_OUT _ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull \
            ); \
    /** @brief see #ZFTYPEID_DECLARE */ \
    inline _ZFP_PropTypeW_##TypeName TypeName##FromData( \
            ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull \
            ) { \
        _ZFP_PropTypeW_##TypeName ret; \
        if(TypeName##FromDataT(ret, serializableData, outErrorHint, outErrorPos)) { \
            return ret; \
        } \
        else { \
            return _ZFP_PropTypeW_##TypeName();\
        } \
        return ret; \
    } \
    /** @brief see #ZFTYPEID_DECLARE */ \
    extern ZFLIB_ zfbool TypeName##ToDataT( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            ); \
    /** @brief see #ZFTYPEID_DECLARE */ \
    inline ZFSerializableData TypeName##ToData( \
            ZF_IN _ZFP_PropTypeW_##TypeName const &v \
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
    /** @brief util method to convert TypeName from string */ \
    extern ZFLIB_ zfbool TypeName##FromStringT( \
            ZF_OUT _ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen = zfindexMax() \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ); \
    /** @brief util method to convert TypeName from string */ \
    inline _ZFP_PropTypeW_##TypeName TypeName##FromString( \
            ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen = zfindexMax() \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        _ZFP_PropTypeW_##TypeName v; \
        TypeName##FromStringT(v, src, srcLen, errorHint); \
        return v; \
    } \
    /** @cond ZFPrivateDoc */ \
    inline zfbool TypeName##FromStringT( \
            ZF_OUT _ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const zfstring &src \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        return TypeName##FromStringT(v, src.cString(), src.length(), errorHint); \
    } \
    inline _ZFP_PropTypeW_##TypeName TypeName##FromString( \
            ZF_IN const zfstring &src \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        _ZFP_PropTypeW_##TypeName v; \
        TypeName##FromStringT(v, src.cString(), src.length(), errorHint); \
        return v; \
    } \
    /** @endcond */ \
    /** @brief util method to convert TypeName to string */ \
    extern ZFLIB_ zfbool TypeName##ToStringT( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ); \
    /** @brief util method to convert TypeName to string */ \
    inline zfstring TypeName##ToString( \
            ZF_IN _ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        zfstring s; \
        TypeName##ToStringT(s, v, errorHint); \
        return s; \
    }

#define _ZFP_ZFTYPEID_CONVERTER_DEFINE(TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    zfbool TypeName##FromDataT( \
            ZF_OUT _ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */ \
            ) { \
        ZFM_EXPAND(serializeFromAction) \
    } \
    zfbool TypeName##ToDataT( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            ) { \
        ZFM_EXPAND(serializeToAction) \
    } \
    zfbool TypeName##FromStringT( \
            ZF_OUT _ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */ \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        ZFM_EXPAND(convertFromStringAction) \
    } \
    zfbool TypeName##ToStringT( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        ZFM_EXPAND(convertToStringAction) \
    }

// ============================================================
#define _ZFP_ZFTYPEID_DEF_SERIALIZABLE_CONVERTER_FROM(TypeName, Type) \
    { \
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_##TypeName(), outErrorHint, outErrorPos) == zfnull) { \
            return zffalse; \
        } \
        zfstring valueString = ZFSerializableUtil::checkPropertyValue(serializableData); \
        if(valueString == zfnull) { \
            v = _ZFP_PropTypeW_##TypeName(); \
            return zftrue; \
        } \
        if(!TypeName##FromStringT(v, valueString)) { \
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, \
                "invalid value: \"%s\"", valueString); \
            return zffalse; \
        } \
        serializableData.resolveMark(); \
        return zftrue; \
    }
#define _ZFP_ZFTYPEID_DEF_SERIALIZABLE_CONVERTER_TO(TypeName, Type) \
    { \
        serializableData.itemClass(ZFTypeId_##TypeName()); \
        zfstring s; \
        if(!TypeName##ToStringT(s, v)) { \
            ZFSerializableUtilErrorOccurred(outErrorHint, \
                "unable to convert value to string"); \
            return zffalse; \
        } \
        serializableData.propertyValue(s.isEmpty() ? zfnull : s.cString()); \
        return zftrue; \
    }
#define _ZFP_ZFTYPEID_DEF_STRING_CONVERTER_FROM(TypeName, Type) \
    { \
        ZFSerializableData serializableData; \
        return (ZFSerializableDataFromZFSD(serializableData, src, srcLen) \
            && TypeName##FromDataT(v, serializableData)); \
    }
#define _ZFP_ZFTYPEID_DEF_STRING_CONVERTER_TO(TypeName, Type) \
    { \
        ZFSerializableData serializableData; \
        if(TypeName##ToDataT(serializableData, v)) { \
            return ZFSerializableDataToZFSD(s, serializableData, zfnull, ZFSDOutputTokenTrim()); \
        } \
        else { \
            return zffalse; \
        } \
    }

// ============================================================
#define _ZFP_ZFTYPEID_REG_IMPL(ZFLIB_, TypeName, Type, TypeIdSerializable_, Scope) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<Scope _ZFP_PropTypeW_##TypeName> : zfextend ZFTypeInfo { \
    public: \
        typedef Scope _ZFP_PropTypeW_##TypeName _ZFP_PropType; \
        typedef Scope v_##TypeName _ZFP_WrapType; \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = TypeIdSerializable_, \
        }; \
        static inline const zfstring &TypeId(void) { \
            return Scope ZFTypeId_##TypeName(); \
        } \
        static inline const ZFClass *TypeIdClass(void) { \
            return _ZFP_WrapType::ClassData(); \
        } \
        zfoverride \
        virtual zfbool typeIdSerializable(void) const { \
            return TypeIdSerializable; \
        } \
        zfoverride \
        virtual const zfstring &typeId(void) const { \
            return TypeId(); \
        } \
        zfoverride \
        virtual const ZFClass *typeIdClass(void) const { \
            return TypeIdClass(); \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN _ZFP_PropType const &v \
                ) { \
            return _ZFP_PropVS<_ZFP_WrapType, _ZFP_PropType>::I(obj, v); \
        } \
        /* \
         * T_Mode:
         * * 0 : original type, or ref to original type or original pointer \
         * * 1 : pointer to original type \
         * * 2 : original type is pointer \
         */ \
        template<typename T_Access = _ZFP_PropType \
            , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropType>::Value) ? 1 \
                : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                    && zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropType>::Value \
                    && !zftTraits<T_Access>::TrIsRef) ? 2 : 0)) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (zfcast(_ZFP_WrapType *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return zfcast(_ZFP_WrapType *, obj)->zfv; \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull || (zfcast(_ZFP_WrapType *, obj) != zfnull); \
            } \
            static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull ? zfnull : &(zfcast(_ZFP_WrapType *, obj)->zfv); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 2> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull || (zfcast(_ZFP_WrapType *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull ? zfnull : (T_Access)(zfcast(_ZFP_WrapType *, obj)->zfv); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
    public: \
        zfoverride \
        virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const { \
            return ValueStore(obj, *(const _ZFP_PropType *)v); \
        } \
        zfoverride \
        virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const { \
            if(!Value<_ZFP_PropType>::zfvAccessAvailable(obj)) { \
                return zfnull; \
            } \
            return (void *)zfnew(_ZFP_PropType, Value<_ZFP_PropType>::zfvAccess(obj)); \
        } \
        zfoverride \
        virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const { \
            zfdelete((_ZFP_PropType *)v); \
            Value<_ZFP_PropType>::zfvAccessFinish(obj); \
        } \
        zfoverride \
        virtual ZFCoreArrayBase *genericArrayNew(void) const { \
            return zfnew(ZFCoreArray<_ZFP_PropType>); \
        } \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_REG(ZFLIB_, TypeName, Type, ...) \
    _ZFP_ZFTYPEID_REG_IMPL(ZFLIB_, TypeName, Type, 1, __VA_ARGS__ ::)

#define _ZFP_ZFTYPEID_ID_DATA_REGISTER(TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(PropTIReg_##TypeName) { \
        _ZFP_ZFTypeInfoRegister(ZFTypeId_##TypeName(), \
            zfnew(ZFTypeId<_ZFP_PropTypeW_##TypeName>)); \
        ZFMethodFuncUserRegister_0(dummy, { \
                return ZFTypeId_##TypeName(); \
            }, ZF_NAMESPACE_CURRENT(), const zfchar *, zftext(ZFM_TOSTRING(ZFTypeId_##TypeName)) \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(PropTIReg_##TypeName) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(ZFTypeId_##TypeName))); \
        zfdelete(_ZFP_ZFTypeInfoUnregister(ZFTypeId_##TypeName())); \
    } \
    ZF_STATIC_REGISTER_END(PropTIReg_##TypeName)

// ============================================================
#define _ZFP_ZFTYPEID_METHOD_REGISTER(TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(PropMtdReg_##TypeName) { \
        ZFMethodFuncUserRegister_4(method_FromDataT, { \
                return TypeName##FromDataT(v, serializableData, outErrorHint, outErrorPos); \
            }, ZF_NAMESPACE_CURRENT(), zfbool, zftext(ZFM_TOSTRING(TypeName##FromDataT)) \
            , ZFMP_OUT(_ZFP_PropTypeW_##TypeName &, v) \
            , ZFMP_IN(const ZFSerializableData &, serializableData) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull) \
            ); \
        ZFMethodFuncUserRegister_3(method_FromData, { \
                return TypeName##FromData(serializableData, outErrorHint, outErrorPos); \
            }, ZF_NAMESPACE_CURRENT(), _ZFP_PropTypeW_##TypeName, zftext(ZFM_TOSTRING(TypeName##FromData)) \
            , ZFMP_IN(const ZFSerializableData &, serializableData) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull) \
            ); \
        ZFMethodFuncUserRegister_3(method_ToDataT, { \
                return TypeName##ToDataT(serializableData, v, outErrorHint); \
            }, ZF_NAMESPACE_CURRENT(), zfbool, zftext(ZFM_TOSTRING(TypeName##ToDataT)) \
            , ZFMP_OUT(ZFSerializableData &, serializableData) \
            , ZFMP_IN(_ZFP_PropTypeW_##TypeName const &, v) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            ); \
        ZFMethodFuncUserRegister_2(method_ToData, { \
                return TypeName##ToData(v, outErrorHint); \
            }, ZF_NAMESPACE_CURRENT(), ZFSerializableData, zftext(ZFM_TOSTRING(TypeName##ToData)) \
            , ZFMP_IN(_ZFP_PropTypeW_##TypeName const &, v) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            ); \
        ZFMethodFuncUserRegister_4(method_FromStringT, { \
                return TypeName##FromStringT(v, src, srcLen, errorHint); \
            }, ZF_NAMESPACE_CURRENT(), zfbool, zftext(ZFM_TOSTRING(TypeName##FromStringT)) \
            , ZFMP_OUT(_ZFP_PropTypeW_##TypeName &, v) \
            , ZFMP_IN(const zfchar *, src) \
            , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        ZFMethodFuncUserRegister_3(method_FromString, { \
                return TypeName##FromString(src, srcLen, errorHint); \
            }, ZF_NAMESPACE_CURRENT(), _ZFP_PropTypeW_##TypeName, zftext(ZFM_TOSTRING(TypeName##FromString)) \
            , ZFMP_IN(const zfchar *, src) \
            , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        ZFMethodFuncUserRegister_3(method_ToStringT, { \
                return TypeName##ToStringT(s, v, errorHint); \
            }, ZF_NAMESPACE_CURRENT(), zfbool, zftext(ZFM_TOSTRING(TypeName##ToStringT)) \
            , ZFMP_OUT(zfstring &, s) \
            , ZFMP_IN(_ZFP_PropTypeW_##TypeName const &, v) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        ZFMethodFuncUserRegister_2(method_ToString, { \
                return TypeName##ToString(v, errorHint); \
            }, ZF_NAMESPACE_CURRENT(), zfstring, zftext(ZFM_TOSTRING(TypeName##ToString)) \
            , ZFMP_IN(_ZFP_PropTypeW_##TypeName const &, v) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(PropMtdReg_##TypeName) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(TypeName##FromDataT) \
                    , ZFTypeId_##TypeName() \
                    , ZFTypeId_ZFSerializableData() \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_ZFSerializableData() \
                    )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(TypeName##FromData) \
                    , ZFTypeId_ZFSerializableData() \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_ZFSerializableData() \
                    )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(TypeName##ToDataT) \
                    , ZFTypeId_ZFSerializableData() \
                    , ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(TypeName##ToData) \
                    , ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(TypeName##FromStringT) \
                    , ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_zfindex() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(TypeName##FromString) \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_zfindex() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(TypeName##ToStringT) \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(TypeName##ToString) \
                    , ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    )); \
    } \
    ZF_STATIC_REGISTER_END(PropMtdReg_##TypeName)

#define _ZFP_ZFTYPEID_DEFINE(TypeName, Type, preferStringConverter) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_SERIALIZABLE(TypeName, Type, preferStringConverter) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMPARABLE(TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_, TypeName, Type, ...) \
    _ZFP_ZFTYPEID_REG_IMPL(ZFLIB_, TypeName, Type, 0, __VA_ARGS__ ::)

#define _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_NOT_SERIALIZABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMPARABLE(TypeName, Type)

#define _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_NOT_SERIALIZABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_UNCOMPARABLE(TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_ALIAS_DECLARE(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    typedef v_##AliasToTypeName v_##TypeName;

#define _ZFP_ZFTYPEID_ALIAS_REG(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion, Scope) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<Scope _ZFP_PropTypeW_##TypeName> : zfextend ZFTypeInfo { \
    public: \
        typedef Scope _ZFP_PropTypeW_##TypeName _ZFP_PropType; \
        typedef Scope v_##TypeName _ZFP_WrapType; \
    public: \
        enum { \
            TypeIdRegistered = ZFTypeId<AliasToType>::TypeIdRegistered, \
            TypeIdSerializable = ZFTypeId<AliasToType>::TypeIdSerializable, \
        }; \
        static inline const zfstring &TypeId(void) { \
            return ZFTypeId<AliasToType>::TypeId(); \
        } \
        static inline const ZFClass *TypeIdClass(void) { \
            return ZFTypeId<AliasToType>::TypeIdClass(); \
        } \
        zfoverride \
        virtual zfbool typeIdSerializable(void) const { \
            return TypeIdSerializable; \
        } \
        zfoverride \
        virtual const zfstring &typeId(void) const { \
            return TypeId(); \
        } \
        zfoverride \
        virtual const ZFClass *typeIdClass(void) const { \
            return TypeIdClass(); \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN _ZFP_PropType const &v \
                ) { \
            return ZFTypeId<AliasToType>::ValueStore(obj, (AliasToType)v); \
        } \
        TypeIdValueConversion(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
    public: \
        zfoverride \
        virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const { \
            return ValueStore(obj, *(const _ZFP_PropType *)v); \
        } \
        zfoverride \
        virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const { \
            if(!Value<_ZFP_PropType>::zfvAccessAvailable(obj)) { \
                return zfnull; \
            } \
            return (void *)zfnew(_ZFP_PropType, Value<_ZFP_PropType>::zfvAccess(obj)); \
        } \
        zfoverride \
        virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const { \
            zfdelete((_ZFP_PropType *)v); \
            Value<_ZFP_PropType>::zfvAccessFinish(obj); \
        } \
        zfoverride \
        virtual ZFCoreArrayBase *genericArrayNew(void) const { \
            return zfnew(ZFCoreArray<_ZFP_PropType>); \
        } \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type)

#define _ZFP_ZFTYPEID_ALIAS_VALUE_ACCESS_DEFAULT(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
        template<typename T_Access = _ZFP_PropType \
            , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropType>::Value) ? 1 \
                : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                    && zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropType>::Value \
                    && !zftTraits<T_Access>::TrIsRef) ? 2 : 0)) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (obj != zfnull && ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessAvailable(obj)); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccess(obj); \
                _ZFP_PropType *v = zfnew(_ZFP_PropType); \
                *v = (_ZFP_PropType)aliasValue; \
                _ZFP_PropAliasAttach(obj, v \
                    , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                    , _ZFP_PropAliasOnDetach \
                    ); \
                return *v; \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
                if(obj) { \
                    _ZFP_PropAliasDetach(obj \
                        , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                        ); \
                } \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach( \
                    ZF_IN ZFObject *obj \
                    , ZF_IN void *v \
                    ) { \
                zfauto objTmp = obj; \
                _ZFP_PropType *vTmp = (_ZFP_PropType *)v; \
                if(ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccessAvailable(objTmp)) { \
                    AliasToType &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccess(objTmp); \
                    aliasValue = (AliasToType)*vTmp; \
                    ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccessFinish(objTmp); \
                } \
                zfdelete(vTmp); \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> { \
        private: \
             typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef; \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (obj != zfnull && ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessAvailable(obj)); \
            } \
            static _TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) { \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccess(obj); \
                _ZFP_PropType *v = zfnew(_ZFP_PropType); \
                *v = (_ZFP_PropType)aliasValue; \
                _TrNoRef *p = zfnew(_TrNoRef); \
                *p = v; \
                _ZFP_PropAliasAttach(obj, p \
                    , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                    , _ZFP_PropAliasOnDetach \
                    ); \
                return *p; \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
                if(obj) { \
                    _ZFP_PropAliasDetach(obj \
                        , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                        ); \
                } \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach( \
                    ZF_IN ZFObject *obj \
                    , ZF_IN void *v \
                    ) { \
                zfauto objTmp = obj; \
                _TrNoRef *p = (_TrNoRef *)v; \
                _ZFP_PropType *vTmp = (_ZFP_PropType *)*p; \
                if(ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccessAvailable(objTmp)) { \
                    AliasToType &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccess(objTmp); \
                    aliasValue = (AliasToType)*vTmp; \
                    ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccessFinish(objTmp); \
                } \
                zfdelete(vTmp); \
                zfdelete(p); \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 2> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull || (ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessAvailable(obj)); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                if(obj == zfnull) { \
                    return zfnull; \
                } \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccess(obj); \
                _ZFP_PropType *v = zfnew(_ZFP_PropType); \
                *v = (_ZFP_PropType)aliasValue; \
                _ZFP_PropAliasAttach(obj, v \
                    , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                    , _ZFP_PropAliasOnDetach \
                    ); \
                return *v; \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
                if(obj) { \
                    _ZFP_PropAliasDetach(obj \
                        , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                        ); \
                } \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach( \
                    ZF_IN ZFObject *obj \
                    , ZF_IN void *v \
                    ) { \
                zfauto objTmp = obj; \
                _ZFP_PropType *vTmp = (_ZFP_PropType *)v; \
                if(ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccessAvailable(objTmp)) { \
                    AliasToType &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccess(objTmp); \
                    aliasValue = (AliasToType)*vTmp; \
                    ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccessFinish(objTmp); \
                } \
                zfdelete(vTmp); \
            } \
        };

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdDeclare_h_

