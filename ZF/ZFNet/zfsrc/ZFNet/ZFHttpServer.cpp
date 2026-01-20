#include "ZFHttpServer.h"

#include "protocol/ZFProtocolZFHttpServer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFHttpServerTask)

ZFMETHOD_DEFINE_0(ZFHttpServerTask, zfstring, headerInfo) {
    zfstring ret;
    for(zfiter it = this->recvHeaderIter(); it; ++it) {
        ret += this->recvHeaderIterKey(it);
        ret += ": ";
        ret += this->recvHeaderIterValue(it);
        ret += "\n";
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFHttpServerTask, zfstring, contentInfo) {
    zfstring ret;
    for(zfiter it = this->recvHeaderIter(); it; ++it) {
        ret += this->recvHeaderIterKey(it);
        ret += ": ";
        ret += this->recvHeaderIterValue(it);
        ret += "\n";
    }
    if(this->recvHeaderCount() > 0) {
        ret += "\n";
    }
    ret += this->recvBody();
    return ret;
}

void ZFHttpServerTask::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    ret += ZFTOKEN_ZFObjectInfoLeft;

    ret += this->recvClientAddr();
    ret += ":";
    zfuintToStringT(ret, this->recvClientPort());
    ret += " ";

    ZFHttpMethodToStringT(ret, this->recvMethod());
    ret += " ";
    ret += this->recvPath();
    {
        zfbool first = zftrue;
        for(zfiter it = this->recvParamIter(); it; ++it) {
            if(first) {
                first = zffalse;
                ret += "?";
            }
            else {
                ret += "&";
            }
            ret += this->recvParamIterKey(it);
            ret += "=";
            ZFUrlEncodeT(ret, this->recvParamIterValue(it));
        }
    }

    ret += " header:";
    zfindexToStringT(ret, this->recvHeaderCount());

    ret += " body:";
    zfindexToStringT(ret, this->recvBody().length());

    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfstring, recvBody)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfindex, recvHeaderCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, recvHeader
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfiter, recvHeaderIter)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, recvHeaderIterKey
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, recvHeaderIterValue
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfindex, recvParamCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, recvParam
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfiter, recvParamIter)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, recvParamIterKey
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, recvParamIterValue
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, ZFHttpMethod, recvMethod)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfstring, recvPath)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfstring, recvClientAddr)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfuint, recvClientPort)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfstring, recvServerAddr)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfuint, recvServerPort)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, void, respCode
        , ZFMP_IN(zfuint, code)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFHttpServerTask, void, respBody
        , ZFMP_IN(const zfstring &, value)
        , ZFMP_IN(const zfstring &, contentType)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFHttpServerTask, void, respBody
        , ZFMP_IN(const ZFInput &, value)
        , ZFMP_IN(const zfstring &, contentType)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfindex, respHeaderCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, respHeader
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFHttpServerTask, zfiter, respHeaderIter)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, respHeaderIterKey
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfstring, respHeaderIterValue
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFHttpServerTask, void, respHeader
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, void, respHeaderRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, zfiter, respHeaderIterFind
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFHttpServerTask, void, respHeaderIterValue
        , ZFMP_IN(const zfiter &, it)
        , ZFMP_IN(const zfstring &, value)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFHttpServerTask, void, respHeaderIterRemove
        , ZFMP_IN(const zfiter &, it)
        )

// ============================================================
ZFOBJECT_REGISTER(ZFHttpServer)

ZFEVENT_REGISTER(ZFHttpServer, OnRequest)
ZFEVENT_REGISTER(ZFHttpServer, OnStart)
ZFEVENT_REGISTER(ZFHttpServer, OnStop)
ZFEVENT_REGISTER(ZFHttpServer, OnError)

ZFOBJECT_ON_INIT_DEFINE_1(ZFHttpServer
        , ZFMP_IN(zfuint, port)
        ) {
    this->port(port);
}
ZFOBJECT_ON_INIT_DEFINE_2(ZFHttpServer
        , ZFMP_IN(const zfstring &, addr)
        , ZFMP_IN(zfuint, port)
        ) {
    this->addr(addr);
    this->port(port);
}

ZFMETHOD_DEFINE_0(ZFHttpServer, void, start) {
    if(d == zfnull) {
        d = ZFPROTOCOL_ACCESS(ZFHttpServer)->start(this);
        if(d) {
            zfRetain(this);
        }
        this->observerNotify(zfself::E_OnStart());
    }
}
ZFMETHOD_DEFINE_0(ZFHttpServer, void, stop) {
    if(d) {
        void *t = d;
        d = zfnull;
        ZFPROTOCOL_ACCESS(ZFHttpServer)->stop(this, t);
        this->observerNotify(zfself::E_OnStop());
        zfRelease(this);
    }
}
ZFMETHOD_DEFINE_0(ZFHttpServer, zfbool, started) {
    return d;
}

ZFMETHOD_DEFINE_1(ZFHttpServer, void, onRequest
        , ZFMP_IN(const ZFListener &, impl)
        ) {
    this->observerAdd(zfself::E_OnRequest(), impl);
}

void ZFHttpServer::_ZFP_ZFHttpServer_onError(ZF_IN const zfstring &errorHint) {
    if(d) {
        void *t = d;
        d = zfnull;
        ZFPROTOCOL_ACCESS(ZFHttpServer)->stop(this, t);
        this->observerNotify(zfself::E_OnError(), zfobj<v_zfstring>(errorHint));
        zfRelease(this);
    }
}

void ZFHttpServer::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfnull;
}
void ZFHttpServer::objectOnDeallocPrepare(void) {
    this->stop();
    zfsuper::objectOnDeallocPrepare();
}
void ZFHttpServer::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    zfstringAppend(ret, "%s%s:%s%s"
            , ZFTOKEN_ZFObjectInfoLeft
            , this->addr()
            , this->port()
            , ZFTOKEN_ZFObjectInfoRight
            );
}

// ============================================================
ZFOBJECT_REGISTER(ZFHttpServerThreadPool)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFHttpServerThreadPool, instance, ZFLevelZFFrameworkEssential)

ZF_NAMESPACE_GLOBAL_END

