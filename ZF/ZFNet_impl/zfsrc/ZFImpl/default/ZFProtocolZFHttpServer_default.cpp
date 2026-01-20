#include "ZFImpl_default_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpServer.h"

#include "ZFImpl/ZFImpl_env.h"

#if defined(__cplusplus) && (__cplusplus >= 201103L) && !ZF_ENV_sys_Qt

#if ZF_ENV_HTTPS
    #define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#define CPPHTTPLIB_NO_EXCEPTIONS
#include "../../../zf3rd/_repo/cpp-httplib/httplib.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFHttpServerImpl_default_TaskQueue : zfextend httplib::TaskQueue {
public:
    zfweakT<ZFHttpServer> owner;
private:
    zfclassNotPOD KeyHash {
    public:
        inline zfstlsize operator () (zfauto const &v) const {
            return (zfstlsize)zfidentityCalc(v.toObject());
        }
    };
    typedef zfstlhashmap<zfauto, zfbool, KeyHash> TaskMapType;
    TaskMapType taskMap; // <ZFTaskId, dummy>
    TaskMapType semaMap; // <ZFSemaphore, dummy>
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
            taskQueue->semaMap[sema] = zftrue;
            ZFCoreMutexUnlock();

            fn();

            ZFCoreMutexLock();
            taskQueue->semaMap.erase(sema);
            taskQueue->taskMap.erase(zfargs.sender());
            ZFCoreMutexUnlock();
            sema->lockAndBroadcast();
        } ZFLISTENER_END()

        ZFCoreMutexLock();
        zfauto taskId = ZFHttpServerThreadPool::instance()->start(impl, zfnull);
        this->taskMap[taskId] = zftrue;
        ZFCoreMutexUnlock();
        return zftrue;
    }

    virtual void shutdown() override {
        ZFCoreMutexLock();
        for(auto it = this->taskMap.begin(); it != this->taskMap.end(); ++it) {
            ZFTaskId *t = it->first;
            t->stop();
        }
        ZFCoreMutexUnlock();

        while(!this->semaMap.empty()) {
            ZFCoreMutexLock();
            zfautoT<ZFSemaphore> sema = this->semaMap.begin()->first;
            this->semaMap.erase(this->semaMap.begin());
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
    zfstlhashmap<zfstring, zfstring> _recvHeaderCache;
    zfstlhashmap<zfstring, zfstring> _recvParamCache;
    ZFHttpMethod _recvMethodCache;
    zfstring *_recvBodyCache;

    zfstlhashmap<zfstring, zfstring> _respHeaderCache;

private:
    template<typename T_Multimap>
    void _headersCacheUpdate(ZF_OUT zfstlhashmap<zfstring, zfstring> &dst, ZF_IN const T_Multimap &src) {
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

private:
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

public:
    zfoverride
    virtual zfindex recvHeaderCount(void) {
        _recvHeaderCacheUpdate();
        return (zfindex)_recvHeaderCache.size();
    }
    zfoverride
    virtual zfstring recvHeader(ZF_IN const zfstring &key) {
        if(_recvHeaderCache.empty()) {
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
            auto it = _recvHeaderCache.find(key);
            if(it != _recvHeaderCache.end()) {
                return it->second;
            }
            else {
                return zfnull;
            }
        }
    }

    zfoverride
    virtual zfiter recvHeaderIter(void) {
        _recvHeaderCacheUpdate();
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = _recvHeaderCache.begin();
        impl->end = _recvHeaderCache.end();
        return zfiter(impl);
    }
    zfoverride
    virtual zfstring recvHeaderIterKey(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->first;
    }
    zfoverride
    virtual zfstring recvHeaderIterValue(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->second;
    }

public:
    zfoverride
    virtual zfindex recvParamCount(void) {
        _recvParamCacheUpdate();
        return _recvParamCache.size();
    }
    zfoverride
    virtual zfstring recvParam(ZF_IN const zfstring &key) {
        if(_recvParamCache.empty()) {
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
            auto it = _recvParamCache.find(key);
            if(it != _recvParamCache.end()) {
                return it->second;
            }
            else {
                return zfnull;
            }
        }
    }

    zfoverride
    virtual zfiter recvParamIter(void) {
        _recvParamCacheUpdate();
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = _recvParamCache.begin();
        impl->end = _recvParamCache.end();
        return zfiter(impl);
    }
    zfoverride
    virtual zfstring recvParamIterKey(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->first;
    }
    zfoverride
    virtual zfstring recvParamIterValue(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->second;
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
        return (zfindex)_respHeaderCache.size();
    }
    zfoverride
    virtual zfstring respHeader(ZF_IN const zfstring &key) {
        if(_respHeaderCache.empty()) {
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
            auto it = _respHeaderCache.find(key);
            if(it != _respHeaderCache.end()) {
                return it->second;
            }
            else {
                return zfnull;
            }
        }
    }

    zfoverride
    virtual zfiter respHeaderIter(void) {
        _respHeaderCacheUpdate();
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = _respHeaderCache.begin();
        impl->end = _respHeaderCache.end();
        return zfiter(impl);
    }
    zfoverride
    virtual zfstring respHeaderIterKey(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->first;
    }
    zfoverride
    virtual zfstring respHeaderIterValue(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->second;
    }

    zfoverride
    virtual void respHeader(ZF_IN const zfstring &key, ZF_IN const zfstring &value) {
        std::string keyTmp = key.cString();
        this->res->headers.erase(keyTmp);
        this->res->headers.insert(std::pair<std::string, std::string>(keyTmp, value.cString()));
        _respHeaderCache.clear();
    }
    zfoverride
    virtual void respHeaderRemove(ZF_IN const zfstring &key) {
        this->res->headers.erase(key.cString());
        _respHeaderCache.clear();
    }
    zfoverride
    virtual zfiter respHeaderIterFind(ZF_IN const zfstring &key) {
        _respHeaderCacheUpdate();
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = _respHeaderCache.find(key.cString());
        impl->end = _respHeaderCache.end();
        return zfiter(impl);
    }
    virtual void respHeaderIterValue(ZF_IN const zfiter &it, ZF_IN const zfstring &value) {
        _Iter *impl = it.impl<_Iter *>();
        std::string key = impl->it->first.cString();
        this->res->headers.erase(key);
        this->res->headers.insert(std::pair<std::string, std::string>(key, value.cString()));
        impl->it->second = value;
    }
    zfoverride
    virtual void respHeaderIterRemove(ZF_IN const zfiter &it) {
        _Iter *impl = it.impl<_Iter *>();
        this->res->headers.erase(impl->it->first.cString());
        _respHeaderCache.erase((impl->it)++);
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

