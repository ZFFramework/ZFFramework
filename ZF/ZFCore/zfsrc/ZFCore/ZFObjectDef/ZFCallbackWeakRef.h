/**
 * @file ZFCallbackWeakRef.h
 * @brief hold weak reference to ZFCallback
 */

#ifndef _ZFI_ZFCallbackWeakRef_h_
#define _ZFI_ZFCallbackWeakRef_h_

#include "ZFObjectCore.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief hold weak reference to ZFCallback
 */
zfclass ZFLIB_ZFCore ZFCallbackWeakRef : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFCallbackWeakRef, ZFObject)

public:
    /** @brief get the callback */
    ZFCallback get(void);
    /** @brief set the callback */
    void set(ZF_IN const ZFCallback &v);
    /** @brief set to null */
    void set(ZF_IN const zfnullT &v);

private:
    _ZFP_ZFCallbackPrivate *d;
    ZFListener c;
protected:
    ZFCallbackWeakRef(void) : d(zfnull), c() {}
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief init with callback */
    virtual void objectOnInit(ZF_IN const ZFCallback &v) {
        this->objectOnInit();
        this->set(v);
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallbackWeakRef_h_

