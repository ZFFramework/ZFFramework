/**
 * @file ZFCoreOrderMap.h
 * @brief core map type for private use only
 */

#ifndef _ZFI_ZFCoreOrderMap_h_
#define _ZFI_ZFCoreOrderMap_h_

#include "ZFMemPool.h"
#include "ZFCoreArray.h"
#include "zfiter.h"
#include "ZFToken.h"
#include "ZFCoreUtilMacro.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFCoreOrderMapPrivate {
public:
    typedef zfidentity (*Fn_Hash)(ZF_IN const void *key);
    typedef zfbool (*Fn_Equal)(ZF_IN const void *key0, ZF_IN const void *key1);
    typedef void * (*Fn_KeyCopy)(ZF_IN const void *key);
    typedef void (*Fn_KeyDestroy)(ZF_IN void *key);
    typedef void * (*Fn_ValueCopy)(ZF_IN_OUT_OPT void *exist, ZF_IN_OPT const void *value);
    typedef void (*Fn_ValueDestroy)(ZF_IN void *value);
    typedef void (*Fn_KeyInfo)(ZF_IN_OUT zfstring &ret, ZF_IN const void *key);
    typedef void (*Fn_ValueInfo)(ZF_IN_OUT zfstring &ret, ZF_IN const void *value);
    typedef zfbool (*Fn_ValueEqual)(ZF_IN const void *value0, ZF_IN const void *value1);
public:
    zfuint refCount;
    Fn_Hash fn_Hash;
    Fn_Equal fn_Equal;
    Fn_KeyCopy fn_KeyCopy;
    Fn_KeyDestroy fn_KeyDestroy;
    Fn_ValueCopy fn_ValueCopy;
    Fn_ValueDestroy fn_ValueDestroy;
public:
    static _ZFP_ZFCoreOrderMapPrivate *create(
            ZF_IN Fn_Hash fn_Hash
            , ZF_IN Fn_Equal fn_Equal
            , ZF_IN Fn_KeyCopy fn_KeyCopy
            , ZF_IN Fn_KeyDestroy fn_KeyDestroy
            , ZF_IN Fn_ValueCopy fn_ValueCopy
            , ZF_IN Fn_ValueDestroy fn_ValueDestroy
            );
    static void destroy(ZF_IN _ZFP_ZFCoreOrderMapPrivate *d);
    _ZFP_ZFCoreOrderMapPrivate(
            ZF_IN Fn_Hash fn_Hash
            , ZF_IN Fn_Equal fn_Equal
            , ZF_IN Fn_KeyCopy fn_KeyCopy
            , ZF_IN Fn_KeyDestroy fn_KeyDestroy
            , ZF_IN Fn_ValueCopy fn_ValueCopy
            , ZF_IN Fn_ValueDestroy fn_ValueDestroy
            )
    : refCount(1)
    , fn_Hash(fn_Hash)
    , fn_Equal(fn_Equal)
    , fn_KeyCopy(fn_KeyCopy)
    , fn_KeyDestroy(fn_KeyDestroy)
    , fn_ValueCopy(fn_ValueCopy)
    , fn_ValueDestroy(fn_ValueDestroy)
    {
    }
    virtual ~_ZFP_ZFCoreOrderMapPrivate(void) {}
public:
    virtual void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN zfindex maxCount
            , ZF_IN const ZFTokenForKeyValueContainer &token
            , ZF_IN Fn_KeyInfo fn_KeyInfo
            , ZF_IN Fn_ValueInfo fn_ValueInfo
            ) zfpurevirtual;
    virtual ZFCompareResult objectCompareValue(
            ZF_IN const _ZFP_ZFCoreOrderMapPrivate *ref
            , ZF_IN Fn_ValueEqual fn_ValueEqual
            ) zfpurevirtual;
    virtual void copyFrom(ZF_IN_OUT _ZFP_ZFCoreOrderMapPrivate *ref) zfpurevirtual;
    virtual zfindex count(void) zfpurevirtual;
    virtual zfbool isEmpty(void) zfpurevirtual;
    virtual zfbool isContain(ZF_IN const void *key) zfpurevirtual;
    virtual void addFrom(ZF_IN_OUT _ZFP_ZFCoreOrderMapPrivate *ref) zfpurevirtual;
    virtual void set(ZF_IN const void *key, ZF_IN const void *value) zfpurevirtual;
    virtual void *get(ZF_IN const void *key) zfpurevirtual;
    virtual void *access(ZF_IN const void *key) zfpurevirtual;
    virtual void remove(ZF_IN const void *key) zfpurevirtual;
    virtual void removeAll(void) zfpurevirtual;
