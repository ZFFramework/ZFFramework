/**
 * @file ZFTypeId_ZFCoreArray.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeId_ZFCoreArray_h_
#define _ZFI_ZFTypeId_ZFCoreArray_h_

#include "ZFTypeId_spec.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFCore zfbool _ZFP_ZFCoreArrayFromStringT(
        ZF_IN const ZFTypeInfo &elementType
        , ZF_IN_OUT ZFCoreArrayBase &v
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
extern ZFLIB_ZFCore zfbool _ZFP_ZFCoreArrayToStringT(
        ZF_IN const ZFTypeInfo &elementType
        , ZF_OUT zfstring &s
        , ZF_IN ZFCoreArrayBase const &v
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
extern ZFLIB_ZFCore zfbool _ZFP_ZFCoreArrayFromDataT(
        ZF_IN const ZFTypeInfo &elementType
        , ZF_IN_OUT ZFCoreArrayBase &v
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        );
extern ZFLIB_ZFCore zfbool _ZFP_ZFCoreArrayToDataT(
        ZF_IN const ZFTypeInfo &elementType
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
inline const zfstring &ZFTypeId_ZFCoreArray(void) {
    static ZFSigName d(zftext("ZFCoreArray"));
    return d;
}

/** @brief type wrapper for #ZFTypeId::Value */
zfclass ZFLIB_ZFCore v_ZFCoreArray : zfextend ZFTypeIdWrapper {
    ZFOBJECT_DECLARE(v_ZFCoreArray, ZFTypeIdWrapper)
    ZFALLOC_CACHE_RELEASE({
        cache->zfvReset();
    })
public:
    /** @brief the value, see #ZFTypeId::Value */
    ZFCoreArrayBase *zfv;
    /** @brief type info for element */
    const ZFTypeInfo *elementType;
private:
    const ZFCorePointer *_ZFP_elementTypeHolder;

public:
    /** @brief init element type */
    virtual zfbool elementTypeInit(ZF_IN const zfstring &elementTypeId);
private:
    zfauto _ZFP_elementTypeCheck(ZF_IN ZFObject *element);

protected:
    /** @brief init with value */
    template<typename T_Type>
    void objectOnInit(ZF_IN const ZFCoreArray<T_Type> &v) {
        this->zfv = v.refNew();
        ZFTypeInfo *t = zfpoolNew(ZFTypeId<T_Type>);
        this->_ZFP_elementTypeHolder = zfpoolNew(ZFCorePointerForPoolObject<ZFTypeInfo *>, t);
        this->elementType = this->_ZFP_elementTypeHolder->pointerValueT<const ZFTypeInfo *>();
    }

    /** @brief init with #elementTypeInit */
    virtual void objectOnInit(ZF_IN const zfstring &elementTypeId) {
        this->elementTypeInit(elementTypeId);
    }

    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->zfv = zfnull;
        this->elementType = zfnull;
        this->_ZFP_elementTypeHolder = zfnull;
    }

    zfoverride
    virtual void objectOnDealloc(void) {
        this->zfvReset();
        zfsuper::objectOnDealloc();
    }

public:
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        if(this->zfv == zfnull) {
            ret += ZFTOKEN_zfnull;
        }
        else {
            this->zfv->objectInfoT(ret);
        }
    }

    zfoverride
    virtual zfidentity objectHashImpl(void) {
        return zfidentityCalc(this->zfv ? this->zfv->refImpl() : zfnull);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        zfself *t = zfcast(zfself *, anotherObj);
        if(t != zfnull
                && this->zfv != zfnull
                && t->zfv != zfnull
                && this->zfv->refImpl() == t->zfv->refImpl()
                ) {
            return ZFCompareEqual;
        }
        else {
            return ZFCompareUncomparable;
        }
    }
