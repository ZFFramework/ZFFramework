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
typedef zfbool (*ZFPathInfoCallbackToFileName)(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackToChild)(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        , ZF_IN const zfchar *childName
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackToParent)(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackPathCreate)(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT zfbool autoMakeParent /* = zftrue */
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackRemove)(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT zfbool isRecursive /* = zftrue */
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackMove)(
        ZF_IN const zfchar *pathDataFrom
        , ZF_IN const zfchar *pathDataTo
        , ZF_IN_OPT zfbool isForce /* = zftrue */
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackFindFirst)(
        ZF_IN_OUT ZFFileFindData &fd
        , ZF_IN const zfchar *pathData
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackFindNext)(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef void (*ZFPathInfoCallbackFindClose)(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef void *(*ZFPathInfoCallbackOpen)(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT ZFFileOpenOptionFlags flag /* = v_ZFFileOpenOption::e_Read */
        , ZF_IN_OPT zfbool autoCreateParent /* = zftrue */
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackClose)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFPathInfoCallbackSize)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFPathInfoCallbackTell)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFPathInfoCallbackSeek)(
        ZF_IN void *token
        , ZF_IN zfindex byteSize
        , ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFPathInfoCallbackRead)(
        ZF_IN void *token
        , ZF_IN void *buf
        , ZF_IN zfindex maxByteSize
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFPathInfoCallbackWrite)(
        ZF_IN void *token
        , ZF_IN const void *src
        , ZF_IN_OPT zfindex maxByteSize /* = zfindexMax() */
        );

