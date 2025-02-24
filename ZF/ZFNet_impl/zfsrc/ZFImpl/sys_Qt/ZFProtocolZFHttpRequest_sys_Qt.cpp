#include "ZFImpl_sys_Qt_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

#if ZF_ENV_sys_Qt

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class _ZFP_ZFHttpRequestImpl_sys_Qt_Task : public QObject {
    Q_OBJECT

public:
    ZFHttpRequest *ownerRequest;
    ZFHttpResponse *ownerResponse;
    ZFHttpMethod httpMethod;
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QList<QByteArray> *rawHeaderList;
    QByteArray body;
    QNetworkReply *response;
    QList<QByteArray> responseRawHeaderList;
    zfautoT<ZFTimer> timeoutTimer;

public:
    _ZFP_ZFHttpRequestImpl_sys_Qt_Task(void)
    : QObject()
    , ownerRequest(zfnull)
    , ownerResponse(zfnull)
    , httpMethod(ZFHttpMethod::e_GET)
    , manager()
    , request()
    , rawHeaderList(NULL)
    , body()
    , response(zfnull)
    , responseRawHeaderList()
    {
        connect(&manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(_ZFP_notifyResponse(QNetworkReply *)));
    }
    ~_ZFP_ZFHttpRequestImpl_sys_Qt_Task(void) {
        if(this->rawHeaderList != NULL) {
            delete this->rawHeaderList;
            this->rawHeaderList = NULL;
        }
    }

public:
    void _ZFP_notifyTimeout() {
        this->timeoutTimer = zfnull;
        this->response->abort();
        this->response->deleteLater();
        this->response = NULL;

        this->ownerResponse->success(zffalse);
        this->ownerResponse->code(504);
        this->ownerResponse->errorHint("request timeout");
        this->ownerResponse->body(zfnull);
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(this->ownerRequest);
    }

    void _ZFP_notifyCancel() {
        if(this->timeoutTimer != zfnull) {
            this->timeoutTimer->stop();
            this->timeoutTimer = zfnull;
        }
        if(this->response != NULL) {
            this->response->abort();
            this->response->deleteLater();
            this->response = NULL;
        }
    }

public slots:
    void _ZFP_notifyResponse(QNetworkReply *nativeResponse) {
        if(this->timeoutTimer == zfnull) {
            return;
        }
        if(this->timeoutTimer != zfnull) {
            this->timeoutTimer->stop();
            this->timeoutTimer = zfnull;
        }

        bool ok = false;
        int code = nativeResponse->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
        this->ownerResponse->success(nativeResponse->error() == QNetworkReply::NoError);
        if(ok) {
            this->ownerResponse->code((zfint)code);
        }
        else {
            this->ownerResponse->code((zfint)nativeResponse->error());
        }
        if(nativeResponse->error() != QNetworkReply::NoError) {
            this->ownerResponse->errorHint(zfstr("recv error, code: %s", (zfint)nativeResponse->error()));
        }

        { // response body
            zfstring body;
            const zfint bufSize = 256;
            do {
                body.capacity(body.length() + bufSize);
                zfint read = (zfint)nativeResponse->read((char *)body.zfunsafe_buffer() + body.length(), (qint64)bufSize);
                if(read > 0) {
                    body.zfunsafe_length(body.length() + read);
                }
                if(read < bufSize) {
                    body.zfunsafe_buffer()[body.length()] = '\0';
                    break;
                }
            } while(true);
            this->ownerResponse->body(body);
        }

        this->responseRawHeaderList = nativeResponse->rawHeaderList();
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(this->ownerRequest);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFHttpRequestImpl_sys_Qt, ZFHttpRequest, ZFProtocolLevel::e_SystemHigh)
    // ============================================================
    // for request
public:
    virtual zfbool httpsAvailable(void) {
        return zftrue;
    }

    virtual void *nativeTaskCreate(
            ZF_IN ZFHttpRequest *request
            , ZF_IN ZFHttpResponse *response
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = new _ZFP_ZFHttpRequestImpl_sys_Qt_Task();
        task->ownerRequest = request;
        task->ownerResponse = response;
        return task;
    }
    virtual void nativeTaskDestroy(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->deleteLater();
    }

    virtual void url(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &url
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->request.setUrl(QUrl(url.cString()));
    }

    virtual void httpMethod(
            ZF_IN void *nativeTask
            , ZF_IN ZFHttpMethod httpMethod
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->httpMethod = httpMethod;
    }

    virtual void header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            , ZF_IN const zfstring &value
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->request.setRawHeader(key.cString(), value.cString());
        if(task->rawHeaderList != NULL) {
            delete task->rawHeaderList;
            task->rawHeaderList = NULL;
        }
    }

    virtual void headerRemove(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->request.setRawHeader(key.cString(), QByteArray());
        if(task->rawHeaderList != NULL) {
            delete task->rawHeaderList;
            task->rawHeaderList = NULL;
        }
    }

    virtual zfstring header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        return task->request.rawHeader(key.cString()).data();
    }

    virtual zfindex headerCount(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        if(task->rawHeaderList == NULL) {
            task->rawHeaderList = new QList<QByteArray>(task->request.rawHeaderList());
        }
        return task->rawHeaderList->size();
    }

    zfclassNotPOD _Iter : zfextend zfiter::Impl {
    public:
        QList<QByteArray>::Iterator it;
        QList<QByteArray>::Iterator end;
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
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        if(task->rawHeaderList == NULL) {
            task->rawHeaderList = new QList<QByteArray>(task->request.rawHeaderList());
        }
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = task->rawHeaderList->begin();
        impl->end = task->rawHeaderList->end();
        return zfiter(impl);
    }
    virtual zfstring headerIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return it.impl<_Iter *>()->it->data();
    }
    virtual zfstring headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        return task->request.rawHeader(it.impl<_Iter *>()->it->data()).data();
    }
    virtual void headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            , ZF_IN const zfstring &value
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->request.setRawHeader(it.impl<_Iter *>()->it->data(), value.cString());
    }
    virtual void headerIterRemove(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->request.setRawHeader(it.impl<_Iter *>()->it->data(), NULL);
    }

    virtual void body(
            ZF_IN void *nativeTask
            , ZF_IN const void *buffer
            , ZF_IN zfindex byteSize
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->body.append((const char *)buffer, byteSize);
    }
    virtual zfstring body(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        return zfstring::shared((const zfchar *)task->body.data(), (zfindex)task->body.size());
    }

    virtual void request(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;

        ZFLISTENER_1(onTimeout
                , _ZFP_ZFHttpRequestImpl_sys_Qt_Task *, task
                ) {
            task->_ZFP_notifyTimeout();
        } ZFLISTENER_END()
        task->timeoutTimer = ZFTimerOnce(task->ownerRequest->timeout(), onTimeout);

        switch(task->httpMethod) {
            case ZFHttpMethod::e_GET:
                task->response = task->manager.get(task->request);
                break;
            case ZFHttpMethod::e_HEAD:
                task->response = task->manager.head(task->request);
                break;
            case ZFHttpMethod::e_PUT:
                task->response = task->manager.put(task->request, task->body);
                break;
            case ZFHttpMethod::e_DELETE:
                task->response = task->manager.deleteResource(task->request);
                break;
            case ZFHttpMethod::e_CONNECT:
            case ZFHttpMethod::e_OPTIONS:
            case ZFHttpMethod::e_TRACE:
            case ZFHttpMethod::e_PATCH:
            case ZFHttpMethod::e_POST:
            default:
                task->response = task->manager.post(task->request, task->body);
                break;
        }
    }

    virtual void requestCancel(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->_ZFP_notifyCancel();
    }

    // ============================================================
    // for response
public:
    virtual zfstring responseHeader(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        if(task->response == NULL) {
            return "";
        }
        return task->response->rawHeader(key.cString()).data();
    }

    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        return task->responseRawHeaderList.size();
    }

    virtual zfiter responseHeaderIter(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = task->responseRawHeaderList.begin();
        impl->end = task->responseRawHeaderList.end();
        return zfiter(impl);
    }
    virtual zfstring responseHeaderIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return it.impl<_Iter *>()->it->data();
    }
    virtual zfstring responseHeaderIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        return task->response->rawHeader(it.impl<_Iter *>()->it->data()).data();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpRequestImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFHttpRequest_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

