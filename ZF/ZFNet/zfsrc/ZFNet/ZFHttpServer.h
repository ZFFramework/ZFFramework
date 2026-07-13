/**
 * @file ZFHttpServer.h
 * @brief http server
 */

#ifndef _ZFI_ZFHttpServer_h_
#define _ZFI_ZFHttpServer_h_

#include "ZFHttpRequest.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFHttpServer */
zfabstract ZFLIB_ZFNet ZFHttpServerTask : zfextend ZFObject {
    ZFOBJECT_DECLARE_ABSTRACT(ZFHttpServerTask, ZFObject)

    // ============================================================
    // recv
public:
    /**
     * @brief recv body
     */
    ZFMETHOD_INTERFACE_0(zfstring, recvBody)

public:
    /** @brief recv header count */
    ZFMETHOD_INTERFACE_0(zfindex, recvHeaderCount)
    /** @brief recv header access */
    ZFMETHOD_INTERFACE_1(zfstring, recvHeader
            , ZFMP_IN(const zfstring &, key)
            )

    /** @brief recv header access */
    ZFMETHOD_INTERFACE_0(zfiter, recvHeaderIter)
    /** @brief recv header access */
    ZFMETHOD_INTERFACE_1(zfstring, recvHeaderIterKey
            , ZFMP_IN(const zfiter &, it)
            )
    /** @brief recv header access */
    ZFMETHOD_INTERFACE_1(zfstring, recvHeaderIterValue
            , ZFMP_IN(const zfiter &, it)
            )

public:
    /** @brief recv header count */
    ZFMETHOD_INTERFACE_0(zfindex, recvParamCount)
    /** @brief recv header access */
    ZFMETHOD_INTERFACE_1(zfstring, recvParam
            , ZFMP_IN(const zfstring &, key)
            )

    /** @brief recv header access */
    ZFMETHOD_INTERFACE_0(zfiter, recvParamIter)
    /** @brief recv header access */
    ZFMETHOD_INTERFACE_1(zfstring, recvParamIterKey
            , ZFMP_IN(const zfiter &, it)
            )
    /** @brief recv header access */
    ZFMETHOD_INTERFACE_1(zfstring, recvParamIterValue
            , ZFMP_IN(const zfiter &, it)
            )

public:
    /** @brief recv http method */
    ZFMETHOD_INTERFACE_0(ZFHttpMethod, recvMethod)
    /** @brief recv url path */
    ZFMETHOD_INTERFACE_0(zfstring, recvPath)
    /** @brief the addr of the client */
    ZFMETHOD_INTERFACE_0(zfstring, recvClientAddr)
    /** @brief the addr of the client */
    ZFMETHOD_INTERFACE_0(zfuint, recvClientPort)
    /** @brief the addr of the server */
    ZFMETHOD_INTERFACE_0(zfstring, recvServerAddr)
    /** @brief the addr of the server */
    ZFMETHOD_INTERFACE_0(zfuint, recvServerPort)

    // ============================================================
    // resp
public:
    /** @brief resp status code */
    ZFMETHOD_INTERFACE_1(void, respCode
            , ZFMP_IN(zfuint, code)
            )

    /** @brief resp body */
    ZFMETHOD_INTERFACE_2(void, respBody
            , ZFMP_IN(const zfstring &, value)
            , ZFMP_IN(const zfstring &, contentType)
            )
    /** @brief resp body */
    ZFMETHOD_INTERFACE_2(void, respBody
            , ZFMP_IN(const ZFInput &, value)
            , ZFMP_IN(const zfstring &, contentType)
            )

public:
    /** @brief resp header count */
    ZFMETHOD_INTERFACE_0(zfindex, respHeaderCount)
    /** @brief resp header access */
    ZFMETHOD_INTERFACE_1(zfstring, respHeader
            , ZFMP_IN(const zfstring &, key)
            )

    /** @brief resp header access */
    ZFMETHOD_INTERFACE_0(zfiter, respHeaderIter)
    /** @brief resp header access */
    ZFMETHOD_INTERFACE_1(zfstring, respHeaderIterKey
            , ZFMP_IN(const zfiter &, it)
            )
    /** @brief resp header access */
    ZFMETHOD_INTERFACE_1(zfstring, respHeaderIterValue
            , ZFMP_IN(const zfiter &, it)
            )

    /** @brief resp header modify */
    ZFMETHOD_INTERFACE_2(void, respHeader
            , ZFMP_IN(const zfstring &, key)
            , ZFMP_IN(const zfstring &, value)
            )
    /** @brief resp header modify */
    ZFMETHOD_INTERFACE_1(void, respHeaderRemove
            , ZFMP_IN(const zfstring &, key)
            )
    /** @brief resp header modify */
    ZFMETHOD_INTERFACE_1(zfiter, respHeaderIterFind
            , ZFMP_IN(const zfstring &, key)
            )
    /** @brief resp header modify */
    ZFMETHOD_INTERFACE_2(void, respHeaderIterValue
            , ZFMP_IN_OUT(zfiter &, it)
            , ZFMP_IN(const zfstring &, value)
            )
    /** @brief resp header modify */
    ZFMETHOD_INTERFACE_1(void, respHeaderIterRemove
            , ZFMP_IN_OUT(zfiter &, it)
            )

    // ============================================================
    // util
public:
    /** @brief print all header, usually for debug use only */
    ZFMETHOD_DECLARE_0(zfstring, headerInfo)
    /** @brief print all content info, usually for debug use only */
    ZFMETHOD_DECLARE_0(zfstring, contentInfo)

protected:
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret);
};

