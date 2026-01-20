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
    virtual zfstring recvBody(void) zfpurevirtual;

public:
    /** @brief recv header count */
    virtual zfindex recvHeaderCount(void) zfpurevirtual;
    /** @brief recv header access */
    virtual zfstring recvHeader(ZF_IN const zfstring &key) zfpurevirtual;

    /** @brief recv header access */
    virtual zfiter recvHeaderIter(void) zfpurevirtual;
    /** @brief recv header access */
    virtual zfstring recvHeaderIterKey(ZF_IN const zfiter &it) zfpurevirtual;
    /** @brief recv header access */
    virtual zfstring recvHeaderIterValue(ZF_IN const zfiter &it) zfpurevirtual;

public:
    /** @brief recv header count */
    virtual zfindex recvParamCount(void) zfpurevirtual;
    /** @brief recv header access */
    virtual zfstring recvParam(ZF_IN const zfstring &key) zfpurevirtual;

    /** @brief recv header access */
    virtual zfiter recvParamIter(void) zfpurevirtual;
    /** @brief recv header access */
    virtual zfstring recvParamIterKey(ZF_IN const zfiter &it) zfpurevirtual;
    /** @brief recv header access */
    virtual zfstring recvParamIterValue(ZF_IN const zfiter &it) zfpurevirtual;

public:
    /** @brief recv http method */
    virtual ZFHttpMethod recvMethod(void) zfpurevirtual;
    /** @brief recv url path */
    virtual zfstring recvPath(void) zfpurevirtual;
    /** @brief the addr of the client */
    virtual zfstring recvClientAddr(void) zfpurevirtual;
    /** @brief the addr of the client */
    virtual zfuint recvClientPort(void) zfpurevirtual;
    /** @brief the addr of the server */
    virtual zfstring recvServerAddr(void) zfpurevirtual;
    /** @brief the addr of the server */
    virtual zfuint recvServerPort(void) zfpurevirtual;

    // ============================================================
    // resp
public:
    /** @brief resp status code */
    virtual void respCode(ZF_IN zfuint code) zfpurevirtual;

    /** @brief resp body */
    virtual void respBody(ZF_IN const zfstring &value, ZF_IN const zfstring &contentType) zfpurevirtual;
    /** @brief resp body */
    virtual void respBody(ZF_IN const ZFInput &value, ZF_IN const zfstring &contentType) zfpurevirtual;

public:
    /** @brief resp header count */
    virtual zfindex respHeaderCount(void) zfpurevirtual;
    /** @brief resp header access */
    virtual zfstring respHeader(ZF_IN const zfstring &key) zfpurevirtual;

    /** @brief resp header access */
    virtual zfiter respHeaderIter(void) zfpurevirtual;
    /** @brief resp header access */
    virtual zfstring respHeaderIterKey(ZF_IN const zfiter &it) zfpurevirtual;
    /** @brief resp header access */
    virtual zfstring respHeaderIterValue(ZF_IN const zfiter &it) zfpurevirtual;

    /** @brief resp header modify */
    virtual void respHeader(ZF_IN const zfstring &key, ZF_IN const zfstring &value) zfpurevirtual;
    /** @brief resp header modify */
    virtual void respHeaderRemove(ZF_IN const zfstring &key) zfpurevirtual;
    /** @brief resp header modify */
    virtual zfiter respHeaderIterFind(ZF_IN const zfstring &key) zfpurevirtual;
    /** @brief resp header modify */
    virtual void respHeaderIterValue(ZF_IN const zfiter &it, ZF_IN const zfstring &value) zfpurevirtual;
    /** @brief resp header modify */
    virtual void respHeaderIterRemove(ZF_IN const zfiter &it) zfpurevirtual;

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

