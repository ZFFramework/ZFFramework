#include "ZFImpl_default_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpServer.h"

#include "ZFImpl/ZFImpl_env.h"

#if defined(__cplusplus) && (__cplusplus >= 201103L) && !ZF_ENV_sys_Qt

#if ZF_ENV_HTTPS
    #define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#define CPPHTTPLIB_NO_EXCEPTIONS
#include "../../../zf3rd/_repo/cpp-httplib/httplib.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFHttpServerImpl_default_TaskQueue : zfextend httplib::TaskQueue {
public:
    zfweakT<ZFHttpServer> owner;
private:
    zfobj<ZFSet> taskMap; // ZFTaskId
    zfobj<ZFSet> semaMap; // ZFSemaphore
public:
    virtual bool enqueue(std::function<void()> fn) override {
        _ZFP_ZFHttpServerImpl_default_TaskQueue *taskQueue = this;
        ZFLISTENER_2(impl
                , _ZFP_ZFHttpServerImpl_default_TaskQueue *, taskQueue
                , std::function<void()>, fn
                ) {
            if(!zfargs.param0()) {
                return;
            }

            ZFCoreMutexLock();
            zfobj<ZFSemaphore> sema;
            taskQueue->semaMap->add(sema);
            ZFCoreMutexUnlock();

            fn();

            ZFCoreMutexLock();
            taskQueue->semaMap->remove(sema);
            taskQueue->taskMap->remove(zfargs.sender());
            ZFCoreMutexUnlock();
            sema->lockAndBroadcast();
        } ZFLISTENER_END()

        ZFCoreMutexLock();
        zfauto taskId = ZFHttpServerThreadPool::instance()->start(impl, zfnull);
        this->taskMap->add(taskId);
        ZFCoreMutexUnlock();
        return zftrue;
    }

    virtual void shutdown() override {
        ZFCoreMutexLock();
        for(zfiter it = this->taskMap->iter(); it; ++it) {
            ZFTaskId *t = this->taskMap->iterValue(it);
            t->stop();
        }
        ZFCoreMutexUnlock();

        while(!this->semaMap->isEmpty()) {
            ZFCoreMutexLock();
            zfiter it = this->semaMap->iter();
            zfautoT<ZFSemaphore> sema = this->semaMap->iterValue(it);
            this->semaMap->iterRemove(it);
            ZFCoreMutexUnlock();

            sema->lockAndWait();
        }
    }
};

zfclass _ZFP_ZFHttpServerImpl_default_Task : zfextend ZFHttpServerTask {
    ZFOBJECT_DECLARE(_ZFP_ZFHttpServerImpl_default_Task, ZFHttpServerTask)

public:
    zfweakT<ZFHttpServer> owner;
    const httplib::Request *req;
    httplib::Response *res;
    const httplib::ContentReader *content_reader;

private:
    ZFCoreMap<zfstring, zfstring> _recvHeaderCache;
    ZFCoreMap<zfstring, zfstring> _recvParamCache;
    ZFHttpMethod _recvMethodCache;
    zfstring *_recvBodyCache;

    ZFCoreMap<zfstring, zfstring> _respHeaderCache;

private:
    template<typename T_Multimap>
    void _headersCacheUpdate(ZF_OUT ZFCoreMap<zfstring, zfstring> &dst, ZF_IN const T_Multimap &src) {
        if(dst.isEmpty() && !src.empty()) {
            for(auto srcIt = src.begin(); srcIt != src.end(); ++srcIt) {
                zfstring key = srcIt->first.c_str();
                zfiter dstIt = dst.iterFind(key);
                if(dstIt) {
                    zfstring &value = dst.iterValue(dstIt);
                    value += ", ";
                    value += srcIt->second.c_str();
                }
                else {
                    dst.set(key, srcIt->second.c_str());
                }
            }
        }
    }

    void _recvHeaderCacheUpdate(void) {
        this->_headersCacheUpdate(_recvHeaderCache, req->headers);
    }
    void _recvParamCacheUpdate(void) {
        this->_headersCacheUpdate(_recvParamCache, req->params);
    }

    void _respHeaderCacheUpdate(void) {
        this->_headersCacheUpdate(_respHeaderCache, res->headers);
    }

protected:
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();

        this->content_reader = zfnull;

        this->_recvMethodCache = (ZFHttpMethod)-1;
        this->_recvBodyCache = zfnull;
    }
    virtual void objectOnDealloc(void) {
        if(this->_recvBodyCache) {
            zfpoolDelete(this->_recvBodyCache);
            this->_recvBodyCache = zfnull;
        }
        zfsuper::objectOnDealloc();
    }

    // ============================================================
    // recv
