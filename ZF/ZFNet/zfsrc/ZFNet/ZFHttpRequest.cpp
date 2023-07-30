#include "ZFHttpRequest.h"

#include "protocol/ZFProtocolZFHttpRequest.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFHttpMethod)

zfclassNotPOD _ZFP_ZFHttpRequestPrivate {
public:
    zfuint refCount;
    void *nativeTask;
    zfautoObjectT<ZFThread *> ownerThread;
    zfautoObjectT<ZFHttpResponse *> response;
    ZFListener callback;
    ZFBuffer body;
    ZFJson *bodyJsonCache;
public:
    _ZFP_ZFHttpRequestPrivate(void)
    : refCount(2)
    , nativeTask(zfnull)
    , ownerThread()
    , response(ZFHttpResponse::ClassData()->newInstance())
    , callback()
    , body()
    , bodyJsonCache(zfnull)
    {
        this->response->d = this;
    }
    ~_ZFP_ZFHttpRequestPrivate(void) {
        if(this->bodyJsonCache) {
            zfdelete(this->bodyJsonCache);
            this->bodyJsonCache = zfnull;
        }
    }

public:
    void notifyResponse(ZF_IN ZFHttpRequest *owner) {
        if(this->bodyJsonCache != zfnull) {
            zfdelete(this->bodyJsonCache);
            this->bodyJsonCache = zfnull;
        }
        owner->observerNotify(ZFHttpRequest::EventOnResponsePrepare(), this->response);
        if(this->callback) {
            this->callback.execute(ZFArgs()
                    .sender(owner)
                    .param0(this->response)
                    .userData(this->callback.userData())
                );
        }
        this->callback = ZFCallback();
        owner->observerNotify(ZFHttpRequest::EventOnResponse(), this->response);
        this->response = zfnull;
        this->ownerThread = zfnull;
        zfRelease(owner);
    }
    void notifyCancel(ZF_IN ZFHttpRequest *owner) {
        this->callback = ZFCallback();
        this->response = zfnull;
        this->ownerThread = zfnull;
        zfRelease(owner);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFHttpRequest)

ZFOBSERVER_EVENT_REGISTER(ZFHttpRequest, OnResponsePrepare)
ZFOBSERVER_EVENT_REGISTER(ZFHttpRequest, OnResponse)

ZFOBJECT_ON_INIT_DEFINE_2(ZFHttpRequest
        , ZFMP_IN(const zfchar *, url)
        , ZFMP_IN_OPT(ZFHttpMethodEnum, method, ZFHttpMethod::e_GET)
        ) {
    this->objectOnInit();
    this->url(url);
    this->httpMethod(method);
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFHttpRequest, zfstring, url) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->url(d->nativeTask, propertyValue);
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFHttpRequest, ZFHttpMethodEnum, httpMethod) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->httpMethod(d->nativeTask, propertyValue);
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfbool, httpsAvailable) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->httpsAvailable();
}

// ============================================================
ZFMETHOD_DEFINE_2(ZFHttpRequest, ZFHttpRequest *, header
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN(const zfchar *, value)
        ) {
    if(!zfstringIsEmpty(key)) {
        if(value == zfnull) {
            value = "";
        }
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->header(d->nativeTask, key, value);
    }
    return this;
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, ZFHttpRequest *, headerRemove
        , ZFMP_IN(const zfchar *, key)
        ) {
    if(!zfstringIsEmpty(key)) {
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerRemove(d->nativeTask, key);
    }
    return this;
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, zfstring, header
        , ZFMP_IN(const zfchar *, key)
        ) {
    if(zfstringIsEmpty(key)) {
        return zfnull;
    }
    else {
        return ZFPROTOCOL_ACCESS(ZFHttpRequest)->header(d->nativeTask, key);
    }
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfindex, headerCount) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerCount(d->nativeTask);
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfiterator, headerIter) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIter(d->nativeTask);
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, zfbool, headerIterValid
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterValid(d->nativeTask, it);
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, void, headerIterNext
        , ZFMP_IN_OUT(zfiterator &, it)
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterNext(d->nativeTask, it);
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, zfstring, headerIterKey
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterKey(d->nativeTask, it);
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, zfstring, headerIterValue
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterValue(d->nativeTask, it);
}
ZFMETHOD_DEFINE_2(ZFHttpRequest, void, headerIterValue
        , ZFMP_IN_OUT(zfiterator &, it)
        , ZFMP_IN(const zfchar *, value)
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterValue(d->nativeTask, it, value);
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, void, headerIterRemove
        , ZFMP_IN_OUT(zfiterator &, it)
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterRemove(d->nativeTask, it);
}