public:
    virtual zfiter iter(void) zfpurevirtual;
    virtual zfiter iterFind(ZF_IN const void *key) zfpurevirtual;
    virtual const void *iterKey(ZF_IN const zfiter &it) zfpurevirtual;
    virtual void *iterValue(ZF_IN const zfiter &it) zfpurevirtual;
    virtual void iterValue(ZF_IN_OUT zfiter &it, ZF_IN const void *value) zfpurevirtual;
    virtual void iterRemove(ZF_IN_OUT zfiter &it) zfpurevirtual;
public:
    virtual void move(
            ZF_IN zfindex from
            , ZF_IN zfindex to
            ) zfpurevirtual;
    virtual const void *keyAt(ZF_IN zfindex index) zfpurevirtual;
    virtual void *valueAt(ZF_IN zfindex index) zfpurevirtual;
    virtual void removeAt(ZF_IN zfindex index) zfpurevirtual;
    virtual zfiter iterAt(ZF_IN zfindex index) zfpurevirtual;
    virtual zfindex iterIndex(ZF_IN const zfiter &it) zfpurevirtual;
};
/**
 * @brief core map type for private use only
 *
 * used to reduce dependency of stl\n
 * key must support #zftHash and `operator ==`
 */
template<typename T_Key, typename T_Value>
zfclassLikePOD ZFLIB_ZFCore ZFCoreOrderMap {
public:
    /**
     * @brief construct an empty map
     */
    ZFCoreOrderMap(void) : d(zfnull) {}
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreOrderMap(ZF_IN const ZFCoreOrderMap<T_Key, T_Value> &ref) : d(ref.d) {
        if(d) {
            ++(d->refCount);
        }
    }
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreOrderMap &operator = (ZF_IN const ZFCoreOrderMap<T_Key, T_Value> &ref) {
        _ZFP_ZFCoreOrderMapPrivate *dTmp = d;
        d = ref.d;
        if(d) {
            ++(d->refCount);
        }
        if(dTmp && (--(dTmp->refCount)) == 0) {
            _ZFP_ZFCoreOrderMapPrivate::destroy(dTmp);
        }
        return *this;
    }
    /**
     * @brief true if same ref
     */
    zfbool operator == (ZF_IN const ZFCoreOrderMap<T_Key, T_Value> &ref) const {return d == ref.d;}
    /**
     * @brief true if not same ref
     */
    zfbool operator != (ZF_IN const ZFCoreOrderMap<T_Key, T_Value> &ref) const {return d != ref.d;}
    ~ZFCoreOrderMap(void) {
        if(d && (--(d->refCount)) == 0) {
            _ZFP_ZFCoreOrderMapPrivate::destroy(d);
        }
    }

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const {
        this->objectInfoOfContentT(ret, 5);
    }
    /** @brief return object info */
    zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
    /** @brief compare by instance */
    ZFCompareResult objectCompare(ZF_IN ZFCoreOrderMap<T_Key, T_Value> const &ref) const {
        return d == ref.d ? ZFCompareEqual : ZFCompareUncomparable;
    }
    /** @brief compare by instance */
    ZFCompareResult objectCompareValue(ZF_IN ZFCoreOrderMap<T_Key, T_Value> const &ref) const {
        if(d) {
            if(ref.d) {
                return d->objectCompareValue(ref.d, _ValueEqual);
            }
            else {
                return d->count() == 0 ? ZFCompareEqual : ZFCompareUncomparable;
            }
        }
        else {
            if(ref.d) {
                return ref.d->count() == 0 ? ZFCompareEqual : ZFCompareUncomparable;
            }
            else {
                return ZFCompareEqual;
            }
        }
    }

public:
    /** @brief see #objectInfoOfContent */
    void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
            ) const {
        ret += token.tokenLeft;
        if(d) {
            d->objectInfoOfContentT(ret, maxCount, token, _KeyInfo, _ValueInfo);
        }
        ret += token.tokenRight;
    }
    /** @brief return contents info */
    zfstring objectInfoOfContent(
            ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
            ) const {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token);
        return ret;
    }

