#include "ZFIOCallback_input.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFInputDummy
static zfindex _ZFP_ZFInputDummy(
        ZF_OUT void *buf
        , ZF_IN zfindex count
        ) {
    return 0;
}
ZFInput ZFInputDummy(void) {
    ZFInput ret = ZFCallbackForFunc(_ZFP_ZFInputDummy);
    ret.callbackSerializeType(ZFCallbackSerializeType_ZFInputDummy);
    ret.callbackSerializeData(ZFSerializableData());
    return ret;
}

// ============================================================
// ZFInputForInputInRange
zfclass _ZFP_I_ZFInputForInputInRangeOwner : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFInputForInputInRangeOwner, ZFObject)
public:
    ZFInput src;
    zfindex srcStart;
    zfindex srcCount;
    zfbool autoRestorePos;
    zfindex savedPos;
    zfindex curPos; // ensured init with start

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
        cache->src = zfnull;
    })
private:
    void _cleanup(void) {
        if(autoRestorePos) {
            autoRestorePos = zffalse;
            src.ioSeek(savedPos);
        }
    }

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->_cleanup();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            )
    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, pos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, onInput
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(buf == zfnull) {
        return src.execute(buf, count);
    }
    if(curPos - srcStart + count > srcCount) {
        count = srcCount - curPos + srcStart;
    }
    count = src.execute(buf, count);
    curPos += count;
    return count;
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForInputInRangeOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, pos)
        ) {
    curPos = ZFIOCallbackCalcFSeek(srcStart, srcCount, curPos, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, ioTell) {
    return curPos - srcStart;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, ioSize) {
    return srcCount;
}
ZFInput ZFInputForInputInRange(
        ZF_IN const ZFInput &inputCallback
        , ZF_IN_OPT zfindex start /* = 0 */
        , ZF_IN_OPT zfindex count /* = zfindexMax() */
        , ZF_IN_OPT zfbool autoRestorePos /* = zftrue */
        ) {
    zfbool valid = zffalse;
    zfindex savedPos = zfindexMax();
    zfindex countFixed = count;
    do {
        if(!inputCallback) {break;}

        savedPos = inputCallback.ioTell();
        if(savedPos == zfindexMax()) {break;}

        if(!inputCallback.ioSeek(start, ZFSeekPosBegin)) {break;}

        zfindex srcCount = inputCallback.ioSize();
        if(srcCount == zfindexMax()) {break;}
        if(start + countFixed > srcCount) {
            countFixed = srcCount - start;
        }

        valid = zftrue;
    } while(zffalse);
    if(!valid) {
        if(savedPos != zfindexMax()) {
            inputCallback.ioSeek(savedPos, ZFSeekPosBegin);
        }
        return zfnull;
    }

    _ZFP_I_ZFInputForInputInRangeOwner *owner = zfAlloc(_ZFP_I_ZFInputForInputInRangeOwner);
    owner->src = inputCallback;
    owner->srcStart = start;
    owner->srcCount = countFixed;
    owner->autoRestorePos = autoRestorePos;
    owner->savedPos = savedPos;
    owner->curPos = start;
    ZFInput ret = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFInputForInputInRangeOwner, onInput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);

    if(inputCallback.callbackId() != zfnull) {
        ret.callbackId(zfstr("ZFInputForInputInRange[%s, %s]:%s", start, count, inputCallback.callbackId()));
    }

    if(!inputCallback.callbackSerializeDisable()) {
        ZFSerializableData inputData;
        if(ZFCallbackToDataT(inputData, inputCallback)) {
            ZFSerializableData customData;
            inputData.category(ZFSerializableKeyword_ZFInputForInputInRange_input);
            customData.child(inputData);

            if(start != 0) {
                customData.attr(ZFSerializableKeyword_ZFInputForInputInRange_start, zfindexToString(start));
            }
            if(count != zfindexMax()) {
                customData.attr(ZFSerializableKeyword_ZFInputForInputInRange_count, zfindexToString(count));
            }
            if(!autoRestorePos) {
                customData.attr(ZFSerializableKeyword_ZFInputForInputInRange_autoRestorePos, zfboolToString(autoRestorePos));
            }

            ret.callbackSerializeType(ZFCallbackSerializeType_ZFInputForInputInRange);
            ret.callbackSerializeData(customData);
        }
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFInputForInputInRange, ZFCallbackSerializeType_ZFInputForInputInRange) {
    ZFCallback input;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFInputForInputInRange_input, ZFCallback, input, {
                return zffalse;
            });

    zfindex start = 0;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos, check,
            ZFSerializableKeyword_ZFInputForInputInRange_start, zfindex, start, {
                return zffalse;
            });

    zfindex count = zfindexMax();
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos, check,
            ZFSerializableKeyword_ZFInputForInputInRange_count, zfindex, count, {
                return zffalse;
            });

    zfbool autoRestorePos = zftrue;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos, check,
            ZFSerializableKeyword_ZFInputForInputInRange_autoRestorePos, zfbool, autoRestorePos, {
                return zffalse;
            });

    serializableData.resolveMark();
    ret = ZFInputForInputInRange(input, start, count, autoRestorePos);
    return zftrue;
}

