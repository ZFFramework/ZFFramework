#include "ZFPathType_http.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

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
        zfstlhashmap<zfindex, zfstring> chunkCache; // <chunkAlign, buffer>
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
                zfstlhashmap<zfindex, zfstring>::iterator it = chunkCache.find(chunkPos);
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
                zfobj<ZFHttpRequest> send(url, v_ZFHttpMethod::e_GET);
                send->header("Range", zfstr("bytes=%s-%s", chunkPos, chunkEnd - 1));
                zfautoT<ZFHttpResponse> recv = send->startSync();
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
        return ZFHttpIsDir(recv);
    }
    static zfbool callbackToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        zfindex pos = zfstringFindReversely(pathData, "?");
        if(pos == zfindexMax()) {
            return ZFPathInfoCallbackToFileNameDefault(ret, pathData);
        }
        else {
            if(pathData >= ret.cString() && pathData < ret.cString() + ret.length()) {
                zfstring tmp;
                if(!ZFPathInfoCallbackToFileNameDefault(tmp, zfstring(pathData, pos))) {
                    return zffalse;
                }
                ret = tmp;
                return zftrue;
            }
            else {
                return ZFPathInfoCallbackToFileNameDefault(ret, zfstring(pathData, pos));
            }
        }
    }
    static zfbool callbackToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            ) {
        zfindex pos = zfstringFindReversely(pathData, "?");
        if(pos == zfindexMax()) {
            return ZFPathInfoCallbackToChildDefault(ret, pathData, childName);
        }
        else {
            zfstring tmp;
            if(!ZFPathInfoCallbackToChildDefault(tmp, zfstring(pathData, pos), childName)) {
                return zffalse;
            }
            tmp += pathData + pos;
            if(pathData >= ret.cString() && pathData < ret.cString() + ret.length()) {
                ret = tmp;
            }
            else {
                ret += tmp;
            }
            return zftrue;
        }
    }
    static zfbool callbackToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        zfindex pos = zfstringFindReversely(pathData, "?");
        if(pos == zfindexMax()) {
            return ZFPathInfoCallbackToParentDefault(ret, pathData);
        }
        else {
            zfstring tmp;
            if(!ZFPathInfoCallbackToParentDefault(tmp, zfstring(pathData, pos))) {
                return zffalse;
            }
            tmp += pathData + pos;
            if(pathData >= ret.cString() && pathData < ret.cString() + ret.length()) {
                ret = tmp;
            }
            else {
                ret += tmp;
            }
            return zftrue;
        }
    }
    static zfbool callbackPathCreate(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool autoMakeParent
            ) {
        return zffalse;
    }
    static zfbool callbackRemove(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool isRecursive
            , ZF_IN_OPT zfbool isForce
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
        if(flag != v_ZFFileOpenOption::e_Read) {
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
    static zfindex callbackSize(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        d->update();
        return d->contentLength;
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
        d->curPos = ZFIOCallbackCalcSeek(0, d->contentLength, d->curPos, byteSize, position);
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
        , _ZFP_ZFPathType_http::callbackSize
        , _ZFP_ZFPathType_http::callbackTell
        , _ZFP_ZFPathType_http::callbackSeek
        , _ZFP_ZFPathType_http::callbackRead
        , _ZFP_ZFPathType_http::callbackWrite
    )

// ============================================================
// ZFInputForHttp
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForHttp
        , ZFMP_IN(const zfstring &, url)
        )

ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFHttpIsDir
        , ZFMP_IN(ZFHttpResponse *, recv)
        ) {
    return recv
        && recv->success()
        && ZFRegExpFind(recv->header("Content-Type"), ".*\\btext/html\\b.*") != ZFIndexRangeMax()
        ;
}

ZF_NAMESPACE_GLOBAL_END

