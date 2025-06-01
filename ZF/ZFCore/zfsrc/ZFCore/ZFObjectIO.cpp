#include "ZFObjectIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFObjectIOData {
public:
    ZFLevel level;
    _ZFP_ZFObjectIOCallback_checker checker;
    _ZFP_ZFObjectIOCallback_fromInput fromInput;
    _ZFP_ZFObjectIOCallback_toOutput toOutput;
};
static ZFCoreArray<_ZFP_ZFObjectIOData *> &_ZFP_ZFObjectIODataList(void) { // higher priority at head
    static ZFCoreArray<_ZFP_ZFObjectIOData *> d;
    return d;
}

void _ZFP_ZFObjectIORegister(
        ZF_IN const zfchar *registerSig
        , ZF_IN _ZFP_ZFObjectIOCallback_checker checker
        , ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput
        , ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput
        , ZF_IN ZFLevel level
        ) {
    ZFCoreMutexLocker();
    ZFCoreArray<_ZFP_ZFObjectIOData *> &l = _ZFP_ZFObjectIODataList();
    zfindex pos = 0;
    for(zfindex i = 0; i < l.count(); ++i) {
        _ZFP_ZFObjectIOData *item = l[i];
        if(item->checker == checker && item->fromInput == fromInput && item->toOutput == toOutput) {
            ZFCoreCriticalMessageTrim("[ZFObjectIO] \"%s\" already registered",
                registerSig);
            return;
        }
        if(level <= item->level) {
            pos = i;
        }
    }

    _ZFP_ZFObjectIOData *data = zfnew(_ZFP_ZFObjectIOData);
    data->level = level;
    data->checker = checker;
    data->fromInput = fromInput;
    data->toOutput = toOutput;
    l.add(data, pos);
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
    zfstring fileName;
    zfstring fileExt;
    {
        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(input.pathInfo().pathType());
        if(impl) {
            if(impl->callbackToFileName(fileName, input.pathInfo().pathData())) {
                ZFFileExtOfT(fileExt, fileName);
                zfstringToLowerT(fileExt);
            }
        }
    }
    for(zfindex i = 0; i < l.count(); ++i) {
        _ZFP_ZFObjectIOData *d = l[i];
        ZFCoreMutexUnlock();
        if(l[i]->checker(input.pathInfo(), fileName, fileExt)) {
            if(l[i]->fromInput(ret, input, outErrorHint)) {
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
    zfstring fileName;
    zfstring fileExt;
    {
        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(output.pathInfo().pathType());
        if(impl) {
            if(impl->callbackToFileName(fileName, output.pathInfo().pathData())) {
                ZFFileExtOfT(fileExt, fileName);
                zfstringToLowerT(fileExt);
            }
        }
    }
    for(zfindex i = 0; i < l.count(); ++i) {
        _ZFP_ZFObjectIOData *d = l[i];
        ZFCoreMutexUnlock();
        if(l[i]->checker(output.pathInfo(), fileName, fileExt)) {
            if(l[i]->toOutput(output, obj, outErrorHint)) {
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

// ============================================================
ZFSTYLE_DECODER_DEFINE(ZFStyleDecoder_ZFObjectIO, {
    if(styleKey[0] != '@') {
        return zffalse;
    }
    ZFInput input;
    input.callbackSerializeDisable(zftrue);
    return ZFInputForPathInfoT(input, ZFPathInfo(styleKey + 1))
        && ZFObjectIOLoadT(ret, input);
})

ZF_NAMESPACE_GLOBAL_END

