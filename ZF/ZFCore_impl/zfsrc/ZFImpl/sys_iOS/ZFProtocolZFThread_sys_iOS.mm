#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

#if ZF_ENV_sys_iOS

// ============================================================
// global type
@interface _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner : NSObject
@property (nonatomic, assign) BOOL cancelFlag;
@property (nonatomic, assign) ZFListener runnable;
@property (nonatomic, assign) ZFListener runnableCleanup;
// private
@property (nonatomic, assign) void *_nativeThreadRegisterToken;
@property (nonatomic, strong) id _selfHolder;
- (void)threadCallback:(id)dummy;
@end
@implementation _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner
- (void)threadCallback:(id)dummy {
    @autoreleasepool {
        if(!self.cancelFlag) {
            self.runnable.execute();
        }
    }

    if(self.runnableCleanup) {
        self.runnableCleanup.execute();
    }
    self._selfHolder = nil;
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global data
typedef void * _ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType;
typedef zfstlmap<_ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType, ZFThread *> _ZFP_ZFThreadImpl_sys_iOS_ThreadMapType;

static _ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType _ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId(void) {
    return (__bridge void *)[NSThread currentThread];
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_sys_iOS_DataHolder, ZFLevelZFFrameworkEssential) {
    mainThread = zfAlloc(ZFThreadMainThread);
    threadMap[_ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId()] = mainThread;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_sys_iOS_DataHolder) {
    zfRelease(mainThread);
    mainThread = zfnull;
}
public:
    ZFThread *mainThread;
    _ZFP_ZFThreadImpl_sys_iOS_ThreadMapType threadMap;
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_sys_iOS_DataHolder)
#define _ZFP_ZFThreadImpl_sys_iOS_mainThreadInstance (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_iOS_DataHolder)->mainThread)
#define _ZFP_ZFThreadImpl_sys_iOS_threadMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_iOS_DataHolder)->threadMap)

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadImpl_sys_iOS, ZFThread, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:NSThread")
public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
    }
    zfoverride
    virtual void protocolOnDealloc(void) {
        zfsuper::protocolOnDealloc();
    }
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType *token = zfnew(_ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType);
        *token = _ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId();
        zfbool exist = (_ZFP_ZFThreadImpl_sys_iOS_threadMap.find(*token) != _ZFP_ZFThreadImpl_sys_iOS_threadMap.end());
        ZFCoreAssertWithMessage(!exist, "thread already registered: %s", ownerZFThread);
        _ZFP_ZFThreadImpl_sys_iOS_threadMap[*token] = ownerZFThread;
        return (void *)token;
    }
    virtual void nativeThreadUnregister(ZF_IN void *token) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_iOS_threadMap.erase(_ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId());
        zfdelete((_ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType *)token);
    }
    virtual ZFThread *threadForToken(ZF_IN void *token) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_iOS_ThreadMapType::iterator it = _ZFP_ZFThreadImpl_sys_iOS_threadMap.find(
            *(_ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType *)token);
        if(it != _ZFP_ZFThreadImpl_sys_iOS_threadMap.end()) {
            return it->second;
        }
        return zfnull;
    }
    virtual ZFThread *mainThread(void) {
        ZFCoreMutexLocker();
        return _ZFP_ZFThreadImpl_sys_iOS_mainThreadInstance;
    }
    virtual ZFThread *currentThread(void) {
        ZFCoreMutexLocker();
        _ZFP_ZFThreadImpl_sys_iOS_ThreadMapType::const_iterator it =
            _ZFP_ZFThreadImpl_sys_iOS_threadMap.find(_ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId());
        if(it == _ZFP_ZFThreadImpl_sys_iOS_threadMap.end()) {
            return zfnull;
        }
        return it->second;
    }

    virtual void *executeInMainThread(ZF_IN const ZFListener &runnable) {
        _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner *threadOwner = [_ZFP_ZFThreadImpl_sys_iOS_ThreadOwner new];
        threadOwner._selfHolder = threadOwner;
        threadOwner.runnable = runnable;
#if 1
        dispatch_async(dispatch_get_main_queue(), ^{
            [threadOwner threadCallback:nil];
        });
#else
        [threadOwner performSelectorOnMainThread:@selector(threadCallback:) withObject:zfnull waitUntilDone:NO];
#endif
        return (__bridge_retained void *)threadOwner;
    }
    virtual void executeInMainThreadCleanup(ZF_IN void *nativeToken) {
        _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner *threadOwner = (__bridge_transfer _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner *)nativeToken;
        threadOwner.cancelFlag = YES;
        threadOwner = nil;
    }

    virtual void *executeInNewThread(
            ZF_IN const ZFListener &runnable
            , ZF_IN const ZFListener &runnableCleanup
            ) {
        _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner *threadOwner = [_ZFP_ZFThreadImpl_sys_iOS_ThreadOwner new];
        threadOwner._selfHolder = threadOwner;
        threadOwner.runnable = runnable;
        threadOwner.runnableCleanup = runnableCleanup;

        [[[NSThread alloc] initWithBlock:^{
            [threadOwner threadCallback:nil];
        }] start];

        return (__bridge_retained void *)threadOwner;
    }
    virtual void executeInNewThreadCleanup(ZF_IN void *nativeToken) {
        _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner *threadOwner = (__bridge_transfer _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner *)nativeToken;
        threadOwner.cancelFlag = YES;
        threadOwner = nil;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

