#include "ZFPathType_http.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(http)

// ============================================================
// http
zfclassNotPOD _ZFP_ZFPathType_http
{
public:
    zfclassNotPOD _Token
    {
    public:
        ZFBuffer buffer;
        zfindex curPos;
    public:
        _Token(void)
        : buffer()
        , curPos(0)
        {
        }
    };

public:
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData)
    {
        zfblockedAlloc(ZFHttpRequest, send, pathData, "HEAD");
        zfautoObjectT<ZFHttpResponse *> recv = send->requestSync();
        return recv != zfnull && recv->success();
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData)
    {
        return zffalse;
    }
    static zfbool callbackToFileName(ZF_IN const zfchar *pathData,
                                     ZF_IN_OUT zfstring &fileName)
    {
        return ZFFilePathInfoCallbackToFileNameDefault(pathData, fileName);
    }
    static zfbool callbackToChild(ZF_IN const zfchar *pathData,
                                  ZF_IN_OUT zfstring &pathDataChild,
                                  ZF_IN const zfchar *childName)
    {
        return ZFFilePathInfoCallbackToChildDefault(pathData, pathDataChild, childName);
    }
    static zfbool callbackToParent(ZF_IN const zfchar *pathData,
                                   ZF_IN_OUT zfstring &pathDataParent)
    {
        return ZFFilePathInfoCallbackToParentDefault(pathData, pathDataParent);
    }
    static zfbool callbackPathCreate(ZF_IN const zfchar *pathData,
                                     ZF_IN_OPT zfbool autoMakeParent,
                                     ZF_OUT_OPT zfstring *errPos)
    {
        return zffalse;
    }
    static zfbool callbackRemove(ZF_IN const zfchar *pathData,
                                 ZF_IN_OPT zfbool isRecursive,
                                 ZF_IN_OPT zfbool isForce,
                                 ZF_IN_OPT zfstring *errPos)
    {
        return zffalse;
    }
    static zfbool callbackFindFirst(ZF_IN_OUT ZFFileFindData &fd,
                                    ZF_IN const zfchar *pathData)
    {
        return zffalse;
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd)
    {
        return zffalse;
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd)
    {
    }
    static void *callbackOpen(ZF_IN const zfchar *pathData,
                              ZF_IN_OPT ZFFileOpenOptionFlags flag,
                              ZF_IN_OPT zfbool autoCreateParent)
    {
        if(flag != ZFFileOpenOption::e_Read)
        {
            return zfnull;
        }
        zfblockedAlloc(ZFHttpRequest, send, pathData, "GET");
        zfautoObjectT<ZFHttpResponse *> recv = send->requestSync();
        if(recv == zfnull || !recv->success())
        {
            return zfnull;
        }
        _Token *d = zfnew(_Token);
        d->buffer = recv->body();
        return d;
    }
    static zfbool callbackClose(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        zfdelete(d);
        return zftrue;
    }
    static zfindex callbackTell(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        return d->curPos;
    }
    static zfbool callbackSeek(ZF_IN void *token,
                               ZF_IN zfindex byteSize,
                               ZF_IN_OPT ZFSeekPos position)
    {
        _Token *d = (_Token *)token;
        d->curPos = ZFIOCallbackCalcFSeek(0, d->buffer.bufferSize(), d->curPos, byteSize, position);
        return zftrue;
    }
    static zfindex callbackRead(ZF_IN void *token,
                                ZF_IN void *buf,
                                ZF_IN zfindex maxByteSize)
    {
        _Token *d = (_Token *)token;
        if(maxByteSize == zfindexMax() || d->curPos + maxByteSize > d->buffer.bufferSize())
        {
            maxByteSize = d->buffer.bufferSize() - d->curPos;
        }
        zfmemcpy(buf, d->buffer.bufferT<const zfbyte *>() + d->curPos, maxByteSize);
        return maxByteSize;
    }
    static zfindex callbackWrite(ZF_IN void *token,
                                 ZF_IN const void *src,
                                 ZF_IN_OPT zfindex maxByteSize)
    {
        return 0;
    }
    static void callbackFlush(ZF_IN void *token)
    {
    }
    static zfbool callbackIsEof(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        return d->curPos >= d->buffer.bufferSize();
    }
    static zfbool callbackIsError(ZF_IN void *token)
    {
        return zffalse;
    }
    static zfindex callbackSize(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        return d->buffer.bufferSize() - d->curPos;
    }
};
ZFPATHTYPE_FILEIO_REGISTER(http, ZFPathType_http()
        , _ZFP_ZFPathType_http::callbackIsExist
        , _ZFP_ZFPathType_http::callbackIsDir
        , _ZFP_ZFPathType_http::callbackToFileName
        , _ZFP_ZFPathType_http::callbackToChild
        , _ZFP_ZFPathType_http::callbackToParent
        , _ZFP_ZFPathType_http::callbackPathCreate
        , _ZFP_ZFPathType_http::callbackRemove
        , _ZFP_ZFPathType_http::callbackFindFirst
        , _ZFP_ZFPathType_http::callbackFindNext
        , _ZFP_ZFPathType_http::callbackFindClose
        , _ZFP_ZFPathType_http::callbackOpen
        , _ZFP_ZFPathType_http::callbackClose
        , _ZFP_ZFPathType_http::callbackTell
        , _ZFP_ZFPathType_http::callbackSeek
        , _ZFP_ZFPathType_http::callbackRead
        , _ZFP_ZFPathType_http::callbackWrite
        , _ZFP_ZFPathType_http::callbackFlush
        , _ZFP_ZFPathType_http::callbackIsEof
        , _ZFP_ZFPathType_http::callbackIsError
        , _ZFP_ZFPathType_http::callbackSize
    )

// ============================================================
// ZFInputForHttp
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForHttp,
                              ZFMP_IN(const zfchar *, url))

ZF_NAMESPACE_GLOBAL_END

