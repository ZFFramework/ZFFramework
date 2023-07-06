#include "ZFImpl_default_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

#include "ZFImpl/ZFImpl_env.h"

#if defined(__cplusplus) && (__cplusplus >= 201103L) && !ZF_ENV_sys_Android

#if ZF_ENV_HTTPS
    #define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#define CPPHTTPLIB_NO_EXCEPTIONS
#include "../../../zf3rd/_repo/cpp-httplib/httplib.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFHttpRequestImpl_default, ZFHttpRequest, ZFProtocolLevel::e_Default)
private:
    zfclassNotPOD NativeTask
    {
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
        ZFHttpResponse *ownerResponse;
        zfidentity taskId;
        zfstring url;
        ZFHttpMethodEnum httpMethod;
        httplib::Headers headers;
        ZFBuffer body;
        httplib::Headers responseHeaders;

    public:
        NativeTask(ZF_IN ZFHttpRequest *request,
                   ZF_IN ZFHttpResponse *response)
        : refCount(1)
        , ownerRequest(request)
        , ownerResponse(response)
        , taskId(zfidentityInvalid())
        , url()
        , httpMethod(ZFHttpMethod::e_GET)
        , headers()
        , body()
        , responseHeaders()
        {
        }
    };

public:
#if !ZF_ENV_sys_Windows
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        signal(SIGPIPE, SIG_IGN);
    }
#endif

    // ============================================================
    // for request
