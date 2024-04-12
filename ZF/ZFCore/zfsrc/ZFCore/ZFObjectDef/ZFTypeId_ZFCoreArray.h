/**
 * @file ZFTypeId_ZFCoreArray.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeId_ZFCoreArray_h_
#define _ZFI_ZFTypeId_ZFCoreArray_h_

#include "ZFTypeId_spec.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFCore zfbool _ZFP_ZFCoreArrayFromString(
        ZF_IN const ZFTypeInfo *elementType
        , ZF_IN_OUT ZFCoreArrayBase &v
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
extern ZFLIB_ZFCore zfbool _ZFP_ZFCoreArrayToString(
        ZF_IN const ZFTypeInfo *elementType
        , ZF_OUT zfstring &s
        , ZF_IN ZFCoreArrayBase const &v
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
extern ZFLIB_ZFCore zfbool _ZFP_ZFCoreArrayFromData(
        ZF_IN const ZFTypeInfo *elementType
        , ZF_IN_OUT ZFCoreArrayBase &v
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        );
extern ZFLIB_ZFCore zfbool _ZFP_ZFCoreArrayToData(
        ZF_IN const ZFTypeInfo *elementType
        , ZF_OUT ZFSerializableData &serializableData
        , ZF_IN ZFCoreArrayBase const &v
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );

// ============================================================
// ZFCoreArray
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFCoreArray>
 *       <SomeType />
 *       ... // each element
 *   </ZFCoreArray>
 * @endcode
 *
 * string data:
 * @code
 *   [e0_encoded_string_data, e1_encoded_string_data, ...]
 * @endcode
 */
#define ZFTypeId_ZFCoreArray() "ZFCoreArray"

/** @brief type wrapper for #ZFTypeId::Value */
zfclass ZFLIB_ZFCore v_ZFCoreArray : zfextend ZFTypeIdWrapper {
    ZFOBJECT_DECLARE(v_ZFCoreArray, ZFTypeIdWrapper)
    ZFALLOC_CACHE_RELEASE({
        cache->wrappedValueReset();
    })
public:
    /** @brief the value, see #ZFTypeId::Value */
    ZFCoreArrayBase *zfv;
    /** @brief type info for element */
    ZFCorePointerForObject<ZFTypeInfo *> elementType;
protected:
    /** @brief init with value */
    template<typename T_Type>
    void objectOnInit(ZF_IN const ZFCoreArray<T_Type> &v) {
        this->objectOnInit();
        this->zfv = v.refNew();
        this->elementType = zfnew(ZFTypeId<T_Type>);
    }

    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->zfv = zfnull;
        this->elementType = zfnull;
    }

    zfoverride
    virtual void objectOnDealloc(void) {
        this->wrappedValueReset();
        zfsuper::objectOnDealloc();
    }

public:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) {
        if(this->zfv == zfnull) {
            ret += ZFTOKEN_zfnull;
        }
        else {
            this->zfv->objectInfoT(ret);
        }
    }

    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj) {
        zfself *t = ZFCastZFObject(zfself *, anotherObj);
        if(t != zfnull
                && this->zfv != zfnull
                && t->zfv != zfnull
                && this->zfv->refImpl() == t->zfv->refImpl()
                ) {
            return ZFCompareTheSame;
        }
        else {
            return ZFCompareUncomparable;
        }
    }