public:
    zfoverride
    virtual void wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref) {
        zfself *t = zfcast(zfself *, ref);
        if(t == this) {
            return;
        }
        ZFCoreArrayBase *zfvOld = this->zfv;
        const ZFCorePointer *holderOld = this->_ZFP_elementTypeHolder;
        if(t != zfnull && t->zfv != zfnull) {
            this->zfv = t->zfv->refNew();
            this->elementType = t->elementType;
            if(t->_ZFP_elementTypeHolder) {
                this->_ZFP_elementTypeHolder = t->_ZFP_elementTypeHolder->refNew();
            }
        }
        else {
            this->zfv = zfnull;
            this->elementType = zfnull;
            this->_ZFP_elementTypeHolder = zfnull;
        }
        if(zfvOld != zfnull) {
            zfvOld->refDelete();
        }
        if(holderOld != zfnull) {
            holderOld->refDelete();
        }
    }
    zfoverride
    virtual const zfstring &zfvTypeId(void) {
        return ZFTypeId_ZFCoreArray();
    }
    zfoverride
    virtual void *wrappedValue(void) {return this->zfv;}
    zfoverride
    virtual void wrappedValue(ZF_IN const void *v) {
        ZFCoreAssertWithMessageTrim(this->elementType != zfnull, "changing wrappedValue without explicit element type");
        ZFCoreAssert(v != zfnull);
        const ZFCoreArrayBase *vTmp = (const ZFCoreArrayBase *)v;
        ZFCoreArrayBase *old = this->zfv;
        this->zfv = vTmp->refNew();
        if(old) {
            old->refDelete();
        }
    }
    /** @brief set the internal value */
    virtual void wrappedValue(ZF_IN const ZFCoreArrayBase &v) {
        this->wrappedValue((const void *)&v);
    }
    /** @brief set the internal value */
    template<typename T_Type>
    void wrappedValue(ZF_IN const ZFCoreArray<T_Type> &v) {
        ZFCoreArrayBase *zfvOld = this->zfv;
        const ZFCorePointer *holderOld = this->_ZFP_elementTypeHolder;
        this->zfv = v.refNew();
        ZFTypeInfo *t = zfpoolNew(ZFTypeId<T_Type>);
        this->_ZFP_elementTypeHolder = zfpoolNew(ZFCorePointerForPoolObject<ZFTypeInfo *>, t);
        this->elementType = this->_ZFP_elementTypeHolder->pointerValueT<const ZFTypeInfo *>();
        if(zfvOld) {
            zfvOld->refDelete();
        }
        if(holderOld) {
            holderOld->refDelete();
        }
    }
    zfoverride
    virtual void wrappedValueCopy(ZF_IN void *v) {
        ZFCoreAssertWithMessageTrim(this->elementType != zfnull, "wrappedValueCopy without explicit element type");
        *(ZFCoreArrayBase *)v = *this->zfv;
    }
public:
    zfoverride
    virtual void zfvReset(void) {
        if(this->zfv) {
            ZFCoreArrayBase *zfvOld = this->zfv;
            const ZFCorePointer *holderOld = this->_ZFP_elementTypeHolder;
            this->zfv = zfnull;
            this->elementType = zfnull;
            this->_ZFP_elementTypeHolder = zfnull;
            zfvOld->refDelete();
            if(holderOld) {
                holderOld->refDelete();
            }
        }
    }
    zfoverride
    virtual zfbool zfvIsInit(void) {
        return this->zfv == zfnull || this->zfv->isEmpty();
    }
public:
    zfoverride
    virtual zfbool zfvFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) {
        if(this->zfv == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "not available for plain array type");
            return zffalse;
        }
        return _ZFP_ZFCoreArrayFromDataT(
                *this->elementType
                , *(this->zfv)
                , serializableData
                , outErrorHint
                , outErrorPos
                );
    }
    zfoverride
    virtual zfbool zfvToData(
            ZF_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) {
        if(this->zfv == zfnull) {
            ZFSerializableUtilErrorOccurred(outErrorHint,
                "not available for plain array type");
            return zffalse;
        }
        return _ZFP_ZFCoreArrayToDataT(
                *this->elementType
                , serializableData
                , *(this->zfv)
                , outErrorHint
                );
    }
    zfoverride
    virtual zfbool zfvFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        if(this->zfv == zfnull) {
            if(errorHint) {
                *errorHint += "not available for plain array type";
            }
            return zffalse;
        }
        return _ZFP_ZFCoreArrayFromStringT(
                *this->elementType
                , *(this->zfv)
                , src
                , srcLen
                , errorHint
                );
    }
    zfoverride
    virtual zfbool zfvToString(
            ZF_IN_OUT zfstring &s
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        if(this->zfv == zfnull) {
            if(errorHint) {
                *errorHint += "not available for plain array type";
            }
            return zffalse;
        }
        return _ZFP_ZFCoreArrayToStringT(
                *this->elementType
                , s
                , *(this->zfv)
                , errorHint
                );
    }