public:
    virtual zfbool httpsAvailable(void)
    {
        #if ZF_ENV_HTTPS
            return zftrue;
        #else
            return zffalse;
        #endif
    }

    virtual void *nativeTaskCreate(ZF_IN ZFHttpRequest *request,
                                   ZF_IN ZFHttpResponse *response)
    {
        return zfnew(NativeTask, request, response);
    }
    virtual void nativeTaskDestroy(ZF_IN void *nativeTask)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        --(nativeTaskTmp->refCount);
        if(nativeTaskTmp->refCount == 0)
        {
            zfdelete(nativeTaskTmp);
        }
    }

    virtual void url(ZF_IN void *nativeTask,
                     ZF_IN const zfchar *url)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->url = url;
    }

    virtual void httpMethod(ZF_IN void *nativeTask,
                            ZF_IN ZFHttpMethodEnum httpMethod)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->httpMethod = httpMethod;
    }

    virtual void header(ZF_IN void *nativeTask,
                        ZF_IN const zfchar *key,
                        ZF_IN const zfchar *value)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->headers.insert(std::pair<std::string, std::string>(key, value));
    }

    virtual void headerRemove(ZF_IN void *nativeTask,
                              ZF_IN const zfchar *key)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->headers.erase(key);
    }

    virtual zfstring header(ZF_IN void *nativeTask,
                            ZF_IN const zfchar *key)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        auto range = nativeTaskTmp->headers.equal_range(key);
        zfstring ret;
        for(auto i = range.first; i != range.second; ++i)
        {
            if(!ret.isEmpty())
            {
                ret += ',';
            }
            ret += i->second.c_str();
        }
        return ret;
    }

    virtual zfindex headerCount(ZF_IN void *nativeTask)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        return (zfindex)nativeTaskTmp->headers.size();
    }

    static void _ZFP_headerIter_d(void *data)
    {
        zfdelete((httplib::Headers::iterator *)data);
    }
    static void *_ZFP_headerIter_c(void *data)
    {
        return zfnew(httplib::Headers::iterator, *(httplib::Headers::iterator *)data);
    }

    virtual zfiterator headerIter(ZF_IN void *nativeTask)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        return zfiterator(
                zfnew(httplib::Headers::iterator, nativeTaskTmp->headers.begin()),
                _ZFP_headerIter_d,
                _ZFP_headerIter_c
            );
    }
    virtual zfbool headerIterValid(ZF_IN void *nativeTask,
                                   ZF_IN const zfiterator &it)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        return (data && *data != nativeTaskTmp->headers.end());
    }
    virtual void headerIterNext(ZF_IN void *nativeTask,
                                ZF_IN_OUT zfiterator &it)
    {
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        ++(*data);
    }
    virtual zfstring headerIterKey(ZF_IN void *nativeTask,
                                   ZF_IN const zfiterator &it)
    {
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        return (*data)->first.c_str();
    }
    virtual zfstring headerIterValue(ZF_IN void *nativeTask,
                                     ZF_IN const zfiterator &it)
    {
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        return (*data)->second.c_str();
    }
    virtual void headerIterValue(ZF_IN void *nativeTask,
                                 ZF_IN_OUT zfiterator &it,
                                 ZF_IN const zfchar *value)
    {
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        (*data)->second = value;
    }
    virtual void headerIterRemove(ZF_IN void *nativeTask,
                                  ZF_IN_OUT zfiterator &it)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        nativeTaskTmp->headers.erase(*data);
    }

    virtual void body(ZF_IN void *nativeTask,
                      ZF_IN const void *buffer,
                      ZF_IN zfindex byteSize)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        nativeTaskTmp->body.bufferAppend(buffer, byteSize);
    }

    virtual ZFBuffer body(ZF_IN void *nativeTask)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        return nativeTaskTmp->body;
    }

    virtual void request(ZF_IN void *nativeTask)
    {
        this->requestCancel(nativeTask);

        NativeTask *task = (NativeTask *)nativeTask;
        ++(task->refCount);
        zfidentity taskId = this->taskIdGen.idAcquire();
        task->taskId = taskId;
        ZFLISTENER_2(asyncRequest
                , NativeTask *, task
                , zfidentity, taskId
                ) {
            zfstring url = task->url;
            zfindex redirectCount = 0;
            while(zftrue)
            {
                if(taskId != task->taskId)
                {
                    return;
                }
                if(redirectCount >= 10)
                {
                    task->ownerResponse->success(zffalse);
                    task->ownerResponse->code(-1);
                    task->ownerResponse->errorHint("exceed redirect count");
                    if(taskId == task->taskId)
                    {
                        ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(task->ownerRequest);
                    }
                    break;
                }
                #if !ZF_ENV_HTTPS
                if(zfstringFind(url, "https://") == 0)
                {
                    task->ownerResponse->success(zffalse);
                    task->ownerResponse->code(-1);
                    task->ownerResponse->errorHint("https not supported");
                    if(taskId == task->taskId)
                    {
                        ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(task->ownerRequest);
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
                    task->body.bufferSize());
                if(result && result.value().status >= 300 && result.value().status < 400)
                {
                    std::string redirectUrl = result.value().get_header_value("Location");
                    if(!redirectUrl.empty())
                    {
                        url = redirectUrl.c_str();
                        ++redirectCount;
                        continue;
                    }
                }

                if(taskId == task->taskId)
                {
                    _ZFP_parseResponse(task, result);
                }
                if(taskId == task->taskId)
                {
                    ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(task->ownerRequest);
                }
                break;
            }
        } ZFLISTENER_END()
        ZFLISTENER_3(asyncRequestOnFinish
                , ZFIdentityGenerator, taskIdGen
                , NativeTask *, task
                , zfidentity, taskId
                ) {
            if(taskId == task->taskId)
            {
                taskIdGen.idRelease(task->taskId);
                task->taskId = zfidentityInvalid();
            }
            --(task->refCount);
            if(task->refCount == 0)
            {
                zfdelete(task);
            }
        } ZFLISTENER_END()
        zfasync(asyncRequest, asyncRequestOnFinish);
    }

    virtual void requestCancel(ZF_IN void *nativeTask)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        if(nativeTaskTmp->taskId != zfidentityInvalid())
        {
            this->taskIdGen.idRelease(nativeTaskTmp->taskId);
            nativeTaskTmp->taskId = zfidentityInvalid();
        }
    }

    // ============================================================
    // for response
