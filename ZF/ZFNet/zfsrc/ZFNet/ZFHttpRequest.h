/**
 * @file ZFHttpRequest.h
 * @brief http operation
 */

#ifndef _ZFI_ZFHttpRequest_h_
#define _ZFI_ZFHttpRequest_h_

#include "ZFNetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief http method type
 */
ZFENUM_BEGIN(ZFLIB_ZFNet, ZFHttpMethod)
    ZFENUM_VALUE(GET) /**< @brief GET */
    ZFENUM_VALUE(HEAD) /**< @brief HEAD */
    ZFENUM_VALUE(POST) /**< @brief POST */
    ZFENUM_VALUE(PUT) /**< @brief PUT */
    ZFENUM_VALUE(DELETE) /**< @brief DELETE */
    ZFENUM_VALUE(CONNECT) /**< @brief CONNECT */
    ZFENUM_VALUE(OPTIONS) /**< @brief OPTIONS */
    ZFENUM_VALUE(TRACE) /**< @brief TRACE */
    ZFENUM_VALUE(PATCH) /**< @brief PATCH */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(GET)
    ZFENUM_VALUE_REGISTER(HEAD)
    ZFENUM_VALUE_REGISTER(POST)
    ZFENUM_VALUE_REGISTER(PUT)
    ZFENUM_VALUE_REGISTER(DELETE)
    ZFENUM_VALUE_REGISTER(CONNECT)
    ZFENUM_VALUE_REGISTER(OPTIONS)
    ZFENUM_VALUE_REGISTER(TRACE)
    ZFENUM_VALUE_REGISTER(PATCH)
ZFENUM_END(ZFLIB_ZFNet, ZFHttpMethod)

zfclassFwd _ZFP_ZFHttpRequestPrivate;
// ============================================================
/**
 * @brief see #ZFHttpRequest
 */
