/**
 * @file ZFTypeIdFwd.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeIdFwd_h_
#define _ZFI_ZFTypeIdFwd_h_

#include "zfautoFwd.h"
#include "zfany.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief shows that the property has no type and can not be used in advanced property copy operation,
 *   ensured empty string
 */
#define ZFTypeId_none() ""

// ============================================================
zfclassFwd ZFSerializableData;
/**
 * @brief base protocol for #ZFTypeId
 */
zfclassNotPOD ZFLIB_ZFCore ZFTypeInfo {
public:
    virtual ~ZFTypeInfo(void) {}

public:
    /**
     * @brief see #ZFTypeId::TypeIdSerializable
     */
    virtual zfbool typeIdSerializable(void) const zfpurevirtual;
    /**
     * @brief the type id
     */
    virtual const zfstring &typeId(void) const zfpurevirtual;
    /**
     * @brief return the proper wrapper type class if available
     *
     * if available:
     * -  if the type is ZFObject type, the object's class would be returned
     * -  if the type is not ZFObject type,
     *   a proper #ZFTypeIdWrapper would be returned
     */
    virtual const ZFClass *typeIdClass(void) const zfpurevirtual;

public:
    /**
     * @brief advanced generic value access
     *
     * note:
     * -  this is raw access without type check,
     *   you must ensure the access is valid
     * -  since aliased type (#ZFTYPEID_ALIAS_DECLARE) share same base #ZFTypeInfo,
     *   the type returned from #ZFTypeInfoForName won't work and always result to crash for aliased type,
     *   it's recommended to access by creating new instance of #ZFTypeId,
     *   instead of access by #ZFTypeInfoForName
     *
     * when access:
     * -  for ZFObject type
     *   -  obj: should store the ZFObject itself
     *   -  v: must be a pointer to `T_Type` of `ZFTypeId<T_Type>`,
     *     e.g. `zfauto`, `zfweakT<SomeZFObject>`, `SomeZFObject *`
     * -  for non-ZFObject type
     *   -  obj: should store the proper #ZFTypeIdWrapper
     *   -  v: must be a pointer to `T_Type` of `ZFTypeId<T_Type>`
     */
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const zfpurevirtual;
    /** @brief see #genericValueStore */
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const zfpurevirtual;
    /** @brief see #genericValueStore */
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const zfpurevirtual;

    /**
     * @brief create generic array type
     */
    virtual ZFCoreArrayBase *genericArrayNew(void) const zfpurevirtual;
};

// ============================================================
extern ZFLIB_ZFCore void _ZFP_ZFTypeInfoRegister(
        ZF_IN const zfstring &typeId
        , ZF_IN ZFTypeInfo *typeIdData
        );
extern ZFLIB_ZFCore ZFTypeInfo *_ZFP_ZFTypeInfoUnregister(ZF_IN const zfstring &typeId);
/**
 * @brief access type id data
 */
extern ZFLIB_ZFCore const ZFTypeInfo *ZFTypeInfoForName(ZF_IN const zfstring &typeId);
/** @brief see #ZFTypeInfoGetAll */
extern ZFLIB_ZFCore void ZFTypeInfoGetAllT(ZF_IN_OUT ZFCoreArray<const ZFTypeInfo *> &ret);
/**
 * @brief access type id data
 */
inline ZFCoreArray<const ZFTypeInfo *> ZFTypeInfoGetAll(void) {
    ZFCoreArray<const ZFTypeInfo *> ret;
    ZFTypeInfoGetAllT(ret);
    return ret;
}

// ============================================================
template<typename T_Dummy>
zfclassNotPOD _ZFP_ZFTypeIdRegCk {
};
/**
 * @brief type data traits for #ZFTYPEID_DECLARE
 *
 * for advanced reflection use only\n
 * type must be registered by #ZFTYPEID_DECLARE before use this template,
 * and T_Type must be plain type without reference,
 * use #zftTraits::TrNoRef if necessary
 */