public:
    zfoverride
    virtual zfstring recvBody(void) {
        if(this->_recvBodyCache == zfnull) {
            this->_recvBodyCache = zfpoolNew(zfstring);
            zfstring &buf = *(this->_recvBodyCache);
            if(this->content_reader) {
                (*(this->content_reader))([&](const char *data, size_t len) {
                    if(len > 0) {
                        buf.append((const zfchar *)data, len);
                    }
                    return true;
                });
            }
        }
        return *(this->_recvBodyCache);
    }

public:
    zfoverride
    virtual zfindex recvHeaderCount(void) {
        _recvHeaderCacheUpdate();
        return _recvHeaderCache.count();
    }
    zfoverride
    virtual zfstring recvHeader(ZF_IN const zfstring &key) {
        if(_recvHeaderCache.isEmpty()) {
            auto range = this->req->headers.equal_range(key.cString());
            zfstring ret;
            for(auto it = range.first; it != range.second; ++it) {
                if(ret) {
                    ret += ", ";
                }
                ret += it->second.c_str();
            }
            return ret;
        }
        else {
            zfiter it = _recvHeaderCache.iterFind(key);
            if(it) {
                return _recvHeaderCache.iterValue(it);
            }
            else {
                return zfnull;
            }
        }
    }

    zfoverride
    virtual zfiter recvHeaderIter(void) {
        _recvHeaderCacheUpdate();
        return _recvHeaderCache.iter();
    }
    zfoverride
    virtual zfstring recvHeaderIterKey(ZF_IN const zfiter &it) {
        return _recvHeaderCache.iterKey(it);
    }
    zfoverride
    virtual zfstring recvHeaderIterValue(ZF_IN const zfiter &it) {
        return _recvHeaderCache.iterValue(it);
    }

public:
    zfoverride
    virtual zfindex recvParamCount(void) {
        _recvParamCacheUpdate();
        return _recvParamCache.count();
    }
    zfoverride
    virtual zfstring recvParam(ZF_IN const zfstring &key) {
        if(_recvParamCache.isEmpty()) {
            auto range = this->req->params.equal_range(key.cString());
            zfstring ret;
            for(auto it = range.first; it != range.second; ++it) {
                if(ret) {
                    ret += ", ";
                }
                ret += it->second.c_str();
            }
            return ret;
        }
        else {
            zfiter it = _recvParamCache.iterFind(key);
            if(it) {
                return _recvParamCache.iterValue(it);
            }
            else {
                return zfnull;
            }
        }
    }

    zfoverride
    virtual zfiter recvParamIter(void) {
        _recvParamCacheUpdate();
        return _recvParamCache.iter();
    }
    zfoverride
    virtual zfstring recvParamIterKey(ZF_IN const zfiter &it) {
        return _recvParamCache.iterKey(it);
    }
    zfoverride
    virtual zfstring recvParamIterValue(ZF_IN const zfiter &it) {
        return _recvParamCache.iterValue(it);
    }

