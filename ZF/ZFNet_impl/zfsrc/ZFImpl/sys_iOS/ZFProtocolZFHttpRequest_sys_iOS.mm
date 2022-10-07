#include "ZFImpl_sys_iOS_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFHttpRequestImpl_sys_iOS_Task : NSObject
@property (nonatomic, assign) ZFHttpRequest *ownerRequest;
@property (nonatomic, assign) ZFHttpResponse *ownerResponse;

@property (nonatomic, strong) NSString *httpMethod;
@property (nonatomic, strong) NSMutableURLRequest *request;
@property (nonatomic, strong) NSMutableData *requestBody;
@property (nonatomic, strong) NSURLSessionDataTask *task;
@property (nonatomic, strong) NSHTTPURLResponse *response;
@end
@implementation _ZFP_ZFHttpRequestImpl_sys_iOS_Task
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFHttpRequestImpl_sys_iOS, ZFHttpRequest, ZFProtocolLevel::e_SystemNormal)
    // ============================================================
    // for request
public:
    virtual void *nativeTaskCreate(ZF_IN ZFHttpRequest *request,
                                   ZF_IN ZFHttpResponse *response)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = [_ZFP_ZFHttpRequestImpl_sys_iOS_Task new];
        task.ownerRequest = request;
        task.ownerResponse = response;
        return (__bridge_retained void *)task;
    }
    virtual void nativeTaskDestroy(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge_transfer _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        NSURLSessionDataTask *taskTmp = task.task;

        task.ownerRequest = zfnull;
        task.ownerResponse = zfnull;
        task.request = nil;
        task.requestBody = nil;
        task.task = nil;
        task.response = nil;
        task = nil;

        [taskTmp cancel];
    }

    virtual void url(ZF_IN void *nativeTask,
                     ZF_IN const zfchar *url)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        task.request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:ZFImpl_sys_iOS_zfstringToNSString(url)]];
        if(task.httpMethod != nil)
        {
            task.request.HTTPMethod = task.httpMethod;
        }
    }

    virtual void httpMethod(ZF_IN void *nativeTask,
                            ZF_IN const zfchar *method)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        task.httpMethod = ZFImpl_sys_iOS_zfstringToNSString(method);
        task.request.HTTPMethod = task.httpMethod;
    }

    virtual void header(ZF_IN void *nativeTask,
                        ZF_IN const zfchar *key,
                        ZF_IN const zfchar *value)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        [task.request setValue:ZFImpl_sys_iOS_zfstringToNSString(value) forHTTPHeaderField:ZFImpl_sys_iOS_zfstringToNSString(key)];
    }

    virtual zfstring header(ZF_IN void *nativeTask,
                            ZF_IN const zfchar *key)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_zfstringFromNSString([task.request valueForHTTPHeaderField:ZFImpl_sys_iOS_zfstringToNSString(key)]);
    }

    virtual zfindex headerCount(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return (zfindex)task.request.allHTTPHeaderFields.count;
    }

    virtual zfiterator headerIter(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_NSDictionaryIter(task.request.allHTTPHeaderFields);
    }
    virtual zfbool headerIterValid(ZF_IN void *nativeTask,
                                   ZF_IN const zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_NSDictionaryIterValid(task.request.allHTTPHeaderFields, it);
    }
    virtual void headerIterNext(ZF_IN void *nativeTask,
                                ZF_IN_OUT zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        ZFImpl_sys_iOS_NSDictionaryIterNext(task.request.allHTTPHeaderFields, it);
    }
    virtual zfstring headerIterKey(ZF_IN void *nativeTask,
                                   ZF_IN const zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_zfstringFromNSString(ZFImpl_sys_iOS_NSDictionaryIterKey(task.request.allHTTPHeaderFields, it));
    }
    virtual zfstring headerIterValue(ZF_IN void *nativeTask,
                                     ZF_IN const zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_zfstringFromNSString(ZFImpl_sys_iOS_NSDictionaryIterValue(task.request.allHTTPHeaderFields, it));
    }
    virtual void headerIterValue(ZF_IN void *nativeTask,
                                 ZF_IN_OUT zfiterator &it,
                                 ZF_IN const zfchar *value)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        ZFImpl_sys_iOS_NSDictionaryIterValue(task.request.allHTTPHeaderFields, it, ZFImpl_sys_iOS_zfstringToNSString(value));
    }
    virtual void headerIterRemove(ZF_IN void *nativeTask,
                                  ZF_IN_OUT zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        ZFImpl_sys_iOS_NSDictionaryIterRemove(task.request.allHTTPHeaderFields, it);
    }

    virtual void body(ZF_IN void *nativeTask,
                      ZF_IN const void *buffer,
                      ZF_IN zfindex byteSize)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        if(task.requestBody == nil)
        {
            task.requestBody = [NSMutableData data];
        }
        [task.requestBody appendBytes:buffer length:byteSize];
    }
    virtual ZFBuffer body(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        ZFBuffer ret;
        ret.bufferChange(task.requestBody.mutableBytes, (zfindex)task.requestBody.length, (zfindex)task.requestBody.length, zffalse);
        return ret;
    }

    virtual void request(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        if(task.requestBody.length > 0)
        {
            task.request.HTTPBody = task.requestBody;
        }
        task.request.timeoutInterval = (NSTimeInterval)task.ownerRequest->timeout() / 1000;

        __weak _ZFP_ZFHttpRequestImpl_sys_iOS_Task *weakTask = task;
        task.task = [[NSURLSession sharedSession] dataTaskWithRequest:task.request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
            if(weakTask.task == nil)
            {
                return;
            }
            if(error != nil)
            {
                weakTask.ownerResponse->success(zffalse);
                weakTask.ownerResponse->code((int)error.code);
                weakTask.ownerResponse->errorHint(ZFImpl_sys_iOS_zfstringFromNSString([NSString stringWithFormat:@"\"%@\"", error.localizedDescription]));
            }
            else
            {
                weakTask.ownerResponse->success(((NSHTTPURLResponse *)response).statusCode == 200);
                weakTask.ownerResponse->code((zfint)(((NSHTTPURLResponse *)response).statusCode));
            }
            weakTask.response = (NSHTTPURLResponse *)response;
            if(data != nil)
            {
                weakTask.ownerResponse->body().bufferCopy(data.bytes, data.length);
            }
            ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(weakTask.ownerRequest);
        }];
        [task.task resume];
    }

    virtual void requestCancel(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        [task.task cancel];
        task.task = nil;
    }

    // ============================================================
    // for response
