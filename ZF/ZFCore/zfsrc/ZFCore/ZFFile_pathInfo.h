/**
 * @file ZFFile_pathInfo.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_pathInfo_h_
#define _ZFI_ZFFile_pathInfo_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackIsExist)(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackIsDir)(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackToFileName)(ZF_IN const zfchar *pathData,
                                               ZF_IN_OUT zfstring &fileName);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackToChild)(ZF_IN const zfchar *pathData,
                                            ZF_IN_OUT zfstring &pathDataChild,
                                            ZF_IN const zfchar *childName);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackToParent)(ZF_IN const zfchar *pathData,
                                             ZF_IN_OUT zfstring &pathDataParent);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackPathCreate)(ZF_IN const zfchar *pathData,
                                               ZF_IN_OPT zfbool autoMakeParent,
                                               ZF_OUT_OPT zfstring *errPos);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackRemove)(ZF_IN const zfchar *pathData,
                                           ZF_IN_OPT zfbool isRecursive,
                                           ZF_IN_OPT zfbool isForce,
                                           ZF_IN_OPT zfstring *errPos);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackFindFirst)(ZF_IN_OUT ZFFileFindData &fd,
                                              ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackFindNext)(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef void (*ZFPathInfoCallbackFindClose)(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef void *(*ZFPathInfoCallbackOpen)(ZF_IN const zfchar *pathData,
                                        ZF_IN_OPT ZFFileOpenOptionFlags flag,
                                        ZF_IN_OPT zfbool autoCreateParent);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackClose)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFPathInfoCallbackTell)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackSeek)(ZF_IN void *token,
                                         ZF_IN zfindex byteSize,
                                         ZF_IN_OPT ZFSeekPos position);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFPathInfoCallbackRead)(ZF_IN void *token,
                                          ZF_IN void *buf,
                                          ZF_IN zfindex maxByteSize);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFPathInfoCallbackWrite)(ZF_IN void *token,
                                           ZF_IN const void *src,
                                           ZF_IN_OPT zfindex maxByteSize);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef void (*ZFPathInfoCallbackFlush)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackIsEof)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackIsError)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFPathInfoCallbackSize)(ZF_IN void *token);

// ============================================================
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackIsExistDefault(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackIsDirDefault(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackToFileNameDefault(ZF_IN const zfchar *pathData,
                                                               ZF_IN_OUT zfstring &fileName);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackToChildDefault(ZF_IN const zfchar *pathData,
                                                            ZF_IN_OUT zfstring &pathDataChild,
                                                            ZF_IN const zfchar *childName);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackToParentDefault(ZF_IN const zfchar *pathData,
                                                             ZF_IN_OUT zfstring &pathDataParent);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackPathCreateDefault(ZF_IN const zfchar *pathData,
                                                               ZF_IN_OPT zfbool autoMakeParent = zftrue,
                                                               ZF_OUT_OPT zfstring *errPos = zfnull);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackRemoveDefault(ZF_IN const zfchar *pathData,
                                                           ZF_IN_OPT zfbool isRecursive = zftrue,
                                                           ZF_IN_OPT zfbool isForce = zffalse,
                                                           ZF_IN_OPT zfstring *errPos = zfnull);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackFindFirstDefault(ZF_IN_OUT ZFFileFindData &fd,
                                                              ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackFindNextDefault(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore void ZFPathInfoCallbackFindCloseDefault(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore void *ZFPathInfoCallbackOpenDefault(ZF_IN const zfchar *pathData,
                                                        ZF_IN_OPT ZFFileOpenOptionFlags flag = ZFFileOpenOption::e_Read,
                                                        ZF_IN_OPT zfbool autoCreateParent = zftrue);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackCloseDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfindex ZFPathInfoCallbackTellDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackSeekDefault(ZF_IN void *token,
                                                         ZF_IN zfindex byteSize,
                                                         ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfindex ZFPathInfoCallbackReadDefault(ZF_IN void *token,
                                                          ZF_IN void *buf,
                                                          ZF_IN zfindex maxByteSize);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfindex ZFPathInfoCallbackWriteDefault(ZF_IN void *token,
                                                           ZF_IN const void *src,
                                                           ZF_IN_OPT zfindex maxByteSize = zfindexMax());
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore void ZFPathInfoCallbackFlushDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackIsEofDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackIsErrorDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfindex ZFPathInfoCallbackSizeDefault(ZF_IN void *token);

// ============================================================
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFPathInfoIsExist,
                        ZFMP_IN(const ZFPathInfo &, pathInfo))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFPathInfoIsDir,
                        ZFMP_IN(const ZFPathInfo &, pathInfo))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoToFileName,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(zfstring &, fileName))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoToChild,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(zfstring &, pathDataChild),
                        ZFMP_IN(const zfchar *, childName))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoToChild,
                        ZFMP_IN_OUT(ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, childName))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoToParent,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(zfstring &, pathDataParent))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFPathInfoToParent,
                        ZFMP_IN_OUT(const ZFPathInfo &, pathInfo))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoPathCreate,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue),
                        ZFMP_OUT_OPT(zfstring *, errPos, zfnull))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFPathInfoRemove,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                        ZFMP_IN_OPT(zfbool, isForce, zffalse),
                        ZFMP_IN_OPT(zfstring *, errPos, zfnull))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoFindFirst,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(ZFFileFindData &, fd))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoFindNext,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(ZFFileFindData &, fd))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFPathInfoFindClose,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(ZFFileFindData &, fd))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, void *, ZFPathInfoOpen,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, ZFFileOpenOption::e_Read),
                        ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoClose,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfindex, ZFPathInfoTell,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFPathInfoSeek,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token),
                        ZFMP_IN(zfindex, byteSize),
                        ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfindex, ZFPathInfoRead,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token),
                        ZFMP_IN(void *, buf),
                        ZFMP_IN(zfindex, maxByteSize))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfindex, ZFPathInfoWrite,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token),
                        ZFMP_IN(const void *, src),
                        ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax()))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFPathInfoFlush,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoIsEof,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoIsError,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfindex, ZFPathInfoSize,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))

/**
 * @brief copy a file or directory from srcPath to dstPath
 *
 * if both src and dst exist, but one is file and another is dir,
 * return zffalse\n
 * merge directory if dst is an existing dir
 * (if isForce not set,
 * would return false if dst has a child file with the same path in src)\n
 * override file if dst is an existing file and isForce is zftrue
 * @note path must be well formed, use #ZFPathFormat if necessary
 * @note always use read/write to copy files,
 *   use #ZFFileCopy for better performance
 */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFCore, zfbool, ZFPathInfoCopy,
                        ZFMP_IN(const ZFPathInfo &, srcPath),
                        ZFMP_IN(const ZFPathInfo &, dstPath),
                        ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                        ZFMP_IN_OPT(zfbool, isForce, zffalse),
                        ZFMP_IN_OPT(ZFPathInfo *, errPos, zfnull))

