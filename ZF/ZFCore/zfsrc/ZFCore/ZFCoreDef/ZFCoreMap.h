/**
 * @file ZFCoreMap.h
 * @brief core map type for private use only
 */

#ifndef _ZFI_ZFCoreMap_h_
#define _ZFI_ZFCoreMap_h_

#include "ZFMemPool.h"
#include "ZFCoreArray.h"
#include "zfiter.h"
#include "ZFToken.h"
#include "ZFCoreUtilMacro.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFCoreMap {
public:
    zfclassNotPOD ZFLIB_ZFCore BaseKey {
    public:
        virtual ~BaseKey(void) {}
    public:
        virtual zfidentity implHash(void) const zfpurevirtual;
        virtual zfbool implEqual(ZF_IN const BaseKey *ref) const zfpurevirtual;
        virtual void implInfo(ZF_IN_OUT zfstring &ret) const zfpurevirtual;
        virtual BaseKey *implCopy(void) const zfpurevirtual;
        virtual void implDestroy(void) zfpurevirtual;
    };
    zfclassNotPOD ZFLIB_ZFCore BaseValue {
    public:
        virtual ~BaseValue(void) {}
    public:
        virtual void implCopy(ZF_IN const BaseValue *ref) zfpurevirtual;
        virtual zfbool implEqual(ZF_IN const BaseValue *ref) const zfpurevirtual;
        virtual void implInfo(ZF_IN_OUT zfstring &ret) const zfpurevirtual;
        virtual BaseValue *implCopy(void) const zfpurevirtual;
        virtual void implDestroy(void) zfpurevirtual;
    };
public:
    typedef BaseValue *(*Fn_ValueCreate)(void);
public:
    zfuint refCount;
public:
    static _ZFP_ZFCoreMap *create(void);
    static void destroy(ZF_IN _ZFP_ZFCoreMap *d);
    _ZFP_ZFCoreMap(void) : refCount(1) {}
    virtual ~_ZFP_ZFCoreMap(void) {}
public:
    virtual void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN zfindex maxCount
            , ZF_IN const ZFTokenForKeyValueContainer &token
            ) zfpurevirtual;
    virtual ZFCompareResult objectCompareValue(ZF_IN const _ZFP_ZFCoreMap *ref) zfpurevirtual;
    virtual void copyFrom(ZF_IN_OUT _ZFP_ZFCoreMap *ref) zfpurevirtual;
    virtual void addFrom(ZF_IN_OUT _ZFP_ZFCoreMap *ref) zfpurevirtual;
    virtual zfindex count(void) zfpurevirtual;
    virtual zfbool isEmpty(void) zfpurevirtual;
    virtual zfbool isContain(ZF_IN BaseKey *key) zfpurevirtual;
    virtual void set(ZF_IN BaseKey *key, ZF_IN BaseValue *value) zfpurevirtual;
    virtual BaseValue *get(ZF_IN BaseKey *key) zfpurevirtual;
    virtual BaseValue *access(ZF_IN BaseKey *key, ZF_IN Fn_ValueCreate fn_ValueCreate) zfpurevirtual;
    virtual void remove(ZF_IN BaseKey *key) zfpurevirtual;
    virtual void removeAll(void) zfpurevirtual;
public:
    virtual zfiter iter(void) zfpurevirtual;
    virtual zfiter iterFind(ZF_IN BaseKey *key) zfpurevirtual;
    virtual const BaseKey *iterKey(ZF_IN const zfiter &it) zfpurevirtual;
    virtual BaseValue *iterValue(ZF_IN const zfiter &it) zfpurevirtual;
    virtual void iterValue(ZF_IN_OUT zfiter &it, ZF_IN BaseValue *value) zfpurevirtual;
    virtual void iterRemove(ZF_IN_OUT zfiter &it) zfpurevirtual;
};
/**
 * @brief core map type for private use only
 *
 * used to reduce dependency of stl\n
 * key must support #zftHash and `operator ==`
 */
