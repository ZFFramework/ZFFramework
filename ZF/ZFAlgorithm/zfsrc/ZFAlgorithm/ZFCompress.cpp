#include "ZFCompress.h"
#include "protocol/ZFProtocolZFCompress.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFCompressLevel)

// ============================================================
// ZFCompressToken
ZFOBJECT_REGISTER(ZFCompressToken)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFCompressToken, zfbool, close)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, read
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, write
        , ZFMP_IN(const zfstring &, itemPath)
        , ZFMP_IN(const ZFInput &, input)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, pathCreate
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, remove
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, move
        , ZFMP_IN(const zfstring &, itemPathFrom)
        , ZFMP_IN(const zfstring &, itemPathTo)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, itemFindFirst
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, itemFindNext
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, void, itemFindClose
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, itemIsExist
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, itemIsDir
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, itemIsDirAt
        , ZFMP_IN(zfindex, itemIndex)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFCompressToken, zfindex, itemCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfindex, itemIndex
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfstring, itemPath
        , ZFMP_IN(zfindex, itemIndex)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, itemPathT
        , ZFMP_IN_OUT(zfstring &, itemPath)
        , ZFMP_IN(zfindex, itemIndex)
        )

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFCompressToken>, ZFCompressOpen
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(ZFIOOpenOptionFlags, flags)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        ) {
    return ZFPROTOCOL_ACCESS(ZFCompress)->compressOpen(pathInfo, flags, compressLevel);
}

// ============================================================
// util
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFCompressFile
        , ZFMP_IN_OUT(const ZFPathInfo &, outputCompress)
        , ZFMP_IN_OUT(const ZFInput &, inputRaw)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        ) {
    zfautoT<ZFCompressToken> compress = ZFCompressOpen(outputCompress, v_ZFIOOpenOption::e_Modify, compressLevel);
    return compress && compress->write(itemPath, inputRaw);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContent
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN_OUT(const ZFPathInfo &, inputCompress)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        ) {
    zfautoT<ZFCompressToken> compress = ZFCompressOpen(inputCompress, v_ZFIOOpenOption::e_Read);
    return compress && compress->read(outputRaw, itemPath);
}

static zfbool _ZFP_ZFCompressDir(
        ZF_IN_OUT ZFCompressToken *compress
        , ZF_IN ZFIOImpl *ioImpl
        , ZF_IN const zfstring &pathData
        , ZF_IN const zfstring &itemPathRel
        ) {
    ZFIOFindData fd;
    if(ioImpl->ioFindFirst(fd, pathData)) {
        zfbool success = zftrue;
        do {
            zfstring pathDataChild;
            if(!ioImpl->ioToChild(pathDataChild, pathData, fd.name())) {
                success = zffalse;
                break;
            }
            zfstring itemPath;
            if(!itemPathRel) {
                itemPath += itemPathRel;
                itemPath += "/";
            }
            itemPath += fd.name();
            if(fd.isDir()) {
                if(!_ZFP_ZFCompressDir(compress, ioImpl, pathDataChild, itemPath)) {
                    success = zffalse;
                    break;
                }
            }
            else {
                if(!compress->write(itemPath, ZFInputForPathInfo(ZFPathInfo(ioImpl->pathType(), pathDataChild)))) {
                    success = zffalse;
                    break;
                }
            }
        } while(ioImpl->ioFindNext(fd));
        ioImpl->ioFindClose(fd);
        return success;
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFCompressDir
        , ZFMP_IN_OUT(const ZFPathInfo &, outputCompress)
        , ZFMP_IN(const ZFPathInfo &, inputPathInfo)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zfnull)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Modify)
        , ZFMP_IN_OPT(ZFCompressLevel, compressLevel, v_ZFCompressLevel::EnumDefault())
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(inputPathInfo.pathType());
    if(!ioImpl) {
        return zffalse;
    }
    zfautoT<ZFCompressToken> compress = ZFCompressOpen(outputCompress, v_ZFIOOpenOption::e_Modify, compressLevel);
    if(!compress) {
        return zffalse;
    }
    if(ioImpl->ioIsDir(inputPathInfo.pathData())) {
        zfstring itemPathFix;
        if(!itemPath) {
            return ioImpl->ioToFileName(itemPathFix, inputPathInfo.pathData())
                && itemPathFix
                && _ZFP_ZFCompressDir(compress, ioImpl, inputPathInfo.pathData(), itemPathFix);
        }
        else if(zfstringIsEqual(itemPath, ".")) {
            return _ZFP_ZFCompressDir(compress, ioImpl, inputPathInfo.pathData(), "");
        }
        else {
            return _ZFP_ZFCompressDir(compress, ioImpl, inputPathInfo.pathData(), itemPath);
        }
    }
    else if(ioImpl->ioIsExist(inputPathInfo.pathData())) {
        if(itemPath && !zfstringIsEqual(itemPath, ".")) {
            return compress->write(itemPath, ZFInputForPathInfo(inputPathInfo));
        }
        else {
            zfstring fileName;
            return ioImpl->ioToFileName(fileName, inputPathInfo.pathData())
                && fileName
                && compress->write(fileName, ZFInputForPathInfo(inputPathInfo));
        }
        return zftrue;
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFDecompressDir
        , ZFMP_IN(const ZFPathInfo &, outputPathInfo)
        , ZFMP_IN_OUT(const ZFPathInfo &, inputCompress)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(outputPathInfo.pathType());
    if(!ioImpl) {
        return zffalse;
    }
    zfautoT<ZFCompressToken> compress = ZFCompressOpen(inputCompress, v_ZFIOOpenOption::e_Read);
    if(!compress) {
        return zffalse;
    }
    for(zfindex i = 0; i < compress->itemCount(); ++i) {
        zfstring itemPath = compress->itemPath(i);
        if(!itemPath) {
            return zffalse;
        }
        zfstring outputPath;
        if(!ioImpl->ioToChild(outputPath, outputPathInfo.pathData(), itemPath)
                || !outputPath
                ) {
            return zffalse;
        }
        if(itemPath[itemPath.length() - 1] == '/') {
            if(!ioImpl->ioPathCreate(outputPath)) {
                return zffalse;
            }
        }
        else {
            zfstring outputParentPath;
            if(!ioImpl->ioToParent(outputParentPath, outputPath)) {
                return zffalse;
            }
            if(!ioImpl->ioPathCreate(outputParentPath)) {
                return zffalse;
            }
            if(!compress->read(ZFOutputForPathInfo(ZFPathInfo(outputPathInfo.pathType(), outputPath)), itemPath)) {
                return zffalse;
            }
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

