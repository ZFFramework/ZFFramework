#include "ZFFile.h"
#include "ZFPathType_file.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfbool ZFPathInfoCallbackIsExistDefault(ZF_IN const zfchar *pathData) {
    return zffalse;
}
zfbool ZFPathInfoCallbackIsDirDefault(ZF_IN const zfchar *pathData) {
    return zffalse;
}
zfbool ZFPathInfoCallbackToFileNameDefault(
        ZF_IN const zfchar *pathData
        , ZF_OUT zfstring &fileName
        ) {
    return ZFFileNameOf(fileName, pathData);
}
zfbool ZFPathInfoCallbackToChildDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OUT zfstring &pathDataChild
        , ZF_IN const zfchar *childName
        ) {
    if(pathData >= pathDataChild.cString() && pathData < pathDataChild.cString() + pathDataChild.length()) {
        if(pathData == pathDataChild.cString()) {
            if(!pathDataChild.isEmpty()) {
                pathDataChild += ZFFileSeparator();
            }
            pathDataChild += childName;
        }
        else {
            zfstring t;
            if(!zfstringIsEmpty(pathData)) {
                t += pathData;
                t += ZFFileSeparator();
            }
            t += childName;
            pathDataChild = t;
        }
    }
    else {
        pathDataChild += pathData;
        if(!zfstringIsEmpty(pathData) && !zfstringIsEmpty(childName)) {
            pathDataChild += ZFFileSeparator();
        }
        pathDataChild += childName;
    }
    ZFPathFormatRelative(pathDataChild);
    return zftrue;
}
zfbool ZFPathInfoCallbackToParentDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OUT zfstring &pathDataParent
        ) {
    ZFPathParentOf(pathDataParent, pathData);
    // always return true
    // typical case: pathData is file at root path
    return zftrue;
}
zfbool ZFPathInfoCallbackPathCreateDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT zfbool autoMakeParent /* = zftrue */
        , ZF_OUT_OPT zfstring *errPos /* = zfnull */
        ) {
    return zffalse;
}
zfbool ZFPathInfoCallbackRemoveDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT zfbool isRecursive /* = zftrue */
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        , ZF_IN_OPT zfstring *errPos /* = zfnull */
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
        , ZF_IN_OPT ZFFileOpenOptionFlags flag /* = ZFFileOpenOption::e_Read */
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
static zfstlmap<zfstring, ZFPathInfoImpl> &_ZFP_ZFPathInfoImplMap(void) {
    static zfstlmap<zfstring, ZFPathInfoImpl> d;
    return d;
}
static ZFPathInfoImpl *_ZFP_ZFPathInfoImplForPathType(ZF_IN const zfchar *pathType) {
    zfstlmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    zfstlmap<zfstring, ZFPathInfoImpl>::iterator it = m.find(pathType);
    if(it != m.end()) {
        return &(it->second);
    }
    else {
        return zfnull;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFPathInfoIsExist
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackIsExistDefault(pathInfo.pathData);
    }
    else {
        return data->callbackIsExist(pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFPathInfoIsDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackIsDirDefault(pathInfo.pathData);
    }
    else {
        return data->callbackIsDir(pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoToFileName
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_OUT(zfstring &, fileName)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackToFileNameDefault(pathInfo.pathData, fileName);
    }
    else {
        return data->callbackToFileName(pathInfo.pathData, fileName);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoToChild
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(zfstring &, pathDataChild)
        , ZFMP_IN(const zfchar *, childName)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackToChildDefault(pathInfo.pathData, pathDataChild, childName);
    }
    else {
        return data->callbackToChild(pathInfo.pathData, pathDataChild, childName);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoToChild
        , ZFMP_IN_OUT(ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childName)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackToChildDefault(pathInfo.pathData, pathInfo.pathData, childName);
    }
    else {
        return data->callbackToChild(pathInfo.pathData, pathInfo.pathData, childName);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoToParent
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(zfstring &, pathDataParent)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackToParentDefault(pathInfo.pathData, pathDataParent);
    }
    else {
        return data->callbackToParent(pathInfo.pathData, pathDataParent);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFPathInfoToParent
        , ZFMP_IN_OUT(ZFPathInfo &, pathInfo)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackToParentDefault(pathInfo.pathData, pathInfo.pathData);
    }
    else {
        return data->callbackToParent(pathInfo.pathData, pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoPathCreate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue)
        , ZFMP_OUT_OPT(zfstring *, errPos, zfnull)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackPathCreateDefault(pathInfo.pathData, autoMakeParent, errPos);
    }
    else {
        return data->callbackPathCreate(pathInfo.pathData, autoMakeParent, errPos);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFPathInfoRemove
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        , ZFMP_IN_OPT(zfstring *, errPos, zfnull)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackRemoveDefault(pathInfo.pathData, isRecursive, isForce, errPos);
    }
    else {
        return data->callbackRemove(pathInfo.pathData, isRecursive, isForce, errPos);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoFindFirst
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackFindFirstDefault(fd, pathInfo.pathData);
    }
    else {
        return data->callbackFindFirst(fd, pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoFindNext
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackFindNextDefault(fd);
    }
    else {
        return data->callbackFindNext(fd);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFPathInfoFindClose
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        ZFPathInfoCallbackFindCloseDefault(fd);
    }
    else {
        data->callbackFindClose(fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(void *, ZFPathInfoOpen
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackOpenDefault(pathInfo.pathData, flag, autoCreateParent);
    }
    else {
        return data->callbackOpen(pathInfo.pathData, flag, autoCreateParent);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoClose
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackCloseDefault(token);
    }
    else {
        return data->callbackClose(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFPathInfoTell
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackTellDefault(token);
    }
    else {
        return data->callbackTell(token);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFPathInfoSeek
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackSeekDefault(token, byteSize, position);
    }
    else {
        return data->callbackSeek(token, byteSize, position);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFPathInfoRead
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackReadDefault(token, buf, maxByteSize);
    }
    else {
        return data->callbackRead(token, buf, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFPathInfoWrite
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const void *, src)
        , ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax())
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackWriteDefault(token, src, maxByteSize);
    }
    else {
        return data->callbackWrite(token, src, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFPathInfoFlush
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        ZFPathInfoCallbackFlushDefault(token);
    }
    else {
        data->callbackFlush(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoIsEof
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackIsEofDefault(token);
    }
    else {
        return data->callbackIsEof(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathInfoIsError
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackIsErrorDefault(token);
    }
    else {
        return data->callbackIsError(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFPathInfoSize
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        ) {
    ZFPathInfoImpl *data = _ZFP_ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data == zfnull) {
        return ZFPathInfoCallbackSizeDefault(token);
    }
    else {
        return data->callbackSize(token);
    }
}

// ============================================================
void _ZFP_ZFPathInfoRegister(
        ZF_IN const zfchar *pathType
        , ZF_IN const ZFPathInfoImpl &data
        ) {
    zfstlmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    zfCoreAssertWithMessage(m.find(pathType) == m.end(),
        "pathType \"%s\" already registered",
        pathType);
    zfCoreAssert(zftrue
            && data.callbackIsExist != zfnull
            && data.callbackIsDir != zfnull
            && data.callbackToFileName != zfnull
            && data.callbackToChild != zfnull
            && data.callbackToParent != zfnull
            && data.callbackPathCreate != zfnull
            && data.callbackRemove != zfnull
            && data.callbackFindFirst != zfnull
            && data.callbackFindNext != zfnull
            && data.callbackFindClose != zfnull
            && data.callbackOpen != zfnull
            && data.callbackClose != zfnull
            && data.callbackTell != zfnull
            && data.callbackSeek != zfnull
            && data.callbackRead != zfnull
            && data.callbackWrite != zfnull
            && data.callbackFlush != zfnull
            && data.callbackIsEof != zfnull
            && data.callbackIsError != zfnull
            && data.callbackSize != zfnull
        );
    m[pathType] = data;
}
void _ZFP_ZFPathInfoUnregister(ZF_IN const zfchar *pathType) {
    zfstlmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    m.erase(pathType);
}
const ZFPathInfoImpl *ZFPathInfoImplForPathType(ZF_IN const zfchar *pathType) {
    zfstlmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    zfstlmap<zfstring, ZFPathInfoImpl>::iterator it = m.find(pathType);
    if(it == m.end()) {
        return zfnull;
    }
    else {
        return &(it->second);
    }
}
void ZFPathInfoImplGetAllT(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &ret) {
    zfstlmap<zfstring, ZFPathInfoImpl> &m = _ZFP_ZFPathInfoImplMap();
    for(zfstlmap<zfstring, ZFPathInfoImpl>::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->first);
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(ZFPathInfo &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childPath)
        ) {
    ret.pathData.removeAll();
    if(ZFPathInfoForLocalT(ret.pathData, pathInfo, childPath)) {
        ret.pathType = pathInfo.pathType;
        return zftrue;
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childPath)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType);
    if(impl == zfnull) {
        return zffalse;
    }
    if(!impl->callbackIsDir(pathInfo.pathData)) {
        zfstring pathData;
        return impl->callbackToParent(pathInfo.pathData, pathData)
            && impl->callbackToChild(pathData, ret, childPath);
    }
    else {
        return impl->callbackToChild(pathInfo.pathData, ret, childPath);
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
zfclass _ZFP_I_ZFInputForPathInfoOwner : zfextends ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFInputForPathInfoOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void) {
        if(this->token != zfnull) {
            this->impl->callbackClose(this->token);
            this->impl = zfnull;
            this->token = zfnull;
        }
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    virtual zfbool openFile(
            ZF_IN const zfchar *pathType
            , ZF_IN const zfchar *pathData
            , ZF_IN ZFFileOpenOptionFlags flags
            ) {
        this->impl = ZFPathInfoImplForPathType(pathType);
        if(this->impl == zfnull) {
            return zffalse;
        }

        this->token = this->impl->callbackOpen(pathData, flags, zftrue);
        return (this->token != zfnull);
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
protected:
    _ZFP_I_ZFInputForPathInfoOwner(void)
    : impl(zfnull)
    , token(zfnull)
    {
    }
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForPathInfoOwner, zfindex, onInput
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    return this->impl->callbackRead(this->token, buf, count);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForPathInfoOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, pos)
        ) {
    return this->impl->callbackSeek(this->token, byteSize, pos);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForPathInfoOwner, zfindex, ioTell) {
    return this->impl->callbackTell(this->token);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForPathInfoOwner, zfindex, ioSize) {
    return this->impl->callbackSize(this->token);
}
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForPathInfoString
        , ZFMP_IN(const zfchar *, pathInfoString)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForPathInfoStringT(ret, pathInfoString, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFInputForPathInfoStringT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, pathInfoString)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        ) {
    zfstring pathType;
    const zfchar *pathData = zfnull;
    if(!ZFPathInfoParse(pathType, pathData, pathInfoString)) {
        return zffalse;
    }
    else {
        return ZFInputForPathInfoT(ret, pathType, pathData, flags);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFInputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, pathType)
        , ZFMP_IN(const zfchar *, pathData)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        ) {
    zfblockedAlloc(_ZFP_I_ZFInputForPathInfoOwner, inputOwner);
    if(!inputOwner->openFile(pathType, pathData, flags)) {
        return zffalse;
    }
    ret = ZFCallbackForMemberMethod(
        inputOwner, ZFMethodAccess(_ZFP_I_ZFInputForPathInfoOwner, onInput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, inputOwner);

    ret.pathInfo(pathType, pathData);

    zfstring callbackId;
    ZFPathInfoToString(callbackId, *ret.pathInfo());
    ret.callbackId(callbackId);

    if(!ret.callbackSerializeCustomDisabled()) {
        ZFSerializableData customData;
        customData.itemClass(ZFSerializableKeyword_node);

        customData.attr(ZFSerializableKeyword_ZFFileCallback_pathInfo, ZFPathInfoToString(*(ret.pathInfo())));

        ZFSerializableUtilSerializeAttributeToDataNoRef(customData, zfnull,
            ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read);

        ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFInputForPathInfo);
        ret.callbackSerializeCustomData(customData);
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFInputForPathInfo, ZFCallbackSerializeCustomType_ZFInputForPathInfo) {
    ZFPathInfo pathInfo;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        require, ZFSerializableKeyword_ZFFileCallback_pathInfo, ZFPathInfo, pathInfo);

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags);

    ret.callbackSerializeCustomDisable(zftrue);
    ZFInputForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
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
zfclass _ZFP_I_ZFOutputForPathInfoOwner : zfextends ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFOutputForPathInfoOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void) {
        if(this->token != zfnull) {
            this->impl->callbackClose(this->token);
            this->impl = zfnull;
            this->token = zfnull;
        }
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    virtual zfbool openFile(
            ZF_IN const zfchar *pathType
            , ZF_IN const zfchar *pathData
            , ZF_IN ZFFileOpenOptionFlags flags
            ) {
        this->impl = ZFPathInfoImplForPathType(pathType);
        if(this->impl == zfnull) {
            return zffalse;
        }

        this->token = this->impl->callbackOpen(pathData, flags, zftrue);
        return (this->token != zfnull);
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
protected:
    _ZFP_I_ZFOutputForPathInfoOwner(void)
    : impl(zfnull)
    , token(zfnull)
    {
    }
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, onOutput
        , ZFMP_IN(const void *, s)
        , ZFMP_IN(zfindex, count)
        ) {
    return this->impl->callbackWrite(this->token, s, count);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForPathInfoOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, pos)
        ) {
    return this->impl->callbackSeek(this->token, byteSize, pos);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, ioTell) {
    return this->impl->callbackTell(this->token);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, ioSize) {
    return this->impl->callbackSize(this->token);
}
ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForPathInfoString
        , ZFMP_IN(const zfchar *, pathInfoString)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoStringT(ret, pathInfoString, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFOutputForPathInfoStringT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, pathInfoString)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create)
        ) {
    zfstring pathType;
    const zfchar *pathData = zfnull;
    if(!ZFPathInfoParse(pathType, pathData, pathInfoString)) {
        return zffalse;
    }
    else {
        return ZFOutputForPathInfoT(ret, pathType, pathData, flags);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFOutputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, pathType)
        , ZFMP_IN(const zfchar *, pathData)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create)
        ) {
    zfblockedAlloc(_ZFP_I_ZFOutputForPathInfoOwner, outputOwner);
    if(!outputOwner->openFile(pathType, pathData, flags)) {
        return zffalse;
    }
    ret = ZFCallbackForMemberMethod(
        outputOwner, ZFMethodAccess(_ZFP_I_ZFOutputForPathInfoOwner, onOutput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, outputOwner);

    ret.pathInfo(pathType, pathData);

    zfstring callbackId;
    ZFPathInfoToString(callbackId, *ret.pathInfo());
    ret.callbackId(callbackId);

    if(!ret.callbackSerializeCustomDisabled()) {
        ZFSerializableData customData;
        customData.itemClass(ZFSerializableKeyword_node);

        customData.attr(ZFSerializableKeyword_ZFFileCallback_pathInfo, ZFPathInfoToString(*(ret.pathInfo())));

        ZFSerializableUtilSerializeAttributeToDataNoRef(customData, zfnull,
            ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create);

        ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFOutputForPathInfo);
        ret.callbackSerializeCustomData(customData);
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputForPathInfo, ZFCallbackSerializeCustomType_ZFOutputForPathInfo) {
    ZFPathInfo pathInfo;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        require, ZFSerializableKeyword_ZFFileCallback_pathInfo, ZFPathInfo, pathInfo);

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags);

    ret.callbackSerializeCustomDisable(zftrue);
    ZFOutputForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
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
        , ZFMP_IN(const ZFPathInfo *, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForLocalT(ret, localPath, pathInfo, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFInputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo *, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        ) {
    if(zfstringIsEmpty(localPath)) {
        return zffalse;
    }
    if(pathInfo == zfnull || pathInfo->isEmpty()) {
        return ZFInputForPathInfoT(ret, ZFPathType_file(), localPath, flags);
    }

    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, *pathInfo, localPath)) {
        return zffalse;
    }
    ret.callbackSerializeCustomDisable(zftrue);
    if(!ZFInputForPathInfoT(ret, pathInfo->pathType, pathDataAbs, flags)) {
        return zffalse;
    }

    if(!ret.callbackSerializeCustomDisabled()) {
        ZFSerializableData customData;
        customData.itemClass(ZFSerializableKeyword_node);

        customData.attr(ZFSerializableKeyword_ZFFileCallback_filePath, localPath);

        ZFSerializableUtilSerializeAttributeToDataNoRef(customData, zfnull,
            ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read);

        ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFInputForLocal);
        ret.callbackSerializeCustomData(customData);
    }

    return zftrue;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFInputForLocal, ZFCallbackSerializeCustomType_ZFInputForLocal) {
    const ZFPathInfo *pathInfo = serializableData.pathInfoCheck();
    if(pathInfo == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "serializableData does not contain path info");
        return zffalse;
    }

    const zfchar *localPath = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFFileCallback_filePath, outErrorHint, outErrorPos);
    if(localPath == zfnull) {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags);

    ret.callbackSerializeCustomDisable(zftrue);
    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, *pathInfo, localPath)) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to get file path: %s, localPath: %s",
            pathInfo,
            localPath);
        return zffalse;
    }
    ZFInputForPathInfoT(ret, pathInfo->pathType, pathDataAbs, flags);
    if(!ret) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s",
            ZFPathInfo(pathInfo->pathType, pathDataAbs));
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForLocal
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo *, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        ) {
    ZFOutput ret;
    ZFOutputForLocalT(ret, localPath, pathInfo, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFOutputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo *, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create)
        ) {
    if(zfstringIsEmpty(localPath)) {
        return zffalse;
    }
    if(pathInfo == zfnull || pathInfo->isEmpty()) {
        return ZFOutputForPathInfoT(ret, ZFPathType_file(), localPath, flags);
    }

    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, *pathInfo, localPath)) {
        return zffalse;
    }
    ret.callbackSerializeCustomDisable(zftrue);
    if(!ZFOutputForPathInfoT(ret, pathInfo->pathType, pathDataAbs, flags)) {
        return zffalse;
    }

    if(!ret.callbackSerializeCustomDisabled()) {
        ZFSerializableData customData;
        customData.itemClass(ZFSerializableKeyword_node);

        customData.attr(ZFSerializableKeyword_ZFFileCallback_filePath, localPath);

        ZFSerializableUtilSerializeAttributeToDataNoRef(customData, zfnull,
            ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create);

        ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFOutputForLocal);
        ret.callbackSerializeCustomData(customData);
    }

    return zftrue;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputForLocal, ZFCallbackSerializeCustomType_ZFOutputForLocal) {
    const ZFPathInfo *pathInfo = serializableData.pathInfoCheck();
    if(pathInfo == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "serializableData does not contain path info");
        return zffalse;
    }

    const zfchar *localPath = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFFileCallback_filePath, outErrorHint, outErrorPos);
    if(localPath == zfnull) {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFFileCallback_flags, ZFFileOpenOptionFlags, flags);

    ret.callbackSerializeCustomDisable(zftrue);
    zfstring pathDataAbs;
    if(!ZFPathInfoForLocalT(pathDataAbs, *pathInfo, localPath)) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to get file path: %s, localPath: %s",
            pathInfo,
            localPath);
        return zffalse;
    }
    ZFOutputForPathInfoT(ret, pathInfo->pathType, pathDataAbs, flags);
    if(!ret) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s",
            ZFPathInfo(pathInfo->pathType, pathDataAbs));
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFSTYLE_DECODER_DEFINE(ZFStyleDecoder_pathInfo, {
    if(*styleKey != '@') {
        return zffalse;
    }
    ZFInput input;
    input.callbackSerializeCustomDisable(zftrue);
    return ZFInputForPathInfoStringT(input, styleKey + 1)
        && ZFObjectIOLoadT(ret, input);
})

