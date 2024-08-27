/**
 * @file ZFTaskId.h
 * @brief abstract task id
 */

#ifndef _ZFI_ZFTaskId_h_
#define _ZFI_ZFTaskId_h_

#include "ZFObjectCore.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief abstract task id
 */
zfabstract ZFLIB_ZFCore ZFTaskId : zfextend ZFObject {
    ZFOBJECT_DECLARE_ABSTRACT(ZFTaskId, ZFObject)
public:
    /** @brief stop task */
    virtual void stop(void) zfpurevirtual;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTaskId_h_

