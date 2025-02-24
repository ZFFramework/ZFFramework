/**
 * @file ZFSerializableData.h
 * @brief serializable data for #ZFSerializable
 */

#ifndef _ZFI_ZFSerializableData_h_
#define _ZFI_ZFSerializableData_h_

#include "zfany.h"
#include "zfautoFwd.h"
#include "ZFPathInfo.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief whether enable resolve check during serialization, true by default
 *
 * resolve check may consume more CPU but useful during develop
 * to find ill-formed serializable data\n
 * when on, check would be performed and an error output would be printed,
 * further more, assert fail if ZFFramework was built as DEBUG mode\n
 * when off, no check would be performed,
 * also, no resolve state would be saved,
 * and all resolve related method would return false as resolved state\n
 * \n
 * note, you must not change this config if some serialization logic is processing,
 * usually, change it in app entry
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataResolveCheckEnable;

/**
 * @brief keyword for #ZFSerializable to hold a no name node,
 *   which means the data doesn't care about its class name
 *
 * @code
 *   <node key="value" />
 * @endcode
 */
#define ZFSerializableKeyword_node "node"
/**
 * @brief keyword for #ZFSerializable to hold attribute or property's name,
 *   see #ZFSerializableData::propertyName
 */
#define ZFSerializableKeyword_prop "prop"
/**
 * @brief keyword for #ZFSerializable to hold attribute or property's value,
 *   see #ZFSerializableData::propertyValue
 */
#define ZFSerializableKeyword_value "value"
/**
 * @brief keyword for #ZFSerializable's to hold category,
 *   see #ZFSerializableData::category
 */
#define ZFSerializableKeyword_category "category"
/**
 * @brief used to declare a null object
 *
 * @code
 *   <null>
 * @endcode
 * serializing this would result a null with success state\n
 * the actual type for the null is determined by the owner
 */
#define ZFSerializableKeyword_null "null"

// ============================================================
zfclassFwd ZFSerializable;
zfclassFwd _ZFP_ZFSerializableDataPrivate;
/**
 * @brief ZFSerializable's data container, see #ZFSerializable
 *
 * this is a data structure similar to xml,
 * designed to reduce xml dependency
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFSerializableData {
public:
    /** @cond ZFPrivateDoc */
    explicit ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d);
    ZFSerializableData(void);
    ZFSerializableData(ZF_IN const ZFSerializableData &ref);
    ZFSerializableData(ZF_IN const zfnullT &dummy);
    ZFSerializableData &operator = (ZF_IN const zfnullT &dummy);
    ZFSerializableData &operator = (ZF_IN const ZFSerializableData &ref);
    zfbool operator == (ZF_IN const ZFSerializableData &ref) const;
    inline zfbool operator != (ZF_IN const ZFSerializableData &ref) const {return !this->operator == (ref);}
    inline zfbool operator == (ZF_IN const zfnullT &dummy) const {return !this->valid();}
    inline zfbool operator != (ZF_IN const zfnullT &dummy) const {return this->valid();}
    inline operator bool (void) const {return this->valid();}
    ~ZFSerializableData(void);
    /** @endcond */

public:
    /**
     * @brief copy all the contents
     */
    zffinal void copyFrom(ZF_IN const ZFSerializableData &ref);
    /**
     * @brief return a copy of this data
     */
    inline ZFSerializableData copy(void) const {
        ZFSerializableData ret;
        ret.copyFrom(*this);
        return ret;
    }

    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

    // ============================================================
    // local path logic
public:
    /**
     * @brief used for impl to store path related info, see #ZFPathInfo
     */
    zffinal ZFPathInfo pathInfo(void) const;
    /** @brief see #pathInfo */
    zffinal void pathInfo(ZF_IN const ZFPathInfo &pathInfo);
    /**
     * @brief recursively check path info from child to parent
     */
    zffinal ZFPathInfo pathInfoCheck(void) const;

    // ============================================================
    // parent
public:
    /**
     * @brief access parent of this node, return true if parent exists
     */
    zffinal zfbool serializableDataParent(ZF_OUT ZFSerializableData &ret) const;

    // ============================================================
    // class
public:
    /** @brief see #ZFSerializable */
    zffinal void itemClass(ZF_IN const zfstring &classNameFull);
    /** @brief see #ZFSerializable */
    zffinal const zfstring &itemClass(void) const;

    // ============================================================
    // other types
