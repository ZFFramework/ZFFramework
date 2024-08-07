#include "ZFObjectIO.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFObjectIOData {
public:
    _ZFP_ZFObjectIOCallback_checker checker;
    _ZFP_ZFObjectIOCallback_fromInput fromInput;
    _ZFP_ZFObjectIOCallback_toOutput toOutput;
};
static zfstldeque<_ZFP_ZFObjectIOData *> &_ZFP_ZFObjectIODataList(void) {
    static zfstldeque<_ZFP_ZFObjectIOData *> d;
    return d;
}

void _ZFP_ZFObjectIORegister(
        ZF_IN const zfchar *registerSig
        , ZF_IN _ZFP_ZFObjectIOCallback_checker checker
        , ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput
        , ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput
        ) {
    zfCoreMutexLocker();
    zfstldeque<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    for(zfstlsize i = 0; i < l.size(); ++i) {
        _ZFP_ZFObjectIOData *item = l[i];
        if(item->checker == checker && item->fromInput == fromInput && item->toOutput == toOutput) {
            zfCoreCriticalMessageTrim("[ZFObjectIO] \"%s\" already registered",
                registerSig);
            return;
        }
    }

    _ZFP_ZFObjectIOData *data = zfnew(_ZFP_ZFObjectIOData);
    data->checker = checker;
    data->fromInput = fromInput;
    data->toOutput = toOutput;
    l.push_back(data);
}
void _ZFP_ZFObjectIOUnregister(
        ZF_IN const zfchar *registerSig
        , ZF_IN _ZFP_ZFObjectIOCallback_checker checker
        , ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput
        , ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput
        ) {
    zfCoreMutexLocker();
    zfstldeque<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    for(zfstlsize i = 0; i < l.size(); ++i) {
        _ZFP_ZFObjectIOData *item = l[i];
        if(item->checker == checker && item->fromInput == fromInput && item->toOutput == toOutput) {
            l.erase(l.begin() + i);
            zfdelete(item);
            break;
        }
    }
}

// ============================================================
zfbool ZFObjectIOLoadT(
        ZF_OUT zfauto &ret
        , ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(input.pathInfo() == zfnull) {
        zfstringAppend(outErrorHint, "callback %s does not have path info", input);
        return zffalse;
    }

    zfCoreMutexLock();
    zfstldeque<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    for(zfstlsize i = l.size() - 1; i != (zfstlsize)-1; --i) {
        _ZFP_ZFObjectIOData *d = l[i];
        zfCoreMutexUnlock();
        if(l[i]->checker(*input.pathInfo())) {
            if(l[i]->fromInput(ret, input, outErrorHint)) {
                zfCoreMutexLock();
                // move to tail for better search performance
                if(i != l.size() - 1) {
                    l.erase(l.begin() + i);
                    l.push_back(d);
                }
                zfCoreMutexUnlock();
                return zftrue;
            }
            else {
                zfstringAppend(outErrorHint, "\n");
            }
        }
        zfCoreMutexLock();
    }
    zfCoreMutexUnlock();
    zfstringAppend(outErrorHint, "no ZFObjectIO impl can resolve %s", input.pathInfo());
    return zffalse;
}
zfauto ZFObjectIOLoad(
        ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    zfauto ret;
    ZFObjectIOLoadT(ret, input, outErrorHint);
    return ret;
}
zfbool ZFObjectIOSave(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN ZFObject *obj
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(output.pathInfo() == zfnull) {
        zfstringAppend(outErrorHint, "callback %s does not have path info",
            output);
        return zffalse;
    }

    zfCoreMutexLock();
    zfstldeque<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    for(zfstlsize i = l.size() - 1; i != (zfstlsize)-1; --i) {
        _ZFP_ZFObjectIOData *d = l[i];
        zfCoreMutexUnlock();
        if(l[i]->checker(*output.pathInfo())) {
            if(l[i]->toOutput(output, obj, outErrorHint)) {
                zfCoreMutexLock();
                // move to tail for better search performance
                if(i != l.size() - 1) {
                    l.erase(l.begin() + i);
                    l.push_back(d);
                }
                zfCoreMutexUnlock();
                return zftrue;
            }
            else {
                zfstringAppend(outErrorHint, "\n");
            }
        }
        zfCoreMutexLock();
    }
    zfCoreMutexUnlock();
    zfstringAppend(outErrorHint, "no ZFObjectIO impl can resolve %s", 
        output.pathInfo());
    return zffalse;
}

const zfchar *ZFObjectIOImplCheckFileExt(ZF_IN const ZFPathInfo &pathInfo) {
    const zfchar *pStart = pathInfo.pathData;
    const zfchar *ret = pStart + pathInfo.pathData.length() - 1;
    while(ret > pStart && *ret != '.') {--ret;}
    if(*ret == '.') {
        return ret + 1;
    }
    else {
        return zfnull;
    }
}
zfbool ZFObjectIOImplCheck(
        ZF_IN const ZFPathInfo &pathInfo
        , ZF_IN const zfstring &desiredFileExt
        ) {
    return (pathInfo.pathData.length() >= desiredFileExt.length()
        && zfstringIsEqual(pathInfo.pathData + pathInfo.pathData.length() - desiredFileExt.length(), desiredFileExt));
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectIOLoadT
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfauto, ZFObjectIOLoad
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectIOSave
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