// ============================================================
ZFMETHOD_DEFINE_2(ZFHttpRequest, ZFHttpRequest *, body
        , ZFMP_IN(const zfchar *, text)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask,
        (const void *)text,
        ((count == zfindexMax()) ? zfslen(text) : count) * sizeof(zfchar));
    return this;
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, ZFHttpRequest *, body
        , ZFMP_IN(const ZFJson &, json)
        ) {
    zfstring text;
    ZFJsonToString(text, json, ZFJsonOutputFlagsTrim());
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask, (const void *)text.cString(), text.length());
    return this;
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, ZFHttpRequest *, body
        , ZFMP_IN(const ZFBuffer &, buf)
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask, buf.buffer(), buf.bufferSize());
    return this;
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, ZFBuffer, body) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask);
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFHttpRequest, ZFHttpRequest *, request
        , ZFMP_IN_OPT(const ZFListener &, callback, ZFCallback())
        ) {
    zfRetain(this); // release in notifyResponse

    if(ZFThread::implAvailable()) {
        d->ownerThread = ZFThread::currentThread();
    }
    d->callback = callback;
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->request(d->nativeTask);

    return this;
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, void, requestCancel) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->requestCancel(d->nativeTask);
    d->notifyCancel(this);
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfautoObjectT<ZFHttpResponse *>, requestSync) {
    ZFHttpRequest *send = this;
    zfautoObjectT<ZFHttpResponse *> recv;
    zfblockedAlloc(ZFSemaphore, waitLock);
    ZFLISTENER_3(onRequest
            , ZFHttpRequest *, send
            , ZFSemaphore *, waitLock
            , zfautoObjectT<ZFHttpResponse *> &, recv
            ) {
        ZFLISTENER_2(onResponse
                , ZFSemaphore *, waitLock
                , zfautoObjectT<ZFHttpResponse *> &, recv
                ) {
            recv = zfargs.param0();
            waitLock->lockAndBroadcast();
        } ZFLISTENER_END()
        send->request(onResponse);
    } ZFLISTENER_END()
    zfasync(onRequest);

    waitLock->lockAndWait();
    return recv;
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfstring, headerInfo) {
    zfstring ret;
    ret.capacity(10 * this->headerCount());
    for(zfiterator it = this->headerIter(); this->headerIterValid(it); this->headerIterNext(it)) {
        zfstringAppend(ret, "%s: %s\n", this->headerIterKey(it).cString(), this->headerIterValue(it).cString());
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFHttpRequest, zfstring, contentInfo) {
    zfstring ret;
    ret.capacity(this->body().textLength() + 10 * this->headerCount());
    for(zfiterator it = this->headerIter(); this->headerIterValid(it); this->headerIterNext(it)) {
        zfstringAppend(ret, "%s: %s\n", this->headerIterKey(it).cString(), this->headerIterValue(it).cString());
    }
    if(this->headerCount() > 0) {
        ret += "\n";
    }
    ret.append(this->body().text(), this->body().textLength());
    return ret;
}

void ZFHttpRequest::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFHttpRequestPrivate);
    d->nativeTask = ZFPROTOCOL_ACCESS(ZFHttpRequest)->nativeTaskCreate(this, d->response);
}
void ZFHttpRequest::objectOnDealloc(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->nativeTaskDestroy(d->nativeTask);
        zfpoolDelete(d);
    }
    zfsuper::objectOnDealloc();
}

void ZFHttpRequest::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    zfstringAppend(ret, " %s:%s", ZFHttpMethodToString(this->httpMethod()).cString(), this->url().cString());
    zfstringAppend(ret, " header:%zi", this->headerCount());
    zfstringAppend(ret, " body:%zi", this->body().bufferSize());
}