public:
    /** @brief see #ZFSerializable, same as set value of attribute "prop" */
    inline void propertyName(ZF_IN const zfstring &name) {
        this->attr(ZFSerializableKeyword_prop, name);
    }
    /** @brief see #propertyName */
    inline zfstring propertyName(void) const {
        return this->attr(ZFSerializableKeyword_prop);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "value" */
    inline void propertyValue(ZF_IN const zfstring &value) {
        this->attr(ZFSerializableKeyword_value, value);
    }
    /** @brief see #propertyValue */
    inline zfstring propertyValue(void) const {
        return this->attr(ZFSerializableKeyword_value);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "category" */
    inline void category(ZF_IN const zfstring &category) {
        this->attr(ZFSerializableKeyword_category, category);
    }
    /** @brief see #category */
    inline zfstring category(void) const {
        return this->attr(ZFSerializableKeyword_category);
    }

    // ============================================================
public:
    /**
     * @brief clear all the contents
     */
    zffinal void removeAll(void);

    // ============================================================
public:
    /**
     * @brief used to store extra state for this serializable data
     *
     * typically used by impl to achieve additional features
     */
    zffinal void serializableDataTag(
            ZF_IN const zfstring &key
            , ZF_IN ZFObject *tag
            );
    /**
     * @brief see #serializableDataTag
     */
    zffinal zfany serializableDataTag(ZF_IN const zfstring &key) const;
    /** @brief see #serializableDataTag */
    zffinal void serializableDataTagGetAllKeyValue(
            ZF_IN_OUT ZFCoreArray<zfstring> &allKey
            , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
            ) const;
    /**
     * @brief remove tag, same as set tag to null
     */
    zffinal void serializableDataTagRemove(ZF_IN const zfstring &key);
    /**
     * @brief remove tag, return removed tag or null if not exist
     */
    zffinal zfauto serializableDataTagRemoveAndGet(ZF_IN const zfstring &key);
    /**
     * @brief see #serializableDataTag
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void serializableDataTagRemoveAll(void);

    // ============================================================
    // attribute
public:
    /**
     * @brief see #ZFSerializable, remove if value is null (same as #attrRemove)
     *
     * note that order of attributes is not ensured
     * after you add, remove or modify attributes
     */
    zffinal void attr(
            ZF_IN const zfstring &name
            , ZF_IN const zfstring &value
            );
    /** @brief see #attr */
    zffinal zfstring attr(ZF_IN const zfstring &name) const;

    /** @brief see #attr */
    zffinal zfindex attrCount(void) const;

    /**
     * @brief remove value with name, see #attr
     */
    zffinal void attrRemove(ZF_IN const zfstring &name);

    /**
     * @brief remove all attribute
     */
    zffinal void attrRemoveAll(void);

    /** @brief see #zfiter */
    zffinal zfiter attrIterFind(ZF_IN const zfstring &name) const;
    /** @brief see #zfiter */
    zffinal zfiter attrIter(void) const;
    /** @brief see #zfiter */
    zffinal zfstring attrIterKey(ZF_IN const zfiter &it) const;
    /** @brief see #zfiter */
    zffinal zfstring attrIterValue(ZF_IN const zfiter &it) const;

    /** @brief see #zfiter */
    zffinal void attrIterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN const zfstring &value
            );
    /** @brief see #zfiter */
    zffinal void attrIterRemove(ZF_IN_OUT zfiter &it);

    /** @brief see #zfiter */
    zffinal zfbool attrIterResolved(ZF_IN const zfiter &it) const;
    /** @brief see #zfiter */
    zffinal void attrIterResolveMark(ZF_IN const zfiter &it) const;
    /** @brief see #zfiter */
    zffinal void attrIterResolveUnmark(ZF_IN const zfiter &it) const;

    // ============================================================
    // element
public:
    /**
     * @brief add child element
     *
     * note this method won't check duplicate data with same name
     */
    zffinal void child(ZF_IN const ZFSerializableData &element);
    /**
     * @brief add child element
     *
     * note this method won't check duplicate data with same name
     */
    zffinal void child(
            ZF_IN const ZFSerializableData &element
            , ZF_IN zfindex atIndex
            );

    /**
     * @brief replace existing element at index or assert fail if index out of range
     */
    zffinal void childReplace(
            ZF_IN zfindex index
            , ZF_IN const ZFSerializableData &element
            );

    /**
     * @brief find element with name or zfindexMax() if not found
     */
    zffinal zfindex childForName(ZF_IN const zfstring &name) const;
    /**
     * @brief see #childForName
     */
    zffinal zfindex childForCategory(ZF_IN const zfstring &name) const;

    /** @brief see #ZFSerializable */
    zffinal zfindex childCount(void) const;
    /** @brief see #ZFSerializable */
    zffinal const ZFSerializableData &childAt(ZF_IN zfindex index) const;
    /** @brief see #ZFSerializable */
    zffinal ZFSerializableData &childAt(ZF_IN zfindex index);

    /**
     * @brief remove element at index
     */
    zffinal void childRemoveAt(ZF_IN zfindex index);
    /**
     * @brief remove all element
     */
    zffinal void childRemoveAll(void);

    // ============================================================
    // resolved state
public:
    /**
     * @brief used to mark whether the data is resolved
     *
     * useful while serializing some type from serializable,
     * after operation's done,
     * you may check whether all data resolved
     * to see whether there's extra unrecognized data
     */
    zffinal zfbool resolved(void) const;
    /** @brief see #resolved */
    zffinal void resolveMark(void) const;
    /** @brief see #resolved */
    zffinal void resolveUnmark(void) const;

    /** @brief see #resolved */
    zffinal zfbool resolvedAttr(ZF_IN const zfstring &name) const;
    /** @brief see #resolved */
    zffinal void resolveAttrMark(ZF_IN const zfstring &name) const;
    /** @brief see #resolved */
    zffinal void resolveAttrUnmark(ZF_IN const zfstring &name) const;

    /** @brief see #resolved */
    zffinal zfbool resolvedPropertyName(void) const {
        return this->resolvedAttr(ZFSerializableKeyword_prop);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyNameMark(void) const {
        this->resolveAttrMark(ZFSerializableKeyword_prop);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyNameUnmark(void) const {
        this->resolveAttrUnmark(ZFSerializableKeyword_prop);
    }

    /** @brief see #resolved */
    zffinal zfbool resolvedPropertyValue(void) const {
        return this->resolvedAttr(ZFSerializableKeyword_value);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyValueMark(void) const {
        this->resolveAttrMark(ZFSerializableKeyword_value);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyValueUnmark(void) const {
        this->resolveAttrUnmark(ZFSerializableKeyword_value);
    }

    /** @brief see #resolved */
    zffinal zfbool resolvedCategory(void) const {
        return this->resolvedAttr(ZFSerializableKeyword_category);
    }
    /** @brief see #resolved */
    zffinal void resolveCategoryMark(void) const {
        this->resolveAttrMark(ZFSerializableKeyword_category);
    }
    /** @brief see #resolved */
    zffinal void resolveCategoryUnmark(void) const {
        this->resolveAttrUnmark(ZFSerializableKeyword_category);
    }

    /**
     * @brief see #resolved, check whether self and all attribute and child element resolved
     */
    zffinal zfbool resolvedAll(
            ZF_OUT_OPT ZFSerializableData *firstNotResolvedElement = zfnull
            , ZF_OUT_OPT zfstring *firstNotResolvedAttr = zfnull
            ) const;
    /**
     * @brief see #resolved, mark self and all attribute and child element
     */
    zffinal void resolveMarkAll(void) const;
    /**
     * @brief see #resolved, unmark self and all attribute and child element
     */
    zffinal void resolveUnmarkAll(void) const;
    /**
     * @brief see #resolved, mark all attribute
     */
    zffinal void resolveAttrMarkAll(void) const;
    /**
     * @brief see #resolved, unmark all attribute
     */
    zffinal void resolveAttrUnmarkAll(void) const;

    // ============================================================
    // other functions
public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /**
     * @brief get a short info
     */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
    /**
     * @brief compare by instance
     */
    zffinal ZFCompareResult objectCompare(ZF_IN const ZFSerializableData &another) const {
        return d == another.d ? ZFCompareEqual : ZFCompareUncomparable;
    }
    /**
     * @brief compare another serializable data by contents
     *
     * if two data considered as same,
     * attribute's order have no need to be same,
     * but child element's order must be same
     */
    zffinal ZFCompareResult objectCompareValue(ZF_IN const ZFSerializableData &another) const;

public:
    /**
     * @brief whether the data is valid
     */
    inline zfbool valid(void) const {return d;}

private:
    _ZFP_ZFSerializableDataPrivate *d;
    friend zfclassFwd _ZFP_ZFSerializableDataPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableData_h_