// ============================================================
zfbool ZFPathInfoParse(
        ZF_OUT zfstring &pathType
        , ZF_OUT const zfchar *&pathData
        , ZF_IN const zfchar *pathInfo
        ) {
    pathData = pathInfo;
    while(*pathData != ZFSerializableKeyword_ZFPathInfo_separator[0] && *pathData != '\0') {++pathData;}
    if(*pathData != ZFSerializableKeyword_ZFPathInfo_separator[0]) {
        pathData = zfnull;
        return zffalse;
    }
    else {
        pathType.append(pathInfo, pathData - pathInfo);
        ++pathData;
        return zftrue;
    }
}
zfbool ZFPathInfoParse(
        ZF_OUT zfstring &pathType
        , ZF_OUT zfstring &pathData
        , ZF_IN const zfchar *pathInfo
        ) {
    const zfchar *pathDataTmp = zfnull;
    if(ZFPathInfoParse(pathType, pathDataTmp, pathInfo)) {
        pathData += pathDataTmp;
        return zftrue;
    }
    else {
        return zffalse;
    }
}

const ZFPathInfoImpl *ZFPathInfoVerify(ZF_IN const zfchar *pathInfo) {
    zfstring pathType;
    const zfchar *pathData = zfnull;
    if(ZFPathInfoParse(pathType, pathData, pathInfo)) {
        return ZFPathInfoImplForPathType(pathType);
    }
    else {
        return zfnull;
    }
}

