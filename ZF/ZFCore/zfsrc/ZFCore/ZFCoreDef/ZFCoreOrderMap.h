/**
 * @file ZFCoreOrderMap.h
 * @brief map type which keeps item order, for private use only
 */

#ifndef _ZFI_ZFCoreOrderMap_h_
#define _ZFI_ZFCoreOrderMap_h_

#include "ZFCorePointer.h"
#include "ZFCoreArray.h"
#include "zfiter.h"
#include "ZFToken.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFCoreOrderMapPrivate;
/**
 * @brief map type which keeps item order, for private use only
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFCoreOrderMap {
public:
    /**
     * @brief construct an empty map
     */
    ZFCoreOrderMap(void);
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreOrderMap(ZF_IN const ZFCoreOrderMap &ref);
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    zffinal ZFCoreOrderMap &operator = (ZF_IN const ZFCoreOrderMap &ref);
    /**
     * @brief true if same ref
     */
    zffinal zfbool operator == (ZF_IN const ZFCoreOrderMap &ref) const;
    /**
     * @brief true if not same ref
     */
    zffinal zfbool operator != (ZF_IN const ZFCoreOrderMap &ref) const {return !this->operator == (ref);}
    ~ZFCoreOrderMap(void);

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief see #objectInfoOfContent */
    zffinal void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
            ) const;
    /** @brief return contents info */
    zffinal zfstring objectInfoOfContent(
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
    zffinal void swap(ZF_IN_OUT ZFCoreOrderMap &ref);

public:
    /**
     * @brief copy all contents from ref, remove all before copy
     */
    zffinal void copyFrom(ZF_IN const ZFCoreOrderMap &ref);

    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

public:
    /**
     * @brief return count
     */
    zffinal zfindex count(void) const;

    /**
     * @brief true if empty
     */
    zffinal zfbool isEmpty(void) const;

    /**
     * @brief true if contains the key
     */
    zffinal zfbool isContain(ZF_IN const zfstring &key) const;

    /**
     * @brief add elements from ref
     */
    zffinal void addFrom(ZF_IN const ZFCoreOrderMap &ref);

    /**
     * @brief change value or create if not exist,
     *   value would be retained by this method
     *
     * null key is considered same as empty string\n
     * assert fail if value is null (use an empty smart pointer to store null value)
     */
    zffinal void set(
            ZF_IN const zfstring &key
            , ZF_IN const ZFCorePointer &value
            );
    /**
     * @brief get value or null if not exist
     */
    zffinal const ZFCorePointer *get(ZF_IN const zfstring &key) const;
    /**
     * @brief get value or null if not exist
     */
    template<typename T_Element>
    T_Element get(ZF_IN const zfstring &key) const {
        const ZFCorePointer *t = this->get(key);
        if(t != zfnull) {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }

    /** @brief see #allKey */
    zffinal void allKeyT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) const;
    /**
     * @brief return a copy of all keys
     */
    inline ZFCoreArray<zfstring> allKey(void) const {
        ZFCoreArray<zfstring> ret;
        this->allKeyT(ret);
        return ret;
    }

    /** @brief see #allValue */
    zffinal void allValueT(ZF_IN_OUT ZFCoreArray<const ZFCorePointer *> &ret) const;
    /**
     * @brief return a copy of all values
     */
    inline ZFCoreArray<const ZFCorePointer *> allValue(void) const {
        ZFCoreArray<const ZFCorePointer *> ret;
        this->allValueT(ret);
        return ret;
    }

    /** @brief see #allValue */
    template<typename T_Element>
    void allValueT(ZF_IN_OUT ZFCoreArray<T_Element> &ret) const {
        ret.capacity(ret.count() + this->count());
        for(zfiter it = this->iter(); it; ++it) {
            ret.add(this->iterValue<T_Element>(it));
        }
    }

    /**
     * @brief remove or do nothing if not exist
     */
    zffinal void remove(ZF_IN const zfstring &key);

    /**
     * @brief remove all content
     */
    zffinal void removeAll(void);

    // ============================================================
    // iterator access
public:
    /** @brief see #zfiter */
    zffinal zfiter iter(void) const;

    /** @brief see #zfiter */
    zffinal zfiter iterFind(ZF_IN const zfstring &key) const;

    /** @brief see #zfiter */
    zffinal zfstring iterKey(ZF_IN const zfiter &it) const;
    /** @brief see #zfiter */
    zffinal const ZFCorePointer *iterValue(ZF_IN const zfiter &it) const;

    /** @brief see #zfiter */
    zffinal void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN const ZFCorePointer &newValue
            );
    /** @brief see #zfiter */
    zffinal void iterRemove(ZF_IN_OUT zfiter &it);

    /** @brief see #zfiter */
    zffinal void iterAdd(
            ZF_IN const zfstring &key
            , ZF_IN const ZFCorePointer &value
            );

    /** @brief see #zfiter */
    template<typename T_Element>
    T_Element iterValue(ZF_IN const zfiter &it) const {
        const ZFCorePointer *t = this->iterValue(it);
        if(t != zfnull) {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }

    // ============================================================
    // order map spec
public:
    /**
     * @brief try update value order by moving to tail, return null if not exist
     */
    zffinal const ZFCorePointer *update(ZF_IN const zfstring &key) const;
    /**
     * @brief try update value order by moving to tail, return null if not exist
     */
    template<typename T_Element>
    T_Element update(ZF_IN const zfstring &key) const {
        const ZFCorePointer *t = this->update(key);
        if(t != zfnull) {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }

private:
    _ZFP_ZFCoreOrderMapPrivate *d;
};
ZFOUTPUT_TYPE(ZFCoreOrderMap, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreOrderMap_h_

