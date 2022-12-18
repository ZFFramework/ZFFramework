#include "ZFIOCallback_input.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFInputDummy
static zfindex _ZFP_ZFInputDummy(ZF_OUT void *buf, ZF_IN zfindex count)
{
    return 0;
}
ZFInput ZFInputDummy(void)
{
    ZFInput ret = ZFCallbackForFunc(_ZFP_ZFInputDummy);
    ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFInputDummy);
    ret.callbackSerializeCustomData(ZFSerializableData());
    return ret;
}

// ============================================================
// ZFInputForInputInRange
zfclass _ZFP_I_ZFInputForInputInRangeOwner : zfextends ZFObject
{
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
    void _cleanup(void)
    {
        if(autoRestorePos)
        {
            autoRestorePos = zffalse;
            src.ioSeek(savedPos);
        }
    }

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->_cleanup();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput,
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, count))
    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, onInput,
                  ZFMP_IN(void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(buf == zfnull)
    {
        return src.execute(buf, count);
    }
    if(curPos - srcStart + count > srcCount)
    {
        count = srcCount - curPos + srcStart;
    }
    count = src.execute(buf, count);
    curPos += count;
    return count;
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForInputInRangeOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    curPos = ZFIOCallbackCalcFSeek(srcStart, srcCount, curPos, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, ioTell)
{
    return curPos - srcStart;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, ioSize)
{
    return srcStart + srcCount - curPos;
}
ZFInput ZFInputForInputInRange(ZF_IN const ZFInput &inputCallback,
                               ZF_IN_OPT zfindex start /* = 0 */,
                               ZF_IN_OPT zfindex count /* = zfindexMax() */,
                               ZF_IN_OPT zfbool autoRestorePos /* = zftrue */)
{
    zfbool valid = zffalse;
    zfindex savedPos = zfindexMax();
    zfindex countFixed = count;
    do
    {
        if(!inputCallback) {break;}

        savedPos = inputCallback.ioTell();
        if(savedPos == zfindexMax()) {break;}

        if(!inputCallback.ioSeek(start, ZFSeekPosBegin)) {break;}

        zfindex srcCount = inputCallback.ioSize();
        if(srcCount == zfindexMax()) {break;}
        if(start + countFixed > srcCount)
        {
            countFixed = srcCount - start;
        }

        valid = zftrue;
    } while(zffalse);
    if(!valid)
    {
        if(savedPos != zfindexMax())
        {
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

    if(inputCallback.callbackId() != zfnull)
    {
        ret.callbackId(zfstringWithFormat("ZFInputForInputInRange[%zi, %zi]:%@", start, count, inputCallback.callbackId()));
    }

    if(!inputCallback.callbackSerializeCustomDisabled())
    {
        ZFSerializableData inputData;
        if(ZFCallbackToData(inputData, inputCallback))
        {
            ZFSerializableData customData;
            customData.itemClass(ZFSerializableKeyword_node);
            inputData.category(ZFSerializableKeyword_ZFInputForInputInRange_input);
            customData.childAdd(inputData);

            if(start != 0)
            {
                customData.attr(ZFSerializableKeyword_ZFInputForInputInRange_start, zfindexToString(start));
            }
            if(count != zfindexMax())
            {
                customData.attr(ZFSerializableKeyword_ZFInputForInputInRange_count, zfindexToString(count));
            }
            if(!autoRestorePos)
            {
                customData.attr(ZFSerializableKeyword_ZFInputForInputInRange_autoRestorePos, zfboolToString(autoRestorePos));
            }

            ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFInputForInputInRange);
            ret.callbackSerializeCustomData(customData);
        }
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFInputForInputInRange, ZFCallbackSerializeCustomType_ZFInputForInputInRange)
{
    ZFCallback input;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
        require, ZFSerializableKeyword_ZFInputForInputInRange_input, ZFCallback, input);

    zfindex start = 0;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos, check,
        ZFSerializableKeyword_ZFInputForInputInRange_start, zfindex, start);

    zfindex count = zfindexMax();
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos, check,
        ZFSerializableKeyword_ZFInputForInputInRange_count, zfindex, count);

    zfbool autoRestorePos = zftrue;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos, check,
        ZFSerializableKeyword_ZFInputForInputInRange_autoRestorePos, zfbool, autoRestorePos);

    serializableData.resolveMark();
    ret = ZFInputForInputInRange(input, start, count, autoRestorePos);
    return zftrue;
}

