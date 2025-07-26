#include "ZFPathType_base64.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(base64)

// ============================================================
// base64
zfclassNotPOD _ZFP_ZFPathType_base64 {
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
    static zfbool callbackToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        return zffalse;
    }
    static zfbool callbackToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            ) {
        return zffalse;
    }
    static zfbool callbackToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        return zffalse;
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
        if(flag != v_ZFFileOpenOption::e_Read) {
            return zfnull;
        }
        zfindex pathDataLen = zfslen(pathData);
        _Token *d = zfnew(_Token);
        d->bufSize = ZFBase64DecodeCalcSize(pathDataLen);
        d->buf = (zfbyte *)zfmalloc(d->bufSize);
        ZFBase64Decode(d->buf, pathData, pathDataLen, &(d->bufSize));
        return d;
    }
    static zfbool callbackClose(ZF_IN void *token) {
        zfdelete((_Token *)token);
        return zftrue;
    }
    static zfindex callbackSize(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        return d->bufSize;
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
        d->pos = ZFIOCallbackCalcSeek(0, d->bufSize, d->pos, byteSize, position);
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
};
ZFPATHTYPE_FILEIO_REGISTER(base64, ZFPathType_base64()
        , _ZFP_ZFPathType_base64::callbackIsExist
        , _ZFP_ZFPathType_base64::callbackIsDir
        , _ZFP_ZFPathType_base64::callbackToFileName
        , _ZFP_ZFPathType_base64::callbackToChild
        , _ZFP_ZFPathType_base64::callbackToParent
        , _ZFP_ZFPathType_base64::callbackPathCreate
        , _ZFP_ZFPathType_base64::callbackRemove
        , _ZFP_ZFPathType_base64::callbackMove
        , _ZFP_ZFPathType_base64::callbackFindFirst
        , _ZFP_ZFPathType_base64::callbackFindNext
        , _ZFP_ZFPathType_base64::callbackFindClose
        , _ZFP_ZFPathType_base64::callbackOpen
        , _ZFP_ZFPathType_base64::callbackClose
        , _ZFP_ZFPathType_base64::callbackSize
        , _ZFP_ZFPathType_base64::callbackTell
        , _ZFP_ZFPathType_base64::callbackSeek
        , _ZFP_ZFPathType_base64::callbackRead
        , _ZFP_ZFPathType_base64::callbackWrite
    )

ZF_NAMESPACE_GLOBAL_END

