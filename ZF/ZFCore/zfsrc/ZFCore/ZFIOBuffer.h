/**
 * @file ZFIOBuffer.h
 * @brief util to hold and connect #ZFInput and #ZFOutput
 */

#ifndef _ZFI_ZFIOBuffer_h_
#define _ZFI_ZFIOBuffer_h_

#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief default value for #ZFIOBuffer::bufferSize,
 *   256KB by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFCore, zfindex, ZFIOBufferSize)

zfclassFwd _ZFP_ZFIOBufferPrivate;
/**
 * @brief util to hold and connect #ZFInput and #ZFOutput
 *
 * usage:
 * @code
 *   zfobj<ZFIOBuffer> io;
 *   outputFunc(io->output()); // output data to io's internal buffer
 *
 *   io->input().ioSeek(0); // set position to 0, ready to read
 *   inputFunc(io->input()); // input data from io's internal buffer
 * @endcode
 *
 * ZFIOBuffer would use memory buffer if contents less than #bufferSize,
 * and change to cache file otherwise\n
 * \n
 * once created, it's ensured safe to store input or output
 * but release the owner ZFIOBuffer's reference
 */
zfclass ZFLIB_ZFCore ZFIOBuffer : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFIOBuffer, ZFObject)

public:
    /**
     * @brief change to use #cacheFilePath when contents exceeds this size,
     *   #ZFIOBufferSize by default
     *
     * set to 0 would change to cache file immediately\n
     * set to zfindexMax would always use memory buffer\n
     * \n
     * do nothing if already using cache file
     */
    ZFPROPERTY_ASSIGN(zfindex, bufferSize, ZFIOBufferSize())
    ZFPROPERTY_ON_ATTACH_DECLARE(zfindex, bufferSize)
    /**
     * @brief change the cache file to use
     *
     * by default, random cache file would be used\n
     * \n
     * when already using cache file, modifying this value would have no effect
     */
    ZFMETHOD_DECLARE_2(void, cacheFilePath
            , ZFMP_IN(const ZFPathInfo &, value)
            , ZFMP_IN_OPT(zfbool, autoRemove, zftrue)
            )
    /** @brief see #cacheFilePath */
    ZFMETHOD_DECLARE_0(ZFPathInfo, cacheFilePath)
    /** @brief see #cacheFilePath */
    ZFMETHOD_DECLARE_0(zfbool, cacheFileAutoRemove)

    /**
     * @brief get current path info
     *
     * return:
     * -  a #ZFPathType_text path info when using memory buffer
     * -  #cacheFilePath when using cache file
     *
     * note:
     * -  the path info points to a temporary memory buffer or cache file,
     *   and should not be used for serialization
     * -  the path info may change from memory buffer type to cache file type
     *   while internal buffer growing
     * -  may return a large path info with #ZFPathType_text
     *   when currently using memory buffer,
     *   recommended to set bufferSize to 0
     *   to force cache file type
     */
    ZFMETHOD_DECLARE_0(ZFPathInfo, pathInfo)

public:
    /**
     * @brief get input callback
     */
    ZFMETHOD_DECLARE_0(ZFInput, input)
    /**
     * @brief get output callback
     */
    ZFMETHOD_DECLARE_0(ZFOutput, output)
    /**
     * @brief remove all of contents
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
private:
    _ZFP_ZFIOBufferPrivate *d;
    ZFInput _ZFP_input;
    ZFOutput _ZFP_output;
};

// ============================================================
/**
 * @brief util to wrap input with io buffer
 *
 * how it works:
 * -# return a wrapped input
 * -# when first time reading or checking from the wrapped input,
 *   sync refInput to internal cache
 *
 * the convCallback's sender would be the #ZFIOBuffer,
 * param0 would be a #v_ZFInput to the refInput
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFInput, ZFIOBufferInput
        , ZFMP_IN(const ZFInput &, refInput)
        , ZFMP_IN_OPT(const ZFListener &, convCallback, zfnull)
        )
/**
 * @brief util to wrap output with io buffer
 *
 * how it works:
 * -# return a wrapped output
 * -# write to the wrapped output
 * -# when done (wrapped output destroyed),
 *   sync to refOutput (optionally by convCallback to convert contents)
 *
 * the convCallback's sender would be the #ZFIOBuffer,
 * param0 would be a #v_ZFOutput to the refOutput
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFOutput, ZFIOBufferOutput
        , ZFMP_IN(const ZFOutput &, refOutput)
        , ZFMP_IN_OPT(const ZFListener &, convCallback, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOBuffer_h_