public:
    /**
     * @brief swap internal data
     */
    void swap(ZF_IN_OUT ZFCoreOrderMap<T_Key, T_Value> &ref) {
        if(d != ref.d) {
            _ZFP_ZFCoreOrderMapPrivate *dTmp = d;
            d = ref.d;
            ref.d = dTmp;
        }
    }

public:
    /**
     * @brief copy all contents from ref, remove all before copy
     */
    void copyFrom(ZF_IN const ZFCoreOrderMap<T_Key, T_Value> &ref) {
        if(d != ref.d) {
            if(d) {
                if(ref.d) {
                    d->copyFrom(ref.d);
                }
                else {
                    d->removeAll();
                }
            }
            else {
                if(ref.d) {
                    _dInit();
                    d->copyFrom(ref.d);
                }
            }
        }
    }

public:
    /**
     * @brief return count
     */
    zfindex count(void) const {
        return d ? d->count() : 0;
    }

    /**
     * @brief true if empty
     */
    zfbool isEmpty(void) const {
        return !d || d->isEmpty();
    }

    /**
     * @brief true if contains the key
     */
    zfbool isContain(ZF_IN const T_Key &key) const {
        return d && d->isContain(&key);
    }

    /**
     * @brief add elements from ref
     */
    void addFrom(ZF_IN const ZFCoreOrderMap<T_Key, T_Value> &ref) {
        if(d != ref.d) {
            if(d) {
                if(ref.d) {
                    d->addFrom(ref.d);
                }
            }
            else {
                if(ref.d) {
                    _dInit();
                    d->addFrom(ref.d);
                }
            }
        }
    }

    /**
     * @brief change value or create if not exist,
     *   value would be retained by this method
     */
    void set(
            ZF_IN const T_Key &key
            , ZF_IN const T_Value &value
            ) {
        _dInit();
        d->set(&key, &value);
    }
    /**
     * @brief get value or null if not exist
     */
    const T_Value *get(ZF_IN const T_Key &key) const {
        if(d) {
            return (const T_Value *)d->get(&key);
        }
        return zfnull;
    }
    /**
     * @brief get value or null if not exist
     */
    T_Value *get(ZF_IN const T_Key &key) {
        if(d) {
            return (T_Value *)d->get(&key);
        }
        return zfnull;
    }
    /**
     * @brief access value or create if not exist
     */
    T_Value &access(ZF_IN const T_Key &key) {
        _dInit();
        return *(T_Value *)d->access(&key);
    }

    /** @brief see #allKey */
    void allKeyT(ZF_IN_OUT ZFCoreArray<T_Key> &ret) const {
        if(d) {
            for(zfiter it = d->iter(); it; ++it) {
                ret.add(*(const T_Key *)d->iterKey(it));
            }
        }
    }
    /**
     * @brief return a copy of all keys
     */
    inline ZFCoreArray<zfstring> allKey(void) const {
        ZFCoreArray<zfstring> ret;
        this->allKeyT(ret);
        return ret;
    }

    /** @brief see #allValue */
    void allValueT(ZF_IN_OUT ZFCoreArray<T_Value> &ret) const {
        if(d) {
            for(zfiter it = d->iter(); it; ++it) {
                ret.add(*(const T_Value *)d->iterValue(it));
            }
        }
    }
    /**
     * @brief return a copy of all values
     */
    inline ZFCoreArray<T_Value> allValue(void) const {
        ZFCoreArray<T_Value> ret;
        this->allValueT(ret);
        return ret;
    }

    /**
     * @brief remove or do nothing if not exist
     */
    void remove(ZF_IN const T_Key &key) {
        if(d) {
            d->remove(&key);
        }
    }

    /**
     * @brief remove all content
     */
    void removeAll(void) {
        if(d) {
            d->removeAll();
        }
    }

    // ============================================================
    // iterator access
public:
    /** @brief see #zfiter */
    zfiter iter(void) const {
        return d ? d->iter() : zfiter();
    }

    /** @brief see #zfiter */
    zfiter iterFind(ZF_IN const T_Key &key) const {
        return d ? d->iterFind(&key) : zfiter();
    }

    /** @brief see #zfiter */
    const T_Key &iterKey(ZF_IN const zfiter &it) const {
        ZFCoreAssert(d && it);
        return *(const T_Key *)d->iterKey(it);
    }
    /** @brief see #zfiter */
    const T_Value &iterValue(ZF_IN const zfiter &it) const {
        ZFCoreAssert(d && it);
        return *(const T_Value *)d->iterValue(it);
    }

    /** @brief see #zfiter */
    T_Value &iterValue(ZF_IN const zfiter &it) {
        ZFCoreAssert(d && it);
        return *(T_Value *)d->iterValue(it);
    }
    /** @brief see #zfiter */
    void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN const T_Value &value
            ) {
        if(d && it) {
            d->iterValue(it, &value);
        }
    }
    /** @brief see #zfiter */
    void iterRemove(ZF_IN_OUT zfiter &it) {
        if(d && it) {
            d->iterRemove(it);
        }
    }

    /** @brief see #zfiter */
    void iterAdd(
            ZF_IN const zfstring &key
            , ZF_IN const T_Value &value
            ) {
        this->set(key, value);
    }

    // ============================================================
    // order map spec