// ============================================================
/** @brief callback data for #ZFPATHTYPE_FILEIO_REGISTER */
zfclassPOD ZFLIB_ZFCore ZFPathInfoImpl
{
public:
    ZFPathInfoCallbackIsExist callbackIsExist; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackIsDir callbackIsDir; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackToFileName callbackToFileName; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackToChild callbackToChild; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackToParent callbackToParent; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackPathCreate callbackPathCreate; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackRemove callbackRemove; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackFindFirst callbackFindFirst; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackFindNext callbackFindNext; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackFindClose callbackFindClose; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackOpen callbackOpen; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackClose callbackClose; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackTell callbackTell; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackSeek callbackSeek; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackRead callbackRead; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackWrite callbackWrite; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackFlush callbackFlush; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackIsEof callbackIsEof; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackIsError callbackIsError; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackSize callbackSize; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
};

extern ZFLIB_ZFCore void _ZFP_ZFPathInfoRegister(ZF_IN const zfchar *pathType,
                                                     ZF_IN const ZFPathInfoImpl &data);
extern ZFLIB_ZFCore void _ZFP_ZFPathInfoUnregister(ZF_IN const zfchar *pathType);

/**
 * @brief register pathType for generic file processing
 *
 * there may be many different file processing methods
 * (normal file, resource file, ftp file, etc),
 * which have quite similar read/write functions,
 * so we introduced this generic version to achieve advanced file processing\n
 * to use, register your path type by this macro,
 * then use #ZFPathInfoOpen series to process files
 *
 * note, for performance, these impl functions must support
 * path conversion inplace: (the input and output string may or may not be same object)
 * -  callbackToFileName
 * -  callbackToChild
 * -  callbackToParent
 */
