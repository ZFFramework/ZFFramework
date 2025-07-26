#include "ZFPathType_encrypt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(encrypt)

// ============================================================
zfclassNotPOD _ZFP_ZFPathType_encrypt {
public:
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        return ZFPathInfoIsExist(refPathInfo);
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        return ZFPathInfoIsDir(refPathInfo);
    }
    static zfbool callbackToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        return ZFPathInfoCallbackToFileNameDefault(ret, refPathInfo.pathData());
    }
    static zfbool callbackToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            ) {
        if(zfstringIsEmpty(childName)) {
            return zftrue;
        }

        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        ZFPathInfoCallbackToChildDefault(selfPathData, selfPathData, childName);
        if(pathData >= ret.cString() && pathData < ret.cString() + ret.length()) {
            ret.removeAll();
        }
        ZFPathInfoChainEncodeT(ret, refPathInfo, selfPathData);
        return zftrue;
    }
    static zfbool callbackToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        ZFPathInfoCallbackToParentDefault(selfPathData, selfPathData);
        if(pathData >= ret.cString() && pathData < ret.cString() + ret.length()) {
            ret.removeAll();
        }
        ZFPathInfoChainEncodeT(ret, refPathInfo, selfPathData);
        return zftrue;
    }
    static zfbool callbackPathCreate(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool autoMakeParent
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        return ZFPathInfoPathCreate(refPathInfo, autoMakeParent);
    }
    static zfbool callbackRemove(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool isRecursive
            , ZF_IN_OPT zfbool isForce
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        return ZFPathInfoRemove(refPathInfo, isRecursive, isForce);
    }
    static zfbool callbackMove(
            ZF_IN const zfchar *pathDataFrom
            , ZF_IN const zfchar *pathDataTo
            , ZF_IN_OPT zfbool isForce
            ) {
        ZFPathInfo refPathInfoFrom;
        zfstring selfPathDataFrom;
        if(!ZFPathInfoChainDecode(refPathInfoFrom, selfPathDataFrom, pathDataFrom)) {
            return zffalse;
        }
        ZFPathInfo refPathInfoTo;
        zfstring selfPathDataTo;
        if(!ZFPathInfoChainDecode(refPathInfoTo, selfPathDataTo, pathDataTo)) {
            return zffalse;
        }
        return ZFPathInfoMove(refPathInfoFrom, refPathInfoTo.pathData(), isForce);
    }

    zfclassNotPOD _FindData {
    public:
        ZFPathInfo refPathInfo;
        ZFFileFindData fd;
    };
    static zfbool callbackFindFirst(
            ZF_IN_OUT ZFFileFindData &fd
            , ZF_IN const zfchar *pathData
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        _FindData *impl = zfpoolNew(_FindData);
        impl->refPathInfo = refPathInfo;
        if(!ZFPathInfoFindFirst(impl->refPathInfo, impl->fd)) {
            zfpoolDelete(impl);
            return zffalse;
        }
        fd.implAttach("ZFPathType_encrypt", impl);
        fd.implCopy(impl->fd);
        return zftrue;
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd) {
        _FindData *impl = (_FindData *)fd.implCheck("ZFPathType_encrypt");
        if(!impl) {
            return zffalse;
        }
        if(!ZFPathInfoFindNext(impl->refPathInfo, impl->fd)) {
            return zffalse;
        }
        fd.implCopy(impl->fd);
        return zftrue;
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd) {
        _FindData *impl = (_FindData *)fd.implCheck("ZFPathType_encrypt");
        if(!impl) {
            return;
        }
        ZFPathInfoFindClose(impl->refPathInfo, impl->fd);
        zfpoolDelete(impl);
    }

    zfclassNotPOD _IOTask {
    public:
        ZFPathInfo refPathInfo;
        zfstring encryptKey;
        zfautoT<ZFIOBuffer> io; // store decrypted contents
        ZFFileOpenOptionFlags flag;
    public:
        zfbool refRead(void) {
            void *token = ZFPathInfoOpen(this->refPathInfo, ZFFileOpenOption::e_Read, zffalse);
            ZFPathInfoCloseHolder(this->refPathInfo, token);
            if(token) {
                ZFInput orig;
                orig.callbackSerializeDisable(zftrue);
                if(ZFInputForPathInfoTokenT(orig, token, this->refPathInfo, ZFFileOpenOption::e_Read, zffalse)) {
                    zfobj<ZFIOBufferByCacheFile> buf;
                    ZFInputRead(buf->output(), orig);
                    ZFDecrypt(this->io->output(), buf->input(), this->encryptKey);
                    return zftrue;
                }
            }
            return zffalse;
        }
    };
    static void *callbackOpen(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT ZFFileOpenOptionFlags flag
            , ZF_IN_OPT zfbool autoCreateParent
            ) {
        _IOTask *impl = zfpoolNew(_IOTask);
        if(!ZFPathInfoChainDecode(impl->refPathInfo, impl->encryptKey, pathData)) {
            zfpoolDelete(impl);
            return zfnull;
        }
        impl->io = zfobj<ZFIOBufferByCacheFile>();
        impl->flag = flag;

        if(ZFBitTest(flag, ZFFileOpenOption::e_Append)) {
            impl->refRead();
            impl->io->input().ioSeek(impl->io->output().ioTell());
        }
        else if(!ZFBitTest(flag, ZFFileOpenOption::e_Create)) {
            if(!impl->refRead()) {
                zfpoolDelete(impl);
                return zfnull;
            }
            impl->io->output().ioSeek(0);
        }
        return impl;
    }
    static zfbool callbackClose(ZF_IN void *token) {
        _IOTask *impl = (_IOTask *)token;
        if(!impl) {
            return zffalse;
        }
        zfbool ret = zftrue;
        if(ZFBitTest(impl->flag, 0
                    | ZFFileOpenOption::e_Create
                    | ZFFileOpenOption::e_Write
                    | ZFFileOpenOption::e_Append
                    )) {
            impl->io->input().ioSeek(0);
            ret &= ZFEncrypt(
                    ZFOutputForPathInfo(impl->refPathInfo, ZFFileOpenOption::e_Create)
                    , impl->io->input()
                    , impl->encryptKey
                    );
        }
        zfpoolDelete(impl);
        return ret;
    }
    static zfindex callbackSize(ZF_IN void *token) {
        _IOTask *impl = (_IOTask *)token;
        if(!impl) {
            return zfindexMax();
        }
        return impl->io->input().ioSize();
    }
    static zfindex callbackTell(ZF_IN void *token) {
        _IOTask *impl = (_IOTask *)token;
        if(!impl) {
            return zfindexMax();
        }
        return impl->io->input().ioTell();
    }
    static zfbool callbackSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos position
            ) {
        _IOTask *impl = (_IOTask *)token;
        if(!impl) {
            return zffalse;
        }
        return impl->io->output().ioSeek(byteSize, position)
            && impl->io->input().ioSeek(byteSize, position)
            ;
    }
    static zfindex callbackRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        _IOTask *impl = (_IOTask *)token;
        if(!impl || !ZFBitTest(impl->flag, ZFFileOpenOption::e_Read)) {
            return 0;
        }
        zfindex ret = impl->io->input().execute(buf, maxByteSize);
        impl->io->output().ioSeek(impl->io->input().ioTell());
        return ret;
    }
    static zfindex callbackWrite(
            ZF_IN void *token
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize
            ) {
        _IOTask *impl = (_IOTask *)token;
        if(!impl || !ZFBitTest(impl->flag, 0
                    | ZFFileOpenOption::e_Create
                    | ZFFileOpenOption::e_Write
                    | ZFFileOpenOption::e_Append
                    )) {
            return zffalse;
        }
        zfindex ret = impl->io->output().execute(src, maxByteSize);
        impl->io->input().ioSeek(impl->io->output().ioTell());
        return ret;
    }
};
ZFPATHTYPE_FILEIO_REGISTER(encrypt, ZFPathType_encrypt()
        , _ZFP_ZFPathType_encrypt::callbackIsExist
        , _ZFP_ZFPathType_encrypt::callbackIsDir
        , _ZFP_ZFPathType_encrypt::callbackToFileName
        , _ZFP_ZFPathType_encrypt::callbackToChild
        , _ZFP_ZFPathType_encrypt::callbackToParent
        , _ZFP_ZFPathType_encrypt::callbackPathCreate
        , _ZFP_ZFPathType_encrypt::callbackRemove
        , _ZFP_ZFPathType_encrypt::callbackMove
        , _ZFP_ZFPathType_encrypt::callbackFindFirst
        , _ZFP_ZFPathType_encrypt::callbackFindNext
        , _ZFP_ZFPathType_encrypt::callbackFindClose
        , _ZFP_ZFPathType_encrypt::callbackOpen
        , _ZFP_ZFPathType_encrypt::callbackClose
        , _ZFP_ZFPathType_encrypt::callbackSize
        , _ZFP_ZFPathType_encrypt::callbackTell
        , _ZFP_ZFPathType_encrypt::callbackSeek
        , _ZFP_ZFPathType_encrypt::callbackRead
        , _ZFP_ZFPathType_encrypt::callbackWrite
    )

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(ZFInput, ZFInputForEncrypt
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfchar *, encryptKey)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(
                ZFPathType_encrypt()
                , ZFPathInfoChainEncode(refPathInfo, encryptKey)
                ), flags);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForEncrypt
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfchar *, encryptKey)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, ZFPathInfo(
                ZFPathType_encrypt()
                , ZFPathInfoChainEncode(refPathInfo, encryptKey)
                ), flags);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

