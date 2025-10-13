/**
 * @file ZFProtocolZFHttpRequest.h
 * @brief protocol for #ZFHttpRequest
 */

#ifndef _ZFI_ZFProtocolZFHttpRequest_h_
#define _ZFI_ZFProtocolZFHttpRequest_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFHttpRequest.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for #ZFHttpRequest
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFNet, ZFHttpRequest)
    // ============================================================
    // for request
public:
    /**
     * @brief whether https supported
     */
    virtual zfbool httpsAvailable(void) zfpurevirtual;

    /**
     * @brief create native task
     */
    virtual void *nativeTaskCreate(ZF_IN ZFHttpRequest *request) zfpurevirtual;
    /**
     * @brief destroy native task
     */
    virtual void nativeTaskDestroy(ZF_IN void *nativeTask) zfpurevirtual;

    /**
     * @brief change the request url
     */
    virtual void url(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &url
            ) zfpurevirtual;

    /**
     * @brief set http method
     */
    virtual void httpMethod(
            ZF_IN void *nativeTask
            , ZF_IN ZFHttpMethod httpMethod
            ) zfpurevirtual;

    /**
     * @brief set http header
     */
    virtual void header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            , ZF_IN const zfstring &value
            ) zfpurevirtual;

    /**
     * @brief remove http header
     */
    virtual void headerRemove(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) zfpurevirtual;

    /**
     * @brief get http header
     */
    virtual zfstring header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) zfpurevirtual;

    /**
     * @brief http header access
     */
    virtual zfindex headerCount(ZF_IN void *nativeTask) zfpurevirtual;

    /**
     * @brief http header access
     */
    virtual zfiter headerIter(ZF_IN void *nativeTask) zfpurevirtual;
    /**
     * @brief http header access
     */
    virtual zfstring headerIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) zfpurevirtual;
    /**
     * @brief http header access
     */
    virtual zfstring headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) zfpurevirtual;
    /**
     * @brief http header access
     */
    virtual void headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            , ZF_IN const zfstring &value
            ) zfpurevirtual;
    /**
     * @brief http header access
     */
    virtual void headerIterRemove(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            ) zfpurevirtual;

    /**
     * @brief append bytes to body
     */
    virtual void body(
            ZF_IN void *nativeTask
            , ZF_IN const void *buffer
            , ZF_IN zfindex byteSize
            ) zfpurevirtual;

    /**
     * @brief return the current body contents
     */
    virtual zfstring body(ZF_IN void *nativeTask) zfpurevirtual;

    /**
     * @brief start the http request, impl must notify #notifyResponse even if fail or error
     */
    virtual void request(ZF_IN void *nativeTask, ZF_IN ZFHttpResponse *response) zfpurevirtual;

    /**
     * @brief cancel the request
     */
    virtual void requestCancel(ZF_IN void *nativeTask) zfpurevirtual;

    // ============================================================
    // for response
public:
    /**
     * @brief implementation must notify when receive response
     *
     * before notify, you must fill these:
     * -  #ZFHttpResponse::success
     * -  #ZFHttpResponse::code
     * -  #ZFHttpResponse::errorHint
     * -  #ZFHttpResponse::body
     */
    zffinal void notifyResponse(ZF_IN ZFHttpRequest *request, ZF_IN ZFHttpResponse *response) {
        request->_ZFP_ZFHttpRequest_notifyResponse(response);
    }

    /**
     * @brief get response header
     */
    virtual zfstring responseHeader(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) zfpurevirtual;

    /**
     * @brief response header access
     */
    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask) zfpurevirtual;

    /**
     * @brief response header access
     */
    virtual zfiter responseHeaderIter(ZF_IN void *nativeTask) zfpurevirtual;
    /**
     * @brief response header access
     */
    virtual zfstring responseHeaderIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) zfpurevirtual;
    /**
     * @brief response header access
     */
    virtual zfstring responseHeaderIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFHttpRequest)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFHttpRequest_h_

