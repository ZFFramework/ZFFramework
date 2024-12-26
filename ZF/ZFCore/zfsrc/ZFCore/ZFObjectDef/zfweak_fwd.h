/**
 * @file zfweak_fwd.h
 * @brief weak reference to ZFObject
 */

#ifndef _ZFI_zfweak_fwd_h_
#define _ZFI_zfweak_fwd_h_

#include "ZFObjectAutoPtr.h"
#include "ZFTypeId.h"
#include "ZFPropertyDeclare.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zffinal zfclass ZFLIB_ZFCore _ZFP_I_zfweak : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_zfweak, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->set(zfnull);
    })

public:
    inline zfbool valid(void) {
        return this->get() != zfnull;
    }
    inline void set(ZF_IN const zfany &obj) {
        this->_ZFP_obj = obj;
    }
    inline zfany get(void) {
        return this->_ZFP_obj;
    }

protected:
    virtual void objectOnInit(ZF_IN ZFObject *obj) {
        this->objectOnInit();
        this->_ZFP_obj = obj;
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
    zfany _ZFP_obj;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfweak_fwd_h_

