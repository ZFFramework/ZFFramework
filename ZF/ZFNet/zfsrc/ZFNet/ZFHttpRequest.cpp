#include "ZFHttpRequest.h"

#include "protocol/ZFProtocolZFHttpRequest.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

// #define _ZFP_ZFHttpRequest_DEBUG 1

#if _ZFP_ZFHttpRequest_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFHttpRequest_log(fmt, ...) \
        zfimplLog("%s [ZFHttpRequest] %s %s" \
                , zfimplTime() \
                , ZFObjectInfo(ZFThread::currentThread()).cString() \
                , zfstr(fmt, ##__VA_ARGS__).cString() \
                )
#else
    #define _ZFP_ZFHttpRequest_log(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFHttpMethod)

zfclassNotPOD _ZFP_ZFHttpRequestPrivate {
public:
    zfuint refCount;
    void *nativeTask;
    zfautoT<ZFThread> ownerThread;
    zfautoT<ZFHttpResponse> response; // not null when running, null otherwise
    ZFListener callback;
public:
    _ZFP_ZFHttpRequestPrivate(void)
    : refCount(1)
    , nativeTask(zfnull)
    , ownerThread()
    , response()
    , callback()
    {
    }

public:
    void requestPrepare(void) {
        this->response = ZFHttpResponse::ClassData()->newInstance();
        this->response->d = this;
        ++(this->refCount); // release on ZFHttpResponse's objectOnDealloc
    }
    void notifyResponse(ZF_IN ZFHttpRequest *owner, ZF_IN ZFResultType resultType) {
        ZFListener callbackTmp = this->callback;
        this->callback = zfnull;
        zfautoT<ZFHttpResponse> responseTmp = this->response;
        this->response = zfnull;
        zfautoT<ZFThread> ownerThreadTmp = this->ownerThread;
        this->ownerThread = zfnull;
        zfobj<v_ZFResultType> resultTypeTmp = resultType;

        owner->observerNotify(ZFHttpRequest::E_OnResponsePrepare(), responseTmp, resultTypeTmp);
        if(callbackTmp) {
            callbackTmp.execute(ZFArgs()
                    .sender(owner)
                    .param0(responseTmp)
                    .param1(resultTypeTmp)
                );
        }
        owner->observerNotify(ZFHttpRequest::E_OnResponse(), responseTmp, resultTypeTmp);
        zfRelease(owner);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFHttpRequest)

ZFEVENT_REGISTER(ZFHttpRequest, OnRequestPrepare)
ZFEVENT_REGISTER(ZFHttpRequest, OnRequest)
ZFEVENT_REGISTER(ZFHttpRequest, OnResponsePrepare)
ZFEVENT_REGISTER(ZFHttpRequest, OnResponse)

ZFOBJECT_ON_INIT_DEFINE_2(ZFHttpRequest
        , ZFMP_IN(const zfstring &, url)
        , ZFMP_IN_OPT(ZFHttpMethod, method, v_ZFHttpMethod::e_GET)
        ) {
    this->url(url);
    this->httpMethod(method);
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFHttpRequest, zfstring, url) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->url(d->nativeTask, propertyValue);
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFHttpRequest, ZFHttpMethod, httpMethod) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->httpMethod(d->nativeTask, propertyValue);
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfbool, httpsAvailable) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->httpsAvailable();
}

