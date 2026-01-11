#include "ZFIODef_fwd.h"
#include "ZFIODef_util.h"
#include "ZFIODef_pathInfo.h"

#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFIOOpenOption
ZFENUM_DEFINE_FLAGS(ZFIOOpenOption, ZFIOOpenOptionFlags)

// ============================================================
// ZFIOFindData
zfclassNotPOD _ZFP_ZFIOFindDataPrivate {
public:
    typedef zfstlhashmap<zfstring, zfauto> ImplTagMapType;

public:
    zfuint refCount;
    zfstring implName; // empty shows find not started
    void *implUserData;
    ZFIOFindData::Impl impl;
    ImplTagMapType *implTagMap;
public:
    _ZFP_ZFIOFindDataPrivate(void)
    : refCount(1)
    , implName()
    , implUserData(zfnull)
    , impl()
    , implTagMap(zfnull)
    {
    }
    ~_ZFP_ZFIOFindDataPrivate(void) {
        ZFCoreAssertWithMessage(this->implName.isEmpty()
            , "have you forgot to close find? module: %s"
            , this->implName
            );
        if(this->implTagMap != zfnull) {
            zfpoolDelete(this->implTagMap);
        }
    }
};

ZFIOFindData::ZFIOFindData(void)
: d(zfpoolNew(_ZFP_ZFIOFindDataPrivate))
{
}
ZFIOFindData::ZFIOFindData(ZF_IN ZFIOFindData const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFIOFindData &ZFIOFindData::operator = (ZF_IN ZFIOFindData const &ref) {
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFIOFindData::operator == (ZF_IN ZFIOFindData const &ref) const {
    return (d->impl.nativeFd == ref.d->impl.nativeFd);
}
ZFIOFindData::~ZFIOFindData(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
    }
}

void ZFIOFindData::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    if(this->isDir()) {
        ret += "(dir)";
    }
    else {
        ret += "(file)";
    }
    ret += this->name();
}

ZFIOFindData::Impl &ZFIOFindData::impl(void) const {
    return d->impl;
}
void ZFIOFindData::implCopy(ZF_IN const ZFIOFindData &ref) const {
    d->impl.name = ref.d->impl.name;
    d->impl.isDir = ref.d->impl.isDir;
}
const zfstring &ZFIOFindData::implName(void) const {
    return d->implName;
}
void *ZFIOFindData::implUserData(void) const {
    return d->implUserData;
}
void ZFIOFindData::implAttach(
        ZF_IN const zfstring &implName
        , ZF_IN_OPT void *implUserData /* = zfnull */
        ) {
    ZFCoreAssertWithMessage(d->implName.isEmpty()
        , "have you forgot to close find? current module: %s"
        , d->implName
        );
    ZFCoreAssert(implName);
    d->implName = implName;
    d->implUserData = implUserData;
}
void ZFIOFindData::implDetach(void) {
    d->implName.removeAll();
    d->implUserData = zfnull;
}
void *ZFIOFindData::implCheck(ZF_IN const zfstring &implName) const {
    ZFCoreAssertWithMessage(!d->implName.isEmpty(), "find not started");
    ZFCoreAssertWithMessage(zfstringIsEqual(d->implName, implName)
        , "have you forgot to close find? desired, %s, got: %s"
        , implName
        , d->implName
        );
    return d->implUserData;
}

void ZFIOFindData::implTag(
        ZF_IN const zfstring &key
        , ZF_IN ZFObject *value
        ) const {
    if(!key) {
        return;
    }
    if(value == zfnull) {
        if(d->implTagMap != zfnull) {
            d->implTagMap->erase(key);
        }
    }
    else {
        if(d->implTagMap == zfnull) {
            d->implTagMap = zfpoolNew(_ZFP_ZFIOFindDataPrivate::ImplTagMapType);
        }
        (*(d->implTagMap))[key] = value;
    }
}
zfany ZFIOFindData::implTag(ZF_IN const zfstring &key) const {
    if(!key || !d->implTagMap) {
        return zfnull;
    }
    else {
        _ZFP_ZFIOFindDataPrivate::ImplTagMapType::iterator it = d->implTagMap->find(key);
        if(it != d->implTagMap->end()) {
            return it->second;
        }
        else {
            return zfnull;
        }
    }
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFIOFindData, ZFIOFindData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFIOFindData, const zfstring &, name)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFIOFindData, zfbool, isDir)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFIOFindData, const zfstring &, implName)

// ============================================================
ZFOBJECT_REGISTER(ZFIOToken)

