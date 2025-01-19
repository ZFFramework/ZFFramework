#include "ZFWebSocket.h"
#include "protocol/ZFProtocolZFWebSocket.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFWebSocket)

ZFEVENT_REGISTER(ZFWebSocket, OnOpen)
ZFEVENT_REGISTER(ZFWebSocket, OnClose)
ZFEVENT_REGISTER(ZFWebSocket, OnRecv)
ZFEVENT_REGISTER(ZFWebSocket, OnRecvBin)

// ============================================================
zfclassNotPOD _ZFP_ZFWebSocketPrivate {
public:
    void *nativeWebSocket;
};

void ZFWebSocket::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFWebSocketPrivate);
    d->nativeWebSocket = zfnull;
}
void ZFWebSocket::objectOnDealloc(void) {
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFWebSocket::objectOnDeallocPrepare(void) {
    this->close();
    zfsuper::objectOnDeallocPrepare();
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFWebSocket, void, open
        , ZFMP_IN(const zfstring &, url)
        ) {
    this->close();
    d->nativeWebSocket = ZFPROTOCOL_ACCESS(ZFWebSocket)->open(this, url);
}
ZFMETHOD_DEFINE_0(ZFWebSocket, void, close) {
    if(d->nativeWebSocket) {
        ZFPROTOCOL_ACCESS(ZFWebSocket)->close(this, d->nativeWebSocket);
        d->nativeWebSocket = zfnull;
    }
}

ZFMETHOD_DEFINE_0(ZFWebSocket, zfbool, valid) {
    return d->nativeWebSocket;
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFWebSocket, void, send
        , ZFMP_IN(const zfstring &, data)
        ) {
    if(d->nativeWebSocket) {
        ZFPROTOCOL_ACCESS(ZFWebSocket)->send(this, d->nativeWebSocket, data.buffer(), data.length());
    }
}
ZFMETHOD_DEFINE_2(ZFWebSocket, void, send
        , ZFMP_IN(const void *, data)
        , ZFMP_IN(zfindex, size)
        ) {
    if(d->nativeWebSocket) {
        ZFPROTOCOL_ACCESS(ZFWebSocket)->send(this, d->nativeWebSocket, data, size);
    }
}
ZFMETHOD_DEFINE_1(ZFWebSocket, void, sendBin
        , ZFMP_IN(const zfstring &, data)
        ) {
    if(d->nativeWebSocket) {
        ZFPROTOCOL_ACCESS(ZFWebSocket)->sendBin(this, d->nativeWebSocket, data.buffer(), data.length());
    }
}
ZFMETHOD_DEFINE_2(ZFWebSocket, void, sendBin
        , ZFMP_IN(const void *, data)
        , ZFMP_IN(zfindex, size)
        ) {
    if(d->nativeWebSocket) {
        ZFPROTOCOL_ACCESS(ZFWebSocket)->sendBin(this, d->nativeWebSocket, data, size);
    }
}

ZF_NAMESPACE_GLOBAL_END

