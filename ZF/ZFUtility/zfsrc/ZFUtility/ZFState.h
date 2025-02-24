/**
 * @file ZFState.h
 * @brief util to store local state
 */

#ifndef _ZFI_ZFState_h_
#define _ZFI_ZFState_h_

#include "ZFUtilityDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFStatePrivate;
/**
 * @brief util to store local state
 */
zfclass ZFLIB_ZFUtility ZFState : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFState, ZFObject)
    ZFOBJECT_SINGLETON_DECLARE(ZFState, instance)

public:
    /**
     * @brief path to store state file
     *
     * when invalid, `ZFPathInfo(ZFPathType_settingPath, 'ZFState')`
     * would be used\n
     * note the old file won't be deleted automatically when this config changed,
     * you should delete it manually if necessary\n
     * note changing this would cause a complete state reload,
     * merging existing state
     */
    ZFPROPERTY_ASSIGN(ZFPathInfo, stateFile)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFPathInfo, stateFile)
    /**
     * @brief util to returon #stateFile or default setting if not specified
     */
    ZFMETHOD_DECLARE_0(ZFPathInfo, stateFileFixed)
    /**
     * @brief cache time to keep state, in miliseconds
     *
     * set <= 0 for infinite cache time
     */
    ZFPROPERTY_ASSIGN(zftimet, cacheTime, 7 * 24 * 60 * 60 * 1000)
    /**
     * @brief max item to keep, ordered by cache time
     */
    ZFPROPERTY_ASSIGN(zfindex, cacheSize, zfindexMax())

public:
    /**
     * @brief whether internal state has been loaded
     *
     * by default, internal state would be loaded when #set or #getAsync called,
     * use #load to explicit load
     */
    ZFMETHOD_DECLARE_0(zfbool, ready)
    /**
     * @brief load and add callback to queue which would called if #ready
     */
    ZFMETHOD_DECLARE_1(zfautoT<ZFTaskId>, load
            , ZFMP_IN(const ZFListener &, callback)
            )

    /**
     * @brief set state
     */
    ZFMETHOD_DECLARE_2(void, set
            , ZFMP_IN(const zfstring &, key)
            , ZFMP_IN(const zfstring &, value)
            )
    /**
     * @brief set state
     */
    ZFMETHOD_DECLARE_1(void, remove
            , ZFMP_IN(const zfstring &, key)
            )
    /**
     * @brief remove all state
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

    /**
     * @brief get state or null if not #ready
     */
    ZFMETHOD_DECLARE_1(zfstring, get
            , ZFMP_IN(const zfstring &, key)
            )
    /**
     * @brief util to #get with #load
     *
     * callback's param0 would be the result value if exists
     */
    ZFMETHOD_DECLARE_2(zfautoT<ZFTaskId>, getAsync
            , ZFMP_IN(const zfstring &, key)
            , ZFMP_IN(const ZFListener &, callback)
            )

    /**
     * @brief return a copy of all keys, for debug use only
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfstring>, allKey)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFStatePrivate *d;
    friend zfclassFwd _ZFP_ZFStatePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFState_h_

