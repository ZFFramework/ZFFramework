/**
 * @file ZFAniBuilder.h
 * @brief util of #ZFAni to build animation quickly
 */

#ifndef _ZFI_ZFAniBuilder_h_
#define _ZFI_ZFAniBuilder_h_

#include "ZFAni.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFAniBuilderPrivate;
/**
 * @brief util of #ZFAni to build animation quickly
 *
 * usage:
 * @code
 *   ZFAniBuilder(target)
 *       .ani("prop0", from0, to0, duration, curve) // ani run in group 1
 *       .loop(2)                                   // loop 2 for group 1
 *
 *       .wait(waitTime)                            // wait before next ani group
 *
 *       .ani("prop1", from1, to1)                  // ani run in group 2
 *       .ani("prop2", from2, to2)                  // ani run in group 2
 *       .loop(2)                                   // loop 2 for group 2
 *       .step(callback)                            // commit group 2
 *
 *       .loop(2)                                   // loop 2 for group 3
 *       .customAni(aniImpl)                        // custom ani, run in group 3
 *
 *       .aniOnStart(callback)                      // attach observers
 *       .aniOnStop(callback)
 *       .aniStart(onStopOrOnInvalidCallback);
 * @endcode
 *
 * see also #ZFAni
 */
zffinal zfclassLikePOD ZFLIB_ZFUtility ZFAniBuilder {
public:
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &ani(
            ZF_IN const zfchar *name
            , ZF_IN ZFObject *from
            , ZF_IN ZFObject *to
            , ZF_IN_OPT zftimet aniDuration = 0
            , ZF_IN_OPT ZFCurve *aniCurve = zfnull
            ) const;

    /**
     * @brief see #ZFAniBuilder
     *
     * aniImpl's param0 is a #v_zffloat holds the ani progress
     */
    const ZFAniBuilder &customAni(
            ZF_IN const ZFListener &aniImpl
            , ZF_IN_OPT zftimet aniDuration = 0
            , ZF_IN_OPT ZFCurve *aniCurve = zfnull
            ) const;

public:
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &wait(ZF_IN zftimet waitTime) const;
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &step(ZF_IN_OPT const ZFListener &cb = ZFListener()) const;
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &loop(ZF_IN zfindex aniLoop) const;

public:
    /** @brief see #ZFAnimationTimeLine::aniInterval */
    const ZFAniBuilder &aniInterval(ZF_IN const zftimet &v) const;
    /** @brief see #ZFAnimationTimeLine::aniInterval */
    zftimet const &aniInterval(void) const;

public:
    /** @brief see #ZFAniBuilder */
    inline const ZFAniBuilder &aniOnInvalid(ZF_IN const ZFListener &cb) const {
        return this->aniOnEvent(ZFAnimation::EventAniOnInvalid(), cb);
    }
    /** @brief see #ZFAniBuilder */
    inline const ZFAniBuilder &aniOnStart(ZF_IN const ZFListener &cb) const {
        return this->aniOnEvent(ZFAnimation::EventAniOnStart(), cb);
    }
    /** @brief see #ZFAniBuilder */
    inline const ZFAniBuilder &aniOnStop(ZF_IN const ZFListener &cb) const {
        return this->aniOnEvent(ZFAnimation::EventAniOnStop(), cb);
    }
    /** @brief see #ZFAniBuilder */
    inline const ZFAniBuilder &aniOnStopOrInvalid(ZF_IN const ZFListener &cb) const {
        return this->aniOnEvent(ZFAnimation::EventAniOnStopOrInvalid(), cb);
    }
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &aniOnEvent(
            ZF_IN zfidentity aniEvent
            , ZF_IN const ZFListener &cb
            ) const;

public:
    /** @brief get the impl animation */
    const ZFAniBuilder &aniStart(void) const;
    /** @brief get the impl animation */
    const ZFAniBuilder &aniStart(ZF_IN const ZFListener &onStopOrOnInvalidCallback) const;

    /** @brief get the aniTarget */
    zfany aniTarget(void) const;
    /** @brief change the aniTarget */
    const ZFAniBuilder &aniTarget(ZF_IN zfany aniTarget) const;
    /** @brief get the impl animation */
    zfautoT<ZFAnimation> toAnimation(void) const;

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief construct with ani target */
    ZFAniBuilder(ZF_IN_OPT ZFObject *aniTarget = zfnull);
    /** @cond ZFPrivateDoc */
    ZFAniBuilder(ZF_IN const ZFAniBuilder &ref);
    ~ZFAniBuilder(void);
    ZFAniBuilder &operator = (ZF_IN const ZFAniBuilder &ref);
    zfbool operator == (ZF_IN const ZFAniBuilder &ref) const {return d == ref.d;}
    zfbool operator != (ZF_IN const ZFAniBuilder &ref) const {return d != ref.d;}
    /** @endcond */

private:
    _ZFP_ZFAniBuilderPrivate *d;
};

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFUtility, ZFAniBuilder, ZFAniBuilder)
ZFOUTPUT_TYPE(ZFAniBuilder, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAniBuilder_h_