public:
    /** @cond ZFPrivateDoc */
    void swap(ZF_IN v_ZFCoreArray *ref) {
        ZFCoreAssertWithMessageTrim(zftrue
                && this->elementType != zfnull
                && ref->elementType != zfnull
                && zfstringIsEqual(this->elementType->typeId(), ref->elementType->typeId())
                , "swap with different element type, this: %s, other: %s"
                , this->elementType ? this->elementType->typeId() : zfnull
                , ref->elementType ? ref->elementType->typeId() : zfnull
                );
        this->zfv->genericSwap(*(ref->zfv));
    }
    void copyFrom(ZF_IN v_ZFCoreArray *ref) {
        if(this->elementType != zfnull
                && ref != zfnull
                && ref->elementType != zfnull
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
        zfauto t = _ZFP_elementTypeCheck(e);
        if(!t) {
            return;
        }
        void *v = this->elementType->genericAccess(t);
        ZFCoreAssertWithMessageTrim(v != zfnull
                , "add with different element type, desired: %s, got: %s"
                , this->elementType ? this->elementType->typeId() : zfnull
                , e ? e->classData()->classNameFull() : zfnull
                );
        this->zfv->genericAdd(v);
        this->elementType->genericAccessFinish(t, v);
    }
    void add(
            ZF_IN ZFObject *e
            , ZF_IN zfindex index
            ) {
        zfauto t = _ZFP_elementTypeCheck(e);
        if(!t) {
            return;
        }
        ZFCoreAssertIndexRange(index, this->count() + 1);
        void *v = this->elementType->genericAccess(t);
        ZFCoreAssertWithMessageTrim(v != zfnull
                , "add with different element type, desired: %s, got: %s"
                , this->elementType ? this->elementType->typeId() : zfnull
                , e ? e->classData()->classNameFull() : zfnull
                );
        this->zfv->genericAdd(v, index);
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
    zfindex find(ZF_IN ZFObject *e) {
        zfauto t = _ZFP_elementTypeCheck(e);
        if(!t) {
            return zfindexMax();
        }
        void *v = this->elementType->genericAccess(t);
        ZFCoreAssertWithMessageTrim(v != zfnull
                , "find with different element type, desired: %s, got: %s"
                , this->elementType ? this->elementType->typeId() : zfnull
                , e ? e->classData()->classNameFull() : zfnull
                );
        zfindex ret = this->zfv->genericFind(v);
        this->elementType->genericAccessFinish(t, v);
        return ret;
    }
    zfindex findReversely(ZF_IN ZFObject *e) {
        zfauto t = _ZFP_elementTypeCheck(e);
        if(!t) {
            return zfindexMax();
        }
        void *v = this->elementType->genericAccess(t);
        ZFCoreAssertWithMessageTrim(v != zfnull
                , "find with different element type, desired: %s, got: %s"
                , this->elementType ? this->elementType->typeId() : zfnull
                , e ? e->classData()->classNameFull() : zfnull
                );
        zfindex ret = this->zfv->genericFindReversely(v);
        this->elementType->genericAccessFinish(t, v);
        return ret;
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
    zfauto removeAndGet(ZF_IN zfindex index) {
        zfauto t = this->get(index);
        this->remove(index);
        return t;
    }
    void removeFirst(void) {
        if(this->zfv != zfnull) {
            this->zfv->removeFirst();
        }
    }
    zfauto removeFirstAndGet() {
        zfauto t = this->getFirst();
        this->removeFirst();
        return t;
    }
    void removeLast(void) {
        if(this->zfv != zfnull) {
            this->zfv->removeLast();
        }
    }
    zfauto removeLastAndGet() {
        zfauto t = this->getLast();
        this->removeLast();
        return t;
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
        zfauto t = _ZFP_elementTypeCheck(e);
        if(!t) {
            return;
        }
        void *v = this->elementType->genericAccess(t);
        ZFCoreAssertWithMessageTrim(v != zfnull
                , "add with different element type, desired: %s, got: %s"
                , this->elementType ? this->elementType->typeId() : zfnull
                , e ? e->classData()->classNameFull() : zfnull
                );
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
        if(this->zfv == zfnull || this->isEmpty()) {
            ZFCoreCriticalIndexOutOfRange(0, this->count());
        }
        zfauto t;
        this->elementType->genericValueStore(t, this->zfv->genericGet(0));
        return t;
    }
    zfauto getLast(void) {
        if(this->zfv == zfnull || this->isEmpty()) {
            ZFCoreCriticalIndexOutOfRange(0, this->count());
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
    zfbool isContain(ZF_IN ZFObject *e) {
        return this->find(e) != zfindexMax();
    }
    void sort(
            ZF_IN_OPT zfindex start = 0
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) {
        if(this->zfv) {
            this->zfv->sort(start, count);
        }
    }
    void sortReversely(
            ZF_IN_OPT zfindex start = 0
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) {
        if(this->zfv) {
            this->zfv->sortReversely(start, count);
        }
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
            return this->zfvFromData(serializableData, outErrorHint, outErrorPos);
        }
        else {
            return zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos);
        }
    }
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            ) {
        if(this->zfv != zfnull && !this->elementType->typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
            return this->zfvToData(serializableData, outErrorHint);
        }
        else {
            return zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner);
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
    static inline const zfstring &TypeId(void) {
        return ZFTypeId_ZFCoreArray();
    }
    static inline const ZFClass *TypeIdClass(void) {
        return v_ZFCoreArray::ClassData();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfstring &typeId(void) const {
        return TypeId();
    }
    zfoverride
    virtual const ZFClass *typeIdClass(void) const {
        return TypeIdClass();
    }
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN ZFCoreArray<T_Type> const &v
            ) {
        ZFCoreMutexLock();
        v_ZFCoreArray *holder = zfunsafe_zfAlloc(v_ZFCoreArray, v);
        obj = holder;
        zfunsafe_zfRelease(holder);
        ZFCoreMutexUnlock();
        return zftrue;
    }
    template<typename T_Access = ZFCoreArray<T_Type>
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, ZFCoreArray<T_Type> >::Value) ? 1
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, ZFCoreArray<T_Type> >::Value
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0))
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = obj;
            return t != zfnull && (
                    t->elementType == zfnull
                    || zfstringIsEqual(t->elementType->typeId(), ZFTypeId<T_Type>::TypeId())
                    );
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            v_ZFCoreArray *t = obj;
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
            v_ZFCoreArray *t = obj;
            return obj == zfnull || (t != zfnull && (
                        t->elementType == zfnull
                        || zfstringIsEqual(t->elementType->typeId(), ZFTypeId<T_Type>::TypeId())
                        ));
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            if(obj == zfnull) {
                return zfnull;
            }
            else {
                v_ZFCoreArray *t = obj;
                if(t->zfv == zfnull) {
                    t->wrappedValue(typename zftTraits<T_Access>::TrType());
                }
                return (typename zftTraits<T_Access>::TrNoRef)(ZFCoreArray<T_Type> *)(t->zfv);
            }
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
        return (void *)zfpoolNew(ZFCoreArray<T_Type>, Value<ZFCoreArray<T_Type> >::zfvAccess(obj));
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfpoolDelete((ZFCoreArray<T_Type> *)v);
        Value<ZFCoreArray<T_Type> >::zfvAccessFinish(obj);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfpoolNew(ZFCoreArray<ZFCoreArray<T_Type> >);
    }
};
/** @endcond */

