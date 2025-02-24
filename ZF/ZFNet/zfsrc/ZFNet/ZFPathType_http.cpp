#include "ZFPathType_http.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(http)

extern zfbool _ZFP_ZFPathType_http_FindFirst(
        ZF_IN_OUT ZFFileFindData &fd
        , ZF_IN const zfchar *pathData
        );
extern zfbool _ZFP_ZFPathType_http_FindNext(ZF_IN_OUT ZFFileFindData &fd);
extern void _ZFP_ZFPathType_http_FindClose(ZF_IN_OUT ZFFileFindData &fd);

// ============================================================
// http
zfclassNotPOD _ZFP_ZFPathType_http {
public:
    zfclassNotPOD _Token {
    public:
        enum {
            ChunkSize = 1 * 1024 * 1024,
            ChunkCacheCount = 4,
            ChunkRetry = 1,
        };

    public:
        zfbool updated;
        zfstring url;
        zfindex contentLength; // total size of the file, zfindexMax if not available
        zfindex curPos;
        zfstlmap<zfindex, zfstring> chunkCache; // <chunkAlign, buffer>
        ZFCoreArray<zfindex> chunkCacheFIFO;
    public:
        _Token(void)
        : updated(zffalse)
        , url()
        , contentLength(zfindexMax())
        , curPos(0)
        , chunkCache()
        , chunkCacheFIFO()
        {
        }
    public:
        void update(void) {
            if(!this->updated) {
                zfautoT<ZFHttpResponse> recv = ZFHttpHeadCache(this->url);
                this->updated = zftrue;
                if(recv == zfnull || !recv->success()) {
                    return;
                }
                zfstring sizeText = recv->header("Content-Length");
                if(sizeText.isEmpty()) {
                    return;
                }
                zfindexFromStringT(this->contentLength, sizeText);
            }
        }
        inline zfindex chunkAlign(ZF_IN zfindex p) {
            return (p / ChunkSize) * ChunkSize;
        }
        zfstring chunkLoad(ZF_IN zfindex chunkPos) {
            if(chunkPos >= contentLength) {
                return zfnull;
            }
            {
                ZFCoreMutexLocker();
                zfstlmap<zfindex, zfstring>::iterator it = chunkCache.find(chunkPos);
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
                zfobj<ZFHttpRequest> send(url, ZFHttpMethod::e_GET);
                send->header("Range", zfstr("bytes=%s-%s", chunkPos, chunkEnd - 1));
                zfautoT<ZFHttpResponse> recv = send->requestSync();
                if(recv != zfnull && recv->success() && recv->body().length() == chunkEnd - chunkPos) {
                    ZFCoreMutexLocker();
                    zfstring ret;
                    ret.swap(recv->body());
                    chunkCache[chunkPos] = ret;
                    chunkCacheFIFO.removeElement(chunkPos);
                    chunkCacheFIFO.add(chunkPos);

                    while(chunkCacheFIFO.count() > ChunkCacheCount) {
                        chunkCache.erase(chunkCacheFIFO.removeAndGet(0));
                    }
                    return ret;
                }
            }
            return zfnull;
        }
    };

public:
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData) {
        if(zfstringIsEmpty(pathData)) {
            return zffalse;
        }
        zfindex len = zfslen(pathData);
        zfautoT<ZFHttpResponse> recv = ZFHttpHeadCache(pathData[len - 1] == '/'
                ? pathData
                : zfstr("%s/", pathData).cString()
                );
        if(recv != zfnull && recv->success()) {
            return zftrue;
        }
        recv = ZFHttpHeadCache(pathData[len - 1] == '/'
                ? zfstring(pathData, len - 1).cString()
                : pathData
                );
        return recv != zfnull && recv->success();
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData) {
        if(zfstringIsEmpty(pathData)) {
            return zftrue;
        }
        zfautoT<ZFHttpResponse> recv = ZFHttpHeadCache(pathData[zfslen(pathData) - 1] == '/'
                ? pathData
                : zfstr("%s/", pathData).cString()
                );
        return recv->success() && zfstringIsEqual(recv->header("Content-Type"), "text/html");
    }
    static zfstring callbackToFileName(
            ZF_IN const zfchar *pathData
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        zfstring ret = ZFPathInfoCallbackToFileNameDefault(pathData, success);
        zfindex pos = zfstringFindReversely(ret, "?");
        if(pos != zfindexMax()) {
            ret.remove(pos);
        }
        if(success) {
            *success = zftrue;
        }
        return ret;
    }
    static zfstring callbackToChild(
            ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        return ZFPathInfoCallbackToChildDefault(pathData, childName, success);
    }
    static zfstring callbackToParent(
            ZF_IN const zfchar *pathData
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        return ZFPathInfoCallbackToParentDefault(pathData, success);
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
    static zfbool callbackMove(
            ZF_IN const zfchar *pathDataFrom
            , ZF_IN const zfchar *pathDataTo
            , ZF_IN_OPT zfbool isForce
            ) {
        return zffalse;
    }
    static zfbool callbackFindFirst(
            ZF_IN_OUT ZFFileFindData &fd
            , ZF_IN const zfchar *pathData
            ) {
        return _ZFP_ZFPathType_http_FindFirst(fd, pathData);
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd) {
        return _ZFP_ZFPathType_http_FindNext(fd);
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd) {
        _ZFP_ZFPathType_http_FindClose(fd);
    }
    static void *callbackOpen(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT ZFFileOpenOptionFlags flag
            , ZF_IN_OPT zfbool autoCreateParent
            ) {
        if(flag != ZFFileOpenOption::e_Read) {
            return zfnull;
        }
        _Token *d = zfnew(_Token);
        d->url = pathData;
        return d;
    }
    static zfbool callbackClose(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        zfdelete(d);
        return zftrue;
    }
    static zfindex callbackTell(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        d->update();
        return d->curPos;
    }
    static zfbool callbackSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos position
            ) {
        _Token *d = (_Token *)token;
        d->update();
        if(d->contentLength == zfindexMax()) {
            return zffalse;
        }
        d->curPos = ZFIOCallbackCalcFSeek(0, d->contentLength, d->curPos, byteSize, position);
        return zftrue;
    }
    static zfindex callbackRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        _Token *d = (_Token *)token;
        d->update();
        if(d->contentLength == zfindexMax()) {
            return 0;
        }
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
            zfstring chunk = d->chunkLoad(chunkPos);
            if(chunk.length() == 0) {
                break;
            }
            zfindex sizeToRead = pEnd > chunkPos + _Token::ChunkSize
                ? (chunkPos + _Token::ChunkSize - p)
                : pEnd - p;
            zfmemcpy((zfbyte *)buf + read, chunk.zfunsafe_buffer() + p - chunkPos, sizeToRead);
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
        d->update();
        return d->curPos >= d->contentLength;
    }
    static zfbool callbackIsError(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        d->update();
        return d->contentLength == zfindexMax();
    }
    static zfindex callbackSize(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        d->update();
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
        , _ZFP_ZFPathType_http::callbackMove
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
        , ZFMP_IN(const zfstring &, url)
        )

ZF_NAMESPACE_GLOBAL_END