template<typename T_Key, typename T_Value>
zfclassLikePOD ZFCoreMap {
public:
    /**
     * @brief construct an empty map
     */
    ZFCoreMap(void) : d(zfnull) {}
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreMap(ZF_IN const ZFCoreMap<T_Key, T_Value> &ref) : d(ref.d) {
        if(d) {
            ++(d->refCount);
        }
    }
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreMap &operator = (ZF_IN const ZFCoreMap<T_Key, T_Value> &ref) {
        _ZFP_ZFCoreMap *dTmp = d;
        d = ref.d;
        if(d) {
            ++(d->refCount);
        }
        if(dTmp && (--(dTmp->refCount)) == 0) {
            _ZFP_ZFCoreMap::destroy(dTmp);
        }
        return *this;
    }
    /**
     * @brief true if same ref
     */
    zfbool operator == (ZF_IN const ZFCoreMap<T_Key, T_Value> &ref) const {return d == ref.d;}
    /**
     * @brief true if not same ref
     */
    zfbool operator != (ZF_IN const ZFCoreMap<T_Key, T_Value> &ref) const {return d != ref.d;}
    ~ZFCoreMap(void) {
        if(d && (--(d->refCount)) == 0) {
            _ZFP_ZFCoreMap::destroy(d);
        }
    }

    /**
     * @brief prepare instance to make it able to be shared between each copy
     */
    void refPrepare(void) {_dInit();}
    /**
     * @brief delete reference
     */
    void refDelete(void) {
        if(d) {
            _ZFP_ZFCoreMap *dTmp = d;
            d = zfnull;
            if(--(dTmp->refCount) == 0) {
                zfpoolDelete(dTmp);
            }
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
    ZFCompareResult objectCompare(ZF_IN ZFCoreMap<T_Key, T_Value> const &ref) const {
        return d == ref.d ? ZFCompareEqual : ZFCompareUncomparable;
    }
    /** @brief compare by instance */
    ZFCompareResult objectCompareValue(ZF_IN ZFCoreMap<T_Key, T_Value> const &ref) const {
        if(d) {
            if(ref.d) {
                return d->objectCompareValue(ref.d);
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
            d->objectInfoOfContentT(ret, maxCount, token);
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
    void swap(ZF_IN_OUT ZFCoreMap<T_Key, T_Value> &ref) {
        if(d != ref.d) {
            _ZFP_ZFCoreMap *dTmp = d;
            d = ref.d;
            ref.d = dTmp;
        }
    }

public:
    /**
     * @brief copy all contents from ref, remove all before copy
     */
    void copyFrom(ZF_IN const ZFCoreMap<T_Key, T_Value> &ref) {
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
        return d && d->isContain(_KeyCreate(key));
    }

    /**
     * @brief add elements from ref
     */
    void addFrom(ZF_IN const ZFCoreMap<T_Key, T_Value> &ref) {
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
        d->set(_KeyCreate(key), _ValueCreate(value));
    }
    /**
     * @brief get value or null if not exist
     */
    const T_Value *get(ZF_IN const T_Key &key) const {
        if(d) {
            ImplValue *value = (ImplValue *)d->get(_KeyCreate(key));
            if(value) {
                return &(value->v);
            }
        }
        return zfnull;
    }
    /**
     * @brief get value or null if not exist
     */
    T_Value *get(ZF_IN const T_Key &key) {
        if(d) {
            ImplValue *value = (ImplValue *)d->get(_KeyCreate(key));
            if(value) {
                return &(value->v);
            }
        }
        return zfnull;
    }
    /**
     * @brief access value or create if not exist
     */
    T_Value &access(ZF_IN const T_Key &key) {
        _dInit();
        return ((ImplValue *)d->access(_KeyCreate(key), _ValueCreate))->v;
    }

    /** @brief see #allKey */
    void allKeyT(ZF_IN_OUT ZFCoreArray<T_Key> &ret) const {
        if(d) {
            for(zfiter it = d->iter(); it; ++it) {
                ret.add(((const ImplKey *)d->iterKey(it))->v);
            }
        }
    }
    /**
     * @brief return a copy of all keys
     */
    inline ZFCoreArray<T_Key> allKey(void) const {
        ZFCoreArray<T_Key> ret;
        this->allKeyT(ret);
        return ret;
    }

    /** @brief see #allValue */
    void allValueT(ZF_IN_OUT ZFCoreArray<T_Value> &ret) const {
        if(d) {
            for(zfiter it = d->iter(); it; ++it) {
                ret.add(((const ImplValue *)d->iterValue(it))->v);
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
            d->remove(_KeyCreate(key));
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
        return d ? d->iterFind(_KeyCreate(key)) : zfiter();
    }

    /** @brief see #zfiter */
    const T_Key &iterKey(ZF_IN const zfiter &it) const {
        return ((const ImplKey *)d->iterKey(it))->v;
    }
    /** @brief see #zfiter */
    const T_Value &iterValue(ZF_IN const zfiter &it) const {
        return ((const ImplValue *)d->iterValue(it))->v;
    }

    /** @brief see #zfiter */
    T_Value &iterValue(ZF_IN const zfiter &it) {
        return ((ImplValue *)d->iterValue(it))->v;
    }
    /** @brief see #zfiter */
    void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN const T_Value &value
            ) {
        if(d && it) {
            d->iterValue(it, _ValueCreate(value));
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
            ZF_IN const T_Key &key
            , ZF_IN const T_Value &value
            ) {
        this->set(key, value);
    }

private:
    zfclassNotPOD ImplKey : zfextend _ZFP_ZFCoreMap::BaseKey {
    public:
        T_Key v;
        ImplKey(ZF_IN T_Key const &v) : v(v) {}
    public:
        virtual zfidentity implHash(void) const {return zfhash(v);}
        virtual zfbool implEqual(ZF_IN const BaseKey *ref) const {return ZFComparerDefault(v, ((ImplKey *)ref)->v) == ZFCompareEqual;}
        virtual void implInfo(ZF_IN_OUT zfstring &ret) const {return zftToStringT(ret, v);}
        virtual BaseKey *implCopy(void) const {return zfpoolNew(ImplKey, v);}
        virtual void implDestroy(void) {zfpoolDelete(this);}
    };
    zfclassNotPOD ImplValue : zfextend _ZFP_ZFCoreMap::BaseValue {
    public:
        T_Value v;
        ImplValue(void) : v() {}
        ImplValue(ZF_IN T_Value const &v) : v(v) {}
    public:
        virtual void implCopy(ZF_IN const BaseValue *ref) {v = ((ImplValue *)ref)->v;}
        virtual zfbool implEqual(ZF_IN const BaseValue *ref) const {return ZFComparerDefault(v, ((ImplValue *)ref)->v) == ZFCompareEqual;}
        virtual void implInfo(ZF_IN_OUT zfstring &ret) const {return zftToStringT(ret, v);}
        virtual BaseValue *implCopy(void) const {return zfpoolNew(ImplValue, v);}
        virtual void implDestroy(void) {zfpoolDelete(this);}
    };
    static _ZFP_ZFCoreMap::BaseKey *_KeyCreate(ZF_IN T_Key const &v) {
        return zfpoolNew(ImplKey, v);
    }
    static _ZFP_ZFCoreMap::BaseValue *_ValueCreate(ZF_IN T_Value const &v) {
        return zfpoolNew(ImplValue, v);
    }
    static _ZFP_ZFCoreMap::BaseValue *_ValueCreate(void) {
        return zfpoolNew(ImplValue);
    }

private:
    inline void _dInit(void) {
        if(!d) {
            d = _ZFP_ZFCoreMap::create();
        }
    }

private:
    _ZFP_ZFCoreMap *d;
};
ZFOUTPUT_TYPE_TEMPLATE(ZFM_EXPAND(typename T_Key, typename T_Value), ZFM_EXPAND(ZFCoreMap<T_Key, T_Value>), {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreMap_h_

