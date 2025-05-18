#include "ZFImpl_default_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

#include "ZFImpl/ZFImpl_env.h"

#if defined(__cplusplus) && (__cplusplus >= 201103L) && !ZF_ENV_sys_Android && !ZF_ENV_sys_Qt

#if ZF_ENV_HTTPS
    #define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#define CPPHTTPLIB_NO_EXCEPTIONS
#include "../../../zf3rd/_repo/cpp-httplib/httplib.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFHttpRequestImpl_default, ZFHttpRequest, v_ZFProtocolLevel::e_Default)
private:
    zfclassNotPOD NativeTask {
    public:
        #if ZF_ENV_HTTPS
            // SSLClient seems not work
            // typedef httplib::SSLClient Client;
            typedef httplib::Client Client;
        #else
            typedef httplib::Client Client;
        #endif

    public:
        zfuint refCount;
        ZFHttpRequest *ownerRequest;
        zfautoT<ZFHttpResponse> ownerResponse;
        zfidentity taskId;
        zfstring url;
        ZFHttpMethod httpMethod;
        httplib::Headers headers;
        zfstlhashmap<zfstring, zfstring> headersCache;
        zfstring body;
        httplib::Headers responseHeaders;
        zfstlhashmap<zfstring, zfstring> responseHeadersCache;

    private:
        void _headersCacheUpdate(ZF_OUT zfstlhashmap<zfstring, zfstring> &dst, ZF_IN const httplib::Headers &src) {
            if(dst.empty() && !src.empty()) {
                for(auto srcIt = src.begin(); srcIt != src.end(); ++srcIt) {
                    zfstring key = srcIt->first.c_str();
                    auto dstIt = dst.find(key);
                    if(dstIt == dst.end()) {
                        dst.insert(zfstlpair<zfstring, zfstring>(key, srcIt->second.c_str()));
                    }
                    else {
                        dstIt->second += ", ";
                        dstIt->second += srcIt->second.c_str();
                    }
                }
            }
        }
    public:
        void headersCacheUpdate(void) {
            _headersCacheUpdate(headersCache, headers);
        }
        void responseHeadersCacheUpdate(void) {
            _headersCacheUpdate(responseHeadersCache, responseHeaders);
        }

    public:
        NativeTask(ZF_IN ZFHttpRequest *request)
        : refCount(1)
        , ownerRequest(request)
        , ownerResponse()
        , taskId(zfidentityInvalid())
        , url()
        , httpMethod(v_ZFHttpMethod::e_GET)
        , headers()
        , headersCache()
        , body()
        , responseHeaders()
        , responseHeadersCache()
        {
        }
    };

public:
#if !ZF_ENV_sys_Windows
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        signal(SIGPIPE, SIG_IGN);
    }
#endif

    // ============================================================
    // for request
