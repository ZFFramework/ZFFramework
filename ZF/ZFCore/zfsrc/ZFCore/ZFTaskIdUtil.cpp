#include "ZFTaskIdUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_zftaskidPrivate
{
public:
    zfuint refCount;
    zfidentity idValue;
public:
    _ZFP_zftaskidPrivate(void)
    : refCount(1)
    , idValue(zfidentityInvalid())
    {
    }
};

zftaskid::zftaskid(void)
: d(zfnull)
{
}
zftaskid::zftaskid(ZF_IN zftaskid const &ref)
: d(ref.d)
{
    if(d)
    {
        ++(d->refCount);
    }
}
zftaskid &zftaskid::operator = (ZF_IN zftaskid const &ref)
{
    _ZFP_zftaskidPrivate *dTmp = d;
    d = ref.d;
    if(d)
    {
        ++(d->refCount);
    }
    if(dTmp)
    {
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            zfdelete(dTmp);
        }
    }
    return *this;
}
zfbool zftaskid::operator == (ZF_IN zftaskid const &ref) const
{
    return (d == ref.d || (d == zfnull && ref.d == zfnull));
}
void zftaskid::idValue(ZF_IN zfidentity v)
{
    if(v == zfidentityInvalid())
    {
        if(d != zfnull)
        {
            _ZFP_zftaskidPrivate *dTmp = d;
            d = zfnull;
            --(dTmp->refCount);
            if(dTmp->refCount == 0)
            {
                zfdelete(dTmp);
            }
        }
    }
    else
    {
        if(d == zfnull)
        {
            d = zfnew(_ZFP_zftaskidPrivate);
            d->idValue = v;
        }
        else
        {
            d->idValue = v;
        }
    }
}
zfidentity zftaskid::idValue(void) const
{
    return d ? d->idValue : zfidentityInvalid();
}
zfbool zftaskid::idValid(void) const
{
    return d != zfnull;
}

ZFTYPEID_ACCESS_ONLY_DEFINE(zftaskid, zftaskid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zftaskid, void, idValue, ZFMP_IN(zfidentity, v))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zftaskid, zfidentity, idValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zftaskid, zfbool, idValid)

ZFEXPORT_VAR_READONLY_DEFINE(zftaskid, zftaskidInvalid, zftaskid())

// ============================================================
zftaskid ZFTaskIdGenerator::idAcquire(void)
{
    zftaskid ret;
    ret.idValue(d.idAcquire());
    return ret;
}
zfbool ZFTaskIdGenerator::idRelease(ZF_IN_OUT zftaskid &taskid)
{
    zfidentity idValue = taskid.idValue();
    taskid.idValue(zfidentityInvalid());
    return d.idRelease(idValue);
}

zfbool ZFTaskIdGenerator::idExist(ZF_IN zftaskid const &taskid) const
{
    return d.idExist(taskid.idValue());
}

void ZFTaskIdGenerator::idExistGetAll(ZF_IN_OUT ZFCoreArray<zftaskid> &ret) const
{
    ZFCoreArrayPOD<zfidentity> tmp;
    d.idExistGetAll(tmp);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        zftaskid taskid;
        taskid.idValue(tmp[i]);
        ret.add(taskid);
    }
}

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFTaskIdGenerator, ZFTaskIdGenerator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTaskIdGenerator, zftaskid, idAcquire)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTaskIdGenerator, zfbool, idRelease, ZFMP_IN_OUT(zftaskid &, taskid))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTaskIdGenerator, zfbool, idExist, ZFMP_IN(zftaskid const &, taskid))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTaskIdGenerator, void, idExistGetAll, ZFMP_IN_OUT(ZFCoreArray<zftaskid> &, ret))

ZF_NAMESPACE_GLOBAL_END

