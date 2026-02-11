/**
 * @file ZFTypeIdDeclare_inner.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeIdDeclare_inner_h_
#define _ZFI_ZFTypeIdDeclare_inner_h_

#include "ZFTypeIdFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_DECLARE(OuterClass, TypeName, Type) \
    ZFTYPEID_INNER_DECLARE_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DECLARE(OuterClass, TypeName, Type)
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_DECLARE_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type) \
    /** \n */ \
    static inline const zfstring &ZFTypeId_##TypeName(void) { \
        static ZFSigName d(zfstr("%s.%s", OuterClass::ClassData()->classNameFull(), #TypeName)); \
        return d; \
    } \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    _ZFP_ZFTYPEID_INNER_CONVERTER_DECLARE(OuterClass, TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_REG(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_REG(OuterClass, TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_DEFINE(OuterClass, TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_INNER_DEFINE_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction), ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_INNER_DEFINE(OuterClass, TypeName, Type, zffalse)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_DEFINE_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    _ZFP_ZFTYPEID_INNER_CONVERTER_DEFINE(OuterClass, TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction), ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_INNER_METHOD_REGISTER(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_ID_DATA_REGISTER(OuterClass, TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_DEFINE_BY_STRING_CONVERTER(OuterClass, TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_INNER_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type, ZFM_EXPAND(convertFromStringAction), ZFM_EXPAND(convertToStringAction)) \
    _ZFP_ZFTYPEID_INNER_DEFINE(OuterClass, TypeName, Type, zftrue)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type, convertFromStringAction, convertToStringAction) \
    ZFTYPEID_INNER_DEFINE_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type \
            , _ZFP_ZFTYPEID_DEF_SERIALIZABLE_CONVERTER_FROM(TypeName, Type) \
            , _ZFP_ZFTYPEID_DEF_SERIALIZABLE_CONVERTER_TO(TypeName, Type) \
            , ZFM_EXPAND(convertFromStringAction) \
            , ZFM_EXPAND(convertToStringAction) \
            )

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_DEFINE_BY_SERIALIZABLE_CONVERTER(OuterClass, TypeName, Type, serializeFromAction, serializeToAction) \
    ZFTYPEID_INNER_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type, ZFM_EXPAND(serializeFromAction), ZFM_EXPAND(serializeToAction)) \
    _ZFP_ZFTYPEID_INNER_DEFINE(OuterClass, TypeName, Type, zffalse)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type, serializeFromAction, serializeToAction) \
    ZFTYPEID_INNER_DEFINE_WITH_CUSTOM_WRAPPER(OuterClass, TypeName, Type \
            , ZFM_EXPAND(serializeFromAction) \
            , ZFM_EXPAND(serializeToAction) \
            , _ZFP_ZFTYPEID_DEF_STRING_CONVERTER_FROM(TypeName, Type) \
            , _ZFP_ZFTYPEID_DEF_STRING_CONVERTER_TO(TypeName, Type) \
            )

// ============================================================
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_ACCESS_ONLY_DECLARE(OuterClass, TypeName, Type) \
    /** \n */ \
    static inline const zfstring &ZFTypeId_##TypeName(void) { \
        static ZFSigName d(zfstr("%s.%s", OuterClass::ClassData()->classNameFull(), #TypeName)); \
        return d; \
    } \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DECLARE(OuterClass, TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_ACCESS_ONLY_REG(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_ACCESS_ONLY_REG(OuterClass, TypeName, Type)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_ACCESS_ONLY_DEFINE(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_ACCESS_ONLY_DEFINE(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_ID_DATA_REGISTER(OuterClass, TypeName, Type)
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_ACCESS_ONLY_DEFINE_UNCOMPARABLE(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_ACCESS_ONLY_DEFINE_UNCOMPARABLE(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_ID_DATA_REGISTER(OuterClass, TypeName, Type)

// ============================================================
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_ALIAS_DECLARE(OuterClass, AliasToTypeName, AliasToType, TypeName, Type) \
    /** @brief see @ref ZFTypeId_##AliasToTypeName */ \
    static inline const zfstring &ZFTypeId_##TypeName(void) { \
        return ZFTypeId_##AliasToTypeName(); \
    } \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    _ZFP_ZFTYPEID_INNER_ALIAS_DECLARE(OuterClass, AliasToTypeName, AliasToType, TypeName, Type) \

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_ALIAS_REG(OuterClass, AliasToTypeName, AliasToType, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_ALIAS_REG(OuterClass, AliasToTypeName, AliasToType, TypeName, Type, _ZFP_ZFTYPEID_INNER_ALIAS_VALUE_ACCESS_DEFAULT)
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_ALIAS_REG_CUSTOM(OuterClass, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion) \
    _ZFP_ZFTYPEID_INNER_ALIAS_REG(OuterClass, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion)

/** @brief see #ZFTYPEID_DECLARE */
#define ZFTYPEID_INNER_ALIAS_DEFINE(OuterClass, AliasToTypeName, AliasToType, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_ALIAS_DEFINE(OuterClass, AliasToTypeName, AliasToType, TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_INNER_WRAPPER_DECLARE(OuterClass, TypeName, Type) \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    zfclass v_##TypeName : zfextend ZFTypeIdWrapper { \
        ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(v_##TypeName, ZFTypeIdWrapper, OuterClass) \
        ZFOBJECT_CACHE_RELEASE({ \
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

#define _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_COMMON(OuterClass, TypeName, Type) \
    ZFOBJECT_REGISTER(OuterClass, v_##TypeName) \
    _ZFP_ZFTypeIdProgressUpdate &OuterClass::v_##TypeName::_ZFP_ZFTypeId_progressUpdate(void) { \
        static _ZFP_ZFTypeIdProgressUpdate d = zfnull; \
        return d; \
    } \
    void OuterClass::v_##TypeName::objectInfoImpl(ZF_IN_OUT zfstring &ret) { \
        zftToStringT(ret, this->zfv); \
    } \
    void OuterClass::v_##TypeName::wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref) { \
        zfself *refTmp = zfcast(zfself *, ref); \
        if(refTmp != zfnull) { \
            this->zfv = refTmp->zfv; \
        } \
    } \
    const zfstring &OuterClass::v_##TypeName::zfvTypeId(void) { \
        return ZFTypeId<_ZFP_PropType>::TypeId(); \
    } \
    ZF_STATIC_REGISTER_INIT(TypeIdReg_##OuterClass##_##TypeName) { \
        ZFMethodUserRegister_1(setterMethod, { \
                invokerObject->to<OuterClass::v_##TypeName *>()->zfv = value; \
            }, OuterClass::v_##TypeName::ClassData(), void, zftext("zfv") \
            , ZFMP_IN(OuterClass::v_##TypeName::_ZFP_PropType const &, value) \
            ); \
        ZFMethodUserRegister_0(getterMethod, { \
                return invokerObject->to<OuterClass::v_##TypeName *>()->zfv; \
            }, OuterClass::v_##TypeName::ClassData(), OuterClass::v_##TypeName::_ZFP_PropType const &, zftext("zfv") \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(TypeIdReg_##OuterClass##_##TypeName) { \
        ZFMethodUserUnregister(OuterClass::v_##TypeName::ClassData()->methodForNameIgnoreParent(zftext("zfv"), ZFTypeId<OuterClass::v_##TypeName::_ZFP_PropType>::TypeId())); \
        ZFMethodUserUnregister(OuterClass::v_##TypeName::ClassData()->methodForNameIgnoreParent(zftext("zfv"), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(TypeIdReg_##OuterClass##_##TypeName)

#define _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_SERIALIZABLE(OuterClass, TypeName, Type, preferStringConverter) \
    zfbool OuterClass::v_##TypeName::wrappedValuePreferStringConverter(void) { \
        return preferStringConverter; \
    } \
    zfbool OuterClass::v_##TypeName::zfvFromData( \
            ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */ \
            ) { \
        return OuterClass::TypeName##FromDataT(this->zfv, serializableData, outErrorHint, outErrorPos); \
    } \
    zfbool OuterClass::v_##TypeName::zfvToData( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            ) { \
        return OuterClass::TypeName##ToDataT(serializableData, this->zfv, outErrorHint); \
    } \
    zfbool OuterClass::v_##TypeName::zfvFromString( \
            ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */ \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        return OuterClass::TypeName##FromStringT(this->zfv, src, srcLen, errorHint); \
    } \
    zfbool OuterClass::v_##TypeName::zfvToString( \
            ZF_IN_OUT zfstring &s \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        return OuterClass::TypeName##ToStringT(s, this->zfv, errorHint); \
    }

#define _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_NOT_SERIALIZABLE(OuterClass, TypeName, Type) \
    zfbool OuterClass::v_##TypeName::wrappedValuePreferStringConverter(void) { \
        return zffalse; \
    } \
    zfbool OuterClass::v_##TypeName::zfvFromData( \
            ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */ \
            ) { \
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, \
            "registered type %s is not serializable", #TypeName); \
        return zffalse; \
    } \
    zfbool OuterClass::v_##TypeName::zfvToData( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            ) { \
        ZFSerializableUtilErrorOccurred(outErrorHint, \
            "registered type %s is not serializable", #TypeName); \
        return zffalse; \
    } \
    zfbool OuterClass::v_##TypeName::zfvFromString( \
            ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */ \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        return zffalse; \
    } \
    zfbool OuterClass::v_##TypeName::zfvToString( \
            ZF_IN_OUT zfstring &s \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        return zffalse; \
    }

#define _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_COMPARABLE(OuterClass, TypeName, Type) \
    zfidentity OuterClass::v_##TypeName::objectHashImpl(void) { \
        return zfhash(this->zfv); \
    } \
    ZFCompareResult OuterClass::v_##TypeName::objectCompareImpl(ZF_IN ZFObject *anotherObj) { \
        ZFTypeIdWrapper *t = zfcast(ZFTypeIdWrapper *, anotherObj); \
        if(t == zfnull || !zfstringIsEqual(this->zfvTypeId(), t->zfvTypeId())) { \
            return ZFCompareUncomparable; \
        } \
        else { \
            return ZFComparerDefault(this->zfv, *(_ZFP_PropType *)t->wrappedValue()); \
        } \
    } \
    zfbool OuterClass::v_##TypeName::zfvIsInit(void) { \
        return (ZFComparerDefault(this->zfv, zftValue<_ZFP_PropType>().zfv) == ZFCompareEqual); \
    }

#define _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_UNCOMPARABLE(OuterClass, TypeName, Type) \
    zfidentity OuterClass::v_##TypeName::objectHashImpl(void) { \
        return zfidentityCalc(this); \
    } \
    ZFCompareResult OuterClass::v_##TypeName::objectCompareImpl(ZF_IN ZFObject *anotherObj) { \
        return this == anotherObj ? ZFCompareEqual : ZFCompareUncomparable; \
    } \
    zfbool OuterClass::v_##TypeName::zfvIsInit(void) { \
        return zffalse; \
    }

