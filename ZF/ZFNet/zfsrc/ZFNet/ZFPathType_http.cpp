#include "ZFPathType_http.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(http)

// ============================================================
// http
zfclassNotPOD _ZFP_ZFPathType_http {
public:
    zfclassNotPOD _Token {
    public:
        enum {
            ChunkSize = 65536,
            ChunkCacheCount = 8,
            ChunkRetry = 1,
        };

    public:
        zfstring url;
        zfindex contentLength; // total size of the file, zfindexMax if not available
        zfindex curPos;
        zfstlmap<zfindex, ZFBuffer> chunkCache; // <chunkAlign, buffer>
        ZFCoreArrayPOD<zfindex> chunkCacheFIFO;
    public:
        _Token(void)
        : url()
        , contentLength(zfindexMax())
        , curPos(0)
        , chunkCache()
        , chunkCacheFIFO()
        {
        }
    public:
        inline zfindex chunkAlign(ZF_IN zfindex p) {
            return (p / ChunkSize) * ChunkSize;
        }
        ZFBuffer chunkLoad(ZF_IN zfindex chunkPos) {
            if(chunkPos >= contentLength) {
                return ZFBuffer();
            }
            {
                zfCoreMutexLocker();
                zfstlmap<zfindex, ZFBuffer>::iterator it = chunkCache.find(chunkPos);
                if(it != chunkCache.end()) {
                    chunkCacheFIFO.removeElement(chunkPos);
                    chunkCacheFIFO.add(chunkPos);
                    return it->second;
                }
            }

            zfindex chunkEnd = chunkPos + ChunkSize;
            if(chunkEnd > contentLength) {
                chunkEnd = contentLength;
            }
            for(zfindex iRetry = 0; iRetry <= ChunkRetry; ++iRetry) {
                zfblockedAlloc(ZFHttpRequest, send, url, ZFHttpMethod::e_GET);
                send->header("Range", zfstr("bytes=%zi-%zi", chunkPos, chunkEnd - 1));
                zfautoObjectT<ZFHttpResponse *> recv = send->requestSync();
                if(recv != zfnull && recv->success() && recv->body().bufferSize() == chunkEnd - chunkPos) {
                    zfCoreMutexLocker();
                    ZFBuffer ret;
                    ret.bufferSwap(recv->body());
                    chunkCache[chunkPos] = ret;
                    chunkCacheFIFO.removeElement(chunkPos);
                    chunkCacheFIFO.add(chunkPos);

                    while(chunkCacheFIFO.count() > ChunkCacheCount) {
                        chunkCache.erase(chunkCacheFIFO.removeAndGet(0));
                    }
                    return ret;
                }
            }
            return ZFBuffer();
        }
    };

public:
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData) {
        zfblockedAlloc(ZFHttpRequest, send, pathData, ZFHttpMethod::e_HEAD);
        zfautoObjectT<ZFHttpResponse *> recv = send->requestSync();
        return recv != zfnull && recv->success();
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData) {
        return zffalse;
    }
    static zfbool callbackToFileName(
            ZF_IN const zfchar *pathData
            , ZF_IN_OUT zfstring &fileName
            ) {
        return ZFPathInfoCallbackToFileNameDefault(pathData, fileName);
    }
    static zfbool callbackToChild(
            ZF_IN const zfchar *pathData
            , ZF_IN_OUT zfstring &pathDataChild
            , ZF_IN const zfchar *childName
            ) {
        return ZFPathInfoCallbackToChildDefault(pathData, pathDataChild, childName);
    }
    static zfbool callbackToParent(
            ZF_IN const zfchar *pathData
            , ZF_IN_OUT zfstring &pathDataParent
            ) {
        return ZFPathInfoCallbackToParentDefault(pathData, pathDataParent);
    }
    static zfbool callbackPathCreate(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool autoMakeParent
            , ZF_OUT_OPT zfstring *errPos
            ) {
        return zffalse;
    }
    static zfbool callbackRemove(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool isRecursive
            , ZF_IN_OPT zfbool isForce
            , ZF_IN_OPT zfstring *errPos
            ) {
        return zffalse;
    }
    static zfbool callbackFindFirst(
            ZF_IN_OUT ZFFileFindData &fd
            , ZF_IN const zfchar *pathData
            ) {
        return zffalse;
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd) {
        return zffalse;
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd) {
    }
    static void *callbackOpen(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT ZFFileOpenOptionFlags flag
            , ZF_IN_OPT zfbool autoCreateParent
            ) {
        if(flag != ZFFileOpenOption::e_Read) {
            return zfnull;
        }
        zfblockedAlloc(ZFHttpRequest, send, pathData, ZFHttpMethod::e_HEAD);
        zfautoObjectT<ZFHttpResponse *> recv = send->requestSync();
        if(recv == zfnull || !recv->success()) {
            return zfnull;
        }
        zfstring sizeText = recv->header("Content-Length");
        if(sizeText.isEmpty()) {
            return zfnull;
        }
        zfindex contentLength;
        if(!zfindexFromString(contentLength, sizeText) || contentLength == zfindexMax()) {
            return zfnull;
        }

        _Token *d = zfnew(_Token);
        d->url = pathData;
        d->contentLength = contentLength;
        return d;
    }
    static zfbool callbackClose(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        zfdelete(d);
        return zftrue;
    }
    static zfindex callbackTell(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        return d->curPos;
    }
    static zfbool callbackSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos position
            ) {
        _Token *d = (_Token *)token;
        d->curPos = ZFIOCallbackCalcFSeek(0, d->contentLength, d->curPos, byteSize, position);
        return zftrue;
    }
    static zfindex callbackRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        _Token *d = (_Token *)token;
        if(maxByteSize == zfindexMax() || d->curPos + maxByteSize > d->contentLength) {
            maxByteSize = d->contentLength - d->curPos;
        }
        if(buf == zfnull) {
            return maxByteSize;
        }

        zfindex read = 0;
        zfindex p = d->curPos;
        zfindex pEnd = d->curPos + maxByteSize;
        while(p < pEnd) {
            zfindex chunkPos = d->chunkAlign(p);
            ZFBuffer chunk = d->chunkLoad(chunkPos);
            if(chunk.bufferSize() == 0) {
                break;
            }
            zfindex sizeToRead = pEnd > chunkPos + _Token::ChunkSize
                ? (chunkPos + _Token::ChunkSize - p)
                : pEnd - p;
            zfmemcpy((zfbyte *)buf + read, chunk.bufferT<zfbyte *>() + p - chunkPos, sizeToRead);
            read += sizeToRead;
            p += sizeToRead;
        }
        d->curPos += read;
        return read;
    }
    static zfindex callbackWrite(
            ZF_IN void *token
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize
            ) {
        return 0;
    }
    static void callbackFlush(ZF_IN void *token) {
    }
    static zfbool callbackIsEof(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        return d->curPos >= d->contentLength;
    }
    static zfbool callbackIsError(ZF_IN void *token) {
        return zffalse;
    }
    static zfindex callbackSize(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        return d->contentLength;
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
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForHttp
        , ZFMP_IN(const zfchar *, url)
        )

ZF_NAMESPACE_GLOBAL_END

