#include "ZFOutputForFormat.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFOutputFormatStep)

// ============================================================
ZFOBJECT_REGISTER(ZFOutputFormat)

// ============================================================
zfclass _ZFP_I_ZFOutputForFormatOwner : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFOutputForFormatOwner, ZFObject)

public:
    ZFOutput output;
    ZFOutputFormat *format;
    ZFOutputFormatStep outputStep;

public:
    ZFOBJECT_CACHE_RELEASE({
        if(cache->format != zfnull) {
            cache->outputEnd();
        }

        cache->output = zfnull;
        if(cache->format != zfnull) {
            zfobjRelease(cache->format);
            cache->format = zfnull;
        }
        cache->outputStep = v_ZFOutputFormatStep::e_OnInit;
    })

protected:
    _ZFP_I_ZFOutputForFormatOwner(void)
    : output()
    , format(zfnull)
    , outputStep(v_ZFOutputFormatStep::e_OnInit)
    {
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        if(this->format != zfnull) {
            this->outputEnd();
            zfobjRelease(this->format);
            this->format = zfnull;
        }
        zfsuper::objectOnDealloc();
    }

public:
    void outputBegin(void) {
        zfstring buf;
        this->format->_ZFP_format(buf, v_ZFOutputFormatStep::e_OnInit, "", 0);
        if(!buf.isEmpty()) {
            this->output.execute(buf.cString(), buf.length());
        }
    }
    void outputEnd(void) {
        zfstring buf;
        this->format->_ZFP_format(buf, v_ZFOutputFormatStep::e_OnOutputEnd, "", 0);
        if(!buf.isEmpty()) {
            if(this->output) {
                this->output.execute(buf.cString(), buf.length());
            }
            buf.removeAll();
        }

        this->format->_ZFP_format(buf, v_ZFOutputFormatStep::e_OnDealloc, "", 0);
        if(!buf.isEmpty()) {
            if(this->output) {
                this->output.execute(buf.cString(), buf.length());
            }
        }
    }
    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_IN(const void *, s)
            , ZFMP_IN(zfindex, count)
            )
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForFormatOwner, zfindex, onOutput
        , ZFMP_IN(const void *, s)
        , ZFMP_IN(zfindex, count)
        ) {
    if(!this->output) {
        return 0;
    }

    if(count == zfindexMax()) {
        count = zfslen((const zfchar *)s);
    }

    zfstring buf;
    this->format->_ZFP_format(buf, v_ZFOutputFormatStep::e_OnOutput, (const zfchar *)s, count);
    if(buf.isEmpty()) {
        return count;
    }

    zfindex written = this->output.execute(buf.cString(), buf.length());
    if(written == buf.length()) {
        return count;
    }
    else {
        return 0;
    }
}


ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFOutputForFormatT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFOutput &, output)
        , ZFMP_IN(ZFOutputFormat *, format)
        ) {
    if(!output) {
        return zffalse;
    }
    if(format == zfnull) {
        ret = output;
        return zffalse;
    }

    _ZFP_I_ZFOutputForFormatOwner *outputOwner = zfobjAlloc(_ZFP_I_ZFOutputForFormatOwner);
    outputOwner->output = output;
    outputOwner->format = zfobjRetain(format);
    ret = ZFCallbackForMemberMethod(outputOwner, ZFMethodAccess(_ZFP_I_ZFOutputForFormatOwner, onOutput));
    ret.callbackOwnerObjectRetain();
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, output.callbackTag(ZFCallbackTagKeyword_ioOwner));
    zfobjRelease(outputOwner);

    if(!ret.callbackSerializeDisable()) {
        ZFSerializableData outputData;
        ZFSerializableData formatData;
        if(format->classData()->classIsTypeOf(ZFSerializable::ClassData())
                && ZFCallbackToDataT(outputData, output)
                && ZFObjectToDataT(formatData, format->toObject())
                ) {
            ZFSerializableData serializableData;
            outputData.category(ZFSerializableKeyword_ZFOutputForFormat_output);
            serializableData.child(outputData);
            formatData.category(ZFSerializableKeyword_ZFOutputForFormat_format);
            serializableData.child(formatData);
            ret.callbackSerializeType(ZFCallbackSerializeType_ZFOutputForFormat);
            ret.callbackSerializeData(serializableData);
        }
    }

    outputOwner->outputBegin();
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForFormat
        , ZFMP_IN(const ZFOutput &, output)
        , ZFMP_IN(ZFOutputFormat *, format)
        ) {
    ZFOutput ret;
    ZFOutputForFormatT(ret, output, format);
    return ret;
}

ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFOutputForFormat, ZFCallbackSerializeType_ZFOutputForFormat) {
    ZFOutput output;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFOutputForFormat_output, ZFCallback, output, {
                return zffalse;
            });

    ZFSerializableData formatData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFOutputForFormat_format, outErrorHint, outErrorPos);
    if(formatData == zfnull) {
        return zffalse;
    }
    zfauto formatHolder;
    if(!ZFObjectFromDataT(formatHolder, formatData, outErrorHint, outErrorPos)) {
        return zffalse;
    }
    ZFOutputFormat *format = formatHolder;
    if(format == zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, formatData,
            "format object %s not type of %s",
            formatHolder,
            ZFOutputFormat::ClassData()->classNameFull());
        return zffalse;
    }

    ZFOutput retTmp;
    retTmp.callbackSerializeDisable(zftrue);
    if(!ZFOutputForFormatT(retTmp, output, format)) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, formatData,
            "unable to create from output %s and format %s",
            output,
            formatHolder);
        return zffalse;
    }
    ret = retTmp;

    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFOutputFormat, zfanyT<ZFOutputFormat>, getFormat
        , ZFMP_IN(const ZFCallback &, output)
        ) {
    _ZFP_I_ZFOutputForFormatOwner *owner = output.callbackOwnerObject();
    if(owner == zfnull) {
        return zfnull;
    }
    else {
        return owner->format;
    }
}

