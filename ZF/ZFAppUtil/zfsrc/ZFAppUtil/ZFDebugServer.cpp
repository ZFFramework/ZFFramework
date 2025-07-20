#include "ZFDebugServer.h"

#include "ZFNet/protocol/ZFProtocolZFHttpServer.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfauto _ZFP_ZFDebugServerImpl(
        ZF_IN_OUT ZFHttpServerTask *task
        , ZF_IN const zfstring &run
        ) {
    zfstring outputBuf;
    ZFOutput orig = ZFOutputDefault();
    ZFOutputForString(outputBuf);
    ZFOutputDefault(ZFOutputForString(outputBuf));

    zfbool success = zffalse;
    zfstring errorHint;
    zfauto ret = ZFLuaExecuteDetail(run, ZFCoreArray<zfauto>(), &success, &errorHint);

    ZFOutputDefault(orig);

    task->respBody(ZFJson()
            .attr("errno", success ? "0" : "-1")
            .attr("error", success ? "success" : errorHint.cString())
            .attr("result", ZFObjectInfo(ret))
            .attr("output", outputBuf)
            .toString(), "application/json");
    return ret;
}

ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFHttpServer>, ZFDebugServer
        , ZFMP_IN(zfuint, port)
        ) {
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFHttpServer)) {
        return zfnull;
    }

    zfobj<ZFHttpServer> ret;
    ret->port(port);
    ZFLISTENER_0(impl
            ) {
        ZFHttpServerTask *task = zfargs.param0();
        ZFJson recv = ZFJsonFromString(task->recvBody());
        zfstring run = recv.attrValue("run");
        if(!run) {
            task->respBody(ZFJson()
                    .attr("errno", "-1")
                    .attr("error", "invalid param")
                    .attr("result", task->recvBody())
                    .attr("output", "")
                    .toString(), "application/json");
            return;
        }

        ZFThread *curThread = ZFThread::currentThread();
        if(curThread && curThread->isMainThread()) {
            _ZFP_ZFDebugServerImpl(task, run);
            return;
        }

        zfobj<ZFSemaphore> sema;
        ZFLISTENER_3(impl
                , ZFHttpServerTask *, task
                , zfstring, run
                , zfautoT<ZFSemaphore>, sema
                ) {
            _ZFP_ZFDebugServerImpl(task, run);
            sema->lockAndBroadcast();
        } ZFLISTENER_END()
        zfpost(impl);
        sema->lockAndWait();
    } ZFLISTENER_END()
    ret->onRequest(impl);
    ret->start();
    ZFLogTrim("[ZFDebugServer] started on port: %s", port);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, ZFDebugClient
        , ZFMP_IN(const zfstring &, url)
        , ZFMP_IN(const zfstring &, luaCode)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        ) {
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFHttpRequest)) {
        if(callback) {
            callback.execute(ZFArgs()
                    .param0(zfnull)
                    .param1(zfobj<v_ZFResultType>(v_ZFResultType::e_Fail))
                    );
        }
        return zfnull;
    }

    zfobj<ZFHttpRequest> req(url, v_ZFHttpMethod::e_POST);
    req->body(ZFJson()
            .attr("run", luaCode)
            .toString());
    if(callback) {
        req->start(callback);
    }
    else {
        ZFLISTENER_0(onRecv
                ) {
            ZFHttpResponse *recv = zfargs.param0();
            if(recv) {
                ZFLogTrim("%s", recv->contentInfo());
            }
        } ZFLISTENER_END()
        req->start(onRecv);
    }
    return req;
}

ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, ZFDebugClient
        , ZFMP_IN(const zfstring &, url)
        , ZFMP_IN(const ZFInput &, luaCode)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        ) {
    zfstring buf;
    ZFInputRead(buf, luaCode);
    return ZFDebugClient(url, buf, callback);
}

ZF_NAMESPACE_GLOBAL_END

