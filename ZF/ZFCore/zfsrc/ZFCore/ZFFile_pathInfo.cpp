#include "ZFFile.h"
#include "ZFPathType_file.h"

#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfbool ZFPathInfoCallbackIsExistDefault(ZF_IN const zfchar *pathData) {
    return zffalse;
}
zfbool ZFPathInfoCallbackIsDirDefault(ZF_IN const zfchar *pathData) {
    return zffalse;
}
zfbool ZFPathInfoCallbackToFileNameDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        ) {
    return ZFFileNameOfT(ret, pathData);
}
zfbool ZFPathInfoCallbackToChildDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        , ZF_IN const zfchar *childName
        ) {
    zfstring pathDataChild;
    pathDataChild += pathData;
    if(!zfstringIsEmpty(pathData) && !zfstringIsEmpty(childName)) {
        pathDataChild += '/';
    }
    pathDataChild += childName;
    if(pathData >= ret.cString() && pathData < ret.cString() + ret.length()) {
        ret.removeAll();
    }
    return ZFPathFormatRelativeT(ret, pathDataChild);
}
zfbool ZFPathInfoCallbackToParentDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        ) {
    ZFPathParentOfT(ret, pathData);
    // always return true
    // typical case: pathData is file at root path
    return zftrue;
}
zfbool ZFPathInfoCallbackPathCreateDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT zfbool autoMakeParent /* = zftrue */
        ) {
    return zffalse;
}
zfbool ZFPathInfoCallbackRemoveDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT zfbool isRecursive /* = zftrue */
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        ) {
    return zffalse;
}
zfbool ZFPathInfoCallbackMoveDefault(
        ZF_IN const zfchar *pathDataFrom
        , ZF_IN const zfchar *pathDataTo
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        ) {
    return zffalse;
}
zfbool ZFPathInfoCallbackFindFirstDefault(
        ZF_IN_OUT ZFFileFindData &fd
        , ZF_IN const zfchar *pathData
        ) {
    return zffalse;
}
zfbool ZFPathInfoCallbackFindNextDefault(ZF_IN_OUT ZFFileFindData &fd) {
    return zffalse;
}
void ZFPathInfoCallbackFindCloseDefault(ZF_IN_OUT ZFFileFindData &fd) {
}
void *ZFPathInfoCallbackOpenDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT ZFFileOpenOptionFlags flag /* = v_ZFFileOpenOption::e_Read */
        , ZF_IN_OPT zfbool autoCreateParent /* = zftrue */
        ) {
    return zfnull;
}
zfbool ZFPathInfoCallbackCloseDefault(ZF_IN void *token) {
    return zffalse;
}
zfindex ZFPathInfoCallbackTellDefault(ZF_IN void *token) {
    return zfindexMax();
}
zfbool ZFPathInfoCallbackSeekDefault(
        ZF_IN void *token
        , ZF_IN zfindex byteSize
        , ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */
        ) {
    return zffalse;
}
zfindex ZFPathInfoCallbackReadDefault(
        ZF_IN void *token
        , ZF_IN void *buf
        , ZF_IN zfindex maxByteSize
        ) {
    return 0;
}
zfindex ZFPathInfoCallbackWriteDefault(
        ZF_IN void *token
        , ZF_IN const void *src
        , ZF_IN_OPT zfindex maxByteSize /* = zfindexMax() */
        ) {
    return 0;
}
void ZFPathInfoCallbackFlushDefault(ZF_IN void *token) {
}
zfbool ZFPathInfoCallbackIsEofDefault(ZF_IN void *token) {
    return zftrue;
}
zfbool ZFPathInfoCallbackIsErrorDefault(ZF_IN void *token) {
    return zftrue;
}
zfindex ZFPathInfoCallbackSizeDefault(ZF_IN void *token) {
    return zfindexMax();
}