ZFEVENT_REGISTER(ZFIOToken, IOCloseOnPrepare)
ZFEVENT_REGISTER(ZFIOToken, IOCloseOnFinish)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFIOToken, zfstring, pathType)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFIOToken, zfstring, pathData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFIOToken, zfbool, ioClose)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFIOToken, zfindex, ioRead
        , ZFMP_OUT(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFIOToken, zfindex, ioWrite
        , ZFMP_OUT(const void *, src)
        , ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFIOToken, void, ioFlush)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFIOToken, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, seekPos, ZFSeekPosBegin)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFIOToken, zfindex, ioTell)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFIOToken, zfindex, ioSize)

// ============================================================
// default/dummy impl
zfbool ZFIOImpl::ioIsExistDefault(ZF_IN const zfstring &pathData) {
    return zffalse;
}
zfbool ZFIOImpl::ioIsDirDefault(ZF_IN const zfstring &pathData) {
    return zffalse;
}
zfbool ZFIOImpl::ioToFileNameDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfstring &pathData
        ) {
    return ZFFileNameOfT(ret, pathData);
}
zfbool ZFIOImpl::ioToChildDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfstring &pathData
        , ZF_IN const zfstring &childName
        ) {
    zfstring pathDataChild;
    pathDataChild += pathData;
    if(!zfstringIsEmpty(pathData) && !zfstringIsEmpty(childName)) {
        pathDataChild += '/';
    }
    pathDataChild += childName;
    if(&ret == &pathData) {
        ret.removeAll();
    }
    return ZFPathFormatRelativeT(ret, pathDataChild);
}
zfbool ZFIOImpl::ioToParentDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfstring &pathData
        ) {
    ZFPathParentOfT(ret, pathData);
    // always return true
    // typical case: pathData is file at root path
    return zftrue;
}
zfbool ZFIOImpl::ioPathCreateDefault(
        ZF_IN const zfstring &pathData
        , ZF_IN_OPT zfbool autoCreateParent /* = zftrue */
        ) {
    return zffalse;
}
zfbool ZFIOImpl::ioRemoveDefault(
        ZF_IN const zfstring &pathData
        , ZF_IN_OPT zfbool isRecursive /* = zftrue */
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        ) {
    return zffalse;
}
zfbool ZFIOImpl::ioMoveDefault(
        ZF_IN const zfstring &pathDataTo
        , ZF_IN const zfstring &pathDataFrom
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        ) {
    return zffalse;
}
zftimet ZFIOImpl::ioModTimeDefault(ZF_IN const zfstring &pathData) {
    return zftimetInvalid();
}
zfbool ZFIOImpl::ioModTimeDefault(ZF_IN const zfstring &pathData, ZF_IN zftimet time) {
    return zffalse;
}
zfbool ZFIOImpl::ioFindFirstDefault(
        ZF_IN_OUT ZFIOFindData &fd
        , ZF_IN const zfstring &pathData
        ) {
    return zffalse;
}
zfbool ZFIOImpl::ioFindNextDefault(ZF_IN_OUT ZFIOFindData &fd) {
    return zffalse;
}
void ZFIOImpl::ioFindCloseDefault(ZF_IN_OUT ZFIOFindData &fd) {
}
zfautoT<ZFIOToken> ZFIOImpl::ioOpenDefault(
        ZF_IN const zfstring &pathData
        , ZF_IN ZFIOOpenOptionFlags flags
        , ZF_IN_OPT zfbool autoCreateParent /* = zftrue */
        ) {
    return zfnull;
}
zfbool ZFIOImpl::ioCloseDefault(ZF_IN void *token) {
    return zffalse;
}
zfindex ZFIOImpl::ioReadDefault(
        ZF_IN void *token
        , ZF_IN void *buf
        , ZF_IN zfindex maxByteSize
        ) {
    return 0;
}
zfindex ZFIOImpl::ioWriteDefault(
        ZF_IN void *token
        , ZF_IN const void *src
        , ZF_IN_OPT zfindex maxByteSize /* = zfindexMax() */
        ) {
    return 0;
}
void ZFIOImpl::ioFlushDefault(ZF_IN void *token) {
}
zfbool ZFIOImpl::ioSeekDefault(
        ZF_IN void *token
        , ZF_IN zfindex byteSize
        , ZF_IN_OPT ZFSeekPos seekPos /* = ZFSeekPosBegin */
        ) {
    return zffalse;
}
zfindex ZFIOImpl::ioTellDefault(ZF_IN void *token) {
    return zfindexMax();
}
zfindex ZFIOImpl::ioSizeDefault(ZF_IN void *token) {
    return zfindexMax();
}