// ============================================================
// ZFInputForBufferUnsafe
zfclass _ZFP_I_ZFInputForBufferUnsafeOwner : zfextends ZFObject
{
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
    ZFMETHOD_DECLARE_2(zfindex, onInput,
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, count))
    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, onInput,
                  ZFMP_IN(void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(buf == zfnull)
    {
        return pEnd - p;
    }
    if(p + count > pEnd)
    {
        count = pEnd - p;
    }
    zfmemcpy(buf, p, count);
    p += count;
    return count;
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForBufferUnsafeOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    p = pStart + ZFIOCallbackCalcFSeek(0, pEnd - pStart, p - pStart, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, ioTell)
{
    return p - pStart;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, ioSize)
{
    return pEnd - p;
}
static ZFInput _ZFP_ZFInputForBuffer(ZF_IN zfbool copy,
                                     ZF_IN const void *src,
                                     ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    if(src == zfnull)
    {
        return zfnull;
    }
    if(count == zfindexMax())
    {
        count = zfslen((const zfchar *)src);
    }
    if(copy)
    {
        zfblockedAlloc(_ZFP_I_ZFInputForBufferUnsafeOwner, owner);
        zfblockedAlloc(v_ZFBuffer, buf);
        buf->zfv.bufferCopy(src, count * sizeof(zfchar));
        owner->pStart = (const zfbyte *)buf->zfv.buffer();
        owner->pEnd = owner->pStart + buf->zfv.bufferSize();
        owner->p = owner->pStart;
        ZFInput ret = ZFCallbackForMemberMethod(
            owner, ZFMethodAccess(_ZFP_I_ZFInputForBufferUnsafeOwner, onInput));
        ret.callbackTag("ZFInputForBufferCopiedBuffer", buf);
        ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
        return ret;
    }
    else
    {
        zfblockedAlloc(_ZFP_I_ZFInputForBufferUnsafeOwner, owner);
        owner->pStart = (const zfbyte *)src;
        owner->pEnd = owner->pStart + count;
        owner->p = owner->pStart;
        ZFInput ret = ZFCallbackForMemberMethod(
            owner, ZFMethodAccess(_ZFP_I_ZFInputForBufferUnsafeOwner, onInput));
        ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
        return ret;
    }
}
ZFInput ZFInputForBufferUnsafe(ZF_IN const void *src,
                               ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    return _ZFP_ZFInputForBuffer(zffalse, src, count);
}
ZFInput ZFInputForBuffer(ZF_IN const void *src,
                         ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    return _ZFP_ZFInputForBuffer(zftrue, src, count);
}
ZFInput ZFInputForString(ZF_IN const zfchar *src,
                         ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    return _ZFP_ZFInputForBuffer(zftrue, src, count);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_2({
        ZFInput input = invokerObject->to<v_ZFCallback *>()->zfv;
        return input.input(buf, count);
    }, v_ZFCallback, zfindex, input, ZFMP_OUT(zfstring &, buf), ZFMP_IN(zfindex, count))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFInput, ZFInputDummy)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(ZFInput, ZFInputForInputInRange, ZFMP_IN(const ZFInput &, inputCallback), ZFMP_IN_OPT(zfindex, start, 0), ZFMP_IN_OPT(zfindex, count, zfindexMax()), ZFMP_IN_OPT(zfbool, autoRestorePos, zftrue))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFInput, ZFInputForBufferUnsafe, ZFMP_IN(const zfchar *, buf), ZFMP_IN_OPT(zfindex, count, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFInput, ZFInputForBuffer, ZFMP_IN(const zfchar *, buf), ZFMP_IN_OPT(zfindex, count, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFInput, ZFInputForString, ZFMP_IN(const zfchar *, buf), ZFMP_IN_OPT(zfindex, count, zfindexMax()))

ZF_NAMESPACE_GLOBAL_END
#endif