// ============================================================
/** @brief convert array from string */
template<typename T_Type>
zfbool ZFCoreArrayFromStringT(
        ZF_IN_OUT ZFCoreArray<T_Type> &v
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    return _ZFP_ZFCoreArrayFromStringT(
            ZFTypeId<T_Type>()
            , v
            , src
            , srcLen
            , errorHint
            );
}
/** @brief convert array from string */
template<typename T_Type>
ZFCoreArray<T_Type> ZFCoreArrayFromString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    ZFCoreArray<T_Type> ret;
    _ZFP_ZFCoreArrayFromStringT(
            ZFTypeId<T_Type>()
            , ret
            , src
            , srcLen
            , errorHint
            );
    return ret;
}
/** @brief convert array to string */
template<typename T_Type>
zfbool ZFCoreArrayToStringT(
        ZF_OUT zfstring &s
        , ZF_IN ZFCoreArray<T_Type> const &v
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    return _ZFP_ZFCoreArrayToStringT(
            ZFTypeId<T_Type>()
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
    ZFCoreArrayToStringT(ret, v, errorHint);
    return ret;
}

// ============================================================
/** @brief convert array from serializable data */
template<typename T_Type>
zfbool ZFCoreArrayFromDataT(
        ZF_IN_OUT ZFCoreArray<T_Type> &v
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        ) {
    return _ZFP_ZFCoreArrayFromDataT(
            ZFTypeId<T_Type>()
            , v
            , serializableData
            , outErrorHint
            , outErrorPos
            );
}
/** @brief convert array from serializable data */
template<typename T_Type>
ZFCoreArray<T_Type> ZFCoreArrayFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        ) {
    ZFCoreArray<T_Type> ret;
    _ZFP_ZFCoreArrayFromDataT(
            ZFTypeId<T_Type>()
            , ret
            , serializableData
            , outErrorHint
            , outErrorPos
            );
    return ret;
}
/** @brief convert array to serializable data */
template<typename T_Type>
zfbool ZFCoreArrayToDataT(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN ZFCoreArray<T_Type> const &v
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        ) {
    return _ZFP_ZFCoreArrayToDataT(
            ZFTypeId<T_Type>()
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
    ZFCoreArrayToDataT(ret, v, outErrorHint);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_ZFCoreArray_h_

