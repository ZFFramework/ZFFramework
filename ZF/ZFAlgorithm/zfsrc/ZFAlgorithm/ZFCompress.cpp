#include "ZFCompress.h"
#include "protocol/ZFProtocolZFCompress.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFCompressLevel)

// ============================================================
// ZFCompress
ZFOBJECT_REGISTER(ZFCompress)

ZFMETHOD_DEFINE_2(ZFCompress, zfbool, open
        , ZFMP_IN(const ZFOutput &, outputZip)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        ) {
    this->close();
    d = ZFPROTOCOL_ACCESS(ZFCompress)->compressBegin(outputZip, compressLevel);
    return d != zfnull;
}
ZFMETHOD_DEFINE_0(ZFCompress, zfbool, close) {
    if(d) {
        zfbool ret = ZFPROTOCOL_ACCESS(ZFCompress)->compressEnd(d);
        d = zfnull;
        return ret;
    }
    else {
        return zftrue;
    }
}
ZFMETHOD_DEFINE_0(ZFCompress, zfbool, valid) {
    return d != zfnull;
}

ZFMETHOD_DEFINE_2(ZFCompress, zfbool, add
        , ZFMP_IN_OUT(const ZFInput &, inputRaw)
        , ZFMP_IN(const zfstring &, filePathInZip)
        ) {
    return d
        && ZFPROTOCOL_ACCESS(ZFCompress)->compressContent(d, inputRaw, filePathInZip)
        ;
}
ZFMETHOD_DEFINE_1(ZFCompress, zfbool, createDir
        , ZFMP_IN(const zfstring &, filePathInZip)
        ) {
    return d
        && ZFPROTOCOL_ACCESS(ZFCompress)->compressContentDir(d, filePathInZip)
        ;
}
ZFMETHOD_DEFINE_1(ZFCompress, zfbool, remove
        , ZFMP_IN(const zfstring &, filePathInZip)
        ) {
    return d
        && ZFPROTOCOL_ACCESS(ZFCompress)->compressContentRemove(d, filePathInZip)
        ;
}
ZFMETHOD_DEFINE_3(ZFCompress, zfbool, move
        , ZFMP_IN(const zfstring &, filePathInZipFrom)
        , ZFMP_IN(const zfstring &, filePathInZipTo)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    return d
        && ZFPROTOCOL_ACCESS(ZFCompress)->compressContentMove(d, filePathInZipFrom, filePathInZipTo)
        ;
}

ZFOBJECT_ON_INIT_DEFINE_2(ZFCompress
        , ZFMP_IN(const ZFOutput &, outputZip)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        ) {
    this->open(outputZip, compressLevel);
}
void ZFCompress::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfnull;
}
void ZFCompress::objectOnDeallocPrepare(void) {
    this->close();
    zfsuper::objectOnDeallocPrepare();
}

// ============================================================
// ZFDecompress
ZFMETHOD_DEFINE_1(ZFDecompress, zfbool, open
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        ) {
    this->close();
    d = ZFPROTOCOL_ACCESS(ZFCompress)->decompressBegin(inputZip);
    return d != zfnull;
}
ZFMETHOD_DEFINE_0(ZFDecompress, zfbool, close) {
    if(d) {
        zfbool ret = ZFPROTOCOL_ACCESS(ZFCompress)->decompressEnd(d);
        d = zfnull;
        return ret;
    }
    else {
        return zftrue;
    }
}
ZFMETHOD_DEFINE_0(ZFDecompress, zfbool, valid) {
    return d != zfnull;
}

ZFMETHOD_DEFINE_2(ZFDecompress, zfbool, outputAt
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN(zfindex, fileIndexInZip)
        ) {
    return d
        && ZFPROTOCOL_ACCESS(ZFCompress)->decompressContent(d, outputRaw, fileIndexInZip)
        ;
}
ZFMETHOD_DEFINE_2(ZFDecompress, zfbool, output
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN(const zfstring &, filePathInZip)
        ) {
    return d
        && ZFPROTOCOL_ACCESS(ZFCompress)->decompressContent(d, outputRaw, this->contentIndex(filePathInZip))
        ;
}

ZFMETHOD_DEFINE_0(ZFDecompress, zfindex, contentCount) {
    if(d) {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentCount(d);
    }
    else {
        return 0;
    }
}
ZFMETHOD_DEFINE_1(ZFDecompress, zfindex, contentIndex
        , ZFMP_IN(const zfstring &, filePathInZip)
        ) {
    if(d) {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentIndex(d, filePathInZip);
    }
    else {
        return zfindexMax();
    }
}
ZFMETHOD_DEFINE_1(ZFDecompress, zfstring, contentPath
        , ZFMP_IN(zfindex, fileIndexInZip)
        ) {
    zfstring tmp;
    if(d && ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentPath(d, tmp, fileIndexInZip)) {
        return tmp;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFDecompress, zfbool, contentPathT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(zfindex, fileIndexInZip)
        ) {
    return (d && ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentPath(d, ret, fileIndexInZip));
}
ZFMETHOD_DEFINE_1(ZFDecompress, zfbool, isDir
        , ZFMP_IN(zfindex, fileIndexInZip)
        ) {
    return d
        && ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentIsDir(d, fileIndexInZip)
        ;
}

ZFMETHOD_DEFINE_2(ZFDecompress, zfbool, findFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(const zfstring &, filePathInZip)
        ) {
    return d && ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentFindFirst(fd, d, filePathInZip);
}
ZFMETHOD_DEFINE_1(ZFDecompress, zfbool, findNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    return d && ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentFindNext(fd);
}
ZFMETHOD_DEFINE_1(ZFDecompress, void, findClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentFindClose(fd);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFDecompress
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        ) {
    this->open(inputZip);
}
void ZFDecompress::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfnull;
}
void ZFDecompress::objectOnDeallocPrepare(void) {
    this->close();
    zfsuper::objectOnDeallocPrepare();
}

