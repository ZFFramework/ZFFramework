#include "ZFIODef_pathInfo.h"

#include "ZFIO_file.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFIOIsExist
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioIsExistDefault(pathInfo.pathData());
    }
    else {
        return ioImpl->ioIsExist(pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFIOIsDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioIsDirDefault(pathInfo.pathData());
    }
    else {
        return ioImpl->ioIsDir(pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFIOToFileName
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioToFileNameDefault(ret, pathInfo.pathData());
    }
    else {
        return ioImpl->ioToFileName(ret, pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFIOToChild
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childName)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioToChildDefault(ret, pathInfo.pathData(), childName);
    }
    else {
        return ioImpl->ioToChild(ret, pathInfo.pathData(), childName);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFIOToParent
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioToParentDefault(ret, pathInfo.pathData());
    }
    else {
        return ioImpl->ioToParent(ret, pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFIOPathCreate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioPathCreateDefault(pathInfo.pathData(), autoCreateParent);
    }
    else {
        return ioImpl->ioPathCreate(pathInfo.pathData(), autoCreateParent);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFIORemove
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioRemoveDefault(pathInfo.pathData(), isRecursive, isForce);
    }
    else {
        return ioImpl->ioRemove(pathInfo.pathData(), isRecursive, isForce);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFIOMove
        , ZFMP_IN(const zfstring &, pathDataTo)
        , ZFMP_IN(const ZFPathInfo &, pathInfoFrom)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfoFrom.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioMoveDefault(pathDataTo, pathInfoFrom.pathData(), isForce);
    }
    else {
        return ioImpl->ioMove(pathDataTo, pathInfoFrom.pathData(), isForce);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zftimet, ZFIOModTime
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioModTimeDefault(pathInfo.pathData());
    }
    else {
        return ioImpl->ioModTime(pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFIOModTime
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(zftimet, time)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioModTimeDefault(pathInfo.pathData(), time);
    }
    else {
        return ioImpl->ioModTime(pathInfo.pathData(), time);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFIOToken>, ZFIOOpen
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(ZFIOOpenOptionFlags, flags)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return ZFIOImpl::ioOpenDefault(pathInfo.pathData(), flags, autoCreateParent);
    }
    else {
        return ioImpl->ioOpen(pathInfo.pathData(), flags, autoCreateParent);
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFPathInfo, ZFLocalPathInfo
        , ZFMP_IN_OPT(const zfstring &, localPath, zfnull)
        ) {
    return ZFPathInfo();
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(ZFPathInfo &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childPath)
        ) {
    zfstring pathData;
    if(ZFPathInfoForLocalT(pathData, pathInfo, childPath)) {
        ret.pathType(pathInfo.pathType());
        ret.pathData(pathData);
        return zftrue;
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childPath)
        ) {
    if(zfstringIsEmpty(childPath)) {
        ret = pathInfo.pathData();
        return zftrue;
    }
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(ioImpl == zfnull) {
        return zffalse;
    }
    if(!ioImpl->ioIsDir(pathInfo.pathData())) {
        return ioImpl->ioToParent(ret, pathInfo.pathData())
            && ioImpl->ioToChild(ret, ret, childPath)
            ;
    }
    else {
        return ioImpl->ioToChild(ret, pathInfo.pathData(), childPath);
    }
}
ZFMETHOD_FUNC_DEFINE_2(ZFPathInfo, ZFPathInfoForLocal
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childPath)
        ) {
    ZFPathInfo ret;
    ZFPathInfoForLocalT(ret, pathInfo, childPath);
    return ret;
}

// ============================================================
// ZFInputForPathInfo
zfclass _ZFP_I_ZFInputForPathInfoOwner : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFInputForPathInfoOwner, ZFObject)

    ZFOBJECT_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void) {
        this->ioImpl = zfnull;
        this->ioToken = zfnull;
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    zfbool setup(
            ZF_IN const zfautoT<ZFIOImpl> &ioImpl
            , ZF_IN const zfautoT<ZFIOToken> &ioToken
            ) {
        if(ioImpl && ioToken) {
            this->ioImpl = ioImpl;
            this->ioToken = ioToken;
            return zftrue;
        }
        else {
            return zffalse;
        }
    }

    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            )
    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, seekPos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)

private:
    zfautoT<ZFIOImpl> ioImpl;
    zfautoT<ZFIOToken> ioToken;
