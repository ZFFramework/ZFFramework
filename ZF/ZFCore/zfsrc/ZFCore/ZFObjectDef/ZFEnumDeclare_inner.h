/**
 * @file ZFEnumDeclare_inner.h
 * @brief ZFEnum declare impl
 */

#ifndef _ZFI_ZFEnumDeclare_inner_h_
#define _ZFI_ZFEnumDeclare_inner_h_

#include "ZFObjectClassTypeFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_BEGIN(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_BEGIN(OuterClass, EnumName)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_VALUE(Value) \
    _ZFP_ZFENUM_INNER_VALUE(Value)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_VALUE_WITH_INIT(Value, initValue) \
    _ZFP_ZFENUM_INNER_VALUE_WITH_INIT(Value, initValue)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_SEPARATOR() \
    _ZFP_ZFENUM_INNER_SEPARATOR(zffalse)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_SEPARATOR_ALLOW_DUPLICATE_VALUE() \
    _ZFP_ZFENUM_INNER_SEPARATOR(zftrue)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_VALUE_REGISTER_WITH_NAME(Value, Name) \
    _ZFP_ZFENUM_INNER_VALUE_REGISTER_WITH_NAME(Value, Name)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_VALUE_REGISTER(Value) \
    _ZFP_ZFENUM_INNER_VALUE_REGISTER_WITH_NAME(Value, zftext(#Value))

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_END(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_END(OuterClass, EnumName)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_END_WITH_DEFAULT(OuterClass, EnumName, defaultEnum) \
    _ZFP_ZFENUM_INNER_END_WITH_DEFAULT(OuterClass, EnumName, defaultEnum)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_END_FLAGS(OuterClass, EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_INNER_END_FLAGS(OuterClass, EnumName, EnumFlagsName)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_END_FLAGS_WITH_DEFAULT(OuterClass, EnumName, EnumFlagsName, defaultEnum, defaultEnumFlags) \
    _ZFP_ZFENUM_INNER_END_FLAGS_WITH_DEFAULT(OuterClass, EnumName, EnumFlagsName, defaultEnum, defaultEnumFlags)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_REG(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_TYPEID_REG(OuterClass, EnumName)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_REG_FLAGS(OuterClass, EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_INNER_TYPEID_REG(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_FLAGS_TYPEID_REG(OuterClass, EnumName, EnumFlagsName)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_DEFINE(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_DEFINE(OuterClass, EnumName)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_INNER_DEFINE_FLAGS(OuterClass, EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_INNER_DEFINE_FLAGS(OuterClass, EnumName, EnumFlagsName)

// ============================================================
#define _ZFP_ZFENUM_INNER_BEGIN(OuterClass, EnumName) \
    zfclass v_##EnumName : zfextend ZFEnum { \
        ZFOBJECT_DECLARE(v_##EnumName, ZFEnum, OuterClass) \
    public: \
        /** @brief see @ref v_##EnumName */ \
        typedef enum {

#define _ZFP_ZFENUM_INNER_VALUE(Value) /** \n */ e_##Value,
#define _ZFP_ZFENUM_INNER_VALUE_WITH_INIT(Value, initValue) /** @brief \n Value(initValue)\n */ e_##Value = initValue,

#define _ZFP_ZFENUM_INNER_SEPARATOR(isEnableDuplicateValue_) \
            /** @brief max enum value */ \
            ZFEnumCount, \
            /* used to ensure sizeof(enum) == sizeof(zfuint) */ \
            /* required for enum value reinterpret cast (ZFTAG_TRICKS: EnumReinterpretCast) */ \
            _ZFP_ZFEnumMax = ((zfuint)-1), \
        } ZFEnumType; \
    public: \
        /** @brief get the count of enum value */ \
        static zfindex EnumCount(void) { \
            return zfself::_ZFP_ZFEnumDataRef()->enumCount(); \
        } \
        /** @brief get the index of enum value, or zfindexMax() if no such enum value */ \
        static zfindex EnumIndexForValue(ZF_IN zfuint value) { \
            return zfself::_ZFP_ZFEnumDataRef()->enumIndexForValue(value); \
        } \
        /** @brief get the value at index, or ZFEnumInvalid if not exist */ \
        static zfuint EnumValueAt(ZF_IN zfindex index) { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueAt(index); \
        } \
        /** @brief get the name at index, or ZFEnumNameInvalid if not exist */ \
        static const zfstring &EnumNameAt(ZF_IN zfindex index) { \
            return zfself::_ZFP_ZFEnumDataRef()->enumNameAt(index); \
        } \
        /** @brief return true if contain the specified value */ \
        static zfbool EnumValueContain(ZF_IN zfuint value) { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueContain(value); \
        } \
        /** @brief get value with name, or ZFEnumInvalid if not exist */ \
        static zfuint EnumValueForName(ZF_IN const zfstring &name) { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueForName(name); \
        } \
        /** @brief get name with value, or ZFEnumNameInvalid if not exist */ \
        static const zfstring &EnumNameForValue(ZF_IN zfuint value) { \
            return zfself::_ZFP_ZFEnumDataRef()->enumNameForValue(value); \
        } \
    public: \
        /** @cond ZFPrivateDoc */ \
        zfoverride \
        virtual zfindex enumCount(void) { \
            return zfself::EnumCount(); \
        } \
        zfoverride \
        virtual zfuint enumDefault(void) { \
            return zfself::EnumDefault(); \
        } \
        zfoverride \
        virtual zfbool enumIsFlags(void) { \
            return zfself::EnumIsFlags(); \
        } \
        zfoverride \
        virtual zfindex enumIndexForValue(ZF_IN zfuint value) { \
            return zfself::EnumIndexForValue(value); \
        } \
        zfoverride \
        virtual zfuint enumValueAt(ZF_IN zfindex index) { \
            return zfself::EnumValueAt(index); \
        } \
        zfoverride \
        virtual const zfstring &enumNameAt(ZF_IN zfindex index) { \
            return zfself::EnumNameAt(index); \
        } \
        zfoverride \
        virtual zfbool enumValueContain(ZF_IN zfuint value) { \
            return zfself::EnumValueContain(value); \
        } \
        zfoverride \
        virtual zfuint enumValueForName(ZF_IN const zfstring &name) { \
            return zfself::EnumValueForName(name); \
        } \
        zfoverride \
        virtual const zfstring &enumNameForValue(ZF_IN zfuint value) { \
            return zfself::EnumNameForValue(value); \
        } \
        /** @endcond */ \
    public: \
        static const _ZFP_ZFEnumData *_ZFP_ZFEnumDataRef(void) { \
            static _ZFP_ZFEnumDataHolder d(_ZFP_ZFEnumDataInit()); \
            return d.d; \
        } \
        static const _ZFP_ZFEnumData *_ZFP_ZFEnumDataInit(void) { \
            ZFCoreMutexLocker(); \
            _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataAccess(zfself::ClassData()); \
            if(d->needInitFlag) { \
                d->needInitFlag = zffalse; \
                zfbool isEnableDuplicateValue = isEnableDuplicateValue_;

#define _ZFP_ZFENUM_INNER_VALUE_REGISTER_WITH_NAME(Value, Name) \
                d->add(isEnableDuplicateValue, zfself::e_##Value, Name);

#define _ZFP_ZFENUM_INNER_END(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_END_DETAIL(OuterClass, EnumName, zffalse, d->enumValueAt(0))
#define _ZFP_ZFENUM_INNER_END_WITH_DEFAULT(OuterClass, EnumName, defaultEnum) \
    _ZFP_ZFENUM_INNER_END_DETAIL(OuterClass, EnumName, zffalse, zfself::e_##defaultEnum)

#define _ZFP_ZFENUM_INNER_END_FLAGS(OuterClass, EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_INNER_END_DETAIL(OuterClass, EnumName, zftrue, d->enumValueAt(0)) \
    _ZFP_ZFENUM_INNER_FLAGS_DECLARE(OuterClass, EnumName, EnumFlagsName, v_##EnumName::EnumDefault())
#define _ZFP_ZFENUM_INNER_END_FLAGS_WITH_DEFAULT(OuterClass, EnumName, EnumFlagsName, defaultEnum, defaultEnumFlags) \
    _ZFP_ZFENUM_INNER_END_DETAIL(OuterClass, EnumName, zftrue, zfself::e_##defaultEnum) \
    _ZFP_ZFENUM_INNER_FLAGS_DECLARE(OuterClass, EnumName, EnumFlagsName, defaultEnumFlags)

#define _ZFP_ZFENUM_INNER_END_DETAIL(OuterClass, EnumName, IsFlags, defaultEnum) \
                d->enumDefault = (zfuint)(defaultEnum); \
                d->enumIsFlags = (IsFlags); \
            } \
            return d; \
        } \
    public: \
        /** @brief see #ZFEnum::enumIsFlags */ \
        static inline zfbool EnumIsFlags(void) { \
            return (v_##EnumName::ZFEnumType)zfself::_ZFP_ZFEnumDataRef()->enumIsFlags; \
        } \
        /** @brief default value for @ref EnumName */ \
        static inline v_##EnumName::ZFEnumType EnumDefault(void) { \
            return (v_##EnumName::ZFEnumType)zfself::_ZFP_ZFEnumDataRef()->enumDefault; \
        } \
    public: \
        /** @brief util method to access original enum value */ \
        zffinal zfself::ZFEnumType zfv(void) { \
            return (zfself::ZFEnumType)this->enumValue(); \
        } \
    public: \
        zfoverride \
        virtual const zfstring &zfvTypeId(void); \
    }; \
    /** @brief see @ref v_##EnumName */ \
    typedef v_##EnumName::ZFEnumType EnumName; \
    _ZFP_ZFENUM_INNER_CONVERTER_DECLARE(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_TYPEID_DECLARE(OuterClass, EnumName)

// ============================================================
#define _ZFP_ZFENUM_INNER_DEFINE(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_CONVERTER_DEFINE(OuterClass, EnumName) \
    ZFOBJECT_REGISTER(OuterClass, v_##EnumName) \
    _ZFP_ZFENUM_INNER_TYPEID_DEFINE(OuterClass, EnumName) \
    ZF_STATIC_REGISTER_INIT(EnumReg_##OuterClass##_##EnumName) { \
        for(zfindex i = 0; i < OuterClass::v_##EnumName::EnumCount(); ++i) { \
            ZFMethodUserRegisterDetail_0(resultMethod, { \
                    return (OuterClass::EnumName)OuterClass::v_##EnumName::EnumValueForName(invokerMethod->methodName() + 2); \
                }, OuterClass::v_##EnumName::ClassData(), public, ZFMethodTypeStatic, \
                OuterClass::EnumName, zfstr("e_%s", OuterClass::v_##EnumName::EnumNameAt(i))); \
            _m.add(resultMethod); \
        } \
        _ZFP_ZFEnumMethodReg(_m, OuterClass::v_##EnumName::_ZFP_ZFEnumDataRef()); \
    } \
    ZF_STATIC_REGISTER_DESTROY(EnumReg_##OuterClass##_##EnumName) { \
        for(zfindex i = 0; i < _m.count(); ++i) { \
            ZFMethodUserUnregister(_m[i]); \
        } \
    } \
    ZFCoreArray<const ZFMethod *> _m; \
    ZF_STATIC_REGISTER_END(EnumReg_##OuterClass##_##EnumName)

#define _ZFP_ZFENUM_INNER_DEFINE_FLAGS(OuterClass, EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_INNER_DEFINE(OuterClass, EnumName) \
    _ZFP_ZFENUM_INNER_FLAGS_DEFINE(OuterClass, EnumName, EnumFlagsName)

// ============================================================
#define _ZFP_ZFENUM_INNER_CONVERTER_DECLARE(OuterClass, EnumName) \
    /** @brief see @ref v_##EnumName, return enum object if success */ \
    static zfbool EnumName##FromStringT( \
            ZF_OUT zfauto &ret \
            , ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen = zfindexMax() \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ); \
    /** @brief see @ref EnumName, return empty string if error */ \
    static zfbool EnumName##ToStringT( \
            ZF_IN_OUT zfstring &ret \
            , ZF_IN v_##EnumName *const &value \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ); \
    /** @brief see @ref v_##EnumName, return empty string if error */ \
    static inline zfstring EnumName##ToString( \
            ZF_IN v_##EnumName *const &value \
            , ZF_OUT_OPT zfstring *errorHint = zfnull \
            ) { \
        zfstring ret; \
        EnumName##ToStringT(ret, value, errorHint); \
        return ret; \
    }
#define _ZFP_ZFENUM_INNER_CONVERTER_DEFINE(OuterClass, EnumName) \
    zfbool OuterClass::EnumName##ToStringT( \
            ZF_IN_OUT zfstring &ret \
            , ZF_IN OuterClass::v_##EnumName *const &value \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        ret += ((value == zfnull) ? zfstring() : value->enumName()); \
        return zftrue; \
    } \
    zfbool OuterClass::EnumName##FromStringT( \
            ZF_OUT zfauto &ret \
            , ZF_IN const zfchar *src \
            , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */ \
            , ZF_OUT_OPT zfstring *errorHint /* = zfnull */ \
            ) { \
        if(zfstringIsEqual(src, srcLen, ZFEnumNameInvalid(), zfindexMax())) { \
            ret = zfobj<v_##EnumName>(ZFEnumInvalid()); \
            return zftrue; \
        } \
        zfuint tmpValue = v_##EnumName::EnumValueForName( \
            (srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString()); \
        if(tmpValue == ZFEnumInvalid()) { \
            zfstringAppend(errorHint, "no enum named: \"%s\"", zfstring(src, srcLen)); \
            return zffalse; \
        } \
        else { \
            ret = zfobj<v_##EnumName>(tmpValue); \
            return zftrue; \
        } \
    }

// ============================================================
#define _ZFP_ZFENUM_INNER_FLAGS_DECLARE(OuterClass, EnumName, EnumFlagsName, defaultValue) \
    /** @brief see @ref v_##EnumName, @ref EnumFlagsName##ToString, @ref EnumFlagsName##FromString */ \
    zffinal zfclassPOD EnumFlagsName { \
        /* this class should be POD-like to support enum value reinterpret cast (ZFTAG_TRICKS: EnumReinterpretCast) */ \
    public: \
        /** @brief default value for EnumFlagsName */ \
        static zfuint EnumDefault(void) { \
            return defaultValue; \
        } \
    public: \
        /** @cond ZFPrivateDoc */ \
        EnumFlagsName(void) : flags(0) {} \
        EnumFlagsName(ZF_IN zfuint const &flags) : flags(flags) {} \
        EnumFlagsName(ZF_IN EnumName const &flags) : flags((zfuint)flags) {} \
        EnumFlagsName(ZF_IN EnumFlagsName const &ref) : flags(ref.flags) {} \
    public: \
        zfuint const &enumValue(void) const {return this->flags;} \
        void enumValue(ZF_IN zfuint const &flags) {this->flags = flags;} \
        void enumValue(ZF_IN EnumName const &flags) {this->flags = (zfuint)flags;} \
    public: \
        operator zfuint const & (void) const {return this->flags;} \
        EnumFlagsName &operator = (ZF_IN zfuint const &flags) {this->flags = flags; return *this;} \
        EnumFlagsName &operator = (ZF_IN EnumName const &flags) {this->flags = (zfuint)flags; return *this;} \
        EnumFlagsName &operator = (ZF_IN EnumFlagsName const &ref) {this->flags = ref.flags; return *this;} \
        zfbool operator == (ZF_IN zfuint const &flags) const {return (this->flags == flags);} \
        zfbool operator == (ZF_IN EnumName const &flags) const {return (this->flags == (zfuint)flags);} \
        zfbool operator == (ZF_IN EnumFlagsName const &ref) const {return (this->flags == ref.flags);} \
        zfbool operator != (ZF_IN zfuint const &flags) const {return (this->flags != flags);} \
        zfbool operator != (ZF_IN EnumName const &flags) const {return (this->flags != (zfuint)flags);} \
        zfbool operator != (ZF_IN EnumFlagsName const &ref) const {return (this->flags != ref.flags);} \
        EnumFlagsName &operator |= (ZF_IN zfuint const &flags) {this->flags |= flags; return *this;} \
        EnumFlagsName &operator |= (ZF_IN EnumName const &flags) {this->flags |= (zfuint)flags; return *this;} \
        EnumFlagsName &operator |= (ZF_IN EnumFlagsName const &ref) {this->flags |= ref.flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN zfuint const &flags) {this->flags &= flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN EnumName const &flags) {this->flags &= (zfuint)flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN EnumFlagsName const &ref) {this->flags &= ref.flags; return *this;} \
    public: \
        void objectInfoT(ZF_IN_OUT zfstring &ret) const; \
        inline zfstring objectInfo(void) const { \
            zfstring ret; \
            this->objectInfoT(ret); \
            return ret; \
        } \
        /** @endcond */ \
    private: \
        zfuint flags; \
    }; \
    _ZFP_ZFENUM_INNER_FLAGS_TYPEID_DECLARE(OuterClass, EnumName, EnumFlagsName)

#define _ZFP_ZFENUM_INNER_FLAGS_DEFINE(OuterClass, EnumName, EnumFlagsName) \
    void OuterClass::EnumFlagsName::objectInfoT(ZF_IN_OUT zfstring &ret) const { \
        zfflagsToStringT(ret, v_##EnumName::ClassData(), (zfflags)this->enumValue()); \
    } \
    _ZFP_ZFENUM_INNER_FLAGS_TYPEID_DEFINE(OuterClass, EnumName, EnumFlagsName)


// ============================================================
// normal enum
#define _ZFP_ZFENUM_INNER_TYPEID_DECLARE(OuterClass, EnumName) \
    ZFTYPEID_INNER_DECLARE_WITH_CUSTOM_WRAPPER(OuterClass, EnumName, EnumName)
#define _ZFP_ZFENUM_INNER_TYPEID_REG(OuterClass, EnumName) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<OuterClass::EnumName> : zfextend ZFTypeInfo { \
    public: \
        typedef OuterClass::EnumName _ZFP_PropType; \
        typedef OuterClass::v_##EnumName _ZFP_WrapType; \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 1, \
        }; \
        static inline const zfstring &TypeId(void) { \
            return OuterClass::ZFTypeId_##EnumName(); \
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
                , ZF_IN zfuint const &v \
                ) { \
            ZFCoreMutexLock(); \
            _ZFP_WrapType *t = zfunsafe_zfAlloc(_ZFP_WrapType, v); \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            ZFCoreMutexUnlock(); \
            return zftrue; \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN _ZFP_PropType const &v \
                ) { \
            ZFCoreMutexLock(); \
            _ZFP_WrapType *t = zfunsafe_zfAlloc(_ZFP_WrapType, (zfuint)v); \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            ZFCoreMutexUnlock(); \
            return zftrue; \
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
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (zfcast(_ZFP_WrapType *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                /* ZFTAG_TRICKS: EnumReinterpretCast */ \
                return *(typename zftTraits<T_Access>::TrNoRef *)(&(zfcast(_ZFP_WrapType *, obj)->_ZFP_ZFEnum_value)); \
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
                if(obj == zfnull) { \
                    return zfnull; \
                } \
                else { \
                    _ZFP_WrapType *t = zfcast(_ZFP_WrapType *, obj); \
                    /* ZFTAG_TRICKS: EnumReinterpretCast */ \
                    return (typename zftTraits<T_Access>::TrNoRef)(&(t->_ZFP_ZFEnum_value)); \
                } \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
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
    /** @endcond */ \
    ZFOUTPUT_TYPE(OuterClass::EnumName, {s += OuterClass::v_##EnumName::EnumNameForValue(v);})

#define _ZFP_ZFENUM_INNER_TYPEID_DEFINE(OuterClass, EnumName) \
    ZFTYPEID_INNER_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(OuterClass, EnumName, EnumName, { \
            if(zfstringIsEqual(src, srcLen, ZFEnumNameInvalid(), zfindexMax())) { \
                v = (EnumName)ZFEnumInvalid(); \
                return zftrue; \
            } \
            v = (EnumName)v_##EnumName::EnumValueForName( \
                    (srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString() \
                ); \
            if(v == ZFEnumInvalid()) { \
                if(errorHint) { \
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen)); \
                } \
                return zffalse; \
            } \
            return zftrue; \
        }, { \
            s += v_##EnumName::EnumNameForValue(v); \
            return zftrue; \
        }) \
    const zfstring &OuterClass::v_##EnumName::zfvTypeId(void) { \
        return ZFTypeId_##EnumName(); \
    }


// ============================================================
// enum flags
#define _ZFP_ZFENUM_INNER_FLAGS_TYPEID_DECLARE(OuterClass, EnumName, EnumFlagsName) \
    ZFTYPEID_INNER_DECLARE_WITH_CUSTOM_WRAPPER(OuterClass, EnumFlagsName, EnumFlagsName) \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    typedef v_##EnumName v_##EnumFlagsName;
#define _ZFP_ZFENUM_INNER_FLAGS_TYPEID_REG(OuterClass, EnumName, EnumFlagsName) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<OuterClass::EnumFlagsName> : zfextend ZFTypeInfo { \
    public: \
        typedef OuterClass::EnumName _ZFP_PropTypeOrig; \
        typedef OuterClass::EnumFlagsName _ZFP_PropType; \
        typedef OuterClass::v_##EnumFlagsName _ZFP_WrapType; \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 1, \
        }; \
        static inline const zfstring &TypeId(void) { \
            return OuterClass::ZFTypeId_##EnumFlagsName(); \
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
                , ZF_IN zfuint const &v \
                ) { \
            ZFCoreMutexLock(); \
            _ZFP_WrapType *t = zfunsafe_zfAlloc(_ZFP_WrapType, v); \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            ZFCoreMutexUnlock(); \
            return zftrue; \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN _ZFP_PropTypeOrig const &v \
                ) { \
            ZFCoreMutexLock(); \
            _ZFP_WrapType *t = zfunsafe_zfAlloc(_ZFP_WrapType, (zfuint)v); \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            ZFCoreMutexUnlock(); \
            return zftrue; \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN _ZFP_PropType const &v \
                ) { \
            ZFCoreMutexLock(); \
            _ZFP_WrapType *t = zfunsafe_zfAlloc(_ZFP_WrapType, (zfuint)v); \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            ZFCoreMutexUnlock(); \
            return zftrue; \
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
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (zfcast(_ZFP_WrapType *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                /* ZFTAG_TRICKS: EnumReinterpretCast */ \
                return *(typename zftTraits<T_Access>::TrNoRef *)(&(zfcast(_ZFP_WrapType *, obj)->_ZFP_ZFEnum_value)); \
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
                if(obj == zfnull) { \
                    return zfnull; \
                } \
                else { \
                    _ZFP_WrapType *t = zfcast(_ZFP_WrapType *, obj); \
                    /* ZFTAG_TRICKS: EnumReinterpretCast */ \
                    return (typename zftTraits<T_Access>::TrNoRef)(&(t->_ZFP_ZFEnum_value)); \
                } \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
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
    /** @endcond */ \
    ZFOUTPUT_TYPE(OuterClass::EnumFlagsName, {v.objectInfoT(s);})

#define _ZFP_ZFENUM_INNER_FLAGS_TYPEID_DEFINE(OuterClass, EnumName, EnumFlagsName) \
    ZFTYPEID_INNER_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(OuterClass, EnumFlagsName, EnumFlagsName, { \
            zfflags flags = 0; \
            if(!zfflagsFromStringT(flags, \
                        v_##EnumName::ClassData(), \
                        src, srcLen)) { \
                if(errorHint) { \
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen)); \
                } \
                return zffalse; \
            } \
            v.enumValue((zfuint)flags); \
            return zftrue; \
        }, { \
            return zfflagsToStringT(s, v_##EnumName::ClassData(), (zfflags)v.enumValue()); \
        }) \
    ZF_STATIC_REGISTER_INIT(EnumReg_##OuterClass##_##EnumFlagsName) { \
        ZFClassAlias(OuterClass::v_##EnumName::ClassData(), OuterClass::v_##EnumName::ClassData()->classNamespace() \
                ? zfstr("%s.%s", OuterClass::v_##EnumName::ClassData()->classNamespace(), #EnumFlagsName).cString() \
                : #EnumFlagsName \
                ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(EnumReg_##OuterClass##_##EnumFlagsName) { \
        ZFClassAliasRemove(OuterClass::v_##EnumName::ClassData(), OuterClass::v_##EnumName::ClassData()->classNamespace() \
                ? zfstr("%s.%s", OuterClass::v_##EnumName::ClassData()->classNamespace(), #EnumFlagsName).cString() \
                : #EnumFlagsName \
                ); \
    } \
    ZF_STATIC_REGISTER_END(EnumReg_##OuterClass##_##EnumFlagsName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnumDeclare_inner_h_

