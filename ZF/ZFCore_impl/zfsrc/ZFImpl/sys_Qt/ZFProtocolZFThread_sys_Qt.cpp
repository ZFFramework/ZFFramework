#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

#if ZF_ENV_sys_Qt
#include <QThread>
#include <QMutex>
#include <QCoreApplication>
#include <QTimer>

zfclassNotPOD _ZFP_ZFThreadImpl_sys_Qt_ListenerHolder {
public:
    ZFListener runnable;
};

class _ZFP_ZFThreadImpl_sys_Qt_MainThreadHolder : public QObject {
    Q_OBJECT

public:
    _ZFP_ZFThreadImpl_sys_Qt_MainThreadHolder(void)
    : QObject()
    {
        connect(this, SIGNAL(executeInMainThread_signal(void *)), SLOT(executeInMainThread_slot(void *)));
    }

public:
    void executeInMainThread(void *listenerHolder) {
        emit executeInMainThread_signal(listenerHolder);
    }

signals:
    void executeInMainThread_signal(void *listenerHolder);

public slots:
    void executeInMainThread_slot(void *listenerHolder) {
        _ZFP_ZFThreadImpl_sys_Qt_ListenerHolder *listenerHolderTmp = (_ZFP_ZFThreadImpl_sys_Qt_ListenerHolder *)listenerHolder;
        if(listenerHolderTmp->runnable) {
            listenerHolderTmp->runnable.execute();
        }
    }
};

class _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder : public QThread {
    Q_OBJECT

public:
    _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder(void)
    : QThread()
    , _ZFP_nativeThreadRegisterToken(zfnull)
    , _ZFP_runnable()
    , _ZFP_runnableCleanup()
    {
    }

public:
    void *_ZFP_nativeThreadRegisterToken;
    ZFListener _ZFP_runnable;
    ZFListener _ZFP_runnableCleanup;

protected:
    virtual void run() {
        if(this->_ZFP_runnable) {
            if(this->_ZFP_runnable) {
                this->_ZFP_runnable.execute();
            }
        }

        this->_ZFP_runnable = zfnull;
        if(this->_ZFP_runnableCleanup) {
            this->_ZFP_runnableCleanup.execute();
            this->_ZFP_runnableCleanup = zfnull;
        }
        this->deleteLater();
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global data
typedef QThread * _ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType;
typedef zfstlhashmap<_ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType, ZFThread *> _ZFP_ZFThreadImpl_sys_Qt_ThreadMapType;

static _ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType _ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId(void) {
    return QThread::currentThread();
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_sys_Qt_DataHolder, ZFLevelZFFrameworkEssential) {
    mainThread = zfAlloc(ZFThreadMainThread);
    threadMap[_ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId()] = mainThread;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_sys_Qt_DataHolder) {
    zfRelease(mainThread);
    mainThread = zfnull;
}
public:
    ZFThread *mainThread;
    _ZFP_ZFThreadImpl_sys_Qt_ThreadMapType threadMap;
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_sys_Qt_DataHolder)
#define _ZFP_ZFThreadImpl_sys_Qt_mainThreadInstance (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Qt_DataHolder)->mainThread)
#define _ZFP_ZFThreadImpl_sys_Qt_threadMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Qt_DataHolder)->threadMap)

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadImpl_sys_Qt, ZFThread, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QThread")
public:
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType *token = new _ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType();
        *token = _ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId();
        zfbool exist = (_ZFP_ZFThreadImpl_sys_Qt_threadMap.find(*token) != _ZFP_ZFThreadImpl_sys_Qt_threadMap.end());
        ZFCoreAssertWithMessage(!exist, "thread already registered: %s", ownerZFThread);
        _ZFP_ZFThreadImpl_sys_Qt_threadMap[*token] = ownerZFThread;
        return (void *)token;
    }
    virtual void nativeThreadUnregister(ZF_IN void *token) {
        {
            ZFCoreMutexLocker();
            _ZFP_ZFThreadImpl_sys_Qt_threadMap.erase(_ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId());
        }
        delete (_ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType *)token;
    }
    virtual ZFThread *threadForToken(ZF_IN void *token) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_Qt_ThreadMapType::iterator it = _ZFP_ZFThreadImpl_sys_Qt_threadMap.find(
            *(_ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType *)token);
        if(it != _ZFP_ZFThreadImpl_sys_Qt_threadMap.end()) {
            return it->second;
        }
        return zfnull;
    }
    virtual ZFThread *mainThread(void) {
        ZFCoreMutexLocker();
        return _ZFP_ZFThreadImpl_sys_Qt_mainThreadInstance;
    }
    virtual ZFThread *currentThread(void) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_Qt_ThreadMapType::const_iterator it =
            _ZFP_ZFThreadImpl_sys_Qt_threadMap.find(_ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId());
        if(it == _ZFP_ZFThreadImpl_sys_Qt_threadMap.end()) {
            return zfnull;
        }
        return it->second;
    }

    virtual void *executeInMainThread(ZF_IN const ZFListener &runnable) {
        _ZFP_ZFThreadImpl_sys_Qt_ListenerHolder *listenerHolder = zfnew(_ZFP_ZFThreadImpl_sys_Qt_ListenerHolder);
        listenerHolder->runnable = runnable;
        this->_mainThreadHolder.executeInMainThread(listenerHolder);
        return listenerHolder;
    }
    virtual void executeInMainThreadCleanup(ZF_IN void *nativeToken) {
        _ZFP_ZFThreadImpl_sys_Qt_ListenerHolder *listenerHolder = (_ZFP_ZFThreadImpl_sys_Qt_ListenerHolder *)nativeToken;
        listenerHolder->runnable = zfnull;
        zfdelete(listenerHolder);
    }

    virtual void *executeInNewThread(
            ZF_IN const ZFListener &runnable
            , ZF_IN const ZFListener &runnableCleanup
            ) {
        _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder *threadHolder = new _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder();
        threadHolder->_ZFP_runnable = runnable;
        threadHolder->_ZFP_runnableCleanup = runnableCleanup;
        threadHolder->start();
        return threadHolder;
    }
    virtual void executeInNewThreadCleanup(ZF_IN void *nativeToken) {
        _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder *threadHolder = (_ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder *)nativeToken;
        threadHolder->_ZFP_runnable = zfnull;
        // run and cleanup during thread
        // threadHolder->_ZFP_runnableCleanup = zfnull;
    }

private:
    _ZFP_ZFThreadImpl_sys_Qt_MainThreadHolder _mainThreadHolder;
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFThread_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