protected:
    _ZFP_I_ZFInputForPathInfoOwner(void)
    : ioImpl(zfnull)
    , ioToken(zfnull)
    {
    }
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForPathInfoOwner, zfindex, onInput
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(this->ioToken) {
        return this->ioToken->ioRead(buf, count);
    }
    else {
        return 0;
    }
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForPathInfoOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, seekPos)
        ) {
    if(this->ioToken) {
        return this->ioToken->ioSeek(byteSize, seekPos);
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForPathInfoOwner, zfindex, ioTell) {
    if(this->ioToken) {
        return this->ioToken->ioTell();
    }
    else {
        return zfindexMax();
    }
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForPathInfoOwner, zfindex, ioSize) {
    if(this->ioToken) {
        return this->ioToken->ioSize();
    }
    else {
        return zfindexMax();
    }
}
ZFMETHOD_FUNC_DEFINE_1(ZFInput, ZFInputForIOToken
        , ZFMP_IN(ZFIOToken *, token)
        ) {
    ZFInput ret;
    ZFInputForIOTokenT(ret, token);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFInputForIOTokenT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(ZFIOToken *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFInputForPathInfoOwner> inputOwner;
    if(!inputOwner->setup(ZFIOImplForPathType(token->pathType()), token)) {
        return zffalse;
    }
    ret = ZFCallbackForMemberMethod(
        inputOwner, ZFMethodAccess(_ZFP_I_ZFInputForPathInfoOwner, onInput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, inputOwner);

    ZFInputMarkSerializable(ret, token->pathInfo());
    return ret;
}
ZFMETHOD_FUNC_DEFINE_1(ZFInput, ZFInputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, pathInfo);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFInputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    return ZFInputForIOTokenT(ret, ZFIOOpen(pathInfo, v_ZFIOOpenOption::e_Read, zffalse));
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFInputForPathInfo, ZFCallbackSerializeType_ZFInputForPathInfo) {
    ZFPathInfo pathInfo;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFIO_pathInfo, ZFPathInfo, pathInfo, {
                return zffalse;
            });

    ret.callbackSerializeDisable(zftrue);
    ZFInputForPathInfoT(ret, pathInfo);
    if(!ret) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s", pathInfo);
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
// ZFOutputForPathInfo
zfclass _ZFP_I_ZFOutputForPathInfoOwner : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFOutputForPathInfoOwner, ZFObject)

    ZFOBJECT_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void) {
        this->ioImpl = zfnull;
        this->ioToken = zfnull;
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    zfbool setup(
            ZF_IN const zfautoT<ZFIOImpl> &ioImpl
            , ZF_IN const zfautoT<ZFIOToken> &token
            ) {
        if(ioImpl && token) {
            this->ioImpl = ioImpl;
            this->ioToken = token;
            return zftrue;
        }
        else {
            return zffalse;
        }
    }

    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_IN(const void *, s)
            , ZFMP_IN(zfindex, count)
            )
    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, seekPos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)

private:
    zfautoT<ZFIOImpl> ioImpl;
    zfautoT<ZFIOToken> ioToken;