// ============================================================
ZFMETHOD_DEFINE_2(ZFHttpRequest, void, header
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const zfstring &, value)
        ) {
    if(key) {
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->header(d->nativeTask, key, value);
    }
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, void, headerRemove
        , ZFMP_IN(const zfstring &, key)
        ) {
    if(key) {
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerRemove(d->nativeTask, key);
    }
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, zfstring, header
        , ZFMP_IN(const zfstring &, key)
        ) {
    if(key) {
        return ZFPROTOCOL_ACCESS(ZFHttpRequest)->header(d->nativeTask, key);
    }
    else {
        return zfnull;
    }
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfindex, headerCount) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerCount(d->nativeTask);
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfiter, headerIter) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIter(d->nativeTask);
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, zfstring, headerIterKey
        , ZFMP_IN(const zfiter &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterKey(d->nativeTask, it);
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, zfstring, headerIterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterValue(d->nativeTask, it);
}
ZFMETHOD_DEFINE_2(ZFHttpRequest, void, headerIterValue
        , ZFMP_IN_OUT(zfiter &, it)
        , ZFMP_IN(const zfstring &, value)
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterValue(d->nativeTask, it, value);
}
ZFMETHOD_DEFINE_1(ZFHttpRequest, void, headerIterRemove
        , ZFMP_IN_OUT(zfiter &, it)
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->headerIterRemove(d->nativeTask, it);
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFHttpRequest, void, body
        , ZFMP_IN(const zfstring &, buf)
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask, buf.buffer(), buf.length());
}
ZFMETHOD_DEFINE_2(ZFHttpRequest, void, body
        , ZFMP_IN(const void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask, buf, ((count == zfindexMax()) ? zfslen((const zfchar *)buf) : count));
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfstring, body) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask);
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFHttpRequest, void, request
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        ) {
    if(d->response) {
        return;
    }
    zfRetain(this); // release in notifyResponse

    if(ZFThread::implAvailable()) {
        d->ownerThread = ZFThread::currentThread();
    }
    d->callback = callback;
    d->requestPrepare();
    zftToString(this);
    _ZFP_ZFHttpRequest_log("request begin: %s %s", this, d->response);
    this->observerNotify(zfself::E_OnRequestPrepare());
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->request(d->nativeTask, d->response);
    this->observerNotify(zfself::E_OnRequest());
    _ZFP_ZFHttpRequest_log("request end: %s %s", this, d->response);
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, void, requestCancel) {
    if(!d->response) {
        _ZFP_ZFHttpRequest_log("requestCancel begin: %s", this);
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->requestCancel(d->nativeTask);
        d->notifyResponse(this, v_ZFResultType::e_Cancel);
        _ZFP_ZFHttpRequest_log("requestCancel end: %s", this);
    }
}

ZFMETHOD_DEFINE_1(ZFHttpRequest, zfautoT<ZFHttpResponse>, requestSync
        , ZFMP_IN_OPT(zftimet, timeout, zftimetInvalid())
        ) {
    if(d->response) {
        _ZFP_ZFHttpRequest_log("requestSync but already running: %s %s", this, d->response);
        return zfnull;
    }

    zfautoT<ZFHttpRequest> send = this;
    zfobj<ZFObjectHolder> recv;
    zfobj<ZFSemaphore> waitLock;
    ZFLISTENER_3(onRequest
            , zfautoT<ZFHttpRequest>, send
            , ZFSemaphore *, waitLock
            , zfautoT<ZFObjectHolder>, recv
            ) {
        ZFLISTENER_3(onResponse
                , ZFSemaphore *, waitLock
                , zfautoT<ZFHttpRequest>, send
                , zfautoT<ZFObjectHolder>, recv
                ) {
            recv->value(zfargs.param0());
            _ZFP_ZFHttpRequest_log("requestSync response begin: %s %s", send, zfargs.param0());
            waitLock->lockAndBroadcast();
            _ZFP_ZFHttpRequest_log("requestSync response end: %s", send);
        } ZFLISTENER_END()
        _ZFP_ZFHttpRequest_log("requestSync request begin: %s", send);
        send->request(onResponse);
        _ZFP_ZFHttpRequest_log("requestSync request end: %s", send);
    } ZFLISTENER_END()
    _ZFP_ZFHttpRequest_log("requestSync begin: %s", this);
    zfasync(onRequest);

    _ZFP_ZFHttpRequest_log("requestSync wait begin: %s", this);
    if(!waitLock->lockAndWait(timeout)) {
        this->requestCancel();
    }
    _ZFP_ZFHttpRequest_log("requestSync wait end: %s", this);
    _ZFP_ZFHttpRequest_log("requestSync end: %s", this);
    return recv->value();
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfstring, headerInfo) {
    zfstring ret;
    ret.capacity(10 * this->headerCount());
    for(zfiter it = this->headerIter(); it; ++it) {
        ret += this->headerIterKey(it);
        ret += ": ";
        ret += this->headerIterValue(it);
        ret += "\n";
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFHttpRequest, zfstring, contentInfo) {
    zfstring ret;
    ret.capacity(this->body().length() + 10 * this->headerCount());
    for(zfiter it = this->headerIter(); it; ++it) {
        ret += this->headerIterKey(it);
        ret += ": ";
        ret += this->headerIterValue(it);
        ret += "\n";
    }
    if(this->headerCount() > 0) {
        ret += "\n";
    }
    ret.append(this->body());
    return ret;
}

void ZFHttpRequest::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFHttpRequestPrivate);
    d->nativeTask = ZFPROTOCOL_ACCESS(ZFHttpRequest)->nativeTaskCreate(this);
}
void ZFHttpRequest::objectOnDealloc(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->nativeTaskDestroy(d->nativeTask);
        zfpoolDelete(d);
    }
    zfsuper::objectOnDealloc();
}

void ZFHttpRequest::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    zfstringAppend(ret, " %s:%s", ZFHttpMethodToString(this->httpMethod()), this->url());
    zfstringAppend(ret, " header:%s", this->headerCount());
    zfstringAppend(ret, " body:%s", this->body().length());
}