const ZFPathInfoImpl *ZFPathInfoParseAndVerify(
        ZF_OUT zfstring &pathType
        , ZF_OUT const zfchar *&pathData
        , ZF_IN const zfchar *pathInfo
        ) {
    zfindex pathTypeLenSaved = pathType.length();
    if(ZFPathInfoParse(pathType, pathData, pathInfo)) {
        const ZFPathInfoImpl *ret = ZFPathInfoImplForPathType(pathType.cString() + pathTypeLenSaved);
        if(ret == zfnull) {
            pathType.remove(pathTypeLenSaved);
            pathData = zfnull;
            return zfnull;
        }
        else {
            return ret;
        }
    }
    else {
        return zfnull;
    }
}

// ============================================================
// all printable chars (0x20 ~ 0x7E) except:
//   '%' : 0x25
//   '|' : 0x7C
const zfchar _ZFP_ZFPathInfoChainCharMap[256] = {
    // 0x00 ~ 0x0F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x10 ~ 0x1F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x20 ~ 0x2F
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x30 ~ 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x40 ~ 0x4F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x50 ~ 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x60 ~ 0x6F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x70 ~ 0x7F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0,
    // 0x80 ~ 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x90 ~ 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xA0 ~ 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xB0 ~ 0xBF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xC0 ~ 0xCF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xD0 ~ 0xDF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xE0 ~ 0xEF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xF0 ~ 0xFF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

zfbool ZFPathInfoChainParse(
        ZF_OUT zfstring &chainPathType
        , ZF_OUT zfstring &chainPathData
        , ZF_OUT const zfchar *&pathData
        , ZF_IN const zfchar *pathDataOrig
        ) {
    zfindex pos = zfstringFind(pathDataOrig, '|');
    if(pos == zfindexMax()) {
        return zffalse;
    }
    zfstring chainPathInfo;
    chainPathInfo.capacity(pos);
    zfCoreDataDecode(chainPathInfo, pathDataOrig, pos);
    if(!ZFPathInfoParse(chainPathType, chainPathData, chainPathInfo)) {
        return zffalse;
    }
    pathData = pathDataOrig + pos + 1;
    return zftrue;
}
zfbool ZFPathInfoChainParse(
        ZF_OUT zfstring &chainPathInfoString
        , ZF_OUT const zfchar *&pathData
        , ZF_IN const zfchar *pathDataOrig
        ) {
    zfindex pos = zfstringFind(pathDataOrig, '|');
    if(pos == zfindexMax()) {
        return zffalse;
    }
    else {
        chainPathInfoString.capacity(chainPathInfoString.length() + pos);
        zfCoreDataDecode(chainPathInfoString, pathDataOrig, pos);
        pathData = pathDataOrig + pos + 1;
        return zftrue;
    }
}

ZF_NAMESPACE_GLOBAL_END

