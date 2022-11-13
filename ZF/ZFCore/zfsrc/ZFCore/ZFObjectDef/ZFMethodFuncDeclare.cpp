#include "ZFMethodFuncDeclare.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFMethodFuncDataHolder)
{
}
zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > methodMap;
ZF_STATIC_INITIALIZER_END(ZFMethodFuncDataHolder)
#define _ZFP_ZFMethodFuncMethodMap ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFuncDataHolder)->methodMap

void _ZFP_ZFMethodFuncRegister(ZF_IN const ZFMethod *method)
{
    const zfchar *methodNamespace = method->methodNamespace() ? method->methodNamespace() : "";
    _ZFP_ZFMethodFuncMethodMap
        [methodNamespace]
        [method->methodName()]
        .push_back(method);
}
void _ZFP_ZFMethodFuncUnregister(ZF_IN const ZFMethod *method)
{
    const zfchar *methodNamespace = method->methodNamespace() ? method->methodNamespace() : "";
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.find(methodNamespace);
    if(itNS == m.end())
    {
        return ;
    }
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.find(method->methodName());
    if(itName == itNS->second.end())
    {
        return ;
    }
    zfstlvector<const ZFMethod *> &l = itName->second;
    for(zfstlsize i = 0; i < l.size(); ++i)
    {
        if(l[i] == method)
        {
            l.erase(l.begin() + i);
            if(l.empty())
            {
                itNS->second.erase(itName);
                if(itNS->second.empty())
                {
                    m.erase(itNS);
                }
                break;
            }
        }
    }
}

// ============================================================
const ZFMethod *ZFMethodFuncForName(ZF_IN const zfchar *methodNamespace,
                                    ZF_IN const zfchar *methodName)
{
    zfCoreMutexLocker();
    methodNamespace = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespace == zfnull)
    {
        methodNamespace = "";
    }
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.find(methodNamespace);
    if(itNS == m.end())
    {
        return zfnull;
    }
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end())
    {
        return zfnull;
    }
    else
    {
        zfstlvector<const ZFMethod *> const &l = itName->second;
        if(l.size() > 1)
        {
            for(zfstlsize i = 0; i < l.size(); ++i)
            {
                if(l[i]->methodParamCountMin() == 0)
                {
                    return l[i];
                }
            }
        }
        return l[0];
    }
}
const ZFMethod *ZFMethodFuncForName(ZF_IN const zfchar *methodNamespace,
                                    ZF_IN const zfchar *methodName
                                    , ZF_IN_OPT const zfchar *methodParamTypeId0
                                    , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                                    , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                                    , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                                    , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                                    , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                                    , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                                    , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                                    )
{
    zfCoreMutexLocker();
    methodNamespace = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespace == zfnull)
    {
        methodNamespace = "";
    }
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.find(methodNamespace);
    if(itNS == m.end())
    {
        return zfnull;
    }
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end())
    {
        return zfnull;
    }

    zfstlvector<const ZFMethod *> &l = itName->second;
    for(zfstlsize i = 0; i < l.size(); ++i)
    {
        const ZFMethod *m = l[i];
        if(m->methodParamTypeIdIsMatch(
                  methodParamTypeId0
                , methodParamTypeId1
                , methodParamTypeId2
                , methodParamTypeId3
                , methodParamTypeId4
                , methodParamTypeId5
                , methodParamTypeId6
                , methodParamTypeId7
            ))
        {
            return m;
        }
    }
    return zfnull;
}

void ZFMethodFuncForNameGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                                ZF_IN const zfchar *methodNamespace,
                                ZF_IN const zfchar *methodName)
{
    zfCoreMutexLocker();
    methodNamespace = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespace == zfnull)
    {
        methodNamespace = "";
    }
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.find(methodNamespace);
    if(itNS == m.end())
    {
        return ;
    }
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end())
    {
        return ;
    }

    zfstlvector<const ZFMethod *> &l = itName->second;
    for(zfstlsize i = 0; i < l.size(); ++i)
    {
        ret.add(l[i]);
    }
}

void ZFMethodFuncGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                         ZF_IN_OPT const ZFFilterForZFMethod *filter /* = zfnull */)
{
    zfCoreMutexLocker();

    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    for(zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.begin();
        itNS != m.end();
        ++itNS)
    {
        for(zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.begin();
            itName != itNS->second.end();
            ++itName)
        {
            if(filter == zfnull)
            {
                for(zfstlsize i = 0; i < itName->second.size(); ++i)
                {
                    ret.add(itName->second[i]);
                }
            }
            else
            {
                for(zfstlsize i = 0; i < itName->second.size(); ++i)
                {
                    if(filter->filterCheckActive(itName->second[i]))
                    {
                        ret.add(itName->second[i]);
                    }
                }
            }
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodFuncForName, ZFMP_IN(const zfchar *, methodNamespace), ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodFuncForName, ZFMP_IN(const zfchar *, methodNamespace), ZFMP_IN(const zfchar *, methodName)
        , ZFMP_IN(const zfchar *, methodParamTypeId0)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull)
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull)
        // , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull)
    )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFMethodFuncForNameGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret), ZFMP_IN(const zfchar *, methodNamespace), ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArrayPOD<const ZFMethod *>, ZFMethodFuncForNameGetAll, ZFMP_IN(const zfchar *, methodNamespace), ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFMethodFuncGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret), ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArrayPOD<const ZFMethod *>, ZFMethodFuncGetAll, ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif

