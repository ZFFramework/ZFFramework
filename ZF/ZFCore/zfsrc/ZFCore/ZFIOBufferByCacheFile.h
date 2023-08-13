/**
 * @file ZFIOBufferByCacheFile.h
 * @brief common impl for #ZFIOBuffer
 */

#ifndef _ZFI_ZFIOBufferByCacheFile_h_
#define _ZFI_ZFIOBufferByCacheFile_h_

#include "ZFIOBuffer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFIOBufferByCacheFile;
/** @brief #ZFIOBuffer using cache file */
zfclass ZFLIB_ZFCore ZFIOBufferByCacheFile : zfextend ZFIOBuffer {
    ZFOBJECT_DECLARE(ZFIOBufferByCacheFile, ZFIOBuffer)

protected:
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
    _ZFP_ZFIOBufferByCacheFile *d;
    ZFInput _ZFP_input;
    ZFInput _ZFP_output;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOBufferByCacheFile_h_

