/**
 * @file ZFCoreSet.h
 * @brief core set type for private use only
 */

#ifndef _ZFI_ZFCoreSet_h_
#define _ZFI_ZFCoreSet_h_

#include "ZFCoreMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFCoreSet {
public:
    zfuint refCount;
public:
    static _ZFP_ZFCoreSet *create(void);
    static void destroy(ZF_IN _ZFP_ZFCoreSet *d);
    _ZFP_ZFCoreSet(void) : refCount(1) {}
    virtual ~_ZFP_ZFCoreSet(void) {}
public:
    virtual void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN zfindex maxCount
            , ZF_IN const ZFTokenForContainer &token
            ) zfpurevirtual;
    virtual ZFCompareResult objectCompareValue(ZF_IN const _ZFP_ZFCoreSet *ref) zfpurevirtual;
    virtual void copyFrom(ZF_IN_OUT _ZFP_ZFCoreSet *ref) zfpurevirtual;
    virtual void addFrom(ZF_IN_OUT _ZFP_ZFCoreSet *ref) zfpurevirtual;
    virtual void capacity(ZF_IN zfindex capacity) zfpurevirtual;
    virtual zfindex count(void) zfpurevirtual;
    virtual zfbool isEmpty(void) zfpurevirtual;
    virtual zfbool isContain(ZF_IN _ZFP_ZFCoreMapKey *key) zfpurevirtual;
    virtual void add(ZF_IN _ZFP_ZFCoreMapKey *key) zfpurevirtual;
    virtual void remove(ZF_IN _ZFP_ZFCoreMapKey *key) zfpurevirtual;
    virtual void removeAll(void) zfpurevirtual;
public:
    virtual zfiter iter(void) zfpurevirtual;
    virtual zfiter iterFind(ZF_IN _ZFP_ZFCoreMapKey *key) zfpurevirtual;
    virtual const _ZFP_ZFCoreMapKey *iterValue(ZF_IN const zfiter &it) zfpurevirtual;
    virtual void iterRemove(ZF_IN_OUT zfiter &it) zfpurevirtual;
    virtual zfiter iterAdd(ZF_IN _ZFP_ZFCoreMapKey *key) zfpurevirtual;
};
/**
 * @brief core set type for private use only
 *
 * used to reduce dependency of stl\n
 * key must support #zftHash and `operator ==`
 */
template<typename T_Key, typename T_Hash = ZFCoreMapKeyHash<T_Key>, typename T_Equal = ZFCoreMapKeyEqual<T_Key> >
zfclassLikePOD ZFCoreSet {
protected:
    /** @brief typedef for self */
    typedef ZFCoreSet<T_Key, T_Hash, T_Equal> zfself;
public:
    /**
     * @brief construct an empty set
     */
    ZFCoreSet(void) : d(zfnull) {}
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreSet(ZF_IN const zfself &ref) : d(ref.d) {
        if(d) {
            ++(d->refCount);
        }
    }
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreSet &operator = (ZF_IN const zfself &ref) {
        _ZFP_ZFCoreSet *dTmp = d;
        d = ref.d;
        if(d) {
            ++(d->refCount);
        }
        if(dTmp && (--(dTmp->refCount)) == 0) {
            _ZFP_ZFCoreSet::destroy(dTmp);
        }
        return *this;
    }
    /**
     * @brief true if same ref
     */
    zfbool operator == (ZF_IN const zfself &ref) const {return d == ref.d;}
    /**
     * @brief true if not same ref
     */
    zfbool operator != (ZF_IN const zfself &ref) const {return d != ref.d;}
    ~ZFCoreSet(void) {
        if(d && (--(d->refCount)) == 0) {
            _ZFP_ZFCoreSet::destroy(d);
        }
    }

    /**
     * @brief prepare instance to make it able to be shared between each copy
     */
    zfself &refPrepare(void) {_dInit(); return *this;}
    /**
     * @brief delete reference
     */
    void refDelete(void) {
        if(d) {
            _ZFP_ZFCoreSet *dTmp = d;
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
    ZFCompareResult objectCompare(ZF_IN zfself const &ref) const {
        return d == ref.d ? ZFCompareEqual : ZFCompareUncomparable;
    }
    /** @brief compare by instance */
    ZFCompareResult objectCompareValue(ZF_IN zfself const &ref) const {
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
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
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
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            ) const {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token);
        return ret;
    }

public:
    /**
     * @brief swap internal data
     */
    void swap(ZF_IN_OUT zfself &ref) {
        if(d != ref.d) {
            _ZFP_ZFCoreSet *dTmp = d;
            d = ref.d;
            ref.d = dTmp;
        }
    }

public:
    /**
     * @brief copy all contents from ref, remove all before copy
     */
    void copyFrom(ZF_IN const zfself &ref) {
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
     * @brief change capacity
     */
    void capacity(ZF_IN zfindex capacity) {
        _dInit();
        d->capacity(capacity);
    }

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
    void addFrom(ZF_IN const zfself &ref) {
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
     * @brief add value
     */
    void add(ZF_IN const T_Key &key) {
        _dInit();
        d->add(_KeyCreate(key));
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
    const T_Key &iterValue(ZF_IN const zfiter &it) const {
        return ((const ImplKey *)d->iterValue(it))->v;
    }
    /** @brief see #zfiter */
    void iterRemove(ZF_IN_OUT zfiter &it) {
        if(d && it) {
            d->iterRemove(it);
        }
    }

    /** @brief see #zfiter */
    zfiter iterAdd(ZF_IN const T_Key &key) {
        _dInit();
        return d->iterAdd(_KeyCreate(key));
    }

private:
    zfclassNotPOD ImplKey : zfextend _ZFP_ZFCoreMapKey {
    public:
        T_Key v;
        ImplKey(ZF_IN T_Key const &v) : v(v) {}
    public:
        virtual zfidentity implHash(void) const {return T_Hash()(v);}
        virtual zfbool implEqual(ZF_IN const _ZFP_ZFCoreMapKey *ref) const {return T_Equal()(v, ((ImplKey *)ref)->v);}
        virtual void implInfo(ZF_IN_OUT zfstring &ret) const {return zftToStringT(ret, v);}
        virtual _ZFP_ZFCoreMapKey *implCopy(void) const {return zfpoolNew(ImplKey, v);}
        virtual void implDestroy(void) {zfpoolDelete(this);}
    };
    static _ZFP_ZFCoreMapKey *_KeyCreate(ZF_IN T_Key const &v) {
        return zfpoolNew(ImplKey, v);
    }

private:
    inline void _dInit(void) {
        if(!d) {
            d = _ZFP_ZFCoreSet::create();
        }
    }

private:
    _ZFP_ZFCoreSet *d;
};
ZFOUTPUT_TYPE_TEMPLATE(ZFM_EXPAND(typename T_Key, typename T_Hash, typename T_Equal), ZFM_EXPAND(ZFCoreSet<T_Key, T_Hash, T_Equal>), {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreSet_h_