public:
    virtual zfbool httpsAvailable(void) {
        #if ZF_ENV_HTTPS
            return zftrue;
        #else
            return zffalse;
        #endif
    }

    virtual void *nativeTaskCreate(ZF_IN ZFHttpRequest *request) {
        return zfnew(NativeTask, request);
    }
    virtual void nativeTaskDestroy(ZF_IN void *nativeTask) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        --(nativeTaskTmp->refCount);
        if(nativeTaskTmp->refCount == 0) {
            zfdelete(nativeTaskTmp);
        }
    }

    virtual void url(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &url
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->url = url;
    }

    virtual void httpMethod(
            ZF_IN void *nativeTask
            , ZF_IN ZFHttpMethod httpMethod
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->httpMethod = httpMethod;
    }

    virtual void header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            , ZF_IN const zfstring &value
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        std::string keyTmp = key.cString();
        nativeTaskTmp->headers.erase(keyTmp);
        nativeTaskTmp->headers.insert(std::pair<std::string, std::string>(keyTmp, value.cString()));
        nativeTaskTmp->headersCache.clear();
    }

    virtual void headerRemove(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->headers.erase(key.cString());
        nativeTaskTmp->headersCache.clear();
    }

    virtual zfstring header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        if(nativeTaskTmp->headersCache.empty()) {
            auto range = nativeTaskTmp->headers.equal_range(key.cString());
            zfstring ret;
            for(auto i = range.first; i != range.second; ++i) {
                if(!ret.isEmpty()) {
                    ret += ", ";
                }
                ret += i->second.c_str();
            }
            return ret;
        }
        else {
            auto it = nativeTaskTmp->headersCache.find(key);
            if(it != nativeTaskTmp->headersCache.end()) {
                return it->second;
            }
            else {
                return zfnull;
            }
        }
    }

    virtual zfindex headerCount(ZF_IN void *nativeTask) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->headersCacheUpdate();
        return (zfindex)nativeTaskTmp->headersCache.size();
    }

    zfclassNotPOD _Iter : zfextend zfiter::Impl {
    public:
        zfstlhashmap<zfstring, zfstring>::iterator it;
        zfstlhashmap<zfstring, zfstring>::iterator end;
    public:
        zfoverride
        virtual zfbool valid(void) {
            return it != end;
        }
        zfoverride
        virtual void next(void) {
            ++it;
        }
        zfoverride
        virtual Impl *copy(void) {
            _Iter *ret = zfpoolNew(_Iter);
            ret->it = it;
            ret->end = end;
            return ret;
        }
        zfoverride
        virtual void destroy(void) {
            zfpoolDelete(this);
        }
        zfoverride
        virtual zfbool isEqual(ZF_IN Impl *d) {
            _Iter *t = (_Iter *)d;
            return it == t->it;
        }
    };
    virtual zfiter headerIter(ZF_IN void *nativeTask) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        _Iter *impl = zfpoolNew(_Iter);
        nativeTaskTmp->headersCacheUpdate();
        impl->it = nativeTaskTmp->headersCache.begin();
        impl->end = nativeTaskTmp->headersCache.end();
        return zfiter(impl);
    }
    virtual zfstring headerIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return it.impl<_Iter *>()->it->first;
    }
    virtual zfstring headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return it.impl<_Iter *>()->it->second;
    }
    virtual void headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            , ZF_IN const zfstring &value
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        _Iter *impl = it.impl<_Iter *>();
        std::string key = impl->it->first.cString();
        nativeTaskTmp->headers.erase(key);
        nativeTaskTmp->headers.insert(std::pair<std::string, std::string>(key, value.cString()));
        impl->it->second = value;
    }
    virtual void headerIterRemove(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        _Iter *impl = it.impl<_Iter *>();
        nativeTaskTmp->headers.erase(impl->it->first.cString());
        nativeTaskTmp->headersCache.erase((impl->it)++);
    }

    virtual void body(
            ZF_IN void *nativeTask
            , ZF_IN const void *buffer
            , ZF_IN zfindex byteSize
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->body.append((const zfchar *)buffer, byteSize);
    }

    virtual zfstring body(ZF_IN void *nativeTask) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        return nativeTaskTmp->body;
    }

    virtual void request(ZF_IN void *nativeTask, ZF_IN ZFHttpResponse *response) {
        this->requestCancel(nativeTask);

        NativeTask *task = (NativeTask *)nativeTask;
        ++(task->refCount);
        task->ownerResponse = response;
        zfidentity taskId = this->taskIdGen.idAcquire();
        task->taskId = taskId;
        ZFLISTENER_2(asyncRequest
                , NativeTask *, task
                , zfidentity, taskId
                ) {
            zfstring url = task->url;
            zfindex redirectCount = 0;
            while(zftrue) {
                if(taskId != task->taskId) {
                    return;
                }
                if(redirectCount >= 10) {
                    task->ownerResponse->success(zffalse);
                    task->ownerResponse->code(-1);
                    task->ownerResponse->errorHint("exceed redirect count");
                    if(taskId == task->taskId) {
                        ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(task->ownerRequest, task->ownerResponse);
                    }
                    break;
                }
                #if !ZF_ENV_HTTPS
                if(zfstringFind(url, "https://") == 0) {
                    task->ownerResponse->success(zffalse);
                    task->ownerResponse->code(-1);
                    task->ownerResponse->errorHint("https not supported");
                    if(taskId == task->taskId) {
                        ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(task->ownerRequest, task->ownerResponse);
                    }
                    break;
                }
                #endif

                zfindex pHostPath = _ZFP_urlParse(url);
                httplib::Result result = _ZFP_asyncRequest(
                    task->httpMethod,
                    url,
                    pHostPath,
                    task->ownerRequest->timeout(),
                    task->headers,
                    task->body.buffer(),
                    task->body.length());
                if(result && result.value().status >= 300 && result.value().status < 400) {
                    std::string redirectUrl = result.value().get_header_value("Location");
                    if(!redirectUrl.empty()) {
                        url = redirectUrl.c_str();
                        ++redirectCount;
                        continue;
                    }
                }

                if(taskId == task->taskId) {
                    _ZFP_parseResponse(task, result);
                }
                if(taskId == task->taskId) {
                    ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(task->ownerRequest, task->ownerResponse);
                }
                break;
            }
        } ZFLISTENER_END()
        ZFLISTENER_3(asyncRequestOnFinish
                , ZFIdentityGenerator, taskIdGen
                , NativeTask *, task
                , zfidentity, taskId
                ) {
            if(taskId == task->taskId) {
                taskIdGen.idRelease(task->taskId);
                task->taskId = zfidentityInvalid();
            }
            --(task->refCount);
            if(task->refCount == 0) {
                zfdelete(task);
            }
        } ZFLISTENER_END()
        zfasync(asyncRequest, asyncRequestOnFinish);
    }

    virtual void requestCancel(ZF_IN void *nativeTask) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        if(nativeTaskTmp->taskId != zfidentityInvalid()) {
            this->taskIdGen.idRelease(nativeTaskTmp->taskId);
            nativeTaskTmp->taskId = zfidentityInvalid();
        }
    }

    // ============================================================
    // for response
public:
    virtual zfstring responseHeader(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        auto range = nativeTaskTmp->responseHeaders.equal_range(key.cString());
        zfstring ret;
        for(auto i = range.first; i != range.second; ++i) {
            if(!ret.isEmpty()) {
                ret += ", ";
            }
            ret += i->second.c_str();
        }
        return ret;
    }

    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->responseHeadersCacheUpdate();
        return (zfindex)nativeTaskTmp->responseHeadersCache.size();
    }

    virtual zfiter responseHeaderIter(ZF_IN void *nativeTask) {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->responseHeadersCacheUpdate();
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = nativeTaskTmp->responseHeadersCache.begin();
        impl->end = nativeTaskTmp->responseHeadersCache.end();
        return zfiter(impl);
    }
    virtual zfstring responseHeaderIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return it.impl<_Iter *>()->it->first;
    }
    virtual zfstring responseHeaderIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return it.impl<_Iter *>()->it->second;
    }

public:
    static zfindex _ZFP_urlParse(ZF_IN const zfchar *url) {
        zfindex p = zfstringFind(url, "://");
        if(p != zfindexMax()) {
            p += 3;
        }
        else {
            p = 0;
            while(url[p] == '/') {
                ++p;
            }
        }
        zfindex t = zfstringFind(url + p, '/');
        if(t != zfindexMax()) {
            return p + t;
        }
        else {
            return zfindexMax();
        }
    }

    static httplib::Result _ZFP_asyncRequest(
            ZF_IN ZFHttpMethod httpMethod
            , ZF_IN const zfchar *url
            , ZF_IN zfindex pHostPath
            , ZF_IN zftimet timeout
            , ZF_IN const httplib::Headers &headers
            , ZF_IN const void *body
            , ZF_IN zfindex bodySize
            ) {
        NativeTask::Client client(zfstring(url, pHostPath).cString());
        time_t secs = timeout / 1000;
        time_t usecs = (timeout % 1000) * 1000;
        client.set_connection_timeout(secs, usecs);
        client.set_read_timeout(secs, usecs);
        client.set_write_timeout(secs, usecs);
        const zfchar *hostPath = (pHostPath == zfindexMax() ? "/" : url + pHostPath);

        switch(httpMethod) {
            case v_ZFHttpMethod::e_GET:
                return client.Get(hostPath, headers);
            case v_ZFHttpMethod::e_HEAD:
                return client.Head(hostPath, headers);
            case v_ZFHttpMethod::e_PUT:
                return client.Put(hostPath, headers, (const char *)body, (size_t)bodySize, "");
            case v_ZFHttpMethod::e_DELETE:
                return client.Delete(hostPath, headers, (const char *)body, (size_t)bodySize, "");
            case v_ZFHttpMethod::e_OPTIONS:
                return client.Options(hostPath, headers);
            case v_ZFHttpMethod::e_PATCH:
                return client.Patch(hostPath, headers, (const char *)body, (size_t)bodySize, "");
            case v_ZFHttpMethod::e_CONNECT:
            case v_ZFHttpMethod::e_TRACE:
            case v_ZFHttpMethod::e_POST:
            default:
                return client.Post(hostPath, headers, (const char *)body, (size_t)bodySize, "");
        }
    }
    static void _ZFP_parseResponse(
            ZF_IN_OUT NativeTask *task
            , ZF_IN const httplib::Result &result
            ) {
        zfbool success = result;
        task->ownerResponse->success(success);
        if(success) {
            task->responseHeaders = std::move(result.value().headers);
            task->ownerResponse->code((zfint)result.value().status);
            task->ownerResponse->body().append(result.value().body.c_str(), result.value().body.length());
        }
        switch(result.error()) {
            case httplib::Error::Success:
                task->ownerResponse->errorHint(zfnull);
                break;
            case httplib::Error::Unknown:
                task->ownerResponse->errorHint("unknown error");
                break;
            case httplib::Error::Connection:
                task->ownerResponse->errorHint("connection failed");
                break;
            case httplib::Error::BindIPAddress:
                task->ownerResponse->errorHint("bind ip address failed");
                break;
            case httplib::Error::Read:
                task->ownerResponse->errorHint("read failed");
                break;
            case httplib::Error::Write:
                task->ownerResponse->errorHint("write failed");
                break;
            case httplib::Error::ExceedRedirectCount:
                task->ownerResponse->errorHint("exceed redirect count");
                break;
            case httplib::Error::Canceled:
                task->ownerResponse->errorHint("canceled");
                break;
            case httplib::Error::SSLConnection:
                task->ownerResponse->errorHint("ssl connection failed");
                break;
            case httplib::Error::SSLLoadingCerts:
                task->ownerResponse->errorHint("ssl cert load failed");
                break;
            case httplib::Error::SSLServerVerification:
                task->ownerResponse->errorHint("ssl server verification failed");
                break;
            case httplib::Error::UnsupportedMultipartBoundaryChars:
                task->ownerResponse->errorHint("unsupported multipart bondary chars");
                break;
            case httplib::Error::Compression:
                task->ownerResponse->errorHint("compression failed");
                break;
            case httplib::Error::ConnectionTimeout:
                task->ownerResponse->errorHint("connection timeout");
                break;
            default:
                task->ownerResponse->errorHint(zfstr("unknown error: %s", (zfint)result.error()));
                break;
        }
    }

private:
    ZFIdentityGenerator taskIdGen;
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpRequestImpl_default)

ZF_NAMESPACE_GLOBAL_END

#endif

