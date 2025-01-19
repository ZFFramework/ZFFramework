#include "ZFPathType_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(modulePath)
ZFPATHTYPE_DEFINE(storagePath)
ZFPATHTYPE_DEFINE(storageSharedPath)
ZFPATHTYPE_DEFINE(cachePath)
ZFPATHTYPE_DEFINE(text)

// ============================================================
#define _ZFP_ZFPathType_common_DEFINE(registerSig, pathType, pathPrefixFunc) \
    zfclassNotPOD _ZFP_ZFPathType_##registerSig { \
    public: \
        static void pathConvert( \
                ZF_OUT zfstring &ret \
                , ZF_IN const zfchar *pathData \
                ) { \
            ret += pathPrefixFunc(); \
            ret += '/'; \
            ret += pathData; \
        } \
        static void pathRevert(ZF_IN_OUT zfstring &pathData) { \
            const zfchar *prefix = pathPrefixFunc(); \
            zfindex prefixLen = zfslen(prefix); \
            if(zfsncmp(pathData, prefix, prefixLen) == 0 \
                    && pathData[prefixLen] == '/' \
                    ) { \
                pathData.remove(0, prefixLen + 1); \
            } \
        } \
    }; \
    ZFPATHTYPE_FILEIO_REGISTER(registerSig, pathType \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackIsExist \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackIsDir \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackToFileName \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackToChild \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackToParent \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackPathCreate \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackRemove \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackMove \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackFindFirst \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackFindNext \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackFindClose \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackOpen \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackClose \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackTell \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackSeek \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackRead \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackWrite \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackFlush \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackIsEof \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackIsError \
            , ZFFileIOImpl::FileIO<_ZFP_ZFPathType_##registerSig>::callbackSize \
        )

_ZFP_ZFPathType_common_DEFINE(modulePath, ZFPathType_modulePath(), ZFPathForModule)
_ZFP_ZFPathType_common_DEFINE(settingPath, ZFPathType_settingPath(), ZFPathForSetting)
_ZFP_ZFPathType_common_DEFINE(storagePath, ZFPathType_storagePath(), ZFPathForStorage)
_ZFP_ZFPathType_common_DEFINE(storageSharedPath, ZFPathType_storageSharedPath(), ZFPathForStorageShared)
_ZFP_ZFPathType_common_DEFINE(cachePath, ZFPathType_cachePath(), ZFPathForCache)

// ============================================================
// text
zfclassNotPOD _ZFP_ZFPathType_text {
public:
    zfclassNotPOD _Token {
    public:
        zfbyte *buf;
        zfindex bufSize;
        zfindex pos;
    public:
        _Token(void) : buf(zfnull), bufSize(0), pos(0) {}
        ~_Token(void) {
            zffree(this->buf);
        }
    };
public:
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData) {
        return zffalse;
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData) {
        return zffalse;
    }
    static zfstring callbackToFileName(
            ZF_IN const zfchar *pathData
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        if(success) {
            *success = zffalse;
        }
        return zfnull;
    }
    static zfstring callbackToChild(
            ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        if(success) {
            *success = zffalse;
        }
        return zfnull;
    }
    static zfstring callbackToParent(
            ZF_IN const zfchar *pathData
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        if(success) {
            *success = zffalse;
        }
        return zfnull;
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
        _Token *d = zfnew(_Token);
        d->bufSize = zfslen(pathData);
        d->buf = (zfbyte *)zfmalloc(d->bufSize);
        zfmemcpy(d->buf, pathData, d->bufSize);
        return d;
    }
    static zfbool callbackClose(ZF_IN void *token) {
        zfdelete((_Token *)token);
        return zftrue;
    }
    static zfindex callbackTell(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        return d->pos;
    }
    static zfbool callbackSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos position
            ) {
        _Token *d = (_Token *)token;
        d->pos = ZFIOCallbackCalcFSeek(0, d->bufSize, d->pos, byteSize, position);
        return zftrue;
    }
    static zfindex callbackRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        _Token *d = (_Token *)token;
        if(maxByteSize > d->bufSize - d->pos) {
            maxByteSize = d->bufSize - d->pos;
        }
        if(buf != zfnull) {
            zfmemcpy(buf, d->buf + d->pos, maxByteSize);
        }
        return maxByteSize;
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
        return (d->pos >= d->bufSize);
    }
    static zfbool callbackIsError(ZF_IN void *token) {
        return zffalse;
    }
    static zfindex callbackSize(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        return d->bufSize;
    }
};
ZFPATHTYPE_FILEIO_REGISTER(text, ZFPathType_text()
        , _ZFP_ZFPathType_text::callbackIsExist
        , _ZFP_ZFPathType_text::callbackIsDir
        , _ZFP_ZFPathType_text::callbackToFileName
        , _ZFP_ZFPathType_text::callbackToChild
        , _ZFP_ZFPathType_text::callbackToParent
        , _ZFP_ZFPathType_text::callbackPathCreate
        , _ZFP_ZFPathType_text::callbackRemove
        , _ZFP_ZFPathType_text::callbackMove
        , _ZFP_ZFPathType_text::callbackFindFirst
        , _ZFP_ZFPathType_text::callbackFindNext
        , _ZFP_ZFPathType_text::callbackFindClose
        , _ZFP_ZFPathType_text::callbackOpen
        , _ZFP_ZFPathType_text::callbackClose
        , _ZFP_ZFPathType_text::callbackTell
        , _ZFP_ZFPathType_text::callbackSeek
        , _ZFP_ZFPathType_text::callbackRead
        , _ZFP_ZFPathType_text::callbackWrite
        , _ZFP_ZFPathType_text::callbackFlush
        , _ZFP_ZFPathType_text::callbackIsEof
        , _ZFP_ZFPathType_text::callbackIsError
        , _ZFP_ZFPathType_text::callbackSize
    )

ZF_NAMESPACE_GLOBAL_END