void ZFHttpRequest::_ZFP_ZFHttpRequest_notifyResponse(ZF_IN ZFHttpResponse *response) {
    if(d->response != response) {
        _ZFP_ZFHttpRequest_log("notifyResponse but task not exist: %s %s", this, response);
        return;
    }

    if(ZFThread::implAvailable()) {
        ZFHttpRequest *owner = this;
        ZFLISTENER_3(notifyResponse
                , zfautoT<ZFHttpRequest>, owner
                , zfautoT<ZFHttpResponse>, response
                , _ZFP_ZFHttpRequestPrivate *, d
                ) {
            if(d->response != response) {
                _ZFP_ZFHttpRequest_log("notifyResponse but task not exist: %s %s", owner, response);
                return;
            }
            _ZFP_ZFHttpRequest_log("notifyResponse begin: %s %s", owner, response);
            d->notifyResponse(owner, v_ZFResultType::e_Success);
            _ZFP_ZFHttpRequest_log("notifyResponse end: %s %s", owner, response);
        } ZFLISTENER_END()
        ZFThread::executeInThread(d->ownerThread, notifyResponse);
    }
    else {
        _ZFP_ZFHttpRequest_log("notifyResponse begin: %s %s", this, response);
        d->notifyResponse(this, v_ZFResultType::e_Success);
        _ZFP_ZFHttpRequest_log("notifyResponse end: %s %s", this, response);
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFHttpResponse)

// ============================================================
ZFMETHOD_DEFINE_1(ZFHttpResponse, zfstring, header
        , ZFMP_IN(const zfstring &, key)
        ) {
    if(key) {
        return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeader(d->nativeTask, key);
    }
    else {
        return zfnull;
    }
}

ZFMETHOD_DEFINE_0(ZFHttpResponse, zfindex, headerCount) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderCount(d->nativeTask);
}

