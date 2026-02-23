/**
 * @file ZFIOUtil.h
 * @brief debug server
 */

#ifndef _ZFI_ZFIOUtil_h_
#define _ZFI_ZFIOUtil_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief remove outdated files
 *
 * filter would be called before removing each file,
 * param0 is #v_ZFPathInfo that describe the child file,
 * and param1 is a #v_zfstring stores the relative path to source pathInfo\n
 * impl can:
 * -  set #ZFArgs::eventFiltered if process done
 * -  set #ZFArgs::result to any valid object, to prevent the file from being removed
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIORemoveOutdate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, filter, zfnull)
        , ZFMP_IN_OPT(zftimet, outdate, zftimetOneWeek())
        )

/**
 * @brief remove empty dir, excluding the pathInfo itself
 *
 * filter would be called before removing each dir,
 * param0 is #v_ZFPathInfo that describe the child file or dir,
 * and param1 is a #v_zfstring stores the relative path to source pathInfo\n
 * impl can:
 * -  set #ZFArgs::param0 or #ZFArgs::param1 to null, to prevent checking children of the file or dir (if isRecursive)
 * -  set #ZFArgs::eventFiltered if process done
 * -  set #ZFArgs::result to any valid object, to prevent the dir from being removed
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIORemoveEmptyDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, filter, zfnull)
        )

/**
 * @brief async version for #ZFIOForEach
 *   to prevent blocking UI update when performing heavy task
 *
 * fileCallback is same as #ZFIOForEach,
 * but would be called with some delay between each call for each file/dir\n
 * \n
 * fileCallback's sender would be the #v_ZFIOFindData,
 * param0 is #v_ZFPathInfo that describe the child file or dir,
 * and param1 is a #v_zfstring stores the relative path to source pathInfo
 * (may be empty string if pathInfo already points to a file)\n
 * impl can:
 * -  set #ZFArgs::param0 to null, to prevent checking children of the file or dir (if isRecursive)
 * -  set #ZFArgs::param0 to a #ZFTask, to perform custom task
 * -  set #ZFArgs::eventFiltered if process done
 * -  set #ZFArgs::result, which would be passed as param1 to finishCallback,
 *   note: the result would be kept acrossing different calls
 *
 * finishCallback's param0 would be a #v_ZFResultType indicates result type,
 * param1 would be the result passed from fileCallback
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIOForEachAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )
/** @brief see #ZFIOForEachAsync */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIOForEachFileAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )
/** @brief see #ZFIOForEachAsync */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFIOForEachDirAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        )

// ============================================================
/**
 * @brief async version of #ZFStyleLoad
 *
 * all items would still be loaded in main thread,
 * but would wait some time between each load task,
 * so that it's possible to update your UI during heavy load task,
 * item load events:
 * -  #ZFGlobalEvent::E_ZFStyleLoadItemBegin
 * -  #ZFGlobalEvent::E_ZFStyleLoadItemEnd
 *
 * finishCallback's param0 would be a #v_ZFResultType indicates result type
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFStyleLoadAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        )

/**
 * @brief util to perform #ZFStyleLoadAsync
 */
zfclass ZFLIB_ZFAppUtil ZFStyleLoadAsyncTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFStyleLoadAsyncTask, ZFTask)

public:
    /**
     * @brief array of #v_ZFPathInfo
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFArray>, taskList)

public:
    /**
     * @brief util to add task
     */
    ZFMETHOD_DECLARE_2(void, child
            , ZFMP_IN(const ZFPathInfo &, child)
            , ZFMP_IN(const zfstring &, relPath)
            )
    /**
     * @brief util to add task
     */
    ZFMETHOD_DECLARE_1(void, child
            , ZFMP_IN(const ZFPathInfo &, child)
            )
    /**
     * @brief util to add task
     */
    ZFMETHOD_DECLARE_1(void, child
            , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, child)
            )
    /**
     * @brief util to add task
     */
    ZFMETHOD_DECLARE_1(void, child
            , ZFMP_IN(ZFArray *, child)
            )

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with task */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const ZFPathInfo &, child)
            , ZFMP_IN(const zfstring &, relPath)
            )
    /** @brief construct with task */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(const ZFPathInfo &, child)
            )
    /** @brief construct with task */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, child)
            )
    /** @brief construct with task */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(ZFArray *, child)
            )
protected:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(void);
private:
    zfautoT<ZFTaskId> _task;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOUtil_h_

