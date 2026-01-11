#include "ZFIO_http.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern zfbool _ZFP_ZFIO_http_FindFirst(
        ZF_IN_OUT ZFIOFindData &fd
        , ZF_IN const zfchar *pathData
        );
extern zfbool _ZFP_ZFIO_http_FindNext(ZF_IN_OUT ZFIOFindData &fd);
extern void _ZFP_ZFIO_http_FindClose(ZF_IN_OUT ZFIOFindData &fd);

zfclass _ZFP_I_ZFIOToken_http : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_http, ZFIOToken)

public:
    zfbool open(ZF_IN const zfstring &url) {
        this->ioClose();
        if(!url) {
            return zffalse;
        }
        _url = url;
        return zftrue;
    }

private:
    enum {
        _ChunkSize = 1 * 1024 * 1024,
        _ChunkRetry = 1,
    };
private:
    zfstring _url; // null when open fail or closed
    zfindex _contentLength; // total size of the file, zfindexMax if not available
    zfindex _pos;
    zfstlhashmap<zfindex, zfstring> _chunkCache; // <chunkAlign, buffer>
    ZFCoreArray<zfindex> _chunkCacheIndex; // last accessed at tail

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _contentLength = zfindexMax();
        _pos = 0;
    }

private:
    zfbool _checkLoadContentLength(void) {
        if(_contentLength != zfindexMax()) {
            return zftrue;
        }
        zfautoT<ZFHttpResponse> recv = ZFHttpHeadCache(_url);
        if(recv == zfnull || !recv->success()) {
            return zffalse;
        }
        zfstring sizeText = recv->header("Content-Length");
        if(sizeText.isEmpty()) {
            return zffalse;
        }
        zfindexFromStringT(_contentLength, sizeText);
        return (_contentLength != zfindexMax());
    }
    inline zfindex _chunkAlign(ZF_IN zfindex p) {
        return (p / _ChunkSize) * _ChunkSize;
    }
    zfstring _chunkLoad(ZF_IN zfindex chunkPos) {
        if(chunkPos >= _contentLength) {
            return zfnull;
        }
        {
            ZFCoreMutexLocker();
            zfstlhashmap<zfindex, zfstring>::iterator it = _chunkCache.find(chunkPos);
            if(it != _chunkCache.end()) {
                _chunkCacheIndex.removeElement(chunkPos);
                _chunkCacheIndex.add(chunkPos);
                return it->second;
            }
        }

        zfindex chunkEnd = chunkPos + _ChunkSize;
        if(chunkEnd > _contentLength) {
            chunkEnd = _contentLength;
        }
        for(zfindex iRetry = 0; iRetry <= _ChunkRetry; ++iRetry) {
            zfobj<ZFHttpRequest> send(_url, v_ZFHttpMethod::e_GET);
            send->header("Range", zfstr("bytes=%s-%s", chunkPos, chunkEnd - 1));
            zfautoT<ZFHttpResponse> recv = send->startSync();
            if(recv != zfnull && recv->success() && recv->body().length() == chunkEnd - chunkPos) {
                ZFCoreMutexLocker();
                zfstring ret;
                ret.swap(recv->body());
                _chunkCache[chunkPos] = ret;
                _chunkCacheIndex.removeElement(chunkPos);
                _chunkCacheIndex.add(chunkPos);

                zfindex cacheLimit = ZFThreadPoolForIO::instance()->maxThread() * 2;
                while(_chunkCacheIndex.count() > cacheLimit) {
                    _chunkCache.erase(_chunkCacheIndex.removeAndGet(0));
                }
                return ret;
            }
        }
        return zfnull;
    }

public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_http();
    }
    zfoverride
    virtual zfstring pathData(void) {
        return _url;
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return v_ZFIOOpenOption::e_Read;
    }
public:
    zfoverride
    virtual zfbool ioClose(void) {
        if(!_url) {
            return zftrue;
        }
        this->observerNotify(E_IOCloseOnPrepare());
        _url = zfnull;
        _contentLength = zfindexMax();
        _chunkCache.clear();
        _chunkCacheIndex.removeAll();
        this->observerNotify(E_IOCloseOnFinish());
        return zftrue;
    }
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        if(buf == zfnull || !_checkLoadContentLength()) {
            return 0;
        }
        if(maxByteSize == zfindexMax() || _pos + maxByteSize > _contentLength) {
            maxByteSize = _contentLength - _pos;
        }

        zfindex read = 0;
        zfindex p = _pos;
        zfindex pEnd = _pos + maxByteSize;
        while(p < pEnd) {
            zfindex chunkPos = _chunkAlign(p);
            zfstring chunk = _chunkLoad(chunkPos);
            if(chunk.length() == 0) {
                break;
            }
            zfindex sizeToRead = pEnd > chunkPos + _ChunkSize
                ? (chunkPos + _ChunkSize - p)
                : pEnd - p;
            zfmemcpy((zfbyte *)buf + read, chunk.zfunsafe_buffer() + p - chunkPos, sizeToRead);
            read += sizeToRead;
            p += sizeToRead;
        }
        _pos += read;
        return read;
    }
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {
        return 0;
    }
    zfoverride
    virtual void ioFlush(void) {}
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {
        if(!_checkLoadContentLength()) {
            return zffalse;
        }
        _pos = ZFIOCallbackCalcSeek(0, _contentLength, _pos, byteSize, seekPos);
        return zftrue;
    }
    zfoverride
    virtual zfindex ioTell(void) {
        if(!_checkLoadContentLength()) {
            return zfindexMax();
        }
        return _pos;
    }
    zfoverride
    virtual zfindex ioSize(void) {
        if(!_checkLoadContentLength()) {
            return zfindexMax();
        }
        return _contentLength;
    }
};
zfclass _ZFP_I_ZFIOImpl_http : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_http, ZFIOImpl)
private:
    zfautoT<ZFIOImpl> _refIOImpl(ZF_IN const zfstring &pathData) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return ZFIOImplForPathType(refPathInfo.pathType());
        }
        else {
            return zfnull;
        }
    }
