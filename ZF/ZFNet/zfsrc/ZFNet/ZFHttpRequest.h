/**
 * @file ZFHttpRequest.h
 * @brief http operation
 */

#ifndef _ZFI_ZFHttpRequest_h_
#define _ZFI_ZFHttpRequest_h_

#include "ZFNetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFHttpRequestPrivate;

// ============================================================
/**
 * @brief see #ZFHttpRequest
 */
zfclass ZFLIB_ZFNet ZFHttpResponse : zfextends ZFObject
{
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
    ZFMETHOD_DECLARE_1(zfstring, header,
                       ZFMP_IN(const zfchar *, key))

    /** @brief response header count */
    ZFMETHOD_DECLARE_0(zfindex, headerCount)

    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_0(zfiterator, headerIter)
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfbool, headerIterValid,
                       ZFMP_IN(const zfiterator &, it))
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(void, headerIterNext,
                       ZFMP_IN_OUT(zfiterator &, it))
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterKey,
                       ZFMP_IN(const zfiterator &, it))
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterValue,
                       ZFMP_IN(const zfiterator &, it))

    // ============================================================
    /** @brief util to access body as plain text */
    ZFMETHOD_DECLARE_0(const zfchar *, bodyText)
    /** @brief util to access body as json, return an invalid json if not able to parse */
    ZFMETHOD_DECLARE_0(ZFJsonItem, bodyJson)

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
 *       ZFJsonItem response = listenerData.param0<ZFHttpResponse *>()->bodyJson();
 *       zfLogTrimT() << response;
 *   } ZFLISTENER_END(onRecv)
 *   zflineAlloc(ZFHttpRequest, "http://xxx", "POST")
 *       ->header("Content-Type", "application/json;charset=UTF-8;")
 *       ->request(ZFJsonItem()
 *           .attr("k", "v")
 *           , onRecv)
 *       ;
 * @endcode
 */
zfclass ZFLIB_ZFNet ZFHttpRequest : zfextends ZFStyleableObject
{
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
    ZFOBJECT_ON_INIT_DECLARE_2(ZFMP_IN(const zfchar *, url),
                               ZFMP_IN_OPT(const zfchar *, method, "GET"))

    /** @brief timeout */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, timeout, 2000)

    /** @brief the request url */
    ZFPROPERTY_ASSIGN(zfstring, url)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, url)

    /** @brief the http method, GET/POST/..., GET by default */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfstring, httpMethod, "GET")
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, httpMethod)

    // ============================================================
    /** @brief set http header */
    ZFMETHOD_DECLARE_2(ZFHttpRequest *, header,
                       ZFMP_IN(const zfchar *, key),
                       ZFMP_IN(const zfchar *, value))
    /** @brief get http header */
    ZFMETHOD_DECLARE_1(zfstring, header,
                       ZFMP_IN(const zfchar *, key))

    /** @brief response header count */
    ZFMETHOD_DECLARE_0(zfindex, headerCount)

    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_0(zfiterator, headerIter)
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfbool, headerIterValid,
                       ZFMP_IN(const zfiterator &, it))
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(void, headerIterNext,
                       ZFMP_IN_OUT(zfiterator &, it))
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterKey,
                       ZFMP_IN(const zfiterator &, it))
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterValue,
                       ZFMP_IN(const zfiterator &, it))
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_2(void, headerIterValue,
                       ZFMP_IN_OUT(zfiterator &, it),
                       ZFMP_IN(const zfchar *, value))
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(void, headerIterRemove,
                       ZFMP_IN_OUT(zfiterator &, it))

    // ============================================================
    /** @brief append content to body */
    ZFMETHOD_DECLARE_2(ZFHttpRequest *, body,
                       ZFMP_IN(const zfchar *, text),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
    /** @brief append content to body */
    ZFMETHOD_DECLARE_1(ZFHttpRequest *, body,
                       ZFMP_IN(const ZFJsonItem &, json))
    /** @brief append content to body */
    ZFMETHOD_DECLARE_1(ZFHttpRequest *, body,
                       ZFMP_IN(const ZFBuffer &, buf))

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
    ZFMETHOD_DECLARE_2(ZFHttpRequest *, request,
                       ZFMP_IN_OPT(const ZFListener &, callback, ZFCallback()),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))

    /** @brief cancel the request */
    ZFMETHOD_DECLARE_0(void, requestCancel)

    /** @brief util to perform sync request */
    ZFMETHOD_DECLARE_0(zfautoObjectT<ZFHttpResponse *>, requestSync)

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

