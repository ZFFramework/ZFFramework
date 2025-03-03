#include "ZFHttpRequest.h"

#include "protocol/ZFProtocolZFHttpRequest.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFHttpMethod)

zfclassNotPOD _ZFP_ZFHttpRequestPrivate {
public:
    zfuint refCount;
    void *nativeTask;
    zfautoT<ZFThread> ownerThread;
    zfautoT<ZFHttpResponse> response;
    ZFListener callback;
    ZFJson *bodyJsonCache;
public:
    _ZFP_ZFHttpRequestPrivate(void)
    : refCount(2) // for ZFHttpRequest and ZFHttpResponse, one for each
    , nativeTask(zfnull)
    , ownerThread()
    , response(ZFHttpResponse::ClassData()->newInstance())
    , callback()
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
    void notifyResponse(ZF_IN ZFHttpRequest *owner, ZF_IN ZFResultType resultType) {
        ZFListener callbackTmp = this->callback;
        this->callback = zfnull;
        zfautoT<ZFHttpResponse> responseTmp = this->response;
        this->response = zfnull;
        zfautoT<ZFThread> ownerThreadTmp = this->ownerThread;
        this->ownerThread = zfnull;
        zfobj<v_ZFResultType> resultTypeTmp = resultType;

        if(this->bodyJsonCache != zfnull) {
            zfdelete(this->bodyJsonCache);
            this->bodyJsonCache = zfnull;
        }
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
    this->objectOnInit();
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
ZFMETHOD_DEFINE_1(ZFHttpRequest, void, body
        , ZFMP_IN(const ZFJson &, json)
        ) {
    zfstring text;
    ZFJsonToStringT(text, json, ZFJsonOutputTokenTrim());
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask, text.buffer(), text.length());
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfstring, body) {
    return ZFPROTOCOL_ACCESS(ZFHttpRequest)->body(d->nativeTask);
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFHttpRequest, void, request
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        ) {
    zfRetain(this); // release in notifyResponse

    if(ZFThread::implAvailable()) {
        d->ownerThread = ZFThread::currentThread();
    }
    d->callback = callback;
    this->observerNotify(zfself::E_OnRequestPrepare());
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->request(d->nativeTask);
    this->observerNotify(zfself::E_OnRequest());
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, void, requestCancel) {
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->requestCancel(d->nativeTask);
    d->notifyResponse(this, v_ZFResultType::e_Cancel);
}

ZFMETHOD_DEFINE_1(ZFHttpRequest, zfautoT<ZFHttpResponse>, requestSync
        , ZFMP_IN_OPT(zftimet, timeout, zftimetInvalid())
        ) {
    ZFHttpRequest *send = this;
    zfautoT<ZFHttpResponse> recv;
    zfobj<ZFSemaphore> waitLock;
    ZFLISTENER_3(onRequest
            , ZFHttpRequest *, send
            , ZFSemaphore *, waitLock
            , zfautoT<ZFHttpResponse> &, recv
            ) {
        ZFLISTENER_2(onResponse
                , ZFSemaphore *, waitLock
                , zfautoT<ZFHttpResponse> &, recv
                ) {
            recv = zfargs.param0();
            waitLock->lockAndBroadcast();
        } ZFLISTENER_END()
        send->request(onResponse);
    } ZFLISTENER_END()
    zfasync(onRequest);

    if(!waitLock->lockAndWait(timeout)) {
        this->requestCancel();
    }
    return recv;
}

ZFMETHOD_DEFINE_0(ZFHttpRequest, zfstring, headerInfo) {
    zfstring ret;
    ret.capacity(10 * this->headerCount());
    for(zfiter it = this->headerIter(); it; ++it) {
        zfstringAppend(ret, "%s: %s\n", this->headerIterKey(it), this->headerIterValue(it));
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFHttpRequest, zfstring, contentInfo) {
    zfstring ret;
    ret.capacity(this->body().length() + 10 * this->headerCount());
    for(zfiter it = this->headerIter(); it; ++it) {
        zfstringAppend(ret, "%s: %s\n", this->headerIterKey(it), this->headerIterValue(it));
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

void ZFHttpRequest::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    zfstringAppend(ret, " %s:%s", ZFHttpMethodToString(this->httpMethod()), this->url());
    zfstringAppend(ret, " header:%s", this->headerCount());
    zfstringAppend(ret, " body:%s", this->body().length());
}

void ZFHttpRequest::_ZFP_ZFHttpRequest_notifyResponse(void) {
    if(ZFThread::implAvailable()) {
        ZFHttpRequest *owner = this;
        ZFLISTENER_2(notifyResponse
                , zfautoT<ZFHttpRequest>, owner
                , _ZFP_ZFHttpRequestPrivate *, d
                ) {
            d->notifyResponse(owner, v_ZFResultType::e_Success);
        } ZFLISTENER_END()
        ZFThread::executeInThread(d->ownerThread, notifyResponse);
    }
    else {
        d->notifyResponse(this, v_ZFResultType::e_Success);
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
ZFMETHOD_DEFINE_0(ZFHttpResponse, ZFJson, bodyJson) {
    if(d->bodyJsonCache == zfnull) {
        d->bodyJsonCache = zfnew(ZFJson, ZFJsonFromString(this->body(), this->body().length()));
    }
    return *(d->bodyJsonCache);
}

ZFMETHOD_DEFINE_0(ZFHttpResponse, zfstring, headerInfo) {
    zfstring ret;
    ret.capacity(10 * this->headerCount());
    for(zfiter it = this->headerIter(); it; ++it) {
        zfstringAppend(ret, "%s: %s\n", this->headerIterKey(it), this->headerIterValue(it));
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFHttpResponse, zfstring, contentInfo) {
    zfstring ret;
    ret.capacity(this->body().length() + 10 * this->headerCount());
    for(zfiter it = this->headerIter(); it; ++it) {
        zfstringAppend(ret, "%s: %s\n", this->headerIterKey(it), this->headerIterValue(it));
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
    zfstring url;
    zftimet cacheTime;
    zfautoT<ZFHttpResponse> cache;
    _ZFP_ZFHttpHeadCache *prev;
    _ZFP_ZFHttpHeadCache *next;
public:
    _ZFP_ZFHttpHeadCache(void)
    : url()
    , cacheTime(ZFTime::currentTime())
    , cache()
    , prev(zfnull)
    , next(zfnull)
    {
    }
};
typedef zfstlmap<const zfchar *, _ZFP_ZFHttpHeadCache *, zfcharConst_zfstlLess> _ZFP_ZFHttpHeadCacheMapType;
static _ZFP_ZFHttpHeadCacheMapType _ZFP_ZFHttpHeadCacheMap; // <url, _ZFP_ZFHttpHeadCache>
static _ZFP_ZFHttpHeadCache *_ZFP_ZFHttpHeadCacheFirst = zfnull;
static _ZFP_ZFHttpHeadCache *_ZFP_ZFHttpHeadCacheLast = zfnull;

ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFHttpResponse>, ZFHttpHeadCache
        , ZFMP_IN(const zfchar *, url)
        ) {
    zftimet curTime = ZFTime::currentTime();
    {
        ZFCoreMutexLocker();
        _ZFP_ZFHttpHeadCacheMapType::iterator cacheIt = _ZFP_ZFHttpHeadCacheMap.find(url);
        if(cacheIt != _ZFP_ZFHttpHeadCacheMap.end()) {
            _ZFP_ZFHttpHeadCache *cache = cacheIt->second;
            if(cache->cacheTime + _ZFP_ZFHttpHeadCacheTime > curTime) {
                // cache valid, move to head
                if(cache != _ZFP_ZFHttpHeadCacheFirst) {
                    if(cache->next != zfnull) {
                        cache->next->prev = cache->prev;
                    }
                    if(cache->prev != zfnull) {
                        cache->prev->next = cache->next;
                    }
                    if(cache == _ZFP_ZFHttpHeadCacheLast && _ZFP_ZFHttpHeadCacheLast->prev != zfnull) {
                        _ZFP_ZFHttpHeadCacheLast = _ZFP_ZFHttpHeadCacheLast->prev;
                    }
                    cache->prev = zfnull;
                    cache->next = _ZFP_ZFHttpHeadCacheFirst;
                    _ZFP_ZFHttpHeadCacheFirst->prev = cache;
                }
                return cache->cache;
            }
            else {
                // cache invalid, remove
                if(cache->next != zfnull) {
                    cache->next->prev = cache->prev;
                }
                if(cache->prev != zfnull) {
                    cache->prev->next = cache->next;
                }
                if(cache == _ZFP_ZFHttpHeadCacheFirst) {
                    _ZFP_ZFHttpHeadCacheFirst = cache->next;
                }
                if(cache == _ZFP_ZFHttpHeadCacheLast) {
                    _ZFP_ZFHttpHeadCacheLast = cache->prev;
                }
                _ZFP_ZFHttpHeadCacheMap.erase(cacheIt);
                zfdelete(cache);
            }
        }
    }

    // no match, load
    zfautoT<ZFHttpResponse> recv = zfobj<ZFHttpRequest>(url, v_ZFHttpMethod::e_HEAD)->requestSync();
    if(recv == zfnull) {
        return recv;
    }

    ZFCoreMutexLocker();
    _ZFP_ZFHttpHeadCache *cache = zfnew(_ZFP_ZFHttpHeadCache);
    cache->url = url;
    cache->cache = recv;
    cache->next = _ZFP_ZFHttpHeadCacheFirst;
    if(_ZFP_ZFHttpHeadCacheFirst != zfnull) {
        _ZFP_ZFHttpHeadCacheFirst->prev = cache;
    }
    else {
        _ZFP_ZFHttpHeadCacheFirst = cache;
        _ZFP_ZFHttpHeadCacheLast = cache;
    }
    _ZFP_ZFHttpHeadCacheMap[cache->url] = cache;

    while(_ZFP_ZFHttpHeadCacheMap.size() > _ZFP_ZFHttpHeadCacheMax) {
        _ZFP_ZFHttpHeadCache *tmp = _ZFP_ZFHttpHeadCacheLast;
        _ZFP_ZFHttpHeadCacheLast = _ZFP_ZFHttpHeadCacheLast->prev;
        _ZFP_ZFHttpHeadCacheLast->next = zfnull;
        _ZFP_ZFHttpHeadCacheMap.erase(tmp->url);
        zfdelete(tmp);
    }

    return recv;
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFHttpHeadCacheClear) {
    ZFCoreMutexLocker();
    if(_ZFP_ZFHttpHeadCacheFirst != zfnull) {
        _ZFP_ZFHttpHeadCacheFirst = zfnull;
        _ZFP_ZFHttpHeadCacheLast = zfnull;
        _ZFP_ZFHttpHeadCacheMapType tmp;
        _ZFP_ZFHttpHeadCacheMap.swap(tmp);
    }
}

ZF_NAMESPACE_GLOBAL_END

