/**
 * @file ZFValueHolder.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFValueHolder_h_
#define _ZFI_ZFValueHolder_h_

#include "ZFObjectCore.h"
#include "ZFObjectRetain.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFValueHolder
zfclassFwd ZFValueHolder;
/**
 * @brief type for #ZFValueHolder
 *
 * to achieve custom #ZFValueHolder, supply a function with following proto type,
 * which cleanup the internal #ZFValueHolder::holdedData:
 * @code
 *   void type(ZF_IN void *holdedData);
 * @endcode
 */
typedef void (*ZFValueHolderType)(ZF_IN void *holdedData);
/**
 * @brief used to hold a non ZFObject type for performance
 */
zffinal zfclass ZFLIB_ZFCore ZFValueHolder : zfextend ZFObject {
    // ============================================================
public:
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFValueHolder, ZFObject)

protected:
    ZFValueHolder(void) : holdedData(zfnull), holderType(zfnull) {}

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void) {
        if(this->holdedData && this->holderType) {
            void *holdedData = this->holdedData;
            ZFValueHolderType holderType = this->holderType;
            this->holdedData = zfnull;
            this->holderType = zfnull;
            holderType(holdedData);
        }
        else {
            this->holdedData = zfnull;
            this->holderType = zfnull;
        }
    }

protected:
    /**
     * @brief init with object
     */
    virtual void objectOnInit(
        ZF_IN void *holdedData
        , ZF_IN ZFValueHolderType holderType
        ) {
        this->holdedData = holdedData;
        this->holderType = holderType;
    }
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->_cleanup();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    /**
     * @brief used to hold the raw pointer
     *
     * you must ensure it's valid during the holder object's life cycle,
     * would be cleared by #holderType when the holder object deallocated
     */
    void *holdedData;
    /**
     * @brief delete callback to clear the #holdedData
     */
    ZFValueHolderType holderType;

public:
    /** @brief util method to cast #holdedData */
    template<typename T_RawType>
    inline T_RawType holdedDataPointer(void) {
        return (T_RawType)this->holdedData;
    }
    /** @brief util method to cast #holdedData */
    template<typename T_RawType>
    inline T_RawType holdedDataRef(void) {
        typedef typename zftTraits<T_RawType>::TrPtr T_RawTypePointer;
        return *(T_RawTypePointer)this->holdedData;
    }

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);
};
zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFValueHolderType {
public:
    static void TypePointerRef(ZF_IN void *holdedData) {
    }
    static void TypePOD(ZF_IN void *holdedData) {
        zffree(holdedData);
    }
    template<typename T_Object>
    static void TypeObject(ZF_IN void *holdedData) {
        zfdelete((T_Object)holdedData);
    }
    template<typename T_Object>
    static void TypePoolObject(ZF_IN void *holdedData) {
        zfpoolDelete((T_Object)holdedData);
    }
};
/** @brief see #ZFValueHolderType */
#define ZFValueHolderTypePointerRef() _ZFP_ZFValueHolderType::TypePointerRef
/** @brief see #ZFValueHolderType */
#define ZFValueHolderTypePOD() _ZFP_ZFValueHolderType::TypePOD
/** @brief see #ZFValueHolderType */
#define ZFValueHolderTypeObject(T_Object) _ZFP_ZFValueHolderType::TypeObject<T_Object>
/** @brief see #ZFValueHolderType */
#define ZFValueHolderTypePoolObject(T_Object) _ZFP_ZFValueHolderType::TypePoolObject<T_Object>

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFValueHolder_h_

