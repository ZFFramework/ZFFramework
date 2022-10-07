/**
 * @file ZFTaskIdUtil.h
 * @brief taskid utility
 */

#ifndef _ZFI_ZFTaskIdUtil_h_
#define _ZFI_ZFTaskIdUtil_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_zftaskidPrivate;
/**
 * @brief taskid utility, see #ZFTaskIdGenerator
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT zftaskid
{
public:
    /** @cond ZFPrivateDoc */
    zftaskid(void);
    zftaskid(ZF_IN zftaskid const &ref);
    zftaskid &operator = (ZF_IN zftaskid const &ref);
    zfbool operator == (ZF_IN zftaskid const &ref) const;
    inline zfbool operator != (ZF_IN zftaskid const &ref) const {return !this->operator == (ref);}
    /** @endcond */
public:
    /** @brief set internal id value */
    void idValue(ZF_IN zfidentity v);
    /** @brief get internal id value */
    zfidentity idValue(void) const;
    /** @brief whether internal id value is valid */
    zfbool idValid(void) const;
private:
    _ZFP_zftaskidPrivate *d;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(zftaskid, zftaskid)

/** @brief invalid taskid */
ZFEXPORT_VAR_READONLY_DECLARE(zftaskid, zftaskidInvalid)

// ============================================================
/**
 * @brief util to manage task id
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFTaskIdGenerator
{
public:
    /**
     * @brief get next task id,
     *   ensured valid and ensured unused,
     *   and auto mark it as used
     */
    zftaskid idAcquire(void);
    /**
     * @brief release and mark the task id unused
     */
    zfbool idRelease(ZF_IN_OUT zftaskid &taskid);

    /**
     * @brief true if the task id is not used
     */
    zfbool idExist(ZF_IN zftaskid const &taskid) const;

    /**
     * @brief get all task id currently used, for debug use only
     */
    void idExistGetAll(ZF_IN_OUT ZFCoreArray<zftaskid> &ret) const;

private:
    ZFIdentityGenerator d;

public:
    /** @cond ZFPrivateDoc */
    ZFTaskIdGenerator(void) : d() {}
    ZFTaskIdGenerator(ZF_IN ZFTaskIdGenerator const &ref) : d(ref.d) {}
    ZFTaskIdGenerator &operator = (ZF_IN ZFTaskIdGenerator const &ref) {d = ref.d; return *this;}
    zfbool operator == (ZF_IN ZFTaskIdGenerator const &ref) const {return d == ref.d;}
    inline zfbool operator != (ZF_IN ZFTaskIdGenerator const &ref) const {return d != ref.d;}
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFTaskIdGenerator, ZFTaskIdGenerator)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTaskIdUtil_h_

