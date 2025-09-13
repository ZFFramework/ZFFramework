/**
 * @file ZFAudioCache.h
 * @brief audio player
 */

#ifndef _ZFI_ZFAudioCache_h_
#define _ZFI_ZFAudioCache_h_

#include "ZFAudio.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief play audio with cache logic, auto stop prev audio task with same src
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfautoT<ZFAudio>, ZFAudioPlay
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        )
/**
 * @brief play audio with cache logic, auto stop prev audio task with same src
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfautoT<ZFAudio>, ZFAudioPlay
        , ZFMP_IN(const zfstring &, url)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        )

// ============================================================
/**
 * @brief audio play task
 */
zfclass ZFLIB_ZFUIKit ZFAudioPlayTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFAudioPlayTask, ZFTask)

public:
    /** @brief load by impl */
    ZFMETHOD_DECLARE_1(void, load
            , ZFMP_IN(ZFAudio *, impl)
            )
    /** @brief load by input */
    ZFMETHOD_DECLARE_1(void, load
            , ZFMP_IN(const ZFInput &, input)
            )
    /** @brief load by url */
    ZFMETHOD_DECLARE_1(void, load
            , ZFMP_IN(const zfstring &, url)
            )

    /** @brief see #ZFAudio::loop */
    ZFPROPERTY_ASSIGN(zfindex, loop)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(ZFAudio *, impl)
            )
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(const ZFInput &, input)
            )
    /** @brief construct with url */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(const zfstring &, url)
            )

protected:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(void);
private:
    zfautoT<ZFAudio> _impl;
    zfobj<ZFObject> _holder;
};

// ============================================================
/**
 * @brief audio cache
 */
zfclass ZFLIB_ZFUIKit ZFAudioCache : zfextend ZFCache {
    ZFOBJECT_DECLARE(ZFAudioCache, ZFCache)
    ZFOBJECT_SINGLETON_DECLARE(ZFAudioCache, instance)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAudioCache_h_