// ============================================================
// ZFInputForBuffer serialization
ZFCALLBACK_SERIALIZE_TYPE_DEFINE(ZFInputForBuffer, ZFCallbackSerializeType_ZFInputForBuffer) {
    zfstring buf = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFInputForBuffer_buf);
    if(buf == zfnull) {
        ret = ZFInputForBufferUnsafe(zfnull, 0, zftrue);
    }
    else {
        zfstring bufDecoded;
        ZFCoreDataDecode(bufDecoded, buf);
        ret = ZFInputForBuffer(bufDecoded.cString(), bufDecoded.length());
    }
    return zftrue;
}
static void _ZFP_ZFInputForBuffer_serialize(
        ZF_IN_OUT ZFInput &ret
        , ZF_IN const void *src
        , ZF_IN zfindex srcLen
        ) {
    zfstring bufEncoded;
    ZFCoreDataEncode(bufEncoded, (const zfchar *)src, srcLen);
    ZFSerializableData customData;
    customData.attr(ZFSerializableKeyword_ZFInputForBuffer_buf, bufEncoded);
    ret.callbackSerializeType(ZFCallbackSerializeType_ZFInputForBuffer);
    ret.callbackSerializeData(customData);
}

// ============================================================
// ZFInputForBufferUnsafe
zfclass _ZFP_I_ZFInputForBufferUnsafeOwner : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFInputForBufferUnsafeOwner, ZFObject)

public:
    const zfbyte *pStart;
    const zfbyte *pEnd; // ensured valid
    const zfbyte *p;

    ZFALLOC_CACHE_RELEASE({
        cache->pStart = zfnull;
        cache->pEnd = zfnull;
        cache->p = zfnull;
    })

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            )
    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, pos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
