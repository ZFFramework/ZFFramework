#include "ZFImpl_sys_Qt_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

#if ZF_ENV_sys_Qt

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class _ZFP_ZFHttpRequestImpl_sys_Qt_Task : public QObject {
    Q_OBJECT

public:
    ZFHttpRequest *ownerRequest;
    ZFHttpResponse *ownerResponse;
    ZFHttpMethodEnum httpMethod;
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QList<QByteArray> *rawHeaderList;
    QByteArray body;
    QNetworkReply *response;
    QList<QByteArray> responseRawHeaderList;
    zfautoObjectT<ZFTimer *> timeoutTimer;

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
        ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(this->ownerRequest);
    }

    void _ZFP_notifyCancel() {
        if(this->timeoutTimer != zfnull) {
            this->timeoutTimer->timerStop();
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
            this->timeoutTimer->timerStop();
            this->timeoutTimer = zfnull;
        }

        bool ok = false;
        int code = nativeResponse->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
        if(ok) {
            this->ownerResponse->success(code == 200);
            this->ownerResponse->code((zfint)code);
        }
        else {
            this->ownerResponse->success(nativeResponse->error() == QNetworkReply::NoError);
            this->ownerResponse->code((zfint)nativeResponse->error());
        }
        if(nativeResponse->error() != QNetworkReply::NoError) {
            this->ownerResponse->errorHint(zfstr("recv error, code: %d", (zfint)nativeResponse->error()));
        }
        { // response body
            ZFBuffer &body = this->ownerResponse->body();
            const zfint bufSize = 256;
            do {
                body.bufferCapacity(body.bufferSize() + bufSize);
                zfint read = (zfint)nativeResponse->read((char *)body.buffer() + body.bufferSize(), (qint64)bufSize);
                if(read > 0) {
                    body.bufferSize(body.bufferSize() + read);
                }
                if(read < bufSize) {
                    body.text()[body.textLength()] = '\0';
                    break;
                }
            } while(true);
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
            , ZF_IN const zfchar *url
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->request.setUrl(QUrl(url));
    }

    virtual void httpMethod(
            ZF_IN void *nativeTask
            , ZF_IN ZFHttpMethodEnum httpMethod
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->httpMethod = httpMethod;
    }

    virtual void header(
            ZF_IN void *nativeTask
            , ZF_IN const zfchar *key
            , ZF_IN const zfchar *value
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->request.setRawHeader(key, value);
        if(task->rawHeaderList != NULL) {
            delete task->rawHeaderList;
            task->rawHeaderList = NULL;
        }
    }

    virtual void headerRemove(
            ZF_IN void *nativeTask
            , ZF_IN const zfchar *key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->request.setRawHeader(key, QByteArray());
        if(task->rawHeaderList != NULL) {
            delete task->rawHeaderList;
            task->rawHeaderList = NULL;
        }
    }

    virtual zfstring header(
            ZF_IN void *nativeTask
            , ZF_IN const zfchar *key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        return task->request.rawHeader(key).data();
    }

    virtual zfindex headerCount(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        if(task->rawHeaderList == NULL) {
            task->rawHeaderList = new QList<QByteArray>(task->request.rawHeaderList());
        }
        return task->rawHeaderList->size();
    }

    virtual zfiterator headerIter(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        if(task->rawHeaderList == NULL) {
            task->rawHeaderList = new QList<QByteArray>(task->request.rawHeaderList());
        }
        return zfiterator((void *)new QList<QByteArray>::Iterator(task->rawHeaderList->begin()), _ZFP_iterDelete, _ZFP_iterCopy);
    }
    virtual zfbool headerIterValid(
            ZF_IN void *nativeTask
            , ZF_IN const zfiterator &it
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        return task->rawHeaderList != NULL && nativeIt != NULL && *nativeIt != task->rawHeaderList->end();
    }
    virtual void headerIterNext(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiterator &it
            ) {
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        if(nativeIt != NULL) {
            ++(*nativeIt);
        }
    }
    virtual zfstring headerIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiterator &it
            ) {
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        if(nativeIt != NULL) {
            return (*nativeIt)->data();
        }
        else {
            return "";
        }
    }
    virtual zfstring headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiterator &it
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        if(nativeIt != NULL) {
            return task->request.rawHeader((*nativeIt)->data()).data();
        }
        else {
            return "";
        }
    }
    virtual void headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiterator &it
            , ZF_IN const zfchar *value
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        if(nativeIt != NULL) {
            task->request.setRawHeader((*nativeIt)->data(), value);
        }
    }
    virtual void headerIterRemove(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiterator &it
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        if(nativeIt != NULL) {
            task->request.setRawHeader((*nativeIt)->data(), NULL);
        }
    }

    virtual void body(
            ZF_IN void *nativeTask
            , ZF_IN const void *buffer
            , ZF_IN zfindex byteSize
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        task->body.append((const char *)buffer, byteSize);
    }
    virtual ZFBuffer body(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        ZFBuffer ret;
        ret.zfunsafe_bufferChange(task->body.data(), task->body.capacity(), task->body.size(), false);
        return ret;
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
            , ZF_IN const zfchar *key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        if(task->response == NULL) {
            return "";
        }
        return task->response->rawHeader(key).data();
    }

    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        return task->responseRawHeaderList.size();
    }

    virtual zfiterator responseHeaderIter(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        return zfiterator((void *)new QList<QByteArray>::Iterator(task->responseRawHeaderList.begin()), _ZFP_iterDelete, _ZFP_iterCopy);
    }
    virtual zfbool responseHeaderIterValid(
            ZF_IN void *nativeTask
            , ZF_IN const zfiterator &it
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        return task->response != NULL && nativeIt != NULL && (*nativeIt) != task->responseRawHeaderList.end();
    }
    virtual void responseHeaderIterNext(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiterator &it
            ) {
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        if(nativeIt != NULL) {
            ++(*nativeIt);
        }
    }
    virtual zfstring responseHeaderIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiterator &it
            ) {
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        if(nativeIt != NULL) {
            return (*nativeIt)->data();
        }
        else {
            return "";
        }
    }
    virtual zfstring responseHeaderIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiterator &it
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Qt_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Qt_Task *)nativeTask;
        QList<QByteArray>::Iterator *nativeIt = it.data<QList<QByteArray>::Iterator *>();
        if(nativeIt != NULL && task->response != NULL) {
            return task->response->rawHeader((*nativeIt)->data()).data();
        }
        else {
            return "";
        }
    }
private:
    static void _ZFP_iterDelete(void *data) {
        delete (QList<QByteArray>::Iterator *)data;
    }
    static void *_ZFP_iterCopy(void *data) {
        return new QList<QByteArray>::Iterator(*(QList<QByteArray>::Iterator *)data);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpRequestImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFHttpRequestImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFHttpRequest_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

