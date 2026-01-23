#include "ZFThread_zfasyncIO.h"
#include "ZFThread_zfpost.h"
#include "ZFHashSet.h"
#include "ZFTime.h"

// #define _ZFP_zfasyncIO_DEBUG 1
// #define _ZFP_zfasyncIO_DEBUG_simulateSlow 500

#if _ZFP_zfasyncIO_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_zfasyncIO_log(fmt, ...) \
        zfimplLog("%s [zfasyncIO] %s", zfimplTime(), zfstr(fmt, ##__VA_ARGS__).cString())
#else
    #define _ZFP_zfasyncIO_log(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFThreadPoolForIO)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFThreadPoolForIO, instance, ZFLevelZFFrameworkEssential)

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, zfasyncIOCustom
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    return ZFThreadPoolForIO::instance()->start(callback, finishCallback);
}

ZFOBJECT_REGISTER(ZFAsyncIOResumable)
ZFMETHOD_DEFINE_0(ZFAsyncIOResumable, void, reset) {
    this->outputId(zfnull);
    this->inputId(zfnull);
    this->blockSize(zfindexMax());
    this->totalSize(zfindexMax());
    this->successBlocks(zfnull);
}

// ============================================================
static zfautoT<ZFTaskId> _ZFP_zfasyncIO_simple(
        ZF_IN const ZFOutput &output
        , ZF_IN const ZFInput &input
        , ZF_IN const ZFListener &finishCallback
        , ZF_IN const ZFListener &progressCallback
        , ZF_IN ZFAsyncIOResumable *resumable
        , ZF_IN zfindex outputOffset
        , ZF_IN zfindex inputOffset
        , ZF_IN zfindex blockSize
        , ZF_IN zfindex totalSize
        ) {
    ZFThread *ownerThread = ZFThread::currentThread();
    ZFLISTENER_9(impl
            , zfautoT<ZFThread>, ownerThread
            , ZFOutput, output
            , ZFInput, input
            , ZFListener, progressCallback
            , zfautoT<ZFAsyncIOResumable>, resumable
            , zfindex, outputOffset
            , zfindex, inputOffset
            , zfindex, blockSize
            , zfindex, totalSize
            ) {
        zfbyte buf[4096];
        zfobj<v_ZFArgs> zfargsHolder(zfargs);
        zfobj<v_zftimet> progressCallbackLastTime;
        zfobj<v_zfindex> sizeWritten;
        // non null if can resume
        zfautoT<ZFHashSet> successBlocks;
        if(resumable && outputOffset != zfindexMax() && inputOffset != zfindexMax()) {
            successBlocks = resumable->successBlocks();
        }
        do {
            if(!zfargs.param0()) {break;}
            if(progressCallback
                    && progressCallbackLastTime->zfv != zftimetInvalid()
                    && ZFTime::currentTime() - progressCallbackLastTime->zfv >= 500
                    ) {
                progressCallbackLastTime->zfv = zftimetInvalid();
                ZFLISTENER_5(onProgress
                        , ZFListener, progressCallback
                        , zfautoT<v_ZFArgs>, zfargsHolder
                        , zfautoT<v_zftimet>, progressCallbackLastTime
                        , zfautoT<v_zfindex>, sizeWritten
                        , zfindex, totalSize
                        ) {
                    if(!zfargsHolder->zfv.param0()) {return;}
                    progressCallback.execute(ZFArgs()
                            .sender(zfargsHolder->zfv.param0())
                            .param0(sizeWritten)
                            .param1(zfobj<v_zfindex>(totalSize))
                            );
                    progressCallbackLastTime->zfv = ZFTime::currentTime();
                } ZFLISTENER_END()
                zfpost(onProgress, ownerThread);
            }
            zfobj<v_zfindex> block((zfindex)(sizeWritten->zfv / blockSize));

            // skip successed block
            if(successBlocks
                    && (sizeWritten->zfv % blockSize) == 0
                    && (ZFObjectLockerHolder(successBlocks), successBlocks->isContain(block))
                    ) {
                output.ioSeek(outputOffset + sizeWritten->zfv);
                input.ioSeek(inputOffset + sizeWritten->zfv);
                sizeWritten->zfv += blockSize;
                _ZFP_zfasyncIO_log("simple task block %s skipped", block->zfv);
                continue;
            }

            zfindex size = input.execute(buf, sizeof(buf));
#if _ZFP_zfasyncIO_DEBUG_simulateSlow
            if((sizeWritten->zfv % blockSize) == 0) {
                ZFThread::sleep(_ZFP_zfasyncIO_DEBUG_simulateSlow);
            }
#endif
            if(size == zfindexMax()) {
                _ZFP_zfasyncIO_log("simple task block %s input fail: %s", block->zfv, sizeWritten->zfv);
                sizeWritten->zfv = zfindexMax();
                return;
            }
            if(!zfargs.param0()) {break;}
            if(output.execute(buf, size) != size) {
                sizeWritten->zfv = zfindexMax();
                _ZFP_zfasyncIO_log("simple task block %s output fail: %s", block->zfv, sizeWritten->zfv);
                return;
            }

            // update resumable
            if(successBlocks && (sizeWritten->zfv % blockSize) == 0) {
                output.ioFlush();
                {
                    ZFObjectLocker(successBlocks);
                    successBlocks->add(block);
                }
                _ZFP_zfasyncIO_log("simple task block %s finish", block->zfv);
            }

            sizeWritten->zfv += size;
            if(size < sizeof(buf)) {
                break;
            }
        } while(zftrue);
        if(zfargs.param0()) {
            zfargs.result(sizeWritten);
        }
    } ZFLISTENER_END()
    ZFLISTENER_2(implOnFinish
            , ZFListener, finishCallback
            , zfautoT<ZFAsyncIOResumable>, resumable
            ) {
        _ZFP_zfasyncIO_log("simple task stop, written: %s", zfargs.param0());
        finishCallback.execute(ZFArgs()
                .sender(zfargs.sender())
                .param0(zfargs.param0() ? zfargs.param0() : zfobj<v_zfindex>(zfindexMax()))
                .param1(resumable)
                );
    } ZFLISTENER_END()
    _ZFP_zfasyncIO_log("simple task start: %s => %s", input.callbackId(), output.callbackId());
    return zfasyncIOCustom(impl, implOnFinish);
}

zfclass _ZFP_I_zfasyncIOSplitTask : zfextend ZFObject, zfimplement ZFTaskId {
    ZFOBJECT_DECLARE(_ZFP_I_zfasyncIOSplitTask, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFTaskId)
public:
    zfobj<ZFObject> outputMutex;
    ZFOutput output; // also used as task running checker
    ZFInput input;
    zfindex outputOffset;
    zfindex inputOffset;
    zfindex blockSize;
    zfindex totalSize;
    zfindex sizeWritten;
    zftimet progressCallbackLastTime;
    zfautoT<ZFAsyncIOResumable> resumable;
    ZFCoreArray<zfautoT<ZFTaskId> > blockTask;
    zfautoT<ZFThread> ownerThread;
public:
    void progressPost(ZF_IN const ZFListener &progressCallback) {
        if(progressCallback
                && this->progressCallbackLastTime != zftimetInvalid()
                && ZFTime::currentTime() - this->progressCallbackLastTime > 500
                ) {
            this->progressCallbackLastTime = zftimetInvalid();
            zfweakT<zfself> owner = this;
            ZFLISTENER_2(onProgress
                    , zfweakT<zfself>, owner
                    , ZFListener, progressCallback
                    ) {
                if(!owner || !owner->output) {return;}
                progressCallback.execute(ZFArgs()
                        .sender(owner.get())
                        .param0(zfobj<v_zfindex>(owner->sizeWritten))
                        .param1(zfobj<v_zfindex>(owner->totalSize))
                        );
                owner->progressCallbackLastTime = ZFTime::currentTime();
            } ZFLISTENER_END()
            zfpost(onProgress, this->ownerThread);
        }
    }
public:
    zfoverride
    virtual void stop(void) {
        if(!this->blockTask.isEmpty()) {
            for(zfindex i = 0; i < this->blockTask.count(); ++i) {
                this->blockTask[i]->stop();
            }
            this->blockTask.removeAll();
        }
        this->output = zfnull;
        this->input = zfnull;
        this->ownerThread = zfnull;
    }
};
ZFOBJECT_REGISTER(_ZFP_I_zfasyncIOSplitTask)

static zfautoT<ZFTaskId> _ZFP_zfasyncIO_split(
        ZF_IN const ZFOutput &output
        , ZF_IN const ZFInput &input
        , ZF_IN const ZFListener &finishCallback
        , ZF_IN const ZFListener &progressCallback
        , ZF_IN ZFAsyncIOResumable *resumable
        , ZF_IN zfindex outputOffset
        , ZF_IN zfindex inputOffset
        , ZF_IN zfindex blockSize
        , ZF_IN zfindex totalSize
        ) {
    zfobj<_ZFP_I_zfasyncIOSplitTask> task;
    task->input = input;
    task->output = output;
    task->outputOffset = outputOffset;
    task->inputOffset = inputOffset;
    task->blockSize = blockSize;
    task->totalSize = totalSize;
    task->sizeWritten = 0;
    task->progressCallbackLastTime = 0;
    task->resumable = resumable;
    task->ownerThread = ZFThread::currentThread();

    ZFLISTENER_2(implOnFinish
            , zfautoT<_ZFP_I_zfasyncIOSplitTask>, task
            , ZFListener, finishCallback
            ) {
        v_zfindex *splitOffset = zfargs.param0();
        if(splitOffset == zfnull || splitOffset->zfv == zfindexMax()) {
            task->stop();
            finishCallback.execute(ZFArgs()
                    .sender(task)
                    .param0(zfobj<v_zfindex>(zfindexMax()))
                    .param1(task->resumable)
                    );
            return;
        }
        task->blockTask.removeElement(zfargs.sender());
        if(task->blockTask.isEmpty()) {
            _ZFP_zfasyncIO_log("split task stop, written: %s", task->sizeWritten);
            finishCallback.execute(ZFArgs()
                    .sender(task)
                    .param0(zfobj<v_zfindex>(task->sizeWritten == task->totalSize ? task->totalSize : zfindexMax()))
                    .param1(task->resumable)
                    );
        }
    } ZFLISTENER_END()

    // non null if can resume
    zfautoT<ZFHashSet> successBlocks;
    if(task->resumable && task->outputOffset != zfindexMax() && task->inputOffset != zfindexMax()) {
        successBlocks = task->resumable->successBlocks();
    }

    _ZFP_zfasyncIO_log("split task start: %s => %s", task->input.callbackId(), task->output.callbackId());
    for(zfindex splitOffset = 0; splitOffset < totalSize; splitOffset += blockSize) {
        zfindex splitSize = zfmMin(blockSize, totalSize - splitOffset);

        // skip successed block
        if(successBlocks
                && (ZFObjectLockerHolder(successBlocks), successBlocks->isContain(zfobj<v_zfindex>(splitOffset / blockSize)))
                ) {
            {
                ZFObjectLocker(task->outputMutex);
                task->sizeWritten += splitSize;
            }
            task->progressPost(progressCallback);
            _ZFP_zfasyncIO_log("split task block %s skipped", (zfindex)(splitOffset / blockSize));
            continue;
        }

        ZFLISTENER_6(impl
                , zfautoT<_ZFP_I_zfasyncIOSplitTask>, task
                , zfindex, splitOffset
                , zfindex, splitSize
                , zfindex, blockSize
                , zfautoT<ZFHashSet>, successBlocks
                , ZFListener, progressCallback
                ) {
            zfbyte buf[4096];
            for(zfindex index = splitOffset; index < splitOffset + splitSize; index += sizeof(buf)) {
                zfindex size = zfmMin(splitOffset + splitSize - index, (zfindex)sizeof(buf));
                {
                    if(!task->output) {return;}
                    ZFObjectLocker(task);
                    if(!task->input.ioSeek(task->inputOffset + index)) {
                        _ZFP_zfasyncIO_log("split task block %s input seek fail: %s", (zfindex)(splitOffset / blockSize), index);
                        return;
                    }
                    if(task->input.execute(buf, size) < size) {
                        _ZFP_zfasyncIO_log("split task block %s input fail: %s", (zfindex)(splitOffset / blockSize), index);
                        return;
                    }
#if _ZFP_zfasyncIO_DEBUG_simulateSlow
                    if(index == splitOffset) {
                        ZFThread::sleep(_ZFP_zfasyncIO_DEBUG_simulateSlow);
                    }
#endif
                }

                {
                    if(!task->output) {return;}
                    ZFObjectLocker(task->outputMutex);
                    if(!task->output.ioSeek(task->outputOffset + index)) {
                        _ZFP_zfasyncIO_log("split task block %s output seek fail: %s", (zfindex)(splitOffset / blockSize), index);
                        return;
                    }
                    if(task->output.execute(buf, size) < size) {
                        _ZFP_zfasyncIO_log("split task block %s output fail: %s", (zfindex)(splitOffset / blockSize), index);
                        return;
                    }
                    {
                        ZFObjectLocker(task->outputMutex);
                        task->sizeWritten += size;
                    }
                }
                task->progressPost(progressCallback);
            }
            _ZFP_zfasyncIO_log("split task block %s finish", (zfindex)(splitOffset / blockSize));
            {
                if(!task->output) {return;}
                ZFObjectLocker(task->outputMutex);
                task->output.ioFlush();
                if(!task->output) {return;}
            }

            // update resumable
            {
                ZFObjectLocker(successBlocks);
                successBlocks->add(zfobj<v_zfindex>(splitOffset / blockSize));
            }

            zfargs.result(zfobj<v_zfindex>(splitOffset));
        } ZFLISTENER_END()
        task->blockTask.add(zfasyncIOCustom(impl, implOnFinish));
    }
    if(task->blockTask.isEmpty()) {
        _ZFP_zfasyncIO_log("split task stop, written: %s", task->sizeWritten);
        finishCallback.execute(ZFArgs()
                .sender(task)
                .param0(zfobj<v_zfindex>(task->sizeWritten == task->totalSize ? task->totalSize : zfindexMax()))
                .param1(task->resumable)
                );
        task->output = zfnull;
    }
    return task;
}

ZFMETHOD_FUNC_DEFINE_5(zfautoT<ZFTaskId>, zfasyncIO
        , ZFMP_IN(ZF_IN const ZFOutput &, output)
        , ZFMP_IN(ZF_IN const ZFInput &, input)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(const ZFListener &, progressCallback, zfnull)
        , ZFMP_IN_OPT(ZFAsyncIOResumable *, resumable, zfnull)
        ) {
    if(!input || !output) {
        finishCallback.execute(ZFArgs()
                .sender(zfnull)
                .param0(zfobj<v_zfindex>(zfindexMax()))
                .param1(resumable)
                );
        return zfnull;
    }

    zfindex outputOffset = output.ioTell();
    zfindex inputOffset = input.ioTell();
    zfindex blockSize = ZFThreadPoolForIO::instance()->blockSize();
    zfindex totalSize = input.ioSize();

    zfbool needReset = zffalse;
    if(output.callbackId()) {
        if(resumable) {
            if(!needReset && resumable->outputId() && resumable->outputId() != output.callbackId()) {
                needReset = zftrue;
            }
            if(!needReset && resumable) {
                ZFHashSet *successBlocks = resumable->successBlocks();
                if(successBlocks) {
                    zfindex outputSize = output.ioSize();
                    if(outputSize != zfindexMax()) {
                        for(zfiter it = successBlocks->iter(); it; ++it) {
                            v_zfindex *block = successBlocks->iterValue(it);
                            if(block->zfv * blockSize > outputSize) {
                                needReset = zftrue;
                            }
                        }
                    }
                }
            }
            resumable->outputId(output.callbackId());
        }
    }
    else {
        if(resumable) {
            resumable = zfnull;
        }
    }
    if(input.callbackId()) {
        if(resumable) {
            if(!needReset && resumable->inputId() && resumable->inputId() != input.callbackId()) {
                needReset = zftrue;
            }
            resumable->inputId(input.callbackId());
        }
    }
    else {
        if(resumable) {
            resumable = zfnull;
        }
    }
    if(!needReset && resumable) {
        needReset = (zffalse
                || resumable->blockSize() != blockSize
                || resumable->totalSize() != totalSize
                );
    }
    if(resumable) {
        if(needReset) {
            resumable->blockSize(blockSize);
            resumable->totalSize(totalSize);
            resumable->successBlocks(zfobj<ZFHashSet>());
        }
        else if(resumable->successBlocks() == zfnull) {
            resumable->successBlocks(zfobj<ZFHashSet>());
        }
    }

    if(totalSize == zfindexMax() || totalSize <= blockSize || outputOffset == zfindexMax() || inputOffset == zfindexMax()) {
        return _ZFP_zfasyncIO_simple(output, input, finishCallback, progressCallback, resumable, outputOffset, inputOffset, blockSize, totalSize);
    }
    else if(totalSize == 0) {
        finishCallback.execute(ZFArgs()
                .sender(zfnull)
                .param0(zfobj<v_zfindex>(0))
                .param1(resumable)
                );
        return zfnull;
    }
    else {
        return _ZFP_zfasyncIO_split(output, input, finishCallback, progressCallback, resumable, outputOffset, inputOffset, blockSize, totalSize);
    }
}

ZF_NAMESPACE_GLOBAL_END