#define ZFPATHTYPE_FILEIO_REGISTER(registerSig, pathType \
        , callbackIsExist_ \
        , callbackIsDir_ \
        , callbackToFileName_ \
        , callbackToChild_ \
        , callbackToParent_ \
        , callbackPathCreate_ \
        , callbackRemove_ \
        , callbackFindFirst_ \
        , callbackFindNext_ \
        , callbackFindClose_ \
        , callbackOpen_ \
        , callbackClose_ \
        , callbackTell_ \
        , callbackSeek_ \
        , callbackRead_ \
        , callbackWrite_ \
        , callbackFlush_ \
        , callbackIsEof_ \
        , callbackIsError_ \
        , callbackSize_ \
    ) \
    ZF_STATIC_REGISTER_INIT(ZFPathInfoReg_##registerSig) \
    { \
        ZFPathInfoImpl data; \
        data.callbackIsExist = callbackIsExist_; \
        data.callbackIsDir = callbackIsDir_; \
        data.callbackToFileName = callbackToFileName_; \
        data.callbackToChild = callbackToChild_; \
        data.callbackToParent = callbackToParent_; \
        data.callbackPathCreate = callbackPathCreate_; \
        data.callbackRemove = callbackRemove_; \
        data.callbackFindFirst = callbackFindFirst_; \
        data.callbackFindNext = callbackFindNext_; \
        data.callbackFindClose = callbackFindClose_; \
        data.callbackOpen = callbackOpen_; \
        data.callbackClose = callbackClose_; \
        data.callbackTell = callbackTell_; \
        data.callbackSeek = callbackSeek_; \
        data.callbackRead = callbackRead_; \
        data.callbackWrite = callbackWrite_; \
        data.callbackFlush = callbackFlush_; \
        data.callbackIsEof = callbackIsEof_; \
        data.callbackIsError = callbackIsError_; \
        data.callbackSize = callbackSize_; \
        _ZFP_ZFPathInfoRegister(pathType, data); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ZFPathInfoReg_##registerSig) \
    { \
        _ZFP_ZFPathInfoUnregister(pathType); \
    } \
    ZF_STATIC_REGISTER_END(ZFPathInfoReg_##registerSig)

/**
 * @brief get data registered by #ZFPATHTYPE_FILEIO_REGISTER
 */
extern ZFLIB_ZFCore const ZFPathInfoImpl *ZFPathInfoImplForPathType(ZF_IN const zfchar *pathType);
/** @brief see #ZFPathInfoImplGetAll */
extern ZFLIB_ZFCore void ZFPathInfoImplGetAllT(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &ret);
/**
 * @brief get all data registered by #ZFPATHTYPE_FILEIO_REGISTER
 */
inline ZFCoreArrayPOD<const zfchar *> ZFPathInfoImplGetAll(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFPathInfoImplGetAllT(ret);
    return ret;
}

// ============================================================
// utils
/**
 * @brief util to make a child path info relative to existing one,
 *   see also #ZFPathInfoCallbackToChild
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForLocalT,
                        ZFMP_OUT(ZFPathInfo &, ret),
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, childPath))
/**
 * @brief util to make a child path info relative to existing one,
 *   see also #ZFPathInfoCallbackToChild
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForLocalT,
                        ZFMP_OUT(zfstring &, ret),
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, childPath))
/**
 * @brief see #ZFPathInfoForLocalT
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFPathInfo, ZFPathInfoForLocal,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, childPath))

// ============================================================
// ZFInputForPathInfo
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node
 *       pathInfo="ZFPathInfo" // required, the path info
 *       flags="ZFFileOpenOptionFlags" // optional, ZFFileOpenOption::e_Read by default
 *   >
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputForPathInfo "ZFInputForPathInfo"

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFInput, ZFInputForPathInfo,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
/**
 * @brief see #ZFInputForPathInfo
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFInput, ZFInputForPathInfoString,
                        ZFMP_IN(const zfchar *, pathInfoString),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
/**
 * @brief see #ZFInputForPathInfo
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFInputForPathInfoStringT,
                        ZFMP_IN_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, pathInfoString),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
/**
 * @brief see #ZFInputForPathInfo
 *
 * this method is designed for performance for further impl, by:
 * -  allow set #ZFCallbackSerializeCustomTypeDisable
 * -  less extra copy on pathInfo
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFInputForPathInfoT,
                        ZFMP_IN_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, pathType),
                        ZFMP_IN(const zfchar *, pathData),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))

// ============================================================
// ZFOutputForPathInfo
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node
 *       pathInfo="ZFPathInfo" // required, the path info
 *       flags="ZFFileOpenOptionFlags" // optional, ZFFileOpenOption::e_Create by default
 *   >
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputForPathInfo "ZFOutputForPathInfo"

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFOutput, ZFOutputForPathInfo,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
/**
 * @brief see #ZFOutputForPathInfo
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFOutput, ZFOutputForPathInfoString,
                        ZFMP_IN(const zfchar *, pathInfoString),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
/**
 * @brief see #ZFOutputForPathInfo
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFOutputForPathInfoStringT,
                        ZFMP_IN_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, pathInfoString),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
/**
 * @brief see #ZFOutputForPathInfo
 *
 * this method is designed for performance for further impl, by:
 * -  allow set #ZFCallbackSerializeCustomTypeDisable
 * -  less extra copy on pathInfo
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFOutputForPathInfoT,
                        ZFMP_IN_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, pathType),
                        ZFMP_IN(const zfchar *, pathData),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))

// ============================================================
// ZFInputForLocal
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node
 *       filePath="zfstring" // required, the local path
 *       flags="ZFFileOpenOptionFlags" // optional, ZFFileOpenOption::e_Read by default
 *   >
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputForLocal "ZFInputForLocal"

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const zfchar *)localPath: local file path to use
 * -  (const ZFPathInfo *)pathInfo: see #ZFPathInfo, when not valid,
 *   this method behaves same as #ZFInputForFile
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, ZFInput, ZFInputForLocal,
                        ZFMP_IN(const zfchar *, localPath),
                        ZFMP_IN(const ZFPathInfo *, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
/**
 * @brief see #ZFInputForLocal
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFInputForLocalT,
                        ZFMP_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, localPath),
                        ZFMP_IN(const ZFPathInfo *, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))

/** @cond ZFPrivateDoc */
inline ZFInput ZFInputForLocal(ZF_IN const zfchar *localPath, ZF_IN const ZFPathInfo &pathInfo, ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read)
{
    return ZFInputForLocal(localPath, &pathInfo, flags);
}
inline zfbool ZFInputForLocalT(ZF_OUT ZFCallback &ret, ZF_IN const zfchar *localPath, ZF_IN const ZFPathInfo &pathInfo, ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read)
{
    return ZFInputForLocalT(ret, localPath, &pathInfo, flags);
}
/** @endcond */

// ============================================================
// ZFOutputForLocal
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node
 *       filePath="zfstring" // required, the local path
 *       flags="ZFFileOpenOptionFlags" // optional, ZFFileOpenOption::e_Create by default
 *   >
 *   </node>
 *   <node>
 *       <zfstring category="filePath" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Create by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputForLocal "ZFOutputForLocal"

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const zfchar *)localPath: local file path to use
 * -  (const ZFPathInfo *)pathInfo: see #ZFPathInfo, when not valid,
 *   this method behaves same as #ZFOutputForFile
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, ZFOutput, ZFOutputForLocal,
                        ZFMP_IN(const zfchar *, localPath),
                        ZFMP_IN(const ZFPathInfo *, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
/**
 * @brief see #ZFOutputForLocal
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFOutputForLocalT,
                        ZFMP_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, localPath),
                        ZFMP_IN(const ZFPathInfo *, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))

/** @cond ZFPrivateDoc */
inline ZFOutput ZFOutputForLocal(ZF_IN const zfchar *localPath, ZF_IN const ZFPathInfo &pathInfo, ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read)
{
    return ZFOutputForLocal(localPath, &pathInfo, flags);
}
inline zfbool ZFOutputForLocalT(ZF_OUT ZFCallback &ret, ZF_IN const zfchar *localPath, ZF_IN const ZFPathInfo &pathInfo, ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read)
{
    return ZFOutputForLocalT(ret, localPath, &pathInfo, flags);
}
/** @endcond */

// ============================================================
/**
 * @brief see #ZFSTYLE_DECODER_DEFINE
 *
 * styleKey should start with '@' and holds a #ZFPathInfo,
 * the result style object would be created by
 * #ZFInputForPathInfo and #ZFObjectIOLoad
 */
#define ZFStyleDecoder_pathInfo pathInfo

// ============================================================
/**
 * @brief util for impl to parse path info for performance
 */
extern ZFLIB_ZFCore zfbool ZFPathInfoParse(ZF_OUT zfstring &pathType,
                                           ZF_OUT const zfchar *&pathData,
                                           ZF_IN const zfchar *pathInfo);
/**
 * @brief util for impl to parse path info for performance
 */
extern ZFLIB_ZFCore zfbool ZFPathInfoParse(ZF_OUT zfstring &pathType,
                                           ZF_OUT zfstring &pathData,
                                           ZF_IN const zfchar *pathInfo);
/**
 * @brief util for impl to parse path info and verify by #ZFPathInfoImplForPathType,
 *   and return the result #ZFPathInfoImpl if success
 */
extern ZFLIB_ZFCore const ZFPathInfoImpl *ZFPathInfoVerify(ZF_IN const zfchar *pathInfo);
/**
 * @brief util for impl to parse path info and verify by #ZFPathInfoImplForPathType,
 *   and return the result #ZFPathInfoImpl if success
 */
extern ZFLIB_ZFCore const ZFPathInfoImpl *ZFPathInfoParseAndVerify(ZF_OUT zfstring &pathType,
                                                                   ZF_OUT const zfchar *&pathData,
                                                                   ZF_IN const zfchar *pathInfo);

// ============================================================
/**
 * @brief util for impl to parse chained path data
 *
 * chained path info format: 'pathType1:pathType2:pathData2|pathData1'\n
 * or, if recursively chained: 'pathType1:pathType2:pathType3:pathData3%7cpathData2|pathData1'\n
 * note if the chained path info contains the special '|' char, it must be escaped by '%7c',
 * or use #zfCoreDataEncode with #ZFPathInfoChainCharMap\n
 * \n
 * typical case:
 * @code
 *   // original path info:
 *   pathType1:pathType2:pathType3:pathData3%7cpathData2|pathData1
 *
 *   // pathDataOrig should be:
 *   pathType2:pathType3:pathData3%7cpathData2|pathData1
 *
 *   // after parse:
 *   //   chainPathType = pathType2
 *   //   chainPathData = pathType3:pathData3|pathData2
 *   //   pathData      = pathData1
 * @endcode
 */
extern ZFLIB_ZFCore zfbool ZFPathInfoChainParse(ZF_OUT zfstring &chainPathType,
                                                ZF_OUT zfstring &chainPathData,
                                                ZF_OUT const zfchar *&pathData,
                                                ZF_IN const zfchar *pathDataOrig);
/** @brief see #ZFPathInfoChainParse */
extern ZFLIB_ZFCore zfbool ZFPathInfoChainParse(ZF_OUT zfstring &chainPathInfoString,
                                                ZF_OUT const zfchar *&pathData,
                                                ZF_IN const zfchar *pathDataOrig);

extern ZFLIB_ZFCore const zfchar _ZFP_ZFPathInfoChainCharMap[256];
/** @brief see #ZFPathInfoChainParse */
#define ZFPathInfoChainCharMap() _ZFP_ZFPathInfoChainCharMap

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_pathInfo_h_

