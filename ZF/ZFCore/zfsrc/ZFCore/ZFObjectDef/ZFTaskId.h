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
zfinterface ZFLIB_ZFCore ZFTaskId : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFTaskId, ZFInterface)
public:
    /** @brief stop task */
    virtual void stop(void) zfpurevirtual;
};

/**
 * @brief basic task id for impl
 */
zfclass ZFLIB_ZFCore ZFTaskIdBasic : zfextend ZFObject, zfimplement ZFTaskId {
    ZFOBJECT_DECLARE(ZFTaskIdBasic, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFTaskId)
public:
    /** @brief for impl to implement #ZFTaskId::stop */
    virtual inline void stopImpl(ZF_IN const ZFListener &v) {
        _stopImpl = v;
    }
    /** @brief see #stopImpl */
    virtual inline const ZFListener &stopImpl(void) {
        return _stopImpl;
    }
public:
    zfoverride
    virtual void stop(void) {
        _stopImpl.execute(ZFArgs().sender(this));
    }
protected:
    zfoverride
    virtual inline void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief init with stopImpl */
    virtual inline void objectOnInit(ZF_IN const ZFListener &stopImpl) {
        this->stopImpl(stopImpl);
    }
private:
    ZFListener _stopImpl;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTaskId_h_