public:
    zfoverride
    virtual void wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref) {
        zfself *t = ZFCastZFObject(zfself *, ref);
        if(t == this) {
            return;
        }
        ZFCoreArrayBase *zfvOld = this->zfv;
        if(t != zfnull && t->zfv != zfnull) {
            this->zfv = t->zfv->refNew();
            this->elementType = t->elementType;
        }
        else {
            this->zfv = zfnull;
            this->elementType = zfnull;
        }
        if(zfvOld != zfnull) {
            zfvOld->refDelete();
        }
    }
    zfoverride
    virtual const zfchar *wrappedValueTypeId(void) {
        return ZFTypeId_ZFCoreArray();
    }
    zfoverride
    virtual void *wrappedValue(void) {return this->zfv;}
    zfoverride
    virtual void wrappedValue(ZF_IN const void *v) {
        zfCoreAssertWithMessageTrim(this->elementType != zfnull, "changing wrappedValue without explicit element type");
        zfCoreAssert(v != zfnull);
        const ZFCoreArrayBase *vTmp = (const ZFCoreArrayBase *)v;
        ZFCoreArrayBase *old = this->zfv;
        this->zfv = vTmp->refNew();
        old->refDelete();
    }
    /** @brief set the internal value */
    virtual void wrappedValue(ZF_IN const ZFCoreArrayBase &v) {
        this->wrappedValue((const void *)&v);
    }
    /** @brief set the internal value */
    template<typename T_Type>
    void wrappedValue(ZF_IN const ZFCoreArray<T_Type> &v) {
        ZFCoreArrayBase *old = this->zfv;
        this->zfv = v.refNew();
        this->elementType = zfnew(ZFTypeId<T_Type>);
        old->refDelete();
    }
    /** @brief set the internal value */
    template<typename T_Type>
    void wrappedValue(ZF_IN const ZFCoreArrayPOD<T_Type> &v) {
        ZFCoreArrayBase *old = this->zfv;
        this->zfv = v.refNew();
        this->elementType = zfnew(ZFTypeId<T_Type>);
        old->refDelete();
    }
    zfoverride
    virtual void wrappedValueCopy(ZF_IN void *v) {
        if(this->zfv != zfnull) {
            *(ZFCoreArrayBase *)v = *this->zfv;
        }
    }
public:
    zfoverride
    virtual void wrappedValueReset(void) {
        if(this->zfv) {
            ZFCoreArrayBase *old = this->zfv;
            this->zfv = zfnull;
            this->elementType = zfnull;
            old->refDelete();
        }
    }
    zfoverride
    virtual zfbool wrappedValueIsInit(void) {
        return this->zfv == zfnull || this->zfv->isEmpty();
    }
public:
    zfoverride
    virtual zfbool wrappedValueFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) {
        if(this->zfv == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "not available for plain array type");
            return zffalse;
        }
        return _ZFP_ZFCoreArrayFromData(
                this->elementType
                , *(this->zfv)
                , serializableData
                , outErrorHint
                , outErrorPos
                );
    }
    zfoverride
    virtual zfbool wrappedValueToData(
            ZF_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) {
        if(this->zfv == zfnull) {
            ZFSerializableUtilErrorOccurred(outErrorHint,
                "not available for plain array type");
            return zffalse;
        }
        return _ZFP_ZFCoreArrayToData(
                this->elementType
                , serializableData
                , *(this->zfv)
                , outErrorHint
                );
    }
    zfoverride
    virtual zfbool wrappedValueFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        if(this->zfv == zfnull) {
            return zffalse;
        }
        return _ZFP_ZFCoreArrayFromString(
                this->elementType
                , *(this->zfv)
                , src
                , srcLen
                , errorHint
                );
    }
    zfoverride
    virtual zfbool wrappedValueToString(
            ZF_IN_OUT zfstring &s
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        if(this->zfv == zfnull) {
            return zffalse;
        }
        return _ZFP_ZFCoreArrayToString(
                this->elementType
                , s
                , *(this->zfv)
                , errorHint
                );
    }

