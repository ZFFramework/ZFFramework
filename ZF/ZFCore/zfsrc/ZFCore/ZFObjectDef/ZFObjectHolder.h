/**
 * @file ZFObjectHolder.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFObjectHolder_h_
#define _ZFI_ZFObjectHolder_h_

#include "ZFObjectAutoPtr.h"
#include "ZFTypeId.h"
#include "ZFPropertyDeclare.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFObjectHolder
/**
 * @brief util object to hold a ZFObject but don't retain it
 *
 * most of classes and methods use auto-retain logic to hold a ZFObject,
 * so we supply this holder class that won't retain it's content,
 * use only if necessary\n
 * typically, you should use #ZFObject::objectHolder for most case
 */
zffinal zfclass ZFLIB_ZFCore ZFObjectHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFObjectHolder, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->objectHolded(zfnull);
    })

public:
    /**
     * @brief set the holded object
     */
    inline void objectHolded(ZF_IN ZFObject *obj) {
        this->_ZFP_objectHolded = obj;
    }
    /**
     * @brief set the holded object
     */
    template<typename T_ZFObject>
    inline void objectHolded(ZF_IN T_ZFObject obj) {
        this->_ZFP_objectHolded = obj;
    }

public:
    /**
     * @brief get the holded object
     */
    inline zfany objectHolded(void) {
        return this->_ZFP_objectHolded;
    }

protected:
    /** @brief see #ZFObject::objectOnInit */
    virtual void objectOnInit(ZF_IN ZFObject *obj) {
        this->objectOnInit();
        this->_ZFP_objectHolded = obj;
    }
    /** @brief see #ZFObject::objectOnInit */
    template<typename T_ZFObject>
    void objectOnInit(ZF_IN T_ZFObject const &obj) {
        this->objectOnInit();
        this->_ZFP_objectHolded = obj;
    }

    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }

    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

public:
    zfoverride
    virtual zfidentity objectHashImpl(void);
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj);

public:
    zfoverride
    virtual zfbool objectIsInternal(void) {
        return zftrue;
    }
    zfoverride
    virtual zfbool objectIsInternalPrivate(void) {
        return zftrue;
    }

public:
    zfany _ZFP_objectHolded;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectHolder_h_