template<typename T_Type
        , int T_isZFObject = zftIsZFObject(typename zftTraits<T_Type>::TrType)
        , int T_isPointer = zftTraits<T_Type>::TrIsPtr
    >
zfclassNotPOD ZFTypeId : zfextend ZFTypeInfo {
public:
    /** @cond ZFPrivateDoc */
    typedef typename _ZFP_ZFTypeIdRegCk<T_Type>::TypeNotRegisteredBy_ZFTYPEID _TypeChecker;
    /** @endcond */
public:
    enum {
        /**
         * @brief whether this type already registered
         *
         * type is registered if declared by any of these:
         * -  #ZFTYPEID_DECLARE
         * -  #ZFTYPEID_ACCESS_ONLY_DECLARE
         * -  #ZFTYPEID_ALIAS_DECLARE
         * -  #ZFTYPEID_DISABLE
         */
        TypeIdRegistered = 0,
        /**
         * @brief whether this type can be serialized
         *
         * type is serializable if declared by any of these:
         * -  #ZFTYPEID_DECLARE
         * -  #ZFTYPEID_ALIAS_DECLARE
         */
        TypeIdSerializable = 0,
    };

    /**
     * @brief get type id, or return #ZFTypeId_none if not registered
     */
    static const zfstring &TypeId(void);

    /**
     * @brief store the value to wrapper object
     *
     * the stored value can be further accessed by #ZFTypeId::Value
     */
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN T_Type const &v
            );

    /**
     * @brief try access as raw value
     *
     * this method used to convert types from/to ZFObject without knowing its actual type,
     * currently used by #ZFMethod::methodGenericInvoker\n
     * \n
     * types must be registered by #ZFTYPEID_DECLARE or #ZFTYPEID_ALIAS_DECLARE,
     * which would automatically declare a wrapper object type for you\n
     * assume your type's name is "YourType",
     * then there would be a wrapper class named "v_YourType",
     * with only one plain member variable named "zfv":
     * @code
     *   zfclass v_YourType : zfextend ZFTypeIdWrapper {
     *       ZFOBJECT_DECLARE(v_YourType, ZFTypeIdWrapper)
     *       ZFALLOC_CACHE_RELEASE({
     *           cache->zfvReset();
     *       })
     *   public:
     *       YourType zfv;
     *   };
     *
     *   // this should return "YourType" instead of "v_YourType"
     *   // see ZFClass::classForName for more info
     *   const zfchar *clsName = v_YourType::ClassData()->className();
     * @endcode
     * \n
     * this method may or may not access the original value\n
     * must first check whether it's available to access
     * by #ZFTypeId::Value::zfvAccessAvailable\n
     * after access, we may (not ensured) call #zfvAccessFinish
     * to allow impl to do some cleanup steps\n
     * \n
     * \n
     * (ZFTAG_LIMITATION) detailed limitation:\n
     * -  aliased type (#ZFTYPEID_ALIAS_DECLARE) may create a temporary object
     *   to hold the actual type,
     *   the temp object would be cleared during zfvAccessFinish,
     *   and the impl value would be copied back to original value during zfvAccessFinish,
     *   thus, the reflection performance of aliased type may be much worse than original type
     * -  when access aliased type as return value,
     *   the zfvAccessFinish won't be called until the original holder object deallocated
     */
    template<typename T_Access = T_Type>
    zfclassNotPOD Value {
    public:
        /** @brief try access as raw value, see #ZFTypeId::Value */
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj);
        /** @brief try access as raw value, see #ZFTypeId::Value */
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj);
        /** @brief finish access as raw value, see #ZFTypeId::Value */
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj);
    };
    /*
     * if available, the templates above should handle these types,
     * and access as proper type:
     * -  Type
     * -  Type &
     * -  Type const &
     * -  Type *
     * -  Type * &
     * -  const Type *
     * -  const Type * &
     * -  Type * const &
     * -  const Type * const &
     */
};

// ============================================================
/**
 * @brief manually register a custom type id
 *
 * you must have these things available before register:
 * -  `const zfstring &ZFTypeId_YourTypeName()`
 * -  specialization for `ZFTypeId<YourType>`
 */