public:
    /** @cond ZFPrivateDoc */
    void copyFrom(ZF_IN v_ZFCoreArray *ref) {
        if(this->zfv != zfnull
                && ref != zfnull
                && ref->zfv != zfnull
                ) {
            this->zfv->genericCopyFrom(*(ref->zfv));
        }
    }
    void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            ) {
        if(this->zfv != zfnull) {
            this->zfv->objectInfoOfContentT(ret, maxCount, token);
        }
        else {
            ret += token.tokenLeft;
            ret += token.tokenRight;
        }
    }
    zfstring objectInfoOfContent(
            ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            ) {
        if(this->zfv != zfnull) {
            return this->zfv->objectInfoOfContent(maxCount, token);
        }
        else {
            zfstring ret;
            ret += token.tokenLeft;
            ret += token.tokenRight;
            return ret;
        }
    }
    zfbool isPODType(void) {
        return this->zfv != zfnull && this->zfv->isPODType();
    }
    void capacity(ZF_IN zfindex newCapacity) {
        if(this->zfv != zfnull) {
            this->zfv->capacity(newCapacity);
        }
    }
    void capacityTrim(void) {
        if(this->zfv != zfnull) {
            this->zfv->capacityTrim();
        }
    }
    zfindex capacity(void) {
        if(this->zfv != zfnull) {
            return this->zfv->capacity();
        }
        else {
            return 0;
        }
    }
    void add(ZF_IN ZFObject *e) {
        if(this->zfv == zfnull) {
            return;
        }
        zfauto t = e;
        void *v = this->elementType->genericAccess(t);
        if(v == zfnull) {
            return;
        }
        this->zfv->genericAdd(v);
        this->elementType->genericAccessFinish(t, v);
    }
    void add(
            ZF_IN zfindex index
            , ZF_IN ZFObject *e
            ) {
        if(this->zfv == zfnull) {
            return;
        }
        if(index > this->count()) {
            zfCoreCriticalIndexOutOfRange(index, this->count() + 1);
            return;
        }
        zfauto t = e;
        void *v = this->elementType->genericAccess(t);
        if(v == zfnull) {
            return;
        }
        this->zfv->genericAdd(index, v);
        this->elementType->genericAccessFinish(t, v);
    }
    void addFrom(ZF_IN v_ZFCoreArray *ref) {
        if(ref == zfnull
                || ref->zfv == zfnull
                || this->zfv == zfnull
                ) {
            return;
        }
        this->zfv->genericAddFrom(*(ref->zfv));
    }
    void remove(ZF_IN zfindex index) {
        if(this->zfv != zfnull) {
            this->zfv->remove(index);
        }
    }
    void remove(
            ZF_IN zfindex index
            , ZF_IN zfindex count
            ) {
        if(this->zfv != zfnull) {
            this->zfv->remove(index, count);
        }
    }
    void removeFirst(void) {
        if(this->zfv != zfnull) {
            this->zfv->removeFirst();
        }
    }
    void removeLast(void) {
        if(this->zfv != zfnull) {
            this->zfv->removeLast();
        }
    }
    void removeAll(void) {
        if(this->zfv != zfnull) {
            this->zfv->removeAll();
        }
    }
    void move(
            ZF_IN zfindex fromIndex
            , ZF_IN zfindex toIndexOrIndexMax
            ) {
        if(this->zfv != zfnull) {
            this->zfv->move(fromIndex, toIndexOrIndexMax);
        }
    }
    void set(
            ZF_IN zfindex index
            , ZF_IN ZFObject *e
            ) {
        if(this->zfv == zfnull) {
            return;
        }
        zfauto t = e;
        void *v = this->elementType->genericAccess(t);
        if(v == zfnull) {
            return;
        }
        this->zfv->genericSet(index, v);
        this->elementType->genericAccessFinish(t, v);
    }
    zfauto get(ZF_IN zfindex index) {
        if(this->zfv == zfnull) {
            return zfnull;
        }
        zfauto t;
        this->elementType->genericValueStore(t, this->zfv->genericGet(index));
        return t;
    }
    zfauto getFirst(void) {
        if(this->zfv == zfnull || this->count() == 0) {
            zfCoreCriticalIndexOutOfRange(0, this->count());
        }
        zfauto t;
        this->elementType->genericValueStore(t, this->zfv->genericGet(0));
        return t;
    }
    zfauto getLast(void) {
        if(this->zfv == zfnull || this->count() == 0) {
            zfCoreCriticalIndexOutOfRange(0, this->count());
        }
        zfauto t;
        this->elementType->genericValueStore(t, this->zfv->genericGet(this->count() - 1));
        return t;
    }
    zfindex count(void) {
        if(this->zfv == zfnull) {
            return 0;
        }
        else {
            return this->zfv->count();
        }
    }
    zfbool isEmpty(void) {
        return this->zfv == zfnull || this->zfv->isEmpty();
    }
    /** @endcond */

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) {
        if(this->zfv != zfnull && !this->elementType->typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
            return this->wrappedValueFromData(serializableData, outErrorHint, outErrorPos);
        }
        else {
            return zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos);
        }
    }
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) {
        if(this->zfv != zfnull && !this->elementType->typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
            return this->wrappedValueToData(serializableData, outErrorHint);
        }
        else {
            return zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint);
        }
    }
};