protected:
    _ZFP_I_ZFOutputForPathInfoOwner(void)
    : ioImpl(zfnull)
    , ioToken(zfnull)
    {
    }
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, onOutput
        , ZFMP_IN(const void *, s)
        , ZFMP_IN(zfindex, count)
        ) {
    if(this->ioToken) {
        return this->ioToken->ioWrite(s, count);
    }
    else {
        return 0;
    }
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForPathInfoOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, seekPos)
        ) {
    if(this->ioToken) {
        return this->ioToken->ioSeek(byteSize, seekPos);
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, ioTell) {
    if(this->ioToken) {
        return this->ioToken->ioTell();
    }
    else {
        return zfindexMax();
    }
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, ioSize) {
    if(this->ioToken) {
        return this->ioToken->ioSize();
    }
    else {
        return zfindexMax();
    }
}
ZFMETHOD_FUNC_DEFINE_1(ZFOutput, ZFOutputForIOToken
        , ZFMP_IN(ZFIOToken *, token)
        ) {
    ZFOutput ret;
    ZFOutputForIOTokenT(ret, token);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFOutputForIOTokenT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(ZFIOToken *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFOutputForPathInfoOwner> outputOwner;
    if(!outputOwner->setup(ZFIOImplForPathType(token->pathType()), token)) {
        return zffalse;
    }
    ret = ZFCallbackForMemberMethod(
        outputOwner, ZFMethodAccess(_ZFP_I_ZFOutputForPathInfoOwner, onOutput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, outputOwner);

    ZFOutputMarkSerializable(ret, token->pathInfo(), token->ioFlags());
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, pathInfo, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFOutputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        ) {
    return ZFOutputForIOTokenT(ret, ZFIOOpen(pathInfo, flags, zftrue));
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFOutputForPathInfo, ZFCallbackSerializeType_ZFOutputForPathInfo) {
    ZFPathInfo pathInfo;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFIO_pathInfo, ZFPathInfo, pathInfo, {
                return zffalse;
            });

    ZFIOOpenOptionFlags flags;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFIO_flags, ZFIOOpenOptionFlags, flags, {
                return zffalse;
            });

    ret.callbackSerializeDisable(zftrue);
    ZFOutputForPathInfoT(ret, pathInfo, flags);
    if(!ret) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s", pathInfo);
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForLocal
        , ZFMP_IN(const zfstring &, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    ZFInput ret;
    ZFInputForLocalT(ret, localPath, pathInfo);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFInputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfstring &, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    if(zfstringIsEmpty(localPath)) {
        return zffalse;
    }
    if(!pathInfo) {
        return ZFInputForPathInfoT(ret, ZFPathInfo(ZFPathType_file(), localPath));
    }

    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, pathInfo, localPath)) {
        return zffalse;
    }
    zfbool callbackSerializeDisable = ret.callbackSerializeDisable();
    ret.callbackSerializeDisable(zftrue);
    if(!ZFInputForPathInfoT(ret, ZFPathInfo(pathInfo.pathType(), pathDataAbs))) {
        return zffalse;
    }

    if(!callbackSerializeDisable) {
        ZFSerializableData customData;

        customData.attr(ZFSerializableKeyword_ZFIO_localPath, localPath);

        ret.callbackSerializeType(ZFCallbackSerializeType_ZFInputForLocal);
        ret.callbackSerializeData(customData);
    }

    return zftrue;
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFInputForLocal, ZFCallbackSerializeType_ZFInputForLocal) {
    ZFPathInfo pathInfo;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFIO_pathInfo, ZFPathInfo, pathInfo, {
                return zffalse;
            });

    zfstring localPath;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFIO_localPath, zfstring, localPath, {
                return zffalse;
            });

    ret.callbackSerializeDisable(zftrue);
    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, pathInfo, localPath)) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to get file path: %s, localPath: %s",
            pathInfo,
            localPath);
        return zffalse;
    }
    ZFInputForPathInfoT(ret, ZFPathInfo(pathInfo.pathType(), pathDataAbs));
    if(!ret) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s",
            ZFPathInfo(pathInfo.pathType(), pathDataAbs));
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForLocal
        , ZFMP_IN(const zfstring &, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        ) {
    ZFOutput ret;
    ZFOutputForLocalT(ret, localPath, pathInfo, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFOutputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfstring &, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        ) {
    if(zfstringIsEmpty(localPath)) {
        return zffalse;
    }
    if(!pathInfo) {
        return ZFOutputForPathInfoT(ret, ZFPathInfo(ZFPathType_file(), localPath), flags);
    }

    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, pathInfo, localPath)) {
        return zffalse;
    }
    zfbool callbackSerializeDisable = ret.callbackSerializeDisable();
    ret.callbackSerializeDisable(zftrue);
    if(!ZFOutputForPathInfoT(ret, ZFPathInfo(pathInfo.pathType(), pathDataAbs), flags)) {
        return zffalse;
    }

    if(!callbackSerializeDisable) {
        ZFSerializableData customData;

        customData.attr(ZFSerializableKeyword_ZFIO_localPath, localPath);

        ret.callbackSerializeType(ZFCallbackSerializeType_ZFOutputForLocal);
        ret.callbackSerializeData(customData);
    }

    return zftrue;
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFOutputForLocal, ZFCallbackSerializeType_ZFOutputForLocal) {
    ZFPathInfo pathInfo;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFIO_pathInfo, ZFPathInfo, pathInfo, {
                return zffalse;
            });

    zfstring localPath;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFIO_localPath, zfstring, localPath, {
                return zffalse;
            });

    ZFIOOpenOptionFlags flags = v_ZFIOOpenOption::e_Write;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFIO_flags, ZFIOOpenOptionFlags, flags, {
                return zffalse;
            });

    ret.callbackSerializeDisable(zftrue);
    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, pathInfo, localPath)) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to get file path: %s, localPath: %s",
            pathInfo,
            localPath);
        return zffalse;
    }
    ZFOutputForPathInfoT(ret, ZFPathInfo(pathInfo.pathType(), pathDataAbs), flags);
    if(!ret) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s",
            ZFPathInfo(pathInfo.pathType(), pathDataAbs));
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(void, ZFInputMarkSerializable
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    ret.pathInfo(pathInfo);

    zfstring pathInfoString;
    ZFPathInfoToStringT(pathInfoString, pathInfo);
    ret.callbackId(pathInfoString);

    if(!ret.callbackSerializeDisable()) {
        ZFSerializableData customData;

        customData.attr(ZFSerializableKeyword_ZFIO_pathInfo, pathInfoString);

        ret.callbackSerializeType(ZFCallbackSerializeType_ZFInputForPathInfo);
        ret.callbackSerializeData(customData);
    }
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFOutputMarkSerializable
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        ) {
    ret.pathInfo(pathInfo);

    zfstring pathInfoString;
    ZFPathInfoToStringT(pathInfoString, pathInfo);
    ret.callbackId(pathInfoString);

    if(!ret.callbackSerializeDisable()) {
        ZFSerializableData customData;

        customData.attr(ZFSerializableKeyword_ZFIO_pathInfo, pathInfoString);
        if(flags != v_ZFIOOpenOption::e_Write) {
            customData.attr(ZFSerializableKeyword_ZFIO_flags, ZFIOOpenOptionFlagsToString(flags));
        }

        ret.callbackSerializeType(ZFCallbackSerializeType_ZFOutputForPathInfo);
        ret.callbackSerializeData(customData);
    }
}

