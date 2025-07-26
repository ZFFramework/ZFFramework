#include "ZFPathType_res_raw.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(resraw)

// ============================================================
static void *_ZFP_ZFPathInfoOpenForResRaw(
        ZF_IN const zfchar *filePath
        , ZF_IN_OPT ZFFileOpenOptionFlags flag = v_ZFFileOpenOption::e_Read
        , ZF_IN_OPT zfbool autoCreateParent = zftrue
        ) {
    if(flag != v_ZFFileOpenOption::e_Read) {
        return zfnull;
    }
    return ZFResRaw::ZFResOpen(filePath);
}
ZFPATHTYPE_FILEIO_REGISTER(resraw, ZFPathType_resraw()
        , ZFResRaw::ZFResIsExist
        , ZFResRaw::ZFResIsDir
        , ZFPathInfoCallbackToFileNameDefault
        , ZFPathInfoCallbackToChildDefault
        , ZFPathInfoCallbackToParentDefault
        , ZFPathInfoCallbackPathCreateDefault
        , ZFPathInfoCallbackRemoveDefault
        , ZFPathInfoCallbackMoveDefault
        , ZFResRaw::ZFResFindFirst
        , ZFResRaw::ZFResFindNext
        , ZFResRaw::ZFResFindClose
        , _ZFP_ZFPathInfoOpenForResRaw
        , ZFResRaw::ZFResClose
        , ZFResRaw::ZFResSize
        , ZFResRaw::ZFResTell
        , ZFResRaw::ZFResSeek
        , ZFResRaw::ZFResRead
        , ZFPathInfoCallbackWriteDefault
    )

// ============================================================
// ZFInputForResRaw
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForResRaw
        , ZFMP_IN(const zfstring &, resFilePath)
        )

ZF_NAMESPACE_GLOBAL_END

