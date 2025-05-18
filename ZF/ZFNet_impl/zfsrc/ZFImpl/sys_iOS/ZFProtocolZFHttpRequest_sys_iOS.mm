#include "ZFImpl_sys_iOS_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFHttpRequestImpl_sys_iOS_Task : NSObject
@property (nonatomic, assign) ZFHttpRequest *ownerRequest;
@property (nonatomic, assign) zfautoT<ZFHttpResponse> ownerResponse;

@property (nonatomic, strong) NSString *httpMethod;
@property (nonatomic, strong) NSMutableURLRequest *request;
@property (nonatomic, strong) NSMutableData *requestBody;
@property (nonatomic, strong) NSURLSessionDataTask *task;
@property (nonatomic, strong) NSHTTPURLResponse *response;
@property (nonatomic, strong) NSData *responseData;
@end
@implementation _ZFP_ZFHttpRequestImpl_sys_iOS_Task
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFHttpRequestImpl_sys_iOS, ZFHttpRequest, v_ZFProtocolLevel::e_SystemNormal)
    // ============================================================
    // for request
public:
    virtual zfbool httpsAvailable(void) {
        return zftrue;
    }

    virtual void *nativeTaskCreate(ZF_IN ZFHttpRequest *request) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = [_ZFP_ZFHttpRequestImpl_sys_iOS_Task new];
        task.ownerRequest = request;
        return (__bridge_retained void *)task;
    }
    virtual void nativeTaskDestroy(ZF_IN void *nativeTask) {
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

    virtual void url(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &url
            ) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        task.request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:ZFImpl_sys_iOS_zfstringToNSString(url)]];
        if(task.httpMethod != nil) {
            task.request.HTTPMethod = task.httpMethod;
        }
    }

    virtual void httpMethod(
            ZF_IN void *nativeTask
            , ZF_IN ZFHttpMethod httpMethod
            ) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        task.httpMethod = ZFImpl_sys_iOS_zfstringToNSString(ZFHttpMethodToString(httpMethod));
        task.request.HTTPMethod = task.httpMethod;
    }

    virtual void header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            , ZF_IN const zfstring &value
            ) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        [task.request addValue:ZFImpl_sys_iOS_zfstringToNSString(value) forHTTPHeaderField:ZFImpl_sys_iOS_zfstringToNSString(key)];
    }

    virtual void headerRemove(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        [task.request setValue:nil forHTTPHeaderField:ZFImpl_sys_iOS_zfstringToNSString(key)];
    }

    virtual zfstring header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_zfstringFromNSString([task.request valueForHTTPHeaderField:ZFImpl_sys_iOS_zfstringToNSString(key)]);
    }

    virtual zfindex headerCount(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return (zfindex)task.request.allHTTPHeaderFields.count;
    }

    virtual zfiter headerIter(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_NSDictionaryIter(task.request.allHTTPHeaderFields);
    }
    virtual zfstring headerIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return ZFImpl_sys_iOS_zfstringFromNSString(ZFImpl_sys_iOS_NSDictionaryIterKey(it));
    }
    virtual zfstring headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return ZFImpl_sys_iOS_zfstringFromNSString(ZFImpl_sys_iOS_NSDictionaryIterValue(it));
    }
    virtual void headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            , ZF_IN const zfstring &value
            ) {
        ZFImpl_sys_iOS_NSDictionaryIterValue(it, ZFImpl_sys_iOS_zfstringToNSString(value));
    }
    virtual void headerIterRemove(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            ) {
        ZFImpl_sys_iOS_NSDictionaryIterRemove(it);
    }

    virtual void body(
            ZF_IN void *nativeTask
            , ZF_IN const void *buffer
            , ZF_IN zfindex byteSize
            ) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        if(task.requestBody == nil) {
            task.requestBody = [NSMutableData data];
        }
        [task.requestBody appendBytes:buffer length:byteSize];
    }
    virtual zfstring body(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return zfstring::shared(task.requestBody.mutableBytes, (zfindex)task.requestBody.length);
    }

    virtual void request(ZF_IN void *nativeTask, ZF_IN ZFHttpResponse *response) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        task.ownerResponse = response;
        if(task.requestBody.length > 0) {
            task.request.HTTPBody = task.requestBody;
        }
        task.request.timeoutInterval = (NSTimeInterval)task.ownerRequest->timeout() / 1000;

        __weak _ZFP_ZFHttpRequestImpl_sys_iOS_Task *weakTask = task;
        task.task = [[NSURLSession sharedSession] dataTaskWithRequest:task.request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
            if(weakTask.task == nil) {
                return;
            }
            if(error != nil) {
                weakTask.ownerResponse->success(zffalse);
                weakTask.ownerResponse->code((int)error.code);
                weakTask.ownerResponse->errorHint(ZFImpl_sys_iOS_zfstringFromNSString([NSString stringWithFormat:@"\"%@\"", error.localizedDescription]));
            }
            else {
                NSInteger code = ((NSHTTPURLResponse *)response).statusCode;
                weakTask.ownerResponse->success(code >= 200 && code < 300);
                weakTask.ownerResponse->code((zfint)(((NSHTTPURLResponse *)response).statusCode));
            }
            weakTask.response = (NSHTTPURLResponse *)response;
            if(data != nil) {
                weakTask.responseData = data;
                weakTask.ownerResponse->body(zfstring::shared(data.bytes, data.length));
            }
            ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(weakTask.ownerRequest, weakTask.ownerResponse);
        }];
        [task.task resume];
    }

    virtual void requestCancel(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        [task.task cancel];
        task.task = nil;
    }

    // ============================================================
    // for response
public:
    virtual zfstring responseHeader(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_zfstringFromNSString([task.response valueForHTTPHeaderField:ZFImpl_sys_iOS_zfstringToNSString(key)]);
    }

    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return (zfindex)task.response.allHeaderFields.count;
    }

    virtual zfiter responseHeaderIter(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_iOS_Task *task = (__bridge _ZFP_ZFHttpRequestImpl_sys_iOS_Task *)nativeTask;
        return ZFImpl_sys_iOS_NSDictionaryIter(task.response.allHeaderFields);
    }
    virtual zfstring responseHeaderIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return ZFImpl_sys_iOS_zfstringFromNSString(ZFImpl_sys_iOS_NSDictionaryIterKey(it));
    }
    virtual zfstring responseHeaderIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        return ZFImpl_sys_iOS_zfstringFromNSString(ZFImpl_sys_iOS_NSDictionaryIterValue(it));
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpRequestImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