// ============================================================
// all printable chars (0x20 ~ 0x7E) except:
//   '%' : 0x25
//   '|' : 0x7C
const zfchar _ZFP_ZFPathInfoChainCharMap[256] = {
    /* 0x00 ~ 0x0F */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0x10 ~ 0x1F */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0x20 ~ 0x2F */
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /* 0x30 ~ 0x3F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /* 0x40 ~ 0x4F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /* 0x50 ~ 0x5F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /* 0x60 ~ 0x6F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /* 0x70 ~ 0x7F */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0,
    /* 0x80 ~ 0x8F */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0x90 ~ 0x9F */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0xA0 ~ 0xAF */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0xB0 ~ 0xBF */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0xC0 ~ 0xCF */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0xD0 ~ 0xDF */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0xE0 ~ 0xEF */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 0xF0 ~ 0xFF */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoChainDecode
        , ZFMP_OUT(ZFPathInfo &, chainPathInfo)
        , ZFMP_IN_OUT(zfstring &, pathData)
        , ZFMP_IN(const zfchar *, pathDataOrig)
        ) {
    zfstring chainPathInfoString;
    return ZFPathInfoChainDecodeString(chainPathInfoString, pathData, pathDataOrig)
        && ZFPathInfoFromStringT(chainPathInfo, chainPathInfoString, chainPathInfoString.length());
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoChainDecodeString
        , ZFMP_OUT(zfstring &, chainPathInfoString)
        , ZFMP_IN_OUT(zfstring &, pathData)
        , ZFMP_IN(const zfchar *, pathDataOrig)
        ) {
    zfindex pos = zfstringFindReversely(pathDataOrig, "|");
    if(pos == zfindexMax()) {
        return zffalse;
    }
    chainPathInfoString.capacity(pos);
    ZFCoreDataDecode(chainPathInfoString, pathDataOrig, pos);
    pathData += pathDataOrig + pos + 1;
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFPathInfoChainEncodeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, chainPathInfo)
        , ZFMP_IN(const zfchar *, pathData)
        ) {
    ret += chainPathInfo.pathType();
    ret += ZFSerializableKeyword_ZFPathInfo_separator;
    ZFCoreDataEncode(ret, chainPathInfo.pathData(), chainPathInfo.pathData().length(), ZFPathInfoChainCharMap());
    ret += '|';
    ret += pathData;
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFPathInfoChainEncode
        , ZFMP_IN(const ZFPathInfo &, chainPathInfo)
        , ZFMP_IN(const zfchar *, pathData)
        ) {
    zfstring ret;
    ZFPathInfoChainEncodeT(ret, chainPathInfo, pathData);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