// ============================================================
// util
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFCompressContent
        , ZFMP_IN_OUT(const ZFOutput &, outputZip)
        , ZFMP_IN_OUT(const ZFInput &, inputRaw)
        , ZFMP_IN_OPT(const zfstring &, filePathInZip, zftext("content"))
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        ) {
    zfobj<ZFCompress> t(outputZip, compressLevel);
    if(!t->valid()) {return zffalse;}
    zfbool success = zftrue;
    success &= t->add(inputRaw, filePathInZip);
    success &= t->close();
    return success;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContent
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        , ZFMP_IN_OPT(const zfstring &, filePathInZip, zftext("content"))
        ) {
    zfobj<ZFDecompress> t(inputZip);
    if(!t->valid()) {return zffalse;}
    zfbool success = zftrue;
    success &= t->output(outputRaw, filePathInZip);
    success &= t->close();
    return success;
}

static zfbool _ZFP_ZFCompressDir(
        ZF_IN_OUT ZFCompress *compressToken
        , ZF_IN const ZFPathInfoImpl &fileImpl
        , ZF_IN const zfstring &pathType
        , ZF_IN const zfstring &pathData
        , ZF_IN const zfstring &relPathInZip
        ) {
    if(!fileImpl.implIsExist(pathData)) {
        return zffalse;
    }

    if(fileImpl.implIsDir(pathData)) {
        if(!relPathInZip.isEmpty()) {
            if(!compressToken->createDir(relPathInZip)) {
                return zffalse;
            }
        }

        ZFFileFindData fd;
        zfbool success = zftrue;
        if(fileImpl.implFindFirst(fd, pathData)) {
            do {
                zfstring pathDataChild = fileImpl.implToChild(pathData, fd.name());
                if(!pathDataChild) {
                    success = zffalse;
                    break;
                }
                zfstring filePathInZip;
                if(!relPathInZip.isEmpty()) {
                    filePathInZip += relPathInZip;
                    filePathInZip += '/';
                }
                filePathInZip += fd.name();
                if(!_ZFP_ZFCompressDir(compressToken, fileImpl, pathType, pathDataChild, filePathInZip)) {
                    success = zffalse;
                    break;
                }
            } while(fileImpl.implFindNext(fd));
            fileImpl.implFindClose(fd);
        }
        return success;
    }
    else {
        zfstring filePathInZip = relPathInZip;
        if(filePathInZip.isEmpty()) {
            filePathInZip = fileImpl.implToFileName(pathData);
            if(filePathInZip.isEmpty()) {
                return zffalse;
            }
        }
        return compressToken->add(ZFInputForPathInfo(ZFPathInfo(pathType, pathData)), filePathInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFCompressDir
        , ZFMP_IN_OUT(const ZFOutput &, outputZip)
        , ZFMP_IN(const ZFPathInfo &, inputPathInfo)
        , ZFMP_IN_OPT(const zfstring &, filePathInZip, zftext("content"))
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        ) {
    const ZFPathInfoImpl *fileImpl = ZFPathInfoImplForPathType(inputPathInfo.pathType());
    if(fileImpl == zfnull) {
        return zffalse;
    }

    zfobj<ZFCompress> compressToken(outputZip, compressLevel);
    if(!compressToken->valid()) {
        return zffalse;
    }

    zfbool success = zftrue;
    success &= _ZFP_ZFCompressDir(compressToken, *fileImpl, inputPathInfo.pathType(), inputPathInfo.pathData(), filePathInZip);
    success &= compressToken->close();
    return success;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFDecompressDir
        , ZFMP_IN(const ZFPathInfo &, outputPathInfo)
        , ZFMP_IN_OUT(const ZFInput &, inputZip)
        ) {
    const ZFPathInfoImpl *fileImpl = ZFPathInfoImplForPathType(outputPathInfo.pathType());
    if(fileImpl == zfnull) {
        return zffalse;
    }

    zfobj<ZFDecompress> decompressToken(inputZip);
    if(!decompressToken->valid()) {
        return zffalse;
    }

    zfindex fileCountInZip = decompressToken->contentCount();
    zfbool success = zftrue;
    do {
        for(zfindex fileIndexInZip = 0; fileIndexInZip < fileCountInZip; ++fileIndexInZip) {
            zfstring filePathInZip = decompressToken->contentPath(fileIndexInZip);
            if(!filePathInZip) {
                success = zffalse;
                break;
            }

            zfstring outputPath = fileImpl->implToChild(outputPathInfo.pathData(), filePathInZip);
            if(!outputPath) {
                success = zffalse;
                break;
            }

            if(!filePathInZip.isEmpty() && filePathInZip[filePathInZip.length() - 1] == '/') {
                if(!fileImpl->implPathCreate(outputPath, zftrue, zfnull)) {
                    success = zffalse;
                    break;
                }
                continue;
            }

            ZFOutput outputRaw;
            if(!ZFOutputForPathInfoT(outputRaw, ZFPathInfo(outputPathInfo.pathType(), outputPath))) {
                success = zffalse;
                break;
            }
            if(!decompressToken->outputAt(outputRaw, fileIndexInZip)) {
                success = zffalse;
                break;
            }
        }
    } while(zffalse);
    success &= decompressToken->close();
    return success;
}

ZF_NAMESPACE_GLOBAL_END