public:
    virtual zfstring responseHeader(ZF_IN void *nativeTask,
                                    ZF_IN const zfchar *key)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        auto range = nativeTaskTmp->responseHeaders.equal_range(key);
        zfstring ret;
        for(auto i = range.first; i != range.second; ++i)
        {
            if(!ret.isEmpty())
            {
                ret += ',';
            }
            ret += i->second.c_str();
        }
        return ret;
    }

    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        return nativeTaskTmp->responseHeaders.size();
    }

    virtual zfiterator responseHeaderIter(ZF_IN void *nativeTask)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        return zfiterator(
                zfnew(httplib::Headers::iterator, nativeTaskTmp->responseHeaders.begin()),
                _ZFP_headerIter_d,
                _ZFP_headerIter_c
            );
    }
    virtual zfbool responseHeaderIterValid(ZF_IN void *nativeTask,
                                           ZF_IN const zfiterator &it)
    {
        NativeTask *nativeTaskTmp = (NativeTask *)nativeTask;
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        return (data && *data != nativeTaskTmp->responseHeaders.end());
    }
    virtual void responseHeaderIterNext(ZF_IN void *nativeTask,
                                        ZF_IN_OUT zfiterator &it)
    {
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        ++(*data);
    }
    virtual zfstring responseHeaderIterKey(ZF_IN void *nativeTask,
                                           ZF_IN const zfiterator &it)
    {
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        return (*data)->first.c_str();
    }
    virtual zfstring responseHeaderIterValue(ZF_IN void *nativeTask,
                                             ZF_IN const zfiterator &it)
    {
        httplib::Headers::iterator *data = it.data<httplib::Headers::iterator *>();
        return (*data)->second.c_str();
    }

public:
    static zfindex _ZFP_urlParse(ZF_IN const zfchar *url)
    {
        zfindex p = zfstringFind(url, "://");
        if(p != zfindexMax())
        {
            p += 3;
        }
        else
        {
            p = 0;
            while(url[p] == '/')
            {
                ++p;
            }
        }
        zfindex t = zfstringFind(url + p, '/');
        if(t != zfindexMax())
        {
            return p + t;
        }
        else
        {
            return zfindexMax();
        }
    }

    static httplib::Result _ZFP_asyncRequest(ZF_IN ZFHttpMethodEnum httpMethod,
                                             ZF_IN const zfchar *url,
                                             ZF_IN zfindex pHostPath,
                                             ZF_IN zftimet timeout,
                                             ZF_IN httplib::Headers headers,
                                             ZF_IN const void *body,
                                             ZF_IN zfindex bodySize)
    {
        NativeTask::Client client(zfstring(url, pHostPath).cString());
        time_t secs = timeout / 1000;
        time_t usecs = (timeout % 1000) * 1000;
        client.set_connection_timeout(secs, usecs);
        client.set_read_timeout(secs, usecs);
        client.set_write_timeout(secs, usecs);
        const zfchar *hostPath = (pHostPath == zfindexMax() ? "/" : url + pHostPath);

        switch(httpMethod)
        {
            case ZFHttpMethod::e_GET:
                return client.Get(hostPath, headers);
            case ZFHttpMethod::e_HEAD:
                return client.Head(hostPath, headers);
            case ZFHttpMethod::e_PUT:
                return client.Put(hostPath, headers, (const char *)body, (size_t)bodySize, "");
            case ZFHttpMethod::e_DELETE:
                return client.Delete(hostPath, headers, (const char *)body, (size_t)bodySize, "");
            case ZFHttpMethod::e_OPTIONS:
                return client.Options(hostPath, headers);
            case ZFHttpMethod::e_PATCH:
                return client.Patch(hostPath, headers, (const char *)body, (size_t)bodySize, "");
            case ZFHttpMethod::e_CONNECT:
            case ZFHttpMethod::e_TRACE:
            case ZFHttpMethod::e_POST:
            default:
                return client.Post(hostPath, headers, (const char *)body, (size_t)bodySize, "");
        }
    }
    static void _ZFP_parseResponse(ZF_IN_OUT NativeTask *task, ZF_IN const httplib::Result &result)
    {
        task->responseHeaders = std::move(result.value().headers);
        task->ownerResponse->success(zftrue);
        task->ownerResponse->code((zfint)result.value().status);
        task->ownerResponse->body().bufferAppend(result.value().body.c_str(), result.value().body.length());
        switch(result.error())
        {
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
                task->ownerResponse->errorHint(zfstringWithFormat("unknown error: %d", (zfint)result.error()));
                break;
        }
    }

private:
    ZFIdentityGenerator taskIdGen;
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpRequestImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFHttpRequestImpl_default)

ZF_NAMESPACE_GLOBAL_END

#endif

