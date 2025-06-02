/**
 * @file ZFIdentityUtil.h
 * @brief identity generator utility
 */

#ifndef _ZFI_ZFIdentityUtil_h_
#define _ZFI_ZFIdentityUtil_h_

#include "ZFCoreTypeDef_OtherType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_Element>
zfclassFwd ZFCoreArray;
zfclassFwd _ZFP_ZFIdentityGeneratorPrivate;
/**
 * @brief identity generator utility
 *
 * used to generate an unused identity
 * @warning this class is not thread-safe,
 *   you must achieve thread-safe manually,
 *   such as #ZFCoreMutexLock
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFIdentityGenerator {
public:
    /** @cond ZFPrivateDoc */
    ZFIdentityGenerator(void);
    ZFIdentityGenerator(ZF_IN ZFIdentityGenerator const &ref);
    ZFIdentityGenerator &operator = (ZF_IN ZFIdentityGenerator const &ref);
    zfbool operator == (ZF_IN ZFIdentityGenerator const &ref) const;
    inline zfbool operator != (ZF_IN ZFIdentityGenerator const &ref) const {return !this->operator == (ref);}
    ~ZFIdentityGenerator(void);
    /** @endcond */

public:
    /**
     * @brief get next identity,
     *   ensured valid and ensured unused,
     *   and auto mark it as used
     */
    zfidentity idAcquire(void);
    /**
     * @brief mark the identity unused
     */
    zfbool idRelease(ZF_IN zfidentity identity);

    /**
     * @brief true if the identity is not used
     */
    zfbool idExist(ZF_IN zfidentity identity) const;

    /**
     * @brief get all identity currently used, for debug use only
     */
    void idExistGetAll(ZF_IN_OUT ZFCoreArray<zfidentity> &ret) const;

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

private:
    _ZFP_ZFIdentityGeneratorPrivate *d;
};
ZFOUTPUT_TYPE(ZFIdentityGenerator, {v.objectInfoT(s);})

// ============================================================
/**
 * @brief calculate identity from string
 */
extern ZFLIB_ZFCore zfidentity zfidentityCalcString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        );
/**
 * @brief calculate identity from raw buffer
 */
extern ZFLIB_ZFCore zfidentity zfidentityCalcBuf(
        ZF_IN const void *src
        , ZF_IN zfindex srcLen
        );
/**
 * @brief calculate identity from POD object
 */
template<typename T_Type>
zfidentity zfidentityCalc(ZF_IN T_Type const &v) {
    return zfidentityCalcBuf(&v, sizeof(T_Type));
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFIdentityUtil_h_