zfclass ZFLIB_ZFNet ZFHttpResponse : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFHttpResponse, ZFObject)
    ZFOBJECT_PRIVATE_ALLOC("can only be created by ZFHttpRequest")

    /** @brief whether the request is considered success */
    ZFPROPERTY_ASSIGN(zfbool, success)
    /** @brief http response code */
    ZFPROPERTY_ASSIGN(zfint, code)
    /** @brief error hint, if error occurred */
    ZFPROPERTY_ASSIGN(zfstring, errorHint)
    /** @brief response body content */
    ZFMETHOD_DECLARE_0(ZFBuffer &, body)

    // ============================================================
    /** @brief get the response header */
    ZFMETHOD_DECLARE_1(zfstring, header
            , ZFMP_IN(const zfchar *, key)
            )

    /** @brief response header count */
    ZFMETHOD_DECLARE_0(zfindex, headerCount)

    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_0(zfiterator, headerIter)
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfbool, headerIterValid
            , ZFMP_IN(const zfiterator &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(void, headerIterNext
            , ZFMP_IN_OUT(zfiterator &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterKey
            , ZFMP_IN(const zfiterator &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterValue
            , ZFMP_IN(const zfiterator &, it)
            )

    // ============================================================
    /** @brief util to access body as plain text */
    ZFMETHOD_DECLARE_0(const zfchar *, bodyText)
    /** @brief util to access body as json, return an invalid json if not able to parse */
    ZFMETHOD_DECLARE_0(ZFJson, bodyJson)

    /** @brief print all header, usually for debug use only */
    ZFMETHOD_DECLARE_0(zfstring, headerInfo)
    /** @brief print all content info, usually for debug use only */
    ZFMETHOD_DECLARE_0(zfstring, contentInfo)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

private:
    _ZFP_ZFHttpRequestPrivate *d;
    friend zfclassFwd ZFHttpRequest;
    friend zfclassFwd _ZFP_ZFHttpRequestPrivate;
};

// ============================================================
/**
 * @brief http request util
 *
 * typical usage:
 * @code
 *   ZFLISTENER(onRecv) {
 *       ZFJson response = zfargs.param0()->to<ZFHttpResponse *>()->bodyJson();
 *       zfLogTrim() << response;
 *   } ZFLISTENER_END()
 *   zflineAlloc(ZFHttpRequest, "http://xxx", "POST")
 *       ->header("Content-Type", "application/json;charset=UTF-8;")
 *       ->body(ZFJson()
 *           .attr("k", "v")
 *       )
 *       ->request(onRecv);
 * @endcode
 */
zfclass ZFLIB_ZFNet ZFHttpRequest : zfextend ZFStyleableObject {
    ZFOBJECT_DECLARE(ZFHttpRequest, ZFStyleableObject)

    /**
     * @brief called when any response received or timeout, before #EventOnResponse and impl callback
     *
     * sender is the #ZFHttpRequest,
     * param0 is the #ZFHttpResponse\n
     * call in the same thread that #request was called
     */
    ZFOBSERVER_EVENT(OnResponsePrepare)
    /**
     * @brief called when any response received or timeout
     *
     * sender is the #ZFHttpRequest,
     * param0 is the #ZFHttpResponse\n
     * call in the same thread that #request was called
     */
    ZFOBSERVER_EVENT(OnResponse)

    /**
     * @brief init and connect
     */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const zfchar *, url)
            , ZFMP_IN_OPT(ZFHttpMethodEnum, method, ZFHttpMethod::e_GET)
            )

    /** @brief timeout */
    ZFPROPERTY_ASSIGN(zftimet, timeout, 2000)

    /** @brief the request url */
    ZFPROPERTY_ASSIGN(zfstring, url)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, url)

    /** @brief the http method, GET/POST/..., GET by default */
    ZFPROPERTY_ASSIGN(ZFHttpMethodEnum, httpMethod, ZFHttpMethod::e_GET)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFHttpMethodEnum, httpMethod)

    /** @brief whether https impl available */
    ZFMETHOD_DECLARE_STATIC_0(zfbool, httpsAvailable)

    // ============================================================
    /**
     * @brief set http header
     *
     * note, null or empty value is also valid,
     * use #headerRemove or #headerIterRemove to remove
     */
    ZFMETHOD_DECLARE_2(ZFHttpRequest *, header
            , ZFMP_IN(const zfchar *, key)
            , ZFMP_IN(const zfchar *, value)
            )
    /** @brief remove http header */
    ZFMETHOD_DECLARE_1(ZFHttpRequest *, headerRemove
            , ZFMP_IN(const zfchar *, key)
            )
    /** @brief get http header */
    ZFMETHOD_DECLARE_1(zfstring, header
            , ZFMP_IN(const zfchar *, key)
            )

    /** @brief response header count */
    ZFMETHOD_DECLARE_0(zfindex, headerCount)

    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_0(zfiterator, headerIter)
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfbool, headerIterValid
            , ZFMP_IN(const zfiterator &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(void, headerIterNext
            , ZFMP_IN_OUT(zfiterator &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterKey
            , ZFMP_IN(const zfiterator &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterValue
            , ZFMP_IN(const zfiterator &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_2(void, headerIterValue
            , ZFMP_IN_OUT(zfiterator &, it)
            , ZFMP_IN(const zfchar *, value)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(void, headerIterRemove
            , ZFMP_IN_OUT(zfiterator &, it)
            )

    // ============================================================
    /** @brief append content to body */
    ZFMETHOD_DECLARE_2(ZFHttpRequest *, body
            , ZFMP_IN(const zfchar *, text)
            , ZFMP_IN_OPT(zfindex, count, zfindexMax())
            )
    /** @brief append content to body */
    ZFMETHOD_DECLARE_1(ZFHttpRequest *, body
            , ZFMP_IN(const ZFJson &, json)
            )
    /** @brief append content to body */
    ZFMETHOD_DECLARE_1(ZFHttpRequest *, body
            , ZFMP_IN(const ZFBuffer &, buf)
            )

    /** @brief return current body contents, note this may cause additional memorpy copy */
    ZFMETHOD_DECLARE_0(ZFBuffer, body)

    // ============================================================
    /**
     * @brief start the request
     *
     * callback would be called in the same thread that called this method,
     * sender is the owner #ZFHttpRequest,
     * param0 is the #ZFHttpResponse
     */
    ZFMETHOD_DECLARE_1(ZFHttpRequest *, request
            , ZFMP_IN_OPT(const ZFListener &, callback, ZFCallback())
            )

    /** @brief cancel the request */
    ZFMETHOD_DECLARE_0(void, requestCancel)

    /** @brief util to perform sync request */
    ZFMETHOD_DECLARE_0(zfautoT<ZFHttpResponse *>, requestSync)

    /** @brief print all header info, usually for debug use only */
    ZFMETHOD_DECLARE_0(zfstring, headerInfo)
    /** @brief print all content info, usually for debug use only */
    ZFMETHOD_DECLARE_0(zfstring, contentInfo)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

private:
    _ZFP_ZFHttpRequestPrivate *d;
public:
    void _ZFP_ZFHttpRequest_notifyResponse(void);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFHttpRequest_h_