public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_http();
    }

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {
        if(pathData.isEmpty()) {
            return zffalse;
        }
        zfindex len = zfslen(pathData);
        zfautoT<ZFHttpResponse> recv = ZFHttpHeadCache(pathData[len - 1] == '/'
                ? pathData
                : zfstr("%s/", pathData)
                );
        if(recv != zfnull && recv->success()) {
            return zftrue;
        }
        recv = ZFHttpHeadCache(pathData[len - 1] == '/'
                ? zfstring(pathData, len - 1)
                : pathData
                );
        return recv != zfnull && recv->success();
    }
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {
        if(pathData.isEmpty()) {
            return zffalse;
        }
        zfautoT<ZFHttpResponse> recv = ZFHttpHeadCache(pathData[zfslen(pathData) - 1] == '/'
                ? pathData
                : zfstr("%s/", pathData)
                );
        return ZFHttpIsDir(recv);
    }
    zfoverride
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {
        zfindex pos = zfstringFindReversely(pathData, "?");
        if(pos == zfindexMax()) {
            return ioToFileNameDefault(ret, pathData);
        }
        else {
            if(&ret == &pathData) {
                zfstring tmp;
                if(!ioToFileNameDefault(tmp, zfstring(pathData, pos))) {
                    return zffalse;
                }
                ret = tmp;
                return zftrue;
            }
            else {
                return ioToFileNameDefault(ret, zfstring(pathData, pos));
            }
        }
    }
    zfoverride
    virtual zfbool ioToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            ) {
        zfindex pos = zfstringFindReversely(pathData, "?");
        if(pos == zfindexMax()) {
            return ioToChildDefault(ret, pathData, childName);
        }
        else {
            zfstring tmp;
            if(!ioToChildDefault(tmp, zfstring(pathData, pos), childName)) {
                return zffalse;
            }
            tmp += pathData + pos;
            if(&ret == &pathData) {
                ret = tmp;
            }
            else {
                ret += tmp;
            }
            return zftrue;
        }
    }
    zfoverride
    virtual zfbool ioToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {
        zfindex pos = zfstringFindReversely(pathData, "?");
        if(pos == zfindexMax()) {
            return ioToParentDefault(ret, pathData);
        }
        else {
            zfstring tmp;
            if(!ioToParentDefault(tmp, zfstring(pathData, pos))) {
                return zffalse;
            }
            tmp += pathData + pos;
            if(&ret == &pathData) {
                ret = tmp;
            }
            else {
                ret += tmp;
            }
            return zftrue;
        }
    }
    zfoverride
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        return zffalse;
    }
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        return zffalse;
    }
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataTo
            , ZF_IN const zfstring &pathDataFrom
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        return zffalse;
    }
    zfoverride
    virtual zftimet ioModTime(ZF_IN const zfstring &pathData) {
        return zftimetInvalid();
    }
    zfoverride
    virtual zfbool ioModTime(ZF_IN const zfstring &pathData, ZF_IN zftimet time) {
        return zffalse;
    }
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {
        return _ZFP_ZFIO_http_FindFirst(fd, pathData);
    }
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {
        return _ZFP_ZFIO_http_FindNext(fd);
    }
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {
        _ZFP_ZFIO_http_FindClose(fd);
    }
    zfoverride
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        if(flags != v_ZFIOOpenOption::e_Read) {
            return zfnull;
        }
        zfobj<_ZFP_I_ZFIOToken_http> ioToken;
        if(ioToken->open(pathData)) {
            return ioToken;
        }
        else {
            return zfnull;
        }
    }
};
ZFIO_DEFINE(http, _ZFP_I_ZFIOImpl_http)

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
        && ZFRegExpMatch(recv->header("Content-Type"), ".*\\btext/html\\b.*")
        ;
}

ZF_NAMESPACE_GLOBAL_END

