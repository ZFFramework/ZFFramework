/**
 * @file ZFAni.h
 * @brief animation util based on time line
 */

#ifndef _ZFI_ZFAni_h_
#define _ZFI_ZFAni_h_

#include "ZFAnimationTimeLine.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util to start a #ZFAnimationTimeLine based animation quickly
 *
 * usage:
 * @code
 *   ZFLISTENER(aniImpl) {
 *       zffloat progress = zfargs.param0()->to<v_zffloat *>()->zfv;
 *       yourAniImpl(progress, zfargs.sender());
 *   } ZFLISTENER_END()
 *   ZFAni(target, aniImpl)->aniStart();
 * @endcode
 * aniImpl's param0 is #v_zffloat that holds progress
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUtility, zfautoT<ZFAnimationTimeLine *>, ZFAni
        , ZFMP_IN(ZFObject *, target)
        , ZFMP_IN(const ZFListener &, aniImpl)
        )

// ============================================================
/**
 * @brief util to start property animation based on #ZFAnimationTimeLine
 *
 * usage:
 * @code
 *   ZFAni(target, "yourProp", from, to)->aniStart();
 * @endcode
 * -  yourPropName can be:
 *   -  name of #ZFProperty
 *   -  name of #ZFMethod,
 *     but target must have proper setter and getter method
 *     similar to #ZFProperty
 * -  from and to value can be:
 *   -  proper #ZFObject value for retain property,
 *     and the property value should be #ZFProgressable
 *   -  proper #ZFTypeIdWrapper for assign property,
 *     and the type of the property must be registered by
 *     #ZFTYPEID_PROGRESS_DEFINE
 *   -  string that can be serialized by #ZFSerializable::serializeFromString
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUtility, zfautoT<ZFAnimationTimeLine *>, ZFAni
        , ZFMP_IN(ZFObject *, target)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(const zfchar *, from)
        , ZFMP_IN(const zfchar *, to)
        )
/** @brief see #ZFAni */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUtility, zfautoT<ZFAnimationTimeLine *>, ZFAni
        , ZFMP_IN(ZFObject *, target)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(ZFObject *, from)
        , ZFMP_IN(ZFObject *, to)
        )

// ============================================================
// utils for impl
/** @brief util for custom ani */
zfclass ZFLIB_ZFUtility ZFAniForCustomAni : zfextend ZFAnimationTimeLine {
    ZFOBJECT_DECLARE(ZFAniForCustomAni, ZFAnimationTimeLine)
public:
    /**
     * @brief the custom ani callback
     *
     * sender is the #ZFAniForCustomAni,
     * param0 is #v_zffloat holds progress of the ani
     */
    ZFPROPERTY_ASSIGN(ZFListener, customAniCallback)
protected:
    zfoverride
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress) {
        if(this->customAniCallback()) {
            this->_ZFP_progressHolder->zfv = progress;
            this->customAniCallback().execute(ZFArgs()
                    .sender(this)
                    .param0(this->_ZFP_progressHolder)
                    .userData(this->customAniCallback().userData())
                );
        }
    }
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->_ZFP_progressHolder = zfAlloc(v_zffloat);
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        zfRetainChange(this->_ZFP_progressHolder, zfnull);
        zfsuper::objectOnDealloc();
    }
private:
    v_zffloat *_ZFP_progressHolder;
};

zfclassFwd _ZFP_ZFAniForGenericPrivate;
/** @brief util for #ZFAni */
zfclass ZFLIB_ZFUtility ZFAniForObject : zfextend ZFAnimationTimeLine {
    ZFOBJECT_DECLARE(ZFAniForObject, ZFAnimationTimeLine)
public:
    /** @brief name of property to animate, see #ZFAni */
    ZFPROPERTY_ASSIGN(zfstring, name)
    /** @brief from value of ani, see #ZFAni */
    ZFPROPERTY_RETAIN(ZFObject *, fromValue)
    /** @brief to value of ani, see #ZFAni */
    ZFPROPERTY_RETAIN(ZFObject *, toValue)

    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, name)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFObject *, fromValue)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFObject *, toValue)
protected:
    zfoverride
    virtual void aniImplTargetOnChange(ZF_IN ZFObject *aniTargetOld);
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniOnStopOrInvalid(ZF_IN zfbool aniValid);
    zfoverride
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress);
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
private:
    _ZFP_ZFAniForGenericPrivate *d;
};

/** @brief util for #ZFAni */
zfclass ZFLIB_ZFUtility ZFAniForGeneric : zfextend ZFAnimationTimeLine {
    ZFOBJECT_DECLARE(ZFAniForGeneric, ZFAnimationTimeLine)
public:
    /** @brief name of property to animate, see #ZFAni */
    ZFPROPERTY_ASSIGN(zfstring, name)
    /** @brief from value of ani, see #ZFAni */
    ZFPROPERTY_ASSIGN(zfstring, fromValue)
    /** @brief to value of ani, see #ZFAni */
    ZFPROPERTY_ASSIGN(zfstring, toValue)

    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, name)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, fromValue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, toValue)
protected:
    zfoverride
    virtual void aniImplTargetOnChange(ZF_IN ZFObject *aniTargetOld);
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniOnStopOrInvalid(ZF_IN zfbool aniValid);
    zfoverride
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress);
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
private:
    _ZFP_ZFAniForGenericPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAni_h_

