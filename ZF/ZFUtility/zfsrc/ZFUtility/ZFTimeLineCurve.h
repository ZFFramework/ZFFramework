/**
 * @file ZFTimeLineCurve.h
 * @brief time based event driver
 */

#ifndef _ZFI_ZFTimeLine_h_
#define _ZFI_ZFTimeLine_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief time based event driver
 *
 * you may supply a subclass to achive custom time line,
 * but must make sure it is serializable and copyable
 */
zfabstract ZFLIB_ZFUtility ZFTimeLineCurve : zfextend ZFObject, zfimplement ZFSerializable, zfimplement ZFCopyable {
    ZFOBJECT_DECLARE_ABSTRACT(ZFTimeLineCurve, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable, ZFCopyable)

public:
    /**
     * @brief calculate progress at time
     *
     * time ensured increasing from 0.0f to 1.0f (with some proper increasing step value),
     * while result have no limits\n
     * e.g. a linear time line would have same result as the time,
     * while a bounce time line may exceeds range [0, 1] as result
     */
    ZFMETHOD_DECLARE_1(zffloat, progressUpdate
            , ZFMP_IN(zffloat, time)
            )

protected:
    /**
     * @brief called by #progressUpdate to update progress
     */
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) zfpurevirtual;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLine_h_