void ZFHttpRequest::_ZFP_ZFHttpRequest_notifyResponse(void) {
    if(ZFThread::implAvailable()) {
        ZFHttpRequest *owner = this;
        ZFLISTENER_2(notifyResponse
                , zfautoObjectT<ZFHttpRequest *>, owner
                , _ZFP_ZFHttpRequestPrivate *, d
                ) {
            d->notifyResponse(owner);
        } ZFLISTENER_END()
        ZFThread::executeInThread(d->ownerThread, notifyResponse);
    }
    else {
        d->notifyResponse(this);
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFHttpResponse)

// ============================================================
ZFMETHOD_DEFINE_0(ZFHttpResponse, ZFBuffer &, body) {
    return d->body;
}

ZFMETHOD_DEFINE_1(ZFHttpResponse, zfstring, header
        , ZFMP_IN(const zfchar *, key)
        ) {
    if(zfstringIsEmpty(key)) {
        return zfnull;
    }
    else {
        return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeader(d->nativeTask, key);
    }
}

ZFMETHOD_DEFINE_0(ZFHttpResponse, zfindex, headerCount) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderCount(d->nativeTask);
}

ZFMETHOD_DEFINE_0(ZFHttpResponse, zfiterator, headerIter) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderIter(d->nativeTask);
}
ZFMETHOD_DEFINE_1(ZFHttpResponse, zfbool, headerIterValid
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderIterValid(d->nativeTask, it);
}
ZFMETHOD_DEFINE_1(ZFHttpResponse, void, headerIterNext
        , ZFMP_IN_OUT(zfiterator &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderIterNext(d->nativeTask, it);
}
ZFMETHOD_DEFINE_1(ZFHttpResponse, zfstring, headerIterKey
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderIterKey(d->nativeTask, it);
}
ZFMETHOD_DEFINE_1(ZFHttpResponse, zfstring, headerIterValue
        , ZFMP_IN(const zfiterator &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderIterValue(d->nativeTask, it);
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFHttpResponse, const zfchar *, bodyText) {
    if(this->body().bufferSize() <= 0) {
        return "";
    }
    else {
        this->body().text()[this->body().textLength()] = '\0';
        return this->body().text();
    }
}
ZFMETHOD_DEFINE_0(ZFHttpResponse, ZFJson, bodyJson) {
    if(d->bodyJsonCache == zfnull) {
        d->bodyJsonCache = zfnew(ZFJson, ZFJsonFromString(this->body().text(), this->body().textLength()));
    }
    return *(d->bodyJsonCache);
}

ZFMETHOD_DEFINE_0(ZFHttpResponse, zfstring, headerInfo) {
    zfstring ret;
    ret.capacity(10 * this->headerCount());
    for(zfiterator it = this->headerIter(); this->headerIterValid(it); this->headerIterNext(it)) {
        zfstringAppend(ret, "%s: %s\n", this->headerIterKey(it).cString(), this->headerIterValue(it).cString());
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFHttpResponse, zfstring, contentInfo) {
    zfstring ret;
    ret.capacity(this->body().textLength() + 10 * this->headerCount());
    for(zfiterator it = this->headerIter(); this->headerIterValid(it); this->headerIterNext(it)) {
        zfstringAppend(ret, "%s: %s\n", this->headerIterKey(it).cString(), this->headerIterValue(it).cString());
    }
    if(this->headerCount() > 0) {
        ret += "\n";
    }
    ret.append(this->body().text(), this->body().textLength());
    return ret;
}

void ZFHttpResponse::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfnull;
}
void ZFHttpResponse::objectOnDealloc(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->nativeTaskDestroy(d->nativeTask);
        zfpoolDelete(d);
    }
    zfsuper::objectOnDealloc();
}

void ZFHttpResponse::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    if(this->code() != 0) {
        zfstringAppend(ret, " code:%d", this->code());
    }
    if(!this->errorHint().isEmpty()) {
        zfstringAppend(ret, " error:%s", this->errorHint().cString());
    }
    zfstringAppend(ret, " header:%zi", this->headerCount());
    zfstringAppend(ret, " body:%zi", this->body().bufferSize());
}

ZF_NAMESPACE_GLOBAL_END