// ============================================================
/**
 * @brief http server util
 *
 * typical usage:
 * @code
 *   ZFLISTENER(onRequest) {
 *       ZFHttpServerTask *task = zfargs.param0();
 *   } ZFLISTENER_END()
 *   zfobj<ZFHttpServer>("localhost", 8080)
 *       ->onRequest(onRequest)
 *       ->start();
 * @endcode
 */
zfclass ZFLIB_ZFNet ZFHttpServer : zfextend ZFObject, zfimplement ZFTaskId {
    ZFOBJECT_DECLARE(ZFHttpServer, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFTaskId)

    /**
     * @brief called when receive from client
     *
     * sender is the #ZFHttpServer,
     * param0 is a #ZFHttpServerTask
     */
    ZFEVENT(OnRequest)

    /**
     * @brief called when server actually started
     */
    ZFEVENT(OnStart)
    /**
     * @brief called when server stopped
     */
    ZFEVENT(OnStop)
    /**
     * @brief called when server start failed or error occurred
     *
     * param0 is a #v_zfstring holds error hint\n
     * note, it depends on impl for this event would be called in which thread
     */
    ZFEVENT(OnError)

public:
    /**
     * @brief init with port
     */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(zfuint, port)
            )
    /**
     * @brief init with addr and port
     */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const zfstring &, addr)
            , ZFMP_IN(zfuint, port)
            )

    /** @brief the addr of the server, typically "0.0.0.0" */
    ZFPROPERTY_ASSIGN(zfstring, addr, zftext("0.0.0.0"))

    /** @brief the port of the server */
    ZFPROPERTY_ASSIGN(zfuint, port, 80)

    /** @brief timeout */
    ZFPROPERTY_ASSIGN(zftimet, timeout, 2000)

    /**
     * @brief start the server
     *
     * the server would be retained until #stop
     */
    ZFMETHOD_DECLARE_0(void, start)

    /**
     * @brief stop the server
     */
    ZFMETHOD_DECLARE_0(void, stop)

    /**
     * @brief whether the server started
     */
    ZFMETHOD_DECLARE_0(zfbool, started)

    /**
     * @brief util for impl
     */
    ZFMETHOD_DECLARE_1(void, onRequest
            , ZFMP_IN(const ZFListener &, impl)
            )

public:
    zffinal void _ZFP_ZFHttpServer_onError(ZF_IN const zfstring &errorHint);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
protected:
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret);

private:
    void *d;
};

/**
 * @brief thread pool for #ZFHttpServer
 */
zfclass ZFLIB_ZFNet ZFHttpServerThreadPool : zfextend ZFThreadPool {
    ZFOBJECT_DECLARE(ZFHttpServerThreadPool, ZFThreadPool)
    ZFOBJECT_SINGLETON_DECLARE(ZFHttpServerThreadPool, instance)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFHttpServer_h_

