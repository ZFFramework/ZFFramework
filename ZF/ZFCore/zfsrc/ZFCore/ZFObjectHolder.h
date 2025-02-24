/**
 * @file ZFObjectHolder.h
 * @brief util class to hold ZFObject
 */

#ifndef _ZFI_ZFObjectHolder_h_
#define _ZFI_ZFObjectHolder_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util class to hold ZFObject
 */
zfclass ZFLIB_ZFCore ZFObjectHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFObjectHolder, ZFObject)

    /** @brief the value holded */
    ZFPROPERTY_RETAIN(zfany, value)

    /** @brief init with object */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFObject *, v))

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }

    zfoverride
    virtual inline void objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
        zfsuper::objectInfoImplAppend(ret);
        if(this->value()) {
            ret += " value=";
            this->value()->objectInfoT(ret);
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectHolder_h_