#define ZFTYPEID_ID_DATA_REGISTER(TypeName, Type) \
    ZF_STATIC_REGISTER_INIT(PropTIReg_##TypeName) { \
        typedef Type _ZFP_PropTypeW2_##TypeName; \
        _ZFP_ZFTypeInfoRegister(ZFTypeId_##TypeName(), \
            zfnew(ZFTypeId<_ZFP_PropTypeW2_##TypeName>)); \
        ZFMethodFuncUserRegister_0(dummy, { \
                return ZFTypeId_##TypeName(); \
            }, ZF_NAMESPACE_CURRENT(), const zfchar *, zftext(ZFM_TOSTRING(ZFTypeId_##TypeName))); \
    } \
    ZF_STATIC_REGISTER_DESTROY(PropTIReg_##TypeName) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(zfnull, ZFM_TOSTRING(ZFTypeId_##TypeName))); \
        zfdelete(_ZFP_ZFTypeInfoUnregister(ZFTypeId_##TypeName())); \
    } \
    ZF_STATIC_REGISTER_END(PropTIReg_##TypeName)

// ============================================================
zfclassFwd ZFProgressable;
typedef zfbool (*_ZFP_ZFTypeIdProgressUpdate)(
        ZF_IN_OUT ZFProgressable *ret
        , ZF_IN ZFProgressable *from
        , ZF_IN ZFProgressable *to
        , ZF_IN zffloat progress
        );
#define _ZFP_ZFTYPEID_WRAPPER_DECLARE(ZFLIB_, TypeName, Type) \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    zfclass ZFLIB_ v_##TypeName : zfextend ZFTypeIdWrapper { \
        ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(v_##TypeName, ZFTypeIdWrapper) \
        ZFALLOC_CACHE_RELEASE({ \
            cache->zfvReset(); \
        }) \
    public: \
        /** @brief the value, see #ZFTypeId::Value */ \
        _ZFP_PropTypeW_##TypeName zfv; \
    protected: \
        v_##TypeName(void) : zfv() {} \
    protected: \
        /** @brief init with value */ \
        virtual void objectOnInit(ZF_IN _ZFP_PropTypeW_##TypeName const &value) { \
            this->objectOnInit(); \
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
        virtual void wrappedValue(ZF_IN const void *v) {this->zfv = *(const _ZFP_PropTypeW_##TypeName *)v;} \
        zfoverride \
        virtual void wrappedValueCopy(ZF_IN void *v) {*(_ZFP_PropTypeW_##TypeName *)v = this->zfv;} \
    public: \
        zfoverride \
        virtual void zfvReset(void) { \
            this->zfv = zftValue<_ZFP_PropTypeW_##TypeName>().zfv; \
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
        virtual zfbool progressUpdate( \
                ZF_IN ZFProgressable *from \
                , ZF_IN ZFProgressable *to \
                , ZF_IN zffloat progress \
                ) { \
            if(zfself::_ZFP_ZFTypeId_progressUpdate()) { \
                return zfself::_ZFP_ZFTypeId_progressUpdate()(this, from, to, progress); \
            } \
            else { \
                return zffalse; \
            } \
        } \
    };

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
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
        return ZFTypeId<_ZFP_PropTypeW_##TypeName>::TypeId(); \
    } \
    ZF_STATIC_REGISTER_INIT(TypeIdReg_##TypeName) { \
        ZFMethodUserRegister_1(setterMethod, { \
                invokerObject->to<v_##TypeName *>()->zfv = value; \
            }, v_##TypeName::ClassData(), void, zftext("zfv") \
            , ZFMP_IN(_ZFP_PropTypeW_##TypeName const &, value) \
            ); \
        ZFMethodUserRegister_0(getterMethod, { \
                return invokerObject->to<v_##TypeName *>()->zfv; \
            }, v_##TypeName::ClassData(), _ZFP_PropTypeW_##TypeName const &, zftext("zfv")); \
    } \
    ZF_STATIC_REGISTER_DESTROY(TypeIdReg_##TypeName) { \
        ZFMethodUserUnregister(v_##TypeName::ClassData()->methodForNameIgnoreParent(zftext("zfv"), ZFTypeId<_ZFP_PropTypeW_##TypeName>::TypeId())); \
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
        return zfidentityCalcPointer(&(this->zfv)); \
    } \
    ZFCompareResult v_##TypeName::objectCompareImpl(ZF_IN ZFObject *anotherObj) { \
        ZFTypeIdWrapper *t = zfcast(ZFTypeIdWrapper *, anotherObj); \
        if(t == zfnull || !zfstringIsEqual(this->zfvTypeId(), t->zfvTypeId())) { \
            return ZFCompareUncomparable; \
        } \
        else { \
            return ZFComparerDefault(this->zfv, *(_ZFP_PropTypeW_##TypeName *)t->wrappedValue()); \
        } \
    } \
    zfbool v_##TypeName::zfvIsInit(void) { \
        return (ZFComparerDefault(this->zfv, zftValue<_ZFP_PropTypeW_##TypeName>().zfv) == ZFCompareEqual); \
    }

#define _ZFP_ZFTYPEID_WRAPPER_DEFINE_UNCOMPARABLE(TypeName, Type) \
    zfidentity v_##TypeName::objectHashImpl(void) { \
        return zfidentityCalcPointer(this); \
    } \
    ZFCompareResult v_##TypeName::objectCompareImpl(ZF_IN ZFObject *anotherObj) { \
        return this == anotherObj ? ZFCompareEqual : ZFCompareUncomparable; \
    } \
    zfbool v_##TypeName::zfvIsInit(void) { \
        return zffalse; \
    }

// ============================================================
#define _ZFP_ZFTYPEID_DECLARE(ZFLIB_, TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DECLARE(ZFLIB_, TypeName, Type) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<_ZFP_PropTypeW_##TypeName> : zfextend ZFTypeInfo { \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 1, \
        }; \
        static inline const zfstring &TypeId(void) { \
            return ZFTypeId_##TypeName(); \
        } \
        static inline const ZFClass *TypeIdClass(void) { \
            return v_##TypeName::ClassData(); \
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
                , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
                ) { \
            ZFCoreMutexLock(); \
            v_##TypeName *t = zfunsafe_zfAlloc(v_##TypeName); \
            t->zfv = v; \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            ZFCoreMutexUnlock(); \
            return zftrue; \
        } \
        template<typename T_Access = _ZFP_PropTypeW_##TypeName \
            , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropTypeW_##TypeName>::Value) ? 1 \
                : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                    && zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropTypeW_##TypeName>::Value \
                    && !zftTraits<T_Access>::TrIsRef) ? 2 : 0)) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (zfcast(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return zfcast(v_##TypeName *, obj)->zfv; \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull || (zfcast(v_##TypeName *, obj) != zfnull); \
            } \
            static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull ? zfnull : &(zfcast(v_##TypeName *, obj)->zfv); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 2> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull || (zfcast(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull ? zfnull : (T_Access)(zfcast(v_##TypeName *, obj)->zfv); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
    public: \
        zfoverride \
        virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const { \
            return ValueStore(obj, *(const _ZFP_PropTypeW_##TypeName *)v); \
        } \
        zfoverride \
        virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const { \
            if(!Value<_ZFP_PropTypeW_##TypeName>::zfvAccessAvailable(obj)) { \
                return zfnull; \
            } \
            return (void *)zfnew(_ZFP_PropTypeW_##TypeName, Value<_ZFP_PropTypeW_##TypeName>::zfvAccess(obj)); \
        } \
        zfoverride \
        virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const { \
            zfdelete((_ZFP_PropTypeW_##TypeName *)v); \
            Value<_ZFP_PropTypeW_##TypeName>::zfvAccessFinish(obj); \
        } \
        zfoverride \
        virtual ZFCoreArrayBase *genericArrayNew(void) const { \
            return zfnew(ZFCoreArray<_ZFP_PropTypeW_##TypeName>); \
        } \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_DEFINE(TypeName, Type, preferStringConverter) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_SERIALIZABLE(TypeName, Type, preferStringConverter) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMPARABLE(TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_, TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DECLARE(ZFLIB_, TypeName, Type) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<_ZFP_PropTypeW_##TypeName> : zfextend ZFTypeInfo { \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 0, \
        }; \
        static inline const zfstring &TypeId(void) { \
            return ZFTypeId_##TypeName(); \
        } \
        static inline const ZFClass *TypeIdClass(void) { \
            return v_##TypeName::ClassData(); \
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
                , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
                ) { \
            ZFCoreMutexLock(); \
            v_##TypeName *t = zfunsafe_zfAlloc(v_##TypeName); \
            t->zfv = v; \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            ZFCoreMutexUnlock(); \
            return zftrue; \
        } \
        template<typename T_Access = _ZFP_PropTypeW_##TypeName \
            , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropTypeW_##TypeName>::Value) ? 1 \
                : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                    && zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropTypeW_##TypeName>::Value \
                    && !zftTraits<T_Access>::TrIsRef) ? 2 : 0)) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (zfcast(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return zfcast(v_##TypeName *, obj)->zfv; \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull || (zfcast(v_##TypeName *, obj) != zfnull); \
            } \
            static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull ? zfnull : &(zfcast(v_##TypeName *, obj)->zfv); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 2> { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull || (zfcast(v_##TypeName *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return obj == zfnull ? zfnull : (T_Access)(zfcast(v_##TypeName *, obj)->zfv); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
    public: \
        zfoverride \
        virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const { \
            return ValueStore(obj, *(const _ZFP_PropTypeW_##TypeName *)v); \
        } \
        zfoverride \
        virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const { \
            if(!Value<_ZFP_PropTypeW_##TypeName>::zfvAccessAvailable(obj)) { \
                return zfnull; \
            } \
            return (void *)zfnew(_ZFP_PropTypeW_##TypeName, Value<_ZFP_PropTypeW_##TypeName>::zfvAccess(obj)); \
        } \
        zfoverride \
        virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const { \
            zfdelete((_ZFP_PropTypeW_##TypeName *)v); \
            Value<_ZFP_PropTypeW_##TypeName>::zfvAccessFinish(obj); \
        } \
        zfoverride \
        virtual ZFCoreArrayBase *genericArrayNew(void) const { \
            return zfnew(ZFCoreArray<_ZFP_PropTypeW_##TypeName>); \
        } \
    }; \
    /** @endcond */

#define _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_NOT_SERIALIZABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMPARABLE(TypeName, Type)

#define _ZFP_ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_COMMON(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_NOT_SERIALIZABLE(TypeName, Type) \
    _ZFP_ZFTYPEID_WRAPPER_DEFINE_UNCOMPARABLE(TypeName, Type)

// ============================================================
#define _ZFP_ZFTYPEID_DISABLE(Type) \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<Type> : zfextend ZFTypeInfo { \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 0, \
        }; \
        static inline const zfstring &TypeId(void) { \
            static ZFSigName d(ZFTypeId_none()); \
            return d; \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
                ) { \
            return zffalse; \
        } \
        template<typename T_Access = Type> \
        zfclassNotPOD Value { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return zffalse; \
            } \
            static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) { \
                return typename zftTraits<T_Access>::TrNoRef(); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
    public: \
        zfoverride \
        virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const { \
            return zffalse; \
        } \
        zfoverride \
        virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const { \
            return zfnull; \
        } \
        zfoverride \
        virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const { \
        } \
        zfoverride \
        virtual ZFCoreArrayBase *genericArrayNew(void) const { \
            return zfnew(ZFCoreArray<_ZFP_PropTypeW_##TypeName>); \
        } \
    }; \
    /** @endcond */

// ============================================================
#define _ZFP_ZFTYPEID_ALIAS_DECLARE(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type, TypeIdValueConversion) \
    /** @cond ZFPrivateDoc */ \
    typedef Type _ZFP_PropTypeW_##TypeName; \
    template<> \
    zfclassNotPOD ZFTypeId<Type> : zfextend ZFTypeInfo { \
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
                , ZF_IN _ZFP_PropTypeW_##TypeName const &v \
                ) { \
            return ZFTypeId<AliasToType>::ValueStore(obj, (AliasToType)v); \
        } \
        TypeIdValueConversion(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
    public: \
        zfoverride \
        virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const { \
            return ValueStore(obj, *(const _ZFP_PropTypeW_##TypeName *)v); \
        } \
        zfoverride \
        virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const { \
            if(!Value<_ZFP_PropTypeW_##TypeName>::zfvAccessAvailable(obj)) { \
                return zfnull; \
            } \
            return (void *)zfnew(_ZFP_PropTypeW_##TypeName, Value<_ZFP_PropTypeW_##TypeName>::zfvAccess(obj)); \
        } \
        zfoverride \
        virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const { \
            zfdelete((_ZFP_PropTypeW_##TypeName *)v); \
            Value<_ZFP_PropTypeW_##TypeName>::zfvAccessFinish(obj); \
        } \
        zfoverride \
        virtual ZFCoreArrayBase *genericArrayNew(void) const { \
            return zfnew(ZFCoreArray<_ZFP_PropTypeW_##TypeName>); \
        } \
    }; \
    /** @endcond */ \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    typedef v_##AliasToTypeName v_##TypeName;

#define _ZFP_ZFTYPEID_ALIAS_DEFINE(AliasToTypeName, AliasToType, TypeName, Type)

#define _ZFP_ZFTYPEID_ALIAS_EXPAND_DEFAULT(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
        template<typename T_Access = _ZFP_PropTypeW_##TypeName \
            , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropTypeW_##TypeName>::Value) ? 1 \
                : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                    && zftIsSame<typename zftTraits<T_Access>::TrNoRef, _ZFP_PropTypeW_##TypeName>::Value \
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
                _ZFP_PropTypeW_##TypeName *v = zfnew(_ZFP_PropTypeW_##TypeName); \
                *v = (_ZFP_PropTypeW_##TypeName)aliasValue; \
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
                _ZFP_PropTypeW_##TypeName *vTmp = (_ZFP_PropTypeW_##TypeName *)v; \
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
                _ZFP_PropTypeW_##TypeName *v = zfnew(_ZFP_PropTypeW_##TypeName); \
                *v = (_ZFP_PropTypeW_##TypeName)aliasValue; \
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
                _ZFP_PropTypeW_##TypeName *vTmp = (_ZFP_PropTypeW_##TypeName *)*p; \
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
                _ZFP_PropTypeW_##TypeName *v = zfnew(_ZFP_PropTypeW_##TypeName); \
                *v = (_ZFP_PropTypeW_##TypeName)aliasValue; \
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
                _ZFP_PropTypeW_##TypeName *vTmp = (_ZFP_PropTypeW_##TypeName *)v; \
                if(ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccessAvailable(objTmp)) { \
                    AliasToType &aliasValue = ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccess(objTmp); \
                    aliasValue = (AliasToType)*vTmp; \
                    ZFTypeId<AliasToType>::Value<AliasToType &>::zfvAccessFinish(objTmp); \
                } \
                zfdelete(vTmp); \
            } \
        };

// ============================================================
// special alias implicit convert
typedef void (*_ZFP_PropAliasDetachCallback)(
        ZF_IN ZFObject *obj
        , ZF_IN void *v
        );
extern ZFLIB_ZFCore void _ZFP_PropAliasAttach(
        ZF_IN ZFObject *obj
        , ZF_IN void *v
        , ZF_IN const zfstring &tagKey
        , ZF_IN _ZFP_PropAliasDetachCallback detachCallback
        );
extern ZFLIB_ZFCore void _ZFP_PropAliasDetach(
        ZF_IN ZFObject *obj
        , ZF_IN const zfstring &tagKey
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdFwd_h_