public:
    /** @brief move order */
    void move(
            ZF_IN zfindex from
            , ZF_IN zfindex to
            ) {
        if(d) {
            d->move(from, to);
        }
    }
    /** @brief key at index */
    T_Key const &keyAt(ZF_IN zfindex index) const {
        ZFCoreAssert(d);
        return *(const T_Key *)d->keyAt(index);
    }
    /** @brief value at index */
    T_Value const &valueAt(ZF_IN zfindex index) const {
        ZFCoreAssert(d);
        return *(const T_Value *)d->valueAt(index);
    }
    /** @brief value at index */
    T_Value &valueAt(ZF_IN zfindex index) {
        ZFCoreAssert(d);
        return *(T_Value *)d->valueAt(index);
    }
    /** @brief remove at index */
    void removeAt(ZF_IN zfindex index) {
        ZFCoreAssert(d);
        d->removeAt(index);
    }
    /** @brief see #zfiter */
    zfiter iterAt(ZF_IN zfindex index) const {
        if(d) {
            return d->iterAt(index);
        }
        else {
            return zfnull;
        }
    }
    /** @brief see #zfiter */
    zfindex iterIndex(ZF_IN const zfiter &it) const {
        if(d) {
            return d->iterIndex(it);
        }
        else {
            return zfindexMax();
        }
    }

private:
    static zfidentity _Hash(ZF_IN const void *key) {
        return zfhash(*(const T_Key *)key);
    }
    static zfbool _Equal(ZF_IN const void *key0, ZF_IN const void *key1) {
        return ZFComparerDefault(*(const T_Key *)key0, *(const T_Key *)key1) == ZFCompareEqual;
    }
    static void *_KeyCopy(ZF_IN const void *key) {
        return zfpoolNew(T_Key, *(const T_Key *)key);
    }
    static void _KeyDestroy(ZF_IN void *key) {
        zfpoolDelete((T_Key *)key);
    }
    static void *_ValueCopy(ZF_IN_OUT_OPT void *exist, ZF_IN const void *value) {
        if(exist) {
            if(value) {
                *(T_Value *)exist = *(const T_Value *)value;
            }
            else {
                *(T_Value *)exist = T_Value();
            }
            return exist;
        }
        else {
            if(value) {
                return zfpoolNew(T_Value, *(const T_Value *)value);
            }
            else {
                return zfpoolNew(T_Value);
            }
        }
    }
    static void _ValueDestroy(ZF_IN void *value) {
        zfpoolDelete((T_Value *)value);
    }
    static void _KeyInfo(ZF_IN_OUT zfstring &ret, ZF_IN const void *key) {
        zftToStringT(ret, *(const T_Key *)key);
    }
    static void _ValueInfo(ZF_IN_OUT zfstring &ret, ZF_IN const void *value) {
        zftToStringT(ret, *(const T_Value *)value);
    }
    static zfbool _ValueEqual(ZF_IN const void *value0, ZF_IN const void *value1) {
        return ZFComparerDefault(*(const T_Value *)value0, *(const T_Value *)value1) == ZFCompareEqual;
    }
private:
    inline void _dInit(void) {
        if(!d) {
            d = _ZFP_ZFCoreOrderMapPrivate::create(
                    _Hash
                    , _Equal
                    , _KeyCopy
                    , _KeyDestroy
                    , _ValueCopy
                    , _ValueDestroy
                    );
        }
    }

private:
    _ZFP_ZFCoreOrderMapPrivate *d;
};
ZFOUTPUT_TYPE_TEMPLATE(ZFM_EXPAND(typename T_Key, typename T_Value), ZFM_EXPAND(ZFCoreOrderMap<T_Key, T_Value>), {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreOrderMap_h_

