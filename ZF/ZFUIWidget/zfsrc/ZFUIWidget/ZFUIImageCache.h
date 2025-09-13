/**
 * @file ZFUIImageCache.h
 * @brief load image with cache logic
 */

#ifndef _ZFI_ZFUIImageCache_h_
#define _ZFI_ZFUIImageCache_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief load image with cache logic
 *
 * callback's param0 would be the result image if success,
 * param1 would be a #ZFResultType indicates load result\n
 * use #ZFIOCache to config cache logic
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIWidget, zfautoT<ZFTaskId>, ZFUIImageLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        )

// ============================================================
/**
 * @brief io load task
 *
 * #ZFTask::result would be the result #ZFUIImage if success
 */
zfclass ZFLIB_ZFUIWidget ZFUIImageLoadTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFUIImageLoadTask, ZFTask)

public:
    /** @brief the src input */
    ZFPROPERTY_ASSIGN(ZFInput, input)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(const ZFInput &, input)
            )

protected:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(void);
private:
    zfautoT<ZFTaskId> _implTaskId;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageCache_h_

