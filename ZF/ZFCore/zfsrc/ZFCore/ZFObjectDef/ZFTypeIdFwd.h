/**
 * @file ZFTypeIdFwd.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeIdFwd_h_
#define _ZFI_ZFTypeIdFwd_h_

#include "zfautoFwd.h"
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
    virtual void *genericAccess(ZF_IN const zfauto &obj) const zfpurevirtual;
    /** @brief see #genericValueStore */
    virtual zfauto genericAccessFinish(ZF_IN const zfauto &obj, ZF_IN void *v) const zfpurevirtual;

    /**
     * @brief create generic array type
     *
     * impl must create by #ZFCoreArrayBase::refNew
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
     *       ZFOBJECT_CACHE_RELEASE({
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
     *   the temp object may (or may not) be released on different condition,
     *   take care of life cycle:
     *   -  if #ZFThread impl is available : release by #zfobjAutoRelease
     *   -  if #ZFThread impl is not available : added to #ZFAutoReleasePool,
     *     you must release it at proper time
     */
    template<typename T_Access = T_Type>
    zfclassNotPOD Value {
    public:
        /** @brief try access as raw value, see #ZFTypeId::Value */
        static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj);
        /** @brief try access as raw value, see #ZFTypeId::Value */
        static T_Access zfvAccess(ZF_IN const zfauto &obj);
        /** @brief finish access as raw value, see #ZFTypeId::Value */
        static zfauto zfvAccessFinish(ZF_IN const zfauto &obj);
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
zfclassFwd ZFProgressable;
typedef zfbool (*_ZFP_ZFTypeIdProgressUpdate)(
        ZF_IN_OUT ZFProgressable *ret
        , ZF_IN ZFProgressable *from
        , ZF_IN ZFProgressable *to
        , ZF_IN zffloat progress
        );

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
extern ZFLIB_ZFCore zfauto _ZFP_PropAliasDetach(
        ZF_IN ZFObject *obj
        , ZF_IN const zfstring &tagKey
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdFwd_h_

