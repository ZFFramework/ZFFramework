#include "ZFPathType_res.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(res)

// ============================================================
static void *_ZFP_ZFPathInfoOpenForRes(
        ZF_IN const zfchar *filePath
        , ZF_IN_OPT ZFFileOpenOptionFlags flag = v_ZFFileOpenOption::e_Read
        , ZF_IN_OPT zfbool autoCreateParent = zftrue
        ) {
    if(flag != v_ZFFileOpenOption::e_Read) {
        return zfnull;
    }
    return ZFResOpen(filePath);
}
ZFPATHTYPE_FILEIO_REGISTER(res, ZFPathType_res()
        , ZFResIsExist
        , ZFResIsDir
        , ZFPathInfoCallbackToFileNameDefault
        , ZFPathInfoCallbackToChildDefault
        , ZFPathInfoCallbackToParentDefault
        , ZFPathInfoCallbackPathCreateDefault
        , ZFPathInfoCallbackRemoveDefault
        , ZFPathInfoCallbackMoveDefault
        , ZFResFindFirst
        , ZFResFindNext
        , ZFResFindClose
        , _ZFP_ZFPathInfoOpenForRes
        , ZFResClose
        , ZFResTell
        , ZFResSeek
        , ZFResRead
        , ZFPathInfoCallbackWriteDefault
        , ZFPathInfoCallbackFlushDefault
        , ZFResIsEof
        , ZFResIsError
        , ZFResSize
    )

// ============================================================
// ZFInputForRes
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForRes
        , ZFMP_IN(const zfchar *, resFilePath)
        )

ZF_NAMESPACE_GLOBAL_END

