#include "ZFObjectIO.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFObjectIOData {
public:
    _ZFP_ZFObjectIOCallback_checker checker;
    _ZFP_ZFObjectIOCallback_fromInput fromInput;
    _ZFP_ZFObjectIOCallback_toOutput toOutput;
};
static ZFCoreArray<_ZFP_ZFObjectIOData *> &_ZFP_ZFObjectIODataList(void) {
    static ZFCoreArray<_ZFP_ZFObjectIOData *> d;
    return d;
}

void _ZFP_ZFObjectIORegister(
        ZF_IN const zfchar *registerSig
        , ZF_IN _ZFP_ZFObjectIOCallback_checker checker
        , ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput
        , ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput
        ) {
    ZFCoreMutexLocker();
    ZFCoreArray<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    for(zfindex i = 0; i < l.count(); ++i) {
        _ZFP_ZFObjectIOData *item = l[i];
        if(item->checker == checker && item->fromInput == fromInput && item->toOutput == toOutput) {
            ZFCoreCriticalMessageTrim("[ZFObjectIO] \"%s\" already registered",
                registerSig);
            return;
        }
    }

    _ZFP_ZFObjectIOData *data = zfnew(_ZFP_ZFObjectIOData);
    data->checker = checker;
    data->fromInput = fromInput;
    data->toOutput = toOutput;
    l.add(data);
}
void _ZFP_ZFObjectIOUnregister(
        ZF_IN const zfchar *registerSig
        , ZF_IN _ZFP_ZFObjectIOCallback_checker checker
        , ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput
        , ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput
        ) {
    ZFCoreMutexLocker();
    ZFCoreArray<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    for(zfindex i = 0; i < l.count(); ++i) {
        _ZFP_ZFObjectIOData *item = l[i];
        if(item->checker == checker && item->fromInput == fromInput && item->toOutput == toOutput) {
            l.remove(i);
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

    ZFCoreMutexLock();
    ZFCoreArray<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    for(zfindex i = l.count() - 1; i != zfindexMax(); --i) {
        _ZFP_ZFObjectIOData *d = l[i];
        ZFCoreMutexUnlock();
        if(l[i]->checker(input.pathInfo())) {
            if(l[i]->fromInput(ret, input, outErrorHint)) {
                ZFCoreMutexLock();
                // move to tail for better search performance
                if(i + 2 < l.count()) {
                    l.removeElement(d);
                    l.add(d);
                }
                ZFCoreMutexUnlock();
                return zftrue;
            }
            else {
                zfstringAppend(outErrorHint, "\n");
            }
        }
        ZFCoreMutexLock();
    }
    ZFCoreMutexUnlock();
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

    ZFCoreMutexLock();
    ZFCoreArray<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    for(zfindex i = l.count() - 1; i != zfindexMax(); --i) {
        _ZFP_ZFObjectIOData *d = l[i];
        ZFCoreMutexUnlock();
        if(l[i]->checker(output.pathInfo())) {
            if(l[i]->toOutput(output, obj, outErrorHint)) {
                ZFCoreMutexLock();
                // move to tail for better search performance
                if(i + 2 < l.count()) {
                    l.removeElement(d);
                    l.add(d);
                }
                ZFCoreMutexUnlock();
                return zftrue;
            }
            else {
                zfstringAppend(outErrorHint, "\n");
            }
        }
        ZFCoreMutexLock();
    }
    ZFCoreMutexUnlock();
    zfstringAppend(outErrorHint, "no ZFObjectIO impl can resolve %s",
        output.pathInfo());
    return zffalse;
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