// ============================================================
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackIsExistDefault(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackIsDirDefault(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackToFileNameDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackToChildDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        , ZF_IN const zfchar *childName
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackToParentDefault(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *pathData
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackPathCreateDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT zfbool autoMakeParent = zftrue
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackRemoveDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT zfbool isRecursive = zftrue
        , ZF_IN_OPT zfbool isForce = zftrue
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackMoveDefault(
        ZF_IN const zfchar *pathDataFrom
        , ZF_IN const zfchar *pathDataTo
        , ZF_IN_OPT zfbool isForce = zftrue
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackFindFirstDefault(
        ZF_IN_OUT ZFFileFindData &fd
        , ZF_IN const zfchar *pathData
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackFindNextDefault(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore void ZFPathInfoCallbackFindCloseDefault(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore void *ZFPathInfoCallbackOpenDefault(
        ZF_IN const zfchar *pathData
        , ZF_IN_OPT ZFFileOpenOptionFlags flag = v_ZFFileOpenOption::e_Read
        , ZF_IN_OPT zfbool autoCreateParent = zftrue
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackCloseDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfindex ZFPathInfoCallbackSizeDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfindex ZFPathInfoCallbackTellDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfbool ZFPathInfoCallbackSeekDefault(
        ZF_IN void *token
        , ZF_IN zfindex byteSize
        , ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfindex ZFPathInfoCallbackReadDefault(
        ZF_IN void *token
        , ZF_IN void *buf
        , ZF_IN zfindex maxByteSize
        );
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZFLIB_ZFCore zfindex ZFPathInfoCallbackWriteDefault(
        ZF_IN void *token
        , ZF_IN const void *src
        , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
        );

// ============================================================
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFPathInfoIsExist
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFPathInfoIsDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoToFileName
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoToChild
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childName)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoToParent
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoPathCreate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoRemove
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoMove
        , ZFMP_IN(const ZFPathInfo &, pathInfoFrom)
        , ZFMP_IN(const zfchar *, pathDataTo)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoFindFirst
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoFindNext
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFPathInfoFindClose
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, void *, ZFPathInfoOpen
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, v_ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFPathInfoClose
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfindex, ZFPathInfoSize
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfindex, ZFPathInfoTell
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFPathInfoSeek
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfindex, ZFPathInfoRead
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        )
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfindex, ZFPathInfoWrite
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const void *, src)
        , ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax())
        )

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
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFPathInfoCopy
        , ZFMP_IN(const ZFPathInfo &, srcPath)
        , ZFMP_IN(const ZFPathInfo &, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )

// ============================================================
/** @brief callback data for #ZFPATHTYPE_FILEIO_REGISTER */
zfclassLikePOD ZFLIB_ZFCore ZFPathInfoImpl {
public:
    ZFPathInfoCallbackIsExist callbackIsExist; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackIsDir callbackIsDir; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackToFileName callbackToFileName; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackToChild callbackToChild; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackToParent callbackToParent; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackPathCreate callbackPathCreate; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackRemove callbackRemove; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackMove callbackMove; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackFindFirst callbackFindFirst; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackFindNext callbackFindNext; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackFindClose callbackFindClose; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackOpen callbackOpen; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackClose callbackClose; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackSize callbackSize; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackTell callbackTell; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackSeek callbackSeek; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackRead callbackRead; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFPathInfoCallbackWrite callbackWrite; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */

public:
    /** @cond ZFPrivateDoc */
    void _ZFP_init(
            ZF_IN_OPT ZFPathInfoCallbackIsExist callbackIsExist = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackIsDir callbackIsDir = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackToFileName callbackToFileName = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackToChild callbackToChild = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackToParent callbackToParent = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackPathCreate callbackPathCreate = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackRemove callbackRemove = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackMove callbackMove = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackFindFirst callbackFindFirst = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackFindNext callbackFindNext = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackFindClose callbackFindClose = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackOpen callbackOpen = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackClose callbackClose = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackSize callbackSize = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackTell callbackTell = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackSeek callbackSeek = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackRead callbackRead = zfnull
            , ZF_IN_OPT ZFPathInfoCallbackWrite callbackWrite = zfnull
            ) {
        this->callbackIsExist = callbackIsExist;
        this->callbackIsDir = callbackIsDir;
        this->callbackToFileName = callbackToFileName;
        this->callbackToChild = callbackToChild;
        this->callbackToParent = callbackToParent;
        this->callbackPathCreate = callbackPathCreate;
        this->callbackRemove = callbackRemove;
        this->callbackMove = callbackMove;
        this->callbackFindFirst = callbackFindFirst;
        this->callbackFindNext = callbackFindNext;
        this->callbackFindClose = callbackFindClose;
        this->callbackOpen = callbackOpen;
        this->callbackClose = callbackClose;
        this->callbackSize = callbackSize;
        this->callbackTell = callbackTell;
        this->callbackSeek = callbackSeek;
        this->callbackRead = callbackRead;
        this->callbackWrite = callbackWrite;
    }
    /** @endcond */

public:
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implIsExist(ZF_IN const zfchar *pathData) const {
        return this->callbackIsExist(pathData);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implIsDir(ZF_IN const zfchar *pathData) const {
        return this->callbackIsDir(pathData);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) const {
        return this->callbackToFileName(ret, pathData);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            ) const {
        return this->callbackToChild(ret, pathData, childName);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) const {
        return this->callbackToParent(ret, pathData);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implPathCreate(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool autoMakeParent = zftrue
            ) const {
        return this->callbackPathCreate(pathData, autoMakeParent);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implRemove(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) const {
        return this->callbackRemove(pathData, isRecursive, isForce);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implMove(
            ZF_IN const zfchar *pathDataFrom
            , ZF_IN const zfchar *pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) const {
        return this->callbackMove(pathDataFrom, pathDataTo, isForce);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implFindFirst(
            ZF_IN_OUT ZFFileFindData &fd
            , ZF_IN const zfchar *pathData
            ) const {
        return this->callbackFindFirst(fd, pathData);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implFindNext(ZF_IN_OUT ZFFileFindData &fd) const {
        return this->callbackFindNext(fd);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline void implFindClose(ZF_IN_OUT ZFFileFindData &fd) const {
        return this->callbackFindClose(fd);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline void *implOpen(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT ZFFileOpenOptionFlags flag = v_ZFFileOpenOption::e_Read
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) const {
        return this->callbackOpen(pathData, flag, autoCreateParent);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implClose(ZF_IN void *token) const {
        return this->callbackClose(token);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfindex implSize(ZF_IN void *token) const {
        return this->callbackSize(token);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfindex implTell(ZF_IN void *token) const {
        return this->callbackTell(token);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfbool implSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin
            ) const {
        return this->callbackSeek(token, byteSize, position);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfindex implRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) const {
        return this->callbackRead(token, buf, maxByteSize);
    }
    /** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    inline zfindex implWrite(
            ZF_IN void *token
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) const {
        return this->callbackWrite(token, src, maxByteSize);
    }
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFPathInfoImpl, const ZFPathInfoImpl *)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFPathInfoImpl, const ZFPathInfoImpl *)

extern ZFLIB_ZFCore void _ZFP_ZFPathInfoRegister(
        ZF_IN const zfstring &pathType
        , ZF_IN const ZFPathInfoImpl &data
        );
extern ZFLIB_ZFCore void _ZFP_ZFPathInfoUnregister(ZF_IN const zfstring &pathType);

/**
 * @brief register pathType for generic file processing
 *
 * there may be many different file processing methods
 * (normal file, resource file, ftp file, etc),
 * which have quite similar read/write functions,
 * so we introduced this generic version to achieve advanced file processing\n
 * to use, register your path type by this macro,
 * then use #ZFPathInfoOpen series to process files
 */
#define ZFPATHTYPE_FILEIO_REGISTER(registerSig, pathType, ...) \
    ZF_STATIC_REGISTER_INIT(ZFPathInfoReg_##registerSig) { \
        ZFPathInfoImpl data; \
        data._ZFP_init(__VA_ARGS__); \
        _ZFP_ZFPathInfoRegister(pathType, data); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ZFPathInfoReg_##registerSig) { \
        _ZFP_ZFPathInfoUnregister(pathType); \
    } \
    ZF_STATIC_REGISTER_END(ZFPathInfoReg_##registerSig)

/**
 * @brief get data registered by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, const ZFPathInfoImpl *, ZFPathInfoImplForPathType
        , ZFMP_IN(const zfstring &, pathType)
        )
/**
 * @brief get all data registered by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFPathInfoImplGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        )
/**
 * @brief get all data registered by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_INLINE_DECLARE_0(ZFLIB_ZFCore, ZFCoreArray<zfstring>, ZFPathInfoImplGetAll) {
    ZFCoreArray<zfstring> ret;
    ZFPathInfoImplGetAllT(ret);
    return ret;
}

// ============================================================
// utils
/**
 * @brief return local path info for current context
 *
 * this method is designed for script only,
 * when called from cpp side,
 * it always return a dummy path info
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFPathInfo, ZFLocalPathInfo
        , ZFMP_IN_OPT(const zfstring &, localPath, zfnull)
        )
/**
 * @brief util to make a child path info relative to existing one,
 *   see also #ZFPathInfoCallbackToChild
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(ZFPathInfo &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childPath)
        )
/**
 * @brief util to make a child path info relative to existing one,
 *   see also #ZFPathInfoCallbackToChild
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childPath)
        )
/**
 * @brief see #ZFPathInfoForLocalT
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFPathInfo, ZFPathInfoForLocal
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfchar *, childPath)
        )

// ============================================================
// ZFInputForPathInfo
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFInputForPathInfo"
 *       pathInfo="ZFPathInfo" // required, the path info
 *       flags="ZFFileOpenOptionFlags" // optional, v_ZFFileOpenOption::e_Read by default
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFInputForPathInfo "ZFInputForPathInfo"

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
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFInput, ZFInputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        )
/**
 * @brief see #ZFInputForPathInfo
 *
 * this method is designed for performance for further impl, by:
 * -  allow set #ZFCallback::callbackSerializeDisable
 * -  less extra copy on pathInfo
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFInputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        )

/** @brief see #ZFInputForPathInfo */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, ZFInput, ZFInputForPathInfoToken
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoClose, zftrue)
        )
/** @brief see #ZFInputForPathInfo */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFCore, zfbool, ZFInputForPathInfoTokenT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoClose, zftrue)
        )

// ============================================================
// ZFOutputForPathInfo
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFOutputForPathInfo"
 *       pathInfo="ZFPathInfo" // required, the path info
 *       flags="ZFFileOpenOptionFlags" // optional, v_ZFFileOpenOption::e_Create by default
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFOutputForPathInfo "ZFOutputForPathInfo"

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
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFOutput, ZFOutputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        )
/**
 * @brief see #ZFOutputForPathInfo
 *
 * this method is designed for performance for further impl, by:
 * -  allow set #ZFCallback::callbackSerializeDisable
 * -  less extra copy on pathInfo
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFOutputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        )

/** @brief see #ZFOutputForPathInfo */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, ZFOutput, ZFOutputForPathInfoToken
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        , ZFMP_IN_OPT(zfbool, autoClose, zftrue)
        )
/** @brief see #ZFOutputForPathInfo */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFCore, zfbool, ZFOutputForPathInfoTokenT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        , ZFMP_IN_OPT(zfbool, autoClose, zftrue)
        )

// ============================================================
// ZFInputForLocal
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFInputForLocal"
 *       localPath="zfstring" // required, the local path
 *       flags="ZFFileOpenOptionFlags" // optional, v_ZFFileOpenOption::e_Read by default
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFInputForLocal "ZFInputForLocal"

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const zfchar *)localPath: local file path to use
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo, when not valid,
 *   this method behaves same as #ZFInputForFile
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, ZFInput, ZFInputForLocal
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        )
/**
 * @brief see #ZFInputForLocal
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFInputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        )

// ============================================================
// ZFOutputForLocal
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFOutputForLocal"
 *       localPath="zfstring" // required, the local path
 *       flags="ZFFileOpenOptionFlags" // optional, v_ZFFileOpenOption::e_Create by default
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFOutputForLocal "ZFOutputForLocal"

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const zfchar *)localPath: local file path to use
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo, when not valid,
 *   this method behaves same as #ZFOutputForFile
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, ZFOutput, ZFOutputForLocal
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        )
/**
 * @brief see #ZFOutputForLocal
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFOutputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfchar *, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        )

// ============================================================
/**
 * @brief util for impl to parse chained path data
 *
 * chained path info format: 'pathType1:pathType2:pathData2|pathData1'\n
 * or, if recursively chained: 'pathType1:pathType2:pathType3:pathData3%7cpathData2|pathData1'\n
 * note if the chained path info contains the special '|' char, it must be escaped by '%7c',
 * or use #ZFCoreDataEncode with #ZFPathInfoChainCharMap\n
 * \n
 * typical case:
 * @code
 *   // if original path info is:
 *   pathType1:pathType2:pathType3:pathData3%7cpathData2|pathData1
 *   // the pathDataOrig should be:
 *             pathType2:pathType3:pathData3%7cpathData2|pathData1
 *
 *   // after decode, chainPathInfo should be:
 *             pathType2
 *                       pathType3:pathData3|pathData2
 *   // pathData should be:
 *                                                       pathData1
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoChainDecode
        , ZFMP_OUT(ZFPathInfo &, chainPathInfo)
        , ZFMP_IN_OUT(zfstring &, pathData)
        , ZFMP_IN(const zfchar *, pathDataOrig)
        )
/** @brief see #ZFPathInfoChainDecode */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoChainDecodeString
        , ZFMP_OUT(zfstring &, chainPathInfoString)
        , ZFMP_IN_OUT(zfstring &, pathData)
        , ZFMP_IN(const zfchar *, pathDataOrig)
        )
/** @brief see #ZFPathInfoChainDecode */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, void, ZFPathInfoChainEncodeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, chainPathInfo)
        , ZFMP_IN(const zfchar *, pathData)
        )
/** @brief see #ZFPathInfoChainDecode */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfstring, ZFPathInfoChainEncode
        , ZFMP_IN(const ZFPathInfo &, chainPathInfo)
        , ZFMP_IN(const zfchar *, pathData)
        )

extern ZFLIB_ZFCore const zfchar _ZFP_ZFPathInfoChainCharMap[256];
/**
 * @brief see #ZFPathInfoChainDecode
 *
 * keep all printable chars except:
 * -  `%`
 * -  `|`
 */
#define ZFPathInfoChainCharMap() _ZFP_ZFPathInfoChainCharMap

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_pathInfo_h_