public:
    zfoverride
    virtual ZFHttpMethod recvMethod(void) {
        if(_recvMethodCache == (ZFHttpMethod)-1) {
            const zfchar *method = this->req->method.c_str();
            if(zffalse) {
            }
            else if(zfsicmp(method, "POST") == 0) {
                _recvMethodCache = v_ZFHttpMethod::e_POST;
            }
            else if(zfsicmp(method, "GET") == 0) {
                _recvMethodCache = v_ZFHttpMethod::e_GET;
            }
            else if(zfsicmp(method, "HEAD") == 0) {
                _recvMethodCache = v_ZFHttpMethod::e_GET;
            }
            else if(zfsicmp(method, "DELETE") == 0) {
                _recvMethodCache = v_ZFHttpMethod::e_GET;
            }
            else if(zfsicmp(method, "OPTIONS") == 0) {
                _recvMethodCache = v_ZFHttpMethod::e_GET;
            }
            else if(zfsicmp(method, "PATCH") == 0) {
                _recvMethodCache = v_ZFHttpMethod::e_GET;
            }
            else if(zfsicmp(method, "CONNECT") == 0) {
                _recvMethodCache = v_ZFHttpMethod::e_GET;
            }
            else if(zfsicmp(method, "TRACE") == 0) {
                _recvMethodCache = v_ZFHttpMethod::e_GET;
            }
            else {
                _recvMethodCache = v_ZFHttpMethod::e_GET;
            }
        }
        return _recvMethodCache;
    }
    zfoverride
    virtual zfstring recvPath(void) {
        return this->req->path.c_str();
    }
    zfoverride
    virtual zfstring recvClientAddr(void) {
        return this->req->remote_addr.c_str();
    }
    zfoverride
    virtual zfuint recvClientPort(void) {
        return (zfuint)this->req->remote_port;
    }
    zfoverride
    virtual zfstring recvServerAddr(void) {
        return this->req->local_addr.c_str();
    }
    zfoverride
    virtual zfuint recvServerPort(void) {
        return (zfuint)this->req->local_port;
    }

    // ============================================================
    // resp
public:
    zfoverride
    virtual void respCode(ZF_IN zfuint code) {
        this->res->status = (int)code;
    }

    zfoverride
    virtual void respBody(ZF_IN const zfstring &value, ZF_IN const zfstring &contentType) {
        this->res->set_content(value.cString(), value.length(), contentType.cString());
    }
    zfoverride
    virtual void respBody(ZF_IN const ZFInput &value, ZF_IN const zfstring &contentType) {
        zfindex size = value.ioSize();
        zfautoT<v_zfstring> buf = zfobj<v_zfstring>();
        buf->zfv.capacity(4096);
        if(size == zfindexMax()) {
            this->res->set_content_provider(contentType.cString(), [=](size_t offset, httplib::DataSink &sink) {
                zfindex read = 0;
                value.input(buf->zfv.zfunsafe_buffer(), buf->zfv.capacity(), &read);
                sink.write((const char *)buf->zfv.buffer(), (size_t)read);
                if(read < buf->zfv.capacity()) {
                    sink.done();
                }
                return true;
            });
        }
        else {
            this->res->set_content_provider((size_t)size, contentType.cString(), [=](size_t offset, size_t length, httplib::DataSink &sink) {
                zfindex read = 0;
                value.input(buf->zfv.zfunsafe_buffer(), buf->zfv.capacity(), &read);
                sink.write((const char *)buf->zfv.buffer(), (size_t)read);
                if(read < buf->zfv.capacity()) {
                    sink.done();
                }
                return true;
            });
        }
    }

public:
    zfoverride
    virtual zfindex respHeaderCount(void) {
        _respHeaderCacheUpdate();
        return _respHeaderCache.count();
    }
    zfoverride
    virtual zfstring respHeader(ZF_IN const zfstring &key) {
        if(_respHeaderCache.isEmpty()) {
            auto range = this->res->headers.equal_range(key.cString());
            zfstring ret;
            for(auto it = range.first; it != range.second; ++it) {
                if(ret) {
                    ret += ", ";
                }
                ret += it->second.c_str();
            }
            return ret;
        }
        else {
            zfiter it = _respHeaderCache.iterFind(key);
            if(it) {
                return _respHeaderCache.iterValue(it);
            }
            else {
                return zfnull;
            }
        }
    }

    zfoverride
    virtual zfiter respHeaderIter(void) {
        _respHeaderCacheUpdate();
        return _respHeaderCache.iter();
    }
    zfoverride
    virtual zfstring respHeaderIterKey(ZF_IN const zfiter &it) {
        return _respHeaderCache.iterKey(it);
    }
    zfoverride
    virtual zfstring respHeaderIterValue(ZF_IN const zfiter &it) {
        return _respHeaderCache.iterValue(it);
    }

    zfoverride
    virtual void respHeader(ZF_IN const zfstring &key, ZF_IN const zfstring &value) {
        std::string keyTmp = key.cString();
        this->res->headers.erase(keyTmp);
        this->res->headers.insert(std::pair<std::string, std::string>(keyTmp, value.cString()));
        _respHeaderCache.removeAll();
    }
    zfoverride
    virtual void respHeaderRemove(ZF_IN const zfstring &key) {
        this->res->headers.erase(key.cString());
        _respHeaderCache.removeAll();
    }
    zfoverride
    virtual zfiter respHeaderIterFind(ZF_IN const zfstring &key) {
        _respHeaderCacheUpdate();
        return _respHeaderCache.iterFind(key);
    }
    virtual void respHeaderIterValue(ZF_IN_OUT zfiter &it, ZF_IN const zfstring &value) {
        std::string key = _respHeaderCache.iterKey(it).cString();
        this->res->headers.erase(key);
        this->res->headers.insert(std::pair<std::string, std::string>(key, value.cString()));
        _respHeaderCache.iterValue(it, value);
    }
    zfoverride
    virtual void respHeaderIterRemove(ZF_IN_OUT zfiter &it) {
        this->res->headers.erase(_respHeaderCache.iterKey(it).cString());
        _respHeaderCache.iterRemove(it);
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFHttpServerImpl_default, ZFHttpServer, v_ZFProtocolLevel::e_Default)
private:
    zfclassNotPOD _TaskToken {
    public:
        httplib::Server impl;
        zfobj<ZFThread> serverThread;
    };

