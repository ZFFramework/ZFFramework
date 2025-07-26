#include "ZFProtocolZFRes.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFRes)

// ============================================================
zfbool _ZFP_ZFImpl_ZFRes_resPathFix(
        ZF_IN const zfstring &resRootPath
        , ZF_IN_OUT zfstring &result
        , ZF_IN const zfstring &original
        ) {
    result.assign(original, resRootPath.length());
    return zftrue;
}

// ============================================================
const zfstring &ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resRootPath(void) {
    static zfstring d = zfstr(".%sres%s", '/', '/');
    return d;
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resIsExist(ZF_IN const zfchar *resPath) {
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    return ZFFileIsExist(tmpPath);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resIsDir(ZF_IN const zfchar *resPath) {
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    return ZFFileIsDir(tmpPath);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resCopy(
        ZF_IN const zfchar *resPath
        , ZF_IN const zfchar *dstPath
        , ZF_IN_OPT zfbool isRecursive /* = zftrue */
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        ) {
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    return ZFFileCopy(tmpPath, dstPath, isRecursive, isForce);
}

// ============================================================
// res RW
void *ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resOpen(ZF_IN const zfchar *resPath) {
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    return ZFFileOpen(tmpPath, v_ZFFileOpenOption::e_Read);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resClose(ZF_IN void *token) {
    return ZFFileClose(token);
}
zfindex ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resTell(ZF_IN void *token) {
    return ZFFileTell(token);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resSeek(
        ZF_IN void *token
        , ZF_IN zfindex byteSize
        , ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */
        ) {
    return ZFFileSeek(token, byteSize, position);
}
zfindex ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resRead(
        ZF_IN void *token
        , ZF_IN void *buf
        , ZF_IN zfindex maxByteSize
        ) {
    return ZFFileRead(token, buf, maxByteSize);
}

// ============================================================
// res find
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resFindFirst(
        ZF_IN_OUT ZFFileFindData::Impl &fd
        , ZF_IN const zfchar *resPath
        ) {
    ZFFileFindData::Impl *normalFd = zfnew(ZFFileFindData::Impl);
    zfstring findPath;
    findPath += this->resRootPath();
    findPath += resPath;
    if(!ZFPROTOCOL_ACCESS(ZFFile)->fileFindFirst(*normalFd, findPath)) {
        zfdelete(normalFd);
        return zffalse;
    }
    fd.name = normalFd->name;
    fd.isDir = normalFd->isDir;
    fd.nativeFd = normalFd;
    return zftrue;
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resFindNext(ZF_IN_OUT ZFFileFindData::Impl &fd) {
    ZFFileFindData::Impl *normalFd = (ZFFileFindData::Impl *)fd.nativeFd;
    if(!ZFPROTOCOL_ACCESS(ZFFile)->fileFindNext(*normalFd)) {
        return zffalse;
    }
    fd.name = normalFd->name;
    fd.isDir = normalFd->isDir;
    return zftrue;
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFRes)::resFindClose(ZF_IN_OUT ZFFileFindData::Impl &fd) {
    ZFFileFindData::Impl *normalFd = (ZFFileFindData::Impl *)fd.nativeFd;
    ZFPROTOCOL_ACCESS(ZFFile)->fileFindClose(*normalFd);
    zfdelete(normalFd);
    fd.nativeFd = zfnull;
}

ZF_NAMESPACE_GLOBAL_END

