#include "ZFMethodFuncDeclare.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlvector.h"
#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef zfstlhashmap<ZFSigName, zfstlvector<const ZFMethod *>, ZFSigName_zfstlHash, ZFSigName_zfstlEqual> _ZFP_ZFMethodFuncNameMapType;
typedef zfstlhashmap<ZFSigName, _ZFP_ZFMethodFuncNameMapType, ZFSigName_zfstlHash, ZFSigName_zfstlEqual> _ZFP_ZFMethodFuncMapType;
static _ZFP_ZFMethodFuncMapType &_ZFP_ZFMethodFuncMap(void) {
     static _ZFP_ZFMethodFuncMapType m;
     return m;
}

void _ZFP_ZFMethodFuncRegister(ZF_IN const ZFMethod *method) {
    _ZFP_ZFMethodFuncMapType &m = _ZFP_ZFMethodFuncMap();
    m[method->methodNamespace()][method->methodName()].push_back(method);
}
void _ZFP_ZFMethodFuncUnregister(ZF_IN const ZFMethod *method) {
    _ZFP_ZFMethodFuncMapType &m = _ZFP_ZFMethodFuncMap();
    _ZFP_ZFMethodFuncMapType::iterator itNS = m.find(method->methodNamespace());
    if(itNS == m.end()) {
        return;
    }
    _ZFP_ZFMethodFuncNameMapType::iterator itName = itNS->second.find(method->methodName());
    if(itName == itNS->second.end()) {
        return;
    }
    zfstlvector<const ZFMethod *> &l = itName->second;
    for(zfstlsize i = 0; i < l.size(); ++i) {
        if(l[i] == method) {
            l.erase(l.begin() + i);
            if(l.empty()) {
                itNS->second.erase(itName);
                if(itNS->second.empty()) {
                    m.erase(itNS);
                }
                break;
            }
        }
    }
}

// ============================================================
const ZFMethod *ZFMethodFuncForName(
        ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethodFuncMapType &m = _ZFP_ZFMethodFuncMap();
    const zfchar *methodNamespaceTmp = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespaceTmp == zfnull) {
        methodNamespaceTmp = "";
    }
    _ZFP_ZFMethodFuncMapType::iterator itNS = m.find(methodNamespaceTmp);
    if(itNS == m.end()) {
        return zfnull;
    }
    _ZFP_ZFMethodFuncNameMapType::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end()) {
        return zfnull;
    }
    else {
        zfstlvector<const ZFMethod *> const &l = itName->second;
        if(l.size() > 1) {
            for(zfstlsize i = 0; i < l.size(); ++i) {
                if(l[i]->paramCountMin() == 0) {
                    return l[i];
                }
            }
        }
        return l[0];
    }
}
const ZFMethod *ZFMethodFuncForName(
        ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN_OPT const zfchar *paramTypeId0
        , ZF_IN_OPT const zfchar *paramTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId7 /* = zfnull */
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethodFuncMapType &m = _ZFP_ZFMethodFuncMap();
    const zfchar *methodNamespaceTmp = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespaceTmp == zfnull) {
        methodNamespaceTmp = "";
    }
    _ZFP_ZFMethodFuncMapType::iterator itNS = m.find(methodNamespaceTmp);
    if(itNS == m.end()) {
        return zfnull;
    }
    _ZFP_ZFMethodFuncNameMapType::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end()) {
        return zfnull;
    }

    zfstlvector<const ZFMethod *> &l = itName->second;
    for(zfstlsize i = 0; i < l.size(); ++i) {
        const ZFMethod *m = l[i];
        if(m->paramTypeIdIsMatch(
                  paramTypeId0
                , paramTypeId1
                , paramTypeId2
                , paramTypeId3
                , paramTypeId4
                , paramTypeId5
                , paramTypeId6
                , paramTypeId7
                )) {
            return m;
        }
    }
    return zfnull;
}

void ZFMethodFuncForNameGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethodFuncMapType &m = _ZFP_ZFMethodFuncMap();
    const zfchar *methodNamespaceTmp = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespaceTmp == zfnull) {
        methodNamespaceTmp = "";
    }
    _ZFP_ZFMethodFuncMapType::iterator itNS = m.find(methodNamespaceTmp);
    if(itNS == m.end()) {
        return;
    }
    _ZFP_ZFMethodFuncNameMapType::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end()) {
        return;
    }

    zfstlvector<const ZFMethod *> &l = itName->second;
    for(zfstlsize i = 0; i < l.size(); ++i) {
        ret.add(l[i]);
    }
}

void ZFMethodFuncGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethodFuncMapType &m = _ZFP_ZFMethodFuncMap();
    for(_ZFP_ZFMethodFuncMapType::iterator itNS = m.begin(); itNS != m.end(); ++itNS) {
        for(_ZFP_ZFMethodFuncNameMapType::iterator itName = itNS->second.begin(); itName != itNS->second.end(); ++itName) {
            for(zfstlsize i = 0; i < itName->second.size(); ++i) {
                ret.add(itName->second[i]);
            }
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodFuncForName
        , ZFMP_IN(const zfstring &, methodNamespace)
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodFuncForName
        , ZFMP_IN(const zfstring &, methodNamespace)
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(const zfchar *, paramTypeId0)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId1, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId2, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId3, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId4, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId5, zfnull)
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(const zfchar *, paramTypeId6, zfnull)
        // , ZFMP_IN_OPT(const zfchar *, paramTypeId7, zfnull)
    )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFMethodFuncForNameGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret)
        , ZFMP_IN(const zfstring &, methodNamespace)
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArray<const ZFMethod *>, ZFMethodFuncForNameGetAll
        , ZFMP_IN(const zfstring &, methodNamespace)
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFMethodFuncGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCoreArray<const ZFMethod *>, ZFMethodFuncGetAll
        )

ZF_NAMESPACE_GLOBAL_END
#endif