/** @cond ZFPrivateDoc */

template<typename T_Type>
zfclassNotPOD ZFTypeId<ZFCoreArray<T_Type>, 0, 0> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = ZFTypeId<T_Type>::TypeIdRegistered,
        TypeIdSerializable = ZFTypeId<T_Type>::TypeIdSerializable,
    };
    static inline const zfchar *TypeId(void) {
        return ZFTypeId_ZFCoreArray();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const {
        return TypeId();
    }
    zfoverride
    virtual const ZFClass *typeIdClass(void) const {
        return v_ZFCoreArray::ClassData();
    }
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN ZFCoreArray<T_Type> const &v
            ) {
        zfCoreMutexLock();
        v_ZFCoreArray *holder = zfunsafe_zfAlloc(v_ZFCoreArray, v);
        obj = holder;
        zfunsafe_zfRelease(holder);
        zfCoreMutexUnlock();
        return zftrue;
    }
    template<typename T_Access = ZFCoreArray<T_Type>
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    ZFCoreArray<T_Type>
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = ZFCastZFObject(v_ZFCoreArray *, obj);
            return t != zfnull && (
                    t->elementType == zfnull
                    || zfstringIsEqual(t->elementType->typeId(), ZFTypeId<T_Type>::TypeId())
                    );
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = ZFCastZFObject(v_ZFCoreArray *, obj);
            if(t->zfv == zfnull) {
                t->wrappedValue(typename zftTraits<T_Access>::TrType());
            }
            return (T_Access)(ZFCoreArray<T_Type> &)(*(t->zfv));
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = ZFCastZFObject(v_ZFCoreArray *, obj);
            return t != zfnull && (
                    t->elementType == zfnull
                    || zfstringIsEqual(t->elementType->typeId(), ZFTypeId<T_Type>::TypeId())
                    );
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = ZFCastZFObject(v_ZFCoreArray *, obj);
            if(t->zfv == zfnull) {
                t->wrappedValue(typename zftTraits<T_Access>::TrType());
            }
            return (T_Access)(ZFCoreArray<T_Type> *)(t->zfv);
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const ZFCoreArray<T_Type> *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        if(!Value<ZFCoreArray<T_Type> >::zfvAccessAvailable(obj)) {
            return zfnull;
        }
        return (void *)zfnew(ZFCoreArray<T_Type>, Value<ZFCoreArray<T_Type> >::zfvAccess(obj));
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((ZFCoreArray<T_Type> *)v);
        Value<ZFCoreArray<T_Type> >::zfvAccessFinish(obj);
    }
};

template<typename T_Type>
zfclassNotPOD ZFTypeId<ZFCoreArrayPOD<T_Type>, 0, 0> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = ZFTypeId<T_Type>::TypeIdRegistered,
        TypeIdSerializable = ZFTypeId<T_Type>::TypeIdSerializable,
    };
    static inline const zfchar *TypeId(void) {
        return ZFTypeId_ZFCoreArray();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const {
        return TypeId();
    }
    zfoverride
    virtual const ZFClass *typeIdClass(void) const {
        return v_ZFCoreArray::ClassData();
    }
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN ZFCoreArray<T_Type> const &v
            ) {
        return ZFTypeId<ZFCoreArray<T_Type> >::ValueStore(obj, v);
    }
    template<typename T_Access = ZFCoreArrayPOD<T_Type>
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    ZFCoreArray<T_Type>
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = ZFCastZFObject(v_ZFCoreArray *, obj);
            return t != zfnull && (
                    t->elementType == zfnull
                    || zfstringIsEqual(t->elementType->typeId(), ZFTypeId<T_Type>::TypeId())
                    );
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = ZFCastZFObject(v_ZFCoreArray *, obj);
            if(t->zfv == zfnull) {
                t->wrappedValue(typename zftTraits<T_Access>::TrType());
            }
            return (T_Access)(ZFCoreArrayPOD<T_Type> &)(*(t->zfv));
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = ZFCastZFObject(v_ZFCoreArray *, obj);
            return t != zfnull && (
                    t->elementType == zfnull
                    || zfstringIsEqual(t->elementType->typeId(), ZFTypeId<T_Type>::TypeId())
                    );
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = ZFCastZFObject(v_ZFCoreArray *, obj);
            if(t->zfv == zfnull) {
                t->wrappedValue(typename zftTraits<T_Access>::TrType());
            }
            return (T_Access)(ZFCoreArrayPOD<T_Type> *)(t->zfv);
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const ZFCoreArrayPOD<T_Type> *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        if(!Value<ZFCoreArrayPOD<T_Type> >::zfvAccessAvailable(obj)) {
            return zfnull;
        }
        return (void *)zfnew(ZFCoreArrayPOD<T_Type>, Value<ZFCoreArrayPOD<T_Type> >::zfvAccess(obj));
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((ZFCoreArrayPOD<T_Type> *)v);
        Value<ZFCoreArrayPOD<T_Type> >::zfvAccessFinish(obj);
    }
};
/** @endcond */

// ============================================================
/** @brief convert array from string */
template<typename T_Type>
zfbool ZFCoreArrayFromString(
        ZF_IN_OUT ZFCoreArray<T_Type> &v
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    return _ZFP_ZFCoreArrayFromString(
            zflineDelete(zfnew(ZFTypeId<T_Type>))
            , v
            , src
            , srcLen
            , errorHint
            );
}
/** @brief convert array to string */
template<typename T_Type>
zfbool ZFCoreArrayToString(
        ZF_OUT zfstring &s
        , ZF_IN ZFCoreArray<T_Type> const &v
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    return _ZFP_ZFCoreArrayToString(
            zflineDelete(zfnew(ZFTypeId<T_Type>))
            , s
            , v
            , errorHint
            );
}
/** @brief convert array to string */
template<typename T_Type>
zfstring ZFCoreArrayToString(
        ZF_IN ZFCoreArray<T_Type> const &v
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    zfstring ret;
    if(ZFCoreArrayToString(ret, v, errorHint)) {
        return ret;
    }
    else {
        return zfstring();
    }
}

// ============================================================
/** @brief convert array from serializable data */
template<typename T_Type>
zfbool ZFCoreArrayFromData(
        ZF_IN_OUT ZFCoreArray<T_Type> &v
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        ) {
    return _ZFP_ZFCoreArrayFromData(
            zflineDelete(zfnew(ZFTypeId<T_Type>))
            , v
            , serializableData
            , outErrorHint
            , outErrorPos
            );
}
/** @brief convert array to serializable data */
template<typename T_Type>
zfbool ZFCoreArrayToData(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN ZFCoreArray<T_Type> const &v
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        ) {
    return _ZFP_ZFCoreArrayToData(
            zflineDelete(zfnew(ZFTypeId<T_Type>))
            , serializableData
            , v
            , outErrorHint
            );
}
/** @brief convert array to serializable data */
template<typename T_Type>
ZFSerializableData ZFCoreArrayToData(
        ZF_IN ZFCoreArray<T_Type> const &v
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        ) {
    ZFSerializableData ret;
    if(ZFCoreArrayToData(ret, v, outErrorHint)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_ZFCoreArray_h_