ZFMETHOD_DEFINE_0(ZFHttpResponse, zfiter, headerIter) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderIter(d->nativeTask);
}
ZFMETHOD_DEFINE_1(ZFHttpResponse, zfstring, headerIterKey
        , ZFMP_IN(const zfiter &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderIterKey(d->nativeTask, it);
}
ZFMETHOD_DEFINE_1(ZFHttpResponse, zfstring, headerIterValue
        , ZFMP_IN(const zfiter &, it)
        ) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->responseHeaderIterValue(d->nativeTask, it);
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFHttpResponse, zfstring, headerInfo) {
    zfstring ret;
    ret.capacity(10 * this->headerCount());
    for(zfiter it = this->headerIter(); it; ++it) {
        ret += this->headerIterKey(it);
        ret += ": ";
        ret += this->headerIterValue(it);
        ret += "\n";
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFHttpResponse, zfstring, contentInfo) {
    zfstring ret;
    ret.capacity(this->body().length() + 10 * this->headerCount());
    for(zfiter it = this->headerIter(); it; ++it) {
        ret += this->headerIterKey(it);
        ret += ": ";
        ret += this->headerIterValue(it);
        ret += "\n";
    }
    if(this->headerCount() > 0) {
        ret += "\n";
    }
    ret.append(this->body());
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

void ZFHttpResponse::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    if(this->code() != 0) {
        zfstringAppend(ret, " code:%s", this->code());
    }
    if(!this->errorHint().isEmpty()) {
        zfstringAppend(ret, " error:%s", this->errorHint());
    }
    zfstringAppend(ret, " header:%s", this->headerCount());
    zfstringAppend(ret, " body:%s", this->body().length());
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFUrlEncode
        , ZFMP_IN(const zfchar *, src)
        ) {
    zfstring ret;
    ZFUrlEncodeT(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFUrlEncodeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    ZFCoreDataEncode(ret, src);
}

ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFUrlDecode
        , ZFMP_IN(const zfchar *, src)
        ) {
    zfstring ret;
    ZFUrlDecodeT(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFUrlDecodeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    ZFCoreDataDecode(ret, src);
}

ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFUrlForParam
        , ZFMP_IN(const zfchar *, url)
        , ZFMP_IN(const ZFJson &, params)
        ) {
    zfstring ret = url;
    ZFUrlParamSet(ret, params);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFUrlParamSet
        , ZFMP_IN_OUT(zfstring &, url)
        , ZFMP_IN(const ZFJson &, params)
        ) {
    for(zfiter it = params.attrIter(); it; ++it) {
        zfstring key = params.attrIterKey(it);
        ZFJson valueHolder = params.attrIterValue(it);
        if(valueHolder.type() != v_ZFJsonType::e_Value) {
            continue;
        }
        zfstring value = valueHolder.value();
        if(key == zfnull || value == zfnull) {
            continue;
        }
        ZFUrlParamSet(url, key, value);
    }
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFUrlParamSet
        , ZFMP_IN_OUT(zfstring &, url)
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN(const zfchar *, value)
        ) {
    zfindex p = zfstringFind(url, zfstr("?%s=", key));
    if(p == zfindexMax()) {
        p = zfstringFind(url, zfstr("&%s=", key));
        if(p == zfindexMax()) {
            url += (zfstringFind(url, '?') != zfindexMax() ? '&' : '?');
            url += key;
            url += '=';
            ZFUrlEncodeT(url, value);
            return;
        }
    }

    zfindex start = p + 1 + zfslen(key) + 1;
    zfindex end = zfstringFind(url + start, '&');
    if(end == zfindexMax()) {
        url.remove(start);
        ZFUrlEncodeT(url, value);
    }
    else {
        url.replace(start, end - start, ZFUrlEncode(value));
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfstring, ZFUrlParamGet
        , ZFMP_IN(const zfchar *, url)
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN_OPT(const zfchar *, def, zfnull)
        ) {
    if(zfstringIsEmpty(url)) {
        return def;
    }
    zfindex p = zfstringFind(url, zfstr("?%s=", key));
    if(p == zfindexMax()) {
        p = zfstringFind(url, zfstr("&%s=", key));
        if(p == zfindexMax()) {
            return def;
        }
    }
    zfindex keyLen = zfslen(key);
    zfindex start = p + 1 + keyLen + 1;
    zfindex end = zfstringFind(url + start, '&');
    if(end == zfindexMax()) {
        return (url + start);
    }
    else {
        return zfstring(url + start, end - start);
    }
}

// ============================================================
#define _ZFP_ZFHttpHeadCacheTime (5*60*1000)
#define _ZFP_ZFHttpHeadCacheMax (32)

zfclassNotPOD _ZFP_ZFHttpHeadCache {
public:
    zftimet cacheTime;
    zfautoT<ZFHttpResponse> cache;
public:
    _ZFP_ZFHttpHeadCache(void)
    : cacheTime(ZFTime::currentTime())
    , cache()
    {
    }
};
static ZFCoreOrderMap _ZFP_ZFHttpHeadCacheMap; // <url, _ZFP_ZFHttpHeadCache>

ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFHttpResponse>, ZFHttpHeadCache
        , ZFMP_IN(const zfstring &, url)
        ) {
    zftimet curTime = ZFTime::currentTime();
    {
        ZFCoreMutexLocker();
        zfiter it = _ZFP_ZFHttpHeadCacheMap.iterFind(url);
        if(it) {
            _ZFP_ZFHttpHeadCache *cache = _ZFP_ZFHttpHeadCacheMap.iterValue<_ZFP_ZFHttpHeadCache *>(it);
            if(cache->cacheTime + _ZFP_ZFHttpHeadCacheTime > curTime) {
                // cache valid, move to head
                _ZFP_ZFHttpHeadCacheMap.update(url);
                return cache->cache;
            }
            _ZFP_ZFHttpHeadCacheMap.iterRemove(it);
        }
    }

    // no match, load
    zfautoT<ZFHttpResponse> recv = zfobj<ZFHttpRequest>(url, v_ZFHttpMethod::e_HEAD)->requestSync();

    // save cache
    if(recv != zfnull) {
        ZFCoreMutexLocker();
        _ZFP_ZFHttpHeadCache *cache = zfpoolNew(_ZFP_ZFHttpHeadCache);
        cache->cacheTime = curTime;
        cache->cache = recv;
        _ZFP_ZFHttpHeadCacheMap.set(url, ZFCorePointerForPoolObject<_ZFP_ZFHttpHeadCache *>(cache));
    }

    return recv;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFHttpResponse>, ZFHttpHeadCacheRemove
        , ZFMP_IN(const zfstring &, url)
        ) {
    ZFCoreMutexLocker();
    zfiter it = _ZFP_ZFHttpHeadCacheMap.iterFind(url);
    if(!it) {
        return zfnull;
    }
    zfautoT<ZFHttpResponse> ret = _ZFP_ZFHttpHeadCacheMap.iterValue<_ZFP_ZFHttpHeadCache *>(it)->cache;
    _ZFP_ZFHttpHeadCacheMap.iterRemove(it);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFHttpHeadCacheRemoveAll) {
    ZFCoreMutexLocker();
    _ZFP_ZFHttpHeadCacheMap.removeAll();
}

ZF_NAMESPACE_GLOBAL_END