ZFMETHOD_DEFINE_1(ZFOutputFormat, ZFOutput, getOutput
        , ZFMP_IN(const ZFCallback &, output)
        ) {
    _ZFP_I_ZFOutputForFormatOwner *owner = output.callbackOwnerObject();
    if(owner == zfnull) {
        return zfnull;
    }
    else {
        return owner->output;
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFOutputFormatBasic)
void ZFOutputFormatBasic::format(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFOutputFormatStep outputStep
        , ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        ) {
    switch(outputStep) {
        case v_ZFOutputFormatStep::e_OnInit:
            _ZFP_needLinePrefix = zftrue;
            return;
        case v_ZFOutputFormatStep::e_OnDealloc:
            return;
        case v_ZFOutputFormatStep::e_OnOutput:
            break;
        case v_ZFOutputFormatStep::e_OnOutputEnd:
            if(_ZFP_outputCount > 0) {
                ret += this->linePostfix();
                ret += this->outputPostfix();
            }
            return;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
    if(_ZFP_outputCount == 0) {
        ret += this->outputPrefix();
    }
    ++_ZFP_outputCount;
    if(_ZFP_needLinePrefix) {
        _ZFP_needLinePrefix = zffalse;
        ret += this->linePrefix();
    }

    const zfchar *srcEnd = src + srcLen;
    const zfchar *p = src;
    zfbool escapeEndl = this->escapeEndl();
    while(src < srcEnd) {
        if(escapeEndl && (*src == '\n' || *src == '\r')) {
            ret.append(p, src - p);
            if(*src == '\r') {
                ret += "\\r";
            }
            else if(*src == '\n') {
                ret += "\\n";
            }
            ++src;
            p = src;
        }
        else if(*src == '\n') {
            if(_ZFP_needLinePrefix) {
                ret += this->linePrefix();
            }
            ret.append(p, src - p);
            ret += this->linePostfix();
            ret += '\n';
            _ZFP_needLinePrefix = zftrue;
            zfcharMoveNext(src);
            p = src;
            continue;
        }
        zfcharMoveNext(src);
    }
    if(p < src) {
        _ZFP_needLinePrefix = zffalse;
        ret.append(p, src - p);
    }
}

ZF_NAMESPACE_GLOBAL_END