// ============================================================
// default impl for chained path info
zfbool ZFIOImpl::ioIsExistForChained(ZF_IN const zfstring &pathData) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
        && ZFIOIsExist(refPathInfo);
}
zfbool ZFIOImpl::ioIsDirForChained(ZF_IN const zfstring &pathData) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
        && ZFIOIsDir(refPathInfo);
}
zfbool ZFIOImpl::ioToFileNameForChained(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfstring &pathData
        ) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
        return zffalse;
    }
    if(&ret == &pathData) {
        zfstring tmp;
        if(!ZFIOToFileName(tmp, refPathInfo)) {
            return zffalse;
        }
        ret = tmp;
        return zftrue;
    }
    else {
        return ZFIOToFileName(ret, refPathInfo);
    }
}
zfbool ZFIOImpl::ioToChildForChained(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfstring &pathData
        , ZF_IN const zfstring &childName
        ) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
        return zffalse;
    }
    zfstring tmp;
    if(!ZFIOToChild(tmp, refPathInfo, childName)) {
        return zffalse;
    }
    refPathInfo.pathData(tmp);
    if(&ret == &pathData) {
        ret = ZFPathInfoChainEncode(refPathInfo, selfPathData);
    }
    else {
        ZFPathInfoChainEncodeT(ret, refPathInfo, selfPathData);
    }
    return zftrue;
}
zfbool ZFIOImpl::ioToParentForChained(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfstring &pathData
        ) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
        return zffalse;
    }
    zfstring tmp;
    if(!ZFIOToParent(tmp, refPathInfo)) {
        return zffalse;
    }
    refPathInfo.pathData(tmp);
    if(&ret == &pathData) {
        ret = ZFPathInfoChainEncode(refPathInfo, selfPathData);
    }
    else {
        ZFPathInfoChainEncodeT(ret, refPathInfo, selfPathData);
    }
    return zftrue;
}
zfbool ZFIOImpl::ioPathCreateForChained(
        ZF_IN const zfstring &pathData
        , ZF_IN_OPT zfbool autoCreateParent /* = zftrue */
        ) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
        && ZFIOPathCreate(refPathInfo, autoCreateParent);
}
zfbool ZFIOImpl::ioRemoveForChained(
        ZF_IN const zfstring &pathData
        , ZF_IN_OPT zfbool isRecursive /* = zftrue */
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        ) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
        && ZFIORemove(refPathInfo, isRecursive, isForce);
}
zfbool ZFIOImpl::ioMoveForChained(
        ZF_IN const zfstring &pathDataTo
        , ZF_IN const zfstring &pathDataFrom
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        ) {
    ZFPathInfo refPathInfoFrom;
    zfstring selfPathDataFrom;
    ZFPathInfo refPathInfoTo;
    zfstring selfPathDataTo;
    return ZFPathInfoChainDecode(refPathInfoFrom, selfPathDataFrom, pathDataFrom)
        && ZFPathInfoChainDecode(refPathInfoTo, selfPathDataTo, pathDataTo)
        && refPathInfoFrom.pathType() == refPathInfoTo.pathType()
        && ZFIOMove(refPathInfoTo.pathData(), refPathInfoFrom, isForce);
}
zftimet ZFIOImpl::ioModTimeForChained(ZF_IN const zfstring &pathData) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    if(ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
        return ZFIOModTime(refPathInfo);
    }
    else {
        return zftimetInvalid();
    }
}
zfbool ZFIOImpl::ioModTimeForChained(
        ZF_IN const zfstring &pathData
        , ZF_IN zftimet time
        ) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
        && ZFIOModTime(refPathInfo, time);
}
zfbool ZFIOImpl::ioFindFirstForChained(
        ZF_IN_OUT ZFIOFindData &fd
        , ZF_IN const zfstring &pathData
        ) {
    ZFPathInfo refPathInfo;
    zfstring selfPathData;
    if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
        return zffalse;
    }
    zfautoT<ZFIOImpl> refIOImpl = ZFIOImplForPathType(refPathInfo.pathType());
    if(refIOImpl == zfnull || !refIOImpl->ioFindFirst(fd, refPathInfo.pathData())) {
        return zffalse;
    }
    fd.implTag("ZFIOForChained", refIOImpl);
    return zftrue;
}
zfbool ZFIOImpl::ioFindNextForChained(ZF_IN_OUT ZFIOFindData &fd) {
    zfautoT<ZFIOImpl> refIOImpl = fd.implTag("ZFIOForChained");
    return refIOImpl && refIOImpl->ioFindNext(fd);
}
void ZFIOImpl::ioFindCloseForChained(ZF_IN_OUT ZFIOFindData &fd) {
    zfautoT<ZFIOImpl> refIOImpl = fd.implTag("ZFIOForChained");
    if(refIOImpl) {
        refIOImpl->ioFindClose(fd);
        fd.implTag("ZFIOForChained", zfnull);
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFIOImpl)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFIOImpl, zfstring, pathType)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFIOImpl, zfbool, ioIsExist
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFIOImpl, zfbool, ioIsDir
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFIOImpl, zfbool, ioToFileName
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFIOImpl, zfbool, ioToChild
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN(const zfstring &, childName)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFIOImpl, zfbool, ioToParent
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFIOImpl, zfbool, ioPathCreate
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFIOImpl, zfbool, ioRemove
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFIOImpl, zfbool, ioMove
        , ZFMP_IN(const zfstring &, pathDataTo)
        , ZFMP_IN(const zfstring &, pathDataFrom)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFIOImpl, zftimet, ioModTime
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFIOImpl, zfbool, ioModTime
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN(zftimet, time)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFIOImpl, zfbool, ioFindFirst
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFIOImpl, zfbool, ioFindNext
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFIOImpl, void, ioFindClose
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFIOImpl, zfautoT<ZFIOToken>, ioOpen
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN(ZFIOOpenOptionFlags, flags)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfbool, ioIsExistDefault
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfbool, ioIsDirDefault
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioToFileNameDefault
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfbool, ioToChildDefault
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN(const zfstring &, childName)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioToParentDefault
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioPathCreateDefault
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfbool, ioRemoveDefault
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfbool, ioMoveDefault
        , ZFMP_IN(const zfstring &, pathDataTo)
        , ZFMP_IN(const zfstring &, pathDataFrom)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zftimet, ioModTimeDefault
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioModTimeDefault
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN(zftimet, time)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioFindFirstDefault
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfbool, ioFindNextDefault
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, void, ioFindCloseDefault
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfautoT<ZFIOToken>, ioOpenDefault
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN(ZFIOOpenOptionFlags, flags)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfbool, ioCloseDefault
        , ZFMP_IN(void *, token)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfindex, ioReadDefault
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfindex, ioWriteDefault
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const void *, src)
        , ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, void, ioFlushDefault
        , ZFMP_IN(void *, token)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfbool, ioSeekDefault
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, seekPos, ZFSeekPosBegin)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfindex, ioTellDefault
        , ZFMP_IN(void *, token)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfindex, ioSizeDefault
        , ZFMP_IN(void *, token)
        )

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfbool, ioIsExistForChained
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfbool, ioIsDirForChained
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioToFileNameForChained
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfbool, ioToChildForChained
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN(const zfstring &, childName)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioToParentForChained
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioPathCreateForChained
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfbool, ioRemoveForChained
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_3(ZFIOImpl, zfbool, ioMoveForChained
        , ZFMP_IN(const zfstring &, pathDataTo)
        , ZFMP_IN(const zfstring &, pathDataFrom)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zftimet, ioModTimeForChained
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioModTimeForChained
        , ZFMP_IN(const zfstring &, pathData)
        , ZFMP_IN(zftimet, time)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_2(ZFIOImpl, zfbool, ioFindFirstForChained
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, zfbool, ioFindNextForChained
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_1(ZFIOImpl, void, ioFindCloseForChained
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        )

// ============================================================
// ZFIOImplForPathType
typedef zfstlhashmap<zfstring, zfautoT<ZFIOImpl> > _ZFP_ZFIOImplMapType;
static _ZFP_ZFIOImplMapType &_ZFP_ZFIOImplMap(void) {
    static _ZFP_ZFIOImplMapType m;
    return m;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFIOImpl>, ZFIOImplForPathType
        , ZFMP_IN(const zfstring &, pathType)
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFIOImplMapType &m = _ZFP_ZFIOImplMap();
    _ZFP_ZFIOImplMapType::iterator it = m.find(pathType);
    if(it != m.end()) {
        return it->second;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<zfautoT<ZFIOImpl> >, ZFIOImplList) {
    ZFCoreMutexLocker();
    ZFCoreArray<zfautoT<ZFIOImpl> > ret;
    _ZFP_ZFIOImplMapType &m = _ZFP_ZFIOImplMap();
    for(_ZFP_ZFIOImplMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ret.add(it->second);
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFIOImplRegister
        , ZFMP_IN(const zfstring &, pathType)
        , ZFMP_IN(ZFIOImpl *, ioImpl)
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFIOImplMapType &m = _ZFP_ZFIOImplMap();
    ZFCoreAssertWithMessageTrim(m.find(pathType) == m.end()
            , "[ZFIOImplRegister] already registered: %s"
            , pathType
            );
    ZFCoreAssert(ioImpl != zfnull);
    m[pathType] = ioImpl;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFIOImplUnregister
        , ZFMP_IN(const zfstring &, pathType)
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFIOImplMap().erase(pathType);
}

ZF_NAMESPACE_GLOBAL_END

