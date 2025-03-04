#include "ZFProtocolZFCompress.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFCompress)

zfclassNotPOD _ZFP_ZFProtocolZFCompress_FindData {
public:
    void *decompressToken;
    zfstring relPath; // ensured no '/' at head or tail
    zfindex lastIndex; // last found index
};

static zfbool _ZFP_ZFProtocolZFCompress_FindNext(
        ZFPROTOCOL_INTERFACE_CLASS(ZFCompress) *impl
        , ZF_IN_OUT ZFFileFindData &fd
        , ZF_IN _ZFP_ZFProtocolZFCompress_FindData *d
        ) {
    zfindex count = impl->decompressContentCount(d->decompressToken);
    zfstring tmp;
    for(zfindex i = d->lastIndex + 1; i < count; ++i) {
        tmp.removeAll();
        if(!impl->decompressContentPath(d->decompressToken, tmp, i) || tmp.isEmpty()) {
            return zffalse;
        }
        const zfchar *path = tmp;
        zfindex pathLen = tmp.length();
        while(path[0] == '/') {
            ++path;
            --pathLen;
        }
        while(pathLen > 0 && path[pathLen - 1] == '/') {
            --pathLen;
        }
        if(pathLen == 0) {
            continue;
        }
        if(!d->relPath.isEmpty()) {
            if(pathLen == d->relPath.length() || zfsncmp(d->relPath.cString(), path, d->relPath.length()) != 0) {
                continue;
            }
            path += d->relPath.length() + 1;
            pathLen -= d->relPath.length() + 1;
            if(pathLen == 0) {
                continue;
            }
        }
        zfindex pos = zfstringFind(path, pathLen, '/');
        if(pos != zfindexMax() && pos != pathLen - 1) {
            continue;
        }

        // found
        d->lastIndex = i;
        fd.impl().name.assign(path, pos == zfindexMax() ? pathLen : pathLen - 1);
        fd.impl().isDir = impl->decompressContentIsDir(d->decompressToken, i);
        return true;
    }
    return zffalse;
}

zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFCompress)::decompressContentFindFirst(
        ZF_IN_OUT ZFFileFindData &fd
        , ZF_IN void *decompressToken
        , ZF_IN const zfstring &filePathInZip
        ) {
    _ZFP_ZFProtocolZFCompress_FindData *d = zfnew(_ZFP_ZFProtocolZFCompress_FindData);
    d->decompressToken = decompressToken;
    d->lastIndex = zfindexMax();
    ZFPathFormatT(d->relPath, filePathInZip);
    while(!d->relPath.isEmpty() && d->relPath[0] == '/') {
        d->relPath.remove(0, 1);
    }
    zfbool ret = _ZFP_ZFProtocolZFCompress_FindNext(this, fd, d);
    if(ret) {
        fd.implAttach("ZFDecompressContentFindFirst", d);
    }
    else {
        zfdelete(d);
    }
    return ret;
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFCompress)::decompressContentFindNext(ZF_IN_OUT ZFFileFindData &fd) {
    _ZFP_ZFProtocolZFCompress_FindData *d = (_ZFP_ZFProtocolZFCompress_FindData *)fd.implUserData();
    return _ZFP_ZFProtocolZFCompress_FindNext(this, fd, d);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFCompress)::decompressContentFindClose(ZF_IN_OUT ZFFileFindData &fd) {
    fd.implDetach();
}

ZF_NAMESPACE_GLOBAL_END

