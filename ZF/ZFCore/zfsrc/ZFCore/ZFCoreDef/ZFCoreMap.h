/**
 * @file ZFCoreMap.h
 * @brief core map type for private use only
 */

#ifndef _ZFI_ZFCoreMap_h_
#define _ZFI_ZFCoreMap_h_

#include "ZFCorePointer.h"
#include "ZFCoreArray.h"
#include "zfiter.h"
#include "ZFToken.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFCoreMapPrivate;
/**
 * @brief core map type for private use only
 *
 * used to reduce dependency of stl\n
 * use string as key,
 * null key is considered same as empty string\n
 * use #ZFCorePointerBase as value,
 * which use retain logic and can hold many types
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFCoreMap {
public:
    /**
     * @brief construct an empty map
     */
    ZFCoreMap(void);
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreMap(ZF_IN const ZFCoreMap &ref);
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    zffinal ZFCoreMap &operator = (ZF_IN const ZFCoreMap &ref);
    /**
     * @brief true if same ref
     */
    zffinal zfbool operator == (ZF_IN const ZFCoreMap &ref) const;
    /**
     * @brief true if not same ref
     */
    zffinal zfbool operator != (ZF_IN const ZFCoreMap &ref) const {return !this->operator == (ref);}
    ~ZFCoreMap(void);

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
    zffinal void swap(ZF_IN_OUT ZFCoreMap &ref);

public:
    /**
     * @brief copy all contents from ref, remove all before copy
     */
    zffinal void copyFrom(ZF_IN const ZFCoreMap &ref);

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
    zffinal void addFrom(ZF_IN const ZFCoreMap &ref);

    /**
     * @brief change value or create if not exist,
     *   value would be retained by this method
     *
     * null key is considered same as empty string\n
     * assert fail if value is null (use an empty smart pointer to store null value)
     */
    zffinal void set(
            ZF_IN const zfstring &key
            , ZF_IN const ZFCorePointerBase &value
            );
    /**
     * @brief get value or null if not exist
     */
    zffinal const ZFCorePointerBase *get(ZF_IN const zfstring &key) const;
    /**
     * @brief get value or null if not exist
     */
    template<typename T_Element>
    T_Element get(ZF_IN const zfstring &key) const {
        const ZFCorePointerBase *t = this->get(key);
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
    zffinal void allValueT(ZF_IN_OUT ZFCoreArray<const ZFCorePointerBase *> &ret) const;
    /**
     * @brief return a copy of all values
     */
    inline ZFCoreArray<const ZFCorePointerBase *> allValue(void) const {
        ZFCoreArray<const ZFCorePointerBase *> ret;
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
    zffinal const ZFCorePointerBase *iterValue(ZF_IN const zfiter &it) const;

    /** @brief see #zfiter */
    zffinal void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN const ZFCorePointerBase &newValue
            );
    /** @brief see #zfiter */
    zffinal void iterRemove(ZF_IN_OUT zfiter &it);

    /** @brief see #zfiter */
    zffinal void iterAdd(
            ZF_IN const zfstring &key
            , ZF_IN const ZFCorePointerBase &value
            );

    /** @brief see #zfiter */
    template<typename T_Element>
    T_Element iterValue(ZF_IN const zfiter &it) const {
        const ZFCorePointerBase *t = this->iterValue(it);
        if(t != zfnull) {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }

private:
    _ZFP_ZFCoreMapPrivate *d;
};
ZFOUTPUT_TYPE(ZFCoreMap, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreMap_h_