ZFOBJECT_REGISTER(_ZFP_I_ZFInputForBufferUnsafeOwner)
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, onInput
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(buf == zfnull) {
        return pEnd - p;
    }
    if(p + count > pEnd) {
        count = pEnd - p;
    }
    zfmemcpy(buf, p, count);
    p += count;
    return count;
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForBufferUnsafeOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, pos)
        ) {
    p = pStart + ZFIOCallbackCalcFSeek(0, pEnd - pStart, p - pStart, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, ioTell) {
    return p - pStart;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, ioSize) {
    return pEnd - pStart;
}
static ZFInput _ZFP_ZFInputForBuffer(
        ZF_IN zfbool copy
        , ZF_IN const void *src
        , ZF_IN_OPT zfindex count /* = zfindexMax() */
        , ZF_IN_OPT zfbool serializable /* = zffalse */
        ) {
    if(src == zfnull) {
        return zfnull;
    }
    if(count == zfindexMax()) {
        count = zfslen((const zfchar *)src);
    }
    if(copy) {
        zfobj<_ZFP_I_ZFInputForBufferUnsafeOwner> owner;
        zfobj<v_zfstring> buf;
        buf->zfv.assign((const zfchar *)src, count);
        owner->pStart = (const zfbyte *)buf->zfv.buffer();
        owner->pEnd = owner->pStart + buf->zfv.length();
        owner->p = owner->pStart;
        ZFInput ret = ZFCallbackForMemberMethod(
            owner, ZFMethodAccess(_ZFP_I_ZFInputForBufferUnsafeOwner, onInput));
        ret.callbackTag("ZFInputForBufferCopiedBuffer", buf);
        ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
        if(serializable) {
            _ZFP_ZFInputForBuffer_serialize(ret, src, count);
        }
        return ret;
    }
    else {
        zfobj<_ZFP_I_ZFInputForBufferUnsafeOwner> owner;
        owner->pStart = (const zfbyte *)src;
        owner->pEnd = owner->pStart + count;
        owner->p = owner->pStart;
        ZFInput ret = ZFCallbackForMemberMethod(
            owner, ZFMethodAccess(_ZFP_I_ZFInputForBufferUnsafeOwner, onInput));
        ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
        if(serializable) {
            _ZFP_ZFInputForBuffer_serialize(ret, src, count);
        }
        return ret;
    }
}
ZFInput ZFInputForBufferUnsafe(
        ZF_IN const void *src
        , ZF_IN_OPT zfindex count /* = zfindexMax() */
        , ZF_IN_OPT zfbool serializable /* = zffalse */
        ) {
    return _ZFP_ZFInputForBuffer(zffalse, src, count, serializable);
}
ZFInput ZFInputForBuffer(
        ZF_IN const void *src
        , ZF_IN_OPT zfindex count /* = zfindexMax() */
        , ZF_IN_OPT zfbool serializable /* = zffalse */
        ) {
    return _ZFP_ZFInputForBuffer(zftrue, src, count, serializable);
}
ZFInput ZFInputForString(
        ZF_IN const zfstring &src
        , ZF_IN_OPT zfbool serializable /* = zffalse */
        ) {
    zfobj<_ZFP_I_ZFInputForBufferUnsafeOwner> owner;
    zfobj<v_zfstring> buf(src);
    owner->pStart = (const zfbyte *)buf->zfv.cString();
    owner->pEnd = owner->pStart + buf->zfv.length();
    owner->p = owner->pStart;
    ZFInput ret = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFInputForBufferUnsafeOwner, onInput));
    ret.callbackTag("ZFInputForBufferCopiedBuffer", buf);
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
    if(serializable) {
        _ZFP_ZFInputForBuffer_serialize(ret, src, src.length());
    }
    return ret;
}

// ============================================================
ZFMETHOD_USER_REGISTER_3({
        v_ZFCallback *owner = invokerObject;
        ZFInput input = owner->zfv;
        input.input(buf, count, result);
        return owner;
    }, v_ZFCallback, v_ZFCallback *, input
    , ZFMP_OUT(zfstring &, buf)
    , ZFMP_IN(zfindex, count)
    , ZFMP_OUT_OPT(zfindex *, result, zfnull)
    )
ZFMETHOD_USER_REGISTER_3({
        v_ZFCallback *owner = invokerObject;
        ZFInput input = owner->zfv;
        input.input(buf, count, result);
        return owner;
    }, v_ZFCallback, v_ZFCallback *, input
    , ZFMP_OUT(void *, buf)
    , ZFMP_IN(zfindex, count)
    , ZFMP_OUT_OPT(zfindex *, result, zfnull)
    )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFInput, ZFInputDummy)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(ZFInput, ZFInputForInputInRange
        , ZFMP_IN(const ZFInput &, inputCallback)
        , ZFMP_IN_OPT(zfindex, start, 0)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        , ZFMP_IN_OPT(zfbool, autoRestorePos, zftrue)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFInput, ZFInputForBufferUnsafe
        , ZFMP_IN(const zfchar *, buf)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        , ZFMP_IN_OPT(zfbool, serializable, zffalse)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFInput, ZFInputForBuffer
        , ZFMP_IN(const zfchar *, buf)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        , ZFMP_IN_OPT(zfbool, serializable, zffalse)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFInput, ZFInputForString
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN_OPT(zfbool, serializable, zffalse)
        )

ZF_NAMESPACE_GLOBAL_END

