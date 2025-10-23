#include "ZFCompress.h"
#include "protocol/ZFProtocolZFCompress.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFCompressLevel)

// ============================================================
// ZFCompressToken
ZFOBJECT_REGISTER(ZFCompressToken)

ZFInput ZFCompressToken::input(ZF_IN const zfstring &itemPath) {
    zfobj<ZFIOBuffer> buf;
    if(this->ioRead(buf->output(), itemPath)) {
        buf->input().ioSeek(0);
        return buf->input();
    }
    else {
        return zfnull;
    }
}
ZFOutput ZFCompressToken::output(ZF_IN const zfstring &itemPath) {
    if(!ZFBitTest(this->ioFlags(), v_ZFIOOpenOption::e_Write | v_ZFIOOpenOption::e_Modify)) {
        return zfnull;
    }
    zfself *owner = this;
    ZFLISTENER_2(conv
            , zfweakT<zfself>, owner
            , zfstring, itemPath
            ) {
        if(owner) {
            ZFIOBuffer *buf = zfargs.sender();
            buf->input().ioSeek(0);
            owner->ioWrite(itemPath, buf->input());
        }
    } ZFLISTENER_END()
    ZFOutput ret = ZFIOBufferOutput(zfnull, conv);

    ZFLISTENER_1(ownerOnClose
            , ZFOutput, ret
            ) {
        ret.ioClose();
    } ZFLISTENER_END()
    this->observerAddForOnce(zfself::E_IOCloseOnPrepare(), ownerOnClose);

    return ret;
}

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, ioRead
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, ioWrite
        , ZFMP_IN(const zfstring &, itemPath)
        , ZFMP_IN(const ZFInput &, input)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, ZFInput, input
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, ZFOutput, output
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, ioPathCreate
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, ioRemove
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, ioMove
        , ZFMP_IN(const zfstring &, itemPathTo)
        , ZFMP_IN(const zfstring &, itemPathFrom)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFCompressToken, zfbool, ioFindFirst
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, ioFindNext
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, void, ioFindClose
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, ioIsExist
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, ioIsDir
        , ZFMP_IN(const zfstring &, itemPath)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFCompressToken, zfindex, itemCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFCompressToken, zfiter, itemIter)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfbool, itemIsDir
        , ZFMP_IN(const zfiter &, it)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCompressToken, zfstring, itemPath
        , ZFMP_IN(const zfiter &, it)
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
    return compress && compress->ioWrite(itemPath, inputRaw);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContent
        , ZFMP_IN_OUT(const ZFOutput &, outputRaw)
        , ZFMP_IN_OUT(const ZFPathInfo &, inputCompress)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        ) {
    zfautoT<ZFCompressToken> compress = ZFCompressOpen(inputCompress, v_ZFIOOpenOption::e_Read);
    return compress && compress->ioRead(outputRaw, itemPath);
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
            if(itemPathRel) {
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
                if(!compress->ioWrite(itemPath, ZFInputForPathInfo(ZFPathInfo(ioImpl->pathType(), pathDataChild)))) {
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
            return compress->ioWrite(itemPath, ZFInputForPathInfo(inputPathInfo));
        }
        else {
            zfstring fileName;
            return ioImpl->ioToFileName(fileName, inputPathInfo.pathData())
                && fileName
                && compress->ioWrite(fileName, ZFInputForPathInfo(inputPathInfo));
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
    for(zfiter it = compress->itemIter(); it; ++it) {
        zfstring itemPath = compress->itemPath(it);
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
            if(!compress->ioRead(ZFOutputForPathInfo(ZFPathInfo(outputPathInfo.pathType(), outputPath)), itemPath)) {
                return zffalse;
            }
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

