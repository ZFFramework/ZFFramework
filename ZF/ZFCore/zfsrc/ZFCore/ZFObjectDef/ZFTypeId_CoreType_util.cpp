#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// string conversion spec
#define _ZFP_zfflagsInvalidToken ("0x0")
zfbool zfflagsToStringT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfflags *flagList
        , ZF_IN const zfchar **nameList
        , ZF_IN zfindex listCount
        , ZF_IN zfflags const &value
        , ZF_IN_OPT zfbool includeNotConverted /* = zftrue */
        , ZF_IN_OPT zfbool exclusiveMode /* = zffalse */
        , ZF_OUT_OPT zfflags *notConverted /* = zfnull */
        , ZF_IN_OPT zfchar separatorToken /* = '|' */
        ) {
    zfindex retLenSaved = ret.length();
    zfflags flagsLeft = value;
    if(exclusiveMode) {
        for(zfindex i = 0; i < listCount; ++i) {
            if(flagList[i] && ZFBitTest(flagsLeft, flagList[i])) {
                ret += nameList[i];
                ret += separatorToken;
                ZFBitUnset(flagsLeft, flagList[i]);
            }
            if(flagsLeft == 0) {
                break;
            }
        }
    }
    else {
        for(zfindex i = 0; i < listCount; ++i) {
            if(flagList[i] && ZFBitTest(value, flagList[i])) {
                ret += nameList[i];
                ret += separatorToken;
                ZFBitUnset(flagsLeft, flagList[i]);
            }
        }
    }

    // check flagsLeft and remove tailing separatorToken from ret
    if(flagsLeft == 0) {
        if(ret.length() == retLenSaved) {
            zfindex zeroTokenIndex = zfindexMax();
            for(zfindex i = 0; i < listCount; ++i) {
                if(flagList[i] == 0) {
                    zeroTokenIndex = i;
                    break;
                }
            }
            if(zeroTokenIndex == zfindexMax()) {
                ret = _ZFP_zfflagsInvalidToken;
            }
            else {
                ret += nameList[zeroTokenIndex];
            }
        }
        else {
            ret.remove(ret.length() - 1);
        }
    }
    else if(includeNotConverted) {
        ret += "0x";
        zfsFromIntT(ret, flagsLeft,
            ZF_HINT("radix")16,
            ZF_HINT("upper case?")zftrue);
    }

    if(notConverted != zfnull) {
        *notConverted = flagsLeft;
    }

    return zftrue;
}
zfbool zfflagsFromStringT(
        ZF_OUT zfflags &ret
        , ZF_IN const zfflags *flagList
        , ZF_IN const zfchar **nameList
        , ZF_IN zfindex listCount
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_IN_OPT zfchar separatorToken /* = '|' */
        ) {
    if(src == zfnull) {
        return zffalse;
    }
    ZFCoreArray<ZFIndexRange> pos;
    zfstring separatorTokens;
    separatorTokens += separatorToken;
    if(!ZFCoreDataPairSplitString(pos, zfindexMax(), src, srcLen, separatorTokens, zfnull, zfnull, zffalse)) {
        return zffalse;
    }
    ret = 0;
    for(zfindex iSrc = 0; iSrc < pos.count(); ++iSrc) {
        zfbool recognized = zffalse;
        for(zfindex iList = 0; iList < listCount; ++iList) {
            if(zfsncmp(src + pos[iSrc].start, nameList[iList], pos[iSrc].count) == 0) {
                recognized = zftrue;
                ZFBitSet(ret, flagList[iList]);
                break;
            }
        }
        if(!recognized) {
            if(pos[iSrc].count > 2 && *(src + pos[iSrc].start) == '0' && *(src + pos[iSrc].start + 1) == 'x') {
                zfflags tmp = 0;
                if(!zfsToIntT(tmp, src + pos[iSrc].start + 2, pos[iSrc].count - 2, 16, zftrue)) {
                    return zffalse;
                }
                ZFBitSet(ret, tmp);
            }
            else {
                return zffalse;
            }
        }
    }
    return zftrue;
}

/** @cond ZFPrivateDoc */
zfbool zfstringFromDataT(
        ZF_OUT const zfchar * &v
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_zfstring(), outErrorHint, outErrorPos) == zfnull) {
        return zffalse;
    }
    zfstring valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(valueString == zfnull) {
        v = zfnull;
        return zftrue;
    }
    serializableData.resolveMark();
    v = valueString;
    return zftrue;
}
zfbool zfstringToDataT(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN const zfchar * const &v
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(v == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "null string");
        return zffalse;
    }

    serializableData.itemClass(ZFTypeId_zfstring());
    serializableData.propertyValue(v);
    return zftrue;
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

