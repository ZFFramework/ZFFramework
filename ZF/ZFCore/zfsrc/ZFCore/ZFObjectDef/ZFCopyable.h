/**
 * @file ZFCopyable.h
 * @brief interface shows a ZFObject is copyable
 */

#ifndef _ZFI_ZFCopyable_h_
#define _ZFI_ZFCopyable_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief interface shows a ZFObject is copyable
 */
zfinterface ZFLIB_ZFCore ZFCopyable : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFCopyable, ZFInterface)

public:
    /**
     * @brief return a copy of this object
     */
    zffinal zfautoT<ZFCopyable> copy(void);
    /**
     * @brief see #copy, anotherObj must be same type as this object (by classData),
     *   otherwise, do nothing
     */
    zffinal void copyFrom(ZF_IN ZFObject *anotherObj);

protected:
    /**
     * @brief called by #copy to copy contents from anotherObj
     *
     * anotherObj is ensured the same type as self,
     * ensured not null,
     * and ensured not same instance of this\n
     * by default this method would do nothing for performance,
     * you may use #ZFPropertyCopyAll to copy all properties for most case
     */
    virtual inline void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj) {
    }
};

/**
 * @brief util to copy as #ZFCopyable or return null if not available
 */
inline zfauto ZFObjectCopy(ZF_IN ZFObject *obj) {
    ZFCopyable *t = zfcast(ZFCopyable *, obj);
    if(t) {
        return t->copy();
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCopyable_h_

