#include "ZFThread_zfasyncIO.h"
#include "ZFThreadPool.h"

// #define _ZFP_zfasyncIO_DEBUG 1

#if _ZFP_zfasyncIO_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_zfasyncIO_log(fmt, ...) \
        zfimplLog("%s [zfasyncIO] " fmt \
                , zfimplTime() \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_zfasyncIO_log(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFThreadPoolForIO)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFThreadPoolForIO, instance, ZFLevelZFFrameworkEssential)

zfclass _ZFP_I_zfasyncIOTask : zfextend ZFTaskId {
    ZFOBJECT_DECLARE(_ZFP_I_zfasyncIOTask, ZFTaskId)
public:
    zfobj<ZFObject> outputMutex;
    ZFOutput output;
    ZFInput input;
    zfindex outputOffset;
    zfindex inputOffset;
    ZFCoreArray<zfautoT<ZFTaskId> > blockTask;
public:
    zfoverride
    virtual void stop(void) {
        if(!this->blockTask.isEmpty()) {
            for(zfindex i = 0; i < this->blockTask.count(); ++i) {
                this->blockTask[i]->stop();
            }
            this->blockTask.removeAll();
        }
        zfsuper::stop();
    }
};
ZFOBJECT_REGISTER(_ZFP_I_zfasyncIOTask)

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, zfasyncIOCustom
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    return ZFThreadPoolForIO::instance()->start(callback, finishCallback);
}
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, zfasyncIO
        , ZFMP_IN(ZF_IN const ZFOutput &, output)
        , ZFMP_IN(ZF_IN const ZFInput &, input)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    if(!input || !output) {
        finishCallback.execute(ZFArgs()
                .sender(zfnull)
                .param0(zfobj<v_zfbool>(zffalse))
                );
        return zfnull;
    }

    zfindex outputOffset = output.ioTell();
    zfindex inputOffset = input.ioTell();
    zfindex srcLen = input.ioSize();
    zfindex blockSize = ZFThreadPoolForIO::instance()->blockSize();
    if(srcLen == zfindexMax() || srcLen <= blockSize || outputOffset == zfindexMax() || inputOffset == zfindexMax()) {
        ZFLISTENER_2(impl
                , ZFOutput, output
                , ZFInput, input
                ) {
            zfbyte buf[4096];
            while(zfargs.param0()) {
                zfindex size = input.execute(buf, sizeof(buf));
                output.execute(buf, size);
                if(size < sizeof(buf)) {
                    break;
                }
            }
        } ZFLISTENER_END()
        return zfasyncIOCustom(impl, finishCallback);
    }

    zfobj<_ZFP_I_zfasyncIOTask> task;
    task->input = input;
    task->output = output;
    task->outputOffset = outputOffset;
    task->inputOffset = inputOffset;
    ZFLISTENER_2(implOnFinish
            , zfautoT<_ZFP_I_zfasyncIOTask>, task
            , ZFListener, finishCallback
            ) {
        zfbool success = zfargs.param0().to<v_zfbool *>()->zfv;
        _ZFP_zfasyncIO_log("task stop %s: %s", success ? "success" : "fail", task->input.callbackId().cString());
        if(!success) {
            task->stop();
            finishCallback.execute(ZFArgs()
                    .sender(task)
                    .param0(zfobj<v_zfbool>(zffalse))
                    );
            return;
        }
        task->blockTask.removeElement(zfargs.sender());
        if(task->blockTask.isEmpty()) {
            finishCallback.execute(ZFArgs()
                    .sender(task)
                    .param0(zfobj<v_zfbool>(zftrue))
                    );
        }
    } ZFLISTENER_END()
    _ZFP_zfasyncIO_log("task start: %s", task->input.callbackId().cString());
    for(zfindex splitOffset = 0; splitOffset < srcLen; splitOffset += blockSize) {
        zfindex splitSize = zfmMin(blockSize, srcLen - splitOffset);
        ZFLISTENER_3(impl
                , zfautoT<_ZFP_I_zfasyncIOTask>, task
                , zfindex, splitOffset
                , zfindex, splitSize
                ) {
            void *buf = zfmalloc(splitSize);
            zfblockedFree(buf);
            do {
                _ZFP_zfasyncIO_log("split begin (%d %d)", (int)splitOffset, (int)splitSize);
                {
                    zfsynchronize(task);
                    if(!task->input.ioSeek(task->inputOffset + splitOffset)) {
                        _ZFP_zfasyncIO_log("split end (%d %d): input seek fail", (int)splitOffset, (int)splitSize);
                        break;
                    }
                    if(task->input.execute(buf, splitSize) < splitSize) {
                        _ZFP_zfasyncIO_log("split end (%d %d): input fail", (int)splitOffset, (int)splitSize);
                        break;
                    }
                }

                {
                    zfsynchronize(task->outputMutex);
                    if(!task->output.ioSeek(task->outputOffset + splitOffset)) {
                        _ZFP_zfasyncIO_log("split end (%d %d): output seek fail", (int)splitOffset, (int)splitSize);
                        break;
                    }
                    if(task->output.execute(buf, splitSize) < splitSize) {
                        _ZFP_zfasyncIO_log("split end (%d %d): output fail", (int)splitOffset, (int)splitSize);
                        break;
                    }
                }

                zfargs.result(zfobj<v_zfbool>(zftrue));
                _ZFP_zfasyncIO_log("split end (%d %d): success", (int)splitOffset, (int)splitSize);
                return;
            } while(zffalse);
            zfargs.result(zfobj<v_zfbool>(zffalse));
        } ZFLISTENER_END()
        task->blockTask.add(zfasyncIOCustom(impl, implOnFinish));
    }
    return task;
}

ZF_NAMESPACE_GLOBAL_END

