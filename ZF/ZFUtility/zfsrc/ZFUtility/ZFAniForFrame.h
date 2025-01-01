/**
 * @file ZFAniForFrame.h
 * @brief abstract animation based on user specified frame duration
 */

#ifndef _ZFI_ZFAniForFrame_h_
#define _ZFI_ZFAniForFrame_h_

#include "ZFAnimation.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFAniForFrame_frames "frames"

// ============================================================
zfclassFwd _ZFP_ZFAniForFramePrivate;
/**
 * @brief animation based on user specified frame duration
 *
 * this is a dummy animation holder which do nothing by default,
 * you should either:
 * -  supply subclass and implement actual animation by overriding
 *   #aniFrameOnUpdate
 * -  attach observer to #E_AniFrameOnUpdate
 *
 * serializable data:
 * @code
 *   <ZFAniForFrame
 *       frames="100,200,..." // optional, a list of frame durations
 *                            // when empty, animation would stop immediately
 *       />
 * @endcode
 */
zfclass ZFLIB_ZFUtility ZFAniForFrame : zfextend ZFAnimation {
    ZFOBJECT_DECLARE(ZFAniForFrame, ZFAnimation)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is a #v_zfindex containing current frame index
     */
    ZFEVENT(AniFrameOnUpdate)

    // ============================================================
    // animation setting
public:
    /** @brief add a frame */
    ZFMETHOD_DECLARE_1(void, frame
            , ZFMP_IN(zftimet, duration)
            )
    /** @brief add multiple frames */
    ZFMETHOD_DECLARE_1(void, frames
            , ZFMP_IN(const ZFCoreArray<zftimet> &, frames)
            )

    /** @brief return frame count */
    ZFMETHOD_DECLARE_0(zfindex, frameCount)
    /** @brief return frame duration at index */
    ZFMETHOD_DECLARE_1(zftimet, frameAt
            , ZFMP_IN(zfindex, index)
            )
    /** @brief remove frame at index */
    ZFMETHOD_DECLARE_1(void, frameRemoveAt
            , ZFMP_IN(zfindex, index)
            )
    /** @brief remove all frame */
    ZFMETHOD_DECLARE_0(void, frameRemoveAll)

    /**
     * @brief current frame index, or zfindexMax if stopped
     */
    ZFMETHOD_DECLARE_0(zfindex, frameIndex)

    // ============================================================
    // property
public:
    /**
     * @brief whether use global timer to update frame, true by default
     *
     * -  when true, we would use special logic to achieve global timer control:
     *   -# calculate frame count by frameDuration/#ZFGlobalTimerIntervalDefault
     *   -# step each frame by #ZFGlobalTimerInterval, until reach the frame count
     *
     *   for example, if you increace #ZFGlobalTimerInterval,
     *   the animation would looks slower\n
     *   this is useful to achieve accurate timer control
     * -  when false, each frame is update by #ZFTimerOnce
     */
    ZFPROPERTY_ASSIGN(zfbool, useGlobalTimer, zftrue)

    // ============================================================
    // object
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        if(this->frameCount() != another->frameCount()
                || this->useGlobalTimer() != another->useGlobalTimer()
                ) {
            return ZFCompareUncomparable;
        }
        for(zfindex i = 0; i < this->frameCount(); ++i) {
            if(this->frameAt(i) != another->frameAt(i)) {
                return ZFCompareUncomparable;
            }
        }
        return ZFCompareEqual;
    }

    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            );

    // ============================================================
    // start stop
public:
    zfoverride
    virtual zftimet durationFixed(void);

protected:
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);

protected:
    /**
     * @brief called to do the actual update
     *
     * this method would be called:
     * -  with 0 index when animation started
     * -  with 1~(frameCount-1) index when frame updated
     */
    virtual void aniFrameOnUpdate(ZF_IN zfindex frameIndex);

private:
    _ZFP_ZFAniForFramePrivate *d;
    friend zfclassFwd _ZFP_ZFAniForFramePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAniForFrame_h_

