/**
 * @file ZFPathType_file.h
 * @brief #ZFPathInfo impl as normal file
 */

#ifndef _ZFI_ZFPathType_file_h_
#define _ZFI_ZFPathType_file_h_

#include "ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is the file path
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
ZFPATHTYPE_DECLARE(ZFLIB_ZFCore, file)

// ============================================================
// ZFInputForFile
/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const zfchar *)filePath: file path to use
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * auto setup callback cache id with res file path
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFCore, ZFInput, ZFInputForFile
        , ZFMP_IN(const zfchar *, filePath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(ZFPathType_file(), filePath), flags);
    return ret;
}

// ============================================================
// ZFOutputForFile
/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const zfchar *)filePath: file path to use
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFCore, ZFOutput, ZFOutputForFile
        , ZFMP_IN(const zfchar *, filePath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, ZFPathInfo(ZFPathType_file(), filePath), flags);
    return ret;
}

// ============================================================
/**
 * @brief util for #ZFPATHTYPE_FILEIO_REGISTER impl
 */
ZF_NAMESPACE_BEGIN_REGISTER(ZFFileIOImpl, ZF_NAMESPACE_GLOBAL)
/**
 * @brief util for #ZFPATHTYPE_FILEIO_REGISTER impl
 *
 * usage:
 * @code
 *   // supply a wrapper that has these methods
 *   zfclassNotPOD MyHolder {
 *   public:
 *       static void pathConvert(
 *               ZF_OUT zfstring &ret
 *               , ZF_IN const zfchar *pathData
 *               ) {
 *           ...
 *       }
 *   };
 *
 *   // use the file IO callback
 *   ZFPATHTYPE_FILEIO_REGISTER(registerSig, yourPathType,
 *       ZFFileIOImpl::FileIO<MyHolder>::callbackIsExist,
 *       ...)
 * @endcode
 */
template<typename T_Holder>
zfclassNotPOD FileIO {
public:
    /** @cond ZFPrivateDoc */
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData) {
        zfstring pathDataAbs;
        T_Holder::pathConvert(pathDataAbs, pathData);
        return ZFFileIsExist(pathDataAbs);
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData) {
        zfstring pathDataAbs;
        T_Holder::pathConvert(pathDataAbs, pathData);
        return ZFFileIsDir(pathDataAbs);
    }
    static zfbool callbackToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        return ZFPathInfoCallbackToFileNameDefault(ret, pathData);
    }
    static zfbool callbackToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            ) {
        return ZFPathInfoCallbackToChildDefault(ret, pathData, childName);
    }
    static zfbool callbackToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        return ZFPathInfoCallbackToParentDefault(ret, pathData);
    }
    static zfbool callbackPathCreate(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool autoMakeParent
            ) {
        zfstring pathDataAbs;
        T_Holder::pathConvert(pathDataAbs, pathData);
        if(!ZFPathCreate(pathDataAbs, autoMakeParent)) {
            return zffalse;
        }
        else {
            return zftrue;
        }
    }
    static zfbool callbackRemove(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool isRecursive
            , ZF_IN_OPT zfbool isForce
            ) {
        zfstring pathDataAbs;
        T_Holder::pathConvert(pathDataAbs, pathData);
        if(!ZFFileRemove(pathDataAbs, isRecursive, isForce)) {
            return zffalse;
        }
        else {
            return zftrue;
        }
    }
    static zfbool callbackMove(
            ZF_IN const zfchar *pathDataFrom
            , ZF_IN const zfchar *pathDataTo
            , ZF_IN_OPT zfbool isForce
            ) {
        zfstring pathDataFromAbs;
        T_Holder::pathConvert(pathDataFromAbs, pathDataFrom);
        zfstring pathDataToAbs;
        T_Holder::pathConvert(pathDataToAbs, pathDataTo);
        return ZFFileMove(pathDataFromAbs, pathDataToAbs, isForce);
    }
    static zfbool callbackFindFirst(
            ZF_IN_OUT ZFFileFindData &fd
            , ZF_IN const zfchar *pathData
            ) {
        zfstring pathDataAbs;
        T_Holder::pathConvert(pathDataAbs, pathData);
        if(ZFFileFindFirst(fd, pathDataAbs)) {
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd) {
        if(ZFFileFindNext(fd)) {
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd) {
        ZFFileFindClose(fd);
    }
    static void *callbackOpen(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT ZFFileOpenOptionFlags flag
            , ZF_IN_OPT zfbool autoCreateParent
            ) {
        zfstring pathDataAbs;
        T_Holder::pathConvert(pathDataAbs, pathData);
        return ZFFileOpen(pathDataAbs, flag, autoCreateParent);
    }
    static zfbool callbackClose(ZF_IN void *token) {
        return ZFFileClose(token);
    }
    static zfindex callbackSize(ZF_IN void *token) {
        return ZFFileSize(token);
    }
    static zfindex callbackTell(ZF_IN void *token) {
        return ZFFileTell(token);
    }
    static zfbool callbackSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos position
            ) {
        return ZFFileSeek(token, byteSize, position);
    }
    static zfindex callbackRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        return ZFFileRead(token, buf, maxByteSize);
    }
    static zfindex callbackWrite(
            ZF_IN void *token
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize
            ) {
        return ZFFileWrite(token, src, maxByteSize);
    }
    /** @endcond */
};
ZF_NAMESPACE_END(ZFFileIOImpl)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_file_h_