public:
    zfoverride
    virtual void *start(ZF_IN ZFHttpServer *owner) {
        zfweakT<ZFHttpServer> ownerHolder = owner;
        _TaskToken *impl = zfpoolNew(_TaskToken);

        impl->impl.new_task_queue = [=]() -> httplib::TaskQueue * {
            _ZFP_ZFHttpServerImpl_default_TaskQueue *ret = new _ZFP_ZFHttpServerImpl_default_TaskQueue();
            ret->owner = ownerHolder;
            return ret;
        };

        auto cb2 = [=](const httplib::Request &req, httplib::Response &res) {
            _notify(ownerHolder, &req, &res, zfnull);
        };
        auto cb3 = [=](const httplib::Request &req, httplib::Response &res, const httplib::ContentReader &content_reader) {
            _notify(ownerHolder, &req, &res, &content_reader);
        };

        impl->impl.Get(".*", cb2);
        impl->impl.Post(".*", cb3);
        impl->impl.Put(".*", cb3);
        impl->impl.Patch(".*", cb3);
        impl->impl.Delete(".*", cb3);
        impl->impl.Options(".*", cb2);

        impl->serverThread->threadName(zfstr("HttpServer:%s:%s", owner->addr(), owner->port()));

        ZFLISTENER_2(threadImpl
                , _TaskToken *, impl
                , zfweakT<ZFHttpServer>, ownerHolder
                ) {
            {
                ZFThread *owner = zfargs.sender();
                if(owner == zfnull || owner->threadStopRequested()) {
                    zfpoolDelete(impl);
                    return;
                }
            }
            if(!impl->impl.listen(ownerHolder->addr().cString(), ownerHolder->port())) {
                if(ownerHolder && ownerHolder->started()) {
                    ZFPROTOCOL_ACCESS(ZFHttpServer)->notifyOnError(ownerHolder, zftext("server start failed"));
                }
            }
            zfpoolDelete(impl);
        } ZFLISTENER_END()
        impl->serverThread->threadRunnable(threadImpl);

        ZFLISTENER_1(threadOnStop
                , _TaskToken *, impl
                ) {
            impl->impl.stop();
        } ZFLISTENER_END()
        impl->serverThread->observerAdd(ZFThread::E_ThreadOnStopRequested(), threadOnStop);

        impl->serverThread->threadStart();
        return impl;
    }
    zfoverride
    virtual void stop(ZF_IN ZFHttpServer *owner, ZF_IN void *token) {
        _TaskToken *impl = (_TaskToken *)token;
        impl->impl.stop();
    }
private:
    void _notify(
            ZF_IN const zfweakT<ZFHttpServer> &owner
            , ZF_IN const httplib::Request *req
            , ZF_IN_OUT httplib::Response *res
            , ZF_IN const httplib::ContentReader *content_reader
            ) {
        zfobj<_ZFP_ZFHttpServerImpl_default_Task> task;
        task->owner = owner;
        task->req = req;
        task->res = res;
        task->content_reader = content_reader;
        this->notifyOnRequest(owner, task);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpServerImpl_default)

ZF_NAMESPACE_GLOBAL_END

#endif

