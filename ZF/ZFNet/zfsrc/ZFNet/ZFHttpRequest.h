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
    /**
     * @brief response body
     *
     * note, you must ensure life cycle of the #ZFHttpResponse
     * longer than the returned string object
     */
    ZFPROPERTY_ASSIGN(zfstring, body)

    // ============================================================
    /** @brief get the response header */
    ZFMETHOD_DECLARE_1(zfstring, header
            , ZFMP_IN(const zfstring &, key)
            )

    /** @brief response header count */
    ZFMETHOD_DECLARE_0(zfindex, headerCount)

    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_0(zfiter, headerIter)
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterKey
            , ZFMP_IN(const zfiter &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterValue
            , ZFMP_IN(const zfiter &, it)
            )

    // ============================================================
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
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

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
 *       ZFHttpResponse *response = zfargs.param0();
 *       ZFLogTrim() << response->bodyJson();
 *   } ZFLISTENER_END()
 *   zfobj<ZFHttpRequest>("http://xxx", "POST")
 *       ->header("Content-Type", "application/json;charset=UTF-8;")
 *       ->body(ZFJson()
 *           .attr("k", "v")
 *       )
 *       ->request(onRecv);
 * @endcode
 */
zfclass ZFLIB_ZFNet ZFHttpRequest : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFHttpRequest, ZFStyle)

    /**
     * @brief called before request
     *
     * sender is the #ZFHttpRequest\n
     * call in the same thread that #request was called
     */
    ZFEVENT(OnRequestPrepare)
    /**
     * @brief called after request
     *
     * sender is the #ZFHttpRequest\n
     * call in the same thread that #request was called
     */
    ZFEVENT(OnRequest)

    /**
     * @brief called when any response received or timeout, before #E_OnResponse and impl callback
     *
     * sender is the #ZFHttpRequest,
     * param0 is the #ZFHttpResponse or null if timeout,
     * param1 is a #ZFResultType to indicate result type\n
     * call in the same thread that #request was called
     */
    ZFEVENT(OnResponsePrepare)
    /**
     * @brief called when any response received or timeout
     *
     * sender is the #ZFHttpRequest,
     * param0 is the #ZFHttpResponse or null if timeout,
     * param1 is a #ZFResultType to indicate result type\n
     * call in the same thread that #request was called
     */
    ZFEVENT(OnResponse)

    /**
     * @brief init and connect
     */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const zfstring &, url)
            , ZFMP_IN_OPT(ZFHttpMethod, method, ZFHttpMethod::e_GET)
            )

    /** @brief timeout */
    ZFPROPERTY_ASSIGN(zftimet, timeout, 2000)

    /** @brief the request url */
    ZFPROPERTY_ASSIGN(zfstring, url)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, url)

    /** @brief the http method, GET/POST/..., GET by default */
    ZFPROPERTY_ASSIGN(ZFHttpMethod, httpMethod, ZFHttpMethod::e_GET)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFHttpMethod, httpMethod)

    /** @brief whether https impl available */
    ZFMETHOD_DECLARE_STATIC_0(zfbool, httpsAvailable)

    // ============================================================
    /**
     * @brief set http header
     *
     * note, null or empty value is also valid,
     * use #headerRemove or #headerIterRemove to remove
     */
    ZFMETHOD_DECLARE_2(void, header
            , ZFMP_IN(const zfstring &, key)
            , ZFMP_IN(const zfstring &, value)
            )
    /** @brief remove http header */
    ZFMETHOD_DECLARE_1(void, headerRemove
            , ZFMP_IN(const zfstring &, key)
            )
    /** @brief get http header */
    ZFMETHOD_DECLARE_1(zfstring, header
            , ZFMP_IN(const zfstring &, key)
            )

    /** @brief response header count */
    ZFMETHOD_DECLARE_0(zfindex, headerCount)

    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_0(zfiter, headerIter)
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterKey
            , ZFMP_IN(const zfiter &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(zfstring, headerIterValue
            , ZFMP_IN(const zfiter &, it)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_2(void, headerIterValue
            , ZFMP_IN_OUT(zfiter &, it)
            , ZFMP_IN(const zfstring &, value)
            )
    /** @brief iterator for #header */
    ZFMETHOD_DECLARE_1(void, headerIterRemove
            , ZFMP_IN_OUT(zfiter &, it)
            )

    // ============================================================
    /** @brief append content to body */
    ZFMETHOD_DECLARE_1(void, body
            , ZFMP_IN(const zfstring &, buf)
            )
    /** @brief append content to body */
    ZFMETHOD_DECLARE_2(void, body
            , ZFMP_IN(const void *, buf)
            , ZFMP_IN(zfindex, count)
            )
    /** @brief append content to body */
    ZFMETHOD_DECLARE_1(void, body
            , ZFMP_IN(const ZFJson &, json)
            )

    /**
     * @brief return current body contents
     *
     * may cause additional memory copy, usually for debug use only
     */
    ZFMETHOD_DECLARE_0(zfstring, body)

    // ============================================================
    /**
     * @brief start the request
     *
     * callback would be called in the same thread that called this method,
     * sender is the owner #ZFHttpRequest,
     * param0 is the #ZFHttpResponse or null if timeout,
     * param1 is a #ZFResultType to indicate result type
     */
    ZFMETHOD_DECLARE_1(void, request
            , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
            )

    /** @brief cancel the request */
    ZFMETHOD_DECLARE_0(void, requestCancel)

    /** @brief util to perform sync request, return null if timeout */
    ZFMETHOD_DECLARE_1(zfautoT<ZFHttpResponse>, requestSync
            , ZFMP_IN_OPT(zftimet, timeout, zftimetInvalid())
            )

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
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

private:
    _ZFP_ZFHttpRequestPrivate *d;
public:
    void _ZFP_ZFHttpRequest_notifyResponse(void);
};

// ============================================================
/** @brief url encode util */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFNet, zfstring, ZFUrlEncode
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief url encode util */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFNet, void, ZFUrlEncodeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )

/** @brief url decode util */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFNet, zfstring, ZFUrlDecode
        , ZFMP_IN(const zfchar *, src)
        )
/** @brief url decode util */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFNet, void, ZFUrlDecodeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        )

/** @brief util to generate url */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFNet, zfstring, ZFUrlForParam
        , ZFMP_IN(const zfchar *, url)
        , ZFMP_IN(const ZFJson &, params)
        )
/** @brief util to modify url */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFNet, void, ZFUrlParamSet
        , ZFMP_IN_OUT(zfstring &, url)
        , ZFMP_IN(const ZFJson &, params)
        )
/** @brief util to modify url */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFNet, void, ZFUrlParamSet
        , ZFMP_IN_OUT(zfstring &, url)
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN(const zfchar *, value)
        )
/** @brief util to get param from url */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFNet, zfstring, ZFUrlParamGet
        , ZFMP_IN(const zfchar *, url)
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN_OPT(const zfchar *, def, zfnull)
        )

// ============================================================
/**
 * @brief util to fetch head info, or load from cache if available
 *
 * use #ZFHttpHeadCacheClear if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFNet, zfautoT<ZFHttpResponse>, ZFHttpHeadCache
        , ZFMP_IN(const zfchar *, url)
        )
/**
 * @brief see #ZFHttpHeadCache
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFNet, void, ZFHttpHeadCacheClear)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFHttpRequest_h_

