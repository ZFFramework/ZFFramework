#include "ZFIOCallback.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfindex ZFIOCallbackCalcSeek(
        ZF_IN zfindex offset
        , ZF_IN zfindex length
        , ZF_IN zfindex curPos
        , ZF_IN zfindex seekByteSize
        , ZF_IN ZFSeekPos seekPos
        ) {
    switch(seekPos) {
        case ZFSeekPosBegin:
            return ((offset + seekByteSize > length) ? length : (offset + seekByteSize));
        case ZFSeekPosCur:
            return ((curPos + seekByteSize > length) ? length : curPos + seekByteSize);
        case ZFSeekPosCurReversely:
            return ((offset + seekByteSize > curPos) ? offset : (curPos - seekByteSize));
        case ZFSeekPosEnd:
            return ((offset + seekByteSize > length) ? offset : (length - seekByteSize));
        default:
            ZFCoreCriticalShouldNotGoHere();
            return zfindexMax();
    }
}

// ============================================================
zfbool ZFIOCallback::ioSeek(
        ZF_IN zfindex byteSize
        , ZF_IN_OPT ZFSeekPos pos /* = ZFSeekPosBegin */
        ) const {
    ZFObject *owner = this->callbackTag(ZFCallbackTagKeyword_ioOwner);
    if(owner == zfnull) {
        return zffalse;
    }
    const ZFMethod *method = owner->classData()->methodForName("ioSeek");
    if(method == zfnull) {
        return zffalse;
    }
    return method->executeExact<zfbool, zfindex, ZFSeekPos>(owner, byteSize, pos);
}
zfindex ZFIOCallback::ioTell(void) const {
    ZFObject *owner = this->callbackTag(ZFCallbackTagKeyword_ioOwner);
    if(owner == zfnull) {
        return zfindexMax();
    }
    const ZFMethod *method = owner->classData()->methodForName("ioTell");
    if(method == zfnull) {
        return zfindexMax();
    }
    return method->executeExact<zfindex>(owner);
}
zfindex ZFIOCallback::ioSize(void) const {
    ZFObject *owner = this->callbackTag(ZFCallbackTagKeyword_ioOwner);
    if(owner == zfnull) {
        return zfindexMax();
    }
    const ZFMethod *method = owner->classData()->methodForName("ioSize");
    if(method == zfnull) {
        return zfindexMax();
    }
    return method->executeExact<zfindex>(owner);
}

// ============================================================
ZFMETHOD_USER_REGISTER_2({
        return ZFIOCallback(invokerObject->to<v_ZFCallback *>()->zfv).ioSeek(byteSize, pos);
    }, v_ZFCallback, zfbool, ioSeek
    , ZFMP_IN(zfindex, byteSize)
    , ZFMP_IN_OPT(ZFSeekPos, pos, ZFSeekPosBegin)
    )
ZFMETHOD_USER_REGISTER_0({
        return ZFIOCallback(invokerObject->to<v_ZFCallback *>()->zfv).ioTell();
    }, v_ZFCallback, zfindex, ioTell
    )
ZFMETHOD_USER_REGISTER_0({
        return ZFIOCallback(invokerObject->to<v_ZFCallback *>()->zfv).ioSize();
    }, v_ZFCallback, zfindex, ioSize
    )
ZFMETHOD_USER_REGISTER_1({
        return ZFIOCallback(invokerObject->to<v_ZFCallback *>()->zfv).ioOwner(ioOwner);
    }, v_ZFCallback, void, ioOwner
    , ZFMP_IN(ZFObject *, ioOwner)
    )
ZFMETHOD_USER_REGISTER_0({
        return ZFIOCallback(invokerObject->to<v_ZFCallback *>()->zfv).ioOwner();
    }, v_ZFCallback, ZFObject *, ioOwner
    )

ZF_NAMESPACE_GLOBAL_END

