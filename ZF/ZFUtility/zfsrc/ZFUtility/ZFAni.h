/**
 * @file ZFAni.h
 * @brief animation util based on timer
 */

#ifndef _ZFI_ZFAni_h_
#define _ZFI_ZFAni_h_

#include "ZFAniForTimer.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util to start a #ZFAniForTimer based animation quickly
 *
 * usage:
 * @code
 *   ZFLISTENER(aniImpl) {
 *       ZFAnimation *ani = zfargs.sender();
 *       zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
 *       yourAniImpl(progress, ani);
 *   } ZFLISTENER_END()
 *   ZFAni(aniImpl)->target(target)->start();
 * @endcode
 * aniImpl's param0 is #v_zffloat that holds progress
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUtility, zfautoT<ZFAniForTimer>, ZFAni
        , ZFMP_IN(const ZFListener &, aniImpl)
        )

// ============================================================
/**
 * @brief util to start property animation based on #ZFAniForTimer
 *
 * usage:
 * @code
 *   ZFAni("yourProp", from, to)->target(target)->start();
 * @endcode
 * -  yourProp can be:
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
 *   -  v_zfstring or ZFDI_WrapperBase that can be serialized by #ZFSerializable::serializeFromString
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUtility, zfautoT<ZFAniForTimer>, ZFAni
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(ZFObject *, from)
        , ZFMP_IN(ZFObject *, to)
        )

// ============================================================
// utils for impl
/** @brief util for custom ani */
zfclass ZFLIB_ZFUtility ZFAniForImpl : zfextend ZFAniForTimer {
    ZFOBJECT_DECLARE(ZFAniForImpl, ZFAniForTimer)
public:
    /**
     * @brief the custom ani callback
     *
     * sender is the #ZFAniForImpl,
     * param0 is #v_zffloat holds progress of the ani
     */
    ZFPROPERTY_ASSIGN(ZFListener, aniImpl)
protected:
    zfoverride
    virtual void aniTimerOnUpdate(ZF_IN zffloat progress) {
        if(this->aniImpl()) {
            this->_ZFP_progressHolder->zfv = progress;
            this->aniImpl().execute(ZFArgs()
                    .sender(this)
                    .param0(this->_ZFP_progressHolder)
                );
        }
    }
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->_ZFP_progressHolder = zfobjAlloc(v_zffloat);
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        zfobjRetainChange(this->_ZFP_progressHolder, zfnull);
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
        if(anotherObj != zfnull && anotherObj->classData()->classIsTypeOf(zfself::ClassData())
                && ZFPropertyAllEqual(this, anotherObj)
                ) {
            return ZFCompareEqual;
        }
        return ZFCompareUncomparable;
    }
private:
    v_zffloat *_ZFP_progressHolder;
};

zfclassFwd _ZFP_ZFAniForGenericPrivate;
/** @brief util for #ZFAni */
zfclass ZFLIB_ZFUtility ZFAniForGeneric : zfextend ZFAniForTimer {
    ZFOBJECT_DECLARE(ZFAniForGeneric, ZFAniForTimer)
public:
    /** @brief name of property to animate, see #ZFAni */
    ZFPROPERTY_ASSIGN(zfstring, name)
    /** @brief from value of ani, see #ZFAni */
    ZFPROPERTY_RETAIN(zfany, fromValue)
    /** @brief to value of ani, see #ZFAni */
    ZFPROPERTY_RETAIN(zfany, toValue)

    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, name)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfany, fromValue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfany, toValue)

    /** @brief whether auto restore to prev state, true by default */
    ZFPROPERTY_ASSIGN(zfbool, autoRestore, zftrue)

protected:
    zfoverride
    virtual void aniImplTargetOnUpdate(ZF_IN ZFObject *targetOld);
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);
    zfoverride
    virtual void aniTimerOnUpdate(ZF_IN zffloat progress);
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
        if(anotherObj != zfnull && anotherObj->classData()->classIsTypeOf(zfself::ClassData())
                && ZFPropertyAllEqual(this, anotherObj)
                ) {
            return ZFCompareEqual;
        }
        return ZFCompareUncomparable;
    }
private:
    _ZFP_ZFAniForGenericPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAni_h_