// ============================================================
static zfstlhashmap<zfstring, ZFPathInfoImpl> &_ZFP_ZFPathInfoImplMap(void) {
    static zfstlhashmap<zfstring, ZFPathInfoImpl> d;
    return d;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFPathInfoIsExist
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackIsExistDefault(pathInfo.pathData());
    }
    else {
        return impl->implIsExist(pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFPathInfoIsDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackIsDirDefault(pathInfo.pathData());
    }
    else {
        return impl->implIsDir(pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoToFileName
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackToFileNameDefault(ret, pathInfo.pathData());
    }
    else {
        return impl->implToFileName(ret, pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoToChild
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childName)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackToChildDefault(ret, pathInfo.pathData(), childName);
    }
    else {
        return impl->implToChild(ret, pathInfo.pathData(), childName);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoToParent
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackToParentDefault(ret, pathInfo.pathData());
    }
    else {
        return impl->implToParent(ret, pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoPathCreate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackPathCreateDefault(pathInfo.pathData(), autoMakeParent);
    }
    else {
        return impl->implPathCreate(pathInfo.pathData(), autoMakeParent);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoRemove
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackRemoveDefault(pathInfo.pathData(), isRecursive, isForce);
    }
    else {
        return impl->implRemove(pathInfo.pathData(), isRecursive, isForce);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoMove
        , ZFMP_IN(const ZFPathInfo &, pathInfoFrom)
        , ZFMP_IN(const zfchar *, pathDataTo)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfoFrom.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackMoveDefault(pathInfoFrom.pathData(), pathDataTo, isForce);
    }
    else {
        return impl->implMove(pathInfoFrom.pathData(), pathDataTo, isForce);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoFindFirst
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackFindFirstDefault(fd, pathInfo.pathData());
    }
    else {
        return impl->implFindFirst(fd, pathInfo.pathData());
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoFindNext
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackFindNextDefault(fd);
    }
    else {
        return impl->implFindNext(fd);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFPathInfoFindClose
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        ZFPathInfoCallbackFindCloseDefault(fd);
    }
    else {
        impl->implFindClose(fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(void *, ZFPathInfoOpen
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, v_ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackOpenDefault(pathInfo.pathData(), flag, autoCreateParent);
    }
    else {
        return impl->implOpen(pathInfo.pathData(), flag, autoCreateParent);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoClose
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackCloseDefault(token);
    }
    else {
        return impl->implClose(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFPathInfoTell
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackTellDefault(token);
    }
    else {
        return impl->implTell(token);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFPathInfoSeek
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackSeekDefault(token, byteSize, position);
    }
    else {
        return impl->implSeek(token, byteSize, position);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFPathInfoRead
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackReadDefault(token, buf, maxByteSize);
    }
    else {
        return impl->implRead(token, buf, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFPathInfoWrite
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const void *, src)
        , ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax())
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackWriteDefault(token, src, maxByteSize);
    }
    else {
        return impl->implWrite(token, src, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFPathInfoFlush
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        ZFPathInfoCallbackFlushDefault(token);
    }
    else {
        impl->implFlush(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoIsEof
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackIsEofDefault(token);
    }
    else {
        return impl->implIsEof(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoIsError
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackIsErrorDefault(token);
    }
    else {
        return impl->implIsError(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFPathInfoSize
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return ZFPathInfoCallbackSizeDefault(token);
    }
    else {
        return impl->implSize(token);
    }
}

// ============================================================
void _ZFP_ZFPathInfoRegister(
        ZF_IN const zfstring &pathType
        , ZF_IN const ZFPathInfoImpl &impl
        ) {
    zfstlhashmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    ZFCoreAssertWithMessage(m.find(pathType) == m.end(),
        "pathType \"%s\" already registered",
        pathType);
    m[pathType] = impl;
}
void _ZFP_ZFPathInfoUnregister(ZF_IN const zfstring &pathType) {
    zfstlhashmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    m.erase(pathType);
}

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPathInfoImpl, const ZFPathInfoImpl *)

ZFMETHOD_FUNC_DEFINE_1(const ZFPathInfoImpl *, ZFPathInfoImplForPathType
        , ZFMP_IN(const zfstring &, pathType)
        ) {
    zfstlhashmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    zfstlhashmap<zfstring, ZFPathInfoImpl>::iterator it = m.find(pathType);
    if(it == m.end()) {
        return zfnull;
    }
    else {
        return &(it->second);
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathInfoImplGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        ) {
    zfstlhashmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    for(zfstlhashmap<zfstring, ZFPathInfoImpl>::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
    }
}
ZFMETHOD_FUNC_INLINE_DEFINE_0(ZFCoreArray<zfstring>, ZFPathInfoImplGetAll)

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFPathInfo, ZFLocalPathInfo
        , ZFMP_IN_OPT(const zfstring &, localPath, zfnull)
        ) {
    return ZFPathInfo();
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(ZFPathInfo &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childPath)
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
        , ZFMP_IN(const zfchar *, childPath)
        ) {
    if(zfstringIsEmpty(childPath)) {
        ret = pathInfo.pathData();
        return zftrue;
    }
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return zffalse;
    }
    if(!impl->implIsDir(pathInfo.pathData())) {
        return impl->implToParent(ret, pathInfo.pathData())
            && impl->implToChild(ret, ret, childPath)
            ;
    }
    else {
        return impl->implToChild(ret, pathInfo.pathData(), childPath);
    }
}
ZFMETHOD_FUNC_DEFINE_2(ZFPathInfo, ZFPathInfoForLocal
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childPath)
        ) {
    ZFPathInfo ret;
    ZFPathInfoForLocalT(ret, pathInfo, childPath);
    return ret;
}

// ============================================================
// ZFInputForPathInfo
zfclass _ZFP_I_ZFInputForPathInfoOwner : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFInputForPathInfoOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void) {
        if(this->token != zfnull) {
            if(this->autoClose) {
                this->impl->implClose(this->token);
            }
            this->impl = zfnull;
            this->token = zfnull;
            this->autoClose = zftrue;
        }
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    zfbool setup(
            ZF_IN const ZFPathInfoImpl *impl
            , ZF_IN void *token
            , ZF_IN zfbool autoClose
            ) {
        if(impl && token) {
            this->impl = impl;
            this->token = token;
            this->autoClose = autoClose;
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
            , ZFMP_IN(ZFSeekPos, pos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)

private:
    const ZFPathInfoImpl *impl;
    void *token;
    zfbool autoClose;
protected:
    _ZFP_I_ZFInputForPathInfoOwner(void)
    : impl(zfnull)
    , token(zfnull)
    , autoClose(zftrue)
    {
    }
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForPathInfoOwner, zfindex, onInput
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    return this->impl->implRead(this->token, buf, count);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForPathInfoOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, pos)
        ) {
    return this->impl->implSeek(this->token, byteSize, pos);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForPathInfoOwner, zfindex, ioTell) {
    return this->impl->implTell(this->token);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForPathInfoOwner, zfindex, ioSize) {
    return this->impl->implSize(this->token);
}
ZFMETHOD_FUNC_DEFINE_4(ZFInput, ZFInputForPathInfoToken
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoClose, zftrue)
        ) {
    ZFInput ret;
    ZFInputForPathInfoTokenT(ret, token, pathInfo, flags, autoClose);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFInputForPathInfoTokenT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoClose, zftrue)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFInputForPathInfoOwner> inputOwner;
    if(!inputOwner->setup(ZFPathInfoImplForPathType(pathInfo.pathType()), token, autoClose)) {
        return zffalse;
    }
    ret = ZFCallbackForMemberMethod(
        inputOwner, ZFMethodAccess(_ZFP_I_ZFInputForPathInfoOwner, onInput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, inputOwner);

    ret.pathInfo(pathInfo);

    zfstring pathInfoString;
    ZFPathInfoToStringT(pathInfoString, pathInfo);
    ret.callbackId(pathInfoString);

    if(!ret.callbackSerializeDisable()) {
        ZFSerializableData customData;

        customData.attr(ZFSerializableKeyword_ZFFileCallback_pathInfo, pathInfoString);

        ZFSerializableUtilSerializeAttrToDataNoRef(customData, zfnull,
                ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read, {
                    return zffalse;
                });

        ret.callbackSerializeType(ZFCallbackSerializeType_ZFInputForPathInfo);
        ret.callbackSerializeData(customData);
    }

    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, pathInfo, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFInputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        ) {
    return ZFInputForPathInfoTokenT(ret, ZFPathInfoOpen(pathInfo, flags, zffalse), pathInfo, flags, zftrue);
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFInputForPathInfo, ZFCallbackSerializeType_ZFInputForPathInfo) {
    ZFPathInfo pathInfo;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFFileCallback_pathInfo, ZFPathInfo, pathInfo, {
                return zffalse;
            });

    ZFFileOpenOptionFlags flags = v_ZFFileOpenOption::e_Read;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, {
                return zffalse;
            });

    ret.callbackSerializeDisable(zftrue);
    ZFInputForPathInfoT(ret, pathInfo, flags);
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

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void) {
        if(this->token != zfnull) {
            if(this->autoClose) {
                this->impl->implClose(this->token);
            }
            this->impl = zfnull;
            this->token = zfnull;
            this->autoClose = zftrue;
        }
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    zfbool setup(
            ZF_IN const ZFPathInfoImpl *impl
            , ZF_IN void *token
            , ZF_IN zfbool autoClose
            ) {
        if(impl && token) {
            this->impl = impl;
            this->token = token;
            this->autoClose = autoClose;
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
            , ZFMP_IN(ZFSeekPos, pos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)

private:
    const ZFPathInfoImpl *impl;
    void *token;
    zfbool autoClose;
protected:
    _ZFP_I_ZFOutputForPathInfoOwner(void)
    : impl(zfnull)
    , token(zfnull)
    , autoClose(zftrue)
    {
    }
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, onOutput
        , ZFMP_IN(const void *, s)
        , ZFMP_IN(zfindex, count)
        ) {
    return this->impl->implWrite(this->token, s, count);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForPathInfoOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, pos)
        ) {
    return this->impl->implSeek(this->token, byteSize, pos);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, ioTell) {
    return this->impl->implTell(this->token);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, ioSize) {
    return this->impl->implSize(this->token);
}
ZFMETHOD_FUNC_DEFINE_4(ZFOutput, ZFOutputForPathInfoToken
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        , ZFMP_IN_OPT(zfbool, autoClose, zftrue)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoTokenT(ret, token, pathInfo, flags, autoClose);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFOutputForPathInfoTokenT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        , ZFMP_IN_OPT(zfbool, autoClose, zftrue)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    zfobj<_ZFP_I_ZFOutputForPathInfoOwner> outputOwner;
    if(!outputOwner->setup(ZFPathInfoImplForPathType(pathInfo.pathType()), token, autoClose)) {
        return zffalse;
    }
    ret = ZFCallbackForMemberMethod(
        outputOwner, ZFMethodAccess(_ZFP_I_ZFOutputForPathInfoOwner, onOutput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, outputOwner);

    ret.pathInfo(pathInfo);

    zfstring pathInfoString;
    ZFPathInfoToStringT(pathInfoString, pathInfo);
    ret.callbackId(pathInfoString);

    if(!ret.callbackSerializeDisable()) {
        ZFSerializableData customData;

        customData.attr(ZFSerializableKeyword_ZFFileCallback_pathInfo, pathInfoString);

        ZFSerializableUtilSerializeAttrToDataNoRef(customData, zfnull,
                ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create, {
                    return zffalse;
                });

        ret.callbackSerializeType(ZFCallbackSerializeType_ZFOutputForPathInfo);
        ret.callbackSerializeData(customData);
    }

    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, pathInfo, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFOutputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        ) {
    return ZFOutputForPathInfoTokenT(ret, ZFPathInfoOpen(pathInfo, flags, zftrue), pathInfo, flags, zftrue);
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFOutputForPathInfo, ZFCallbackSerializeType_ZFOutputForPathInfo) {
    ZFPathInfo pathInfo;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFFileCallback_pathInfo, ZFPathInfo, pathInfo, {
                return zffalse;
            });

    ZFFileOpenOptionFlags flags = v_ZFFileOpenOption::e_Create;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, {
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
ZFMETHOD_FUNC_DEFINE_3(ZFInput, ZFInputForLocal
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForLocalT(ret, localPath, pathInfo, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFInputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        ) {
    if(zfstringIsEmpty(localPath)) {
        return zffalse;
    }
    if(!pathInfo) {
        return ZFInputForPathInfoT(ret, ZFPathInfo(ZFPathType_file(), localPath), flags);
    }

    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, pathInfo, localPath)) {
        return zffalse;
    }
    ret.callbackSerializeDisable(zftrue);
    if(!ZFInputForPathInfoT(ret, ZFPathInfo(pathInfo.pathType(), pathDataAbs), flags)) {
        return zffalse;
    }

    if(!ret.callbackSerializeDisable()) {
        ZFSerializableData customData;

        customData.attr(ZFSerializableKeyword_ZFFileCallback_localPath, localPath);

        ZFSerializableUtilSerializeAttrToDataNoRef(customData, zfnull,
                ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read, {
                    return zffalse;
                });

        ret.callbackSerializeType(ZFCallbackSerializeType_ZFInputForLocal);
        ret.callbackSerializeData(customData);
    }

    return zftrue;
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFInputForLocal, ZFCallbackSerializeType_ZFInputForLocal) {
    const ZFPathInfo pathInfo = serializableData.pathInfoCheck();
    if(pathInfo == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "serializableData does not contain path info");
        return zffalse;
    }

    zfstring localPath = ZFSerializableUtil::requireAttr(serializableData, ZFSerializableKeyword_ZFFileCallback_localPath, outErrorHint, outErrorPos);
    if(localPath == zfnull) {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = v_ZFFileOpenOption::e_Read;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, {
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
    ZFInputForPathInfoT(ret, ZFPathInfo(pathInfo.pathType(), pathDataAbs), flags);
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
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        ) {
    ZFOutput ret;
    ZFOutputForLocalT(ret, localPath, pathInfo, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFOutputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
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
    ret.callbackSerializeDisable(zftrue);
    if(!ZFOutputForPathInfoT(ret, ZFPathInfo(pathInfo.pathType(), pathDataAbs), flags)) {
        return zffalse;
    }

    if(!ret.callbackSerializeDisable()) {
        ZFSerializableData customData;

        customData.attr(ZFSerializableKeyword_ZFFileCallback_localPath, localPath);

        ZFSerializableUtilSerializeAttrToDataNoRef(customData, zfnull,
                ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create, {
                    return zffalse;
                });

        ret.callbackSerializeType(ZFCallbackSerializeType_ZFOutputForLocal);
        ret.callbackSerializeData(customData);
    }

    return zftrue;
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFOutputForLocal, ZFCallbackSerializeType_ZFOutputForLocal) {
    const ZFPathInfo pathInfo = serializableData.pathInfoCheck();
    if(pathInfo == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "serializableData does not contain path info");
        return zffalse;
    }

    zfstring localPath = ZFSerializableUtil::requireAttr(serializableData, ZFSerializableKeyword_ZFFileCallback_localPath, outErrorHint, outErrorPos);
    if(localPath == zfnull) {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = v_ZFFileOpenOption::e_Create;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, {
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
    zfindex pos = zfstringFindReversely(pathDataOrig, '|');
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

