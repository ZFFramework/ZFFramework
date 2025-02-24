/**
 * @file ZFIOBufferByMemory.h
 * @brief common impl for #ZFIOBuffer
 */

#ifndef _ZFI_ZFIOBufferByMemory_h_
#define _ZFI_ZFIOBufferByMemory_h_

#include "ZFIOBuffer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFIOBufferByMemory;
/** @brief #ZFIOBuffer using memory */
zfclass ZFLIB_ZFCore ZFIOBufferByMemory : zfextend ZFIOBuffer {
    ZFOBJECT_DECLARE(ZFIOBufferByMemory, ZFIOBuffer)

public:
    zfoverride
    virtual ZFInput implInput(void) {
        return this->_ZFP_input;
    }
    zfoverride
    virtual ZFOutput implOutput(void) {
        return this->_ZFP_output;
    }
    zfoverride
    virtual void implRemoveAll(void);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFIOBufferByMemory *d;
    ZFInput _ZFP_input;
    ZFOutput _ZFP_output;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOBufferByMemory_h_