// ============================================================
#define _ZFP_ZFTYPEID_INNER_CONVERTER_DECLARE(OuterClass, TypeName, Type) \
    /** @brief see #ZFTYPEID_DECLARE */ \
    static zfbool TypeName##FromDataT( \
            ZF_OUT _ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull \
            ); \
    /** @brief see #ZFTYPEID_DECLARE */ \
    static inline _ZFP_PropTypeW_##TypeName TypeName##FromData( \
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
    static zfbool TypeName##ToDataT( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            ); \
    /** @brief see #ZFTYPEID_DECLARE */ \
    static inline ZFSerializableData TypeName##ToData( \
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
    static zfbool TypeName##FromStringT( \
            ZF_OUT _ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen = zfindexMax() \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ); \
    /** @brief util method to convert TypeName from string */ \
    static inline _ZFP_PropTypeW_##TypeName TypeName##FromString( \
            ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen = zfindexMax() \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        _ZFP_PropTypeW_##TypeName v; \
        TypeName##FromStringT(v, src, srcLen, errorHint); \
        return v; \
    } \
    /** @cond ZFPrivateDoc */ \
    static inline zfbool TypeName##FromStringT( \
            ZF_OUT _ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const zfstring &src \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        return TypeName##FromStringT(v, src.cString(), src.length(), errorHint); \
    } \
    static inline _ZFP_PropTypeW_##TypeName TypeName##FromString( \
            ZF_IN const zfstring &src \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        _ZFP_PropTypeW_##TypeName v; \
        TypeName##FromStringT(v, src.cString(), src.length(), errorHint); \
        return v; \
    } \
    /** @endcond */ \
    /** @brief util method to convert TypeName to string */ \
    static zfbool TypeName##ToStringT( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ); \
    /** @brief util method to convert TypeName to string */ \
    static inline zfstring TypeName##ToString( \
            ZF_IN _ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        zfstring s; \
        TypeName##ToStringT(s, v, errorHint); \
        return s; \
    }

