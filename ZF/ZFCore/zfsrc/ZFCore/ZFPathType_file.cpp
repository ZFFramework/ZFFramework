#include "ZFPathType_file.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFFileIOImpl)
ZF_NAMESPACE_END(ZFFileIOImpl)

ZFPATHTYPE_DEFINE(file)

// ============================================================
ZFPATHTYPE_FILEIO_REGISTER(file, ZFPathType_file()
        , ZFFileIsExist
        , ZFFileIsDir
        , ZFPathInfoCallbackToFileNameDefault
        , ZFPathInfoCallbackToChildDefault
        , ZFPathInfoCallbackToParentDefault
        , ZFPathCreate
        , ZFFileRemove
        , ZFFileMove
        , ZFFileFindFirst
        , ZFFileFindNext
        , ZFFileFindClose
        , ZFFileOpen
        , ZFFileClose
        , ZFFileTell
        , ZFFileSeek
        , ZFFileRead
        , ZFFileWrite
        , ZFFileFlush
        , ZFFileIsEof
        , ZFFileIsError
        , ZFFileSize
    )

// ============================================================
// ZFInputForFile
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFInput, ZFInputForFile
        , ZFMP_IN(const zfchar *, filePath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        )

// ============================================================
// ZFOutputForFile
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFOutput, ZFOutputForFile
        , ZFMP_IN(const zfchar *, filePath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create)
        )

ZF_NAMESPACE_GLOBAL_END