public:
    virtual zfstring responseHeader(ZF_IN void *nativeTask,
                                    ZF_IN const zfchar *key)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_zfstringFromNSString([task.response.allHeaderFields objectForKey:ZFImpl_sys_iOS_zfstringToNSString(key)]);
    }

    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return (zfindex)task.response.allHeaderFields.count;
    }

    virtual zfiterator responseHeaderIter(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_NSDictionaryIter(task.response.allHeaderFields);
    }
    virtual zfbool responseHeaderIterValid(ZF_IN void *nativeTask,
                                           ZF_IN const zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_NSDictionaryIterValid(task.response.allHeaderFields, it);
    }
    virtual void responseHeaderIterNext(ZF_IN void *nativeTask,
                                        ZF_IN_OUT zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        ZFImpl_sys_iOS_NSDictionaryIterNext(task.response.allHeaderFields, it);
    }
    virtual zfstring responseHeaderIterKey(ZF_IN void *nativeTask,
                                           ZF_IN const zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_zfstringFromNSString(ZFImpl_sys_iOS_NSDictionaryIterKey(task.response.allHeaderFields, it));
    }
    virtual zfstring responseHeaderIterValue(ZF_IN void *nativeTask,
                                             ZF_IN const zfiterator &it)
    {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_zfstringFromNSString(ZFImpl_sys_iOS_NSDictionaryIterValue(task.response.allHeaderFields, it));
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpRequestImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFHttpRequestImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