#define _ZFP_ZFTYPEID_INNER_CONVERTER_DEFINE(OuterClass, TypeName, Type, serializeFromAction, serializeToAction, convertFromStringAction, convertToStringAction) \
    zfbool OuterClass::TypeName##FromDataT( \
            ZF_OUT OuterClass::_ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const ZFSerializableData &serializableData \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */ \
            ) { \
        ZFM_EXPAND(serializeFromAction) \
    } \
    zfbool OuterClass::TypeName##ToDataT( \
            ZF_OUT ZFSerializableData &serializableData \
            , ZF_IN OuterClass::_ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */ \
            ) { \
        ZFM_EXPAND(serializeToAction) \
    } \
    zfbool OuterClass::TypeName##FromStringT( \
            ZF_OUT OuterClass::_ZFP_PropTypeW_##TypeName &v \
            , ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */ \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        ZFM_EXPAND(convertFromStringAction) \
    } \
    zfbool OuterClass::TypeName##ToStringT( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN OuterClass::_ZFP_PropTypeW_##TypeName const &v \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        ZFM_EXPAND(convertToStringAction) \
    }

// ============================================================
#define _ZFP_ZFTYPEID_INNER_REG_IMPL(OuterClass, TypeName, Type, TypeIdSerializable_) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<OuterClass::_ZFP_PropTypeW_##TypeName> : zfextend ZFTypeInfo { \
    public: \
        typedef OuterClass::_ZFP_PropTypeW_##TypeName _ZFP_PropType; \
        typedef OuterClass::v_##TypeName _ZFP_WrapType; \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = TypeIdSerializable_, \
        }; \
        static inline const zfstring &TypeId(void) { \
            return OuterClass::ZFTypeId_##TypeName(); \
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
            return _ZFP_TidVS<_ZFP_WrapType, _ZFP_PropType>::I(obj, v); \
        } \
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
            static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
                return (zfcast(_ZFP_WrapType *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN const zfauto &obj) { \
                return zfcast(_ZFP_WrapType *, obj)->zfv; \
            } \
            static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
                return zfnull; \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
                return obj == zfnull || (zfcast(_ZFP_WrapType *, obj) != zfnull); \
            } \
            static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN const zfauto &obj) { \
                return obj == zfnull ? zfnull : &(zfcast(_ZFP_WrapType *, obj)->zfv); \
            } \
            static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
                return zfnull; \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 2> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
                return obj == zfnull || (zfcast(_ZFP_WrapType *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN const zfauto &obj) { \
                return obj == zfnull ? zfnull : (T_Access)(zfcast(_ZFP_WrapType *, obj)->zfv); \
            } \
            static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
                return zfnull; \
            } \
        }; \
    public: \
        zfoverride \
        virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const { \
            return ValueStore(obj, *(const _ZFP_PropType *)v); \
        } \
        zfoverride \
        virtual void *genericAccess(ZF_IN const zfauto &obj) const { \
            if(!Value<_ZFP_PropType>::zfvAccessAvailable(obj)) { \
                return zfnull; \
            } \
            else { \
                return _ZFP_genericAccessWrap<_ZFP_PropType>(Value<_ZFP_PropType>::zfvAccess(obj)); \
            } \
        } \
        zfoverride \
        virtual zfauto genericAccessFinish(ZF_IN const zfauto &obj, ZF_IN void *v) const { \
            return _ZFP_genericAccessFinishWrap(Value<_ZFP_PropType>::zfvAccessFinish(obj), v, _ZFP_genericAccessFinish<_ZFP_PropType>); \
        } \
        zfoverride \
        virtual ZFCoreArrayBase *genericArrayNew(void) const { \
            return ZFCoreArray<_ZFP_PropType>().refNew(); \
        } \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_INNER_REG(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_REG_IMPL(OuterClass, TypeName, Type, 1)

#define _ZFP_ZFTYPEID_INNER_ID_DATA_REGISTER(OuterClass, TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(TidReg_##OuterClass##_##TypeName) { \
        _ZFP_ZFTypeInfoRegister(OuterClass::ZFTypeId_##TypeName(), \
            zfnew(ZFTypeId<OuterClass::_ZFP_PropTypeW_##TypeName>)); \
        ZFMethodUserRegisterDetail_0(dummy, { \
                return OuterClass::ZFTypeId_##TypeName(); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , const zfchar *, zftext(ZFM_TOSTRING(ZFTypeId_##TypeName)) \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(TidReg_##OuterClass##_##TypeName) { \
        ZFMethodUserUnregister(OuterClass::ClassData()->methodForNameIgnoreParent(ZFM_TOSTRING(ZFTypeId_##TypeName))); \
        zfdelete(_ZFP_ZFTypeInfoUnregister(OuterClass::ZFTypeId_##TypeName())); \
    } \
    ZF_STATIC_REGISTER_END(TidReg_##OuterClass##_##TypeName)

// ============================================================
#define _ZFP_ZFTYPEID_INNER_METHOD_REGISTER(OuterClass, TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(TidMReg_##OuterClass##_##TypeName) { \
        ZFMethodUserRegisterDetail_4(method_FromDataT, { \
                return OuterClass::TypeName##FromDataT(v, serializableData, outErrorHint, outErrorPos); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , zfbool, zftext(ZFM_TOSTRING(TypeName##FromDataT)) \
            , ZFMP_OUT(OuterClass::_ZFP_PropTypeW_##TypeName &, v) \
            , ZFMP_IN(const ZFSerializableData &, serializableData) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull) \
            ); \
        ZFMethodUserRegisterDetail_3(method_FromData, { \
                return OuterClass::TypeName##FromData(serializableData, outErrorHint, outErrorPos); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , OuterClass::_ZFP_PropTypeW_##TypeName, zftext(ZFM_TOSTRING(TypeName##FromData)) \
            , ZFMP_IN(const ZFSerializableData &, serializableData) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull) \
            ); \
        ZFMethodUserRegisterDetail_3(method_ToDataT, { \
                return OuterClass::TypeName##ToDataT(serializableData, v, outErrorHint); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , zfbool, zftext(ZFM_TOSTRING(TypeName##ToDataT)) \
            , ZFMP_OUT(ZFSerializableData &, serializableData) \
            , ZFMP_IN(OuterClass::_ZFP_PropTypeW_##TypeName const &, v) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            ); \
        ZFMethodUserRegisterDetail_2(method_ToData, { \
                return OuterClass::TypeName##ToData(v, outErrorHint); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , ZFSerializableData, zftext(ZFM_TOSTRING(TypeName##ToData)) \
            , ZFMP_IN(OuterClass::_ZFP_PropTypeW_##TypeName const &, v) \
            , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull) \
            ); \
        ZFMethodUserRegisterDetail_4(method_FromStringT, { \
                return OuterClass::TypeName##FromStringT(v, src, srcLen, errorHint); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , zfbool, zftext(ZFM_TOSTRING(TypeName##FromStringT)) \
            , ZFMP_OUT(OuterClass::_ZFP_PropTypeW_##TypeName &, v) \
            , ZFMP_IN(const zfchar *, src) \
            , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        ZFMethodUserRegisterDetail_3(method_FromString, { \
                return OuterClass::TypeName##FromString(src, srcLen, errorHint); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , OuterClass::_ZFP_PropTypeW_##TypeName, zftext(ZFM_TOSTRING(TypeName##FromString)) \
            , ZFMP_IN(const zfchar *, src) \
            , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        ZFMethodUserRegisterDetail_3(method_ToStringT, { \
                return OuterClass::TypeName##ToStringT(s, v, errorHint); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , zfbool, zftext(ZFM_TOSTRING(TypeName##ToStringT)) \
            , ZFMP_OUT(zfstring &, s) \
            , ZFMP_IN(OuterClass::_ZFP_PropTypeW_##TypeName const &, v) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
        ZFMethodUserRegisterDetail_2(method_ToString, { \
                return OuterClass::TypeName##ToString(v, errorHint); \
            }, OuterClass::ClassData(), public, ZFMethodTypeStatic \
            , zfstring, zftext(ZFM_TOSTRING(TypeName##ToString)) \
            , ZFMP_IN(OuterClass::_ZFP_PropTypeW_##TypeName const &, v) \
            , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(TidMReg_##OuterClass##_##TypeName) { \
        const ZFClass *cls = OuterClass::ClassData(); \
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent(ZFM_TOSTRING(TypeName##FromDataT) \
                    , OuterClass::ZFTypeId_##TypeName() \
                    , ZFTypeId_ZFSerializableData() \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_ZFSerializableData() \
                    )); \
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent(ZFM_TOSTRING(TypeName##FromData) \
                    , ZFTypeId_ZFSerializableData() \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_ZFSerializableData() \
                    )); \
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent(ZFM_TOSTRING(TypeName##ToDataT) \
                    , ZFTypeId_ZFSerializableData() \
                    , OuterClass::ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent(ZFM_TOSTRING(TypeName##ToData) \
                    , OuterClass::ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent(ZFM_TOSTRING(TypeName##FromStringT) \
                    , OuterClass::ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_zfindex() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent(ZFM_TOSTRING(TypeName##FromString) \
                    , ZFTypeId_zfstring() \
                    , ZFTypeId_zfindex() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent(ZFM_TOSTRING(TypeName##ToStringT) \
                    , ZFTypeId_zfstring() \
                    , OuterClass::ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    )); \
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent(ZFM_TOSTRING(TypeName##ToString) \
                    , OuterClass::ZFTypeId_##TypeName() \
                    , ZFTypeId_zfstring() \
                    )); \
    } \
    ZF_STATIC_REGISTER_END(TidMReg_##OuterClass##_##TypeName)

#define _ZFP_ZFTYPEID_INNER_DEFINE(OuterClass, TypeName, Type, preferStringConverter) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_COMMON(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_SERIALIZABLE(OuterClass, TypeName, Type, preferStringConverter) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_COMPARABLE(OuterClass, TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_INNER_ACCESS_ONLY_REG(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_REG_IMPL(OuterClass, TypeName, Type, 0)

#define _ZFP_ZFTYPEID_INNER_ACCESS_ONLY_DEFINE(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_COMMON(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_NOT_SERIALIZABLE(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_COMPARABLE(OuterClass, TypeName, Type)

#define _ZFP_ZFTYPEID_INNER_ACCESS_ONLY_DEFINE_UNCOMPARABLE(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_COMMON(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_NOT_SERIALIZABLE(OuterClass, TypeName, Type) \
    _ZFP_ZFTYPEID_INNER_WRAPPER_DEFINE_UNCOMPARABLE(OuterClass, TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_INNER_ALIAS_DECLARE(OuterClass, AliasToTypeName, AliasToType, TypeName, Type) \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    typedef v_##AliasToTypeName v_##TypeName;

#define _ZFP_ZFTYPEID_INNER_ALIAS_REG(OuterClass, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<OuterClass::_ZFP_PropTypeW_##TypeName> : zfextend ZFTypeInfo { \
    public: \
        typedef OuterClass::_ZFP_PropTypeW_##TypeName _ZFP_PropType; \
        typedef OuterClass::v_##TypeName _ZFP_WrapType; \
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
        TypeIdValueConversion(OuterClass, AliasToTypeName, AliasToType, TypeName, Type) \
    public: \
        zfoverride \
        virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const { \
            return ValueStore(obj, *(const _ZFP_PropType *)v); \
        } \
        zfoverride \
        virtual void *genericAccess(ZF_IN const zfauto &obj) const { \
            if(!Value<_ZFP_PropType>::zfvAccessAvailable(obj)) { \
                return zfnull; \
            } \
            else { \
                return _ZFP_genericAccessWrap<_ZFP_PropType>(Value<_ZFP_PropType>::zfvAccess(obj)); \
            } \
        } \
        zfoverride \
        virtual zfauto genericAccessFinish(ZF_IN const zfauto &obj, ZF_IN void *v) const { \
            return _ZFP_genericAccessFinishWrap(Value<_ZFP_PropType>::zfvAccessFinish(obj), v, _ZFP_genericAccessFinish<_ZFP_PropType>); \
        } \
        zfoverride \
        virtual ZFCoreArrayBase *genericArrayNew(void) const { \
            return ZFCoreArray<_ZFP_PropType>().refNew(); \
        } \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_INNER_ALIAS_DEFINE(OuterClass, AliasToTypeName, AliasToType, TypeName, Type)

#define _ZFP_ZFTYPEID_INNER_ALIAS_VALUE_ACCESS_DEFAULT(OuterClass, AliasToTypeName, AliasToType, TypeName, Type) \
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
            static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
                return (obj != zfnull && ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessAvailable(obj)); \
            } \
            static T_Access zfvAccess(ZF_IN const zfauto &obj) { \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccess(obj); \
                _ZFP_PropType *v = zfpoolNew(_ZFP_PropType); \
                *v = (_ZFP_PropType)aliasValue; \
                _ZFP_PropAliasAttach(obj, v \
                    , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                    , _ZFP_PropAliasOnDetach \
                    ); \
                ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessFinish(obj); \
                return *v; \
            } \
            static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
                if(obj) { \
                    return _ZFP_PropAliasDetach(obj \
                        , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                        ); \
                } \
                else { \
                    return zfnull; \
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
                zfpoolDelete(vTmp); \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> { \
        private: \
             typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef; \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
                return (obj != zfnull && ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessAvailable(obj)); \
            } \
            static _TrNoRef zfvAccess(ZF_IN const zfauto &obj) { \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccess(obj); \
                _ZFP_PropType *v = zfpoolNew(_ZFP_PropType); \
                *v = (_ZFP_PropType)aliasValue; \
                _TrNoRef *p = zfpoolNew(_TrNoRef); \
                *p = v; \
                _ZFP_PropAliasAttach(obj, p \
                    , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                    , _ZFP_PropAliasOnDetach \
                    ); \
                ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessFinish(obj); \
                return *p; \
            } \
            static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
                if(obj) { \
                    return _ZFP_PropAliasDetach(obj \
                        , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                        ); \
                } \
                else { \
                    return zfnull; \
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
                zfpoolDelete(vTmp); \
                zfpoolDelete(p); \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 2> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
                return obj == zfnull || (ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessAvailable(obj)); \
            } \
            static T_Access zfvAccess(ZF_IN const zfauto &obj) { \
                if(obj == zfnull) { \
                    return zfnull; \
                } \
                AliasToType const &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccess(obj); \
                _ZFP_PropType *v = zfpoolNew(_ZFP_PropType); \
                *v = (_ZFP_PropType)aliasValue; \
                _ZFP_PropAliasAttach(obj, v \
                    , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                    , _ZFP_PropAliasOnDetach \
                    ); \
                ZFTypeId<AliasToType>::Value<AliasToType const &>::zfvAccessFinish(obj); \
                return *v; \
            } \
            static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
                if(obj) { \
                    return _ZFP_PropAliasDetach(obj \
                        , zfstr("_ZFP_PropAlias:%s:%s", #TypeName, zftTraits<T_Access>::ModifierName()) \
                        ); \
                } \
                else { \
                    return zfnull; \
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
                zfpoolDelete(vTmp); \
            } \
        };

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdDeclare_inner_h_

